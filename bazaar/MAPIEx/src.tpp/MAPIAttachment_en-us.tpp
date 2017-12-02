topic "class MAPIAttachment: public MAPIObject";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 MAPIAttachment]]}}&]
[s0;i448;kKO9;@(0.0.255)0 &]
[s0;i448;a25;kKO9;:MAPIAttachment`:`:class: [@(0.0.255)3 class][3 _][*3 MAPIAttachment][3 _:_][@(0.0.255)3 p
ublic][3 _][*@3;3 MAPIObject]&]
[s9; Outlook message attachment.&]
[s3;2 &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s4; &]
[s5;:MAPIAttachment`:`:MAPIAttachment`(`): [* MAPIAttachment]()&]
[s2;%% Default constructor.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s4; &]
[s5;:MAPIAttachment`:`:GetDisplayName`(`): [_^String^ String]_[* GetDisplayName]()&]
[s2;%% Gets the display name of the message store.&]
[s3; &]
[s4; &]
[s5;:MAPIAttachment`:`:GetFileName`(`): [_^String^ String]_[* GetFileName]()&]
[s2;%% Gets the suggested file name for saving the attachment. File 
name length is restricted to eight characters plus a three`-character 
extension.&]
[s3; &]
[s4; &]
[s5;:MAPIAttachment`:`:GetLongFileName`(`): [_^String^ String]_[* GetLongFileName]()&]
[s2;%% Gets the suggested file name for saving the attachment.&]
[s3; &]
[s4; &]
[s5;:MAPIAttachment`:`:SetDisplayName`(const String`&`): [@(0.0.255) bool]_[* SetDisplayN
ame]([@(0.0.255) const]_[_^String^ String]_`&[*@3 szDisplayName])&]
[s2;%% Sets with [%-*@3 szDisplayName] the display name of the message 
store.&]
[s3;%% &]
[s4; &]
[s5;:MAPIAttachment`:`:SetFileName`(const String`&`): [@(0.0.255) bool]_[* SetFileName]([@(0.0.255) c
onst]_[_^String^ String]_`&[*@3 szFileName])&]
[s2;%% Sets with [%-*@3 szFileName ]the suggested file name for saving 
the attachment. File name length is restricted to eight characters 
plus a three`-character extension.&]
[s3;%% &]
[s4; &]
[s5;:MAPIAttachment`:`:SetLongFileName`(const String`&`): [@(0.0.255) bool]_[* SetLongFil
eName]([@(0.0.255) const]_[_^String^ String]_`&[*@3 szLongFileName])&]
[s2;%% Sets with [%-*@3 szLongFileName] the suggested file name for 
saving the attachment.&]
[s3;%% &]
[s4; &]
[s5;:MAPIAttachment`:`:LoadAttachment`(const String`&`): [@(0.0.255) bool]_[* LoadAttachm
ent]([@(0.0.255) const]_[_^String^ String]_`&[*@3 szPath])&]
[s2;%% Loads an attachment from file [%-*@3 szPath].&]
[s3;%% &]
[s4; &]
[s5;:MAPIAttachment`:`:SaveAttachment`(const String`&`): [@(0.0.255) bool]_[* SaveAttachm
ent]([@(0.0.255) const]_[_^String^ String]_`&[*@3 szPath])&]
[s2;%% Saves an attachment to file [%-*@3 szPath].&]
[s3;%% &]
[s4; &]
[s5;:MAPIAttachment`:`:OpenStream`(bool`): [@(0.0.255) bool]_[* OpenStream]([@(0.0.255) boo
l]_[*@3 bCreate]_`=_[@(0.0.255) false])&]
[s2;%% Opens an attachment. If [%-*@3 bCreate]. is true, a new attachment 
is created.&]
[s3;%% &]
[s4; &]
[s5;:MAPIAttachment`:`:Read`(BYTE`*`,int`): [@(0.0.255) int]_[* Read](BYTE[@(0.0.255) `*]_[*@3 p
Data], [@(0.0.255) int]_[*@3 nCount])&]
[s2;%% Reads [%-*@3 nCount ]bytes from last attachment stream opened 
with OpenStream(false) and sets it in [%-*@3 pData].&]
[s3;%% &]
[s4; &]
[s5;:MAPIAttachment`:`:Write`(BYTE`*`,int`): [@(0.0.255) int]_[* Write](BYTE[@(0.0.255) `*]_
[*@3 pData], [@(0.0.255) int]_[*@3 nCount])&]
[s2;%% Writes [%-*@3 nCount] bytes from [%-*@3 pData] to last attachment 
stream opened with OpenStream(true).&]
[s3;%% &]
[s4; &]
[s5;:MAPIAttachment`:`:CloseStream`(`): [@(0.0.255) void]_[* CloseStream]()&]
[s2;%% Closes last opened stream.&]
[s3; &]
[s0;%% ]