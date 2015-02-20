# libkafka-asio
C++ Kafka Client Library using Boost Asio

[![Build Status](https://travis-ci.org/danieljoos/libkafka-asio.svg?branch=master)](https://travis-ci.org/danieljoos/libkafka-asio)

## Introduction

`libkafka-asio` is a C++ header-only library, implementing the Kafka client protocol. The following Kafka APIs are implemented at the moment:

* Metadata
* Produce
* Fetch
* Offset

For now it is missing support for Snappy/GZIP compression, but I plan to include this as well.

The library was tested on Linux (GCC 4.9) and Windows (MSVC 10, 11, 12). There is a small (yet not complete) set of unit tests available under `test`.
See the `examples` directory for some examples on how to use the library.

## Dependencies

`libkafka-asio` depends on the Boost C++ libraries, specially on Boost Asio. The following Boost sub-libraries are explicitly included in the project:

* boost asio
* boost bind
* boost crc
* boost foreach
* boost integer
* boost iterator
* boost optional
* boost smart ptr
* boost system

You need to link against `boost_thread` and `boost_system`.

So installing the boost library package on your distribution should do the trick (e.g. `apt-get install libboost-dev` on Ubuntu, or `pacman -S boost` on Arch).
