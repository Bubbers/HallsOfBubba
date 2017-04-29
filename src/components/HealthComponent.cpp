//
// Created by simon on 2017-04-29.
//

#include "HealthComponent.h"

HealthComponent::HealthComponent(int maxHealth) : maxHealth(maxHealth), health(maxHealth) {
    m_deathSound = std::shared_ptr<sf::Sound>(AudioManager::loadAndFetchSound("../assets/sound/death.wav"));
    m_hurtSound = std::shared_ptr<sf::Sound>(AudioManager::loadAndFetchSound("../assets/sound/hurt.wav"));

}

void HealthComponent::update(float dt) {

}

void HealthComponent::beforeCollision(std::shared_ptr<GameObject> collider) {
    if(collider->getIdentifier() & 1){
        health--;
        for (auto listener : damageListeners) {
            listener(1);
        }
        m_hurtSound->play();
    }
    if(health <= 0){
        printf("YOU DEAD!\n");
        m_deathSound->play();
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

