//
// Created by 33771 on 26/03/2021.
//

#ifndef LIFPROJET_COMMANDQUEUE_HPP
#define LIFPROJET_COMMANDQUEUE_HPP

#include "Command.hpp"
#include<queue>


class CommandQueue {
public:
    CommandQueue();

    void push(const Command& command);
    Command pop();
    bool isEmpty();

private:
    std::queue<Command> mQueue;
};


#endif //LIFPROJET_COMMANDQUEUE_HPP
