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
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 SqlCtrls]]}}&]
[s9; &]
[s1;:Upp`:`:SqlCtrls`:`:class: [@(0.0.255)3 class][3 _][*3 SqlCtrls][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 IdCtrls]&]
[s2;%% This class extends [^topic`:`/`/CtrlLib`/src`/IdCtrls`$en`-us^ IdCtrls] 
class with SQL related operations.&]
[s0;i448;a25;kKO9;:noref:@(0.0.255) &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s4; &]
[s5;:Upp`:`:SqlCtrls`:`:operator`(`)`(Upp`:`:SqlId`,Upp`:`:Ctrl`&`): [_^Upp`:`:SqlCtrls^ S
qlCtrls][@(0.0.255) `&]_[* operator()]([_^Upp`:`:SqlId^ SqlId]_[*@3 id], 
[_^Upp`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 ctrl])&]
[s2;%% Calls IdCtrls`::Add([%-*@3 id], [%-*@3 ctrl]).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SqlCtrls`:`:Table`(Upp`:`:Ctrl`&`,Upp`:`:SqlId`): [@(0.0.255) void]_[* Table](
[_^Upp`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 dlg], [_^Upp`:`:SqlId^ SqlId]_[*@3 table])&]
[s2;%% Adds such widgets of [%-*@3 dlg] whose layout id (that is the 
same as the id of theirs member variable) matches any column 
name in [%-*@3 table], based on database schema file (.sch). Widgets 
are added with uppercased layout id. Label and Button widget 
are excluded (tested using dynamic`_cast)&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SqlCtrls`:`:operator`(`)`(Upp`:`:Ctrl`&`,Upp`:`:SqlId`): [_^Upp`:`:SqlCtrls^ S
qlCtrls][@(0.0.255) `&]_[* operator()]([_^Upp`:`:Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 dlg], 
[_^Upp`:`:SqlId^ SqlId]_[*@3 table])&]
[s2;%% Same as Table([%-*@3 dlg], [%-*@3 table]).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SqlCtrls`:`:Set`(`)const: [_^Upp`:`:SqlSet^ SqlSet]_[* Set]()_[@(0.0.255) const
]&]
[s2;%% Returns SqlSet of all ids of SqlCtrls.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SqlCtrls`:`:operator SqlSet`(`)const: [* operator_SqlSet]()_[@(0.0.255) const
]&]
[s2;%% Same as Set().&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SqlCtrls`:`:Read`(Upp`:`:Sql`&`): [@(0.0.255) void]_[* Read]([_^Upp`:`:Sql^ Sql
][@(0.0.255) `&]_[*@3 sql])&]
[s2;%% Assigns values of [%-*@3 sql] (which is supposed to contain 
some fetched line from SQL database) into widgets, based on match 
of widget ids and column names.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SqlCtrls`:`:Fetch`(Upp`:`:Sql`&`): [@(0.0.255) bool]_[* Fetch]([_^Upp`:`:Sql^ S
ql][@(0.0.255) `&]_[*@3 sql])&]
[s2;%% Does [%-*@3 sql].Fetch(). If that is false, returns false, otherwise 
performs Read(sql) and returns true. In other words, fetches 
the line from SQL database and assigns corresponding values to 
SqlCtrls widgets.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SqlCtrls`:`:Load`(Upp`:`:Sql`&`,Upp`:`:SqlSelect`): [@(0.0.255) bool]_[* Load
]([_^Upp`:`:Sql^ Sql][@(0.0.255) `&]_[*@3 sql], [_^Upp`:`:SqlSelect^ SqlSelect]_[*@3 select
])&]
[s2;%% Executes [%-*@3 select] statement on [%-*@3 sql] and then calls 
Fetch([%-*@3 sql]) and returns its return value.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SqlCtrls`:`:Load`(Upp`:`:Sql`&`,Upp`:`:SqlId`,Upp`:`:SqlBool`): [@(0.0.255) b
ool]_[* Load]([_^Upp`:`:Sql^ Sql][@(0.0.255) `&]_[*@3 sql], [_^Upp`:`:SqlId^ SqlId]_[*@3 tabl
e], [_^Upp`:`:SqlBool^ SqlBool]_[*@3 where])&]
[s2;%% Similar to Load, but [%-*@3 select] statement is constructed 
based on ids of widgets, given [%-*@3 table] and [%-*@3 where] expression.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SqlCtrls`:`:Fetch`(`): [@(0.0.255) bool]_[* Fetch]()&]
[s2;%% Same as Fetch(SQL).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SqlCtrls`:`:Load`(Upp`:`:SqlSelect`): [@(0.0.255) bool]_[* Load]([_^Upp`:`:SqlSelect^ S
qlSelect]_[*@3 select])&]
[s2;%% Same as Load(SQL, [%-*@3 select]).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SqlCtrls`:`:Load`(Upp`:`:SqlId`,Upp`:`:SqlBool`): [@(0.0.255) bool]_[* Load](
[_^Upp`:`:SqlId^ SqlId]_[*@3 table], [_^Upp`:`:SqlBool^ SqlBool]_[*@3 set])&]
[s2;%% Same as Load(SQL, [%-*@3 table], [%-*@3 set]).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SqlCtrls`:`:Insert`(Upp`:`:SqlInsert`&`)const: [@(0.0.255) void]_[* Insert]([_^Upp`:`:SqlInsert^ S
qlInsert][@(0.0.255) `&]_[*@3 insert])_[@(0.0.255) const]&]
[s2;%% Adds all id `- widget value pairs to [%-*@3 insert].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SqlCtrls`:`:Update`(Upp`:`:SqlUpdate`&`)const: [@(0.0.255) void]_[* Update]([_^Upp`:`:SqlUpdate^ S
qlUpdate][@(0.0.255) `&]_[*@3 update])_[@(0.0.255) const]&]
[s2;%% Adds all id `- widget value pairs to [%-*@3 update].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SqlCtrls`:`:UpdateModified`(Upp`:`:SqlUpdate`&`)const: [@(0.0.255) void]_[* U
pdateModified]([_^Upp`:`:SqlUpdate^ SqlUpdate][@(0.0.255) `&]_[*@3 update])_[@(0.0.255) c
onst]&]
[s2;%% Adds all id `- widget value pairs, where widgets have `'modified`' 
flag set, to [%-*@3 update] .&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SqlCtrls`:`:Insert`(Upp`:`:SqlId`)const: [_^Upp`:`:SqlInsert^ SqlInsert]_[* I
nsert]([_^Upp`:`:SqlId^ SqlId]_[*@3 table])_[@(0.0.255) const]&]
[s2;%% Constructs SqlInsert statement into [%-*@3 table] which contains 
all id `- widget value pairs as inserted values.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SqlCtrls`:`:Update`(Upp`:`:SqlId`)const: [_^Upp`:`:SqlUpdate^ SqlUpdate]_[* U
pdate]([_^Upp`:`:SqlId^ SqlId]_[*@3 table])_[@(0.0.255) const]&]
[s2;%% Constructs SqlUpdate statement of [%-*@3 table] which contains 
all id `- widget value pairs as updated values.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SqlCtrls`:`:UpdateModified`(Upp`:`:SqlId`)const: [_^Upp`:`:SqlUpdate^ SqlUp
date]_[* UpdateModified]([_^Upp`:`:SqlId^ SqlId]_[*@3 table])_[@(0.0.255) const]&]
[s2;%% Constructs SqlUpdate statement of [%-*@3 table] which contains 
all id `- widget value pairs, where widgets have `'modified`' 
flag set, as updated values.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SqlCtrls`:`:operator`(`)`(int`)const: [_^Upp`:`:SqlId^ SqlId]_[* operator()](
[@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s5;:Upp`:`:SqlCtrls`:`:GetKey`(int`)const: [_^Upp`:`:SqlId^ SqlId]_[* GetKey]([@(0.0.255) i
nt]_[*@3 i])_[@(0.0.255) const]&]
[s2;%% Returns id of entry [%-*@3 i].&]
[s0;%% ]]