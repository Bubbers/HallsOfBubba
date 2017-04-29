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
#include "room.h"

Room::Room()
{
}

Room::~Room()
{
}

void Room::load(std::shared_ptr<TopDownCamera> camera) {
    m_collider = std::shared_ptr<Collider>(ColliderFactory::getTwoPhaseCollider());

    auto standardShader = ResourceManager::loadAndFetchShaderProgram(SIMPLE_SHADER_NAME,
                                                                     "",
                                                                     "");

    m_shootSound = std::shared_ptr<sf::Sound>(AudioManager::loadAndFetchSound("../assets/sound/shoot.wav"));
    auto playerMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/bubba.obj");
    auto playerCollisionMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/bubba_collision.obj");
    auto monsterMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/monster.obj");

    m_scene = std::make_shared<Scene>();
    auto spawnBullet = [&](GameObject *shooter) mutable {
        chag::float4x4 rotationMatrix = chag::makematrix(shooter->getAbsoluteRotation());
        chag::float4 startDirection = chag::make_vector(0.0f, 0.0f, 1.0f, 0.0f);
        chag::float3 direction = chag::make_vector3(rotationMatrix * startDirection);

        auto standardShader = ResourceManager::loadAndFetchShaderProgram(SIMPLE_SHADER_NAME,
                                                                         "",
                                                                         "");
        auto bulletMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/bullet.obj");

        auto bulletObject = std::make_shared<GameObject>(bulletMesh);
        bulletObject->setLocation(shooter->getAbsoluteLocation() + chag::make_vector(0.0f, 2.0f, 0.0f));
        bulletObject->setRotation(shooter->getAbsoluteRotation());
        bulletObject->setScale(chag::make_vector(3.0f,3.0f,3.0f));
        bulletObject->addComponent(new TimedLife(20.0f));
        bulletObject->addComponent(new MoveComponent(
                chag::make_quaternion_axis_angle(chag::make_vector(0.0f, 1.0f, 0.0f), 0.0f),
                direction * 10,
                chag::make_vector(0.0f, 0.0f, 0.0f),
                chag::make_vector(0.0f, 0.0f, 0.0f)
        ));

        int shootId = 0;
        if (shooter->getIdentifier() == PLAYER_IDENTIFIER) {
            shootId = PLAYER_SPAWNED_BULLET;
            bulletObject->addCollidesWith(ENEMY_IDENTIFIER);
        } else {
            shootId = ENEMY_SPAWNED_BULLET;
            bulletObject->addCollidesWith(PLAYER_IDENTIFIER);
        }
        bulletObject->setIdentifier(shootId);
        bulletObject->addCollidesWith({DOOR_IDENTIFIER, OBSTACLE_IDENTIFIER, WALL_IDENTIFIER});
        bulletObject->addComponent(new DirtyBulletOnCollision());

        bulletObject->update(0);
        StandardRenderer* stdRenderer = new StandardRenderer(bulletMesh, standardShader);
        bulletObject->addRenderComponent(stdRenderer);
        bulletObject->setDynamic(true);

        m_scene->addShadowCaster(bulletObject);
        m_shootSound->play();
    };

    createWalls();

    auto playerObject = std::make_shared<GameObject>(playerMesh, playerCollisionMesh);
    playerObject->addComponent(new PlayerController(spawnBullet, camera.get()));
    HealthComponent *playerHealth = new HealthComponent(2);
    allAlive.push_back(playerHealth);
    playerObject->addComponent(playerHealth);
    playerObject->setLocation(chag::make_vector(0.0f, 0.0f, 0.0f));
    StandardRenderer* stdrenderer = new StandardRenderer(playerMesh, standardShader);
    playerObject->addRenderComponent(stdrenderer);
    playerObject->setDynamic(true);
    playerObject->setIdentifier(PLAYER_IDENTIFIER);
    playerObject->addCollidesWith({DOOR_IDENTIFIER, ENEMY_SPAWNED_BULLET, WALL_IDENTIFIER, OBSTACLE_IDENTIFIER});
    m_scene->addShadowCaster(playerObject);
    //Enemy mesh
    auto monsterObject = std::make_shared<GameObject>(monsterMesh, playerCollisionMesh);
    HealthComponent *monsterHealth = new HealthComponent(2);
    monsterObject->addComponent(new EnemyComponent(spawnBullet, playerObject));
    allAlive.push_back(monsterHealth);
    monsterObject->addComponent(monsterHealth);
    monsterObject->setLocation(chag::make_vector(-8.0f, 0.0f, 8.0f));
    monsterObject->setRotation(chag::make_quaternion_axis_angle(chag::make_vector(0.0f,1.0f,0.0f), -6*M_PI/5));
    monsterObject->initializeModelMatrix();
    StandardRenderer *stdMonsterRenderer = new StandardRenderer(monsterMesh, standardShader);
    monsterObject->addRenderComponent(stdMonsterRenderer);
    monsterObject->setDynamic(true);
    monsterObject->setIdentifier(ENEMY_IDENTIFIER);
    monsterObject->addCollidesWith(PLAYER_SPAWNED_BULLET);

    m_scene->addShadowCaster(monsterObject);


    // Ground mesh
    auto floorMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/floor.obj");

    auto floorObject = std::make_shared<GameObject>(floorMesh);
    floorObject->setLocation(chag::make_vector(0.0f, 0.0f, 0.0f));
    StandardRenderer* stdFloorRenderer = new StandardRenderer(floorMesh, standardShader);
    floorObject->addRenderComponent(stdFloorRenderer);

    m_scene->addShadowCaster(floorObject);

    // Door mesh
    auto doorMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/door.obj");

    auto doorObject = std::make_shared<GameObject>(doorMesh);
    doorObject->setLocation(chag::make_vector(0.0f, 0.0f, 12.5f));
    StandardRenderer* stdDoorRenderer = new StandardRenderer(doorMesh, standardShader);
    doorObject->addRenderComponent(stdDoorRenderer);
    doorObject->addComponent(new WinOnCollisionComponent(m_scene));
    doorObject->setIdentifier(DOOR_IDENTIFIER);

    m_scene->addShadowCaster(doorObject);

    // Obstacle mesh
    auto obstacleMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/obstacle.obj");

    auto obstacleObject = std::make_shared<GameObject>(obstacleMesh);
    obstacleObject->setLocation(chag::make_vector(-5.0f, 0.0f, 5.0f));
    StandardRenderer* stdObstacleRenderer = new StandardRenderer(obstacleMesh, standardShader);
    obstacleObject->addRenderComponent(stdObstacleRenderer);
    obstacleObject->setIdentifier(OBSTACLE_IDENTIFIER);
    m_scene->addShadowCaster(obstacleObject);

    // HUD
    std::shared_ptr<GameObject> hudObj = std::make_shared<GameObject>();
    hudRenderer = new HudRenderer();
    hudRenderer->setWorldCamera(camera.get());
    hudObj->addRenderComponent(hudRenderer);
    HealthBar* playerHealthBar = new HealthBar(playerHealth);
    hudRenderer->addRelativeLayout(playerObject, playerHealthBar);
    HealthBar* monsterHealthBar = new HealthBar(monsterHealth);
    hudRenderer->addRelativeLayout(monsterObject, monsterHealthBar);

    m_scene->addTransparentObject(hudObj);

    createLight();
}

