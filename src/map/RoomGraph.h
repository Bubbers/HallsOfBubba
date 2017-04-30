//
// Created by simon on 2017-04-30.
//

#ifndef HALLSOFBUBBA_ROOMGRAPH_H
#define HALLSOFBUBBA_ROOMGRAPH_H

#include <map>
#include "map/HallwayRoom.h"
#include <map/Direction.h>

class RoomGraph  {

public:

    RoomGraph(std::function<void(Direction)> walkCallback);

    std::shared_ptr<HallwayRoom> getCurrentRoom();
    void walk(Direction direction);

    void addPath(std::shared_ptr<HallwayRoom> nextRoom, Direction direction);

private:

    int currentX = 0;
    int currentY = 0;
    std::map<Direction, std::weak_ptr<RoomGraph>> paths;
    std::shared_ptr<HallwayRoom> graph[7][7] = {{},{},{},{},{},{},{}};

    std::pair<int, int> getNextFromDir(Direction direction);

    bool hasWalked = false;
    Direction directionWalked;


    void generateGraph(std::function<void(Direction)> walkCallback);
    void generatePath(std::pair<int, int> startRoom,
                      std::pair<int, int> targetRoom);

    void generateDoors(std::function<void(Direction)> walkCallback);
};


#endif //HALLSOFBUBBA_ROOMGRAPH_H
