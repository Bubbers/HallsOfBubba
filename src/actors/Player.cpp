#include "Player.h"

Player::Player(ControlStatus::Activator activator) : activator(activator){

}

std::shared_ptr<HealthComponent> Player::getHealthComponent() {
    return healthComponent;
}

ControlStatus::Activator Player::getActivator() {
    return activator;
}
