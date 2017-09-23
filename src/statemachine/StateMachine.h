#pragma once

#include <stack>
#include <map>
#include "StateNode.h"
#include <memory>
#include <vector>

/**
 * Used to handle transitions between states in the Game.
 * Example usage:
 *
 *     const StateKey INACTIVE = 0;
 *     const StateKey ACTIVE   = 1;
 *     StateMachine statemachine({INACTIVE, ACTIVE});
 *
 *     statemachine.connect(INACTIVE, ACTIVE, []() {
 *           Logger::logDebug("New game");
 *     });
 *
 *     statemachine.transitionToState(ACTIVE);
 */
class StateMachine {

public:

    /**
     * Create a state machine with a set of states.
     * @param keys The keys representing States to be created.
     */
    StateMachine(const std::vector<StateKey> keys);
    ~StateMachine();

    /**
     * Transition to a state with a given StateKey from the current state.
     * std::invalid_argument will be thrown if there is no connection to the next state from the current state.
     * @param newStateKey The StateKey matching the next state.
     */
    void queueTransition(StateKey newStateKey);

    /**
     * Do all queued transitions
     */
    void doTransitions();

    /**
     * Creates a directed connection from one State to another State.
     * When a transition occurs using this connection, the transition callback will get called.
     *
     * std::invalid_argument will be thrown if the StateKeys does not match any State.
     *
     * @param fromKey The StateKey matching the State to connect from.
     * @param toKey  The StateKey matching the State to connect to.
     * @param transitionCallback The callback that will be called when there is a transition.
     */
    void connect(StateKey fromKey, StateKey toKey, transition_callback_t transitionCallback);

    /**
     * Checks if there is a State with the given StateKey
     */
    bool hasState(const StateKey key) const;

private:

    void addState(StateKey stateKey);

    std::map<StateKey, std::shared_ptr<StateNode>> state_map;

    StateKey currentStateKey;

    std::vector<StateKey> waitingTransitions;

    void doTransition(StateKey newStateKey);
};
