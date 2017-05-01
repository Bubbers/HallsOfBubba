#pragma once

#include <IComponent.h>
#include "HudRenderer.h"

class BossDeathWin : public IComponent{
public:
    BossDeathWin(HudRenderer *hudRenderer);

    void onDeath() override;

    void update(float dt) override;

private:
    HudRenderer *hudRenderer;
};



