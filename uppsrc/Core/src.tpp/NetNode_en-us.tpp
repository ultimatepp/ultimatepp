topic "NetNode";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 NetNode]]}}&]
[s3; &]
[s1;:NetNode`:`:class: [@(0.0.255)3 class][3 _][*3 NetNode][3 _:_][@(0.0.255)3 private][3 _][*@3;3 M
oveable][@(0.0.255)3 <][*3 NetNode][@(0.0.255)3 >][3 _]&]
[s6;l0;%% WIN32 specific&]
[s9;%% This class represents single network node (network resource). 
It is [^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ moveable] and 
has deep copy.&]
[s3;%% &]
[s0;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor Detail]]}}&]
[s3; &]
[s5;:NetNode`:`:NetNode`(`): [* NetNode]()&]
[s2;%% Default constructor.&]
[s3; &]
[s4; &]
[s5;:NetNode`:`:NetNode`(const NetNode`&`): [* NetNode]([@(0.0.255) const]_[* NetNode][@(0.0.255) `&
]_[*@3 s])&]
[s2;%% Deep copy constructor.&]
[s3;%% &]
[s0;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Member List]]}}&]
[s3; &]
[s5;:NetNode`:`:GetName`(`)const: [_^String^ String]_[* GetName]()_[@(0.0.255) const]&]
[s2;%% Returns the name of network node (e.g. for displaying in GUI).&]
[s3; &]
[s4; &]
[s5;:NetNode`:`:GetPath`(`)const: [_^String^ String]_[* GetPath]()_[@(0.0.255) const]&]
[s2;%% If node represents a disk share, it returns a path to this 
share (like `\`\SERVER`\DATA). FindFile can be used to retrieve 
the list of files on this path (you have to add a file mask). 
If this method returns empty String, node is not a disk share.&]
[s3; &]
[s4; &]
[s5;:NetNode`:`:Enum`(`)const: [_^Array^ Array][@(0.0.255) <][_^NetNode^ NetNode][@(0.0.255) >
]_[* Enum]()_[@(0.0.255) const]&]
[s2;%% Returns the array of subnodes.&]
[s3; &]
[s4; &]
[s5;:NetNode`:`:Serialize`(Stream`&`): [@(0.0.255) void]_[* Serialize]([_^Stream^ Stream][@(0.0.255) `&
]_[*@3 s])&]
[s2;%% Serializes the Node.&]
[s3; &]
[s4; &]
[s5;:NetNode`:`:EnumRoot`(`): [@(0.0.255) static] [_^Array^ Array][@(0.0.255) <][_^NetNode^ N
etNode][@(0.0.255) >]_[* EnumRoot]()&]
[s2;%% Enumerates root nodes.&]
[s3; &]
[s4; &]
[s5;:NetNode`:`:operator`=`(const NetNode`&`): [_^NetNode^ NetNode][@(0.0.255) `&]_[* opera
tor`=]([@(0.0.255) const]_[_^NetNode^ NetNode][@(0.0.255) `&]_[*@3 s])&]
[s2;%% Deep copy assignment.&]
[s3; &]
[s0; ]