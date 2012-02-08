topic "class Vector";
[ $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9; $$1,0#37138531426314131252341829483380:structitem]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:codeitem]
[{_} 
[s1;:noref: [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>&]
[s1;:Vector`:`:class: [@(0.0.255) class]_[* Vector]&]
[s2;%% Class template implementing a very basic Vector container. 
The idea behind reimplementing such basic thing is to avoid including 
a huge portion of U`+`+/STL, which slows down compilation and 
also add dependencies.&]
[s4; &]
[s5;:Vector`:`:Vector`(`): [* Vector]()&]
[s2;%% Default constructor&]
[s3; &]
[s4; &]
[s5;:Vector`:`:`~Vector`(`): [@(0.0.255) `~][* Vector]()&]
[s2;%% Destructor&]
[s3; &]
[s4; &]
[s5;:Vector`:`:GetCount`(`)const: [@(0.0.255) int]_[* GetCount]()[@(0.0.255) const]&]
[s2;%% Returns the number of elements stored&]
[s3; &]
[s4; &]
[s5;:Vector`:`:operator`[`]`(int`): [*@4 T][@(0.0.255) `&]_[* operator`[`]]([@(0.0.255) int]_
[*@3 n])&]
[s2;%% Returns reference to element stored at [%-*@3 n]th position&]
[s3;%% &]
[s4; &]
[s5;:Vector`:`:operator`[`]`(int`)const: [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[* operato
r`[`]]([@(0.0.255) int]_[*@3 n])_[@(0.0.255) const]&]
[s2;%% Returns const reference to element stored at [%-*@3 n]th position&]
[s3;%% &]
[s4; &]
[s5;:Vector`:`:Add`(`): [*@4 T][@(0.0.255) `&]_[* Add]()&]
[s2;%% Adds default constructed element at the end of the vector&]
[s3; &]
[s4; &]
[s5;:Vector`:`:Add`(const T`&`): [*@4 T][@(0.0.255) `&]_[* Add]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 x])&]
[s2;%% Adds an element copy constructed from [%-*@3 x] at the end of 
the vector&]
[s3;%% &]
[s4; &]
[s5;:Vector`:`:AddUnique`(const T`&`): [*@4 T][@(0.0.255) `&]_[* AddUnique]([@(0.0.255) const
]_[*@4 T][@(0.0.255) `&]_[*@3 v])&]
[s2;%% Adds element [%-*@3 v] at the end of the vector if it doesn`'t 
exist already&]
[s3;%% &]
[s4; &]
[s5;:Vector`:`:Find`(const T`&`)const: [@(0.0.255) int]_[* Find]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 v])_[@(0.0.255) const]&]
[s2;%% Returns position of [%-*@3 v] in the vector, or `-1 if not present&]
[s3;%% &]
[s4; &]
[s5;:Vector`:`:Resize`(int`): [@(0.0.255) void]_[* Resize]([@(0.0.255) int]_[*@3 len])&]
[s2;%% Allocates memory for [%-*@3 len] fields. If the vector is longer, 
it is truncated.&]
[s3;%% &]
[s0;%% ]