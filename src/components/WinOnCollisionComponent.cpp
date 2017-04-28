#include "WinOnCollisionComponent.h"

WinOnCollisionComponent::WinOnCollisionComponent() {

}

void WinOnCollisionComponent::beforeCollision(std::shared_ptr<GameObject> collider) {
    printf("WIN\n");
}

void WinOnCollisionComponent::update(float dt) {

}
