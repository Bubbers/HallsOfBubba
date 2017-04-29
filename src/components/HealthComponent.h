//
// Created by simon on 2017-04-29.
//

#ifndef HALLSOFBUBBA_HEALTHCOMPONENT_H
#define HALLSOFBUBBA_HEALTHCOMPONENT_H


#include <IComponent.h>

class HealthComponent : public IComponent {

public:

    HealthComponent(int maxHealth);

    virtual void beforeCollision(std::shared_ptr<GameObject> collider) override;
    virtual void update(float dt);
    void addDamageListener(std::function<void(int)> damageListener);

    int getMaxHealth();
    int getHealth();

private:
    int health;
    int maxHealth;
    std::vector<std::function<void(int)>> damageListeners;
};


#endif //HALLSOFBUBBA_HEALTHCOMPONENT_H
