#pragma once

#include <IComponent.h>

class DirtyBulletOnCollision : public IComponent{
public:
    void update(float dt) override;

    void beforeCollision(std::shared_ptr<GameObject> collider) override;

};
