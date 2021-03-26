//
// Created by 33771 on 26/03/2021.
//

#include "CommandQueue.hpp"

CommandQueue::CommandQueue() {}

void CommandQueue::push(const Command &command) {
    mQueue.push(command);
}

Command CommandQueue::pop() {
    Command command = mQueue.front();
    mQueue.pop();
    return command;
}

bool CommandQueue::isEmpty() {
    return mQueue.empty();
}

