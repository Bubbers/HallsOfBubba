
#pragma once

#include <Scene.h>
#include <Collider.h>
#include <Renderer.h>
#include <ResourceManager.h>
#include <cameras/TopDownCamera.h>
#include <map/Direction.h>
#include <components/HealthComponent.h>
#include "Room.h"

class HudRenderer;

class HallwayRoom : public Room {

public:
    HallwayRoom(bool includeObstacle);
    ~HallwayRoom();

    void loadGameObjects() override ;

private:

    bool includeObstacle;

    void addCrates(chag::float3 centerPosition) const;
    std::shared_ptr<GameObject> getCrateObject() const;

    std::shared_ptr<GameObject> getEnemyObject(std::function<void(std::weak_ptr<GameObject>, std::shared_ptr<Texture>)> spawnBullet,
                                               std::vector<std::shared_ptr<GameObject>> playerObject,
                                               HudRenderer *hudRenderer,
                                               chag::float3 location);
    void loadRevivePoint(chag::float3 locationOrigin);

    void randomlyGenerateObjectAtPos(chag::float3 location);
};
