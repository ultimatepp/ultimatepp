topic "NetNode";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a17;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[{_}%EN-US 
[s0;%- NetNode&]
[s1;K@(0.0.255) &]
[s1;K:`:`:NetNode`:`:class:%- [@(0.0.255) class][@(64) _]NetNode[@(64) _:_][@(0.0.255) private
][@(64) _][^`:`:Moveable^@(64) Moveable][@(64) <]NetNode[@(64) >_]&]
[s2; This class represents single network node (network resource). 
It moveable and has deep copy.&]
[s0; &]
[s5;K:`:`:NetNode`:`:GetName`(`)const:%- [^`:`:String^ String]_GetName[@(64) ()_][@(0.0.255) c
onst]&]
[s2; Returns the name of network node (e.g. for displaying in GUI).&]
[s0; &]
[s5;K:`:`:NetNode`:`:GetPath`(`)const:%- [^`:`:String^ String]_GetPath[@(64) ()_][@(0.0.255) c
onst]&]
[s2; If node represents a disk share, it returns a path to this share 
(like `\`\SERVER`\DATA). FindFile can be used to retrieve the 
list of files on this path (you have to add a file mask). If 
this method returns empty String, node is not a disk share.&]
[s0; &]
[s5;K:`:`:NetNode`:`:Enum`(`)const:%- [^`:`:Array^ Array]<[^`:`:NetNode^ NetNode]>_Enum[@(64) (
)_][@(0.0.255) const]&]
[s2; Returns the array of subnodes.&]
[s0; &]
[s5;K:`:`:NetNode`:`:Serialize`(`:`:Stream`&`):%- [@(0.0.255) void][@(64) _]Serialize[@(64) (
][^`:`:Stream^@(64) Stream][@(64) `&_][@3 s][@(64) )]&]
[s2; Serializes the Node.&]
[s0; &]
[s5;K:`:`:NetNode`:`:EnumRoot`(`):%- static [^`:`:Array^ Array]<[^`:`:NetNode^ NetNode]>_
EnumRoot[@(64) ()]&]
[s2; Enumerates root nodes.&]
[s0; &]
[s5;K:`:`:NetNode`:`:`:`:NetNode`(`):%- NetNode[@(64) ()]&]
[s2; Default constructor.&]
[s0; &]
[s5;K:`:`:NetNode`:`:`:`:NetNode`(const`:`:NetNode`&`):%- NetNode[@(64) (][@(0.0.255) con
st][@(64) _]NetNode[@(64) `&_][@3 s][@(64) )]&]
[s2; Deep copy constructor.&]
[s0; &]
[s5;K:`:`:NetNode`:`:operator`=`(const`:`:NetNode`&`):%- [^`:`:NetNode^ NetNode]`&_oper
ator`=[@(64) (][@(0.0.255) const][@(64) _][^`:`:NetNode^@(64) NetNode][@(64) `&_][@3 s][@(64) )
]&]
[s2; Deep copy assignment.&]
[s0; &]
[s0; ]