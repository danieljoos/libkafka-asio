//
// libkafka_asio.h
// ---------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef LIBKAFKA_ASIO_H_D2B00651_6C27_47B1_9403_2856E2D065EA
#define LIBKAFKA_ASIO_H_D2B00651_6C27_47B1_9403_2856E2D065EA

#include <libkafka_asio/primitives.h>
#include <libkafka_asio/constants.h>
#include <libkafka_asio/error.h>

#include <libkafka_asio/connection.h>
#include <libkafka_asio/message.h>

// Metadata API
#include <libkafka_asio/metadata_request.h>

// Produce API
#include <libkafka_asio/produce_request.h>

// Fetch API
#include <libkafka_asio/fetch_request.h>

// Offset API
#include <libkafka_asio/offset_request.h>

// Offset Commit/Fetch API
#include <libkafka_asio/consumer_metadata_request.h>
#include <libkafka_asio/offset_commit_request.h>
#include <libkafka_asio/offset_fetch_request.h>

#endif  // LIBKAFKA_ASIO_H_D2B00651_6C27_47B1_9403_2856E2D065EA
