#ifndef HALLSOFBUBBA_PLAYERHEALTHCOMPONENT_H
#define HALLSOFBUBBA_PLAYERHEALTHCOMPONENT_H


#include <event/EventHandler.h>
#include "HealthComponent.h"

class PlayerHealthComponent : public HealthComponent {

public:
    PlayerHealthComponent(int maxHealth);
    virtual ~PlayerHealthComponent();

private:
    eventListenerIdentifier_t listenerId;

};


#endif //HALLSOFBUBBA_PLAYERHEALTHCOMPONENT_H
