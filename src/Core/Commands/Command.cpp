//
// Created by 33771 on 26/03/2021.
//

#include "Command.hpp"

Command::Command(bool team, int sender, int receiver, CommandType::ID type)
: mTeam(team)
, mSender(sender)
, mReceiver(receiver)
, mType(type) {}