void Room::createWalls() {

    auto wallMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/wall.obj");
    auto upVect = chag::make_vector(0.0f, 1.0f, 0.0f);

    chag::SmallVector3<float> wallPos[] = {
        chag::make_vector(-12.5f,  0.0f,  12.5f),
        chag::make_vector(-12.5f,  0.0f,  12.5f),
        chag::make_vector( 12.5f,  0.0f, -12.5f),
        chag::make_vector( 12.5f,  0.0f, -12.5f)
    };

    for (int rotMult = 0; rotMult < 4; ++rotMult) {

        auto wallGob = std::make_shared<GameObject>(wallMesh, wallMesh);

        wallGob->setScale(chag::make_vector(25.0f, 4.0f, 1.0f));

        wallGob->setLocation(wallPos[rotMult]);
        auto rotationQuat = chag::make_quaternion_axis_angle(upVect, M_PI_2 * rotMult);
        wallGob->setRotation(rotationQuat);

        wallGob->setIdentifier(WALL_IDENTIFIER);
        m_scene->addShadowCaster(wallGob);
    }
}

void Room::update(float dt) {

    bool someoneDied = false;
    for(auto it = allAlive.begin(); it < allAlive.end(); it++){
        if((*it)->getHealth() <= 0){
            it = allAlive.erase(it);
            someoneDied = true;
        }
    }
    if(someoneDied) {
        hudRenderer->updateLayout();
    }
    m_scene->update(dt);
    m_collider->updateCollision(m_scene.get());

}

void Room::display(Renderer &renderer,
                   std::shared_ptr<Camera> camera,
                   float timeSinceStart,
                   float timeSinceLastCall)
{
    renderer.drawScene(camera.get(), m_scene.get(), timeSinceStart);
}

void Room::createLight() {

    DirectionalLight directionalLight = DirectionalLight();
    directionalLight.diffuseColor= chag::make_vector(0.50f,0.50f,0.50f);
    directionalLight.specularColor= chag::make_vector(0.50f,0.50f,0.50f);
    directionalLight.ambientColor= chag::make_vector(0.50f,0.50f,0.50f);

    directionalLight.direction= -chag::make_vector(0.0f,-10.0f,0.0f);
    m_scene->directionalLight = directionalLight;

    PointLight pointLight;
    pointLight.diffuseColor= chag::make_vector(0.50f,0.50f,0.50f);
    pointLight.specularColor= chag::make_vector(0.00f,0.00f,0.00f);
    pointLight.ambientColor= chag::make_vector(0.050f,0.050f,0.050f);
    pointLight.position = chag::make_vector(18.0f, 3.0f, 0.0f);
    m_scene->pointLights.push_back(pointLight);
}
