topic "SQL Tutorial";
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[H4;b83;*4 $$3,5#07864147445237544204411237157677:title]
[i288;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;ph2 $$5,5#45413000475342174754091244180557:text]
[l288;b17;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;C@5+75 $$7,7#20902679421464641399138805415013:code]
[b2503;2 $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base]
[C2 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam]
[b167;C2 $$13,13#92430459443460461911108080531343:item1]
[i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128)2 $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7;2 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line]
[l160;*C+117 $$18,5#88603949442205825958800053222425:package`-title]
[2 $$19,0#53580023442335529039900623488521:gap]
[C2 $$20,20#70211524482531209251820423858195:class`-nested]
[b50;2 $$21,21#03324558446220344731010354752573:Par]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_} 
[s2;%% SQL Tutorial&]
[s3;%% Table of contents&]
[s0;%% &]
[s0;%% [^topic`:`/`/Sql`/srcdoc`/tutorial`$en`-us`#1^ 1. SqlSession, 
Sql, opening database connection]&]
[s0;%% [^topic`:`/`/Sql`/srcdoc`/tutorial`$en`-us`#2^ 2. Using global 
main database, executing statements with parameters, getting 
resultset info]&]
[s0;%% [^topic`:`/`/Sql`/srcdoc`/tutorial`$en`-us`#3^ 3. Using SqlExp]&]
[s0;%% [^topic`:`/`/Sql`/srcdoc`/tutorial`$en`-us`#4^ 4. Schema file]&]
[s0;%% [^topic`:`/`/Sql`/srcdoc`/tutorial`$en`-us`#5^ 5. Using schema 
file to define SqlId constants]&]
[s0;%% [^topic`:`/`/Sql`/srcdoc`/tutorial`$en`-us`#6^ 6. Using structures 
defined by schema files]&]
[s0;%% &]
[s3;:1:%% 1. SqlSession, Sql, opening database connection&]
[s5;%% SqlSession derived objects represent database connection. 
Each SQL database (Sqlite3, Microsoft SQL, Oracle, MySQL, PostgreSQL) 
has its own session class derived from SqlSession. Sql class 
is used to issue SQL statements and retrieve results:&]
[s5;%% &]
[s7;l320; [@(128.0.255) #include][@0  ][@(0.0.255) <][@0 Core][@(0.0.255) /][@0 Core][@(0.0.255) .][@0 h
][@(0.0.255) >]&]
[s0;l320; [C@(128.0.255)+75 #include][C+75  ][C@(0.0.255)+75 <][C+75 plugin][C@(0.0.255)+75 /][C+75 s
qlite3][C@(0.0.255)+75 /][C+75 Sqlite3][C@(0.0.255)+75 .][C+75 h][C@(0.0.255)+75 >]&]
[s0;l320;C+75 &]
[s0;l320; [*C@(0.0.255)+75 using][C+75  ][*C@(0.0.255)+75 namespace][C+75  
Upp;]&]
[s0;l320;C+75 &]
[s0;l320; [C+75 CONSOLE`_APP`_MAIN]&]
[s0;l320; [C+75 `{]&]
[s0;l320; [C+75     ][*_C+75 Sqlite3Session][C+75  sqlite3;]&]
[s0;l320; [C+75     ][*C@(0.0.255)+75 if][C+75 (][C@(0.0.255)+75 !][C+75 sqlite3][C@(0.0.255)+75 .
][*_C+75 Open][C+75 (ConfigFile(][C@3+75 `"simple.db`"][C+75 ))) `{]&]
[s0;l320; [C+75         Cout() ][C@(0.0.255)+75 <<][C+75  ][C@3+75 `"Can`'t 
create or open database file][C@(0.0.255)+75 `\n][C@3+75 `"][C+75 ;]&]
[s0;l320; [C+75         ][*_C@(128.0.255)+75 return][C+75 ;]&]
[s0;l320; [C+75     `}]&]
[s0;l320; [C+75     ]&]
[s0;l320; [C@(128.0.255)+75 #ifdef][C+75  `_DEBUG]&]
[s0;l320; [C+75     sqlite3][C@(0.0.255)+75 .][*_C+75 SetTrace][C+75 ();]&]
[s0;l320; [C@(128.0.255)+75 #endif]&]
[s0;l320;C+75 &]
[s0;l320; [C+75     ][*_C+75 Sql][C+75  sql(sqlite3);]&]
[s0;l320; [C+75     sql][C@(0.0.255)+75 .][*_C+75 Execute][C+75 (][C@3+75 `"select 
date(`'now`')`"][C+75 );]&]
[s0;l320; [C+75     ][*C@(0.0.255)+75 while][C+75 (sql][C@(0.0.255)+75 .][*_C+75 Fetch][C+75 ())]&]
[s0;l320; [C+75         Cout() ][C@(0.0.255)+75 <<][C+75  ][*_C+75 sql][*_C@(0.0.255)+75 `[][*_C@3+75 0
][*_C@(0.0.255)+75 `]][C+75  ][C@(0.0.255)+75 <<][C+75  ][C@3+75 `'][C@(0.0.255)+75 `\n][C@3+75 `'
][C+75 ;]&]
[s0;l320; [C+75 `}]&]
[s5;%% &]
[s5;%% In this tutorial, we are using Sqlite3 database. The connection 
method varies with database; in this case it is done using [* Open 
]statement.&]
[s5;%% [* SetTrace ]is useful in debug mode `- all issued SQL statements 
and SQL errors are logged in standard U`+`+ log.&]
[s5;%% Each [* Sql] instance has to be associated to some SqlSession 
`- it is passed as constructor parameter (parameter`-less Sql 
constructor uses global session, more on that in section 2.). 
To execute SQL statements, use [* Execute]. If executed statement 
is Select, it may return a result set, which is retrieved using 
[* Fetch]. Columns of result set are then accessed by Sql`::operator`[`] 
using index of column (starts with 0). Values are returned as 
Value type.&]
[s5;%% &]
[s3;:2:%% 2. Using global main database, executing statements with 
parameters, getting resultset info&]
[s5;%% Most applications need to work with just single database backend, 
therefore repeating SqlSession parameter in all Sql declarations 
would be tedious.&]
[s5;%% To this end U`+`+ supports concept of `"main database`" which 
is represented by [* SQL] variable. [* SQL] is of Sql type. When 
any other Sql variable is created with default constructor (no 
session parameter provided), it uses the same session as the 
one the SQL is bound to. To assign session to global SQL, use 
operator`=:&]
[s5; &]
[s0;l320; [C@(128.0.255)+75 #include][C+75  ][C@(0.0.255)+75 <][C+75 Core][C@(0.0.255)+75 /][C+75 C
ore][C@(0.0.255)+75 .][C+75 h][C@(0.0.255)+75 >]&]
[s0;l320; [C@(128.0.255)+75 #include][C+75  ][C@(0.0.255)+75 <][C+75 plugin][C@(0.0.255)+75 /][C+75 s
qlite3][C@(0.0.255)+75 /][C+75 Sqlite3][C@(0.0.255)+75 .][C+75 h][C@(0.0.255)+75 >]&]
[s0;l320;C+75 &]
[s0;l320; [*C@(0.0.255)+75 using][C+75  ][*C@(0.0.255)+75 namespace][C+75  
Upp;]&]
[s0;l320;C+75 &]
[s0;l320; [C+75 CONSOLE`_APP`_MAIN]&]
[s0;l320; [C+75 `{]&]
[s0;l320; [C+75     Sqlite3Session sqlite3;]&]
[s0;l320; [C+75     ][*C@(0.0.255)+75 if][C+75 (][C@(0.0.255)+75 !][C+75 sqlite3][C@(0.0.255)+75 .
][C+75 Open(ConfigFile(][C@3+75 `"simple.db`"][C+75 ))) `{]&]
[s0;l320; [C+75         Cout() ][C@(0.0.255)+75 <<][C+75  ][C@3+75 `"Can`'t 
create or open database file][C@(0.0.255)+75 `\n][C@3+75 `"][C+75 ;]&]
[s0;l320; [C+75         ][*_C@(128.0.255)+75 return][C+75 ;]&]
[s0;l320; [C+75     `}]&]
[s0;l320; [C+75     ]&]
[s0;l320; [C@(128.0.255)+75 #ifdef][C+75  `_DEBUG]&]
[s0;l320; [C+75     sqlite3][C@(0.0.255)+75 .][C+75 SetTrace();]&]
[s0;l320; [C@(128.0.255)+75 #endif]&]
[s0;l320;C+75 &]
[s0;l320; [C+75     ][*_C+75 SQL][C+75  ][C@(0.0.255)+75 `=][C+75  sqlite3;]&]
[s0;l320; [C+75     ]&]
[s0;l320; [C+75     ][*_C+75 SQL][C@(0.0.255)+75 .][C+75 Execute(][C@3+75 `"drop 
table TEST`"][C+75 );]&]
[s0;l320; [C+75     SQL][C@(0.0.255)+75 .][C+75 ClearError();]&]
[s0;l320; [C+75     SQL][C@(0.0.255)+75 .][C+75 Execute(][C@3+75 `"create 
table TEST (A INTEGER, B TEXT)`"][C+75 );]&]
[s0;l320;C+75 &]
[s0;l320; [C+75     ][*C@(0.0.255)+75 for][C+75 (][*C@(0.0.255)+75 int][C+75  
i ][C@(0.0.255)+75 `=][C+75  ][C@3+75 0][C+75 ; i ][C@(0.0.255)+75 <][C+75  
][C@3+75 10][C+75 ; i][C@(0.0.255)+75 `+`+][C+75 )]&]
[s0;l320; [C+75         SQL][C@(0.0.255)+75 .][*_C+75 Execute][C+75 (][C@3+75 `"insert 
into TEST(A, B) values (?, ?)`"][C+75 , i, AsString(][C@3+75 3][C+75  
][C@(0.0.255)+75 `*][C+75  i));]&]
[s0;l320;C+75 &]
[s0;l320; [C+75     Sql sql;]&]
[s0;l320; [C+75     sql][C@(0.0.255)+75 .][C+75 Execute(][C@3+75 `"select 
`* from TEST`"][C+75 );]&]
[s0;l320; [C+75     ][*C@(0.0.255)+75 for][C+75 (][*C@(0.0.255)+75 int][C+75  
i ][C@(0.0.255)+75 `=][C+75  ][C@3+75 0][C+75 ; i ][C@(0.0.255)+75 <][C+75  
sql][C@(0.0.255)+75 .][*_C+75 GetColumns][C+75 (); i][C@(0.0.255)+75 `+`+][C+75 )]&]
[s0;l320; [C+75         Cout() ][C@(0.0.255)+75 <<][C+75  sql][C@(0.0.255)+75 .][*_C+75 GetColu
mnInfo(i)][*_C@(0.0.255)+75 .][*_C+75 name][C+75  ][C@(0.0.255)+75 <<][C+75  
][C@3+75 `'][C@(0.0.255)+75 `\n][C@3+75 `'][C+75 ;]&]
[s0;l320; [C+75     ][*C@(0.0.255)+75 while][C+75 (sql][C@(0.0.255)+75 .][C+75 Fetch())]&]
[s0;l320; [C+75         Cout() ][C@(0.0.255)+75 <<][C+75  sql][C@(0.0.255)+75 `[][C@3+75 0][C@(0.0.255)+75 `]
][C+75  ][C@(0.0.255)+75 <<][C+75  ][C@3+75 `" ][C@(0.0.255)+75 `\`'][C@3+75 `"][C+75  
][C@(0.0.255)+75 <<][C+75  sql][C@(0.0.255)+75 `[][C@3+75 1][C@(0.0.255)+75 `]][C+75  
][C@(0.0.255)+75 <<][C+75  ][C@3+75 `"][C@(0.0.255)+75 `\`'`\n][C@3+75 `"][C+75 ;]&]
[s0;l320; [C+75 `}]&]
[s5;l320;%% &]
[s5;%% As global [* SQL] is regular Sql variable too, it can be used 
to issue SQL statements.&]
[s5;%% [/ Warning: While it is possible to issue ][*/ select][/  statements 
through ][*/ SQL][/ , based on experience this is not recommended 
`- way too often result set of ][*/ select][/  is canceled by issuing 
some other command, e.g. in routine called as part of Fetch loop. 
One exception to this rule is using SQL`::operator% to fetch single 
value like ][*C@5+75 String txt `= SQL % Select(TEXT).From(DOCTEMPLATE).Where(ID 
`=`= id); ][/ (see further tutorial topics for detailed explanation 
of this code).]&]
[s5;%% To get information about result set columns, you can use [* GetColumns 
]to retrieve the number of columns and [* GetColumnInfo] to retrieve 
information about columns `- returns [* SqlColumnInfo] reference 
with information like name or type of column.&]
[s5;%% &]
[s3;:3:%% 3. Using SqlExp&]
[s5;%% U`+`+ contains an unique feature, `"SqlExp`". This is a mechanism 
where you construct SQL statements as C`+`+ expressions (using 
heavily overloaded operators).&]
[s5;%% There are three advantages to this approach:&]
[s5;l160;i150;O0;%% SQL statements are at least partially checked 
at compile time&]
[s5;l160;i150;O0;%% As such statements are yet to be interpreted, 
it is possible to hide some differences between DB engines&]
[s5;l160;i150;O0;%% It is much easier to create complex dynamic SQL 
statements&]
[s5;%% Database entity identifiers (like table or column names) can 
be defined as [* SqlId] type. For the complete lest of supported 
SQL statements, see [^topic`:`/`/Sql`/src`/SqlExp`$en`-us^ SqlExp 
in examples].&]
[s5;%% &]
[s7;l320; [@(128.0.255) #include][@0  ][@(0.0.255) <][@0 Core][@(0.0.255) /][@0 Core][@(0.0.255) .][@0 h
][@(0.0.255) >]&]
[s0;l320; [C@(128.0.255)+75 #include][C+75  ][C@(0.0.255)+75 <][C+75 plugin][C@(0.0.255)+75 /][C+75 s
qlite3][C@(0.0.255)+75 /][C+75 Sqlite3][C@(0.0.255)+75 .][C+75 h][C@(0.0.255)+75 >]&]
[s0;l320;C+75 &]
[s0;l320; [*C@(0.0.255)+75 using][C+75  ][*C@(0.0.255)+75 namespace][C+75  
Upp;]&]
[s0;l320;C+75 &]
[s0;l320; [C+75 CONSOLE`_APP`_MAIN]&]
[s0;l320; [C+75 `{]&]
[s0;l320; [C+75     Sqlite3Session sqlite3;]&]
[s0;l320; [C+75     ][*C@(0.0.255)+75 if][C+75 (][C@(0.0.255)+75 !][C+75 sqlite3][C@(0.0.255)+75 .
][C+75 Open(ConfigFile(][C@3+75 `"simple.db`"][C+75 ))) `{]&]
[s0;l320; [C+75         Cout() ][C@(0.0.255)+75 <<][C+75  ][C@3+75 `"Can`'t 
create or open database file][C@(0.0.255)+75 `\n][C@3+75 `"][C+75 ;]&]
[s0;l320; [C+75         ][*_C@(128.0.255)+75 return][C+75 ;]&]
[s0;l320; [C+75     `}]&]
[s0;l320; [C+75     ]&]
[s0;l320; [C@(128.0.255)+75 #ifdef][C+75  `_DEBUG]&]
[s0;l320; [C+75     sqlite3][C@(0.0.255)+75 .][C+75 SetTrace();]&]
[s0;l320; [C@(128.0.255)+75 #endif]&]
[s0;l320;C+75 &]
[s0;l320; [C+75     SQL ][C@(0.0.255)+75 `=][C+75  sqlite3;]&]
[s0;l320; [C+75     ]&]
[s0;l320; [C+75     SQL][C@(0.0.255)+75 .][C+75 Execute(][C@3+75 `"drop table 
TEST`"][C+75 );]&]
[s0;l320; [C+75     SQL][C@(0.0.255)+75 .][C+75 ClearError();]&]
[s0;l320; [C+75     SQL][C@(0.0.255)+75 .][C+75 Execute(][C@3+75 `"create 
table TEST (A INTEGER, B TEXT)`"][C+75 );]&]
[s0;l320;C+75 &]
[s0;l320; [C+75     ][*_C+75 SqlId][C+75  A(][C@3+75 `"A`"][C+75 ), B(][C@3+75 `"B`"][C+75 ), 
TEST(][C@3+75 `"TEST`"][C+75 );]&]
[s0;l320;C+75 &]
[s0;l320; [C+75     ][*C@(0.0.255)+75 for][C+75 (][*C@(0.0.255)+75 int][C+75  
i ][C@(0.0.255)+75 `=][C+75  ][C@3+75 0][C+75 ; i ][C@(0.0.255)+75 <][C+75  
][C@3+75 10][C+75 ; i][C@(0.0.255)+75 `+`+][C+75 )]&]
[s0;l320; [C+75         SQL ][*_C@(0.0.255)+75 `*][*_C+75  Insert(TEST)(A, 
i)(B, AsString(][*_C@3+75 3][*_C+75  ][*_C@(0.0.255)+75 `*][*_C+75  i))][C+75 ;]&]
[s0;l320;C+75 &]
[s0;l320; [C+75     Sql sql;]&]
[s0;l320; [C+75     sql ][*_C@(0.0.255)+75 `*][*_C+75  Select(A, B)][*_C@(0.0.255)+75 .][*_C+75 F
rom(TEST)][C+75 ;]&]
[s0;l320; [C+75     ][*C@(0.0.255)+75 while][C+75 (sql][C@(0.0.255)+75 .][C+75 Fetch())]&]
[s0;l320; [C+75         Cout() ][C@(0.0.255)+75 <<][C+75  ][*_C+75 sql][*_C@(0.0.255)+75 `[][*_C+75 A
][*_C@(0.0.255)+75 `]][C+75  ][C@(0.0.255)+75 <<][C+75  ][C@3+75 `" ][C@(0.0.255)+75 `\`'][C@3+75 `"
][C+75  ][C@(0.0.255)+75 <<][C+75  ][*_C+75 sql][*_C@(0.0.255)+75 `[][*_C+75 B][*_C@(0.0.255)+75 `]
][C+75  ][C@(0.0.255)+75 <<][C+75  ][C@3+75 `"][C@(0.0.255)+75 `\`'`\n][C@3+75 `"][C+75 ;]&]
[s0;l320; [C+75 `}]&]
[s5;l320; &]
[s5;%% SqlId identifiers can be also used as parameter of Sql`::operator`[`] 
to retrieve particular columns of result`-set.&]
[s5;%% &]
[s3;:4:%% 4. Schema file&]
[s5;%% Schema files can be used to describe the database schema. 
Such schema files can be used to upload the schema to the database, 
to defined SqlId constants and also to work with database records 
as C`+`+ structures.&]
[s5;%% Following example demonstrates using schema file to create 
database schema in SQL database server.&]
[s5;%% [/ MyApp.sch:]&]
[s5;%% &]
[s0;l320; [*C@(0.0.255)+75 TABLE][C+75 (TEST)]&]
[s0;l320; [C+75     ][*C@(0.0.255)+75 INT][C+75         (A)]&]
[s0;l320; [C+75     ][*C@(0.0.255)+75 STRING][C+75      (B, ][C@3+75 200][C+75 )]&]
[s0;l320; [*C@(0.0.255)+75 END][C@(0.0.255)+75 `_][*C@(0.0.255)+75 TABLE]&]
[s5;%% &]
[s5;%% [/ MyApp.h:]&]
[s5;/%% &]
[s0;l320; [C@(128.0.255)+75 #ifndef][C+75  `_MyApp`_h`_]&]
[s0;l320; [C@(128.0.255)+75 #define][C+75  `_MyApp`_h`_]&]
[s0;l320;C+75 &]
[s0;l320; [C@(128.0.255)+75 #include][C+75  ][C@(0.0.255)+75 <][C+75 Core][C@(0.0.255)+75 /][C+75 C
ore][C@(0.0.255)+75 .][C+75 h][C@(0.0.255)+75 >]&]
[s0;l320; [C@(128.0.255)+75 #include][C+75  ][C@(0.0.255)+75 <][C+75 plugin][C@(0.0.255)+75 /][C+75 s
qlite3][C@(0.0.255)+75 /][C+75 Sqlite3][C@(0.0.255)+75 .][C+75 h][C@(0.0.255)+75 >]&]
[s0;l320;C+75 &]
[s0;l320; [*C@(0.0.255)+75 using][C+75  ][*C@(0.0.255)+75 namespace][C+75  
Upp;]&]
[s0;l320;C+75 &]
[s0;l320; [*_C@(128.0.255)+75 #define][*_C+75  SCHEMADIALECT ][*_C@(0.0.255)+75 <][*_C+75 plu
gin][*_C@(0.0.255)+75 /][*_C+75 sqlite3][*_C@(0.0.255)+75 /][*_C+75 Sqlite3Schema][*_C@(0.0.255)+75 .
][*_C+75 h][*_C@(0.0.255)+75 >]&]
[s0;l320; [*_C@(128.0.255)+75 #define][*_C+75  MODEL ][*_C@(0.0.255)+75 <][*_C+75 Sql04][*_C@(0.0.255)+75 /
][*_C+75 MyApp][*_C@(0.0.255)+75 .][*_C+75 sch][*_C@(0.0.255)+75 >]&]
[s0;l320; [*_C@(128.0.255)+75 #include][*_C+75  ][*_C@3+75 `"Sql/sch`_header.h`"]&]
[s0;l320;C+75 &]
[s0;l320; [C@(128.0.255)+75 #endif]&]
[s5;%% &]
[s5;%% [/ main.cpp:]&]
[s5;/%% &]
[s0;l320; [C@(128.0.255)+75 #include][C+75  ][C@3+75 `"MyApp.h`"]&]
[s0;l320;C+75 &]
[s0;l320; [*_C@(128.0.255)+75 #include][*_C+75  ][*_C@(0.0.255)+75 <][*_C+75 Sql][*_C@(0.0.255)+75 /
][*_C+75 sch`_schema][*_C@(0.0.255)+75 .][*_C+75 h][*_C@(0.0.255)+75 >]&]
[s0;l320; [*_C@(128.0.255)+75 #include][*_C+75  ][*_C@(0.0.255)+75 <][*_C+75 Sql][*_C@(0.0.255)+75 /
][*_C+75 sch`_source][*_C@(0.0.255)+75 .][*_C+75 h][*_C@(0.0.255)+75 >]&]
[s0;l320;C+75 &]
[s0;l320; [C+75 CONSOLE`_APP`_MAIN]&]
[s0;l320; [C+75 `{]&]
[s0;l320; [C+75     Sqlite3Session sqlite3;]&]
[s0;l320; [C+75     ][*C@(0.0.255)+75 if][C+75 (][C@(0.0.255)+75 !][C+75 sqlite3][C@(0.0.255)+75 .
][C+75 Open(ConfigFile(][C@3+75 `"simple.db`"][C+75 ))) `{]&]
[s0;l320; [C+75         Cout() ][C@(0.0.255)+75 <<][C+75  ][C@3+75 `"Can`'t 
create or open database file][C@(0.0.255)+75 `\n][C@3+75 `"][C+75 ;]&]
[s0;l320; [C+75         ][*_C@(128.0.255)+75 return][C+75 ;]&]
[s0;l320; [C+75     `}]&]
[s0;l320; [C+75     ]&]
[s0;l320; [C@(128.0.255)+75 #ifdef][C+75  `_DEBUG]&]
[s0;l320; [C+75     sqlite3][C@(0.0.255)+75 .][C+75 SetTrace();]&]
[s0;l320; [C@(128.0.255)+75 #endif]&]
[s0;l320;C+75 &]
[s0;l320; [C+75     SQL ][C@(0.0.255)+75 `=][C+75  sqlite3;]&]
[s0;l320;C+75 &]
[s0;l320; [C+75     ][*_C+75 SqlSchema sch(SQLITE3);]&]
[s0;l320; [C+75     ][*_C+75 All`_Tables(sch);]&]
[s0;l320; [C+75     ][*_C+75 SqlPerformScript(sch][*_C@(0.0.255)+75 .][*_C+75 Upgrade());]&]
[s0;l320; [C+75     ][*_C+75 SqlPerformScript(sch][*_C@(0.0.255)+75 .][*_C+75 Attributes());]&]
[s0;l320; [C+75     ][*_C+75 SQL][*_C@(0.0.255)+75 .][*_C+75 ClearError();]&]
[s0;l320;C+75 &]
[s0;l320; [C+75     SqlId A(][C@3+75 `"A`"][C+75 ), B(][C@3+75 `"B`"][C+75 ), 
TEST(][C@3+75 `"TEST`"][C+75 );]&]
[s0;l320;C+75 &]
[s0;l320; [C+75     ][*C@(0.0.255)+75 for][C+75 (][*C@(0.0.255)+75 int][C+75  
i ][C@(0.0.255)+75 `=][C+75  ][C@3+75 0][C+75 ; i ][C@(0.0.255)+75 <][C+75  
][C@3+75 10][C+75 ; i][C@(0.0.255)+75 `+`+][C+75 )]&]
[s0;l320; [C+75         SQL ][C@(0.0.255)+75 `*][C+75  Insert(TEST)(A, 
i)(B, AsString(][C@3+75 3][C+75  ][C@(0.0.255)+75 `*][C+75  i));]&]
[s0;l320;C+75 &]
[s0;l320; [C+75     Sql sql;]&]
[s0;l320; [C+75     sql ][C@(0.0.255)+75 `*][C+75  Select(A, B)][C@(0.0.255)+75 .][C+75 From(TE
ST);]&]
[s0;l320; [C+75     ][*C@(0.0.255)+75 while][C+75 (sql][C@(0.0.255)+75 .][C+75 Fetch())]&]
[s0;l320; [C+75         Cout() ][C@(0.0.255)+75 <<][C+75  sql][C@(0.0.255)+75 `[][C+75 A][C@(0.0.255)+75 `]
][C+75  ][C@(0.0.255)+75 <<][C+75  ][C@3+75 `" ][C@(0.0.255)+75 `\`'][C@3+75 `"][C+75  
][C@(0.0.255)+75 <<][C+75  sql][C@(0.0.255)+75 `[][C+75 B][C@(0.0.255)+75 `]][C+75  
][C@(0.0.255)+75 <<][C+75  ][C@3+75 `"][C@(0.0.255)+75 `\`'`\n][C@3+75 `"][C+75 ;]&]
[s0;l320; [C+75 `}]&]
[s5;%% &]
[s3;:5:%% 5. Using schema file to define SqlId constants&]
[s5;%% As names of columns are present in the database schema, it 
is natural to recycle them to create SqlId constants.&]
[s5;%% However, due to C`+`+ one definition rule (.sch files are 
interpreted as C`+`+ sources, using changing set of macros), 
you have to mark identifiers using underscore:&]
[s5;%% [/ MyApp.sch:]&]
[s5;/%% &]
[s0;l320; [*C@(0.0.255)+75 TABLE][C@(0.0.255)+75 `_][C+75 (TEST)]&]
[s0;l320; [C+75     ][*C@(0.0.255)+75 INT][C@(0.0.255)+75 `_][C+75       
  (A)]&]
[s0;l320; [C+75     ][*C@(0.0.255)+75 STRING][C@(0.0.255)+75 `_][C+75    
  (B, ][C@3+75 200][C+75 )]&]
[s0;l320; [*C@(0.0.255)+75 END][C@(0.0.255)+75 `_][*C@(0.0.255)+75 TABLE]&]
[s0;l320;C+75 &]
[s0;l320; [*C@(0.0.255)+75 TABLE][C@(0.0.255)+75 `_][C+75 (TEST2)]&]
[s0;l320; [C+75     ][*C@(0.0.255)+75 INT][C+75         (A)]&]
[s0;l320; [C+75     ][*C@(0.0.255)+75 STRING][C+75      (B, ][C@3+75 200][C+75 )]&]
[s0;l320; [*C@(0.0.255)+75 END][C@(0.0.255)+75 `_][*C@(0.0.255)+75 TABLE]&]
[s5;%% &]
[s5;%% MyApp.h:&]
[s5;%% &]
[s0;l320; [C@(128.0.255)+75 #ifndef][C+75  `_MyApp`_h`_]&]
[s0;l320; [C@(128.0.255)+75 #define][C+75  `_MyApp`_h`_]&]
[s0;l320;C+75 &]
[s0;l320; [C@(128.0.255)+75 #include][C+75  ][C@(0.0.255)+75 <][C+75 Core][C@(0.0.255)+75 /][C+75 C
ore][C@(0.0.255)+75 .][C+75 h][C@(0.0.255)+75 >]&]
[s0;l320; [C@(128.0.255)+75 #include][C+75  ][C@(0.0.255)+75 <][C+75 plugin][C@(0.0.255)+75 /][C+75 s
qlite3][C@(0.0.255)+75 /][C+75 Sqlite3][C@(0.0.255)+75 .][C+75 h][C@(0.0.255)+75 >]&]
[s0;l320;C+75 &]
[s0;l320; [*C@(0.0.255)+75 using][C+75  ][*C@(0.0.255)+75 namespace][C+75  
Upp;]&]
[s0;l320;C+75 &]
[s0;l320; [C@(128.0.255)+75 #define][C+75  SCHEMADIALECT ][C@(0.0.255)+75 <][C+75 plugin][C@(0.0.255)+75 /
][C+75 sqlite3][C@(0.0.255)+75 /][C+75 Sqlite3Schema][C@(0.0.255)+75 .][C+75 h][C@(0.0.255)+75 >
]&]
[s0;l320; [C@(128.0.255)+75 #define][C+75  MODEL ][C@(0.0.255)+75 <][C+75 Sql05][C@(0.0.255)+75 /
][C+75 MyApp][C@(0.0.255)+75 .][C+75 sch][C@(0.0.255)+75 >]&]
[s0;l320; [C@(128.0.255)+75 #include][C+75  ][C@3+75 `"Sql/sch`_header.h`"]&]
[s0;l320;C+75 &]
[s0;l320; [C@(128.0.255)+75 #endif]&]
[s5;%% &]
[s5;%% main.cpp:&]
[s5;%% &]
[s0;l320; [C@(128.0.255)+75 #include][C+75  ][C@3+75 `"MyApp.h`"]&]
[s0;l320;C+75 &]
[s0;l320; [C@(128.0.255)+75 #include][C+75  ][C@(0.0.255)+75 <][C+75 Sql][C@(0.0.255)+75 /][C+75 s
ch`_schema][C@(0.0.255)+75 .][C+75 h][C@(0.0.255)+75 >]&]
[s0;l320; [C@(128.0.255)+75 #include][C+75  ][C@(0.0.255)+75 <][C+75 Sql][C@(0.0.255)+75 /][C+75 s
ch`_source][C@(0.0.255)+75 .][C+75 h][C@(0.0.255)+75 >]&]
[s0;l320;C+75 &]
[s0;l320; [C+75 CONSOLE`_APP`_MAIN]&]
[s0;l320; [C+75 `{]&]
[s0;l320; [C+75     Sqlite3Session sqlite3;]&]
[s0;l320; [C+75     ][*C@(0.0.255)+75 if][C+75 (][C@(0.0.255)+75 !][C+75 sqlite3][C@(0.0.255)+75 .
][C+75 Open(ConfigFile(][C@3+75 `"simple.db`"][C+75 ))) `{]&]
[s0;l320; [C+75         Cout() ][C@(0.0.255)+75 <<][C+75  ][C@3+75 `"Can`'t 
create or open database file][C@(0.0.255)+75 `\n][C@3+75 `"][C+75 ;]&]
[s0;l320; [C+75         ][*_C@(128.0.255)+75 return][C+75 ;]&]
[s0;l320; [C+75     `}]&]
[s0;l320; [C+75     ]&]
[s0;l320; [C@(128.0.255)+75 #ifdef][C+75  `_DEBUG]&]
[s0;l320; [C+75     sqlite3][C@(0.0.255)+75 .][C+75 SetTrace();]&]
[s0;l320; [C@(128.0.255)+75 #endif]&]
[s0;l320;C+75 &]
[s0;l320; [C+75     SQL ][C@(0.0.255)+75 `=][C+75  sqlite3;]&]
[s0;l320;C+75 &]
[s0;l320; [C+75     SqlSchema sch(SQLITE3);]&]
[s0;l320; [C+75     All`_Tables(sch);]&]
[s0;l320; [C+75     SqlPerformScript(sch][C@(0.0.255)+75 .][C+75 Upgrade());]&]
[s0;l320; [C+75     SqlPerformScript(sch][C@(0.0.255)+75 .][C+75 Attributes());]&]
[s0;l320; [C+75     SQL][C@(0.0.255)+75 .][C+75 ClearError();]&]
[s0;l320;C+75 &]
[s0;l320; [C+75     ][*C@(0.0.255)+75 for][C+75 (][*C@(0.0.255)+75 int][C+75  
i ][C@(0.0.255)+75 `=][C+75  ][C@3+75 0][C+75 ; i ][C@(0.0.255)+75 <][C+75  
][C@3+75 10][C+75 ; i][C@(0.0.255)+75 `+`+][C+75 )]&]
[s0;l320; [C+75         SQL ][C@(0.0.255)+75 `*][C+75  Insert(TEST)(A, 
i)(B, AsString(][C@3+75 3][C+75  ][C@(0.0.255)+75 `*][C+75  i));]&]
[s0;l320;C+75 &]
[s0;l320; [C+75     Sql sql;]&]
[s0;l320; [C+75     sql ][C@(0.0.255)+75 `*][C+75  Select(A, B)][C@(0.0.255)+75 .][C+75 From(TE
ST);]&]
[s0;l320; [C+75     ][*C@(0.0.255)+75 while][C+75 (sql][C@(0.0.255)+75 .][C+75 Fetch())]&]
[s0;l320; [C+75         Cout() ][C@(0.0.255)+75 <<][C+75  sql][C@(0.0.255)+75 `[][C+75 A][C@(0.0.255)+75 `]
][C+75  ][C@(0.0.255)+75 <<][C+75  ][C@3+75 `" ][C@(0.0.255)+75 `\`'][C@3+75 `"][C+75  
][C@(0.0.255)+75 <<][C+75  sql][C@(0.0.255)+75 `[][C+75 B][C@(0.0.255)+75 `]][C+75  
][C@(0.0.255)+75 <<][C+75  ][C@3+75 `"][C@(0.0.255)+75 `\`'`\n][C@3+75 `"][C+75 ;]&]
[s0;l320; [C+75 `}]&]
[s5;%% &]
[s3;:6:%% 6. Using structures defined by schema files&]
[s5;%% Schema files also define structures that can be used to fetch, 
insert or update database records. Names of such structures are 
identical to the names of tables, with [* S`_] prefix:&]
[s5;%% &]
[s0;l320; [C@(128.0.255)+75 #include][C+75  ][C@3+75 `"MyApp.h`"]&]
[s0;l320;C+75 &]
[s0;l320; [C@(128.0.255)+75 #include][C+75  ][C@(0.0.255)+75 <][C+75 Sql][C@(0.0.255)+75 /][C+75 s
ch`_schema][C@(0.0.255)+75 .][C+75 h][C@(0.0.255)+75 >]&]
[s0;l320; [C@(128.0.255)+75 #include][C+75  ][C@(0.0.255)+75 <][C+75 Sql][C@(0.0.255)+75 /][C+75 s
ch`_source][C@(0.0.255)+75 .][C+75 h][C@(0.0.255)+75 >]&]
[s0;l320;C+75 &]
[s0;l320; [C+75 CONSOLE`_APP`_MAIN]&]
[s0;l320; [C+75 `{]&]
[s0;l320; [C+75     Sqlite3Session sqlite3;]&]
[s0;l320; [C+75     ][*C@(0.0.255)+75 if][C+75 (][C@(0.0.255)+75 !][C+75 sqlite3][C@(0.0.255)+75 .
][C+75 Open(ConfigFile(][C@3+75 `"simple.db`"][C+75 ))) `{]&]
[s0;l320; [C+75         Cout() ][C@(0.0.255)+75 <<][C+75  ][C@3+75 `"Can`'t 
create or open database file][C@(0.0.255)+75 `\n][C@3+75 `"][C+75 ;]&]
[s0;l320; [C+75         ][*_C@(128.0.255)+75 return][C+75 ;]&]
[s0;l320; [C+75     `}]&]
[s0;l320; [C+75     ]&]
[s0;l320; [C@(128.0.255)+75 #ifdef][C+75  `_DEBUG]&]
[s0;l320; [C+75     sqlite3][C@(0.0.255)+75 .][C+75 SetTrace();]&]
[s0;l320; [C@(128.0.255)+75 #endif]&]
[s0;l320;C+75 &]
[s0;l320; [C+75     SQL ][C@(0.0.255)+75 `=][C+75  sqlite3;]&]
[s0;l320;C+75 &]
[s0;l320; [C+75     SqlSchema sch(SQLITE3);]&]
[s0;l320; [C+75     All`_Tables(sch);]&]
[s0;l320; [C+75     SqlPerformScript(sch][C@(0.0.255)+75 .][C+75 Upgrade());]&]
[s0;l320; [C+75     SqlPerformScript(sch][C@(0.0.255)+75 .][C+75 Attributes());]&]
[s0;l320; [C+75     SQL][C@(0.0.255)+75 .][C+75 ClearError();]&]
[s0;l320;C+75 &]
[s0;l320; [C+75     ][*_C+75 S`_TEST x;]&]
[s0;l320; [C+75     ][*C@(0.0.255)+75 for][C+75 (][*C@(0.0.255)+75 int][C+75  
i ][C@(0.0.255)+75 `=][C+75  ][C@3+75 0][C+75 ; i ][C@(0.0.255)+75 <][C+75  
][C@3+75 10][C+75 ; i][C@(0.0.255)+75 `+`+][C+75 ) `{]&]
[s0;l320; [C+75         ][*_C+75 x][*_C@(0.0.255)+75 .][*_C+75 A ][*_C@(0.0.255)+75 `=][*_C+75  
i;]&]
[s0;l320; [C+75         ][*_C+75 x][*_C@(0.0.255)+75 .][*_C+75 B ][*_C@(0.0.255)+75 `=][*_C+75  
AsString(][*_C@3+75 3][*_C+75  ][*_C@(0.0.255)+75 `*][*_C+75  i);]&]
[s0;l320; [C+75         ][*_C+75 SQL ][*_C@(0.0.255)+75 `*][*_C+75  Insert(x);]&]
[s0;l320; [C+75     `}]&]
[s0;l320;C+75 &]
[s0;l320; [C+75     Sql sql;]&]
[s0;l320; [C+75     sql ][C@(0.0.255)+75 `*][C+75  Select(][*_C+75 x][C+75 )][C@(0.0.255)+75 .][C+75 F
rom(TEST);]&]
[s0;l320; [C+75     ][*C@(0.0.255)+75 while][C+75 (sql][C@(0.0.255)+75 .][C+75 Fetch(][*_C+75 x][C+75 )
)]&]
[s0;l320; [C+75         Cout() ][C@(0.0.255)+75 <<][C+75  ][*_C+75 x][*_C@(0.0.255)+75 .][*_C+75 A
][C+75  ][C@(0.0.255)+75 <<][C+75  ][C@3+75 `" ][C@(0.0.255)+75 `\`'][C@3+75 `"][C+75  
][C@(0.0.255)+75 <<][C+75  ][*_C+75 x][*_C@(0.0.255)+75 .][*_C+75 B][C+75  
][C@(0.0.255)+75 <<][C+75  ][C@3+75 `"][C@(0.0.255)+75 `\`'`\n][C@3+75 `"][C+75 ;]&]
[s0;l320; [C+75 `}]&]
[s5;l320;%% &]
[s3;%% Recommended tutorials:&]
[s5;%% If you want to learn more, we have several tutorials that 
you can find useful:&]
[s5;l160;i150;O0;~~~0;%% [^topic`:`/`/Core`/srcdoc`/Tutorial`_en`-us^ U`+`+ 
Core value types] `- still not very confident with U`+`+. In 
this tutorial you will learn basics.]]