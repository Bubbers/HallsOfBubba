
#include <constants.h>
#include <Window.h>
#include <Logger.h>
#include <StdOutLogHandler.h>
#include <ControlsManager.h>
#include <KeyboardButton.h>
#include <MouseButton.h>
#include <Renderer.h>
#include <map/RoomGraph.h>
#include <components/HealthComponent.h>
#include "cameras/TopDownCamera.h"
#include "map/Room.h"
#include "controls.h"
#include "ObjectIdentifiers.h"

Renderer renderer;
std::shared_ptr<Room> room;
std::shared_ptr<RoomGraph> roomGraph;
HealthComponent* playerHealth;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

std::shared_ptr<TopDownCamera> camera;

void idle(float timeSinceStart,float timeSinceLastCall) {
    room->update(timeSinceLastCall);
}

// Called by the window mainloop
void display(float timeSinceStart,float timeSinceLastCall) {
    room->display(renderer, camera, timeSinceStart, timeSinceLastCall);
}

// Called when the window gets resized
void resize(int newWidth, int newHeight) {
    renderer.resize(newWidth, newHeight);
}

void createKeyListeners() {
    ControlsManager* cm = ControlsManager::getInstance();
    cm->addBinding(MOVE_HORIZONTAL, new KeyboardButton(sf::Keyboard::A, sf::Keyboard::D));
    cm->addBinding(MOVE_VERTICAL, new KeyboardButton(sf::Keyboard::W, sf::Keyboard::S));
    cm->addBinding(SHOOT_BUTTON_LMB, new MouseButton(sf::Mouse::Left));
    cm->addBinding(SHOOT_BUTTON_RMB, new MouseButton(sf::Mouse::Right));
}

void walk(Direction direction){
    roomGraph->walk(direction);
    room = roomGraph->getCurrentRoom();
    room->load(camera, playerHealth, direction);
}

int main() {
    srand(time(NULL));

    Logger::addLogHandler(new StdOutLogHandler());
    Logger::setLogLevel(LogLevel::INFO);

    Window* win = new Window(SCREEN_WIDTH, SCREEN_HEIGHT, "Halls of Bubba");

    // Setup callbacks for window events
    win->setResizeMethod(resize);
    win->setIdleMethod(idle);
    win->setDisplayMethod(display);

    renderer.setBackgroundColor(chag::make_vector(0.0f,0.0f,0.0f));
    renderer.initRenderer(SCREEN_WIDTH, SCREEN_HEIGHT);

    camera = std::make_shared<TopDownCamera>(chag::make_vector(0.0f, 0.0f, 0.0f),
                                             SCREEN_WIDTH,
                                             SCREEN_HEIGHT);

    playerHealth = new HealthComponent(2);
    roomGraph = std::make_shared<RoomGraph>(walk);
    room = roomGraph->getCurrentRoom();
    room->load(camera, playerHealth, Direction::UP);

    createKeyListeners();

    win->start(60);

    return 0;
}
