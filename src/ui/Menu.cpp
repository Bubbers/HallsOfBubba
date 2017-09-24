#include <ListLayout.h>
#include <TextLayout.h>
#include <FontManager.h>
#include <HUDGraphic.h>
#include <ResourceManager.h>
#include "Menu.h"

Menu::Menu(playersPickedCallback_t playersPickedCallback)
        : playersPickedCallback(playersPickedCallback), PositioningLayout(Dimension::fromPercentage(100), Dimension::fromPercentage(100)){

    std::shared_ptr<Texture> bg = ResourceManager::loadAndFetchTexture("../assets/meshes/brick_wall_final.jpg");
    std::shared_ptr<Texture> buttonBg = ResourceManager::loadAndFetchTexture("../assets/meshes/button.png");
    std::shared_ptr<Texture> buttonHoverBg = ResourceManager::loadAndFetchTexture("../assets/meshes/button_hover.png");
    setBackground(new HUDGraphic(bg.get()));
    
    Font* ubuntuFont28 = FontManager::getInstance()->loadAndFetchFont("../fonts/Ubuntu-M.ttf", 28);

    TextLayout *titleLayout = new TextLayout("Halls of Bubba", ubuntuFont28, Dimension::fromPixels(200), Dimension::fromPixels(32));
    addChild(titleLayout, Dimension::fromPercentagePlusPixels(50, -100), Dimension::fromPercentage(10));

    Font* ubuntuFont20 = FontManager::getInstance()->loadAndFetchFont("../fonts/Ubuntu-M.ttf", 20);

    TextLayout *choosePlayersText = new TextLayout("Choose number of players.", ubuntuFont20, Dimension::fromPixels(300), Dimension::fromPixels(24));
    addChild(choosePlayersText, Dimension::fromPercentagePlusPixels(50, -150), Dimension::fromPercentage(20));

    Layout *player1Picker = createPlayerNumberPicker(ubuntuFont20, 1, buttonBg, buttonHoverBg);
    Layout *player2Picker = createPlayerNumberPicker(ubuntuFont20, 2, buttonBg, buttonHoverBg);

    ListLayout *buttonList = new ListLayout(ListLayout::Orientation::HORIZONTAL, Dimension::fromPixels(300), Dimension::fromPixels(30));
    buttonList->addChild(player1Picker);
    buttonList->addChild(player2Picker);

    addChild(buttonList, Dimension::fromPercentagePlusPixels(50, -150), Dimension::fromPercentagePlusPixels(50, -15));

}

Layout * Menu::createPlayerNumberPicker(Font *ubuntuFont20, int players, std::shared_ptr<Texture> buttonBg,
                                            std::shared_ptr<Texture> buttonHoverBg) {
    Layout *playerButton = new TextLayout(std::to_string(players) + " Player", ubuntuFont20,
                                              Dimension::fromPixels(150), Dimension::fromPixels(30));
    playerButton->addHoverListener(getHoverListener(buttonBg, buttonHoverBg));
    playerButton->addClickListener(getPlayerPickerListener(players));
    HUDGraphic *hudGraphic = new HUDGraphic(buttonBg.get());
    playerButton->setBackground(hudGraphic);
    return playerButton;
}

Layout::EventFunction Menu::getHoverListener(std::shared_ptr<Texture> buttonBg, std::shared_ptr<Texture> buttonHoverBg) {
    return [buttonBg, buttonHoverBg](int x, int y, Layout* element, bool enteredElseLeaving){
        std::shared_ptr<Texture> background = enteredElseLeaving ? buttonHoverBg : buttonBg;
        element->getGraphic()->setBackground(background.get());
    };
}

Layout::EventFunction Menu::getPlayerPickerListener(int numPlayers) {
    playersPickedCallback_t callback = playersPickedCallback;
    return [numPlayers, callback](int x, int y, Layout* element, bool enteredElseLeaving){
        callback(numPlayers);
    };
}
