#include <ShaderProgram.h>
#include <ResourceManager.h>
#include <StandardRenderer.h>
#include <ColliderFactory.h>
#include <constants.h>

#include <components/PlayerController.h>
#include <components/HealthComponent.h>
#include <components/EnemyComponent.h>
#include <components/WinOnCollisionComponent.h>
#include <ui/HealthBar.h>
#include <components/TimedLife.h>
#include <components/DirtyBulletOnCollision.h>
#include <ParticleGenerator.h>
#include <ParticleRenderer.h>
#include <particles/BackgroundParticle.h>
#include <particles/TorchParticle.h>
#include <particles/FirstAttackParticle.h>
#include <particles/SecondAttackParticle.h>
#include "HallwayRoom.h"


HallwayRoom::HallwayRoom(bool includeObstacle) : includeObstacle(includeObstacle){

}


HallwayRoom::~HallwayRoom()
{
}

std::shared_ptr<GameObject> HallwayRoom::getEnemyObject(std::function<void(GameObject *, std::shared_ptr<Texture>)> spawnBullet,
                                                 std::shared_ptr<GameObject> playerObject,
                                                 HudRenderer *hudRenderer,
                                                 chag::float3 location)
{
    auto standardShader = ResourceManager::loadAndFetchShaderProgram(SIMPLE_SHADER_NAME,
                                                                     "",
                                                                     "");
    auto monsterMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/monster.obj");
    auto monsterCollisionMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/bubba_collision.obj");
    auto monsterObject = std::make_shared<GameObject>(monsterMesh, monsterCollisionMesh);
    HealthComponent *monsterHealth = new HealthComponent(2);

    monsterObject->addComponent(new EnemyComponent(spawnBullet, playerObject));
    allAlive.push_back(monsterHealth);
    monsterObject->addComponent(monsterHealth);
    monsterObject->setLocation(location);
    monsterObject->setRotation(make_quaternion_axis_angle(make_vector(0.0f, 1.0f, 0.0f), -6 * M_PI / 5));
    monsterObject->initializeModelMatrix();
    StandardRenderer *stdMonsterRenderer = new StandardRenderer(monsterMesh, standardShader);
    monsterObject->addRenderComponent(stdMonsterRenderer);
    monsterObject->setDynamic(true);
    monsterObject->setIdentifier(ENEMY_IDENTIFIER);
    monsterObject->addCollidesWith({PLAYER_SPAWNED_BULLET, WALL_IDENTIFIER, OBSTACLE_IDENTIFIER});

    HealthBar* monsterHealthBar = new HealthBar(monsterHealth);
    hudRenderer->addRelativeLayout(monsterObject, monsterHealthBar);

    return monsterObject;
}

void HallwayRoom::addCrates(chag::float3 centerPosition) const {
    m_scene->addShadowCaster(getCrateObject(centerPosition, chag::make_vector(1.0f, 0.0f, 1.0f)));
    m_scene->addShadowCaster(getCrateObject(centerPosition, chag::make_vector(-1.0f, 0.0f, 1.0f)));
    m_scene->addShadowCaster(getCrateObject(centerPosition, chag::make_vector(0.0f, 0.0f, -1.0f)));

    const std::shared_ptr<GameObject> &object = getCrateObject(centerPosition, chag::make_vector(0.0f, 2.0f, 0.0f));
    object->setRotation(make_quaternion_axis_angle(make_vector(0.0f, 1.0f, 0.0f), degreeToRad(135)));
    m_scene->addShadowCaster(object);
}

