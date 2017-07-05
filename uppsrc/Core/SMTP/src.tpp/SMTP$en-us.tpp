topic "SMTP";
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
[ {{10000@(113.42.0) [s0; [*@7;4 Smtp]]}}&]
[s0;i448;a25;kKO9;@(0.0.255)%- &]
[s1;:Smtp`:`:class:%- [@(0.0.255)3 class][3 _][*3 Smtp][3 _:_][@(0.0.255)3 public][3 _][*@3;3 TcpSoc
ket]&]
[s2; Sends emails using SMTP protocol.&]
[s9; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:Smtp`:`:RequestTimeout`(int`):%- [_^Smtp^ Smtp][@(0.0.255) `&]_[* RequestTimeout]([@(0.0.255) i
nt]_[*@3 ms])&]
[s2; Total request timeout in milliseconds. Default is 120000 (2 
minutes).&]
[s3; &]
[s4; &]
[s5;:Smtp`:`:Host`(const String`&`):%- [_^Smtp^ Smtp][@(0.0.255) `&]_[* Host]([@(0.0.255) con
st]_[_^String^ String][@(0.0.255) `&]_[*@3 h])&]
[s2; SMTP host.&]
[s3; &]
[s4; &]
[s5;:Smtp`:`:Port`(int`):%- [_^Smtp^ Smtp][@(0.0.255) `&]_[* Port]([@(0.0.255) int]_[*@3 p])&]
[s2; SMTP host port. If Null, 25 is used for normal SMTP, 465 for 
SMTPS (using SSL), and 587 for SMTPS (using STARTTLS).&]
[s3; &]
[s4; &]
[s5;:Smtp`:`:SSL`(bool`):%- [_^Smtp^ Smtp][@(0.0.255) `&]_[* SSL]([@(0.0.255) bool]_[*@3 b]_`=_
[@(0.0.255) true])&]
[s2; Activates SMTPS mode (using SSL). &]
[s6; Requires Core/SSL package.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Smtp`:`:StartTLS`(bool`):%- [_^Upp`:`:Smtp^ Smtp][@(0.0.255) `&]_[* StartTLS]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Activates SMTPS mode (using STARTTLS). &]
[s6; Requires Core/SSL package.&]
[s3; &]
[s4;%- &]
[s5;:Smtp`:`:Auth`(const String`&`,const String`&`):%- [_^Smtp^ Smtp][@(0.0.255) `&]_[* Aut
h]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 user], [@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 pwd])&]
[s2; Sets username and password.&]
[s3; &]
[s4;%- &]
[s5;:Smtp`:`:From`(const String`&`,const String`&`,const String`&`):%- [_^Smtp^ Smtp][@(0.0.255) `&
]_[* From]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 email], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 name]_`=_Null, 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 sender]_`=_Null)&]
[s2; Sets the sender.&]
[s3; &]
[s4; &]
[s5;:Smtp`:`:To`(const String`&`,const String`&`,AS`):%- [_^Smtp^ Smtp][@(0.0.255) `&]_[* T
o]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 email], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 name], AS_[*@3 a]_`=_TO)&]
[s2; Sets recipient. Can be called multiple times to add any number 
of recipients. Type of recipient is defined as [%-*@3 a], can be 
one of Smtp`::TO, Smtp`::BC and Smtp`::BCC, correspening to normal 
email recipient types.&]
[s3; &]
[s4; &]
[s5;:Smtp`:`:To`(const String`&`,AS`):%- [_^Smtp^ Smtp][@(0.0.255) `&]_[* To]([@(0.0.255) con
st]_[_^String^ String][@(0.0.255) `&]_[*@3 email], AS_[*@3 a]_`=_TO)&]
[s2; Same as To([%-*@3 email], Null, [%-*@3 a]).&]
[s3; &]
[s4; &]
[s5;:Smtp`:`:Cc`(const String`&`,const String`&`):%- [_^Smtp^ Smtp][@(0.0.255) `&]_[* Cc]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 email], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 name]_`=_Null)&]
[s2; Same as To([%-*@3 email], [%-*@3 name], Smtp`::CC).&]
[s3; &]
[s4; &]
[s5;:Smtp`:`:Bcc`(const String`&`,const String`&`):%- [_^Smtp^ Smtp][@(0.0.255) `&]_[* Bcc](
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 email], [@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 name]_`=_Null)&]
[s2; Same as To([%-*@3 email], [%-*@3 name], Smtp`::BCC).&]
[s3; &]
[s4; &]
[s5;:Smtp`:`:ReplyTo`(const String`&`,const String`&`):%- [_^Smtp^ Smtp][@(0.0.255) `&]_[* R
eplyTo]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 email], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 name]_`=_Null)&]
[s2; Sets the reply address.&]
[s3; &]
[s4; &]
[s5;:Smtp`:`:TimeSent`(Time`):%- [_^Smtp^ Smtp][@(0.0.255) `&]_[* TimeSent]([_^Time^ Time]_[*@3 t
])&]
[s2; Sets the sent time attribute of email.&]
[s3; &]
[s4; &]
[s5;:Smtp`:`:Subject`(const String`&`):%- [_^Smtp^ Smtp][@(0.0.255) `&]_[* Subject]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 s])&]
[s2; Sets the subject of email.&]
[s3; &]
[s4; &]
[s5;:Smtp`:`:Body`(const String`&`,const String`&`):%- [_^Smtp^ Smtp][@(0.0.255) `&]_[* Bod
y]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s], [@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 mime`_]_`=_Null)&]
[s2; Adds email body with specified MIME time. Can be called multiple 
times to add multiple bodies. Null [%-*@3 mime`_] corresponds to 
`"text/plain`" with current default charset.&]
[s3; &]
[s4; &]
[s5;:Smtp`:`:AttachFile`(const char`*`,const char`*`):%- [_^Smtp^ Smtp][@(0.0.255) `&]_[* A
ttachFile]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 filename], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 mime]_`=_[@3 0])&]
[s2; Attaches a file to email.&]
[s3; &]
[s4; &]
[s5;:Smtp`:`:Attach`(const char`*`,const String`&`,const char`*`):%- [_^Smtp^ Smtp][@(0.0.255) `&
]_[* Attach]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 name], [@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 data], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 mime]_`=_[@3 0])
&]
[s2; Attaches a String as file attachment..&]
[s3; &]
[s4;%- &]
[s5;:Smtp`:`:AddHeader`(const String`&`):%- Smpt[@(0.0.255) `&]_[* AddHeader]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 text])&]
[s2; Adds additional [%-*@3 text] message header. Text should [* not] 
be terminated by CRLF.&]
[s3; &]
[s4;%- &]
[s5;:Smtp`:`:AddHeader`(const char`*`,const String`&`):%- [_^Smtp^ Smtp][@(0.0.255) `&]_[* A
ddHeader]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) const]_[_^String^ St
ring][@(0.0.255) `&]_[*@3 txt])&]
[s2; Adds additional field [%-*@3 id] with value [%-*@3 txt] to header.&]
[s3; &]
[s4; &]
[s5;:Smtp`:`:New`(`):%- [_^Smtp^ Smtp][@(0.0.255) `&]_[* New]()&]
[s2; Restarts Smtp for sending of next email (resets all addresses, 
bodies, attachments, subject).&]
[s3; &]
[s4;%- &]
[s5;:Smtp`:`:GetMessage`(`):%- [_^String^ String]_[* GetMessage]()&]
[s2; Returns the message in RFC`-822 format, without sending it.&]
[s3;%- &]
[s4;%- &]
[s5;:Smtp`:`:GetMessageID`(`):%- [_^String^ String]_[* GetMessageID]()&]
[s2; Returns the `"Message`-ID`" header of current message, without 
`"<`", `">`" characters. New Message ID is generated by constructor 
or New method. Domain part is taken from message sender; correct 
result can therefore obtained only after `'From`' method was 
called.&]
[s3;%- &]
[s4;%- &]
[s5;:Smtp`:`:Send`(const String`&`):%- [@(0.0.255) bool]_[* Send]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 message])&]
[s2; Send the raw message in RFC`-822 format. Note that sender and 
recipient of message has to be defined using From, To methods 
even as they are contained in message.&]
[s3; &]
[s4; &]
[s5;:Smtp`:`:Send`(`):%- [@(0.0.255) bool]_[* Send]()&]
[s2; Sends email. Returns true on success. Same as Send(GetMessage()), 
except that this form is able to send attachments in chunks.&]
[s3; &]
[s4; &]
[s5;:Smtp`:`:GetError`(`)const:%- [_^String^ String]_[* GetError]()_[@(0.0.255) const]&]
[s2; Returns description of last error.&]
[s3; &]
[s4; &]
[s5;:Smtp`:`:Smtp`(`):%- [* Smtp]()&]
[s2; Default constructor.&]
[s3; &]
[s4; &]
[s5;:Smtp`:`:Trace`(bool`):%- [@(0.0.255) static] [@(0.0.255) void]_[* Trace]([@(0.0.255) boo
l]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Activates logging of SMTP.&]
[s3; &]
[s0; ]]