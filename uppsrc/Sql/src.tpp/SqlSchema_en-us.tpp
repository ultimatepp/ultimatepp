topic "Schema introspection";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 Schema introspection]]}}&]
[s3; &]
[s5;:FindSchJoin`(const String`&`): [_^SqlBool^ SqlBool]_[* FindSchJoin]([@(0.0.255) const]_
[_^String^ String][@(0.0.255) `&]_[*@3 tables])&]
[s2;%% Finds a join condition based in REFERENCE attributes of .sch 
files for a list of tables [%-*@3 tables] separated by comma. Function 
runs two phases of matching foreign keys, in the first phase 
it performs a match based on the name too `- the same named columns 
or foreign key is referenced`_table `+ `'`_`' `+ referenced`_primary`_key 
(e.g. PERSON`_ID matches ID of table PERSON) are match. If no 
match is found in the first phase, in second phase, any correct 
foreign key matches. If there are more matching columns, first 
column in .sch definition is used.&]
[s3;%% &]
[s4; &]
[s5;:GetSchTables`(`): [_^Vector^ Vector]<[_^String^ String]>_[* GetSchTables]()&]
[s2;%% Returns a list of all tables defined by .sch schema files.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:GetSchAll`(`): [_^Upp`:`:ArrayMap^ ArrayMap]<_[_^Upp`:`:String^ String], 
[_^Upp`:`:Array^ Array]<[_^Upp`:`:SchColumnInfo^ SchColumnInfo]>_>_[* GetSchAll]()&]
[s2;%% Returns the information of all .sch defined tables. Table 
names are map keys. SchColumnInfo contains name of column, the 
table it REFERENCES (if any) and the name of foreign key.&]
[s3; &]
[s4; &]
[s5;:GetSchColumns`(const String`&`): [_^Vector^ Vector]<[_^String^ String]>_[* GetSchColum
ns]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 table])&]
[s2;%% Returns all columns for [%-*@3 table] defined by .sch schema 
files.&]
[s0;%% ]]