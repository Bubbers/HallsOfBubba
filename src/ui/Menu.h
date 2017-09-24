#ifndef HALLSOFBUBBA_MENU_H
#define HALLSOFBUBBA_MENU_H

#include <PositioningLayout.h>
#include <TextLayout.h>
#include <statemachine/StateMachine.h>

typedef std::function<void(int)> playersPickedCallback_t;

class Menu : public PositioningLayout {


public:
    Menu(playersPickedCallback_t playersPickedCallback);

    Layout::EventFunction getHoverListener(std::shared_ptr<Texture> buttonBg, std::shared_ptr<Texture> buttonHoverBg);

    EventFunction getPlayerPickerListener(int i);

    Layout *createPlayerNumberPicker(Font *ubuntuFont20, int players, std::shared_ptr<Texture> ptr,
                                             std::shared_ptr<Texture> shared_ptr);

private:
    playersPickedCallback_t playersPickedCallback;

};


#endif //HALLSOFBUBBA_MENU_H
