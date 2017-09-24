#include <event/EventHandler.h>
#include <event/Events.h>
#include <Logger.h>
#include "FatBossController.h"

FatBossController::FatBossController(std::vector<std::shared_ptr<GameObject>> playerObjects, std::vector<std::shared_ptr<Player>> players)
        : players(players), playerObjects(playerObjects), JumpingController(FAT_BOSS_STRENGTH){

}

void FatBossController::update(float dt) {

    if(owner.expired()) {
        return;
    }

    JumpingController::update(dt);

    float rand = ((float) std::rand() / RAND_MAX);

    if(rand < 1.0f/60.0f) {
        jump();
    }

}

void FatBossController::onLand() {
    Logger::logInfo("Landed!");
    EventHandler::fire(Event::EARTHQUAKE);
}
