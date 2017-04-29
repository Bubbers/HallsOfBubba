
#include <Renderer.h>
#include <cameras/TopDownCamera.h>
#include <constants.h>
#include <ResourceManager.h>
#include <StandardRenderer.h>
#include <Scene.h>
#include <Window.h>
#include <ColliderFactory.h>
#include <components/WinOnCollisionComponent.h>
#include <Logger.h>
#include <StdOutLogHandler.h>
#include <ControlsManager.h>
#include <KeyboardButton.h>
#include <components/PlayerController.h>
#include <components/HealthComponent.h>
#include <MouseButton.h>
#include "controls.h"
#include "ObjectIdentifiers.h"

Renderer renderer;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

TopDownCamera *camera;
std::shared_ptr<Scene> scene;
Collider *collider = ColliderFactory::getTwoPhaseCollider();

void idle(float timeSinceStart,float timeSinceLastCall) {

    scene->update(timeSinceLastCall);
    collider->updateCollision(scene.get());

}

// Called by the window mainloop
void display(float timeSinceStart,float timeSinceLastCall) {
    renderer.drawScene(camera, scene.get(), timeSinceStart);
}

// Called when the window gets resized
void resize(int newWidth, int newHeight) {
    renderer.resize(newWidth, newHeight);
}

void spawnBullet(chag::float3 direction, chag::float3 position){
    std::shared_ptr<ShaderProgram> standardShader = ResourceManager::loadAndFetchShaderProgram(SIMPLE_SHADER_NAME, "", "");
    std::shared_ptr<Mesh> bulletMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/bubba.obj");

    std::shared_ptr<GameObject> bulletObject = std::make_shared<GameObject>(bulletMesh);
    bulletObject->setLocation(position);
    bulletObject->setScale(chag::make_vector(0.1f, 0.1f, 0.1f));
    bulletObject->addComponent(new MoveComponent(
            chag::make_quaternion_axis_angle(chag::make_vector(0.0f, 1.0f, 0.0f), 0.0f),
            direction,
            chag::make_vector(0.0f, 0.0f, 0.0f),
            chag::make_vector(0.0f, 0.0f, 0.0f)
    ));
    bulletObject->setIdentifier(ENEMY_SPAWNED_BULLET);
    StandardRenderer* stdRenderer = new StandardRenderer(bulletMesh, standardShader);
    bulletObject->addRenderComponent(stdRenderer);

    scene->addShadowCaster(bulletObject);
}

void createLight() {

    DirectionalLight directionalLight = DirectionalLight();
    directionalLight.diffuseColor= chag::make_vector(0.50f,0.50f,0.50f);
    directionalLight.specularColor= chag::make_vector(0.50f,0.50f,0.50f);
    directionalLight.ambientColor= chag::make_vector(0.50f,0.50f,0.50f);

    directionalLight.direction= -chag::make_vector(0.0f,-10.0f,0.0f);
    scene->directionalLight = directionalLight;

    PointLight pointLight;
    pointLight.diffuseColor= chag::make_vector(0.50f,0.50f,0.50f);
    pointLight.specularColor= chag::make_vector(0.00f,0.00f,0.00f);
    pointLight.ambientColor= chag::make_vector(0.050f,0.050f,0.050f);
    pointLight.position = chag::make_vector(18.0f, 3.0f, 0.0f);
    scene->pointLights.push_back(pointLight);
}

void loadWorld() {
    std::shared_ptr<ShaderProgram> standardShader = ResourceManager::loadAndFetchShaderProgram(SIMPLE_SHADER_NAME, "", "");
    std::shared_ptr<Mesh> playerMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/bubba.obj");

    std::shared_ptr<GameObject> playerObject = std::make_shared<GameObject>(playerMesh);
    playerObject->addComponent(new PlayerController(spawnBullet));
    playerObject->addComponent(new HealthComponent());
    playerObject->setLocation(chag::make_vector(0.0f, 0.0f, 0.0f));
    StandardRenderer* stdrenderer = new StandardRenderer(playerMesh, standardShader);
    playerObject->addRenderComponent(stdrenderer);
    playerObject->setDynamic(true);
    playerObject->setIdentifier(PLAYER_IDENTIFIER);
    playerObject->addCollidesWith(DOOR_IDENTIFIER);
    playerObject->addCollidesWith(ENEMY_SPAWNED_BULLET);

    scene = std::make_shared<Scene>();
    scene->addShadowCaster(playerObject);

    // Ground mesh
    std::shared_ptr<Mesh> floorMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/floor.obj");

    std::shared_ptr<GameObject> floorObject = std::make_shared<GameObject>(floorMesh);
    floorObject->setLocation(chag::make_vector(0.0f, 0.0f, 0.0f));
    StandardRenderer* stdFloorRenderer = new StandardRenderer(floorMesh, standardShader);
    floorObject->addRenderComponent(stdFloorRenderer);

    scene->addShadowCaster(floorObject);

    // Door mesh
    std::shared_ptr<Mesh> doorMesh = ResourceManager::loadAndFetchMesh("../assets/meshes/door.obj");

    std::shared_ptr<GameObject> doorObject = std::make_shared<GameObject>(doorMesh);
    doorObject->setLocation(chag::make_vector(0.0f, 0.0f, 12.5f));
    StandardRenderer* stdDoorRenderer = new StandardRenderer(doorMesh, standardShader);
    doorObject->addRenderComponent(stdDoorRenderer);
    doorObject->addComponent(new WinOnCollisionComponent());
    doorObject->setIdentifier(DOOR_IDENTIFIER);

    scene->addShadowCaster(doorObject);
}

void createKeyListeners() {
    ControlsManager* cm = ControlsManager::getInstance();
    cm->addBinding(MOVE_HORIZONTAL, new KeyboardButton(sf::Keyboard::A, sf::Keyboard::D));
    cm->addBinding(MOVE_VERTICAL, new KeyboardButton(sf::Keyboard::W, sf::Keyboard::S));
    cm->addBinding(SHOOT_BUTTON, new MouseButton(sf::Mouse::Left));
}

int main() {

    Logger::addLogHandler(new StdOutLogHandler());
    Logger::setLogLevel(LogLevel::INFO);

    Window* win = new Window(SCREEN_WIDTH, SCREEN_HEIGHT, "Halls of Bubba");

    // Setup callbacks for window events
    win->setResizeMethod(resize);
    win->setIdleMethod(idle);
    win->setDisplayMethod(display);

    renderer.initRenderer(SCREEN_WIDTH, SCREEN_HEIGHT);

    camera = new TopDownCamera(chag::make_vector(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);

    loadWorld();
    createLight();
    createKeyListeners();

    win->start(60);

    return 0;
}
