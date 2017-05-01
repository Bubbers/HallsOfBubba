

#include <HudRenderer.h>
#include <ListLayout.h>
#include <TextLayout.h>
#include <FontManager.h>
#include <PositioningLayout.h>
#include "BossDeathWin.h"

BossDeathWin::BossDeathWin(HudRenderer *hudRenderer) : hudRenderer(hudRenderer){}

void BossDeathWin::onDeath() {
    PositioningLayout *layout = new PositioningLayout(Dimension::fromPercentage(100), Dimension::fromPercentage(100));
    TextLayout *tx = new TextLayout("You win",
                                    FontManager::getInstance()->loadAndFetchFont("../fonts/Ubuntu-M.ttf", 40),
                                    Dimension::fromPixels(200),
                                    Dimension::fromPixels(20));
    layout->addChild(tx, Dimension::fromPercentagePlusPixels(50, -100),  Dimension::fromPercentagePlusPixels(50, -10));
    hudRenderer->setLayout(layout);
}

void BossDeathWin::update(float dt) {

}
