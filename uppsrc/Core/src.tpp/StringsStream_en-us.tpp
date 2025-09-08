topic "StringsStream[In|Out]";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 StringsStreamOut]]}}&]
[s1;@(0.0.255)3 &]
[s1;:Upp`:`:StringsStreamOut: [@(0.0.255)3 class][3  ][*3 StringsStreamOut][3  
][@(0.0.255)3 :][3  ][@(0.0.255)3 public][3  Stream]&]
[s2; [3 Output stream that stores data into Vector<String> output chunks. 
This is useful when handling very large serializations `- it 
allows output to be > 2GB (which is String hard size limit because 
of optimisation concerns) but also splits allocation into smaller 
chunks which is potentially faster for repeated operation.]&]
[s3; &]
[s0;i448;a25;kKO9;:noref:@(0.0.255) &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:StringsStreamOut`:`:StringsStreamOut`(int`): [* StringsStreamOut]([@(0.0.255) i
nt] [*@3 part`_size] [@(0.0.255) `=] [@N 1024`*1024 `- 256])&]
[s2;%% Constructor `- [*@3 pa][%-*@3 rt`_size] is the size of data chunk.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StringsStreamOut`:`:PickResult`(`): Vector<String> [* PickResult]()&]
[s2;%% Returns the output data. Can be called just once for any instance.&]
[s3; &]
[s0;%% &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 StringsStreamIn]]}}&]
[s3; &]
[s1;:Upp`:`:StringsStreamIn: [@(0.0.255)3 class][3  ][*3 StringsStreamIn][3  
][@(0.0.255)3 :][3  ][@(0.0.255)3 public][3  Stream]&]
[s2;%% Input stream corresponding to StringsStreamOut `- reads data 
from multiple chunks.&]
[s0;i448;a25;kKO9;:noref:@(0.0.255) &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:StringsStreamIn`:`:StringsStreamIn`(const Vector`&`): [* StringsStreamIn]([@(0.0.255) c
onst] Vector<String>[@(0.0.255) `&] [*@3 part])&]
[s2;%% Creates input stream for [%-*@3 part] data chunks.&]
[s3; &]
[s0;%% ]]