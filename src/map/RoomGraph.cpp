#include <Logger.h>
#include "RoomGraph.h"
#include "BossRoom.h"
#include "HallwayRoom.h"

RoomGraph::RoomGraph(walk_callback_t walkCallback)
{
    generateGraph(walkCallback);
}

RoomGraph::~RoomGraph()
{
    for (int x = 0; x < GRAPH_WIDTH; ++x) {
        for (int y = 0; y < GRAPH_HEIGHT; ++y) {
            for (int level = 0; level < GRAPH_LEVELS; ++level) {
                 graph[x][y][level].reset();
            }
        }
    }
}

std::shared_ptr<Room> RoomGraph::getCurrentRoom()
{
    return graph[currentX][currentY][currentLevel];
}

void RoomGraph::nextLevel(win_callback_t winCallback)
{
    if (currentLevel < GRAPH_LEVELS - 1) {
        enterNewLevel(currentLevel + 1);
    } else {
        winCallback();
    }
}

void RoomGraph::walk(Direction direction)
{
    level_pos_t next = getNextFromDir(direction);
    if (!graph[next.first][next.second][currentLevel]) {
        throw "Tried to get a room that didn't exist";
    }

    currentX = next.first;
    currentY = next.second;
}

level_pos_t RoomGraph::getNextFromDir(Direction direction)
{
    int nextX = 0, nextY = 0;
    switch (direction) {
        case RIGHT: nextX =  1; break;
        case LEFT:  nextX = -1; break;
        case UP:    nextY =  1; break;
        case DOWN:  nextY = -1; break;
        default:    break;
    }
    return level_pos_t(currentX + nextX, currentY + nextY);
};

void RoomGraph::generateGraph(walk_callback_t walkCallback)
{

    for (unsigned int level = 0; level < GRAPH_LEVELS; ++level) {

        levelStartPositions[level].first = (unsigned) rand() % GRAPH_WIDTH;
        levelStartPositions[level].second = 0;
        auto &startRoomPos = levelStartPositions[level];

        auto bossRoomPos = randomNewLevelPos({startRoomPos});
        auto treasureRoomPos = randomNewLevelPos({startRoomPos, bossRoomPos});

        // TODO Make the boss rooms have different bosses
        auto bossRoom = std::make_shared<BossRoom>();
        bossRoom->addDoor(NEXT_LEVEL, walkCallback);
        graph[bossRoomPos.first][bossRoomPos.second][level] = bossRoom;

        generatePath(startRoomPos, treasureRoomPos, level);
        generatePath(startRoomPos, bossRoomPos, level);
        generateDoors(walkCallback, level);
    }

    enterNewLevel(0);
}

level_pos_t RoomGraph::randomNewLevelPos(std::vector<level_pos_t> occupied)
{
    while (true) {
        level_pos_t rpos = randomLevelPos();

        for (level_pos_t opos : occupied) {
            if (opos == rpos) {
                continue;
            }
        }
        return rpos;
    }
}

level_pos_t RoomGraph::randomLevelPos()
{
    return level_pos_t(rand() % GRAPH_WIDTH,
                       rand() % GRAPH_HEIGHT);
}

void RoomGraph::generatePath(level_pos_t startRoom,
                             level_pos_t targetRoom,
                             unsigned int level)
{
    const int xMin = std::min(startRoom.first, targetRoom.first);
    const int xMax = std::max(startRoom.first, targetRoom.first);

    const int yMin = std::min(startRoom.second, targetRoom.second);
    const int yMax = std::max(startRoom.second, targetRoom.second);

    // Create corridor in Y axis
    for (int y = yMin; y <= yMax; ++y) {
        int x = startRoom.first;

        if (!graph[x][y][level]) {
            graph[x][y][level] = std::make_shared<HallwayRoom>(false);
        }
    }

    // Create corridor in X axis
    for (int x = xMin; x <= xMax; ++x) {
        int y = targetRoom.second;

        if (!graph[x][y][level]) {
            graph[x][y][level] = std::make_shared<HallwayRoom>(false);
        }
    }
}

void RoomGraph::generateDoors(walk_callback_t walkCallback,
                              unsigned int level)
{
    for (int x = 0; x < GRAPH_WIDTH; ++x) {
        for (int y = 0; y < GRAPH_HEIGHT; ++y) {
            auto room = graph[x][y][level];
            if (room) {
                if (y + 1 < GRAPH_HEIGHT && graph[x][y + 1][level]) {
                    room->addDoor(Direction::UP, walkCallback);
                }
                if (y > 0 && graph[x][y - 1][level]) {
                    room->addDoor(Direction::DOWN, walkCallback);
                }
                if (x + 1 < GRAPH_WIDTH && graph[x + 1][y][level]) {
                    room->addDoor(Direction::RIGHT, walkCallback);
                }
                if (x > 0 && graph[x - 1][y][level]) {
                    room->addDoor(Direction::LEFT, walkCallback);
                }
            }
        }
    }
}

void RoomGraph::enterNewLevel(unsigned int level)
{

    printLevelLayout(level);

    currentLevel = level;
    auto &startRoomPos = levelStartPositions[currentLevel];
    currentX = startRoomPos.first;
    currentY = startRoomPos.second;
}

void RoomGraph::printLevelLayout(unsigned int level)
{
    Logger::logInfo("Layout of level " + std::to_string(level));
    std::string hline = "";

    for (unsigned int i = 0; i < GRAPH_WIDTH + 2; ++i) {
        hline.append("-");
    }

    Logger::logInfo(hline);
    for (int y = (GRAPH_HEIGHT - 1); y >= 0; --y) {
        std::string mline = "|";
        for (int x = 0; x < GRAPH_WIDTH; ++x) {
            if (graph[x][y][level]) {
                auto &startRoomPos = levelStartPositions[level];

                if (x == startRoomPos.first && y == startRoomPos.second) {
                    mline.append("S");
                } else {
                    mline.append(graph[x][y][level]->getMapSymbol());
                }
            } else {
                mline.append(" ");
            }

        }
        mline.append("|");
        Logger::logInfo(mline);
    }
    Logger::logInfo(hline);
}

