//
// Created by simon on 2017-04-29.
//

#include <ControlsManager.h>
#include <controls.h>
#include <MousePosition.h>
#include <linmath/SmallVector2.h>
#include <Globals.h>
#include "PlayerController.h"
#include "Camera.h"


PlayerController::PlayerController(std::function<void(GameObject*)> spawnBulletFunc, Camera *camera) :
        spawnBulletFunc(spawnBulletFunc), camera(camera){

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
    ControlsManager* cm = ControlsManager::getInstance();
    ControlStatus horizontalStatus = cm->getStatus(MOVE_HORIZONTAL);
    ControlStatus verticalStatus = cm->getStatus(MOVE_VERTICAL);
    chag::float3 prevLocation = owner->getRelativeLocation();
    if(horizontalStatus.isActive()){
        prevLocation.x += horizontalStatus.getValue() / 100.0f * dt * 3.0f;
    }
    if(verticalStatus.isActive()){
        prevLocation.z += verticalStatus.getValue() / 100.0f * dt * 3.0f;
    }
    owner->setLocation(prevLocation);

    ControlStatus shootButton = cm->getStatus(SHOOT_BUTTON);
    if(shootButton.isActive()) {
        spawnBulletFunc(owner);
    }

    auto targetLookAt = getPlaneIntersectionPoint();

    chag::float3 vectorBetweenMouseAndPlayer = chag::normalize(targetLookAt - owner->getAbsoluteLocation());

    chag::float3 defaultDir = chag::make_vector(0.0f, 0.0f, 1.0f);
    float angle = acos(chag::dot(defaultDir, vectorBetweenMouseAndPlayer));
    if(vectorBetweenMouseAndPlayer.x < 0) {
        angle = -angle ;
    }

    owner->setRotation(chag::make_quaternion_axis_angle(chag::make_vector(0.0f, 1.0f, 0.0f), angle));
}
