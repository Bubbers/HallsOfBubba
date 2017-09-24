#include <Logger.h>
#include "JumpingController.h"

JumpingController::JumpingController(float strength) : strength(strength){

}

void JumpingController::update(float dt) {
    if (isJumping()) {
        MoveComponent::update(dt);
        std::shared_ptr<GameObject> sharedOwner = owner.lock();
        chag::float3 location = sharedOwner->getRelativeLocation();
        if (location.y <= 0.0f) {
            location.y = 0.0f;
            sharedOwner->setLocation(location);
            setAcceleration(chag::make_vector(0.0f, 0.0f, 0.0f));
            setVelocity(chag::make_vector(0.0f, 0.0f, 0.0f));
            jumping = false;
            onLand();
        }
    }
}

void JumpingController::jump() {
    if(!isJumping()) {
        Logger::logInfo("jumping");
        setVelocity(chag::make_vector(0.0f, strength, 0.0f));
        setAcceleration(chag::make_vector(0.0f, -70.0f, 0.0f));
        jumping = true;
    }
}

bool JumpingController::isJumping() {
    return jumping;
}
