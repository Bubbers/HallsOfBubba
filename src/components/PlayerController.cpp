//
// Created by simon on 2017-04-29.
//

#include <ControlsManager.h>
#include <controls.h>
#include <MousePosition.h>
#include <linmath/SmallVector2.h>
#include <Globals.h>
#include <ResourceManager.h>
#include <Lights.h>
#include "PlayerController.h"
#include "Camera.h"


PlayerController::PlayerController(std::function<void(std::weak_ptr<GameObject>, std::shared_ptr<Texture>)> spawnBulletFunc,
                                   std::function<void(std::weak_ptr<GameObject>, std::shared_ptr<Texture>)> spawnBlastBulletFunc,
                                   Camera *camera, std::shared_ptr<PointLight> light) :
        spawnBulletFunc(spawnBulletFunc), spawnBlastBulletFunc(spawnBlastBulletFunc), camera(camera), light(light){

}

/*
 *
 * Article used to calculate plane/ray intersection
 * https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection#Algebraic_form
 *
 * Article used to calculate getMouseRayInWorldSpace
 * http://antongerdelan.net/opengl/raycasting.html
 */
chag::float3 PlayerController::getPlaneIntersectionPoint() {
    float mouseX = (float)(new MousePosition())->getWindowX();
    float mouseY = (float)(new MousePosition())->getWindowY();

    int width = Globals::get(Globals::WINDOW_WIDTH);
    int height = Globals::get(Globals::WINDOW_HEIGHT);

    auto l = getMouseRayInWorldSpace(mouseX, mouseY, width, height);

    auto p0 = chag::make_vector(0.0f, 0.0f, 0.0f);
    auto l0 = camera->getPosition();
    auto n = chag::make_vector(0.0f, 1.0f, 0.0f);

    auto d = chag::dot(p0 - l0, n) / chag::dot(l, n);

    auto intersectionPoint = d*l + l0;
    return intersectionPoint;
}

chag::SmallVector3<float> PlayerController::getMouseRayInWorldSpace(float mouseX, float mouseY, int width, int height) const {
    float mouseXInClipCoord = (mouseX / width) * 2 - 1;
    float mouseYInClipCoord = 1 - ((mouseY / height) * 2);
    float mouseZInClipCoord = 1.0f;
    float mouseWInClipCoord = 1.0f;

    auto mouseInClipCoord = chag::make_vector(mouseXInClipCoord, mouseYInClipCoord, mouseZInClipCoord, mouseWInClipCoord);

    auto mouseInEyeSpace = inverse(camera->getProjectionMatrix()) * mouseInClipCoord;
    mouseInEyeSpace.z = -1.0f;
    mouseInEyeSpace.w = 0.0f;

    auto mouseInWorldSpace = inverse(camera->getViewMatrix()) * mouseInEyeSpace;
    auto l = normalize(make_vector3(mouseInWorldSpace));
    return l;
}

void PlayerController::update(float dt) {
    if (owner.expired()) {
        return;
    }

    std::shared_ptr<GameObject> owner_ptr = owner.lock();

    ControlsManager* cm = ControlsManager::getInstance();
    ControlStatus horizontalStatus = cm->getStatus(MOVE_HORIZONTAL);
    ControlStatus verticalStatus = cm->getStatus(MOVE_VERTICAL);
    chag::float3 prevLocation = owner_ptr->getRelativeLocation();
    locationAtLastUpdate = owner_ptr->getRelativeLocation();
    if(horizontalStatus.isActive()){
        prevLocation.x += horizontalStatus.getValue() / 30.0f * dt * 3.0f;
    }
    if(verticalStatus.isActive()){
        prevLocation.z += verticalStatus.getValue() / 30.0f * dt * 3.0f;
    }
    owner_ptr->setLocation(prevLocation);

    timeSinceLastShotAttackLMB += dt;
    if(timeSinceLastShotAttackLMB > 1.0f) {
        ControlStatus shootButton = cm->getStatus(SHOOT_BUTTON_LMB);
        if (shootButton.isActive()) {
            spawnBulletFunc(owner, ResourceManager::loadAndFetchTexture("../assets/meshes/fire.png"));
            timeSinceLastShotAttackLMB = 0.0f;
        }
    }

    timeSinceLastShotAttackRMB += dt;
    if(timeSinceLastShotAttackRMB > 5.0f) {
        ControlStatus shootButton = cm->getStatus(SHOOT_BUTTON_RMB);
        if (shootButton.isActive()) {
            for (int i = 0; i < 8; ++i) {
                owner_ptr->setRotation(owner_ptr->getAbsoluteRotation() * chag::make_quaternion_axis_angle(chag::make_vector(0.0f, 1.0f, 0.0f), degreeToRad(45.0f * i)));
                spawnBlastBulletFunc(owner, ResourceManager::loadAndFetchTexture("../assets/meshes/blast.png"));
            }
            timeSinceLastShotAttackRMB = 0.0f;
        }
    }

    auto targetLookAt = getPlaneIntersectionPoint();

    chag::float3 vectorBetweenMouseAndPlayer = chag::normalize(targetLookAt - owner_ptr->getAbsoluteLocation());

    chag::float3 defaultDir = chag::make_vector(0.0f, 0.0f, 1.0f);
    float angle = (float)acos(chag::dot(defaultDir, vectorBetweenMouseAndPlayer));
    if(vectorBetweenMouseAndPlayer.x < 0) {
        angle = -angle ;
    }

    owner_ptr->setRotation(chag::make_quaternion_axis_angle(chag::make_vector(0.0f, 1.0f, 0.0f), angle));
    light->position = owner_ptr->getAbsoluteLocation() + chag::make_vector(0.0f, 2.0f, 0.0f);
}

void PlayerController::beforeCollision(std::shared_ptr<GameObject> collider) {
    if (owner.expired()) {
        return;
    }
    std::shared_ptr<GameObject> owner_ptr = owner.lock();
    owner_ptr->setLocation(locationAtLastUpdate);
}

void PlayerController::duringCollision(std::shared_ptr<GameObject> collider) {
    if (owner.expired()) {
        return;
    }
    std::shared_ptr<GameObject> owner_ptr = owner.lock();
    owner_ptr->setLocation(locationAtLastUpdate);
}
