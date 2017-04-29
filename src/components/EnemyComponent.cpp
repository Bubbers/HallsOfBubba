
#include "EnemyComponent.h"

EnemyComponent::EnemyComponent(std::function<void(GameObject*)> bulletSpawner, std::shared_ptr<GameObject> playerObject) :
        bulletSpawner(bulletSpawner), playerObject(playerObject){

}

void EnemyComponent::update(float dt) {
    float randNum = float(rand()) / RAND_MAX;
    if(randNum < 1.0f/120.0f){ // one bullet every other second (on average)
        bulletSpawner(owner);
    }

    orientEnemyTowardsPlayer();

}

void EnemyComponent::orientEnemyTowardsPlayer() const {
    chag::float3 vectorBetweenEnemyAndPlayer = chag::normalize(playerObject->getAbsoluteLocation() - owner->getAbsoluteLocation());

    chag::float3 defaultDir = chag::make_vector(0.0f, 0.0f, 1.0f);
    float angle = acos(dot(defaultDir, vectorBetweenEnemyAndPlayer));
    if(vectorBetweenEnemyAndPlayer.x < 0) {
        angle = -angle ;
    }

    owner->setRotation(make_quaternion_axis_angle(chag::make_vector(0.0f, 1.0f, 0.0f), angle));
}
