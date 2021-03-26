//
// Created by 33771 on 26/03/2021.
//

#ifndef LIFPROJET_COMMAND_HPP
#define LIFPROJET_COMMAND_HPP

namespace CommandType {
    enum ID {
        MakeTeam,
        TeamAccept,
        TeamDeny,
        InPosition,
        Assault,
    };
}

struct Command {
    bool mTeam; // true -> red | false -> blue
    int mSender;
    int mReceiver; // 9999 -> whole team
    CommandType::ID mType;

    Command(bool t, int mSender, int mReceiver, CommandType::ID type);
};


#endif //LIFPROJET_COMMAND_HPP
