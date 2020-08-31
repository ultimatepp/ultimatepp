topic "TurtleServer";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 TurtleServer]]}}&]
[s2; &]
[s1;:Upp`:`:TurtleServer`:`:class: [@(0.0.255)3 class][3 _][*3 TurtleServer][3 _:_][@(0.0.255)3 p
ublic][3 _][*@3;3 VirtualGui]&]
[s2;#%% This class implements a remote gui virtualization server 
for U`+`+ applications. By utilizing the modern web technologies 
such as HTML`-5 canvas and websockets, TurtleServer allows U`+`+ 
gui applications to be accessed remotely via modern web browsers, 
or, possibly, via specialized client software that understands 
the Turtle wire protocol.&]
[s3;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:TurtleServer`:`:Bind`(const Upp`:`:String`&`): [_^Upp`:`:TurtleServer^ Turt
leServer][@(0.0.255) `&]_[* Bind]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 addr])&]
[s2;%% Sets the server bind address to [%-*@3 addr]. Default is `"0.0.0.0`". 
Returns `*this for method chaining.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:TurtleServer`:`:Host`(const Upp`:`:String`&`): [_^Upp`:`:TurtleServer^ Turt
leServer][@(0.0.255) `&]_[* Host]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 host])&]
[s2;%% Sets the host URL to [%-*@3 host]. Default URL is `"localhost`". 
Returns `*this for method chaining.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:TurtleServer`:`:Port`(int`): [_^Upp`:`:TurtleServer^ TurtleServer][@(0.0.255) `&
]_[* Port]([@(0.0.255) int]_[*@3 port])&]
[s2;%% Sets the connection port number. Default port number is 8888. 
Returns `*this for method chaining.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:TurtleServer`:`:MaxConnections`(int`): [_^Upp`:`:TurtleServer^ TurtleServer
][@(0.0.255) `&]_[* MaxConnections]([@(0.0.255) int]_[*@3 limit])&]
[s2;%% Sets a limit to the maximum number of concurrent client conntections. 
Default max. connection limit is 100. Returns `*this for method 
chaining.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:TurtleServer`:`:DebugMode`(bool`): [@(0.0.255) static] 
[@(0.0.255) void]_[* DebugMode]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% If true, the server will not spawn child processes (no forking). 
Useful for debugging purposes. This switch is only effective 
in DEBUG mode.&]
[s3;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s3; &]
[s5;:Upp`:`:TurtleServer`:`:TurtleServer`(`): [* TurtleServer]()&]
[s2;%% Default constructor. Initializes the server bind address to 
`"0.0.0.0`", the host URL to `"localhost`", and the connection 
port number to 8888.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:TurtleServer`:`:TurtleServer`(const Upp`:`:String`&`,int`): [* TurtleServer
]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 host], 
[@(0.0.255) int]_[*@3 port])&]
[s2;%% Constructor overload. Initializes the host URL and the connection 
port number to provided values.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:TurtleServer`:`:TurtleServer`(const Upp`:`:String`&`,Upp`:`:String`&`,int`): [* T
urtleServer]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 ip], 
[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 host], [@(0.0.255) int]_[*@3 port])&]
[s2;%% Constructor overload. Initializes the server bind adrress, 
the host URL and the connection port number to provided values.&]
[s3;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Function List]]}}&]
[s3;%% &]
[s5;:Upp`:`:RunTurtleGui`(Upp`:`:TurtleServer`&`,Upp`:`:Event`<`>`): [@(0.0.255) void]_
[* RunTurtleGui]([_^Upp`:`:TurtleServer^ TurtleServer][@(0.0.255) `&]_[*@3 gui], 
[_^Upp`:`:Event^ Event]<>_[*@3 app`_main])&]
[s2;%% Starts the Turtle GUI virtualization server and runs a U`+`+ 
GUI application over it.&]
[s3;%% &]
[s0;%% ]]