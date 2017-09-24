//
// Created by simon on 2017-04-29.
//

#ifndef HALLSOFBUBBA_OBJECTIDENTIFIERS_H
#define HALLSOFBUBBA_OBJECTIDENTIFIERS_H

// If and only if the identifier is a bullets, it should have its last bit set to 1
enum ObjectIdentifiers : int {
    PLAYER_IDENTIFIER = 2,
    ENEMY_IDENTIFIER = 4,
    PLAYER_SPAWNED_BULLET = 1,
    ENEMY_SPAWNED_BULLET = 3,
    DOOR_IDENTIFIER = 6,
    WALL_IDENTIFIER = 8,
    OBSTACLE_IDENTIFIER = 10,
    BOSS_SHIELD = 5,
    REVIVE_POINT_IDENTIFIER = 12
};

#endif //HALLSOFBUBBA_OBJECTIDENTIFIERS_H
