#ifndef HALLSOFBUBBA_PLAYER_H
#define HALLSOFBUBBA_PLAYER_H


#include <ControlStatus.h>
#include <components/HealthComponent.h>

class Player {

public:
    Player(ControlStatus::Activator activator);
    std::shared_ptr<HealthComponent> getHealthComponent();
    ControlStatus::Activator getActivator();

private:
    std::shared_ptr<HealthComponent> healthComponent = std::make_shared<HealthComponent>(2);
    ControlStatus::Activator activator;

};


#endif //HALLSOFBUBBA_PLAYER_H
