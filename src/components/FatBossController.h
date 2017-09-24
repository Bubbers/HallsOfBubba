#ifndef HALLSOFBUBBA_FATBOSSCONTROLLER_H
#define HALLSOFBUBBA_FATBOSSCONTROLLER_H

#define FAT_BOSS_STRENGTH 25.0f

#include <actors/Player.h>
#include "JumpingController.h"

class FatBossController : public JumpingController {

public:

    FatBossController(std::vector<std::shared_ptr<GameObject>> playerObjects, std::vector<std::shared_ptr<Player>> players);

    virtual void update(float dt);

protected:
    virtual void onLand();

private:
    std::vector<std::shared_ptr<GameObject>> playerObjects;
    std::vector<std::shared_ptr<Player>> players;

};


#endif //HALLSOFBUBBA_FATBOSSCONTROLLER_H
