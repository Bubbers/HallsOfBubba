#pragma once

#include <map>
#include <map/Direction.h>
#include "Room.h"

#define GRAPH_WIDTH 7
#define GRAPH_HEIGHT 7
#define GRAPH_LEVELS 3

typedef std::pair<unsigned int, unsigned int> level_pos_t;
typedef std::function<void()> win_callback_t;

class RoomGraph  {

public:

    RoomGraph(walk_callback_t walkCallback);

    ~RoomGraph();

    std::shared_ptr<Room> getCurrentRoom();
    void walk(Direction direction);
    void nextLevel(win_callback_t winCallback);

private:

    unsigned int currentX = 0;
    unsigned int currentY = 0;
    unsigned int currentLevel = 0;

    std::shared_ptr<Room> graph[GRAPH_WIDTH][GRAPH_HEIGHT][GRAPH_LEVELS];

    level_pos_t getNextFromDir(Direction direction);
    level_pos_t levelStartPositions[GRAPH_LEVELS];

    void generateGraph(walk_callback_t walkCallback);

    void generatePath(level_pos_t startRoom,
                      level_pos_t targetRoom,
                      unsigned int level);

    void generateDoors(walk_callback_t walkCallback, unsigned int level);

    level_pos_t randomLevelPos();
    void enterNewLevel(unsigned int level);

    void printLevelLayout(unsigned int level);

    level_pos_t randomNewLevelPos(std::vector<level_pos_t> occupied);
};
