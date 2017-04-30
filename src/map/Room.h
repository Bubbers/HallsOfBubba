
#pragma once

#include <Scene.h>
#include <Collider.h>
#include <Renderer.h>
#include <AudioManager.h>
#include <cameras/TopDownCamera.h>
#include <map/Direction.h>
#include <components/HealthComponent.h>

class HudRenderer;

class Room {

public:
    Room(bool includeObstacle);
    ~Room();

    void load(std::shared_ptr<TopDownCamera> camera, HealthComponent* player, Direction enteredDirection);
    void addDoor(Direction direction, std::function<void(Direction)> callback);
    void update(float dt);
    void display(Renderer &renderer,
                 std::shared_ptr<Camera> camera,
                 float timeSinceStart,
                 float timeSinceLastCall);


private:
    void createLight();
    void createWalls();

    std::shared_ptr<Scene> m_scene;
    std::shared_ptr<Collider> m_collider;

    HudRenderer* hudRenderer;
    std::vector<HealthComponent*> allAlive;

    std::shared_ptr<sf::Sound> m_shootSound;
    std::vector<std::pair<Direction, std::function<void(Direction)>>> doors;

    bool includeObstacle;
    bool isLoaded = false;
    void addCrates(chag::float3 centerPosition) const;

    std::shared_ptr<GameObject> getCrateObject(chag::float3 centerPosition, chag::float3 offset) const;
    std::shared_ptr<GameObject> generateBulletBase(GameObject* shooter);
};