std::shared_ptr<GameObject> HallwayRoom::getCrateObject(float3 centerPosition, float3 offset) const {
    std::shared_ptr<ShaderProgram> standardShader = ResourceManager::loadAndFetchShaderProgram(SIMPLE_SHADER_NAME, "", "");
    std::shared_ptr<Mesh> obstacleMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/obstacle.obj");;
    std::shared_ptr<GameObject> obstacleObject = std::make_shared<GameObject>(obstacleMesh);
    obstacleObject->setLocation(centerPosition + offset);
    //obstacleObject->setRotation(make_quaternion_axis_angle(make_vector(0.0f, 1.0f, 0.0f), degreeToRad(180)));
    StandardRenderer* stdObstacleRenderer = new StandardRenderer(obstacleMesh, standardShader);
    obstacleObject->addRenderComponent(stdObstacleRenderer);
    obstacleObject->setIdentifier(OBSTACLE_IDENTIFIER);
    return obstacleObject;
}

void HallwayRoom::loadLights() {
    std::shared_ptr<DirectionalLight> directionalLight = std::make_shared<DirectionalLight>();
    directionalLight->diffuseColor= chag::make_vector(0.050f,0.050f,0.050f);
    directionalLight->specularColor= chag::make_vector(0.050f,0.050f,0.050f);
    directionalLight->ambientColor= chag::make_vector(0.000f,0.000f,0.000f);

    directionalLight->direction= -chag::make_vector(0.0f,-10.0f,10.0f);
    m_scene->directionalLight = directionalLight;
}

void HallwayRoom::loadGameObjects() {
    auto standardShader = ResourceManager::loadAndFetchShaderProgram(SIMPLE_SHADER_NAME,
                                                                     "",
                                                                     "");

    // HUD
    std::shared_ptr<GameObject> hudObj = std::make_shared<GameObject>();
    hudObj->addRenderComponent(hudRenderer);
    m_scene->addTransparentObject(hudObj);


    //Enemy mesh
    std::shared_ptr<GameObject> monsterObject = getEnemyObject(spawnBullet,
                                                               playerObject,
                                                               hudRenderer,
                                                               make_vector(-8.0f, 0.0f, 8.0f));
    m_scene->addShadowCaster(monsterObject);

    std::shared_ptr<GameObject> monsterObject2 = getEnemyObject(spawnBullet,
                                                                playerObject,
                                                                hudRenderer,
                                                                make_vector(8.0f, 0.0f, 8.0f));
    m_scene->addShadowCaster(monsterObject2);



    // Windmil mesh
    auto windMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/wind.fbx");

    auto windObject = std::make_shared<GameObject>(windMesh);
    windObject->setLocation(chag::make_vector(11.0f, 0.0f, 10.0f));
    windObject->setScale(chag::make_vector(0.005f, 0.005f, 0.005f));
    windObject->setRotation(chag::make_quaternion_axis_angle(chag::make_vector(0.0f, 1.0f, 0.0f), degreeToRad(235)));
    StandardRenderer* stdWindRenderer = new StandardRenderer(windMesh, standardShader);
    windObject->addRenderComponent(stdWindRenderer);
    windObject->setIdentifier(OBSTACLE_IDENTIFIER);
    m_scene->addShadowCaster(windObject);

    // Obstacle mesh
    if (includeObstacle) {
        auto obstacleMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/obstacle.obj");

        auto obstacleObject = std::make_shared<GameObject>(obstacleMesh);
        obstacleObject->setLocation(chag::make_vector(-5.0f, 0.0f, 5.0f));
        obstacleObject->setRotation(
                chag::make_quaternion_axis_angle(chag::make_vector(0.0f, 1.0f, 0.0f), degreeToRad(180)));
        StandardRenderer *stdObstacleRenderer = new StandardRenderer(obstacleMesh, standardShader);
        obstacleObject->addRenderComponent(stdObstacleRenderer);
        obstacleObject->setIdentifier(OBSTACLE_IDENTIFIER);
        m_scene->addShadowCaster(obstacleObject);
    }
    addCrates(chag::make_vector(-9.0f, 0.0f, 0.0f));
    addCrates(chag::make_vector(8.0f, 0.0f, -6.0f));

    createTorch(make_vector(-5.0f, 0.0f, 9.0f));
    createTorch(make_vector(5.0f, 0.0f, 9.0f));
}




