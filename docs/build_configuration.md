
Build Configuration Options
===========================

Compression Options
-------------------

For handling compressed messages, _libkafka-asio_ relies on common compression
libraries: 
[zlib](http://www.zlib.net/) and 
[snappy](https://code.google.com/p/snappy/).
Of course, this means that an application that uses _libkafka-asio_ must link
against those compression libraries. If, for some reason, you do not
want to do this, there are several build configuration options available to
either turn off specific compression algorithms or compression handling at all.

### Turn off handling of compressed messages

```cpp
#define LIBKAFKAASIO_NO_COMPRESSION
```

### Turn off GZIP compression algorithm

```cpp
#define LIBKAFKAASIO_NO_COMPRESSION_GZIP
```

### Turn off Snappy compression algorithm

```cpp
#define LIBKAFKAASIO_NO_COMPRESSION_SNAPPY
```
