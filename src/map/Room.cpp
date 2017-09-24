

#include <ResourceManager.h>
#include <ObjectIdentifiers.h>
#include <ColliderFactory.h>
#include <ParticleRenderer.h>
#include <particles/FirstAttackParticle.h>
#include <ParticleGenerator.h>
#include <particles/SecondAttackParticle.h>
#include <constants.h>
#include <components/TimedLife.h>
#include <MoveComponent.h>
#include <components/DirtyBulletOnCollision.h>
#include <StandardRenderer.h>
#include <particles/BackgroundParticle.h>
#include <components/ChangeRoomOnCollision.h>
#include <components/PlayerController.h>
#include <ui/HealthBar.h>
#include <particles/TorchParticle.h>
#include <particles/SecondAttackParticle.h>
#include <components/BossController.h>
#include <Logger.h>
#include <ControlStatus.h>
#include <components/HealOnRevivePoint.h>
#include "Room.h"
#include "HallwayRoom.h"

Room::Room()
{
}

void Room::load(std::shared_ptr<TopDownCamera> camera, std::vector<std::shared_ptr<Player>> players, Direction enteredDirection) {

    if (isLoaded) {
        return;
    }
    isLoaded = true;

    m_collider = std::shared_ptr<Collider>(ColliderFactory::getTwoPhaseCollider());
    m_scene = std::make_shared<Scene>();
    this->camera = camera;

    loadHud(camera);
    loadWalls();
    loadFloor();
    loadBulletFunctions(camera);
    loadDoors();
    loadDirectionalLight();
    loadLights();
    loadPlayer(players, enteredDirection);
    loadGameObjects();
}

void Room::loadHud(const std::shared_ptr<TopDownCamera> &camera) {
    hudRenderer = new HudRenderer();
    hudRenderer->setWorldCamera(camera.get());
    std::shared_ptr<GameObject> hudObj = std::make_shared<GameObject>();
    hudObj->addRenderComponent(hudRenderer);
    m_scene->addTransparentObject(hudObj);
}

