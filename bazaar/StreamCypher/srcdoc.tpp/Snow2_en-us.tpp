topic "Snow2 Encoder";
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s0;%- [*R6 Snow2 Encoder]&]
[s0; &]
[s0; Snow2 [^topic`:`/`/StreamCypher`/srcdoc`/StreamCryptography`$en`-us^ Stream 
Encoder] is somehow more complicated as RC4 one. but has a strong 
security, unviolated up do current days.&]
[s0; The implementation choosen here is the [/ fast ]version of original 
reference implementation code by its creators.&]
[s0; The complete algorhithm description can be found [^http`:`/`/en`.wikipedia`.org`/wiki`/SNOW^ h
ere].&]
[s0; One small caveat of snow2 algorythm is the need of a key of 
exactly 128 OR 256 bits (16 OR 32 bytes) length.&]
[s0; &]
[s0; The usage is quite simple :&]
[s0; &]
[s0;l160; [/ #include <StreamCypher/StreamCypher.h>]&]
[s0;l160;/ &]
[s0;l160; [/ String EncodeWithSnow2(String key, String data)]&]
[s0;l160; [/ `{]&]
[s0;l160; [/ -|Snow2 snow2(key);]&]
[s0;l160; [/ -|return snow2.Encode(data);]&]
[s0;l160; [/ `}]&]
[s0; &]
[s0; There are also encoding functions for in`-place buffer encoding 
and for out`-of`-place buffer encoding; you can also give the 
key as a buffer/length couple.&]
[s0; See the source code (Snow2.h) for details.&]
[s0; ]