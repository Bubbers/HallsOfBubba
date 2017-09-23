#pragma once


#include <cameras/TopDownCamera.h>
#include <components/HealthComponent.h>
#include <Scene.h>
#include <Collider.h>
#include <Renderer.h>
#include <actors/Player.h>
#include "Direction.h"
#include <vector>
#include "HudRenderer.h"

class Room {
public:
    Room(std::function<void()> &allPlayersDead);

    void load(std::shared_ptr<TopDownCamera> camera, std::vector<std::shared_ptr<Player>> players, Direction enteredDirection);
    void addDoor(Direction direction, std::function<void(Direction direction)> callback);

    void display(Renderer &renderer,
                 std::shared_ptr<Camera> camera,
                 float timeSinceStart,
                 float timeSinceLastCall);

    void update(float dt);

protected:

    virtual void loadLights() {};
    virtual void loadGameObjects() {};

    void createTorch(chag::float3 location);

    bool isLoaded = false;

    std::vector<std::shared_ptr<HealthComponent>> players;
    std::vector<std::shared_ptr<HealthComponent>> enemies;

    std::shared_ptr<Scene> m_scene;
    std::shared_ptr<Collider> m_collider;
    std::vector<std::shared_ptr<GameObject>> playerObjects;
    std::shared_ptr<TopDownCamera> camera;

    void loadBulletFunctions(std::shared_ptr<TopDownCamera> camera);

    std::shared_ptr<GameObject> generateBulletBase(std::weak_ptr<GameObject> shooter);

    std::vector<std::pair<Direction, std::function<void(Direction)>>> doors;

    HudRenderer* hudRenderer;

    std::shared_ptr<sf::Sound> m_shootSound;
    std::shared_ptr<sf::Sound> m_blastSound;
    std::function<void(std::weak_ptr<GameObject>, std::shared_ptr<Texture>)> spawnBullet;
    std::function<void(std::weak_ptr<GameObject>, std::shared_ptr<Texture>)> spawnBlastBullet;

private:
    void loadWalls();
    void loadDoors();
    void loadFloor();

    void loadPlayer(std::vector<std::shared_ptr<Player>> players, Direction enteredDirection);

    void loadDirectionalLight();

    void loadHud(const std::shared_ptr<TopDownCamera> &camera);

    std::function<void()> allPlayersDead;
};



