//
// Created by simon on 2017-04-29.
//

#ifndef HALLSOFBUBBA_HEALTHCOMPONENT_H
#define HALLSOFBUBBA_HEALTHCOMPONENT_H


#include <IComponent.h>
#include <AudioManager.h>

class HealthComponent : public IComponent {

public:
    HealthComponent();

    virtual void beforeCollision(std::shared_ptr<GameObject> collider) override;
    virtual void update(float dt);

private:
    int health = 2;

    std::shared_ptr<sf::Sound> m_deathSound;
    std::shared_ptr<sf::Sound> m_hurtSound;
};


#endif //HALLSOFBUBBA_HEALTHCOMPONENT_H
