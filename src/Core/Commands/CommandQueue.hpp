//
// Created by 33771 on 26/03/2021.
//

#ifndef LIFPROJET_COMMANDQUEUE_HPP
#define LIFPROJET_COMMANDQUEUE_HPP

#include "Command.hpp"
#include<queue>

/**
 * @class CommandQueue
 * @brief Stores all the commands to process next update (wrapper of the std::queue)
 * @author SFML Game Development
 */
class CommandQueue {
public:
    /**
     * @brief Default constructor
     */
    CommandQueue();

    /**
     * @brief Pushes a command into the queue
     * @param command Command to push
     */
    void push(const Command& command);
    /**
     * @brief Returns the next command and takes it off the queue
     * @return Last command in the queue
     */
    Command pop();
    /**
     * @brief Returns if the queue is empty
     * @return true if the queue is empty, false otherwise
     */
    bool isEmpty();

private:
    std::queue<Command> mQueue;
};


#endif //LIFPROJET_COMMANDQUEUE_HPP
