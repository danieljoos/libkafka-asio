//
// StreamTest.h
// ------------
//
// Copyright (c) 2015 Daniel Joos
//
// Distributed under MIT license. (See file LICENSE)
//

#ifndef STREAM_TEST_H_FE8E717B_A2CE_4CA4_8A94_EA5C745A278F
#define STREAM_TEST_H_FE8E717B_A2CE_4CA4_8A94_EA5C745A278F

#include <iostream>
#include <boost/asio/streambuf.hpp>
#include <boost/shared_ptr.hpp>
#include <libkafka_asio/primitives.h>

// Little helper for testing stream based operations
class StreamTest
{
protected:

  void ResetStream()
  {
    streambuf.reset(new boost::asio::streambuf());
    stream.reset(new std::iostream(streambuf.get()));
  }

  libkafka_asio::Bytes ReadEverything()
  {
    using libkafka_asio::Bytes;
    Bytes result(new Bytes::element_type());
    while (stream->good())
    {
      char c = 0;
      stream->get(c);
      if (stream->good())
      {
        result->push_back(c);
      }
    }
    return result;
  }

  typedef boost::shared_ptr<boost::asio::streambuf> StreamBufType;
  typedef boost::shared_ptr<std::iostream> StreamType;
  StreamBufType streambuf;
  StreamType stream;
};

#endif  // STREAM_TEST_H_FE8E717B_A2CE_4CA4_8A94_EA5C745A278F
