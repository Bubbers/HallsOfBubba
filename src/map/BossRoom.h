#pragma once

#include "Room.h"

class BossRoom : public Room{
public:
    BossRoom();

protected:
    void loadGameObjects() override;
    void createBoss();

    std::shared_ptr<GameObject> getEnemyObject(std::function<void(GameObject *, std::shared_ptr<Texture>)> spawnBullet,
                                               std::shared_ptr<GameObject> playerObject, HudRenderer *hudRenderer,
                                               chag::float3 location);
};



