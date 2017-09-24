//
// Created by simon on 2016-10-01.
//

#ifndef BUBBAROGUEFORT_HEALTHBAR_H
#define BUBBAROGUEFORT_HEALTHBAR_H

#include <components/HealthComponent.h>
#include "PositioningLayout.h"


class HealthBar : public PositioningLayout {

public:
    HealthBar(std::shared_ptr<HealthComponent> health);

    virtual void getGLSquares(float layoutXPos, float layoutYPos, float layoutWidth, float layoutHeight,
                              std::map<std::string, IHudDrawable *> *map) override;

protected:
    void updateLife();
    std::shared_ptr<HealthComponent> health;
    PositioningLayout* bar;
    bool dead = false;

};


#endif //BUBBAROGUEFORT_HEALTHBAR_H
