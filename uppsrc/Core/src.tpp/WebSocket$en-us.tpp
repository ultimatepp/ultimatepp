topic "WebSocket";
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
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 WebSocket]]}}&]
[s3;%- &]
[s1;:WebSocket`:`:class:%- [@(0.0.255)3 class][3 _][*3 WebSocket][3 _:_][@(0.0.255)3 public][3 _][*@3;3 T
cpSocket]&]
[s2; Provides the WebSocket protocol support.&]
[s0;i448;a25;kKO9;:noref:@(0.0.255)%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:WebSocket`:`:WebAccept`(TcpSocket`&`):%- [@(0.0.255) bool]_[* WebAccept]([_^TcpSocket^ T
cpSocket][@(0.0.255) `&]_[*@3 server])&]
[s2; Accepts a websocekt connectiont from listening socket [%-*@3 server] 
and performs a handshake. Returns true on success.&]
[s3; &]
[s4; &]
[s5;:WebSocket`:`:RecieveRaw`(`):%- [@(0.0.255) bool]_[* RecieveRaw]()&]
[s2; Recieves the message. This variant does not automatically handle 
PING and CLOSE messages. Returns true on success.&]
[s3; &]
[s4; &]
[s5;:WebSocket`:`:Recieve`(`):%- [_^String^ String]_[* Recieve]()&]
[s2; Recieves the message. PING and CLOSE messages are handled and 
are not returned; PING message is replied with PONG and not reported 
to client `- Recieve does not return and waits for the next message, 
CLOSE is replied by CLOSE and Recieve returns. Returns the content 
of message recieved on String`::GetVoid().&]
[s3; &]
[s4; &]
[s5;:WebSocket`:`:IsFin`(`):%- [@(0.0.255) bool]_[* IsFin]()&]
[s2; Returns true if the last message was marked as final.&]
[s3; &]
[s4; &]
[s5;:WebSocket`:`:GetOpCode`(`)const:%- [@(0.0.255) int]_[* GetOpCode]()_[@(0.0.255) const]&]
[s2; Returns the websocket opcode of the last recieved message.&]
[s3; &]
[s4; &]
[s5;:WebSocket`:`:IsText`(`)const:%- [@(0.0.255) bool]_[* IsText]()_[@(0.0.255) const]&]
[s2; Returns true if the last message recieved was text.&]
[s3; &]
[s4; &]
[s5;:WebSocket`:`:IsBinary`(`)const:%- [@(0.0.255) bool]_[* IsBinary]()_[@(0.0.255) const]&]
[s2; Returns true if the last message recieved was binary.&]
[s3; &]
[s4; &]
[s5;:WebSocket`:`:IsClosed`(`)const:%- [@(0.0.255) bool]_[* IsClosed]()_[@(0.0.255) const]&]
[s2; Returns true if CLOSE message was recieved.&]
[s3; &]
[s4; &]
[s5;:WebSocket`:`:GetData`(`)const:%- [_^String^ String]_[* GetData]()_[@(0.0.255) const]&]
[s2; Returns the content of the last message.&]
[s3; &]
[s4; &]
[s5;:WebSocket`:`:SendRaw`(int`,const void`*`,int64`):%- [@(0.0.255) bool]_[* SendRaw]([@(0.0.255) i
nt]_[*@3 hdr], [@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], [_^int64^ int64]_[*@3 len])
&]
[s2; Sends the message, [%-*@3 hdr] is used as the first byte of websocket 
protocol. Returns true on success.&]
[s3; &]
[s4; &]
[s5;:WebSocket`:`:SendText`(const void`*`,int64`,bool`):%- [@(0.0.255) bool]_[* SendText](
[@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], [_^int64^ int64]_[*@3 len], 
[@(0.0.255) bool]_[*@3 fin]_`=_[@(0.0.255) true])&]
[s5;:WebSocket`:`:SendText`(const String`&`,bool`):%- [@(0.0.255) bool]_[* SendText]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 data], [@(0.0.255) bool]_[*@3 fin]_`=_[@(0.0.255) t
rue])&]
[s2; Sends the text message. Returns true on success.&]
[s3; &]
[s4; &]
[s5;:WebSocket`:`:SendBinary`(const void`*`,int64`,bool`):%- [@(0.0.255) bool]_[* SendBin
ary]([@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 data], [_^int64^ int64]_[*@3 len], 
[@(0.0.255) bool]_[*@3 fin]_`=_[@(0.0.255) true])&]
[s5;:WebSocket`:`:SendBinary`(const String`&`,bool`):%- [@(0.0.255) bool]_[* SendBinary](
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 data], [@(0.0.255) bool]_[*@3 fin]_
`=_[@(0.0.255) true])&]
[s2; Sends the binary message. Returns true on success.&]
[s3; &]
[s4; &]
[s5;:WebSocket`:`:Close`(`):%- [@(0.0.255) void]_[* Close]()&]
[s2; Closes the websocket.&]
[s3; &]
[s4; &]
[s5;:WebSocket`:`:MaxLen`(int64`):%- [_^WebSocket^ WebSocket][@(0.0.255) `&]_[* MaxLen]([_^int64^ i
nt64]_[*@3 maxlen`_])&]
[s2; Sets the maximum length of recieved message to [%-*@3 maxlen] 
. Default is 10M.&]
[s3; &]
[s0; ]]