//
// Created by 33771 on 31/01/2021.
//

#include "StateStack.hpp"

StateStack::StateStack(State::Context context) : mStack(), mPendingList(), mContext(context), mFactories() {}

void StateStack::update(sf::Time dt) {
    for(auto itr = mStack.rbegin() ; itr != mStack.rend() ; ++itr) {
        if(!(*itr)->update(dt))
            break;
    }
}

void StateStack::draw() {
    FOREACH(State::Ptr& state, mStack)
        state->draw();
}

void StateStack::handleEvent(const sf::Event& event) {
    for(auto itr = mStack.rbegin() ; itr != mStack.rend() ; ++itr) {
        if(!(*itr)->handleEvent(event))
            break;
    }

    applyPendingChanges();
}

void StateStack::pushState(States::ID stateID) {
    mPendingList.emplace_back(Push, stateID);
}

void StateStack::popState() {
    mPendingList.emplace_back(Pop);
}

void StateStack::clearStates() {
    mPendingList.emplace_back(Clear);
}

bool StateStack::isEmpty() const {
    return mStack.empty();
}

State::Ptr StateStack::createState(States::ID stateID) {
    auto found = mFactories.find(stateID);
    assert(found != mFactories.end());

    return found->second();
}

void StateStack::applyPendingChanges() {
    FOREACH(PendingChange change, mPendingList) {
        switch (change.action) {
            case Push:
                mStack.push_back(createState(change.stateID));
                break;

            case Pop:
                mStack.pop_back();
                break;

            case Clear:
                mStack.clear();
                break;
        }
    }

    mPendingList.clear();
}

StateStack::PendingChange::PendingChange(Action action, States::ID stateID) : action(action), stateID(stateID) {}