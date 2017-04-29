#include <ObjectIdentifiers.h>
#include "WinOnCollisionComponent.h"

WinOnCollisionComponent::WinOnCollisionComponent(std::shared_ptr<Scene> scene) :scene(scene){

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
    }
}

void WinOnCollisionComponent::update(float dt) {

}
