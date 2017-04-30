#include <ObjectIdentifiers.h>
#include "WinOnCollisionComponent.h"

WinOnCollisionComponent::WinOnCollisionComponent(std::shared_ptr<Scene> scene, std::function<void(void)> onWin)
        :scene(scene), onWin(onWin) {
     m_sound = std::shared_ptr<sf::Sound>(AudioManager::loadAndFetchSound("../assets/sound/win.wav"));
}

void WinOnCollisionComponent::beforeCollision(std::shared_ptr<GameObject> collider) {
    int numEnemies = 0;
    for(auto gameObject : scene->getGameObjects()) {
        if(gameObject->getIdentifier() == ENEMY_IDENTIFIER) {
            numEnemies++;
        }
    }
    if(collider->getIdentifier() == PLAYER_IDENTIFIER && numEnemies == 0) {
        printf("WIN\n");
        m_sound->play();
        onWin();
    }
}

void WinOnCollisionComponent::update(float dt) {

}
