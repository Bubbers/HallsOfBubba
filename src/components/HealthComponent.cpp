//
// Created by simon on 2017-04-29.
//

#include <ObjectIdentifiers.h>
#include "HealthComponent.h"

void HealthComponent::update(float dt) {

}

void HealthComponent::beforeCollision(std::shared_ptr<GameObject> collider) {
    if(collider->getIdentifier() == ENEMY_SPAWNED_BULLET){
        health--;
    }
    if(health <= 0){
        printf("YOU DEAD!\n");
        owner->makeDirty();
    }
}

