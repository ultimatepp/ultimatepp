topic "SqlSession";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 SqlSession]]}}&]
[s3; &]
[s1;:SqlSession`:`:class: [@(0.0.255)3 class][3 _][*3 SqlSession][3 _:_][@(0.0.255)3 public][3 _][*@3;3 S
qlSource]&]
[s2;%% SqlSesion represents single database session. SqlSession has 
ability to start (Begin) and finish (Commit or Rollback) transactions; 
when no transaction is started, each SQL statement is treated 
as single transaction. SqlSession is used as constructor parameter 
for Sql context. As most application usually work with single 
default session, or alternatively read/write sessions, this is 
reflected by existence of default sessions represented by global 
pseodovariables SQL and SQLR.&]
[s2;%% SqlSesion serves as base class for database specific session 
classes, which provide their unique methods for connection and 
other tasks.&]
[s0;i448;a25;kKO9;:noref:@(0.0.255) &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3;%% &]
[s5;:SqlSession`:`:Begin`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* Begin]()&]
[s2;%% Starts a transaction.&]
[s3; &]
[s4; &]
[s5;:SqlSession`:`:Commit`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* Commit]()&]
[s2;%% Commits transaction.&]
[s3; &]
[s4; &]
[s5;:SqlSession`:`:Rollback`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* Rollback]()&]
[s2;%% Rollbacks transaction.&]
[s3; &]
[s4; &]
[s5;:SqlSession`:`:GetTransactionLevel`(`)const: [@(0.0.255) virtual] 
[@(0.0.255) int]_[* GetTransactionLevel]()_[@(0.0.255) const]&]
[s2;%% Returns 0 if there is no transaction or 1 if transaciton is 
active. (Originally, U`+`+ allowed nested transaction, this is 
now deprecated).&]
[s3;%% &]
[s4;%% &]
[s5;:SqlSession`:`:IsOpen`(`)const: [@(0.0.255) virtual] [@(0.0.255) bool]_[* IsOpen]()_[@(0.0.255) c
onst]&]
[s2;%% Returns true if session is connected and ready to recieve 
statements.&]
[s3;%% &]
[s4;%% &]
[s5;:SqlSession`:`:EnumUsers`(`): [@(0.0.255) virtual] [_^Vector^ Vector]<[_^String^ String
]>_[* EnumUsers]()&]
[s2;%% Enumerates users of database. Exact function depends on database.&]
[s3;%% &]
[s4;%% &]
[s5;:SqlSession`:`:EnumDatabases`(`): [@(0.0.255) virtual] [_^Vector^ Vector]<[_^String^ St
ring]>_[* EnumDatabases]()&]
[s2;%% Enumaretes all databases. Exact function depends on database.&]
[s3;%% &]
[s4;%% &]
[s5;:SqlSession`:`:EnumTables`(String`): [@(0.0.255) virtual] [_^Vector^ Vector]<[_^String^ S
tring]>_[* EnumTables]([_^String^ String]_[*@3 database])&]
[s2;%% Enumerates all tables of [%-*@3 database]. Exact function depends 
on database.&]
[s3;%% &]
[s4;%% &]
[s5;:SqlSession`:`:EnumViews`(String`): [@(0.0.255) virtual] [_^Vector^ Vector]<[_^String^ S
tring]>_[* EnumViews]([_^String^ String]_[*@3 database])&]
[s2;%% Enumerates all views of [%-*@3 database]. Exact function depends 
on database.&]
[s3;%% &]
[s4;%% &]
[s5;:SqlSession`:`:EnumSequences`(String`): [@(0.0.255) virtual] [_^Vector^ Vector]<[_^String^ S
tring]>_[* EnumSequences]([_^String^ String]_[*@3 database])&]
[s2;%% Enumerates all sequences of [%-*@3 database]. Exact function 
depends on database.&]
[s3;%% &]
[s4;%% &]
[s5;:SqlSession`:`:EnumColumns`(String`,String`): [@(0.0.255) virtual] 
[_^Vector^ Vector]<[_^SqlColumnInfo^ SqlColumnInfo]>_[* EnumColumns]([_^String^ String]_[*@3 d
atabase], [_^String^ String]_[*@3 table])&]
[s2;%% Enumerates all columns of [%-*@3 database] [%-*@3 table]. Exact 
function depends on database.&]
[s3;%% &]
[s4;%% &]
[s5;:SqlSession`:`:EnumPrimaryKey`(String`,String`): [@(0.0.255) virtual] 
[_^Vector^ Vector]<[_^String^ String]>_[* EnumPrimaryKey]([_^String^ String]_[*@3 database],
 [_^String^ String]_[*@3 table])&]
[s2;%% Enumerates all primary keys of [%-*@3 database] [%-*@3 table]. 
Exact function depends on database.&]
[s3;%% &]
[s4;%% &]
[s5;:SqlSession`:`:GetDialect`(`)const: [@(0.0.255) int]_[* GetDialect]()_[@(0.0.255) const
]&]
[s2;%% Returns SQL dialect of session (ORACLE, SQLITE3, MY`_SQL, 
MSSQL, PGSQL).&]
[s3;%% &]
[s4;%% &]
[s5;:SqlSession`:`:SetTrace`(Stream`&`): [@(0.0.255) void]_[* SetTrace]([_^Stream^ Stream][@(0.0.255) `&
]_[*@3 s]_`=_VppLog())&]
[s2;%% Activates tracing of SQL statements issued on this session 
and SQL errors to the stream [%-*@3 s].&]
[s3;%% &]
[s4;%% &]
[s5;:SqlSession`:`:GetTrace`(`)const: [_^Stream^ Stream]_`*[* GetTrace]()_[@(0.0.255) const
]&]
[s2;%% Returns current trace stream or NULL if not active.&]
[s3;%% &]
[s4;%% &]
[s5;:SqlSession`:`:KillTrace`(`): [@(0.0.255) void]_[* KillTrace]()&]
[s2;%% Deactivates tracing.&]
[s3;%% &]
[s4;%% &]
[s5;:SqlSession`:`:LogErrors`(Stream`&`): [@(0.0.255) void]_[* LogErrors]([_^Stream^ Stream
][@(0.0.255) `&]_[*@3 s]_`=_VppLog())&]
[s2;%% Activates tracing SQL errors only to the stream [%-*@3 s].&]
[s3;%% &]
[s4;%% &]
[s5;:SqlSession`:`:LogErrors`(bool`): [@(0.0.255) void]_[* LogErrors]([@(0.0.255) bool]_[*@3 b
])&]
[s2;%% If [%-*@3 b] is true, same as LogErrors(), if false, deactivates 
tracing of SQL errors.&]
[s3;%% &]
[s4;%% &]
[s5;:SqlSession`:`:TraceTime`(bool`): [@(0.0.255) void]_[* TraceTime]([@(0.0.255) bool]_[*@3 b
]_`=_[@(0.0.255) true])&]
[s2;%% Adds execution times to commands traced.&]
[s3;%% &]
[s4;%% &]
[s5;:SqlSession`:`:IsTraceTime`(`)const: [@(0.0.255) bool]_[* IsTraceTime]()_[@(0.0.255) co
nst]&]
[s2;%% Returns TraceTime mode status.&]
[s3;%% &]
[s4;%% &]
[s5;:SqlSession`:`:TraceSlow`(int`): [_^SqlSession^ SqlSession][@(0.0.255) `&]_[* TraceSlow
]([@(0.0.255) int]_[*@3 ms]_`=_[@3 5000])&]
[s2;%% Activates tracing SQL statements longer than [%-*@3 ms] as errors.&]
[s3;%% &]
[s4;%% &]
[s5;:SqlSession`:`:ThrowOnError`(bool`): [_^SqlSession^ SqlSession][@(0.0.255) `&]_[* Throw
OnError]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% If this mode is active, SqlExc is throwed each time when SQL 
error happens.&]
[s3;%% &]
[s4;%% &]
[s5;:SqlSession`:`:IsThrowOnError`(`)const: [@(0.0.255) bool]_[* IsThrowOnError]()_[@(0.0.255) c
onst]&]
[s2;%% Returns true if ThrowOnError is active.&]
[s3;%% &]
[s4;%% &]
[s5;:SqlSession`:`:WasError`(`)const: [@(0.0.255) bool]_[* WasError]()_[@(0.0.255) const]&]
[s2;%% Returns true if there was error since the start of session 
or since ClearError.&]
[s3;%% &]
[s4;%% &]
[s5;:SqlSession`:`:SetError`(String`,String`,int`,const char`*`,ERRORCLASS`): [@(0.0.255) v
oid]_[* SetError]([_^String^ String]_[*@3 error], [_^String^ String]_[*@3 stmt], 
[@(0.0.255) int]_[*@3 code]_`=_[@3 0], [@(0.0.255) const]_[@(0.0.255) char]_`*_[*@3 scode]_`=
_NULL, Sql`::ERRORCLASS_[*@3 clss]_`=_Sql`::ERROR`_UNSPECIFIED)&]
[s2;%% Externally sets session to error state.&]
[s3;%% &]
[s4;%% &]
[s5;:SqlSession`:`:GetLastError`(`)const: [_^String^ String]_[* GetLastError]()_[@(0.0.255) c
onst]&]
[s2;%% Returns the description of last error.&]
[s3;%% &]
[s4;%% &]
[s5;:SqlSession`:`:GetErrorStatement`(`)const: [_^String^ String]_[* GetErrorStatement]()
_[@(0.0.255) const]&]
[s2;%% Returns the statement that caused the last error.&]
[s3;%% &]
[s4;%% &]
[s5;:SqlSession`:`:GetErrorCode`(`)const: [@(0.0.255) int]_[* GetErrorCode]()_[@(0.0.255) c
onst]&]
[s2;%% Returns the error`-code of last error.&]
[s3;%% &]
[s4;%% &]
[s5;:SqlSession`:`:GetErrorCodeString`(`)const: [_^String^ String]_[* GetErrorCodeString](
)_[@(0.0.255) const]&]
[s2;%% Returns the error`-code text.&]
[s3;%% &]
[s4;%% &]
[s5;:SqlSession`:`:GetErrorClass`(`)const: Sql`::ERRORCLASS_[* GetErrorClass]()_[@(0.0.255) c
onst]&]
[s2;%% Returns the class of error. Can be ERROR`_UNSPECIFIED or CONNECTION`_BROKEN.&]
[s3;%% &]
[s4;%% &]
[s5;:SqlSession`:`:ClearError`(`): [@(0.0.255) void]_[* ClearError]()&]
[s2;%% Clears the last error. This should be called as part of error 
handling.&]
[s3;%% &]
[s4;%% &]
[s5;:SqlSession`:`:InstallErrorHandler`(bool`(`*`)`(String error`,String stmt`,int code`,const char`*scode`,ERRORCLASS clss`)`): [@(0.0.255) v
oid]_[* InstallErrorHandler]([@(0.0.255) bool]_(`*[*@3 handler])(String_error, 
String_stmt, [@(0.0.255) int]_code, [@(0.0.255) const]_[@(0.0.255) char]_`*scode, 
Sql`::ERRORCLASS_clss))&]
[s2;%% Installs alternate error handler which is invoked on error.&]
[s3;%% &]
[s4;%% &]
[s5;:SqlSession`:`:operator bool`(`)const: [* operator_bool]()_[@(0.0.255) const]&]
[s2;%% Same as IsOpen().&]
[s3;%% &]
[s4;%% &]
[s5;:SqlSession`:`:PerThread`(bool`): [@(0.0.255) static] [@(0.0.255) void]_[* PerThread]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% In multithreded mode, activates mode where each thread has 
assigned unique per`-thread default session (otherwise default 
session is shared).&]
[s3;%% &]
[s0;%% ]]