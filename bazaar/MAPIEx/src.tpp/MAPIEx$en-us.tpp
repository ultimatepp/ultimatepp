topic "MAPIEx";
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
[ {{10000@(113.42.0) [s0;:Dl`:`:class:%% [*@7;4 MAPIEx]]}}&]
[s3; &]
[s1;:MAPIEx`:`:class: [@(0.0.255)3 class][3 _][*3 MAPIEx]&]
[s9;%% MAPIEx base class.&]
[s3;2 &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s4; &]
[s5;:MAPIEx`:`:MAPIEx`(`): [* MAPIEx]()&]
[s2;%% Default constructor.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s4; &]
[s5;:MAPIEx`:`:Login`(const String`,DWORD`,bool`): [@(0.0.255) bool]_[* Login]([@(0.0.255) c
onst]_[_^String^ String]_[*@3 szProfileName]_`=_NULL, DWORD_[*@3 dwFlags]_`=_[@3 0], 
[@(0.0.255) bool]_[*@3 bInitAsService]_`=_[@(0.0.255) false])&]
[s2;%% MAPI login function.&]
[s2;%% To change default parameters check its use [^http`:`/`/msdn`.microsoft`.com`/en`-us`/library`/cc815545`.aspx^ h
ere].&]
[s3;%% &]
[s4; &]
[s5;:MAPIEx`:`:GetProfileName`(`): [_^String^ String]_[* GetProfileName]()&]
[s2;%% Gets the Outlook profile name.&]
[s3; &]
[s4; &]
[s5;:MAPIEx`:`:GetProfileEmail`(`): [_^String^ String]_[* GetProfileEmail]()&]
[s2;%% Gets the email of the actual Outlook profile.&]
[s3; &]
[s4; &]
[s5;:MAPIEx`:`:OpenMessageStore`(int`,ULONG`): [@(0.0.255) bool]_[* OpenMessageStore]([@(0.0.255) i
nt]_[*@3 nIndex], ULONG_[*@3 ulFlags][@(0.0.255) `=]MAPI`_MODIFY_`|_MAPI`_NO`_CACHE)&]
[s2;%% Opens [%-*@3 nIndex] message store with flags [%-*@3 ulFlags].&]
[s3;%% &]
[s4; &]
[s5;:MAPIEx`:`:OpenMessageStore`(String`,ULONG`): [@(0.0.255) bool]_[* OpenMessageStore](
[_^String^ String]_[*@3 szStore][@(0.0.255) `=]NULL, ULONG_[*@3 ulFlags][@(0.0.255) `=]MAPI
`_MODIFY_`|_MAPI`_NO`_CACHE)&]
[s2;%% Opens message store with name [%-*@3 szStore] with flags [%-*@3 ulFlags].&]
[s3;%% &]
[s4; &]
[s5;:MAPIEx`:`:GetMessageStoreCount`(`): [@(0.0.255) int]_[* GetMessageStoreCount]()&]
[s2;%% Gets the messages number of opened message store.&]
[s3; &]
[s4; &]
[s5;:MAPIEx`:`:GetMessageStoreProperties`(int`,String`&`,bool`&`): [@(0.0.255) bool]_[* G
etMessageStoreProperties]([@(0.0.255) int]_[*@3 nIndex], [_^String^ String][@(0.0.255) `&
]_[*@3 strName], [@(0.0.255) bool`&]_[*@3 bDefaultStore])&]
[s2;%% Gets the properties of [%-*@3 nIndex] message store like its 
name in [%-*@3 strName] and if it is the default store in [%-*@3 bDefaultStore].&]
[s3;%% &]
[s4;%% &]
[s5;:MAPIEx`:`:OpenFolder`(const String`&`,MAPIFolder`&`): [@(0.0.255) bool]_[* OpenFolde
r]([@(0.0.255) const]_[_^String^ String `&][*@3 folderName], [_^MAPIFolder^ MAPIFolder]_`&
[*@3 folder])&]
[s2;%% Sets in [%-*@3 folder] the folder with name [%-*@3 folderName].&]
[s3;%% &]
[s4; &]
[s5;:MAPIEx`:`:OpenRootFolder`(MAPIFolder`&`): [@(0.0.255) bool]_[* OpenRootFolder]([_^MAPIFolder^ M
APIFolder]_`&[*@3 folder])&]
[s2;%% Sets in [%-*@3 folder ]the root folder of the folder tree.&]
[s3;%% &]
[s4; &]
[s5;:MAPIEx`:`:OpenInbox`(MAPIFolder`&`): [@(0.0.255) bool]_[* OpenInbox]([_^MAPIFolder^ MA
PIFolder]_`&[*@3 folder])&]
[s2;%% Sets in [%-*@3 folder ]the Inbox folder.&]
[s3;%% &]
[s4; &]
[s5;:MAPIEx`:`:OpenOutbox`(MAPIFolder`&`): [@(0.0.255) bool]_[* OpenOutbox]([_^MAPIFolder^ M
APIFolder]_`&[*@3 folder])&]
[s2;%% Sets in  [%-*@3 folder ]the Outbox folder.&]
[s3;%% &]
[s4; &]
[s5;:MAPIEx`:`:OpenSentItems`(MAPIFolder`&`): [@(0.0.255) bool]_[* OpenSentItems]([_^MAPIFolder^ M
APIFolder]_`&[*@3 folder])&]
[s2;%% Sets in  [%-*@3 folder ]the Sent Items folder.&]
[s3;%% &]
[s4; &]
[s5;:MAPIEx`:`:OpenDeletedItems`(MAPIFolder`&`): [@(0.0.255) bool]_[* OpenDeletedItems]([_^MAPIFolder^ M
APIFolder]_`&[*@3 folder])&]
[s2;%% Sets in  [%-*@3 folder ]the Deleted Items folder.&]
[s3;%% &]
[s4; &]
[s5;:MAPIEx`:`:OpenContacts`(MAPIFolder`&`): [@(0.0.255) bool]_[* OpenContacts]([_^MAPIFolder^ M
APIFolder]_`&[*@3 folder])&]
[s2;%% Sets in  [%-*@3 folder ]the Contacts.&]
[s3;%% &]
[s4; &]
[s5;:MAPIEx`:`:OpenDrafts`(MAPIFolder`&`): [@(0.0.255) bool]_[* OpenDrafts]([_^MAPIFolder^ M
APIFolder]_`&[*@3 folder])&]
[s2;%% Sets in  [%-*@3 folder ]the Drafts folder.&]
[s3;%% &]
[s4; &]
[s5;:MAPIEx`:`:OpenCalendar`(MAPIFolder`&`): [@(0.0.255) bool]_[* OpenCalendar]([_^MAPIFolder^ M
APIFolder]_`&[*@3 folder])&]
[s2;%% Sets in  [%-*@3 folder ]the Calendar.&]
[s3;%% &]
[s4; &]
[s5;:MAPIEx`:`:Notify`(LPNOTIFCALLBACK`,LPVOID`,ULONG`): [@(0.0.255) bool]_[* Notify](LPN
OTIFCALLBACK_[*@3 OnNewEvent], LPVOID_[*@3 lpvContext], ULONG_[*@3 ulEventMask])&]
[s2;%% Sets a function as a callback to receive Outlook events.&]
[s2;%% This function has to be declared:&]
[s2;%% [C long CALLBACK ][%-*C@3 OnNewEvent][C (LPVOID lpvContext, ULONG 
cNotification, LPNOTIFICATION lpNotifications)] &]
[s2;%% When an event occurs in Outlook the [%-*@3 OnNewEvent] function 
is called with the pointer included in [%-*@3 lpvContext]. cNotification 
will include the reason of the event.&]
[s2;%% The possible sources of this callback are included in [%-*@3 ulEventMask] 
mask. Some sources are:&]
[s2;i150;O0;%% fnevNewMail &]
[s2;i150;O0;%% fnevObjectCreated&]
[s2;i150;O0;%% fnevObjectDeleted&]
[s2;i150;O0;%% fnevObjectModified&]
[s2;i150;O0;%% fnevObjectMoved&]
[s2;i150;O0;%% fnevObjectCopied&]
[s2;i150;O0;%% fnevSearchComplete&]
[s2;%% &]
[s2;%% Remember that a new mail event will probably arrive a little 
while after the message arrives depending on your Exchange Server`'s 
setup.&]
[s2;%% The callback is called once per event (if four emails are 
deleted at the same time, four events will be received.&]
[s3;%% &]
[s4; &]
[s5;:MAPIEx`:`:SelectContacts`(Vector`<String`>`&`,const String`): [@(0.0.255) bool]_[* S
electContacts]([_^Vector^ Vector]<[_^String^ String]>_`&[*@3 emails], 
[@(0.0.255) const]_[_^String^ String]_[*@3 caption])&]
[s2;%% Shows a window to choose emails from Contacts. All selected 
emails will be returned in [%-*@3 emails] Vector. The window title 
is in [%-*@3 caption].&]
[s3;%% &]
[s4; ]