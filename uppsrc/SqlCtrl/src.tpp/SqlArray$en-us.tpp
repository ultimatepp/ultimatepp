topic "class SqlArray : public ArrayCtrl";
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
[s0; ]