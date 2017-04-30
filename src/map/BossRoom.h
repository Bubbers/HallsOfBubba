#pragma once

#include "Room.h"

class BossRoom : public Room{
public:
    BossRoom();

protected:
    void loadGameObjects() override;

};



