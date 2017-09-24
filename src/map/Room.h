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

typedef std::function<void()> lose_callback_t;
typedef std::function<void(Direction)> walk_callback_t;

class Room {
public:
    Room();

    void load(std::shared_ptr<TopDownCamera> camera, std::vector<std::shared_ptr<Player>> players, Direction enteredDirection);
    void addDoor(Direction direction, walk_callback_t callback);

    std::shared_ptr<Scene> getScene();

    void update(float dt, lose_callback_t loseCallback);

    const std::string getMapSymbol();

protected:

    virtual void loadLights() {};
    virtual void loadGameObjects() {};

    void createTorch(chag::float3 location);

    bool isLoaded = false;

    std::vector<std::shared_ptr<Player>> players;
    std::vector<std::shared_ptr<HealthComponent>> enemies;

    std::shared_ptr<Scene> m_scene;
    std::shared_ptr<Collider> m_collider;
    std::vector<std::shared_ptr<GameObject>> playerObjects;
    std::shared_ptr<TopDownCamera> camera;

    void loadBulletFunctions(std::shared_ptr<TopDownCamera> camera);

    std::shared_ptr<GameObject> generateBulletBase(std::weak_ptr<GameObject> shooter);

    std::vector<std::pair<Direction, walk_callback_t>> doors;

    HudRenderer* hudRenderer;

    std::shared_ptr<sf::Sound> m_shootSound;
    std::shared_ptr<sf::Sound> m_blastSound;
    std::function<void(std::weak_ptr<GameObject>, std::shared_ptr<Texture>)> spawnBullet;
    std::function<void(std::weak_ptr<GameObject>, std::shared_ptr<Texture>)> spawnBlastBullet;

    std::string mapSymbol;

private:
    void loadWalls();
    void loadDoors();
    void loadFloor();

    void loadPlayer(std::vector<std::shared_ptr<Player>> players, Direction enteredDirection);

    void loadDirectionalLight();

    void loadHud(const std::shared_ptr<TopDownCamera> &camera);
};



