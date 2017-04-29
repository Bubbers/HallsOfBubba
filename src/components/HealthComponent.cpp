//
// Created by simon on 2017-04-29.
//

#include "HealthComponent.h"

HealthComponent::HealthComponent() {

    m_deathSound = std::shared_ptr<sf::Sound>(AudioManager::loadAndFetchSound("../assets/sound/death.wav"));
    m_hurtSound = std::shared_ptr<sf::Sound>(AudioManager::loadAndFetchSound("../assets/sound/hurt.wav"));
}

void HealthComponent::update(float dt) {

}

void HealthComponent::beforeCollision(std::shared_ptr<GameObject> collider) {
    if(collider->getIdentifier() & 1){
        health--;
        m_hurtSound->play();
    }
    if(health <= 0){
        printf("YOU DEAD!\n");
        m_deathSound->play();
        owner->makeDirty();
    }
}

