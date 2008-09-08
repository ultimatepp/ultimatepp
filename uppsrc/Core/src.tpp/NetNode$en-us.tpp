topic "NetNode";[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[l416;2 $$7,7#55548704457842300043401641954952:nested`-desc]
[l288;i448;a25;kO9;*2 $$8,8#64691275497409617375831514634295:nested`-class]
[{_}%EN-US 
[s3; [%00-00 NetNode]&]
[s1;K@(0.0.255) &]
[s1;K:`:`:NetNode`:`:class: [%00-00@(0.0.255) class][%00-00 _][%00-00@0 NetNode][%00-00 _:_][%00-00@(0.0.255) p
rivate][%00-00 _][%00-00^`:`:Moveable^ Moveable][%00-00 <][%00-00@0 NetNode][%00-00 >_]&]
[s2; This class represents single network node (network resource). 
It moveable and has deep copy.&]
[s0; &]
[s5;K:`:`:NetNode`:`:GetName`(`)const:@(0.0.255) [%00-00^`:`:String^@(64) String][%00-00@(64) _
][%00-00@0 GetName][%00-00@(64) ()_][%00-00 const]&]
[s2; Returns the name of network node (e.g. for displaying in GUI).&]
[s0; &]
[s5;K:`:`:NetNode`:`:GetPath`(`)const:@(0.0.255) [%00-00^`:`:String^@(64) String][%00-00@(64) _
][%00-00@0 GetPath][%00-00@(64) ()_][%00-00 const]&]
[s2; If node represents a disk share, it returns a path to this share 
(like `\`\SERVER`\DATA). FindFile can be used to retrieve the 
list of files on this path (you have to add a file mask). If 
this method returns empty String, node is not a disk share.&]
[s0; &]
[s5;K:`:`:NetNode`:`:Enum`(`)const:@(0.0.255) [%00-00^`:`:Array^@(64) Array][%00-00@(64) <
][%00-00^`:`:NetNode^@(64) NetNode][%00-00@(64) >_][%00-00@0 Enum][%00-00@(64) ()_][%00-00 c
onst]&]
[s2; Returns the array of subnodes.&]
[s0; &]
[s5;K:`:`:NetNode`:`:Serialize`(`:`:Stream`&`): [%00-00@(0.0.255) void][%00-00 _][%00-00@0 S
erialize][%00-00 (][%00-00^`:`:Stream^ Stream][%00-00 `&_][%00-00@3 s][%00-00 )]&]
[s2; Serializes the Node.&]
[s0; &]
[s5;K:`:`:NetNode`:`:EnumRoot`(`): [%00-00 static ][%00-00^`:`:Array^ Array][%00-00 <][%00-00^`:`:NetNode^ N
etNode][%00-00 >_][%00-00@0 EnumRoot][%00-00 ()]&]
[s2; Enumerates root nodes.&]
[s0; &]
[s5;K:`:`:NetNode`:`:`:`:NetNode`(`): [%00-00@0 NetNode][%00-00 ()]&]
[s2; Default constructor.&]
[s0; &]
[s5;K:`:`:NetNode`:`:`:`:NetNode`(const`:`:NetNode`&`): [%00-00@0 NetNode][%00-00 (][%00-00@(0.0.255) c
onst][%00-00 _][%00-00@0 NetNode][%00-00 `&_][%00-00@3 s][%00-00 )]&]
[s2; Deep copy constructor.&]
[s0; &]
[s5;K:`:`:NetNode`:`:operator`=`(const`:`:NetNode`&`): [%00-00^`:`:NetNode^ NetNode][%00-00 `&
_][%00-00@0 operator`=][%00-00 (][%00-00@(0.0.255) const][%00-00 _][%00-00^`:`:NetNode^ Net
Node][%00-00 `&_][%00-00@3 s][%00-00 )]&]
[s2; Deep copy assignment.&]
[s0; &]
[s0; ]