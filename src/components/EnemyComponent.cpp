
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

    chag::float3 prevLocation = owner->getRelativeLocation();
    locationAtLastUpdate = owner->getRelativeLocation();

    if(rand() % 60 == 0) {

        bool randa = (rand() % 10) < 5;
        printf("a:%d ",randa);
        if(randa) {
            previousXSpeed = -60;
        } else {
            previousXSpeed = 60;
        }

        randa = (rand() % 10) < 5;
        printf("b:%d \n",randa);
        if(randa) {
            previousYSpeed = -60;
        } else {
            previousYSpeed = 60;
        }
    }

    prevLocation.x += previousXSpeed / 30.0f * dt * 3.0f;
    prevLocation.z += previousYSpeed / 30.0f * dt * 3.0f;
    owner->setLocation(prevLocation);
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

void EnemyComponent::beforeCollision(std::shared_ptr<GameObject> collider) {
    owner->setLocation(locationAtLastUpdate);
    previousXSpeed = 0;
}

void EnemyComponent::duringCollision(std::shared_ptr<GameObject> collider) {
    owner->setLocation(locationAtLastUpdate);
    previousXSpeed = 0;
}
