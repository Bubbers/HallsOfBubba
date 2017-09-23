
#include <Window.h>
#include <Logger.h>
#include <StdOutLogHandler.h>
#include <ControlsManager.h>
#include <KeyboardButton.h>
#include <MouseButton.h>
#include <Renderer.h>
#include <map/RoomGraph.h>
#include <JoystickAxis.h>
#include <JoystickButton.h>
#include <JoystickTranslator.h>
#include <ResourceManager.h>
#include <statemachine/StateMachine.h>
#include <actors/Player.h>
#include "controls.h"

Renderer renderer;
std::shared_ptr<Room> room;
std::shared_ptr<RoomGraph> roomGraph;
std::vector<std::shared_ptr<Player>> players;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

std::shared_ptr<TopDownCamera> camera;

void idle(float timeSinceStart,float timeSinceLastCall) {
    room->update(timeSinceLastCall);
    ResourceManager::update();
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
    JoystickTranslator::getInstance()->init("../assets/controls.json");
    ControlsManager* cm = ControlsManager::getInstance();
    cm->addBindings(MOVE_HORIZONTAL, {new KeyboardButton(sf::Keyboard::A, sf::Keyboard::D),
                                      new JoystickAxis(IJoystickTranslation::Axis::LEFT_THUMBSTICK_X, true)});
    cm->addBindings(MOVE_VERTICAL, {new KeyboardButton(sf::Keyboard::W, sf::Keyboard::S),
                                    new JoystickAxis(IJoystickTranslation::Axis::LEFT_THUMBSTICK_Y, true)});
    cm->addBindings(SHOOT_BUTTON_LMB, {new MouseButton(sf::Mouse::Left), new JoystickAxis(IJoystickTranslation::Axis::RT, false)});
    cm->addBindings(SHOOT_BUTTON_RMB, {new MouseButton(sf::Mouse::Right), new JoystickButton(IJoystickTranslation::Button::RB)});
    cm->addBinding(AIM_HORIZONTAL, new JoystickAxis(IJoystickTranslation::Axis::RIGHT_THUMBSTICK_X, true));
    cm->addBinding(AIM_VERTICAL, new JoystickAxis(IJoystickTranslation::Axis::RIGHT_THUMBSTICK_Y, true));
}

void walk(Direction direction){
    roomGraph->walk(direction);
    room = roomGraph->getCurrentRoom();
    room->load(camera, players, direction);
}

int main() {
    srand(time(NULL));

    const StateKey INACTIVE = 0;
    const StateKey ACTIVE = 1;

    StateMachine statemachine({INACTIVE, ACTIVE});

    statemachine.connect(INACTIVE, ACTIVE, []() {
        Logger::logDebug("New game");
    });

    statemachine.transitionToState(ACTIVE);

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

    ControlStatus::Activator activator = (ControlStatus::Activator)(ControlStatus::Activator::KEYBOARD | ControlStatus::Activator::MOUSE);
    players.push_back(std::make_shared<Player>(activator));
    players.push_back(std::make_shared<Player>(ControlStatus::Activator::JOYSTICK));
    roomGraph = std::make_shared<RoomGraph>(walk);
    room = roomGraph->getCurrentRoom();
    room->load(camera, players, Direction::UP);

    createKeyListeners();

    win->start(60);

    return 0;
}
