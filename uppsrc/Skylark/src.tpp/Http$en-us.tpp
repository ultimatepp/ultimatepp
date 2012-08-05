topic "";
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
[ {{10000@(113.42.0) [s0; [*@7;4 Title]]}}&]
[s0;%- &]
[s1;:Http`:`:class:%- [@(0.0.255)3 class][3 _][*3 Http][3 _:_][@(0.0.255)3 public][3 _][*@3;3 Render
er]&]
[s2; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s4;%- &]
[s5;:Http`:`:operator`(`)`(const char`*`,const Value`&`):%- [_^Http^ Http][@(0.0.255) `&]_
[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) const]_[_^Value^ V
alue][@(0.0.255) `&]_[*@3 v])&]
[s2; Calls Renderer`::operator([%-*@3 id] , [%-*@3 v]) to set the value 
of variable from shared variable space. Return `*this.&]
[s3; &]
[s4; &]
[s5;:Http`:`:operator`(`)`(const ValueMap`&`):%- [@(0.0.255) virtual] 
[_^Http^ Http][@(0.0.255) `&]_[* operator()]([@(0.0.255) const]_[_^ValueMap^ ValueMap][@(0.0.255) `&
]_[*@3 map])&]
[s2; Calls Renderer`::operator([%-*@3 map]) to set the values of variables 
from shared variable space based on key`-value pairs of [%-*@3 map]. 
Return `*this.&]
[s3; &]
[s4; &]
[s5;:Http`:`:operator`(`)`(const char`*`,void`(`*`)`(Http`&`)`):%- [_^Http^ Http][@(0.0.255) `&
]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) void]_(`*[*@3 v
iew])([_^Http^ Http][@(0.0.255) `&]))&]
[s2; Calls Render [%-*@3 id] [%-*@3 view] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:operator`(`)`(const char`*`,void`(`*`)`(Http`&`)`,const Value`&`):%- [@(0.0.255) v
irtual] [_^Http^ Http][@(0.0.255) `&]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*
[*@3 id], [@(0.0.255) void]_(`*[*@3 view])([_^Http^ Http][@(0.0.255) `&]), 
[@(0.0.255) const]_Value[@(0.0.255) `&]_[*@3 arg1])&]
[s2;  [%-*@3 id] [%-*@3 view] [%-*@3 arg1] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:operator`(`)`(const char`*`,void`(`*`)`(Http`&`)`,const Value`&`,const Value`&`):%- [@(0.0.255) v
irtual] [_^Http^ Http][@(0.0.255) `&]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*
[*@3 id], [@(0.0.255) void]_(`*[*@3 view])([_^Http^ Http][@(0.0.255) `&]), 
[@(0.0.255) const]_Value[@(0.0.255) `&]_[*@3 arg1], [@(0.0.255) const]_Value[@(0.0.255) `&]_
[*@3 arg2])&]
[s2;  [%-*@3 id] [%-*@3 view] [%-*@3 arg1] [%-*@3 arg2] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:operator`(`)`(const Sql`&`):%- [_^Http^ Http][@(0.0.255) `&]_[* operator()]([@(0.0.255) c
onst]_[_^Sql^ Sql][@(0.0.255) `&]_[*@3 sql])&]
[s2;  [%-*@3 sql] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:operator`(`)`(Fields`):%- [_^Http^ Http][@(0.0.255) `&]_[* operator()]([_^Fields^ F
ields]_[*@3 rec])&]
[s2;  [%-*@3 rec] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:operator`(`)`(const SqlSelect`&`):%- [_^Http^ Http][@(0.0.255) `&]_[* operator
()]([@(0.0.255) const]_[_^SqlSelect^ SqlSelect][@(0.0.255) `&]_[*@3 row`_sel])&]
[s2;  [%-*@3 row`_sel] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:operator`(`)`(const char`*`,const SqlSelect`&`):%- [_^Http^ Http][@(0.0.255) `&
]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) const]_[_^SqlSelect^ S
qlSelect][@(0.0.255) `&]_[*@3 sel])&]
[s2;  [%-*@3 id] [%-*@3 sel] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:Render`(const char`*`,const String`&`):%- [@(0.0.255) virtual] 
[_^Http^ Http][@(0.0.255) `&]_[* Render]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 template`_name])&]
[s2;  [%-*@3 id] [%-*@3 template`_name] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:Render`(const String`&`):%- [@(0.0.255) virtual] [_^Value^ Value]_[* Render]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 template`_name])&]
[s2;  [%-*@3 template`_name] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:GetHeader`(const char`*`)const:%- [_^String^ String]_[* GetHeader]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 s])_[@(0.0.255) const]&]
[s2;  [%-*@3 s] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:GetLength`(`)const:%- [@(0.0.255) int]_[* GetLength]()_[@(0.0.255) const]&]
[s2; &]
[s3; &]
[s4; &]
[s5;:Http`:`:GetViewId`(`)const:%- [_^String^ String]_[* GetViewId]()_[@(0.0.255) const]&]
[s2; &]
[s3; &]
[s4; &]
[s5;:Http`:`:operator`[`]`(const char`*`)const:%- [@(0.0.255) virtual] 
[_^Value^ Value]_[* operator`[`]]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id])_[@(0.0.255) c
onst]&]
[s2;  [%-*@3 id] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:Int`(const char`*`)const:%- [@(0.0.255) int]_[* Int]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 id])_[@(0.0.255) const]&]
[s2;  [%-*@3 id] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:operator`[`]`(int`)const:%- [_^String^ String]_[* operator`[`]]([@(0.0.255) in
t]_[*@3 i])_[@(0.0.255) const]&]
[s2;  [%-*@3 i] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:Int`(int`)const:%- [@(0.0.255) int]_[* Int]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) c
onst]&]
[s2;  [%-*@3 i] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:GetParamCount`(`)const:%- [@(0.0.255) int]_[* GetParamCount]()_[@(0.0.255) con
st]&]
[s2; &]
[s3; &]
[s4; &]
[s5;:Http`:`:ContentType`(const char`*`):%- [_^Http^ Http][@(0.0.255) `&]_[* ContentType]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 s])&]
[s2;  [%-*@3 s] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:Content`(const char`*`,const Value`&`):%- [_^Http^ Http][@(0.0.255) `&]_[* Con
tent]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 s], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[*@3 data])&]
[s2;  [%-*@3 s] [%-*@3 data] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:operator`<`<`(const Value`&`):%- [_^Http^ Http][@(0.0.255) `&]_[* operator<<](
[@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 s])&]
[s2;  [%-*@3 s] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:SetRawCookie`(const char`*`,const String`&`,Time`,const char`*`,const char`*`,bool`,bool`):%- [_^Http^ H
ttp][@(0.0.255) `&]_[* SetRawCookie]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 value], [_^Time^ Time]_[*@3 expires
]_`=_Null, [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 path]_`=_NULL, 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 domain]_`=_NULL, [@(0.0.255) bool]_[*@3 secure]_
`=_[@(0.0.255) false], [@(0.0.255) bool]_[*@3 httponly]_`=_[@(0.0.255) false])&]
[s2;  [%-*@3 id] [%-*@3 value] [%-*@3 expires] [%-*@3 path] [%-*@3 domain] 
[%-*@3 secure] [%-*@3 httponly] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:SetCookie`(const char`*`,const String`&`,Time`,const char`*`,const char`*`,bool`,bool`):%- [_^Http^ H
ttp][@(0.0.255) `&]_[* SetCookie]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 value], [_^Time^ Time]_[*@3 expires
]_`=_Null, [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 path]_`=_NULL, 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 domain]_`=_NULL, [@(0.0.255) bool]_[*@3 secure]_
`=_[@(0.0.255) false], [@(0.0.255) bool]_[*@3 httponly]_`=_[@(0.0.255) false])&]
[s2;  [%-*@3 id] [%-*@3 value] [%-*@3 expires] [%-*@3 path] [%-*@3 domain] 
[%-*@3 secure] [%-*@3 httponly] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:ClearSession`(`):%- [_^Http^ Http][@(0.0.255) `&]_[* ClearSession]()&]
[s2; &]
[s3; &]
[s4; &]
[s5;:Http`:`:SessionSet`(const char`*`,const Value`&`):%- [_^Http^ Http][@(0.0.255) `&]_[* S
essionSet]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) const]_[_^Value^ Va
lue][@(0.0.255) `&]_[*@3 value])&]
[s2;  [%-*@3 id] [%-*@3 value] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:NewIdentity`(`):%- [_^Http^ Http][@(0.0.255) `&]_[* NewIdentity]()&]
[s2; &]
[s3; &]
[s4; &]
[s5;:Http`:`:NewSessionId`(`):%- [_^Http^ Http][@(0.0.255) `&]_[* NewSessionId]()&]
[s2; &]
[s3; &]
[s4; &]
[s5;:Http`:`:SetLanguage`(int`):%- [_^Http^ Http][@(0.0.255) `&]_[* SetLanguage]([@(0.0.255) i
nt]_[*@3 lang])&]
[s2;  [%-*@3 lang] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:Response`(int`,const String`&`):%- [_^Http^ Http][@(0.0.255) `&]_[* Response](
[@(0.0.255) int]_[*@3 code`_], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 ctex
t])&]
[s2;  [%-*@3 code`_] [%-*@3 ctext] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:RenderResult`(const char`*`):%- [_^Http^ Http][@(0.0.255) `&]_[* RenderResult](
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 template`_name])&]
[s2;  [%-*@3 template`_name] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:Redirect`(const char`*`,int`):%- [_^Http^ Http][@(0.0.255) `&]_[* Redirect]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 url], [@(0.0.255) int]_[*@3 code`_]_`=_[@3 302])&]
[s2;  [%-*@3 url] [%-*@3 code`_] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:Redirect`(void`(`*`)`(Http`&`)`,const Vector`<Value`>`&`):%- [_^Http^ Http
][@(0.0.255) `&]_[* Redirect]([@(0.0.255) void]_(`*[*@3 view])([_^Http^ Http][@(0.0.255) `&])
, [@(0.0.255) const]_Vector<Value>`&_[*@3 arg])&]
[s2;  [%-*@3 view] [%-*@3 arg] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:Redirect`(void`(`*`)`(Http`&`)`):%- [_^Http^ Http][@(0.0.255) `&]_[* Redirect](
[@(0.0.255) void]_(`*[*@3 view])([_^Http^ Http][@(0.0.255) `&]))&]
[s2;  [%-*@3 view] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:Redirect`(void`(`*`)`(Http`&`)`,const Value`&`):%- [_^Http^ Http][@(0.0.255) `&
]_[* Redirect]([@(0.0.255) void]_(`*[*@3 view])([_^Http^ Http][@(0.0.255) `&]), 
[@(0.0.255) const]_Value[@(0.0.255) `&]_[*@3 v1])&]
[s2;  [%-*@3 view] [%-*@3 v1] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:Redirect`(void`(`*`)`(Http`&`)`,const Value`&`,const Value`&`):%- [_^Http^ H
ttp][@(0.0.255) `&]_[* Redirect]([@(0.0.255) void]_(`*[*@3 view])([_^Http^ Http][@(0.0.255) `&
]), [@(0.0.255) const]_Value[@(0.0.255) `&]_[*@3 v1], [@(0.0.255) const]_Value[@(0.0.255) `&
]_[*@3 v2])&]
[s2;  [%-*@3 view] [%-*@3 v1] [%-*@3 v2] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:Ux`(const char`*`,const String`&`):%- [_^Http^ Http][@(0.0.255) `&]_[* Ux]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 t
ext])&]
[s2;  [%-*@3 id] [%-*@3 text] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:UxRender`(const char`*`,const char`*`):%- [_^Http^ Http][@(0.0.255) `&]_[* UxR
ender]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) const]_[@(0.0.255) char
]_`*[*@3 template`_name])&]
[s2;  [%-*@3 id] [%-*@3 template`_name] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:UxSet`(const char`*`,const String`&`):%- [_^Http^ Http][@(0.0.255) `&]_[* UxSe
t]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 value])&]
[s2;  [%-*@3 id] [%-*@3 value] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:UxRun`(const String`&`):%- [_^Http^ Http][@(0.0.255) `&]_[* UxRun]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 js`_code])&]
[s2;  [%-*@3 js`_code] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:GetResponse`(`)const:%- [_^String^ String]_[* GetResponse]()_[@(0.0.255) const
]&]
[s2; &]
[s3; &]
[s4; &]
[s5;:Http`:`:Dispatch`(TcpSocket`&`):%- [@(0.0.255) void]_[* Dispatch]([_^TcpSocket^ TcpSoc
ket][@(0.0.255) `&]_[*@3 socket])&]
[s2;  [%-*@3 socket] .&]
[s3; &]
[s4; &]
[s5;:Http`:`:App`(`)const:%- [@(0.0.255) const]_[_^SkylarkApp^ SkylarkApp][@(0.0.255) `&]_[* A
pp]()_[@(0.0.255) const]&]
[s2; &]
[s3; &]
[s4; &]
[s5;:Http`:`:Http`(SkylarkApp`&`):%- [* Http]([_^SkylarkApp^ SkylarkApp][@(0.0.255) `&]_[*@3 a
pp])&]
[s2;  [%-*@3 app] .&]
[s3; &]
[s0; ]]