#include <ObjectIdentifiers.h>
#include "DirtyBulletOnCollision.h"

void DirtyBulletOnCollision::update(float dt) {

}

void DirtyBulletOnCollision::beforeCollision(std::shared_ptr<GameObject> collider) {
    if (owner.expired()) {
        return;
    }
    std::shared_ptr<GameObject> owner_ptr = owner.lock();

    // Prevent boss shield from absorbing the boss' bullets
    if(!(owner_ptr->getIdentifier() == ENEMY_SPAWNED_BULLET && collider->getIdentifier() == BOSS_SHIELD)) {
        owner_ptr->makeDirty();
    }
}
