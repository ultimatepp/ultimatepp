topic "Sql";
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
[ {{10000@(113.42.0) [s0; [*@7;4 Sql]]}}&]
[s1;@(0.0.255)3%- &]
[s1;:Sql`:`:class:%- [@(0.0.255)3 class][3 _][*3 Sql]&]
[s2; This class  represents a SQL execution context. It is associated 
with SqlSession; several Sql contexts can be associated with 
single session. As there is usually single default SqlSession 
per process (or per thread), when session is not specified with 
constructor, this default session is used.&]
[s3;%- &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:Sql`:`:Compile`(const SqlStatement`&`):%- [_^String^ String]_[* Compile]([@(0.0.255) c
onst]_[_^SqlStatement^ SqlStatement][@(0.0.255) `&]_[*@3 s])&]
[s2; Compiles SqlExp based statement [%-*@3 s] for SQL dialect associated 
with the session of this Sql.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:Clear`(`):%- [@(0.0.255) void]_[* Clear]()&]
[s2; Clears the current SQL statement.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:SetParam`(int`,const Value`&`):%- [@(0.0.255) void]_[* SetParam]([@(0.0.255) in
t]_[*@3 i], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 val])&]
[s2; Sets the positional parameter [%-*@3 i] to [%-*@3 val]. Positional 
parameters are expressed as `'?`' in the statement.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:SetStatement`(const String`&`):%- [@(0.0.255) void]_[* SetStatement]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 s])&]
[s2; Assigns SQL statement [%-*@3 s].&]
[s3; &]
[s4; &]
[s5;:Sql`:`:SetStatement`(const SqlStatement`&`):%- [@(0.0.255) void]_[* SetStatement]([@(0.0.255) c
onst]_[_^SqlStatement^ SqlStatement][@(0.0.255) `&]_[*@3 s])&]
[s2; Assingle SqlExp statement.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:Execute`(`):%- [@(0.0.255) bool]_[* Execute]()&]
[s2; Executes Sql statement assigned by SetStatement. Note that with 
some databases, prepared statements are used and if statement 
has not changed, Execute runs previously prepared statement.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:Run`(`):%- [@(0.0.255) bool]_[* Run]()&]
[s2; Same as Execute().&]
[s3; &]
[s4; &]
[s5;:Sql`:`:Execute`(const String`&`):%- [@(0.0.255) bool]_[* Execute]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 s])&]
[s2; Same as SetStatement([%-*@3 s]); return Execute().&]
[s3; &]
[s4; &]
[s5;:Sql`:`:Execute`(const SqlStatement`&`):%- [@(0.0.255) bool]_[* Execute]([@(0.0.255) co
nst]_[_^SqlStatement^ SqlStatement][@(0.0.255) `&]_[*@3 s])&]
[s2; Same as SetStatement([%-*@3 s]); return Execute().&]
[s3; &]
[s4; &]
[s5;:Sql`:`:Run`(const Value`&`[`,const Value`&v2`.`.`.`]`):%- [@(0.0.255) bool]_[* Run](
[@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v1]_`[, [@(0.0.255) const]_[_^Value^ V
alue][@(0.0.255) `&]_[*@3 v2]_...`])&]
[s2; Runs current statment with set of positional parameters. This 
is especially useful for (re)executing prepared statements.&]
[s3; &]
[s4;%- &]
[s5;:Sql`:`:Execute`(const String`&`,const Value`&`[`,const Value`&v2`.`.`.`]`):%- [@(0.0.255) b
ool]_[* Execute]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s], 
[@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v1]_`[, [@(0.0.255) const]_[_^Value^ V
alue][@(0.0.255) `&]_v2_...`])&]
[s2; Assigns a new statement and executes it with a set of position 
parameters.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:Fetch`(`):%- [@(0.0.255) bool]_[* Fetch]()&]
[s2; Attempts to fetch a new row from database (assumes that the 
previous statement executes was [/ select] or some other statement 
with result set). If the row was fetched, returns true.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:Fetch`(Ref`[`,Ref v2`.`.`.`]`):%- [@(0.0.255) bool]_[* Fetch]([_^Ref^ Ref]_[*@3 v
1]_`[, [_^Ref^ Ref]_[*@3 v2]_...`])&]
[s2; Performs Fetch and then assigns fetched values into referenced 
variables.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:Fetch`(Vector`<Value`>`&`):%- [@(0.0.255) bool]_[* Fetch]([_^Vector^ Vector]<[_^Value^ V
alue]>`&_[*@3 row])&]
[s2; Performs Fetch and assigns fetched values into [%-*@3 row].&]
[s3; &]
[s4; &]
[s5;:Sql`:`:Fetch`(ValueMap`&`):%- [@(0.0.255) bool]_[* Fetch]([_^ValueMap^ ValueMap][@(0.0.255) `&
]_[*@3 row])&]
[s2; Performs Fetch and assigns fetched values into [%-*@3 row].&]
[s3; &]
[s4; &]
[s5;:Sql`:`:Fetch`(Fields`):%- [@(0.0.255) bool]_[* Fetch]([_^Fields^ Fields]_[*@3 fields])&]
[s2; Performs Fetch and assigns fetched values into [%-*@3 fields]. 
Fields usually represent some S`_`* table row structure based 
on .sch file.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:GetRowsProcessed`(`)const:%- [@(0.0.255) int]_[* GetRowsProcessed]()_[@(0.0.255) c
onst]&]
[s2; After [/ update] statement, this returns a number of rows that 
have been affected. Note that for some databases, this is the 
number of rows that really changed, for others, this is the number 
of rows that match [/ where].&]
[s2; After [/ insert] statement, this returns a number of rows deleted.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:GetColumns`(`)const:%- [@(0.0.255) int]_[* GetColumns]()_[@(0.0.255) const]&]
[s5;:Sql`:`:GetColumnCount`(`)const:%- [@(0.0.255) int]_[* GetColumnCount]()_[@(0.0.255) co
nst]&]
[s2; Returns the number of columns of resultset after executing [/ select] 
statement.&]
[s3;%- &]
[s4; &]
[s5;:Sql`:`:GetColumn`(int`,Ref`)const:%- [@(0.0.255) void]_[* GetColumn]([@(0.0.255) int]_
[*@3 i], [_^Ref^ Ref]_[*@3 r])_[@(0.0.255) const]&]
[s2; Sets value referenced by [%-*@3 r] to the fetched value at column 
[%-*@3 i]. Only valid after successfull Fetch and if there exists 
column [%-*@3 i] in the resultset.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:GetColumn`(SqlId`,Ref`)const:%- [@(0.0.255) void]_[* GetColumn]([_^SqlId^ SqlId
]_[*@3 colid], [_^Ref^ Ref]_[*@3 r])_[@(0.0.255) const]&]
[s2; Sets value referenced by [%-*@3 r] to the fetched value at column 
with name [%-*@3 colid] (match is case insensitive). Only valid 
after successfull Fetch and if there exists column [%-*@3 colid] 
in the resultset.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:operator`[`]`(int`)const:%- [_^Value^ Value]_[* operator`[`]]([@(0.0.255) int]_
[*@3 i])_[@(0.0.255) const]&]
[s2; Returns the fetched value at column [%-*@3 i]. Only valid after 
successfull Fetch and if there exists column [%-*@3 i] in the resultset.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:operator`[`]`(SqlId`)const:%- [_^Value^ Value]_[* operator`[`]]([_^SqlId^ SqlId
]_[*@3 colid])_[@(0.0.255) const]&]
[s2; Returns the fetched value at column with name [%-*@3 colid] (match 
is case insensitive). Only valid after successfull Fetch and 
if there exists column [%-*@3 colid] in the resultset.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:GetColumnInfo`(int`)const:%- [@(0.0.255) const]_[_^SqlColumnInfo^ SqlColumnIn
fo][@(0.0.255) `&]_[* GetColumnInfo]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2; Returns the information about column [%-*@3 i] after executing 
[/ select] statement.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:GetRow`(`)const:%- [_^Vector^ Vector]<[_^Value^ Value]>_[* GetRow]()_[@(0.0.255) c
onst]&]
[s2; Returns the whole fetched row. Only valid after successfull 
Fetch.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:GetRowMap`(`)const:%- [_^ValueMap^ ValueMap]_[* GetRowMap]()_[@(0.0.255) const]&]
[s5;:Upp`:`:Sql`:`:operator`~`(`)const:%- [_^Upp`:`:ValueMap^ ValueMap]_[* operator`~]()_
[@(0.0.255) const]&]
[s2; Returns the whole fetched row, keys of ValueMap are names of 
columns. Only valid after successfull Fetch.&]
[s3;%- &]
[s4; &]
[s5;:Sql`:`:operator Vector`<Value`>`(`)const:%- [* operator_Vector<Value>]()_[@(0.0.255) c
onst]&]
[s2; Returns GetRow(). Only valid after successfull Fetch.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:Get`(Fields`):%- [@(0.0.255) void]_[* Get]([_^Fields^ Fields]_[*@3 fields])&]
[s2; Retrieves fetched row as [%-*@3 fields]. Fields usually represent 
some S`_`* table row structure based on .sch file. Only valid 
after successfull Fetch.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:Cancel`(`):%- [@(0.0.255) void]_[* Cancel]()&]
[s2; Cancels the current statement. This in practice means that the 
rest of resultset is released.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:Insert`(Fields`):%- [@(0.0.255) bool]_[* Insert]([_^Fields^ Fields]_[*@3 nf])&]
[s2; Executes [/ insert] statement that inserts Fields [%-*@3 nf] into 
database. Fields usually represent some S`_`* table row structure 
based on .sch file.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:Insert`(Fields`,const char`*`):%- [@(0.0.255) bool]_[* Insert]([_^Fields^ Field
s]_[*@3 nf], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 table])&]
[s5;:Sql`:`:Insert`(Fields`,SqlId`):%- [@(0.0.255) bool]_[* Insert]([_^Fields^ Fields]_[*@3 n
f], [_^SqlId^ SqlId]_[*@3 table])&]
[s2; Executes [/ insert] statement that inserts Fields [%-*@3 nf] columns 
into database, but uses different table as target. Fields usually 
represent some S`_`* table row structure based on .sch file.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:InsertNoKey`(Fields`):%- [@(0.0.255) bool]_[* InsertNoKey]([_^Fields^ Fields]_[*@3 n
f])&]
[s2; Executes [/ insert] statement that inserts Fields [%-*@3 nf] into 
database, first column of [%-*@3 nf] is not used (this column usually 
contains key). Fields usually represent some S`_`* table row 
structure based on .sch file.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:InsertNoKey`(Fields`,const char`*`):%- [@(0.0.255) bool]_[* InsertNoKey]([_^Fields^ F
ields]_[*@3 nf], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 table])&]
[s5;:Sql`:`:InsertNoKey`(Fields`,SqlId`):%- [@(0.0.255) bool]_[* InsertNoKey]([_^Fields^ Fi
elds]_[*@3 nf], [_^SqlId^ SqlId]_[*@3 table])&]
[s2; Executes [/ insert] statement that inserts Fields [%-*@3 nf] columns 
into database, but uses different table as target. First column 
of [%-*@3 nf] is not used (this column usually contains primary 
key). Fields usually represent some S`_`* table row structure 
based on .sch file.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:InsertNoNulls`(Fields`):%- [@(0.0.255) bool]_[* InsertNoNulls]([_^Fields^ Field
s]_[*@3 nf])&]
[s2; Executes [/ insert] statement that inserts Fields [%-*@3 nf] into 
database, columns that are null are not used. Fields usually 
represent some S`_`* table row structure based on .sch file.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:InsertNoNulls`(Fields`,const char`*`):%- [@(0.0.255) bool]_[* InsertNoNulls](
[_^Fields^ Fields]_[*@3 nf], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 table])&]
[s5;:Sql`:`:InsertNoNulls`(Fields`,SqlId`):%- [@(0.0.255) bool]_[* InsertNoNulls]([_^Fields^ F
ields]_[*@3 nf], [_^SqlId^ SqlId]_[*@3 table])&]
[s2; Executes [/ insert] statement that inserts Fields [%-*@3 nf] columns 
into database, but uses different table as target. Columns that 
are null are not used. Fields usually represent some S`_`* table 
row structure based on .sch file.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:Update`(Fields`):%- [@(0.0.255) bool]_[* Update]([_^Fields^ Fields]_[*@3 nf])&]
[s2; Executes [/ update] statement that updates a table row in the 
database. First column of [%-*@3 nf] is used as primary key (for 
[/ where] part of statement). Fields usually represent some S`_`* 
table row structure based on .sch file.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:Update`(Fields`,const char`*`):%- [@(0.0.255) bool]_[* Update]([_^Fields^ Field
s]_[*@3 nf], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 table])&]
[s5;:Sql`:`:Update`(Fields`,SqlId`):%- [@(0.0.255) bool]_[* Update]([_^Fields^ Fields]_[*@3 n
f], [_^SqlId^ SqlId]_[*@3 table])&]
[s2; Executes [/ update] statement that updates a table row in the 
database, but uses different table as target. First column of 
[%-*@3 nf] is used as primary key (for [/ where] part of statement). 
Fields usually represent some S`_`* table row structure based 
on .sch file.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:Delete`(const char`*`,const char`*`,const Value`&`):%- [@(0.0.255) bool]_[* D
elete]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 table], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 key], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 keyval])&]
[s5;:Sql`:`:Delete`(SqlId`,SqlId`,const Value`&`):%- [@(0.0.255) bool]_[* Delete]([_^SqlId^ S
qlId]_[*@3 table], [_^SqlId^ SqlId]_[*@3 key], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[*@3 keyval])&]
[s2; Removes a table row from the databases. This is sort of deprecated 
in favor of SqlExp Delete.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:ToString`(`)const:%- [_^String^ String]_[* ToString]()_[@(0.0.255) const]&]
[s2; Returns the current SQL statement.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:operator`*`(const SqlStatement`&`):%- [@(0.0.255) bool]_[* operator`*]([@(0.0.255) c
onst]_[_^SqlStatement^ SqlStatement][@(0.0.255) `&]_[*@3 q])&]
[s2; Same as Execute(Compile(q)).&]
[s3; &]
[s4; &]
[s5;:Sql`:`:operator`%`(const SqlStatement`&`):%- [_^Value^ Value]_[* operator%]([@(0.0.255) c
onst]_[_^SqlStatement^ SqlStatement][@(0.0.255) `&]_[*@3 q])&]
[s2; Executes a query, fetches a single row and returns the value 
of first column. If there is no row in resultset, returns Null. 
In case of error, returns ErrorValue (which is also interpreted 
as Null).&]
[s3; &]
[s4; &]
[s5;:Sql`:`:operator`^`(const SqlStatement`&`):%- [_^ValueMap^ ValueMap]_[* operator`^]([@(0.0.255) c
onst]_[_^SqlStatement^ SqlStatement][@(0.0.255) `&]_[*@3 q])&]
[s2; Executes a query, fetches a single row and returns this row 
as ValueMap where keys are names of columns. If there is no row 
in resultset or in case of error returns empty ValueMap.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:operator`/`(const SqlStatement`&`):%- [_^ValueArray^ ValueArray]_[* operator/
]([@(0.0.255) const]_[_^SqlStatement^ SqlStatement][@(0.0.255) `&]_[*@3 q])&]
[s2; Executes a query and returns the resultset as ValueArray of 
ValueMaps (keys are column names). If there is no row in resultset 
or in case of error returns empty ValueArray.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:GetSession`(`)const:%- [_^SqlSession^ SqlSession][@(0.0.255) `&]_[* GetSession](
)_[@(0.0.255) const]&]
[s2; Returns the associated SqlSession.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:GetDialect`(`)const:%- [@(0.0.255) int]_[* GetDialect]()_[@(0.0.255) const]&]
[s2; Returns the SQL dialect of associated SqlSession.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:GetInsertedId`(`)const:%- [_^Value^ Value]_[* GetInsertedId]()_[@(0.0.255) cons
t]&]
[s2; For databases with auto`-increment options of primary keys, 
returns the last value of primary key inserted. (Note that for 
PGSQL, it is required that the auto`-increment primary key is 
named `"ID`").&]
[s4; &]
[s5;:Sql`:`:IsOpen`(`):%- [@(0.0.255) bool]_[* IsOpen]()&]
[s2; Returns true there is associated SqlSession this this instance 
and the SqlSession is connected to database.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:Sql`(SqlSource`&`):%- [* Sql]([_^SqlSource^ SqlSource][@(0.0.255) `&]_[*@3 src])&]
[s2; Creates Sql instance associated with session [%-*@3 src]. (SqlSession 
is derived from SqlSource, which serves some special internal 
purposes).&]
[s3; &]
[s4; &]
[s5;:Sql`:`:Sql`(`):%- [* Sql]()&]
[s2; Creates Sql instance associated with default session.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:Sql`(const char`*`):%- [* Sql]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 stmt])
&]
[s2; Creates Sql instance associated with default session and assigns 
it a statement.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:Sql`(const SqlStatement`&`):%- [* Sql]([@(0.0.255) const]_[_^SqlStatement^ SqlS
tatement][@(0.0.255) `&]_[*@3 s])&]
[s2; Creates Sql instance associated with default session and assigns 
it a SqlExp statement.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:Sql`(const char`*`,SqlSource`&`):%- [* Sql]([@(0.0.255) const]_[@(0.0.255) char
]_`*[*@3 stmt], [_^SqlSource^ SqlSource][@(0.0.255) `&]_[*@3 session])&]
[s2; Creates Sql instance associated with [%-*@3 session] and assigns 
it a statement.&]
[s3; &]
[s4; &]
[s5;:Sql`:`:Sql`(const SqlStatement`&`,SqlSource`&`):%- [* Sql]([@(0.0.255) const]_[_^SqlStatement^ S
qlStatement][@(0.0.255) `&]_[*@3 s], [_^SqlSource^ SqlSource][@(0.0.255) `&]_[*@3 session])
&]
[s2; Creates Sql instance associated with [%-*@3 session] and assigns 
it a SqlExp statement.&]
[s3; &]
[s4;%- &]
[s5;:Sql`:`:PerThread`(bool`):%- [@(0.0.255) static] [@(0.0.255) void]_[* PerThread]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; In multithreded mode, activates mode where each thread has assigned 
unique per`-thread default session (otherwise default session 
is shared).&]
[s3; &]
[s0; ]]