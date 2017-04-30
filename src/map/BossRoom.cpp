

#include "BossRoom.h"

BossRoom::BossRoom() {}

void BossRoom::loadGameObjects() {
    createTorch(chag::make_vector(8.0f, 0.0f, 8.0f));
    createTorch(chag::make_vector(-8.0f, 0.0f, 8.0f));
    createTorch(chag::make_vector(-8.0f, 0.0f, -8.0f));
    createTorch(chag::make_vector(8.0f, 0.0f, -8.0f));
}
