
# Type `MessageSet`

**Header File:** `<libkafka_asio/message.h>`

**Namespace:** `libkafka_asio`

```cpp
typedef std::vector<MessageAndOffset> MessageSet;
```

<center>
<img src="http://yuml.me/diagram/nofunky;scale:80/class/
[Message|+attributes;+key:Bytes;+value:Bytes]^-[MessageAndOffset|+offset],
[Message]+- 0..1[MessageSet{bg:orange}],
[MessageSet]++-*[MessageAndOffset]"
/>
</center>
