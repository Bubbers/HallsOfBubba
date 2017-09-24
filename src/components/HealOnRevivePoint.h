

#pragma once

#include <IComponent.h>
#include "HealthComponent.h"


class HealOnRevivePoint : public IComponent{
public:
    HealOnRevivePoint(std::shared_ptr<HealthComponent> healthComponent);

    void update(float dt) override;

    void beforeCollision(std::shared_ptr<GameObject> collider) override;

private:
    std::shared_ptr<HealthComponent> healthComponent;
};



