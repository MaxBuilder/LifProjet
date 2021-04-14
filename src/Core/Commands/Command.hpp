//
// Created by 33771 on 26/03/2021.
//

#ifndef LIFPROJET_COMMAND_HPP
#define LIFPROJET_COMMAND_HPP

#include "../../Game/Map/EntityInfo.hpp"

/**
 * @enum CommandType
 * @brief Defines the type (and the effect) of the command
 */
namespace CommandType {
    enum ID {
        // Entity communication commands
        MakeTeam,
        TeamAccept,
        TeamDeny,
        InPosition,
        Assault,
        CastleAssaulted,
        FallBack,

        // System communication commands
        Dead,
        MakeArrow,
    };
}

/**
 * @struct Command
 * @brief Stores all the information needed for the command (team, sender, receiver and effect)
 */
struct Command {
    EntityInfo::Team mTeam;
    int mSender;
    int mReceiver; // 9999 -> whole team
    CommandType::ID mType;

    /**
     * @brief Parameter constructor
     * @param team Identifier of the targeted team
     * @param sender Id of the sender
     * @param receiver Id of the receiver (9999 for all the team)
     * @param type Identifier of the command type (defines the effect)
     */
    Command(EntityInfo::Team team, int sender, int receiver, CommandType::ID type);
};


#endif //LIFPROJET_COMMAND_HPP
