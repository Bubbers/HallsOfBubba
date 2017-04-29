//
// Created by simon on 2017-04-29.
//

#ifndef HALLSOFBUBBA_HEALTHCOMPONENT_H
#define HALLSOFBUBBA_HEALTHCOMPONENT_H


#include <IComponent.h>

class HealthComponent : public IComponent {

public:

    virtual void beforeCollision(std::shared_ptr<GameObject> collider) override;
    virtual void update(float dt);

private:
    int health = 2;
};


#endif //HALLSOFBUBBA_HEALTHCOMPONENT_H
