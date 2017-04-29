#include "DirtyBulletOnCollision.h"

void DirtyBulletOnCollision::update(float dt) {

}

void DirtyBulletOnCollision::beforeCollision(std::shared_ptr<GameObject> collider) {
    owner->makeDirty();
}
