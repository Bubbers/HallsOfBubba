
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
#include <ui/Menu.h>
#include <ui/Stats.h>
#include "controls.h"
#include <GameObject.h>

Renderer renderer;
std::shared_ptr<RoomGraph> roomGraph;
std::vector<std::shared_ptr<Player>> players;

const StateKey INACTIVE = 0;
const StateKey ACTIVE = 1;
const StateKey MENU = 2;
std::shared_ptr<StateMachine> statemachine = std::shared_ptr<StateMachine>(new StateMachine({MENU, INACTIVE, ACTIVE}));

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int numPlayers = 1;

std::shared_ptr<TopDownCamera> camera;
std::shared_ptr<Scene> currentScene;

void winCallback() {
    statemachine->queueTransition(INACTIVE);
    statemachine->queueTransition(MENU);
};

void loseCallback() {
    statemachine->queueTransition(INACTIVE);
    statemachine->queueTransition(MENU);
};

void idle(float timeSinceStart, float timeSinceLastCall) {
    statemachine->doTransitions();

    if(roomGraph) {
        roomGraph->getCurrentRoom()->update(timeSinceLastCall, loseCallback);
    } else { // currentScene = menu scene if no room is active
        currentScene->update(timeSinceLastCall);
    }

#ifdef __linux__
    ResourceManager::update();
#endif
}

// Called by the window mainloop
void display(float timeSinceStart,float timeSinceLastCall) {
    renderer.drawScene(camera.get(), currentScene.get(), timeSinceStart);
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

void walk(Direction direction) {
    if (direction == NEXT_LEVEL) {
        roomGraph->nextLevel(winCallback);
    } else {
        roomGraph->walk(direction);
    }
    roomGraph->getCurrentRoom()->load(camera, players, direction);
    currentScene = roomGraph->getCurrentRoom()->getScene();
}

void initMenu() {
    currentScene = std::make_shared<Scene>();
    std::shared_ptr<GameObject> hudObj = std::make_shared<GameObject>();
    HudRenderer *hudRenderer = new HudRenderer();
    hudRenderer->setLayout(new Menu([](int newNumPlayers){
        players.clear();
        numPlayers = newNumPlayers;
        statemachine->queueTransition(ACTIVE);
    }));
    hudObj->addRenderComponent(hudRenderer);
    currentScene->addTransparentObject(hudObj);
    roomGraph = nullptr;
}

unsigned int playTime = 0;
unsigned int points = 0;

void initStats() {
    currentScene = std::make_shared<Scene>();
    std::shared_ptr<GameObject> hudObj = std::make_shared<GameObject>();
    HudRenderer *hudRenderer = new HudRenderer();
    hudRenderer->setLayout(new Stats(playTime, points, [](){
        statemachine->queueTransition(MENU);
    }));
    hudObj->addRenderComponent(hudRenderer);
    currentScene->addTransparentObject(hudObj);
    roomGraph = nullptr;
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

    std::function<void()> initGame = []() {
        Logger::logInfo("New game");

        playTime = 0;
        points = 1337;

        if(numPlayers == 1) {
            players.push_back(std::make_shared<Player>(ControlStatus::Activator::ALL));
        } else if (numPlayers == 2) {
            ControlStatus::Activator activator = (ControlStatus::Activator)
                    (ControlStatus::KEYBOARD | ControlStatus::MOUSE);
            players.push_back(std::make_shared<Player>(activator));
            players.push_back(std::make_shared<Player>(ControlStatus::Activator::JOYSTICK));
        } else {
            throw std::invalid_argument("Only one or two players supported.");
        }

        roomGraph = std::make_shared<RoomGraph>(walk);
        roomGraph->getCurrentRoom()->load(camera, players, Direction::UP);
        currentScene = roomGraph->getCurrentRoom()->getScene();
    };

    statemachine->connect(MENU, ACTIVE, initGame);

    statemachine->connect(INACTIVE, MENU, initMenu);

    statemachine->connect(ACTIVE, INACTIVE, [&]() {
        Logger::logInfo("Stop game");
        initStats();
    });

    createKeyListeners();

    initMenu();

    win->start(60);

    return 0;
}

