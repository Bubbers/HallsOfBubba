#pragma once

#include <map>
#include <functional>

typedef int StateKey;
typedef std::function<void()> transition_callback_t;

class State {


public:
    State(StateKey id);
    ~State();

    const StateKey id;

    void connect(StateKey id, std::function<void()> transitionCallback);
    bool hasConnection(StateKey id);
    void transition(StateKey id);

private:

    std::map<StateKey, transition_callback_t> connections;
};

