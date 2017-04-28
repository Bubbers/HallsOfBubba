//
// Created by simon on 2017-04-29.
//

#include <ControlsManager.h>
#include <controls.h>
#include "PlayerController.h"


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
}
