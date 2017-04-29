#pragma once
#include "IComponent.h"

class TimedLife : public IComponent {
public:
    TimedLife(float lifeTime);
    ~TimedLife();

    void update(float dt) override;
private:
    float lifeTime;
};
