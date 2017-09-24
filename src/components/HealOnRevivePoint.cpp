

#include <GameObject.h>
#include <ObjectIdentifiers.h>
#include <Logger.h>
#include "HealOnRevivePoint.h"
#include "HealthComponent.h"

HealOnRevivePoint::HealOnRevivePoint(std::shared_ptr<HealthComponent> healthComponent) {
    this->healthComponent = healthComponent;
}

void HealOnRevivePoint::update(float dt) {

}

void HealOnRevivePoint::beforeCollision(std::shared_ptr<GameObject> collider) {
    Logger::logInfo("COLLISION");
    if (owner.expired()) {
        return;
    }
    std::shared_ptr<GameObject> owner_ptr = owner.lock();


    if(collider->getIdentifier() == REVIVE_POINT_IDENTIFIER) {
        this->healthComponent->restoreHealth();
        collider->makeDirty();
    }
}
