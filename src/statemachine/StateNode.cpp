#include "StateNode.h"

StateNode::StateNode(int id): id(id), connections({})
{
}

StateNode::~StateNode()
{
}

void StateNode::connect(int id, std::function<void()> transitionCallback)
{
    if (hasConnection(id)) {
        return;
    }

    connections[id] = transitionCallback;
}

bool StateNode::hasConnection(int id) {
    return connections.count(id) > 0;
}

void StateNode::transition(int id) {
    connections.at(id)();
}
