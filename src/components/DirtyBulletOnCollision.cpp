#include <ObjectIdentifiers.h>
#include "DirtyBulletOnCollision.h"

void DirtyBulletOnCollision::update(float dt) {

}

void DirtyBulletOnCollision::beforeCollision(std::shared_ptr<GameObject> collider) {
    // Prevent boss shield from absorbing the boss' bullets
    if(!(owner->getIdentifier() == ENEMY_SPAWNED_BULLET && collider->getIdentifier() == BOSS_SHIELD)) {
        owner->makeDirty();
    }
}
