#include "StateMachine.h"
#include <exception>
#include <Logger.h>

StateMachine::StateMachine(const std::vector<StateKey> keys)
{
    if (keys.size() == 0) {
        throw std::invalid_argument("StateMachine can not be contructed with no states");
    }

    for (auto key : keys) {
        addState(key);
    }

    prevStateKey = keys.front();
    currentStateKey = keys.front();
}

StateMachine::~StateMachine()
{
}

void StateMachine::transitionToPrevState()
{
    if (prevStateKey == currentStateKey) {
        Logger::logWarning("Can not transition to previous state"
                           " since it is the same as the current state.");
        return;
    }

    transitionToState(prevStateKey);
}

void StateMachine::transitionToState(StateKey newStateKey)
{
    if (!hasState(newStateKey)) {
        throw std::invalid_argument("Trying to transition to non-existing state");
    }

    auto currentState = state_map[currentStateKey];
    auto newState = state_map[newStateKey];

    if (!currentState->hasConnection(newState->id)) {
        throw std::invalid_argument("Trying to transition to a state to which there"
                                    " is no connection from the current state");
    }

    currentState->transition(newState->id);

    prevStateKey = currentStateKey;
    currentStateKey = newStateKey;
}

void StateMachine::addState(StateKey stateKey)
{
    if (hasState(stateKey)) {
        return;
    }

    state_map[stateKey] = std::make_shared<State>(stateKey);
}

void StateMachine::connect(StateKey fromKey, StateKey toKey, transition_callback_t callback)
{
    if (!hasState(fromKey)) {
        throw std::invalid_argument("Trying to connect from state \""
                                    + std::to_string(fromKey)
                                    + "\" which does not exist in the state machine.");
    }

    if (!hasState(toKey)) {
        throw std::invalid_argument("Trying to connect from state \""
                                    + std::to_string(toKey)
                                    + "\" which does not exist in the state machine.");
    }

    state_map[fromKey]->connect(toKey, callback);
}

bool StateMachine::hasState(const StateKey key) const
{
    return state_map.count(key) > 0;
}
