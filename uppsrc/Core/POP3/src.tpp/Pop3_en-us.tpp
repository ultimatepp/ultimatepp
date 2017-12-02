topic "POP3";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 Pop3]]}}&]
[s1;@(0.0.255)3 &]
[s1;:Pop3`:`:class: [@(0.0.255)3 class][3 _][*3 Pop3][3 _:_][@(0.0.255)3 public][3 _][*@3;3 TcpSocke
t]&]
[s2;%% Encapsulates the Post Office Protocol, Version 3 (POP3) as 
specified in the [^http`:`/`/tools`.ietf`.org`/html`/rfc1939^ RFC 
1939]. This class provides a simplified interface to a single 
POP3 mailbox.&]
[s2;%% Important: Pop3 indicies are 1 based! Thus the correct loop 
to obtain all messages is for(int i `= 1; i <`= pop3.GetMessageCount(); 
i`+`+).&]
[s2;%% &]
[s6;%% Requires OpenSSL library.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s0; &]
[s5;:Pop3`:`:Host`(const String`&`): [_^Pop3^ Pop3][@(0.0.255) `&]_[* Host]([@(0.0.255) const
]_[_^String^ String][@(0.0.255) `&]_[*@3 h])&]
[s2;%% Sets hostname. Returns `*this for method chaining.&]
[s3;%% &]
[s4; &]
[s5;:Pop3`:`:Port`(int`): [_^Pop3^ Pop3][@(0.0.255) `&]_[* Port]([@(0.0.255) int]_[*@3 p])&]
[s2;%% Sets port number. Returns `*this for method chaining.&]
[s3;%% &]
[s4; &]
[s5;:Pop3`:`:User`(const String`&`,const String`&`): [_^Pop3^ Pop3][@(0.0.255) `&]_[* User](
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 u], [@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 p])&]
[s2;%% Sets username and password. Returns `*this for method chaining.&]
[s3;%% &]
[s4; &]
[s5;:Pop3`:`:SSL`(bool`): [_^Pop3^ Pop3][@(0.0.255) `&]_[* SSL]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) t
rue])&]
[s2;%% Activates POP3S mode (using SSL). Requires Core/SSL package. 
Returns `*this for method chaining.&]
[s3;%% &]
[s4; &]
[s5;:Pop3`:`:Proxy`(const String`&`,int`): [_^Pop3^ Pop3][@(0.0.255) `&]_[* Proxy]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 host], [@(0.0.255) int]_[*@3 port])&]
[s2;%% Sets HTTP CONNECT proxy for connection.&]
[s3;%% &]
[s4; &]
[s5;:Pop3`:`:Proxy`(const char`*`): [_^Pop3^ Pop3][@(0.0.255) `&]_[* Proxy]([@(0.0.255) const
]_[@(0.0.255) char]_`*[*@3 p])&]
[s2;%% Sets HTTP CONNECT proxy for connection, parameter contains 
host and port separated by `':`'.&]
[s3;%% &]
[s4; &]
[s5;:Pop3`:`:ProxyAuth`(const String`&`,const String`&`): [_^Pop3^ Pop3][@(0.0.255) `&]_[* P
roxyAuth]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 u], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 p])&]
[s2;%% Sets the username and password for proxy..&]
[s3;%% &]
[s4; &]
[s5;:Pop3`:`:GetMessageCount`(`): [@(0.0.255) int]_[* GetMessageCount]()&]
[s2;%% Returns the number of currently available messages in the 
POP3 mailbox. Returns `-1 on failure.&]
[s3; &]
[s4; &]
[s5;:Pop3`:`:GetMessage`(int`): [_^String^ String]_[* GetMessage]([@(0.0.255) int]_[*@3 index
])&]
[s2;%% Retreives a message with the given [%-*@3 index] from the POP3 
mailbox. Returns Null on failure. First message has index 1, 
last GetMessageCount().&]
[s3;%% &]
[s4; &]
[s5;:Pop3`:`:GetMessageHeader`(int`): [_^String^ String]_[* GetMessageHeader]([@(0.0.255) i
nt]_[*@3 index])&]
[s2;%% Retreives the header section of a message with the given [%-*@3 index] 
from the POP3 mailbox. Returns Null on failure. First message 
has index 1, last GetMessageCount().&]
[s3;%% &]
[s4; &]
[s5;:Pop3`:`:GetMessageList`(ValueMap`&`): [@(0.0.255) bool]_[* GetMessageList]([_^ValueMap^ V
alueMap][@(0.0.255) `&]_[*@3 list])&]
[s2;%% Retrieves the list of currently available messages in the 
POP3 mailbox. [%-*@3 list] is a ValueMap containing messages indices 
and sizes as integer key and integer value pairs. Returns true 
on success.&]
[s3;%% &]
[s4; &]
[s5;:Pop3`:`:GetMessageUniqueId`(int`): [_^String^ String]_[* GetMessageUniqueId]([@(0.0.255) i
nt]_[*@3 index])&]
[s2;%% Retreives the unique identifier string of a message with the 
given [%-*@3 index] from the POP3 mailbox. Returns Null on failure.&]
[s3;%% &]
[s4; &]
[s5;:Pop3`:`:GetMessageUniqueIds`(ValueMap`&`): [@(0.0.255) bool]_[* GetMessageUniqueIds](
[_^topic`:`/`/Core`/src`/ValueMap`$en`-us`#ValueMap`:`:class^ ValueMap][@(0.0.255) `&
]_[*@3 uids])&]
[s2;%% Retrieves the unique identifier strings of currently available 
messages in the POP3 mailbox. [%-*@3 uids] is a ValueMap containing 
messages indices and unique identifiers as integer key and String 
value pairs. Returns true on success.&]
[s3;%% &]
[s4; &]
[s5;:Pop3`:`:RemoveMessage`(int`): [@(0.0.255) bool]_[* RemoveMessage]([@(0.0.255) int]_[*@3 i
ndex])&]
[s2;%% Marks a message with the given [%-*@3 index] to be deleted from 
the POP3 mailbox. Messages marked for deletion are only deleted 
by the POP3 server after a successful [^topic`:`/`/POP3`/src`/Pop3`$en`-us`#Pop3`:`:Logout`(`)^ L
ogout()]. If you want to unmark messages, you must invoke [^topic`:`/`/POP3`/src`/Pop3`$en`-us`#Pop3`:`:Undo`(`)^ U
ndo()] method before logging out. Returns true on success. &]
[s3;%% &]
[s4; &]
[s5;:Pop3`:`:Undo`(`): [@(0.0.255) bool]_[* Undo]()&]
[s2;%% Resets POP3 session. Useful for unmarking the messages marked 
to be deleted. This method must be invoked before any [^topic`:`/`/POP3`/src`/Pop3`$en`-us`#Pop3`:`:Logout`(`)^ L
ogout()]. Returns true on success.&]
[s3;%% &]
[s4; &]
[s5;:Pop3`:`:Noop`(`): [@(0.0.255) bool]_[* Noop]()&]
[s2;%% Sends a NOOP command to the POP3 server. Useful for keeping 
connections alive.&]
[s3; &]
[s4; &]
[s5;:Pop3`:`:Login`(`): [@(0.0.255) bool]_[* Login]()&]
[s2;%% Connect and login to the POP3 server. User name and password 
must be set. Uses APOP authentication mechanism, if available. 
Returns true on success.&]
[s3; &]
[s4; &]
[s5;:Pop3`:`:Logout`(`): [@(0.0.255) bool]_[* Logout]()&]
[s2;%% Logout of and disconnect from the POP3 server. Messages marked 
for deletion will be permanently deleted from the server after 
a successful logout. Returns true on success. Calling Logout 
on mailbox that is not online does nothing.&]
[s3; &]
[s4; &]
[s5;:Pop3`:`:IsOnline`(`)const: [@(0.0.255) bool]_[* IsOnline]()_[@(0.0.255) const]&]
[s2;%% Returns true if the POP3 mailbox is online. &]
[s3; &]
[s4; &]
[s5;:Pop3`:`:GetLastError`(`): [_^String^ String]_[* GetLastError]()&]
[s2;%% Returns the description of last error.&]
[s3; &]
[s4; &]
[s5;:Pop3`:`:Trace`(bool`): [@(0.0.255) static] [@(0.0.255) void]_[* Trace]([@(0.0.255) bool]_
[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% Activates logging of POP3.&]
[s3;%% &]
[s4; &]
[s5;:Pop3`:`:Pop3`(`): [* Pop3]()&]
[s2;%% Default constructor.&]
[s3; &]
[s4; &]
[s5;:Pop3`:`:`~Pop3`(`): [@(0.0.255) `~][* Pop3]()&]
[s2;%% Destructor calls Logout.&]
[s3; &]
[s0;%% ]]