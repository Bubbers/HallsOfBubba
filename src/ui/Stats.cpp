#include "Stats.h"
#include <ListLayout.h>
#include <TextLayout.h>
#include <FontManager.h>
#include <HUDGraphic.h>
#include <ResourceManager.h>

Stats::Stats(unsigned int time, unsigned int points, std::function<void()> playAgain)
        : PositioningLayout(Dimension::fromPercentage(100), Dimension::fromPercentage(100)){

    std::shared_ptr<Texture> bg = ResourceManager::loadAndFetchTexture("../assets/meshes/brick_wall_final.jpg");
    std::shared_ptr<Texture> buttonBg = ResourceManager::loadAndFetchTexture("../assets/meshes/button.png");
    std::shared_ptr<Texture> buttonHoverBg = ResourceManager::loadAndFetchTexture("../assets/meshes/button_hover.png");
    setBackground(new HUDGraphic(bg.get()));

    Font* ubuntuFont28 = FontManager::getInstance()->loadAndFetchFont("../fonts/Ubuntu-M.ttf", 28);

    // WON MSG
    TextLayout *titleLayout = new TextLayout("Congratulations you won!", ubuntuFont28, Dimension::fromPixels(200), Dimension::fromPixels(32));
    addChild(titleLayout, Dimension::fromPercentagePlusPixels(50, -100), Dimension::fromPercentage(10));

    Font* ubuntuFont20 = FontManager::getInstance()->loadAndFetchFont("../fonts/Ubuntu-M.ttf", 20);

    // STATS TEXT
    TextLayout *choosePlayersText = new TextLayout("Time: " + std::to_string(time) + " Points: " + std::to_string(points),
                                                   ubuntuFont20, Dimension::fromPixels(300), Dimension::fromPixels(24));
    addChild(choosePlayersText, Dimension::fromPercentagePlusPixels(50, -150), Dimension::fromPercentage(40));

    // PLAY AGAIN BUTTON
    Layout *playerButton = new TextLayout("Play again", ubuntuFont20,
                                          Dimension::fromPixels(150), Dimension::fromPixels(30));
    playerButton->addHoverListener(getHoverListener(buttonBg, buttonHoverBg));

    Layout::EventFunction playAgain_ = [playAgain](int x, int y, Layout* element, bool enteredElseLeaving) {
        playAgain();
    };
    playerButton->addClickListener(playAgain_);
    HUDGraphic *hudGraphic = new HUDGraphic(buttonBg.get());
    playerButton->setBackground(hudGraphic);

    addChild(playerButton, Dimension::fromPercentagePlusPixels(50, -70), Dimension::fromPercentage(60));
}

Layout::EventFunction Stats::getHoverListener(std::shared_ptr<Texture> buttonBg, std::shared_ptr<Texture> buttonHoverBg) {
    return [buttonBg, buttonHoverBg](int x, int y, Layout* element, bool enteredElseLeaving){
        std::shared_ptr<Texture> background = enteredElseLeaving ? buttonHoverBg : buttonBg;
        element->getGraphic()->setBackground(background.get());
    };
}
