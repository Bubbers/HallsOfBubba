//
// Created by simon on 2017-04-30.
//

#include "RoomGraph.h"

RoomGraph::RoomGraph(std::function<void(Direction)> walkCallback) {
    generateGraph(walkCallback);
}

std::shared_ptr<Room> RoomGraph::getCurrentRoom(){
    return graph[currentX][currentY];
}

void RoomGraph::walk(Direction direction) {
    std::pair<int, int> next = getNextFromDir(direction);
    std::shared_ptr<Room> theRoom = graph[next.first][next.second];
    if(!theRoom){
        throw "Tried to get a room that didn't exist";
    }
    currentX = next.first;
    currentY = next.second;
}

void RoomGraph::addPath(std::shared_ptr<Room> nextRoom, Direction direction) {
    std::pair<int, int> offset = getNextFromDir(direction);
    if(!graph[offset.first][offset.second]){
        throw "You can't assign a direction two different rooms.";
    }
    graph[offset.first][offset.second] = nextRoom;
}

std::pair<int, int> RoomGraph::getNextFromDir(Direction direction){
    int nextX = 0, nextY = 0;
    if(direction == RIGHT){
        nextX = 1;
    }else if(direction == LEFT){
        nextX = -1;
    }else if(direction == DOWN){
        nextY = -1;
    }else if(direction == UP){
        nextY = 1;
    }
    return std::pair<int,int>(currentX+nextX, currentY+nextY);
};


void RoomGraph::generateGraph(std::function<void(Direction)> walkCallback)
{
    auto startRoom    = std::pair<int, int>(3, 0);
    auto treasureRoom = std::pair<int, int>(6, 4);
    auto bossRoom     = std::pair<int, int>(0, 1);

    generatePath(startRoom, treasureRoom, walkCallback);
    generatePath(startRoom, bossRoom, walkCallback);

    currentX = startRoom.first;
    currentY = startRoom.second;
}

void RoomGraph::generatePath(std::pair<int, int> startRoom,
                             std::pair<int, int> targetRoom,
                             std::function<void(Direction)> walkCallback)
{
    const int xMin = std::min(startRoom.first, targetRoom.first);
    const int xMax = std::max(startRoom.first, targetRoom.first);

    const int yMin = std::min(startRoom.second, targetRoom.second);
    const int yMax = std::max(startRoom.second, targetRoom.second);

    // Create corridor in Y axis
    for (int y = yMin; y < yMax; ++y) {
        int x = startRoom.first;

        if (!graph[x][y]) {
            graph[x][y] = std::make_shared<Room>(false);

            if (y < yMax) {
                graph[x][y]->addDoor(Direction::UP, walkCallback);
            }
            if (y > yMin) {
                graph[x][y]->addDoor(Direction::DOWN, walkCallback);
            }
        }
    }

    // Create corridor in X axis
    for (int x = xMin; x < xMax; ++x) {
        int y = targetRoom.second;

        if (!graph[x][y]) {
            graph[x][y] = std::make_shared<Room>(false);

            if (x < (xMax - 1)) {
                graph[x][y]->addDoor(Direction::RIGHT, walkCallback);
            }
            if (x > xMin) {
                graph[x][y]->addDoor(Direction::LEFT, walkCallback);
            }
        }
    }
}

