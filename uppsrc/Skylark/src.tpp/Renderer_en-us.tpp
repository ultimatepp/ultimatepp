topic "Renderer";
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
[b42;a42;2 $$10,10#45413000475342174754091244180557:text]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 Renderer]]}}&]
[s0; &]
[s1;:Renderer`:`:class: [@(0.0.255)3 class][3 _][*3 Renderer]&]
[s2;%% This class represents [/ shared variable space]. It is separated 
from Http class to be available in cases when association with 
current HTTP request is not desirable (e.g. subtemplate rendering). 
Renderer is used as base class of Http.&]
[s2;%% (Note: the term [/ shared variable space] reflects the fact, 
that for single HTTP request all related values are stored into 
this single space: values set by handler, <form> or Ajax variables, 
session variables and cookies).&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Renderer`:`:operator`(`)`(const char`*`,const Value`&`): [_^Renderer^ Renderer][@(0.0.255) `&
]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) const]_[_^Value^ V
alue][@(0.0.255) `&]_[*@3 v])&]
[s5;:Renderer`:`:operator`(`)`(const char`*`,const char`*`): [_^Renderer^ Renderer][@(0.0.255) `&
]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 v])&]
[s5;:Renderer`:`:operator`(`)`(const char`*`,const String`&`): [_^Renderer^ Renderer][@(0.0.255) `&
]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 v])&]
[s2;%% Adds variable id [%-*@3 id] with value [%-*@3 v] to shared variable 
space. Returns `*this.&]
[s3;%% &]
[s4; &]
[s5;:Renderer`:`:operator`(`)`(const ValueMap`&`): [_^Renderer^ Renderer][@(0.0.255) `&]_
[* operator()]([@(0.0.255) const]_[_^ValueMap^ ValueMap][@(0.0.255) `&]_[*@3 map])&]
[s2;%% Adds all key/value pairs of [%-*@3 map] as variables id / value 
to shared variable space. Returns `*this.&]
[s3;%% &]
[s4; &]
[s5;:Renderer`:`:operator`(`)`(const char`*`,const HandlerId`&`): [_^Renderer^ Renderer
][@(0.0.255) `&]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], 
[@(0.0.255) const]_[_^HandlerId^ HandlerId][@(0.0.255) `&]_[*@3 handler])&]
[s5;:Renderer`:`:operator`(`)`(const char`*`,void`(`*`)`(Http`&`)`): [_^Renderer^ Rende
rer][@(0.0.255) `&]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], 
[@(0.0.255) void]_(`*[*@3 handler])(Http[@(0.0.255) `&]))&]
[s2;%% Adds variable [%-*@3 id] to shared variable space; the value 
of variable will be actual URL to [%-*@3 handler] (with no URL 
parameters). Returns `*this.&]
[s3;%% &]
[s4; &]
[s5;:Renderer`:`:operator`(`)`(const char`*`,const HandlerId`&`,const Value`&`): [_^Renderer^ R
enderer][@(0.0.255) `&]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], 
[@(0.0.255) const]_[_^HandlerId^ HandlerId][@(0.0.255) `&]_[*@3 handler], 
[@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 arg1])&]
[s5;:Renderer`:`:operator`(`)`(const char`*`,void`(`*`)`(Http`&`)`,const Value`&`): [_^Renderer^ R
enderer][@(0.0.255) `&]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], 
[@(0.0.255) void]_(`*[*@3 handler])(Http[@(0.0.255) `&]), [@(0.0.255) const]_Value[@(0.0.255) `&
]_[*@3 arg1])&]
[s2;%% Adds variable [%-*@3 id] to shared variable space; the value 
of variable will be actual URL to [%-*@3 handler], placing [%-*@3 arg1] 
to appropriate place (defined by handler URL pattern) as URL 
parameter. Returns `*this.&]
[s3;%% &]
[s4; &]
[s5;:Renderer`:`:operator`(`)`(const char`*`,const HandlerId`&`,const Value`&`,const Value`&`): [_^Renderer^ R
enderer][@(0.0.255) `&]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], 
[@(0.0.255) const]_[_^HandlerId^ HandlerId][@(0.0.255) `&]_[*@3 handler], 
[@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 arg1], [@(0.0.255) const]_[_^Value^ V
alue][@(0.0.255) `&]_[*@3 arg2])&]
[s5;:Renderer`:`:operator`(`)`(const char`*`,void`(`*`)`(Http`&`)`,const Value`&`,const Value`&`): [_^Renderer^ R
enderer][@(0.0.255) `&]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], 
[@(0.0.255) void]_(`*[*@3 handler])(Http[@(0.0.255) `&]), [@(0.0.255) const]_Value[@(0.0.255) `&
]_[*@3 arg1], [@(0.0.255) const]_Value[@(0.0.255) `&]_[*@3 arg2])&]
[s2;%% Adds variable [%-*@3 id] to shared variable space; the value 
of variable will be actual URL to [%-*@3 handler], placing [%-*@3 arg1] 
and [%-*@3 arg2] to appropriate place (defined by handler URL pattern) 
as URL parameter. Returns `*this.&]
[s3;%% &]
[s4; &]
[s5;:Renderer`:`:operator`(`)`(const Sql`&`): [_^Renderer^ Renderer][@(0.0.255) `&]_[* oper
ator()]([@(0.0.255) const]_[_^Sql^ Sql][@(0.0.255) `&]_[*@3 sql])&]
[s2;%% Method expects that Fetch was performed on [%-*@3 sql] and adds 
columns of the fetched row to shared variable space, Each column 
will be stored as a single variable with id equal to uppercased 
name of the column. Returns `*this.&]
[s3;%% &]
[s4; &]
[s5;:Renderer`:`:operator`(`)`(Fields`): [_^Renderer^ Renderer][@(0.0.255) `&]_[* operator(
)]([_^Fields^ Fields]_[*@3 rec])&]
[s2;%% Method stores fields of [%-*@3 rec] as shared variable values, 
each variable with id defined by field name and value by field 
value. Note that Fields is usually created as mapping object 
of S`_`* database records. Returns `*this.&]
[s3;%% &]
[s4; &]
[s5;:Renderer`:`:operator`(`)`(const SqlSelect`&`): [_^Renderer^ Renderer][@(0.0.255) `&]_
[* operator()]([@(0.0.255) const]_[_^SqlSelect^ SqlSelect][@(0.0.255) `&]_[*@3 row`_sel])&]
[s2;%% Methods executes [%-*@3 row`_sel] with SQLR (read session), 
fetches single row and adds columns of the fetched row to shared 
variable space, Each column will be stored as a single variable 
with id equal to uppercased name of the column. If no column 
is fetched, BadRequestExc exception is raised, which eventually 
(if not handled sooner) results in 400/Bad request response from 
the web server. Returns `*this.&]
[s3;%% &]
[s4; &]
[s5;:Renderer`:`:operator`(`)`(const char`*`,const SqlSelect`&`): [_^Renderer^ Renderer
][@(0.0.255) `&]_[* operator()]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], 
[@(0.0.255) const]_[_^SqlSelect^ SqlSelect][@(0.0.255) `&]_[*@3 sel])&]
[s2;%% Methods executes [%-*@3 sel] with SQLR (read session), fetches 
[*/ all] rows and creates an ValueArray of ValueMaps (with column`-name/column`-val
ue pairs) and stores the result into [%-*@3 id] shared variable. 
Returns `*this.&]
[s3;%% &]
[s4; &]
[s5;:Renderer`:`:SetLanguage`(int`): [_^Renderer^ Renderer][@(0.0.255) `&]_[* SetLanguage](
[@(0.0.255) int]_[*@3 lang])&]
[s2;%% Sets the language to be used while rendering witz template. 
Basically, it affects the choice of the template file, language 
specific templates have priority.E.g. if language is set to [* LNG`_(`'C`',`'S`',`'
C`',`'Z`')] and there exists file [%-* index.cs`-cz.witz], it has 
priority over [%-* index.witz][%- . The default value is LNG`_ENGLISH 
(en`-us).] Returns `*this.&]
[s3;%% &]
[s4; &]
[s5;:Renderer`:`:Update`(SqlId`): [_^SqlUpdate^ SqlUpdate]_[* Update]([_^SqlId^ SqlId]_[*@3 t
able])&]
[s2;%% This method creates an update SQL statement for [%-*@3 table] 
based on current shared variables. It obtains a list of columns 
for the [%-*@3 table] and adds to SqlUpdate all columns found in 
as shared variables; for each column it first tries to find it 
case`-sensitive, then lower`-cased, then upper`-cased. Columns 
not found as shared variable are not added.&]
[s3;%% &]
[s4; &]
[s5;:Renderer`:`:Insert`(SqlId`): [_^SqlInsert^ SqlInsert]_[* Insert]([_^SqlId^ SqlId]_[*@3 t
able])&]
[s2;%% This method creates an insert SQL statement for [%-*@3 table] 
based on current shared variables. It obtains a list of columns 
for the [%-*@3 table] and adds to SqlUpdate all columns found in 
as shared variables; for each column it first tries to find it 
case`-sensitive, then lower`-cased, then upper`-cased. Columns 
not found as shared variable are not added.&]
[s3;%% &]
[s4; &]
[s5;:Renderer`:`:operator`[`]`(const char`*`)const: [_^Value^ Value]_[* operator`[`]]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 id])_[@(0.0.255) const]&]
[s2;%% Returns the value of shared variable [%-*@3 id].&]
[s3;%% &]
[s4; &]
[s5;:Renderer`:`:RenderString`(const String`&`): [_^String^ String]_[* RenderString]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 template`_name])&]
[s2;%% Renders [%-*@3 template`_name] using current set of variables.&]
[s3;%% &]
[s4; &]
[s5;:Renderer`:`:Render`(const String`&`): [_^Value^ Value]_[* Render]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 template`_name])&]
[s2;%% Same as Raw(RenderString(template`_name)). The purpose is 
that Strings represented by RawHtmlText (a result of Raw) are 
not escaped when insterted into witz templates.&]
[s3;%% &]
[s4; &]
[s5;:Renderer`:`:Render`(const char`*`,const String`&`): [_^Renderer^ Renderer][@(0.0.255) `&
]_[* Render]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 template`_name])&]
[s2;%% Same as operator()([%-*@3 id], Render([%-*@3 template`_name])), 
in other words sets the variable to the result of template rendering. 
Returns `*this.&]
[s4; &]
[s5;:Renderer`:`:Variables`(`)const: [@(0.0.255) const]_[_^VectorMap^ VectorMap]<[_^String^ S
tring], [_^Value^ Value]>`&_[* Variables]()_[@(0.0.255) const]&]
[s2;%% Returns a reference to all shared variables.&]
[s3; ]]