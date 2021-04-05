//
// Created by 33771 on 26/03/2021.
//

#ifndef LIFPROJET_COMMAND_HPP
#define LIFPROJET_COMMAND_HPP

#include "../../Game/Map/EntityInfo.hpp"

namespace CommandType {
    enum ID {
        MakeTeam,
        TeamAccept,
        TeamDeny,
        InPosition,
        Assault,
        Dead,
    };
}

struct Command {
    EntityInfo::Team mTeam; // true -> red | false -> blue
    int mSender;
    int mReceiver; // 9999 -> whole team
    CommandType::ID mType;

    Command(EntityInfo::Team team, int sender, int receiver, CommandType::ID type);
};


#endif //LIFPROJET_COMMAND_HPP
