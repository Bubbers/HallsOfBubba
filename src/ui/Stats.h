#pragma once

#include <PositioningLayout.h>
#include <TextLayout.h>
#include <statemachine/StateMachine.h>

class Stats: public PositioningLayout
{
public:
    Stats(unsigned int time, unsigned int points, std::function<void()> playAgain);

    Layout::EventFunction getHoverListener(std::shared_ptr<Texture> buttonBg, std::shared_ptr<Texture> buttonHoverBg);
};
