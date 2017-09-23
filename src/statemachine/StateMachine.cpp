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

    currentStateKey = keys.front();
}

StateMachine::~StateMachine()
{
}

void StateMachine::queueTransition(StateKey newStateKey)
{
    if (!hasState(newStateKey)) {
        throw std::invalid_argument("Trying to transition to non-existing state");
    }

    Logger::logInfo("Queueing a transition to " + std::to_string(newStateKey));
    waitingTransitions.push_back(newStateKey);
}

void StateMachine::addState(StateKey stateKey)
{
    if (hasState(stateKey)) {
        return;
    }

    state_map[stateKey] = std::make_shared<StateNode>(stateKey);
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

void StateMachine::doTransitions()
{
    Logger::logInfo("Waiting transitions: " + std::to_string(waitingTransitions.size()));
    for (auto it = waitingTransitions.begin(); it < waitingTransitions.end();) {
        auto newStateKey = *it;
        waitingTransitions.erase(it);
        doTransition(newStateKey);
    }
}

void StateMachine::doTransition(StateKey newStateKey) {
    auto currentState = state_map[currentStateKey];
    auto newState = state_map[newStateKey];

    Logger::logInfo("Current: " + std::to_string(currentStateKey) + " next: " + std::to_string(newStateKey));
    if (!currentState->hasConnection(newState->id)) {
        throw std::invalid_argument("Trying to transition to a state to which there"
                                            " is no connection from the current state");
    }

    currentState->transition(newState->id);
    currentStateKey = newStateKey;

}
