topic "RC4 Encoder";
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s0;%- [*R6 RC4 Encoder]&]
[s0; &]
[s0; RC4 Encoder is a simple and fast [^topic`:`/`/StreamCypher`/srcdoc`/StreamCryptography`$en`-us^ s
tream encoder] with quite weak security.&]
[s0; The complete algorhithm description can be found [^http`:`/`/en`.wikipedia`.org`/wiki`/RC4^ h
ere].&]
[s0; &]
[s0; The usage is quite simple :&]
[s0; &]
[s0;l160; [/ #include <StreamCypher/StreamCypher.h>]&]
[s0;l160;/ &]
[s0;l160; [/ String EncodeWithRC4(String key, String data)]&]
[s0;l160; [/ `{]&]
[s0;l160; [/ -|RC4 rc4(key);]&]
[s0;l160; [/ -|return rc4.Encode(data);]&]
[s0;l160; [/ `}]&]
[s0; &]
[s0; There are also encoding functions for in`-place buffer encoding 
and for out`-of`-place buffer encoding; you can also give the 
key as a buffer/length couple.&]
[s0; See the source code (RC4.h) for details.&]
[s0; ]