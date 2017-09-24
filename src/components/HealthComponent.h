//
// Created by simon on 2017-04-29.
//

#ifndef HALLSOFBUBBA_HEALTHCOMPONENT_H
#define HALLSOFBUBBA_HEALTHCOMPONENT_H


#include <IComponent.h>
#include <functional>
#include "ResourceManager.h"

class HealthComponent : public IComponent {

public:
    HealthComponent(int maxHealth);

    virtual void beforeCollision(std::shared_ptr<GameObject> collider) override;
    virtual void update(float dt) override;
    void addDamageListener(std::function<void(int)> damageListener);

    int getMaxHealth();
    int getHealth();

    void restoreHealth();

    void takeDamage(int damage);

protected:
    int health;
    int maxHealth;
    std::vector<std::function<void(int)>> damageListeners;

    std::shared_ptr<sf::Sound> m_deathSound;
    std::shared_ptr<sf::Sound> m_hurtSound;
};


#endif //HALLSOFBUBBA_HEALTHCOMPONENT_H
