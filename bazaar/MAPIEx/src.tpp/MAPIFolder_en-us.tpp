topic "class MAPIFolder : public MAPIObject";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 MAPIFolder]]}}&]
[s0;i448;kKO9;@(0.0.255)0 &]
[s0;i448;a25;kKO9;:MAPIFolder`:`:class: [@(0.0.255)3 class][3 _][*3 MAPIFolder][3 _:_][@(0.0.255)3 p
ublic][3 _][*@3;3 MAPIObject]&]
[s9; Outlook folder.&]
[s3;2 &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s4; &]
[s5;:MAPIFolder`:`:MAPIFolder`(`): [* MAPIFolder]()&]
[s2;%% Default constructor&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s4; &]
[s5;:MAPIFolder`:`:GetName`(`): [_^String^ String]_[* GetName]()&]
[s2;%% Gets folder name.&]
[s3; &]
[s4; &]
[s5;:MAPIFolder`:`:OpenSubFolder`(const String`&`,MAPIFolder`&`): [@(0.0.255) bool]_[* Op
enSubFolder]([@(0.0.255) const]_[_^String^ String `&][*@3 subFolderName], 
[_^MAPIFolder^ MAPIFolder]_`&[*@3 subFolder])&]
[s2;%% Opens [%-*@3 subFolderName] under [%-*@3 subFolder].&]
[s3;%% &]
[s4; &]
[s5;:MAPIFolder`:`:CreateSubFolder`(const String`&`,MAPIFolder`&`): [@(0.0.255) bool]_[* C
reateSubFolder]([@(0.0.255) const]_[_^String^ String]_`&[*@3 subFolderName], 
[_^MAPIFolder^ MAPIFolder]_`&[*@3 subFolder])&]
[s2;%% Creates a folder named [%-*@3 subFolderName] under [%-*@3 subFolder].&]
[s3;%% &]
[s4; &]
[s5;:MAPIFolder`:`:DeleteSubFolder`(const String`&`): [@(0.0.255) bool]_[* DeleteSubFolde
r]([@(0.0.255) const]_[_^String^ String]_`&[*@3 subFolder])&]
[s2;%% Deletes a subfolder named [%-*@3 subFolder].&]
[s3;%% &]
[s4; &]
[s5;:MAPIFolder`:`:DeleteSubFolder`(MAPIFolder`&`): [@(0.0.255) bool]_[* DeleteSubFolder](
[_^MAPIFolder^ MAPIFolder]_`&[*@3 folder])&]
[s2;%% Deletes [%-*@3 folder].subfolder.&]
[s3;%% &]
[s4; &]
[s5;:MAPIFolder`:`:GetCount`(`): [@(0.0.255) int]_[* GetCount]()&]
[s2;%% Gets the number of items in a folder.&]
[s3; &]
[s4; &]
[s5;:MAPIFolder`:`:SortContents`(ULONG`,ULONG`): [@(0.0.255) bool]_[* SortContents](ULONG
_[*@3 ulSortParam]_`=_TABLE`_SORT`_ASCEND, ULONG_[*@3 ulSortField]_`=_PR`_MESSAGE`_DE
LIVERY`_TIME)&]
[s2;%% Sorts folder items following [%-*@3 ulSortParam] criteria and 
using [%-*@3 ulSortField] column.&]
[s3;%% &]
[s4; &]
[s5;:MAPIFolder`:`:SetUnreadOnly`(bool`): [@(0.0.255) bool]_[* SetUnreadOnly]([@(0.0.255) b
ool]_[*@3 bUnreadOnly]_`=_[@(0.0.255) true])&]
[s2;%% If [%-*@3 bUnreadOnly ]is true only unread items are considered.&]
[s3;%% &]
[s4; &]
[s5;:MAPIFolder`:`:GetNextMessage`(MAPIMessage`&`): [@(0.0.255) bool]_[* GetNextMessage](
[_^MAPIMessage^ MAPIMessage][@(0.0.255) `&]_[*@3 message])&]
[s2;%% Returns in [%-*@3 message] the next message.&]
[s2;%% If there are no more messages it returns false.&]
[s3;%% &]
[s4; &]
[s5;:MAPIFolder`:`:GetNextContact`(MAPIContact`&`): [@(0.0.255) bool]_[* GetNextContact](
[_^MAPIContact^ MAPIContact][@(0.0.255) `&]_[*@3 contact])&]
[s2;%% Returns in [%-*@3 contact] the next contact.&]
[s2;%% If there are no more contacts it returns false.&]
[s3;%% &]
[s4; &]
[s5;:MAPIFolder`:`:GetNextAppointment`(MAPIAppointment`&`): [@(0.0.255) bool]_[* GetNextA
ppointment]([_^MAPIAppointment^ MAPIAppointment][@(0.0.255) `&]_[*@3 appointment])&]
[s2;%% Returns in [%-*@3 appointment ]the next appointment.&]
[s2;%% If there are no more appointments it returns false.&]
[s3;%% &]
[s4; &]
[s5;:MAPIFolder`:`:GetNextSubFolder`(MAPIFolder`&`,String`&`): [@(0.0.255) bool]_[* GetNe
xtSubFolder]([_^MAPIFolder^ MAPIFolder][@(0.0.255) `&]_[*@3 folder], 
[_^String^ String][@(0.0.255) `&]_[*@3 strFolder])&]
[s2;%% Returns in [%-*@3 folder] the next subfolder and in [%-*@3 strFolder 
]its name.&]
[s2;%% If there are no more sub folders it returns false.&]
[s3;%% &]
[s4; &]
[s5;:MAPIFolder`:`:DeleteMessage`(MAPIMessage`&`): [@(0.0.255) bool]_[* DeleteMessage]([_^MAPIMessage^ M
APIMessage][@(0.0.255) `&]_[*@3 message])&]
[s2;%% Deletes the message [%-*@3 message].&]
[s3;%% &]
[s4; &]
[s5;:MAPIFolder`:`:CopyMessage`(MAPIMessage`&`,MAPIFolder`&`): [@(0.0.255) bool]_[* CopyM
essage]([_^MAPIMessage^ MAPIMessage][@(0.0.255) `&]_[*@3 message], 
[_^MAPIFolder^ MAPIFolder][@(0.0.255) `&]_[*@3 pFolderDest])&]
[s2;%% Copies the message [%-*@3 message] to folder [%-*@3 pFolderDest].&]
[s3;%% &]
[s4; &]
[s5;:MAPIFolder`:`:MoveMessage`(MAPIMessage`&`,MAPIFolder`&`): [@(0.0.255) bool]_[* MoveM
essage]([_^MAPIMessage^ MAPIMessage][@(0.0.255) `&]_[*@3 message], 
[_^MAPIFolder^ MAPIFolder][@(0.0.255) `&]_[*@3 pFolderDest])&]
[s2;%% Moves the message [%-*@3 message] to folder [%-*@3 pFolderDest].&]
[s3;%% &]
[s4; &]
[s5;:MAPIFolder`:`:DeleteContact`(MAPIContact`&`): [@(0.0.255) bool]_[* DeleteContact]([_^MAPIContact^ M
APIContact][@(0.0.255) `&]_[*@3 contact])&]
[s2;%% Deletes the contact [%-*@3 contact].&]
[s3;%% &]
[s4; &]
[s5;:MAPIFolder`:`:DeleteAppointment`(MAPIAppointment`&`): [@(0.0.255) bool]_[* DeleteApp
ointment]([_^MAPIAppointment^ MAPIAppointment][@(0.0.255) `&]_[*@3 appointment])&]
[s2;%% Deletes the appointment [%-*@3 appointment].&]
[s3;%% &]
[s0;%% ]