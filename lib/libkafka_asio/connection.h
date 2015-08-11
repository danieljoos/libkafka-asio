//
// connection.h
// ------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef CONNECTION_H_229D7905_40B7_49F1_BAC5_910B10FADDBA
#define CONNECTION_H_229D7905_40B7_49F1_BAC5_910B10FADDBA

#include <libkafka_asio/detail/basic_connection.h>
#include <libkafka_asio/detail/connection_service.h>

namespace libkafka_asio
{

typedef detail::BasicConnection<detail::ConnectionService> Connection;

}  // namespace libkafka_asio

#endif  // CONNECTION_H_229D7905_40B7_49F1_BAC5_910B10FADDBA
