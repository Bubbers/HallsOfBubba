#include <event/EventHandler.h>
#include <event/Events.h>
#include <Logger.h>
#include "FatBossController.h"
#include <linmath/float3x3.h>
#include <particles/PukeParticle.h>

FatBossController::FatBossController(std::vector<std::shared_ptr<GameObject>> playerObjects,
                                     std::vector<std::shared_ptr<Player>> players, std::shared_ptr<PukeParticle> puke)
        : players(players), playerObjects(playerObjects), puke(puke), JumpingController(FAT_BOSS_STRENGTH),
          attackState({PUKING, JUMPING, NONE}){

    /*attackState.connect(NONE, JUMPING, []() -> {
        jump();
        currentAttack = nopAttack;
        timeSinceStateChange = 0.0f;
    });
    attackState.connect(NONE, PUKING, []() -> {
        timeSinceStateChange = 0.0f;
        currentAttack = pukeAttack;
    });
    attackState.connect(PUKING, NONE, []() -> {
        timeSinceStateChange = 0.0f;
        currentAttack = nopAttack;
    });
    attackState.connect(JUMPING, NONE, []() -> {
        timeSinceStateChange = 0.0f;
        currentAttack = nopAttack;
    });*/

}

void FatBossController::update(float dt) {

    if(owner.expired()) {
        return;
    }

    std::shared_ptr<GameObject> lockedOwner = owner.lock();

    JumpingController::update(dt);

    float rand = ((float) std::rand() / RAND_MAX);

    if(rand < 1.0f/300.0f) {
        jump();
    }

    if(isJumping()) {
        return;
    }

    std::shared_ptr<GameObject> closestPlayer;
    float closestDistance = std::numeric_limits<float>::max();
    for (auto playerObject : playerObjects) {
        float dist = chag::length(playerObject->getAbsoluteLocation() - lockedOwner->getAbsoluteLocation());
        if (dist < closestDistance) {
            closestPlayer = playerObject;
        }
    }

    chag::float3 bossToPlayerVector = lockedOwner->getAbsoluteLocation() - closestPlayer->getAbsoluteLocation();
    double angle = atan(bossToPlayerVector.z/bossToPlayerVector.x);
    if(bossToPlayerVector.x < 0) {
        angle += M_PI;
    }
    chag::Quaternion rotation = chag::make_quaternion_axis_angle(chag::make_vector(0.0f, 1.0f, 0.0f), (float) -(angle + M_PI_2l));
    lockedOwner->setRotation(rotation);
    chag::float3 directionToPlayer = chag::toMatrix3x3(rotation) * chag::make_vector(0.0f, 0.0f, 1.0f);
    chag::float3 location = lockedOwner->getRelativeLocation();
    chag::float3 newLocation = location + directionToPlayer * 0.075f;
    newLocation.y = location.y; // Don't ruin the jump
    lockedOwner->setLocation(newLocation);

}

void FatBossController::nopAttack(float dt) {}

void FatBossController::pukeAttack(float dt) {

}

void FatBossController::onLand() {
    EventHandler::fire(Event::EARTHQUAKE);
    attackState.queueTransition(NONE);
}
