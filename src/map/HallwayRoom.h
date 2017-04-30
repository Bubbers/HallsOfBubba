
#pragma once

#include <Scene.h>
#include <Collider.h>
#include <Renderer.h>
#include <AudioManager.h>
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
    void loadLights() override ;

    bool includeObstacle;

    void addCrates(chag::float3 centerPosition) const;
    std::shared_ptr<GameObject> getCrateObject(chag::float3 centerPosition, chag::float3 offset) const;

    std::shared_ptr<GameObject> getEnemyObject(std::function<void(GameObject *, std::shared_ptr<Texture>)> spawnBullet,
                                               std::shared_ptr<GameObject> playerObject,
                                               HudRenderer *hudRenderer,
                                               chag::float3 location);
};
