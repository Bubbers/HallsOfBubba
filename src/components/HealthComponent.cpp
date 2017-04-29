//
// Created by simon on 2017-04-29.
//

#include "HealthComponent.h"

void HealthComponent::update(float dt) {

}

void HealthComponent::beforeCollision(std::shared_ptr<GameObject> collider) {
    if(collider->getIdentifier() & 1){
        health--;
        for(auto it = damageListeners.begin(); it < damageListeners.end(); it++){
            (*it)(1);
        }
    }
    if(health <= 0){
        printf("YOU DEAD!\n");
        owner->makeDirty();
    }
}

void HealthComponent::addDamageListener(std::function<void(int)> damageListener) {
    damageListeners.push_back(damageListener);
}

int HealthComponent::getHealth() {
    return health;
}

int HealthComponent::getMaxHealth() {
    return maxHealth;
}

HealthComponent::HealthComponent(int maxHealth) : maxHealth(maxHealth), health(maxHealth) {

}
