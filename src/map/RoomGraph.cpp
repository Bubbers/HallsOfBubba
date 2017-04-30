//
// Created by simon on 2017-04-30.
//

#include "RoomGraph.h"

RoomGraph::RoomGraph(std::function<void(Direction)> walkCallback) {
    graph[currentX][currentY] = std::make_shared<Room>(true);
    graph[currentX][currentY+1] = std::make_shared<Room>(false);
    graph[currentX][currentY]->addDoor(Direction::UP, walkCallback);
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

