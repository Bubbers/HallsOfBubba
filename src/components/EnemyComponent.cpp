//
// Created by simon on 2017-04-29.
//

#include "EnemyComponent.h"

void EnemyComponent::update(float dt) {

    float randNum = float(rand()) / RAND_MAX;
    if(randNum < 1.0f/120.0f){ // one bullet every other second (on average)
        bulletSpawner(owner);
    }

}

EnemyComponent::EnemyComponent(std::function<void(GameObject*)> bulletSpawner) : bulletSpawner(bulletSpawner){

}
