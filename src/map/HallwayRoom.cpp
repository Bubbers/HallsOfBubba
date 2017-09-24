#include <ShaderProgram.h>
#include <ResourceManager.h>
#include <StandardRenderer.h>
#include <ColliderFactory.h>
#include <constants.h>

#include <components/PlayerController.h>
#include <components/HealthComponent.h>
#include <components/EnemyComponent.h>
#include <components/ChangeRoomOnCollision.h>
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


HallwayRoom::HallwayRoom(bool includeObstacle)
        : includeObstacle(includeObstacle)
{
    mapSymbol = "X";
}

HallwayRoom::~HallwayRoom()
{
}

std::shared_ptr<GameObject> HallwayRoom::getEnemyObject(std::function<void(std::weak_ptr<GameObject>, std::shared_ptr<Texture>)> spawnBullet,
                                                 std::vector<std::shared_ptr<GameObject>> playerObjects,
                                                 HudRenderer *hudRenderer,
                                                 chag::float3 location)
{
    auto standardShader = ResourceManager::loadAndFetchShaderProgram(SIMPLE_SHADER_NAME,
                                                                     "",
                                                                     "");
    auto monsterMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/monster.obj");
    auto monsterCollisionMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/bubba_collision.obj");
    auto monsterObject = std::make_shared<GameObject>(monsterMesh, monsterCollisionMesh);
    std::shared_ptr<HealthComponent> monsterHealth = std::make_shared<HealthComponent>(2);

    monsterObject->addComponent(new EnemyComponent(spawnBullet, playerObjects));
    enemies.push_back(monsterHealth);
    monsterObject->addComponent(monsterHealth.get());
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
    auto gobParent = getCrateObject();

    auto gob1 = getCrateObject();
    gob1->setLocation(chag::make_vector(1.0f, -2.0f, 1.0f));
    auto gob2 = getCrateObject();
    gob2->setLocation(chag::make_vector(-1.0f, -2.0f, 1.0f));
    auto gob3 = getCrateObject();
    gob3->setLocation(chag::make_vector(0.0f, -2.0f, -1.0f));

    gobParent->addChild(gob1);
    gobParent->addChild(gob2);
    gobParent->addChild(gob3);
    gobParent->setLocation(centerPosition + chag::make_vector(0.0f, 2.0f, 0.0f));
    gobParent->setRotation(make_quaternion_axis_angle(make_vector(0.0f, 1.0f, 0.0f), degreeToRad(rand() % 360)));
    m_scene->addShadowCaster(gobParent);
}

std::shared_ptr<GameObject> HallwayRoom::getCrateObject() const {
    std::shared_ptr<ShaderProgram> standardShader = ResourceManager::loadAndFetchShaderProgram(SIMPLE_SHADER_NAME, "", "");
    std::shared_ptr<Mesh> obstacleMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/obstacle.obj");
    std::shared_ptr<GameObject> obstacleObject = std::make_shared<GameObject>(obstacleMesh);

    StandardRenderer* stdObstacleRenderer = new StandardRenderer(obstacleMesh, standardShader);
    obstacleObject->addRenderComponent(stdObstacleRenderer);
    obstacleObject->setIdentifier(OBSTACLE_IDENTIFIER);
    return obstacleObject;
}

void HallwayRoom::loadGameObjects() {
    randomlyGenerateObjectAtPos(chag::make_vector(8.0f, 0.0f, 8.0f));
    randomlyGenerateObjectAtPos(chag::make_vector(-8.0f, 0.0f, 8.0f));
    randomlyGenerateObjectAtPos(chag::make_vector(-8.0f, 0.0f, -8.0f));
    randomlyGenerateObjectAtPos(chag::make_vector(8.0f, 0.0f, -8.0f));
    randomlyGenerateObjectAtPos(chag::make_vector(0.0f, 0.0f, 0.0f));
}

void HallwayRoom::loadRevivePoint(chag::float3 locationOrigin) {
    auto standardShader = ResourceManager::loadAndFetchShaderProgram(SIMPLE_SHADER_NAME,
                                                                     "",
                                                                     "");

    std::shared_ptr<Mesh> reviveCircleMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/revive.fbx");
    std::shared_ptr<Mesh> reviveCircleCollisionMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/1mcubeCollision.obj");
    std::shared_ptr<GameObject> reviveCircleObject = std::make_shared<GameObject>(reviveCircleMesh,reviveCircleCollisionMesh);
    reviveCircleObject->setIdentifier(REVIVE_POINT_IDENTIFIER);
    reviveCircleObject->addCollidesWith(PLAYER_IDENTIFIER);
    StandardRenderer* stdReviveRenderer = new StandardRenderer(reviveCircleMesh, standardShader);
    reviveCircleObject->addRenderComponent(stdReviveRenderer);
    reviveCircleObject->setLocation(locationOrigin);

    std::shared_ptr<Mesh> reviveHealthMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/health.fbx");
    std::shared_ptr<GameObject> reviveHealthObject = std::make_shared<GameObject>(reviveHealthMesh);

    StandardRenderer* stdReviveHealthRenderer = new StandardRenderer(reviveHealthMesh, standardShader);
    reviveHealthObject->addRenderComponent(stdReviveHealthRenderer);

    reviveHealthObject->setLocation(chag::make_vector(0.0f, 2.0f, 0.0f));
    reviveHealthObject->setScale(chag::make_vector(0.6f, 0.6f, 0.6f));
    reviveCircleObject->addChild(reviveHealthObject);

    std::shared_ptr<SpotLight> spotLight = std::make_shared<SpotLight>();
    spotLight->diffuseColor= chag::make_vector(0.50f,0.50f,0.20f);
    spotLight->specularColor= chag::make_vector(0.00f,0.00f,0.00f);
    spotLight->ambientColor= chag::make_vector(0.050f,0.050f,0.050f);
    spotLight->position = locationOrigin + chag::make_vector(0.0f, 10.0f, -1.0f);
    spotLight->direction = chag::make_vector(0.0f, -1.0f, 0.1f);
    spotLight->cutOff = cosf(degreeToRad(3.0f));
    spotLight->outerCutOff = cosf(degreeToRad(10.0f));

    m_scene->spotLights.push_back(spotLight);

    m_scene->addTransparentObject(reviveCircleObject);
}

void HallwayRoom::randomlyGenerateObjectAtPos(chag::float3 location) {
    auto standardShader = ResourceManager::loadAndFetchShaderProgram(SIMPLE_SHADER_NAME,
                                                                     "",
                                                                     "");
    int randomNum = rand() % 6;

    if(randomNum == 0) {
        auto obstacleObject = getCrateObject();
        obstacleObject->setLocation(location);
        obstacleObject->setRotation(chag::make_quaternion_axis_angle(chag::make_vector(0.0f, 1.0f, 0.0f), degreeToRad(rand() % 360) ));
        m_scene->addShadowCaster(obstacleObject);
    } else if(randomNum == 1) {
        std::shared_ptr<GameObject> monsterObject2 = getEnemyObject(spawnBullet, playerObjects, hudRenderer, location);
        m_scene->addShadowCaster(monsterObject2);
    } else if(randomNum == 2) {
        addCrates(location);
    } else if(randomNum == 3) {
        createTorch(location);
    } else if(randomNum == 4) {
        loadRevivePoint(location);
    }
}



