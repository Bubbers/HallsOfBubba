#include "State.h"

State::State(int id): id(id), connections({})
{
}

State::~State()
{
}

void State::connect(int id, std::function<void()> transitionCallback)
{
    if (hasConnection(id)) {
        return;
    }

    connections[id] = transitionCallback;
}

bool State::hasConnection(int id) {
    return connections.count(id) > 0;
}

void State::transition(int id) {
    connections.at(id)();
}
