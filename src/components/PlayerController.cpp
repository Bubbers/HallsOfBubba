//
// Created by simon on 2017-04-29.
//

#include <ControlsManager.h>
#include <controls.h>
#include "PlayerController.h"


PlayerController::PlayerController(std::function<void(chag::float3, chag::float3)> spawnBulletFunc) : spawnBulletFunc(spawnBulletFunc){

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
        chag::float4x4 rotationMatrix = chag::makematrix(owner->getAbsoluteRotation());
        chag::float4 startDirection = chag::make_vector(0.0f, 0.0f, 1.0f, 0.0f);
        chag::float4 currentDirection = rotationMatrix * startDirection;

        spawnBulletFunc(chag::make_vector3(currentDirection), owner->getAbsoluteLocation());
    }
}
