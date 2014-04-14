topic "Basic Use and Description";
[2 $$0,0#00000000000000000000000000000000:Default]
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[b83;*4 $$3,5#07864147445237544204411237157677:title]
[i288;O9;C2 $$4,6#40027414424643823182269349404212:item]
[l25;b25;a25;2 $$5,5#45413000475342174754091244180557:text]
[l288;b17;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;t246;C@5;1 $$7,7#20902679421464641399138805415013:code]
[b2503;2 $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base]
[t4167;C2 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam]
[b167;C2 $$13,13#92430459443460461911108080531343:item1]
[i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128)2 $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7;2 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line]
[l160;t4167;*C+117 $$18,5#88603949442205825958800053222425:package`-title]
[2 $$19,0#53580023442335529039900623488521:gap]
[t4167;C2 $$20,20#70211524482531209251820423858195:class`-nested]
[b50;2 $$21,21#03324558446220344731010354752573:Par]
[b83;_R+117 $$22,5#50327538467692101372348457405326:heading1]
[b50;a25;*2 $$23,24#10189570498413014020290115411120:heading2]
[l25;b25;a25;R2 $$24,24#24747716437513346533020414014942:text2]
[l25;b25;a25;*/+75 $$25,25#14962814463678751281510843401240:code`_header]
[{_}%EN-US 
[s3; U`+`+ SQL&]
[s22; Basic Use and Description&]
[s24; For this section, the example used will be oriented to PostgreSQL 
use. See the SQL example packages provided in the Upp examples 
for using MySQL and SQLite as well.&]
[s23; The Schema description file (.sch file)&]
[s24; In each schema description file, you describe the table and 
column layout of your database. &]
[s25; Postgresql Example (`"person`_db.sch`"):&]
[s7; TABLE`_ (PERSON)&]
[s7; -|SERIAL`_ (PERSON`_ID) PRIMARY`_KEY&]
[s7; -|STRING`_ (NAME, 25)&]
[s7; -|DATE`_ -|(BIRTH`_DATE)&]
[s7; -|INT`_ -|(NUM`_CHILDREN)&]
[s7; -|DATE`_ -|(DATE`_ADDED) SQLDEFAULT(CURRENT`_DATE)&]
[s7; END`_TABLE&]
[s7; &]
[s7; TABLE`_ (EMPLOYEE)&]
[s7; -|SERIAL`_ (EMPLOYEE`_ID) PRIMARY`_KEY&]
[s7; -|STRING`_ (DEPARTMENT, 50)&]
[s7; -|STRING`_ (LOCATION, 50)&]
[s7; -|DATE`_ (START`_DATE)&]
[s7; -|BOOL`_ -|(IS`_SUPERVISOR)&]
[s7; -|TIME`_ -|(WORKDAY`_START)&]
[s7; -|TIME`_ -|(WORKDAY`_END)&]
[s7; -|INT64 (PERSON`_ID) REFERENCES(PERSON.PERSON`_ID)&]
[s7; END`_TABLE&]
[s24; In this schema, we have described a `'person`' table and an 
`'employee`' table, with the [_ foreign key] 1 to 1 relationship 
`"an employee is a person`". &]
[s24; The different types mentioned in this example map to SQL types. 
More information about types should be referenced by looking 
at the source code header files for the database type. In this 
example, all of the types referenced are found defined in the 
file `"PostgreSQLSchema.h`" from the `"PostgreSQL`" U`+`+ package.&]
[s24; Each type declaration has 2 variants; one with an underscore 
`"`_`" and one without. When an underscore is used, an [C SqlId] 
object is automatically created for use as a variable in your 
source files. When not used, you must manually define the [C SqlID] 
object in your source. Reference the [_^topic`:`/`/ACWarehouseIO`/sql`_info`/BasicUse`$en`-us`#SqlId Objects^ S
qlId objects] section below for further explanation.&]
[s24; Note: if you use a name more than once, you should use an underscore 
[*/ only the first time] you declare the name, otherwise you will 
get `"already defined`" compilation errors. This is shown in 
the above example where the column name `"PERSON`_ID`" is used 
twice; there is an underscore only the first time it is used.&]
[s23; Source Files (for PostgreSQL example)&]
[s25; Header file includes/defines (`"person.hpp`"):&]
[s7; #include <PostgreSQL/PostgreSQL.h>&]
[s7; #define SCHEMADIALECT <PostgreSQL/PostgreSQLSchema.h>&]
[s7; #define MODEL <MyPackage/person`_db.sch>&]
[s7; #include `"Sql/sch`_header.h`"&]
[s7; &]
[s25; Source file includes (`"person.cpp`"):&]
[s7; #include `"person.hpp`"&]
[s7; #include <Sql/sch`_schema.h>&]
[s7; #include <Sql/sch`_source.h>&]
[s7; &]
[s25; Session objects:&]
[s7; PostgreSQLSession m`_session;&]
[s24; The session object is used to control the connection and session 
information. Each database dialect will have its own session 
object to use.&]
[s25; Database connection using session:&]
[s7; bool good`_conn `= m`_session.Open(`"host`=localhost dbname`=persons 
user`=user1 password`=pass1`")&]
[s24; The [C Open()] function returns a true or false value depending 
on success of connecting to database.&]
[s25; [ISqlid objects; SqlId objects:]&]
[s24; SqlId objects aid the formation of sql statements by mapping 
database field/column names to local variables.&]
[s7; SqlId all(`"`*`");&]
[s7; SqlId person`_name(`"NAME`");&]
[s24; We will now be able to use `"all`" and `"person`_name`" in our 
SQL CRUD statements in our code.&]
[s24; As mentioned previously, all of the declarations in our schema 
file that end in an underscore will automatically be declared 
as SqlId variables we can access in our source code. &]
[s25; [1 Example use of SqlId variables:]&]
[s7; sql `* Insert(PERSON)(NAME, `"John Smith`") (BIRTH`_DATE, Date(1980,8,20)) 
(NUM`_CHILDREN, 1)&]
[s24; The variables [C PERSON, NAME, BIRTH`_DATE, NUM`_CHILDREN] were 
available to us even though we didn`'t define them in our source. 
We could have also used the variable [C person`_name ]instead of 
[C NAME ]as we defined it ourselves.&]
[s25; Sql objects&]
[s24; Sql objects are used for CRUD operations on the database; they 
operate on a session.&]
[s7; Sql sql(m`_session); //define Sql object to act on Session object 
m`_session.&]
[s25; Queries&]
[s25; [1 Select example:]&]
[s7; sql `* Select(all).From(PERSON).Where(person`_name `=`= `"John 
Smith`");&]
[s24; Note: Here we can use `"all`" because we defined it as an [C SqlId 
]variable above (same goes for `"person`_name`").&]
[s23; Exceptions vs Checking for errors.&]
[s24; There 2 ways to make sql statements.&]
[s24; 1. Manual error checking.&]
[s24; -|Manual error checking uses the [* asterisk (`"`*`") operator] 
when writing SQL statements.&]
[s7; sql `* Select(all).From(PERSON).Where(NAME `=`= `"John Smith`");&]
[s7; if(sql.IsError())`{&]
[s7; -|Cout() << m`_session.GetErrorCodeString() << `"`\n`";&]
[s7; `}&]
[s24; 2. Exception handling.&]
[s24; -|Specify exception handling by using the [* ampersand (`"`&`") 
operator] when writing SQL statements.&]
[s7; try`{&]
[s7; -|sql `& Select(all).From(PERSON).Where(NAME `=`= `"John Smith`");&]
[s7; `}catch(SqlExc`& err)`{&]
[s7; -|Cout() << err << `"`\n`";&]
[s7; -|// Or we can get the error from the session too...&]
[s7; -|Cout() << m`_session.GetErrorCodeString() << `"`\n`";&]
[s7; `}&]
[s24; `*Remember, SqlExc is a subclass of Exc, which is a subclass 
of String, so it can be used as a string to get its error. &]
[s24; &]
[s23; Getting Values from Sql Queries&]
[s24; The [C Fetch()] method will fetch the next row resulting from 
the query into the Sql object and return true. If there are no 
more rows to fetch, it will return false.&]
[s7; while(sql.Fetch())`{&]
[s7;     Cout() << Format(`"Row: %s %s %s `\n`", `\&]
[s7; -|-|-|    AsString(sql`[NAME`]), `\&]
[s7; -|-|-|    AsString(sql`[BIRTH`_DATE`]), `\&]
[s7; -|-|-|    AsString(sql`[NUM`_CHILDREN`]));&]
[s7; `}&]
[s24; You can reference each row by SqlId as above, or by integer 
array index (Ie. `"[C sql`[0`]]`").&]
[s24; Notice the use of AsString() here. sql`[id`] returns a U`+`+ 
Value type object. You can then convert that Value type to its 
appropriate type afterward.&]
[s0; ]]