void Room::loadWalls() {

    auto wallMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/wall.obj");
    auto upVect = chag::make_vector(0.0f, 1.0f, 0.0f);

    chag::SmallVector3<float> wallPos[] = {
            chag::make_vector(-13.0f,  0.0f,  13.0f),
            chag::make_vector(-13.0f,  0.0f,  13.0f),
            chag::make_vector( 13.0f,  0.0f, -13.0f),
            chag::make_vector( 13.0f,  0.0f, -13.0f)
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

void Room::loadBulletFunctions(std::shared_ptr<TopDownCamera> camera) {
    m_shootSound = std::shared_ptr<sf::Sound>(ResourceManager::loadAndFetchSound("../assets/sound/shoot.wav"));
    m_blastSound = std::shared_ptr<sf::Sound>(ResourceManager::loadAndFetchSound("../assets/sound/blast.wav"));

    spawnBullet = [this, camera](std::weak_ptr<GameObject> shooter, std::shared_ptr<Texture> particleTexture) mutable {
        auto bulletObject = generateBulletBase(shooter);

        std::shared_ptr<ParticleRenderer> particleRenderer = std::make_shared<ParticleRenderer>(particleTexture, camera, ParticleRenderer::defaultShader());
        std::shared_ptr<FirstAttackParticle> backgroundParticleConf = std::make_shared<FirstAttackParticle>();

        ParticleGenerator *gen = new ParticleGenerator(200, particleRenderer, backgroundParticleConf, camera);
        auto particleGenerator = std::make_shared<GameObject>(bulletObject);
        particleGenerator->addRenderComponent(gen);
        particleGenerator->setLocation(chag::make_vector(0.0f, 0.0f, 0.0f));
        bulletObject->addChild(particleGenerator);
        particleGenerator->initializeModelMatrix();


        m_scene->addShadowCaster(bulletObject);
        m_shootSound->play();
    };

    spawnBlastBullet = [this, camera](std::weak_ptr<GameObject> shooter, std::shared_ptr<Texture> particleTexture) mutable {
        auto bulletObject = generateBulletBase(shooter);

        std::shared_ptr<ParticleRenderer> particleRenderer = std::make_shared<ParticleRenderer>(particleTexture, camera, ParticleRenderer::defaultShader());
        std::shared_ptr<SecondAttackParticle> backgroundParticleConf = std::make_shared<SecondAttackParticle>();

        ParticleGenerator *gen = new ParticleGenerator(200, particleRenderer, backgroundParticleConf, camera);
        auto particleGenerator = std::make_shared<GameObject>(bulletObject);
        particleGenerator->addRenderComponent(gen);
        particleGenerator->setLocation(chag::make_vector(0.0f, 0.0f, 0.0f));
        bulletObject->addChild(particleGenerator);
        particleGenerator->initializeModelMatrix();

        m_scene->addShadowCaster(bulletObject);
        m_blastSound->play();
    };
}

std::shared_ptr<GameObject> Room::generateBulletBase(std::weak_ptr<GameObject> shooter) {
    if(shooter.expired()) {
        return std::shared_ptr<GameObject>();
    }
    std::shared_ptr<GameObject> shooter_ptr = shooter.lock();

    chag::float4x4 rotationMatrix = chag::makematrix(shooter_ptr->getAbsoluteRotation());
    chag::float4 startDirection = chag::make_vector(0.0f, 0.0f, 1.0f, 0.0f);
    chag::float3 direction = chag::make_vector3(rotationMatrix * startDirection);

    auto standardShader = ResourceManager::loadAndFetchShaderProgram(SIMPLE_SHADER_NAME,
                                                                     "",
                                                                     "");
    auto bulletMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/bullet.obj");

    auto bulletObject = std::make_shared<GameObject>(bulletMesh);
    bulletObject->setLocation(shooter_ptr->getAbsoluteLocation() + chag::make_vector(0.0f, 2.0f, 0.0f));
    bulletObject->setRotation(shooter_ptr->getAbsoluteRotation());
    bulletObject->setScale(chag::make_vector(3.0f,3.0f,3.0f));
    bulletObject->addComponent(new TimedLife(20.0f));
    bulletObject->addComponent(new MoveComponent(
            chag::make_quaternion_axis_angle(chag::make_vector(0.0f, 1.0f, 0.0f), 0.0f),
            direction * 10,
            chag::make_vector(0.0f, 0.0f, 0.0f),
            chag::make_vector(0.0f, 0.0f, 0.0f)
    ));

    int shootId = 0;
    if (shooter_ptr->getIdentifier() == PLAYER_IDENTIFIER) {
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

    return bulletObject;
}

void Room::loadFloor() {
    auto standardShader = ResourceManager::loadAndFetchShaderProgram(SIMPLE_SHADER_NAME,
                                                                     "",
                                                                     "");
    auto floorMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/floor.obj");

    auto floorObject = std::make_shared<GameObject>(floorMesh);
    floorObject->setLocation(chag::make_vector(0.0f, 0.0f, 0.0f));
    StandardRenderer *stdFloorRenderer = new StandardRenderer(floorMesh, standardShader);
    floorObject->addRenderComponent(stdFloorRenderer);

    m_scene->addShadowCaster(floorObject);

    std::shared_ptr<Texture> particleTexture = ResourceManager::loadAndFetchTexture("../assets/meshes/fire.png");

    std::shared_ptr<ParticleRenderer> particleRenderer = std::make_shared<ParticleRenderer>(particleTexture, camera, ParticleRenderer::defaultShader());
    std::shared_ptr<BackgroundParticle> backgroundParticleConf = std::make_shared<BackgroundParticle>();

    ParticleGenerator *gen = new ParticleGenerator(200, particleRenderer, backgroundParticleConf, camera);
    auto particleGenerator = std::make_shared<GameObject>(floorObject);
    particleGenerator->addRenderComponent(gen);
    particleGenerator->setLocation(make_vector(4.0f, -20.0f, 12.5f));
    floorObject->addChild(particleGenerator);
    particleGenerator->initializeModelMatrix();
}

void Room::loadDoors() {
    auto standardShader = ResourceManager::loadAndFetchShaderProgram(SIMPLE_SHADER_NAME,
                                                                     "",
                                                                     "");
    auto doorMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/door.obj");

    for(auto door : doors){

        auto doorObject = std::make_shared<GameObject>(doorMesh);
        chag::float3 location;
        chag::Quaternion rotation;
        switch(door.first) {
            case UP:
                location = chag::make_vector(  0.0f, 0.0f,  12.5f); break;
            case DOWN:
                location = chag::make_vector(  0.0f, 0.0f, -12.5f); break;
            case RIGHT:
                rotation = chag::make_quaternion_axis_angle(chag::make_vector(0.0f, 1.0f, 0.0f), degreeToRad(90));
                location = chag::make_vector(-12.5f, 0.0f,   0.0f); break;
            case LEFT:
                rotation = chag::make_quaternion_axis_angle(chag::make_vector(0.0f, 1.0f, 0.0f), degreeToRad(90));
                location = chag::make_vector( 12.5f, 0.0f,   0.0f); break;
            case NEXT_LEVEL:
                location = chag::make_vector(  0.0f, 0.0f, 0.0f); break;
        }

        doorObject->setLocation(location);
        doorObject->setRotation(rotation);
        StandardRenderer *stdDoorRenderer = new StandardRenderer(doorMesh, standardShader);
        doorObject->addRenderComponent(stdDoorRenderer);
        doorObject->addComponent(new ChangeRoomOnCollision(m_scene, [door]() -> void { door.second(door.first); }));
        doorObject->setIdentifier(DOOR_IDENTIFIER);

        m_scene->addShadowCaster(doorObject);
    }

}

void Room::addDoor(Direction direction, walk_callback_t walkCallback) {
    doors.push_back(std::pair<Direction, walk_callback_t>(direction, walkCallback));
}

void Room::update(float dt, lose_callback_t loseCallback) {
    bool someoneDied = false;
    for (auto it = players.begin(); it < players.end();) {
        if((*it)->getHealth() <= 0){
            it = players.erase(it);
            someoneDied = true;
        } else {
            it++;
        }
    }

    for (auto it = enemies.begin(); it < enemies.end();) {
        if((*it)->getHealth() <= 0){
            it = enemies.erase(it);
            someoneDied = true;
        } else {
            it++;
        }

    }

    if(someoneDied) {
        hudRenderer->updateLayout();
    }

    if (players.empty()) {
        loseCallback();
    }

    m_scene->update(dt);
    m_collider->updateCollision(m_scene.get());
}

void Room::loadPlayer(std::vector<std::shared_ptr<Player>> players, Direction enteredDirection) {
    auto standardShader = ResourceManager::loadAndFetchShaderProgram(SIMPLE_SHADER_NAME,
                                                                     "",
                                                                     "");
    auto playerMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/bubba.obj");
    auto playerCollisionMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/bubba_collision.obj");

    float i = 0;

    for (auto player : players) {

        if (player->getHealthComponent()->getHealth() > 0) {

            std::shared_ptr<PointLight> pointLight = std::make_shared<PointLight>();
            pointLight->diffuseColor= chag::make_vector(0.50f,0.50f,0.20f);
            pointLight->specularColor= chag::make_vector(0.00f,0.00f,0.00f);
            pointLight->ambientColor= chag::make_vector(0.050f,0.050f,0.050f);
            pointLight->position = chag::make_vector(0.0f, 2.0f, 0.0f);
            Attenuation att;

            att.linear = 5;
            pointLight->attenuation = att;
            m_scene->pointLights.push_back(pointLight);

            std::shared_ptr<GameObject> playerObject = std::make_shared<GameObject>(playerMesh, playerCollisionMesh);
            playerObject->addComponent(new PlayerController(spawnBullet, spawnBlastBullet, camera.get(), pointLight,
                                                            player->getActivator()));

            auto playerHealth = player->getHealthComponent();
            this->players.push_back(playerHealth);
            playerObject->addComponent(playerHealth.get());

            playerObject->addComponent(new HealOnRevivePoint(playerHealth));

            switch (enteredDirection) {
                case Direction::UP:
                    playerObject->setLocation(chag::make_vector(i, 0.0f, -8.0f));
                    break;
                case Direction::DOWN:
                    playerObject->setLocation(chag::make_vector(-i, 0.0f, 8.0f));
                    break;
                case Direction::LEFT:
                    playerObject->setLocation(chag::make_vector(-8.0f, 0.0f, i));
                    break;
                case Direction::RIGHT:
                    playerObject->setLocation(chag::make_vector(8.0f, 0.0f, -i));
                    break;
                case Direction::NEXT_LEVEL:
                    playerObject->setLocation(chag::make_vector(-i, 0.0f, -8.0f));
                    break;
            }

            StandardRenderer *stdrenderer = new StandardRenderer(playerMesh, standardShader);
            playerObject->addRenderComponent(stdrenderer);
            playerObject->setDynamic(true);
            playerObject->setIdentifier(PLAYER_IDENTIFIER);
            playerObject->addCollidesWith({DOOR_IDENTIFIER, ENEMY_SPAWNED_BULLET, WALL_IDENTIFIER, OBSTACLE_IDENTIFIER});
            m_scene->addShadowCaster(playerObject);
            HealthBar *playerHealthBar = new HealthBar(player->getHealthComponent());
            hudRenderer->addRelativeLayout(playerObject, playerHealthBar);
            playerObjects.push_back(playerObject);

            i += 4;
        }

    }
}

void Room::createTorch(chag::float3 location)  {
    auto standardShader = ResourceManager::loadAndFetchShaderProgram(SIMPLE_SHADER_NAME,
                                                                     "",
                                                                     "");
    auto torchMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/torch.obj");

    std::shared_ptr<GameObject> torchObject= std::make_shared<GameObject>(torchMesh);
    torchObject->setLocation(location);
    torchObject->setScale(make_vector(5.0f, 5.0f, 5.0f));
    StandardRenderer *stdTorchRenderer = new StandardRenderer(torchMesh, standardShader);
    torchObject->addRenderComponent(stdTorchRenderer);
    torchObject->setIdentifier(OBSTACLE_IDENTIFIER);
    m_scene->addShadowCaster(torchObject);

    std::shared_ptr<Texture> particleTexture = ResourceManager::loadAndFetchTexture("../assets/meshes/fire.png");
    std::shared_ptr<ParticleRenderer> torchParticleRenderer = std::make_shared<ParticleRenderer>(particleTexture, camera, ParticleRenderer::defaultShader());
    std::shared_ptr<TorchParticle> torchParticleConf = std::make_shared<TorchParticle>();
    ParticleGenerator *torchParticleGenerator = new ParticleGenerator(500, torchParticleRenderer, torchParticleConf, camera);
    auto particleGeneratorObject = std::make_shared<GameObject>(torchObject);
    particleGeneratorObject->addRenderComponent(torchParticleGenerator);
    particleGeneratorObject->setLocation(make_vector(-0.06f, 0.0f, 0.0f));
    particleGeneratorObject->setScale(chag::make_vector(0.1f, 0.1f, 0.1f));
    torchObject->addChild(particleGeneratorObject);
    particleGeneratorObject->initializeModelMatrix();

    std::shared_ptr<PointLight> pointLight = std::make_shared<PointLight>();
    pointLight->diffuseColor= chag::make_vector(6.50f,3.50f,1.00f);
    pointLight->specularColor= chag::make_vector(0.05f,0.02f,0.005f);
    pointLight->ambientColor= chag::make_vector(0.350f,.350f,0.350f);
    pointLight->position = location + chag::make_vector(0.0f, 2.0f, 0.0f);
    Attenuation att;

    att.linear = 1;
    att.exp = 1;
    pointLight->attenuation = att;
    m_scene->pointLights.push_back(pointLight);
}

void Room::loadDirectionalLight() {
    std::shared_ptr<DirectionalLight> directionalLight = std::make_shared<DirectionalLight>();
    directionalLight->diffuseColor= chag::make_vector(0.050f,0.050f,0.050f);
    directionalLight->specularColor= chag::make_vector(0.050f,0.050f,0.050f);
    directionalLight->ambientColor= chag::make_vector(0.000f,0.000f,0.000f);

    directionalLight->direction= -chag::make_vector(0.0f,-10.0f,10.0f);
    m_scene->directionalLight = directionalLight;
}

const std::string Room::getMapSymbol() {
    return mapSymbol;
}

std::shared_ptr<Scene> Room::getScene() {
    return m_scene;
}
