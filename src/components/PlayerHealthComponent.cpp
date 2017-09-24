#include "PlayerHealthComponent.h"
#include "../event/Events.h"
#include <GameObject.h>
#include <Logger.h>

PlayerHealthComponent::PlayerHealthComponent(int maxHealth)  : HealthComponent(maxHealth) {
    listenerId = EventHandler::subscribe(Event::EARTHQUAKE, [&](){
        Logger::logInfo("event triggered");
        if(owner.expired()){
            return;
        }
        std::shared_ptr<GameObject> sharedOwner = owner.lock();
        if(sharedOwner->getRelativeLocation().y < 0.1f) {
            takeDamage(1);
        }
    });
}

PlayerHealthComponent::~PlayerHealthComponent() {
    EventHandler::unsubscribe(listenerId);
}
