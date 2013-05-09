topic "SqlArray";
[ $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9; $$1,0#37138531426314131252341829483380:structitem]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:codeitem]
[{_} 
[s1;:SqlArray`:`:class: [@(0.0.255) class]_[* SqlArray]_:_[@(0.0.255) public]_[*@3 ArrayCtrl]&]
[s2;%% SqlArray extends ArrayCtrl to support browsing and editing 
of SQL tables.&]
[s3; &]
[s4; &]
[s5;:SqlArray`:`:WhenPreQuery: [_^Callback^ Callback]_[* WhenPreQuery]&]
[s2;%% Invoked [/ before] SqlArray issues SQL query to fill its content.&]
[s3; &]
[s4; &]
[s5;:SqlArray`:`:WhenPostQuery: [_^Callback^ Callback]_[* WhenPostQuery]&]
[s2;%% Invoked [/ after][*/  ]SQL query to fills SqlArray.&]
[s3; &]
[s4; &]
[s5;:SqlArray`:`:WhenFilter: [_^Gate1^ Gate1]<[@(0.0.255) const]_VectorMap<[_^Id^ Id], 
[_^Value^ Value]>`&>_[* WhenFilter]&]
[s2;%% This callback adds additional filtering of records fetched 
from the database. Only records for whose it returns true are 
add to the list. The VectorMap parameter contains database column 
`- record value pairs.&]
[s3; &]
[s4; &]
[s5;:SqlArray`:`:StdBar`(Bar`&`): [@(0.0.255) void]_[* StdBar]([_^Bar^ Bar][@(0.0.255) `&]_[*@3 m
enu])&]
[s2;%% Implements standard menu`-bar of SqlArray. This menu contains 
items for inserting, duplicating, editing and removing database 
table rows. Various items are enabled/disabled according to SqlArray 
setup.&]
[s3;%% &]
[s4; &]
[s5;:SqlArray`:`:CanInsert`(`)const: [@(0.0.255) bool]_[* CanInsert]()_[@(0.0.255) const]&]
[s2;%% Returns true if it is possible to insert row into database. 
In reality, this can be false for detail SqlArray in master`-detail 
(see Join) relation when master SqlArray has no row selected 
(and thus there is no foreign key to bind the record to).&]
[s3; &]
[s4; &]
[s5;:SqlArray`:`:StartInsert`(`): [@(0.0.255) void]_[* StartInsert]()&]
[s2;%% Starts editing of newly inserted row. If there is non`-Null 
value in the key (first index of ArrayCtrl) after the line is 
inserted into the ArrayCtrl or if AutoInsertId is active the 
row is inserted into table (using SQL INSERT) immediately even 
before editing. Accepting or canceling edit then updates or removes 
the row. Note that non`-Null value for such row can be set by 
InsertValue. AutoInsertId is intended for SQL engines that do 
support auto`-increment columns `- in that case, empty row is 
inserted into the table and its auto`-increment value is read 
to SqlArray.&]
[s2;%% If above condition is not satisfied, the row is not inserted 
into database at this moment. In that case, it is inserted only 
after the row is accepted (typically after closing the editation 
mode by pressing Enter key). While this seems more logical, however 
the major disadvantage is that if such table acts as master in 
master`-detail relation, the detail row cannot be inserted (as 
foreign keys is not yet known) until the line edit is finished, 
which is contra`-intuitive. Note that this `'late insert`' mode 
is usually reserved for tables where key is logical and edited 
by user.&]
[s0;%% &]
[s3; &]
[s4; &]
[s5;:SqlArray`:`:StartDuplicate`(`): [@(0.0.255) void]_[* StartDuplicate]()&]
[s2;%% Similar to StartInsert, but copies the content of current 
line to the newly inserted one.&]
[s3; &]
[s4; &]
[s5;:SqlArray`:`:DoRemove`(`): [@(0.0.255) void]_[* DoRemove]()&]
[s2;%% Performs removal of line from SqlArray AND database. If AskRemove 
is active (default), user is prompted (using PromptYesNo) before 
removing.&]
[s3; &]
[s4; &]
[s5;:SqlArray`:`:SetSession`(SqlSession`&`): [@(0.0.255) void]_[* SetSession]([_^SqlSession^ S
qlSession][@(0.0.255) `&]_[*@3 `_session])&]
[s2;%% Sets the sql session used for. (Default is SQL).&]
[s3;%% &]
[s4; &]
[s5;:SqlArray`:`:Join`(SqlId`,ArrayCtrl`&`): [@(0.0.255) void]_[* Join]([_^SqlId^ SqlId]_[*@3 f
k], [_^ArrayCtrl^ ArrayCtrl][@(0.0.255) `&]_[*@3 master])&]
[s2;%% Joins this SqlArray with [%-*@3 master] using foreign key [%-*@3 fk]. 
SqlArray will represent a detail; when cursor in master changes, 
SqlArray reloads with a query (issuing select to database) formed 
by (optional) SetWhere SqlBool expression and fk `=`= master.GetKey().&]
[s3;%% &]
[s4; &]
[s5;:SqlArray`:`:Join`(ArrayCtrl`&`): [@(0.0.255) void]_[* Join]([_^ArrayCtrl^ ArrayCtrl][@(0.0.255) `&
]_[*@3 master])&]
[s2;%% Joins [%-*@3 master].using the name of key (first index) as 
foreign key name.&]
[s3;%% &]
[s4; &]
[s5;:SqlArray`:`:Query`(`): [@(0.0.255) void]_[* Query]()&]
[s2;%% Reloads the content (issuing select).&]
[s3; &]
[s4; &]
[s5;:SqlArray`:`:AppendQuery`(SqlBool`): [@(0.0.255) void]_[* AppendQuery]([_^SqlBool^ SqlB
ool]_[*@3 where])&]
[s2;%% Appends a query with [%-*@3 where], adding rows to current content. 
Foreign keys and master defined by Join are combined with where. 
This is rather untypical operation.&]
[s3;%% &]
[s4; &]
[s5;:SqlArray`:`:Query`(SqlBool`): [@(0.0.255) void]_[* Query]([_^SqlBool^ SqlBool]_[*@3 wher
e])&]
[s2;%% Performs SetWhere([%-*@3 where]); Query().&]
[s3;%% &]
[s4; &]
[s5;:SqlArray`:`:ReQuery`(`): [@(0.0.255) void]_[* ReQuery]()&]
[s2;%% Similar to Query, but restores the position of cursor to the 
row with same key as was active before this operations if possible. 
Tries to keep relative scroll position as well.&]
[s3; &]
[s4; &]
[s5;:SqlArray`:`:Limit`(int`,int`): [@(0.0.255) void]_[* Limit]([@(0.0.255) int]_[*@3 `_offse
t], [@(0.0.255) int]_[*@3 `_count])&]
[s2;%% Allows to fetch only part of query results, on database engines 
which support it.&]
[s3;%% &]
[s4; &]
[s5;:SqlArray`:`:Limit`(int`): [@(0.0.255) void]_[* Limit]([@(0.0.255) int]_[*@3 count])&]
[s2;%% Same as Limit(0, [%-*@3 count]).&]
[s3;%% &]
[s4; &]
[s5;:SqlArray`:`:SetTable`(SqlId`): [_^SqlArray^ SqlArray][@(0.0.255) `&]_[* SetTable]([_^SqlId^ S
qlId]_[*@3 `_table])&]
[s2;%% Sets the source table for this SqlArray.&]
[s3;%% &]
[s4; &]
[s5;:SqlArray`:`:SetTable`(SqlId`,SqlId`): [_^SqlArray^ SqlArray][@(0.0.255) `&]_[* SetTabl
e]([_^SqlId^ SqlId]_[*@3 `_table], [_^SqlId^ SqlId]_[*@3 key])&]
[s2;%% Sets both the table and the primary key. Same as SetTable([%-*@3 `_table]); 
AddKey([%-*@3 key)].&]
[s3;%% &]
[s4; &]
[s5;:SqlArray`:`:SetWhere`(SqlBool`): [_^SqlArray^ SqlArray][@(0.0.255) `&]_[* SetWhere]([_^SqlBool^ S
qlBool]_[*@3 where])&]
[s2;%% Sets the [%-*@3 where] expression. This expression is combined 
with foreign key (defined by Join) expression when performing 
Query.&]
[s3;%% &]
[s4; &]
[s5;:SqlArray`:`:SetOrderBy`(SqlSet`): [_^SqlArray^ SqlArray][@(0.0.255) `&]_[* SetOrderBy](
[_^SqlSet^ SqlSet]_[*@3 `_orderby])&]
[s5;:SqlArray`:`:SetOrderBy`(const SqlVal`&`): [_^SqlArray^ SqlArray][@(0.0.255) `&]_[* Set
OrderBy]([@(0.0.255) const]_[_^SqlVal^ SqlVal][@(0.0.255) `&]_[*@3 a])&]
[s5;:SqlArray`:`:SetOrderBy`(const SqlVal`&`,const SqlVal`&`): [_^SqlArray^ SqlArray][@(0.0.255) `&
]_[* SetOrderBy]([@(0.0.255) const]_[_^SqlVal^ SqlVal][@(0.0.255) `&]_[*@3 a], 
[@(0.0.255) const]_[_^SqlVal^ SqlVal][@(0.0.255) `&]_[*@3 b])&]
[s5;:SqlArray`:`:SetOrderBy`(const SqlVal`&`,const SqlVal`&`,const SqlVal`&`): [_^SqlArray^ S
qlArray][@(0.0.255) `&]_[* SetOrderBy]([@(0.0.255) const]_[_^SqlVal^ SqlVal][@(0.0.255) `&]_
[*@3 a], [@(0.0.255) const]_[_^SqlVal^ SqlVal][@(0.0.255) `&]_[*@3 b], 
[@(0.0.255) const]_[_^SqlVal^ SqlVal][@(0.0.255) `&]_[*@3 c])&]
[s2;%% Sets the sorting order `- affects `"ORDER BY`" part of query.&]
[s3;%% &]
[s4; &]
[s5;:SqlArray`:`:GoEndPostQuery`(bool`): [_^SqlArray^ SqlArray][@(0.0.255) `&]_[* GoEndPost
Query]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% If active, cursor is placed to the last row of SqlArray after 
the query instead of first one.&]
[s3;%% &]
[s4; &]
[s5;:SqlArray`:`:AutoInsertId`(bool`): [_^SqlArray^ SqlArray][@(0.0.255) `&]_[* AutoInsertI
d]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% Lets the database engine to create the value for auto`-increment 
keys.&]
[s3;%% &]
[s4; &]
[s5;:SqlArray`:`:AppendingAuto`(`): [_^SqlArray^ SqlArray][@(0.0.255) `&]_[* AppendingAuto](
)&]
[s2;%% Combines Appending and AutoInsertId.&]
[s3; &]
[s4; &]
[s5;:SqlArray`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%% Clears the content.&]
[s3; &]
[s4; &]
[s5;:SqlArray`:`:Reset`(`): [@(0.0.255) void]_[* Reset]()&]
[s2;%% Resets SqlArray to initial state `- all settings, indexes 
and columns are removed.&]
[s0; ]]