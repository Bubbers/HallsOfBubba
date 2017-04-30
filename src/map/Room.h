#pragma once


#include <cameras/TopDownCamera.h>
#include <components/HealthComponent.h>
#include <Scene.h>
#include <Collider.h>
#include <Renderer.h>
#include "Direction.h"
#include "HudRenderer.h"

class Room {
public:
    void load(std::shared_ptr<TopDownCamera> camera, HealthComponent* player, Direction enteredDirection);
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

    std::vector<HealthComponent*> allAlive;

    std::shared_ptr<Scene> m_scene;
    std::shared_ptr<Collider> m_collider;
    std::shared_ptr<GameObject> playerObject;
    std::shared_ptr<TopDownCamera> camera;

    void loadBulletFunctions(std::shared_ptr<TopDownCamera> camera);

    std::shared_ptr<GameObject> generateBulletBase(GameObject *shooter);

    std::vector<std::pair<Direction, std::function<void(Direction)>>> doors;

    HudRenderer* hudRenderer;

    std::shared_ptr<sf::Sound> m_shootSound;
    std::shared_ptr<sf::Sound> m_blastSound;
    std::function<void(GameObject *, std::shared_ptr<Texture>)> spawnBullet;
    std::function<void(GameObject *, std::shared_ptr<Texture>)> spawnBlastBullet;

private:
    void loadWalls();
    void loadDoors();
    void loadFloor();

    void loadPlayer(HealthComponent *pComponent, Direction enteredDirection);

    void loadDirectionalLight();

    void loadHud(const std::shared_ptr<TopDownCamera> &camera);
};



