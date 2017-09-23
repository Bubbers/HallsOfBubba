#pragma once

#include "Room.h"

class BossRoom : public Room{
public:
    BossRoom();

protected:
    void loadGameObjects() override;
    void createBoss();

    std::shared_ptr<GameObject> getEnemyObject(std::function<void(std::weak_ptr<GameObject>, std::shared_ptr<Texture>)> spawnBullet,
                                               std::vector<std::shared_ptr<GameObject>> playerObjects, HudRenderer *hudRenderer,
                                               chag::float3 location);
};



