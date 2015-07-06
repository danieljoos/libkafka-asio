
class `MessageAndOffset`
========================

**Header File:** `<libkafka_asio/message.h>`

**Namespace:** `libkafka_asio`

Adds offset information to the [`Message`](message) class. Used for message 
consumption from Kafka.

<center>
<img src="http://yuml.me/diagram/nofunky;scale:80/class/
[Message|+attributes;+key:Bytes;+value:Bytes]^-[MessageAndOffset{bg:orange}|+offset],
[Message]+- 0..1[MessageSet],
[MessageSet]++-*[MessageAndOffset]" 
/>
</center>

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
