#include <ResourceManager.h>
#include <ObjectIdentifiers.h>
#include <StandardRenderer.h>
#include <components/BossController.h>
#include <constants.h>
#include <ui/HealthBar.h>
#include <components/BossDeathWin.h>
#include "BossRoom.h"
#include "GameObject.h"

BossRoom::BossRoom(std::function<void()> &allPlayersDead): Room(allPlayersDead)
{
}

void BossRoom::loadGameObjects() {
    createTorch(chag::make_vector(8.0f, 0.0f, 8.0f));
    createTorch(chag::make_vector(-8.0f, 0.0f, 8.0f));
    createTorch(chag::make_vector(-8.0f, 0.0f, -8.0f));
    createTorch(chag::make_vector(8.0f, 0.0f, -8.0f));
    createBoss();

    m_scene->addShadowCaster(getEnemyObject(spawnBullet, playerObjects, hudRenderer, chag::make_vector(0.0f, 0.0f, 8.0f)));
    m_scene->addShadowCaster(getEnemyObject(spawnBullet, playerObjects, hudRenderer, chag::make_vector(0.0f, 0.0f, -8.0f)));
}

void BossRoom::createBoss() {
    //Boss
    auto bossMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/boss.obj");
    auto bossCollisionMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/boss_collision.obj");
    auto shieldMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/boss_shield.obj");
    auto standardShader = ResourceManager::loadAndFetchShaderProgram(SIMPLE_SHADER_NAME, "", "");


    auto bossObject = std::make_shared<GameObject>(bossMesh, bossCollisionMesh);

    auto bossShieldObject = std::make_shared<GameObject>(shieldMesh, bossObject);
    bossShieldObject->setIdentifier(BOSS_SHIELD);
    bossShieldObject->addCollidesWith(PLAYER_SPAWNED_BULLET);
    bossShieldObject->setDynamic(true);
    StandardRenderer *stdShieldRenderer = new StandardRenderer(shieldMesh, standardShader);
    bossShieldObject->addRenderComponent(stdShieldRenderer);
    bossObject->addChild(bossShieldObject);

    std::shared_ptr<HealthComponent> bossHealth = std::make_shared<HealthComponent>(6);
    bossObject->addComponent(new BossController(spawnBullet, playerObjects));
    enemies.push_back(bossHealth);
    bossObject->addComponent(bossHealth.get());
    bossObject->setLocation(chag::make_vector(8.0f, 0.0f, 0.0f));
    bossObject->setScale(chag::make_vector(1.2f, 1.2f, 1.2f));
    bossObject->setRotation(chag::make_quaternion_axis_angle(chag::make_vector(0.0f,1.0f,0.0f), M_PI));
    bossObject->initializeModelMatrix();
    StandardRenderer *stdBossRenderer = new StandardRenderer(bossMesh, standardShader);
    bossObject->addRenderComponent(stdBossRenderer);
    bossObject->setDynamic(true);
    bossObject->setIdentifier(ENEMY_IDENTIFIER);
    bossObject->addCollidesWith({PLAYER_SPAWNED_BULLET, WALL_IDENTIFIER, OBSTACLE_IDENTIFIER});
    bossObject->addComponent(new BossDeathWin(hudRenderer));

    HealthBar* monsterHealthBar = new HealthBar(bossHealth);
    hudRenderer->addRelativeLayout(bossObject, monsterHealthBar);

    m_scene->addShadowCaster(bossObject);
}

std::shared_ptr<GameObject> BossRoom::getEnemyObject(std::function<void(std::weak_ptr<GameObject>, std::shared_ptr<Texture>)> spawnBullet,
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
    players.push_back(monsterHealth);
    monsterObject->addComponent(monsterHealth.get());
    monsterObject->setLocation(location);
    monsterObject->setRotation(chag::make_quaternion_axis_angle(chag::make_vector(0.0f, 1.0f, 0.0f), -6 * M_PI / 5));
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

