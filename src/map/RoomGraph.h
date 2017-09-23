//
// Created by simon on 2017-04-30.
//

#ifndef HALLSOFBUBBA_ROOMGRAPH_H
#define HALLSOFBUBBA_ROOMGRAPH_H

#include <map>
#include <map/Direction.h>
#include "Room.h"

class RoomGraph  {

public:

    RoomGraph(std::function<void(Direction)> walkCallback, std::function<void()> &allPlayersDead);
    ~RoomGraph();

    std::shared_ptr<Room> getCurrentRoom();
    void walk(Direction direction);

    void addPath(std::shared_ptr<Room> nextRoom, Direction direction);

private:

    int currentX = 0;
    int currentY = 0;
    std::map<Direction, std::weak_ptr<RoomGraph>> paths;
    std::shared_ptr<Room> graph[7][7] = {{},{},{},{},{},{},{}};

    std::pair<int, int> getNextFromDir(Direction direction);

    bool hasWalked = false;
    Direction directionWalked;


    void generateGraph(std::function<void(Direction)> walkCallback, std::function<void()> &function);
    void generatePath(std::pair<int, int> startRoom,
                      std::pair<int, int> targetRoom,
                      std::function<void()> &allPlayersDead);

    void generateDoors(std::function<void(Direction)> walkCallback);
};


#endif //HALLSOFBUBBA_ROOMGRAPH_H
