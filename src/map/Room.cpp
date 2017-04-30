

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
#include <components/WinOnCollisionComponent.h>
#include <components/PlayerController.h>
#include <ui/HealthBar.h>
#include "Room.h"
#include "HallwayRoom.h"

void Room::load(std::shared_ptr<TopDownCamera> camera, HealthComponent *playerHealth, Direction enteredDirection) {

    if (isLoaded) {
        return;
    }
    isLoaded = true;

    m_collider = std::shared_ptr<Collider>(ColliderFactory::getTwoPhaseCollider());
    m_scene = std::make_shared<Scene>();

    hudRenderer = new HudRenderer();
    hudRenderer->setWorldCamera(camera.get());
    this->camera = camera;

    loadWalls();
    loadFloor();
    loadBulletFunctions(camera);
    loadDoors();
    loadLights();
    loadPlayer(playerHealth);
    loadGameObjects();

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
    m_shootSound = std::shared_ptr<sf::Sound>(AudioManager::loadAndFetchSound("../assets/sound/shoot.wav"));
    m_blastSound = std::shared_ptr<sf::Sound>(AudioManager::loadAndFetchSound("../assets/sound/blast.wav"));

    spawnBullet = [this, camera](GameObject* shooter, std::shared_ptr<Texture> particleTexture) mutable {
        auto bulletObject = generateBulletBase(shooter);

        std::shared_ptr<ParticleRenderer> particleRenderer = std::make_shared<ParticleRenderer>(particleTexture, camera, ParticleRenderer::defaultShader());
        std::shared_ptr<FirstAttackParticle> backgroundParticleConf = std::make_shared<FirstAttackParticle>();

        ParticleGenerator *gen = new ParticleGenerator(200, particleRenderer, backgroundParticleConf, camera);
        GameObject *particleGenerator = new GameObject(bulletObject.get());
        particleGenerator->addRenderComponent(gen);
        particleGenerator->setLocation(chag::make_vector(0.0f, 0.0f, 0.0f));
        bulletObject->addChild(particleGenerator);
        particleGenerator->initializeModelMatrix();

        m_scene->addShadowCaster(bulletObject);
        m_shootSound->play();
    };

    spawnBlastBullet = [this, camera](GameObject* shooter, std::shared_ptr<Texture> particleTexture) mutable {
        auto bulletObject = generateBulletBase(shooter);

        std::shared_ptr<ParticleRenderer> particleRenderer = std::make_shared<ParticleRenderer>(particleTexture, camera, ParticleRenderer::defaultShader());
        std::shared_ptr<SecondAttackParticle> backgroundParticleConf = std::make_shared<SecondAttackParticle>();

        ParticleGenerator *gen = new ParticleGenerator(200, particleRenderer, backgroundParticleConf, camera);
        GameObject *particleGenerator = new GameObject(bulletObject.get());
        particleGenerator->addRenderComponent(gen);
        particleGenerator->setLocation(chag::make_vector(0.0f, 0.0f, 0.0f));
        bulletObject->addChild(particleGenerator);
        particleGenerator->initializeModelMatrix();

        m_scene->addShadowCaster(bulletObject);
        m_blastSound->play();
    };
}

std::shared_ptr<GameObject> Room::generateBulletBase(GameObject* shooter) {
    if(shooter == nullptr) {
        return std::shared_ptr<GameObject>();
    }
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
    GameObject *particleGenerator = new GameObject(floorObject.get());
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
        }
        doorObject->setLocation(location);
        doorObject->setRotation(rotation);
        StandardRenderer *stdDoorRenderer = new StandardRenderer(doorMesh, standardShader);
        doorObject->addRenderComponent(stdDoorRenderer);
        doorObject->addComponent(new WinOnCollisionComponent(m_scene, [door]() -> void { door.second(door.first); }));
        doorObject->setIdentifier(DOOR_IDENTIFIER);

        m_scene->addShadowCaster(doorObject);
    }
}

void Room::addDoor(Direction direction, std::function<void(Direction direction)> callback) {
    doors.push_back(std::pair<Direction, std::function<void(Direction)>>(direction, callback));
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

void Room::display(Renderer &renderer, std::shared_ptr<Camera> camera, float timeSinceStart, float timeSinceLastCall) {
    renderer.drawScene(camera.get(), m_scene.get(), timeSinceStart);
}

void Room::loadPlayer(HealthComponent *playerHealth) {
    auto standardShader = ResourceManager::loadAndFetchShaderProgram(SIMPLE_SHADER_NAME,
                                                                     "",
                                                                     "");
    auto playerMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/bubba_animated.fbx");
    auto playerCollisionMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/bubba_collision.obj");

    std::shared_ptr<PointLight> pointLight = std::make_shared<PointLight>();
    pointLight->diffuseColor= chag::make_vector(0.50f,0.50f,0.20f);
    pointLight->specularColor= chag::make_vector(0.00f,0.00f,0.00f);
    pointLight->ambientColor= chag::make_vector(0.050f,0.050f,0.050f);
    pointLight->position = chag::make_vector(0.0f, 2.0f, 0.0f);
    Attenuation att;

    att.linear = 5;
    pointLight->attenuation = att;
    m_scene->pointLights.push_back(pointLight);

    playerObject = std::make_shared<GameObject>(playerMesh, playerCollisionMesh);
    playerObject->addComponent(new PlayerController(spawnBullet, spawnBlastBullet, camera.get(), pointLight));

    allAlive.push_back(playerHealth);
    playerObject->addComponent(playerHealth);
    playerObject->setLocation(chag::make_vector(0.0f, 0.0f, 0.0f));
    StandardRenderer *stdrenderer = new StandardRenderer(playerMesh, standardShader);
    playerObject->addRenderComponent(stdrenderer);
    playerObject->setDynamic(true);
    playerObject->setIdentifier(PLAYER_IDENTIFIER);
    playerObject->addCollidesWith({DOOR_IDENTIFIER, ENEMY_SPAWNED_BULLET, WALL_IDENTIFIER, OBSTACLE_IDENTIFIER});
    m_scene->addShadowCaster(playerObject);
    HealthBar* playerHealthBar = new HealthBar(playerHealth);
    hudRenderer->addRelativeLayout(playerObject, playerHealthBar);
}

