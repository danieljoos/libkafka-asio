//
// message_fwd.h
// -------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef MESSAGE_FWD_H_2FB308F0_768F_4F8A_8943_FBD773BAA21D
#define MESSAGE_FWD_H_2FB308F0_768F_4F8A_8943_FBD773BAA21D

#include <vector>

namespace libkafka_asio
{

class Message;

class MessageAndOffset;

typedef std::vector<MessageAndOffset> MessageSet;

}  // namespace libkafka_asio

#endif  // MESSAGE_FWD_H_2FB308F0_768F_4F8A_8943_FBD773BAA21D
