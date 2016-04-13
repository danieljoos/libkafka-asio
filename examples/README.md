# Examples

Here you can find some examples showing how to use `libkafka-asio`.

* [`cpp03`](cpp03):
  Examples using the C++03 language and features. This should also work with
  rather old compiler versions.
* [`cpp11`](cpp11):
  Showcasing some C++11 language features. Requires newer compilers and at 
  least CMake version 3.1
   
The following examples are available in the above folders:

- [`fetch.cpp`](cpp03/fetch.cpp):
  Use a fetch request to get messages for a topic partition.
  ([C++03](cpp03/fetch.cpp), 
   [C++11](cpp11/fetch.cpp))
- [`metadata.cpp`](cpp03/metadata.cpp):
  Get topic metadata to determine leading brokers.
  ([C++03](cpp03/metadata.cpp),
   [C++11](cpp11/metadata.cpp))
- [`offset.cpp`](cpp03/offset.cpp)
  Get the current offset number of a topic partition.
  ([C++03](cpp03/offset.cpp),
   [C++11](cpp11/offset.cpp))
- [`offset_fetch.cpp`](cpp03/offset_fetch.cpp):
  Shows how to get offset data for a topic in a consumer group. Also 
  illustrates the use of futures and promises.
  ([C++03](cpp03/offset_fetch.cpp),
   [C++11](cpp11/offset_fetch.cpp))
- [`produce.cpp`](cpp03/produce.cpp):
  Produce a _Hello World_ message on a topic partition.
  ([C++03](cpp03/produce.cpp),
   [C++11](cpp11/produce.cpp))
