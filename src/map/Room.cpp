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
#include <particles/SecondAttackParticle.h>
#include "Room.h"


Room::Room(bool includeObstacle) : includeObstacle(includeObstacle){

}


Room::~Room()
{
}

void Room::load(std::shared_ptr<TopDownCamera> camera, HealthComponent* playerHealth, Direction enteredDirection) {

    if(isLoaded){
        return;
    }

    m_collider = std::shared_ptr<Collider>(ColliderFactory::getTwoPhaseCollider());

    auto standardShader = ResourceManager::loadAndFetchShaderProgram(SIMPLE_SHADER_NAME,
                                                                     "",
                                                                     "");

    m_shootSound = std::shared_ptr<sf::Sound>(AudioManager::loadAndFetchSound("../assets/sound/shoot.wav"));
    auto playerMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/bubba.obj");
    auto playerCollisionMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/bubba_collision.obj");
    auto monsterMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/monster.obj");

    m_scene = std::make_shared<Scene>();
    auto spawnBullet = [&](GameObject *shooter, std::shared_ptr<Texture> particleTexture) mutable {
        chag::float4x4 rotationMatrix = chag::makematrix(shooter->getAbsoluteRotation());
        chag::float4 startDirection = chag::make_vector(0.0f, 0.0f, 1.0f, 0.0f);
        chag::float3 direction = chag::make_vector3(rotationMatrix * startDirection);

        auto standardShader = ResourceManager::loadAndFetchShaderProgram(SIMPLE_SHADER_NAME,
                                                                         "",
                                                                         "");
        auto bulletMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/bullet.obj");

        auto bulletObject = std::make_shared<GameObject>(bulletMesh);
        bulletObject->setLocation(shooter->getAbsoluteLocation());
        bulletObject->setRotation(shooter->getAbsoluteRotation());
        bulletObject->setScale(chag::make_vector(3.0f, 3.0f, 3.0f));
        bulletObject->addComponent(new TimedLife(20.0f));
        bulletObject->addComponent(new MoveComponent(
                chag::make_quaternion_axis_angle(chag::make_vector(0.0f, 1.0f, 0.0f), 0.0f),
                direction * 10,
                chag::make_vector(0.0f, 0.0f, 0.0f),
                chag::make_vector(0.0f, 0.0f, 0.0f)
        ));


        std::shared_ptr<ParticleRenderer> particleRenderer = std::make_shared<ParticleRenderer>(particleTexture, camera, ParticleRenderer::defaultShader());
        std::shared_ptr<SecondAttackParticle> backgroundParticleConf = std::make_shared<SecondAttackParticle>();

        ParticleGenerator *gen = new ParticleGenerator(200, particleRenderer, backgroundParticleConf, camera);
        GameObject *particleGenerator = new GameObject(bulletObject.get());
        particleGenerator->addRenderComponent(gen);
        particleGenerator->setLocation(make_vector(0.0f, 0.0f, 0.0f));
        bulletObject->addChild(particleGenerator);
        particleGenerator->initializeModelMatrix();

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
        StandardRenderer *stdRenderer = new StandardRenderer(bulletMesh, standardShader);
        bulletObject->addRenderComponent(stdRenderer);
        bulletObject->setDynamic(true);

        m_scene->addShadowCaster(bulletObject);
        m_shootSound->play();
    };

    createWalls();

    auto playerObject = std::make_shared<GameObject>(playerMesh, playerCollisionMesh);
    playerObject->addComponent(new PlayerController(spawnBullet, camera.get()));
    allAlive.push_back(playerHealth);
    playerObject->addComponent(playerHealth);
    playerObject->setLocation(chag::make_vector(0.0f, 0.0f, 0.0f));
    StandardRenderer *stdrenderer = new StandardRenderer(playerMesh, standardShader);
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
    monsterObject->setRotation(chag::make_quaternion_axis_angle(chag::make_vector(0.0f, 1.0f, 0.0f), -6 * M_PI / 5));
    monsterObject->initializeModelMatrix();
    StandardRenderer *stdMonsterRenderer = new StandardRenderer(monsterMesh, standardShader);
    monsterObject->addRenderComponent(stdMonsterRenderer);
    monsterObject->setDynamic(true);
    monsterObject->setIdentifier(ENEMY_IDENTIFIER);
    monsterObject->addCollidesWith({PLAYER_SPAWNED_BULLET, WALL_IDENTIFIER, OBSTACLE_IDENTIFIER});

    m_scene->addShadowCaster(monsterObject);

    //Enemy mesh2
    auto monsterObject2 = std::make_shared<GameObject>(monsterMesh, playerCollisionMesh);
    HealthComponent *monsterHealth2 = new HealthComponent(2);
    monsterObject2->addComponent(new EnemyComponent(spawnBullet, playerObject));
    allAlive.push_back(monsterHealth2);
    monsterObject2->addComponent(monsterHealth2);
    monsterObject2->setLocation(chag::make_vector(8.0f, 0.0f, 8.0f));
    monsterObject2->setRotation(chag::make_quaternion_axis_angle(chag::make_vector(0.0f, 1.0f, 0.0f), -6 * M_PI / 5));
    monsterObject2->initializeModelMatrix();
    StandardRenderer *stdMonsterRenderer2 = new StandardRenderer(monsterMesh, standardShader);
    monsterObject2->addRenderComponent(stdMonsterRenderer2);
    monsterObject2->setDynamic(true);
    monsterObject2->setIdentifier(ENEMY_IDENTIFIER);
    monsterObject2->addCollidesWith({PLAYER_SPAWNED_BULLET, WALL_IDENTIFIER, OBSTACLE_IDENTIFIER});

    m_scene->addShadowCaster(monsterObject2);

    // Ground mesh
    auto floorMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/floor.obj");

    auto floorObject = std::make_shared<GameObject>(floorMesh);
    floorObject->setLocation(chag::make_vector(0.0f, 0.0f, 0.0f));
    StandardRenderer *stdFloorRenderer = new StandardRenderer(floorMesh, standardShader);
    floorObject->addRenderComponent(stdFloorRenderer);

    m_scene->addShadowCaster(floorObject);

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

    auto torchMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/torch.obj");

    auto torchObject = std::make_shared<GameObject>(torchMesh);
    torchObject->setLocation(chag::make_vector(-11.0f, 0.0f, -11.0f));
    torchObject->setScale(chag::make_vector(5.0f, 5.0f, 5.0f));
    StandardRenderer *stdTorchRenderer = new StandardRenderer(torchMesh, standardShader);
    torchObject->addRenderComponent(stdTorchRenderer);
    torchObject->setIdentifier(OBSTACLE_IDENTIFIER);
    m_scene->addShadowCaster(torchObject);

    auto torchObject2 = std::make_shared<GameObject>(torchMesh);
    torchObject2->setLocation(chag::make_vector(11.0f, 0.0f, -11.0f));
    torchObject2->setScale(chag::make_vector(5.0f, 5.0f, 5.0f));
    StandardRenderer *stdTorchRenderer2 = new StandardRenderer(torchMesh, standardShader);
    torchObject2->addRenderComponent(stdTorchRenderer2);
    torchObject2->setIdentifier(OBSTACLE_IDENTIFIER);
    m_scene->addShadowCaster(torchObject2);

    // Door mesh
    auto doorMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/door.obj");

    for(auto door : doors){

        auto doorObject = std::make_shared<GameObject>(doorMesh);
        chag::float3 location;
        switch(door.first) {
            case UP:
                location = chag::make_vector(  0.0f, 0.0f,  12.5f); break;
            case DOWN:
                location = chag::make_vector(  0.0f, 0.0f, -12.5f); break;
            case RIGHT:
                location = chag::make_vector(-12.5f, 0.0f,   0.0f); break;
            case LEFT:
                location = chag::make_vector( 12.5f, 0.0f,   0.0f); break;
        }
        doorObject->setLocation(location);
        StandardRenderer *stdDoorRenderer = new StandardRenderer(doorMesh, standardShader);
        doorObject->addRenderComponent(stdDoorRenderer);
        doorObject->addComponent(new WinOnCollisionComponent(m_scene, [door]() -> void { door.second(door.first); }));
        doorObject->setIdentifier(DOOR_IDENTIFIER);

        m_scene->addShadowCaster(doorObject);
    }

    // HUD
    std::shared_ptr<GameObject> hudObj = std::make_shared<GameObject>();
    hudRenderer = new HudRenderer();
    hudRenderer->setWorldCamera(camera.get());
    hudObj->addRenderComponent(hudRenderer);
    HealthBar* playerHealthBar = new HealthBar(playerHealth);
    hudRenderer->addRelativeLayout(playerObject, playerHealthBar);
    HealthBar* monsterHealthBar = new HealthBar(monsterHealth);
    hudRenderer->addRelativeLayout(monsterObject, monsterHealthBar);
    HealthBar* monsterHealthBar2 = new HealthBar(monsterHealth2);
    hudRenderer->addRelativeLayout(monsterObject2, monsterHealthBar2);

    m_scene->addTransparentObject(hudObj);

    createLight();

    // Cool background effect
    std::shared_ptr<Texture> particleTexture = ResourceManager::loadAndFetchTexture("../assets/meshes/fire.png");

    std::shared_ptr<ParticleRenderer> particleRenderer = std::make_shared<ParticleRenderer>(particleTexture, camera, ParticleRenderer::defaultShader());
    std::shared_ptr<BackgroundParticle> backgroundParticleConf = std::make_shared<BackgroundParticle>();

    ParticleGenerator *gen = new ParticleGenerator(200, particleRenderer, backgroundParticleConf, camera);
    GameObject *particleGenerator = new GameObject(floorObject.get());
    particleGenerator->addRenderComponent(gen);
    particleGenerator->setLocation(make_vector(4.0f, -20.0f, 12.5f));
    floorObject->addChild(particleGenerator);
    particleGenerator->initializeModelMatrix();

    // torch particles
    std::shared_ptr<ParticleRenderer> torchParticleRenderer = std::make_shared<ParticleRenderer>(particleTexture, camera, ParticleRenderer::defaultShader());
    std::shared_ptr<TorchParticle> torchParticleConf = std::make_shared<TorchParticle>();

    ParticleGenerator *torchParticleGenerator = new ParticleGenerator(500, torchParticleRenderer, torchParticleConf, camera);
    GameObject *particleGeneratorObject = new GameObject(torchObject.get());
    particleGeneratorObject->addRenderComponent(torchParticleGenerator);
    particleGeneratorObject->setLocation(make_vector(0.0f, 0.0f, 0.0f));
    particleGeneratorObject->setScale(chag::make_vector(0.1f, 0.1f, 0.1f));
    torchObject->addChild(particleGeneratorObject);
    particleGeneratorObject->initializeModelMatrix();

    ParticleGenerator *torchParticleGenerator2 = new ParticleGenerator(500, torchParticleRenderer, torchParticleConf, camera);
    GameObject *particleGeneratorObject2 = new GameObject(torchObject2.get());
    particleGeneratorObject2->addRenderComponent(torchParticleGenerator2);
    particleGeneratorObject2->setLocation(make_vector(0.0f, 0.0f, 0.0f));
    particleGeneratorObject2->setScale(chag::make_vector(0.1f, 0.1f, 0.1f));
    torchObject2->addChild(particleGeneratorObject2);
    particleGeneratorObject2->initializeModelMatrix();

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

        wallGob->setScale(chag::make_vector(30.0f, 4.0f, 1.0f));

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
    directionalLight.diffuseColor= chag::make_vector(0.550f,0.550f,0.550f);
    directionalLight.specularColor= chag::make_vector(0.050f,0.050f,0.050f);
    directionalLight.ambientColor= chag::make_vector(0.050f,0.050f,0.050f);

    directionalLight.direction= -chag::make_vector(0.0f,-10.0f,0.0f);
    m_scene->directionalLight = directionalLight;

    PointLight pointLight;
    pointLight.diffuseColor= chag::make_vector(5.50f,0.50f,0.50f);
    pointLight.specularColor= chag::make_vector(0.00f,0.00f,0.00f);
    pointLight.ambientColor= chag::make_vector(0.050f,0.050f,0.050f);
    pointLight.position = chag::make_vector(-11.0f, 2.0f, -11.0f);
    Attenuation att;

    att.linear = 2;
    pointLight.attenuation = att;
    m_scene->pointLights.push_back(pointLight);

    PointLight pointLight2;
    pointLight2.diffuseColor= chag::make_vector(5.50f,0.50f,0.50f);
    pointLight2.specularColor= chag::make_vector(0.00f,0.00f,0.00f);
    pointLight2.ambientColor= chag::make_vector(0.050f,0.050f,0.050f);
    pointLight2.position = chag::make_vector(11.0f, 2.0f, -11.0f);
    pointLight2.attenuation = att;
    m_scene->pointLights.push_back(pointLight2);
}

void Room::addDoor(Direction direction, std::function<void(Direction direction)> callback) {

    doors.push_back(std::pair<Direction, std::function<void(Direction)>>(direction, callback));

}
