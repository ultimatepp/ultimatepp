topic ".sch SQL schema files";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 .sch SQL schema files]]}}&]
[s0;%% &]
[s0; [3 .sch files represent SQL database schema. These files are included 
into target application with various definitions of .sch `"keywords`" 
to provide]&]
[s0;3 &]
[s0;i150;O0; [3 SqlId constants]&]
[s0;i150;O0; [^topic`:`/`/Sql`/src`/S`_type`_en`-us^3 S`_][/^topic`:`/`/Sql`/src`/S`_type`_en`-us^3 t
ype][3  structures to map table rows into C`+`+ structure]&]
[s0;i150;O0; [3 .sql scripts to create/upgrade/drop the schema]&]
[s0;3 &]
[s0; [3 Example of .sch content:]&]
[s0;*C@5;3 &]
[s0; [*C@5;3 TABLE`_(DEMO)]&]
[s0; [*C@5;3 -|INT`_(ID) PRIMARY`_KEY AUTOINCREMENT]&]
[s0; [*C@5;3 -|STRING`_(UN, 200) UNIQUE]&]
[s0; [*C@5;3 -|STRING`_(W, 200) INDEX]&]
[s0; [*C@5;3 -|INT`_ (F`_ID) REFERENCES(TABLE2)]&]
[s0; [*C@5;3 END`_TABLE]&]
[s0;3 &]
[s0; [3 All .sch keywords that contain the name of table, type or column, 
have two variants `- simple and with `'`_`' posfix (e.g. TABLE 
and TABLE`_). Variant with `'`_`' postfix also generates global 
SqlId variable that has the same name and value as is the name 
of table/type/column (content can be altered with SQL`_NAME). 
This prepares column and table names to be used with SQL expressions.]&]
[s0;3 &]
[s0; [3 Most column definition keywords come with ARRAY variant, this 
is not using any SQL engine array capabilities but rather inserts 
array size count of items with special names to emulate the fixed 
array.]&]
[s0;3 &]
[s0; [3 List of .sch file keywords:]&]
[s0;3 &]
[ {{5007:4993^ [s0; [* TABLE(][*/ table][* )]]
:: [s0; Defines a new table. This creates S`_ structures to map SQL 
table to C`+`+ structure, introspection records and also can 
create SQL schema creation/upgrade scripts.]
:: [s0; [* TABLE`_I(][*/ table][* , ][*/ base1][* )]]
:: [s0; Defines a new table which also has all columns of [/ base1] which 
can be either table or type.]
:: [s0; [* TABLE`_II(][*/ table][* , ][*/ base1][* , ][*/ base2][* )]]
:: [s0; Defines a new table which also has all columns of [/ base1 ]and 
[/ base2 ]which can be either tables or types.]
:: [s0; [* TABLE`_III(][*/ table][* , ][*/ base1][* , ][*/ base2][* , ][*/ base3][* )]]
:: [s0; Defines a new table which also has all columns of [/ base1 , base2 
]and [/ base3] which can be either tables or types.]
:: [s0; [* END`_TABLE]]
:: [s0; Ends table definition.]
:: [s0; [* TYPE(][*/ type][* )]]
:: [s0; Defines a new type. Unlike TABLE, TYPE does not produce table 
creation code in schema sql scripts. Following this header is 
a list of columns and inline attributes, ending with END`_TYPE.]
:: [s0; [* TYPE`_I(][*/ type][* , ][*/ base][* )]]
:: [s0; Defines a new type which also has all columns of [/ base.]]
:: [s0; [* TYPE`_II(][*/ type][* , ][*/ base1][* , ][*/ base2][* )]]
:: [s0; Defines a new type which also has all columns of [/ base1 ]and 
[/ base2].]
:: [s0; [* TYPE`_III(][*/ type][* , ][*/ base1][* , ][*/ base2][* , ][*/ base3][* )]]
:: [s0; Defines a new type which also has all columns of [/ base1 , base2 
]and [/ base3].]
:: [s0; [* END`_TYPE]]
:: [s0; Ends type definition.]
:: [s0; [* SQL`_NAME(][*/ id][* )]]
:: [s0; Provides an alternate SQL name of previous item. When used, 
the column name will be used in C`+`+ context, while this alternate 
name will be use in SQL context. [*/ id] is C literal.]
:: [s0; ]
:: [s0; ]
:: [s0; [* INT(][*/ column`_name][* )]]
:: [s0; Column capable of storing 32`-bit integer value, int type in 
C`+`+.]
:: [s0; [* INT`_ARRAY(][*/ column`_name][* , ][*/ items][* )]]
:: [s0; Array of 32`-bit integers.]
:: [s0; [* INT64(][*/ column`_name][* )]]
:: [s0; Column capable of storing 64`-bit integer value, int64 type 
in C`+`+. `[sqlite3`]]
:: [s0; [* INT64`_ARRAY(][*/ column`_name][* , ][*/ items][* )]]
:: [s0; Array of 64`-bit integers. `[sqlite3`]]
:: [s0; [* DOUBLE(][*/ column`_name][* )]]
:: [s0; Column capable of storing double precision floating point value, 
double type in C`+`+.]
:: [s0; [* DOUBLE`_ARRAY(][*/ column`_name][* , ][*/ items][* )]]
:: [s0; Array of double values.]
:: [s0; [* STRING(][*/ column`_name][* , ][*/ maxlen][* )]]
:: [s0; Column capable of storing string with character count limit 
[/ maxlen], String type in C`+`+.]
:: [s0; [* STRING`_ARRAY(][*/ column`_name][* , n, items)]]
:: [s0; Array of strings.]
:: [s0; [* DATE(][*/ column`_name][* )]]
:: [s0; Column capable of storing calendar date (without time), Date 
type in C`+`+.]
:: [s0; [* DATE`_ARRAY(][*/ column`_name][* , items)]]
:: [s0; Array of dates.]
:: [s0; [* TIME(][*/ column`_name][* )]]
:: [s0; Column capable of storing calendar date with time, Time type 
in C`+`+.]
:: [s0; [* TIME`_ARRAY(][*/ column`_name][* , items)]]
:: [s0; Array of date`-times.]
:: [s0; [* BOOL(][*/ column`_name][* )]]
:: [s0; Column capable of storing boolean value, bool type in C`+`+. 
Important: for compatibility reasons between various SQL engines, 
BOOL is always emulated with single character text value (`"1`" 
for true). Conversion is provided for S`_ types, but SQL commands 
must account for this. ]
:: [s0; [* BOOL`_ARRAY(][*/ column`_name][* , items)]]
:: [s0; Array of bools.]
:: [s0; [* BIT]]
:: [s0; 0 or 1. C`+`+ type is int. `[mssql`]]
:: [s0; [* BIT`_ARRAY]]
:: [s0; Array of 0 or 1. `[mssql`]]
:: [s0; [* BLOB(][*/ column`_name][* )]]
:: [s0; Binary data of unlimited size. C`+`+ type is String.]
:: [s0; [* CLOB(][*/ column`_name][* )]]
:: [s0; Text of unlimited size. C`+`+ type is String. `[mysql`] `[postgresql`] 
`[oracle`]]
:: [s0; [* SERIAL(][*/ column`_name][* )]]
:: [s0; PostgreSQL [* serial] type (equivalent of autoincrement in some 
other SQL engines). Database type is 32`-bit unsigned integer, 
C`+`+ type is int64.]
:: [s0; [* ISERIAL(][*/ column`_name][* )]]
:: [s0; PostgreSQL [* serial] type with int as C`+`+ type.]
:: [s0; [* BIGSERIAL(][*/ column`_name][* )]]
:: [s0; PostgreSQL 64`-bit [* bigserial] type with int64 as C`+`+ type.]
:: [s0; [* NOT`_NULL]]
:: [s0; Not null constraint.]
:: [s0; [* PRIMARY`_KEY]]
:: [s0; Primary key column.]
:: [s0; [* AUTO`_INCREMENT]]
:: [s0; Autoincrement column. `[sqlite3`] `[mysql`]]
:: [s0; [* REFERENCES(][*/ table][* )]]
:: [s0; Foreign key specification for column.]
:: [s0; [* REFERENCES`_CASCADE(][*/ table][* )]]
:: [s0; Foreign key with `"ON DELETE CASCADE`" option. `[sqlite3`]]
:: [s0; [* DUAL`_PRIMARY`_KEY(][*/ column1][* , ][*/ column2][* )]]
:: [s0; Dual primary key for table.]
:: [s0; [* SQLDEFAULT(v)]]
:: [s0; Default value for column. `[sqlite3`] `[mysql`] `[postgresql`]]
:: [s0; [* INDEX]]
:: [s0; Column has index.]
:: [s0; [* PARTIAL`_INDEX(][*/ condition][* )]]
:: [s0; Column has partial index. `[postgresql`]]
:: [s0; [* PARTIAL`_INDEX`_(][*/ constraint`_name][* , ][*/ condition][* )]]
:: [s0; Column has named partial index. `[postgresql`]]
:: [s0; [* UNIQUE]]
:: [s0; Column has UNIQUE constraint.]
:: [s0; [* UNIQUE`_LIST(][*/ constraint`_name][* , ][*/ list][* )]]
:: [s0; Creates UNIQUE constraint for set of columns. [*/ list] is a C 
literal with comma separated column names.]
:: [s0; [* DUAL`_UNIQUE(][*/ column1][* , ][*/ column1][* )]]
:: [s0; UNIQUE constraint for two columns.]
:: [s0; [* INDEX`_LIST(][*/ constraint`_name][* , ][*/ list][* )]]
:: [s0; [*/ list] is a C literal with comma separated column names. `[sqlite3`]]
:: [s0; [* CHECK`_IN(][*/ list][* )]]
:: [s0; Constraints column`'s value to a [*/ list], which is comma separated 
C literal. `[sqlite3`]]
:: [s0; [* CHECK`_COND(][*/ condition][* )]]
:: [s0; Constraints column`'s value to satisfy a [*/ condition], which 
is C literal with expression. `[sqlite3`]]
:: [s0; [* SQLCHECK(][*/ constraint`_name][* , ][*/ check][* )]]
:: [s0; Adds named CHECK constraint, [*/ check] is C literal with check 
expression. `[sqlite3`] `[postgresql`] `[mssql`]]
:: [s0; [* INNODB]]
:: [s0; Table has INNODB type. `[mysql`]]
:: [s0; [* SEQUENCE(][*/ sequence`_name][* )]]
:: [s0; Sequence. `[postgresql`] `[mssql`] `[oracle`]]
:: [s0; [* COMMEN(][*/ comment][* )]]
:: [s0; Inserts a comment into the script.]}}&]
[s0;%% &]
[s0;3 &]
[s0;3 &]
[s0;%% ]]