
#include <Texture.h>
#include <ResourceManager.h>
#include "EnemyComponent.h"

EnemyComponent::EnemyComponent(std::function<void(std::weak_ptr<GameObject>, std::shared_ptr<Texture>)> bulletSpawner, std::shared_ptr<GameObject> playerObject) :
        bulletSpawner(bulletSpawner), playerObject(playerObject){

}

void EnemyComponent::update(float dt) {
    if (owner.expired()) {
        return;
    }
    std::shared_ptr<GameObject> owner_ptr = owner.lock();

    float randNum = float(rand()) / RAND_MAX;
    if(randNum < 1.0f/120.0f){ // one bullet every other second (on average)
        bulletSpawner(owner, ResourceManager::loadAndFetchTexture("../assets/meshes/fire.png"));
    }

    orientEnemyTowardsPlayer();

    chag::float3 prevLocation = owner_ptr->getRelativeLocation();
    locationAtLastUpdate = owner_ptr->getRelativeLocation();

    if(rand() % 60 == 0) {

        bool randa = (rand() % 10) < 5;
        if(randa) {
            previousXSpeed = -60;
        } else {
            previousXSpeed = 60;
        }

        randa = (rand() % 10) < 5;
        if(randa) {
            previousYSpeed = -60;
        } else {
            previousYSpeed = 60;
        }
    }

    prevLocation.x += previousXSpeed / 30.0f * dt * 3.0f;
    prevLocation.z += previousYSpeed / 30.0f * dt * 3.0f;
    owner_ptr->setLocation(prevLocation);
}

void EnemyComponent::orientEnemyTowardsPlayer() const {
    if (owner.expired()) {
        return;
    }
    std::shared_ptr<GameObject> owner_ptr = owner.lock();

    chag::float3 vectorBetweenEnemyAndPlayer = chag::normalize(playerObject->getAbsoluteLocation() - owner_ptr->getAbsoluteLocation());

    chag::float3 defaultDir = chag::make_vector(0.0f, 0.0f, 1.0f);
    float angle = acos(dot(defaultDir, vectorBetweenEnemyAndPlayer));
    if(vectorBetweenEnemyAndPlayer.x < 0) {
        angle = -angle ;
    }

    owner_ptr->setRotation(make_quaternion_axis_angle(chag::make_vector(0.0f, 1.0f, 0.0f), angle));
}

void EnemyComponent::beforeCollision(std::shared_ptr<GameObject> collider) {
    if (owner.expired()) {
        return;
    }

    owner.lock()->setLocation(locationAtLastUpdate);
    previousXSpeed = 0;
}

void EnemyComponent::duringCollision(std::shared_ptr<GameObject> collider) {
    if (owner.expired()) {
        return;
    }
    owner.lock()->setLocation(locationAtLastUpdate);
    previousXSpeed = 0;
}
