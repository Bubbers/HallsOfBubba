#ifndef HALLSOFBUBBA_FATBOSSCONTROLLER_H
#define HALLSOFBUBBA_FATBOSSCONTROLLER_H

#define FAT_BOSS_STRENGTH 25.0f

#include <actors/Player.h>
#include <particles/PukeParticle.h>
#include <statemachine/StateMachine.h>
#include "JumpingController.h"

class FatBossController : public JumpingController {

public:

    FatBossController(std::vector<std::shared_ptr<GameObject>> playerObjects,
                      std::vector<std::shared_ptr<Player>> players, std::shared_ptr<PukeParticle> puke);

    virtual void update(float dt);

protected:
    virtual void onLand();

private:
    std::vector<std::shared_ptr<GameObject>> playerObjects;
    std::vector<std::shared_ptr<Player>> players;
    std::shared_ptr<PukeParticle> puke;
    StateMachine attackState;
    float timeSinceStateChange;

    std::function<void(float dt)> currentAttack;

    enum AttackState : StateKey {
        PUKING,
        JUMPING,
        NONE
    };

    void nopAttack(float dt);

    void pukeAttack(float dt);
};


#endif //HALLSOFBUBBA_FATBOSSCONTROLLER_H
