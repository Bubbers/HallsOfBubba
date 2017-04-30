//
// Created by simon on 2017-05-01.
//

#include "BossController.h"

BossController::BossController(std::function<void(GameObject *, std::shared_ptr<Texture>)> bulletSpawner,
                               std::shared_ptr<GameObject> playerObject) : EnemyComponent(bulletSpawner, playerObject){

}


void BossController::update(float dt) {

    if (stunnedFor > 0.0f) {
        stunnedFor -= dt;
    } else if (charging) {

        locationAtLastUpdate = owner->getRelativeLocation();
        owner->setLocation(owner->getRelativeLocation() + chargeSpeed);

    } else if(rand() % 240 == 0) {

        hitPlayer = false;
        locationAtLastUpdate = owner->getRelativeLocation();

        orientEnemyTowardsPlayer();
        chag::float4 forward = chag::make_vector(0.0f, 0.0f, 1.0f, 0.0f);
        chag::float3 towardsPlayer = normalize(make_vector3(chag::makematrix(owner->getAbsoluteRotation()) * forward));
        chargeSpeed = towardsPlayer / 2.0f;
        charging = true;

        owner->setLocation(owner->getRelativeLocation() + chargeSpeed);

    } else {
        EnemyComponent::update(dt);
    }

}

void BossController::beforeCollision(std::shared_ptr<GameObject> collider) {
    if(charging){
        if(collider->getIdentifier() == PLAYER_IDENTIFIER) {
            hitPlayer = true;
        }else{
            charging = false;
            if(!hitPlayer){
                stunnedFor = 2.0f;
            }
        }
    }else{
        EnemyComponent::beforeCollision(collider);
    }
}

void BossController::duringCollision(std::shared_ptr<GameObject> collider) {
    if(collider->getIdentifier() != PLAYER_IDENTIFIER) {
        EnemyComponent::duringCollision(collider);
    }
}
