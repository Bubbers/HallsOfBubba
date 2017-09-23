#include <ObjectIdentifiers.h>
#include <ResourceManager.h>
#include "ChangeRoomOnCollision.h"
#include "GameObject.h"

ChangeRoomOnCollision::ChangeRoomOnCollision(std::shared_ptr<Scene> scene, std::function<void(void)> onCollision)
        :scene(scene), onCollision(onCollision) {
     m_sound = std::shared_ptr<sf::Sound>(ResourceManager::loadAndFetchSound("../assets/sound/win.wav"));
}

void ChangeRoomOnCollision::beforeCollision(std::shared_ptr<GameObject> collider) {
    int numEnemies = 0;
    for(auto gameObject : scene->getGameObjects()) {
        if(gameObject->getIdentifier() == ENEMY_IDENTIFIER) {
            numEnemies++;
        }
    }
    if(collider->getIdentifier() == PLAYER_IDENTIFIER && numEnemies == 0) {
        m_sound->play();
        onCollision();
    }
}

void ChangeRoomOnCollision::update(float dt) {

}
