#include "components/TimedLife.h"

TimedLife::TimedLife(float lifeTime) : lifeTime(lifeTime) {
}

TimedLife::~TimedLife() {
}

void TimedLife::update(float dt) {
    if (owner.expired()) {
        return;
    }

    lifeTime -= dt;
    if (lifeTime <= 0) {
        owner.lock()->makeDirty();
    }
}
