//
// Created by 33771 on 31/01/2021.
//

#ifndef LIFPROJET_STATESTACK_HPP
#define LIFPROJET_STATESTACK_HPP

#include "State.hpp"

#include "../../Core/Util/Foreach.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>

#include <functional>
#include <utility>
#include <cassert>
#include <vector>
#include <map>

/**
 * @class StateStack
 * @brief Structure used to store the current states involved in the form of a stack
 * @author SFML Game Development
 */
class StateStack : private sf::NonCopyable {
public:
    /**
     * @enum Action
     * @brief Enumeration used to describe the type of changed to apply
     */
    enum Action {
        Push,
        Pop,
        Clear
    };

public:
    /**
     * @brief Parameter constructor
     * @param context Context used to access useful structures
     */
    explicit StateStack(State::Context context);
    /**
     * @brief Default destructor
     */
    ~StateStack();

    /**
     * @brief Register a state to the StateStack and create it's factory
     * @tparam T Type of the State class specialization
     * @param stateID Identifier of the registered state
     */
    template<typename T>
    void registerState(States::ID stateID);

    /**
     * @brief Draw the states on the stack
     */
    void draw();
    /**
     * @brief Update the states on the stack
     * @param dt Time interval since the last update
     */
    void update(sf::Time dt);
    /**
     * @brief Handle the events of the states on the stack
     * @param event
     */
    void handleEvent(const sf::Event& event);

    /**
     * @brief Push a registered state on the stack
     * @param stateID Identifier of the state to push
     */
    void pushState(States::ID stateID);
    /**
     * @brief Pop the first state on the stack
     */
    void popState();
    /**
     * @brief Clear all the states on the stack
     */
    void clearStates();

    /**
     * @brief Returns if the stack is empty
     * @return true if the stack is empty
     */
    bool isEmpty() const;


private:
    /**
     * Creates the actual state
     * @param stateID Identifier of the state to create
     * @return Pointer to the created state
     */
    State::Ptr createState(States::ID stateID);
    /**
     * @brief applies all the changes in the pendingChange list
     */
    void applyPendingChanges();


private:
    /**
     * @struct PendingChange
     * @brief Stores a stack modification to apply
     */
    struct PendingChange {
        /**
         * @brief Parameter constructor
         * @param action Action to apply
         * @param stateID Identifier if needed (None by default)
         */
        explicit PendingChange(Action action, States::ID stateID = States::None);

        Action action;
        States::ID stateID;
    };


private:
    std::vector<State::Ptr> mStack;
    std::vector<PendingChange> mPendingList;

    State::Context mContext;
    std::map<States::ID, std::function<State::Ptr()>> mFactories;
};


template <typename T>
void StateStack::registerState(States::ID stateID) {
    mFactories[stateID] = [this] () {
        return State::Ptr(new T(*this, mContext));
    };
}


#endif //LIFPROJET_STATESTACK_HPP
