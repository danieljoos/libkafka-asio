
class `Message`
===============

**Header File:** `<libkafka_asio/message.h>`

**Namespace:** `libkafka_asio`

A message communication object as described on the 
[Kafka wiki](https://cwiki.apache.org/confluence/display/KAFKA/A+Guide+To+The+Kafka+Protocol#AGuideToTheKafkaProtocol-Messagesets).
Messages consist of a key-value pair and some metadata, like CRC for checking
the message integrity and some attributes for handling compression.

Kafka handles compression by marshalling a complete set of messages, compressing
it using one of the supported compression algorithms and finally putting it
as the value of a single message, which has it's attributes set to the
corresponding compression algorithm flag. Therefore, the `Message` class of
_libkafka-asio_ defines a pointer to a possibly nested message set.
When consuming compressed data from Kafka, the nested message set will hold 
the actual uncompressed messages.


<img src="http://yuml.me/diagram/nofunky;scale:80/class/
[Message|+attributes;+key:Bytes;+value:Bytes]^-[MessageAndOffset|+offset],
[Message]+- 0..1[MessageSet],
[MessageSet]++-*[MessageAndOffset]" 
/>

### Message
```cpp
Message()
```

Creates a message with an empty key and empty value.


### Message (Copy Constructor) 
```cpp
Message(const Message& orig, bool deep = false)
```

Creates a message by copying the given original message. If the optional `deep`
parameter is set to `true`, the byte arrays for `key` and `value` as well as the
nested message set, will be copied, too. Otherwise, the new message object will
point to the same `key`, `value` and nested message set on the heap.

### Message (Assignment Operator)
```cpp
Message& operator= (const Message& rhs)
```

Flat-copy the given message.


Member functions
----------------

### magic_byte
```cpp
Int8 magic_byte() const;
```

Always returns zero.


### attributes
```cpp
Int8 attributes() const;
```

Returns the attribute bitset. The lowest 2 bits indicate the compression
algorithm.


### set_attributes
```cpp
void set_attributes(Int8 attributes);
```

Sets the attributes byte of this message object.


### key
```cpp
const Bytes& key() const;
Bytes& mutable_key();
```

Optional message key. Can be `NULL` (default).


### value
```cpp
const Bytes& value() const;
Bytes& mutable_value();
```

Actual message data as byte array


### nested_message_set
```cpp
const MessageSet& nested_message_set() const;
MessageSet& mutable_nested_message_set();
```

Compressed messages contain a nested message set (see description above).


### compression
```cpp
constants::Compression compression() const;
```

Returns the compression algorithm, used for compressing the message value. The
function only evaluates the lowest 2 bits of the attributes field.


class `MessageAndOffset`
========================

**Header File:** `<libkafka_asio/message.h>`

**Namespace:** `libkafka_asio`

Adds offset information to the `Message` class. Used for message consumption
from Kafka.

### MessageAndOffset (overload 1 of 2)
```cpp
MessageAndOffset()
```

Creates an empty `MessageAndOffset` object. The offset information defaults
to zero.


### MessageAndOffset (overload 2 of 2)
```cpp
MessageAndOffset(const Message& message, Int64 offset)
```

Creates a `MessageAndOffset` object by copying the given `Message` object (flat
copy) and using the given `offset` information.


## Member Functions

### offset
```cpp
Int64 offset() const
```

Returns the offset information.


### set_offset
```cpp
void set_offset(Int64 offset)
```

Sets the offset information.


# Type `MessageSet`

```cpp
typedef std::vector<MessageAndOffset> MessageSet;
```