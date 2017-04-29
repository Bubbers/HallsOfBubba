#include <ObjectIdentifiers.h>
#include "WinOnCollisionComponent.h"

WinOnCollisionComponent::WinOnCollisionComponent() {

}

void WinOnCollisionComponent::beforeCollision(std::shared_ptr<GameObject> collider) {
    if(collider->getIdentifier() == PLAYER_IDENTIFIER) {
        printf("WIN\n");
    }
}

void WinOnCollisionComponent::update(float dt) {

}
