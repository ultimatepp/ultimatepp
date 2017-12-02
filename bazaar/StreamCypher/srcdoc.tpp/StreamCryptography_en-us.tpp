topic "Stream Cryptography";
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s0;%- [*R6 Stream Cyptography]&]
[s0;/%- &]
[s0; There are 2 main types of cryptography, represented by [* block 
encoders] and [* stream encoders].&]
[s0; &]
[s0; [* Block encoders], like AES, are designed to work with fixed`-size 
data blocks, for example of 256 bytes size.&]
[s0; [* Stream encoders], on the other side, are designed to encode 
continuous byte streams, without limit of  block size.&]
[s0; &]
[s0; Formers are usually used for media encryption (as disk sectors, 
for example) or other kinds of data which structure is represented 
by fixed size of data chunks; latter are better suited for contiuous 
data streams, for example audio and video streams, or variable`-sized 
data like disk files.&]
[s0; &]
[s0; [* Stream encoders] can, of course, be used for block encoding, 
but they`'re usually less efficient and believed less secure 
than block encoders; using [* block encoders] for streaming data 
is also possible, but with the caveat of having to wait for complete 
data blocks before proceeding or to pad partial data blocks with 
some (usually null) dummy data.&]
[s0; &]
[s0; Another differentiation between encoders is given by [* symmetric 
]and [* asymmetric ]ones.&]
[s0; Without going too deep inside matter, the main difference between 
both is that symmetric encoders use the same key for encoding 
and decoding, while asymmetric encoders don`'t.&]
[s0; &]
[s0; Here we present 2 [* Symmetric Stream encoders], one slightly 
faster but by far less secure [^topic`:`/`/StreamCypher`/srcdoc`/RC4`$en`-us^ RC4],
 and the other slightly slower but giving very strong encoding, 
[^topic`:`/`/StreamCypher`/srcdoc`/Snow2`$en`-us^ Snow2].&]
[s0; As the only advantage or RC4 over Snow2 is the smaller code 
footprint and a (really small) speed advantage, we suggest Snow2 
usage on all case where security is the main need; RC4 is, for 
example, used for WEP wireless encryption which is known to be 
broken with ease.]