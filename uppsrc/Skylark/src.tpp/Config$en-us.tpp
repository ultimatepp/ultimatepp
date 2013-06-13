topic "Skylark configuration";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 Skylark configuration]]}}&]
[s1;@(0.0.255)3 &]
[ {{10000@1 [s0; [*3 SkylarkSessionConfig]]}}&]
[s0; &]
[s1;:SkylarkSessionConfig`:`:struct: [@(0.0.255)3 struct][3 _][*3 SkylarkSessionConfig]&]
[s2;%% Structure that groups session configuration parameters.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Member List]]}}&]
[s3; &]
[s5;:SkylarkSessionConfig`:`:cookie: [_^String^ String]_[* cookie]&]
[s2;%% The name of session cookie. Default value is loaded from `"session`_cookie`" 
.ini parameter, which defaults to `"`_`_skylark`_session`_cookie`_`_`".&]
[s3; &]
[s4; &]
[s5;:SkylarkSessionConfig`:`:dir: [_^String^ String]_[* dir]&]
[s2;%% Directory used to store Skylark sessions. Skylark sessions 
are stored either in SQL table or in this directory. If [%-* table] 
parameter is not empty, SQL table is used, otherwise sessions 
are stored in [%-* dir]. Default value is loaded from `"session`_dir`" 
.ini parameter, which defaults to ConfigFile(`"session`").&]
[s3; &]
[s4; &]
[s5;:SkylarkSessionConfig`:`:format: [@(0.0.255) int]_[* format]&]
[s2;%% Format used to store sessions. Can be `"JSON`", `"XML`" or 
`"BINARY`". Default value is loaded from `"session`_format`" 
.ini parameter, which defaults to `"BINARY`".&]
[s3; &]
[s4; &]
[s5;:SkylarkSessionConfig`:`:data`_column: [_^SqlId^ SqlId]_[* data`_column]&]
[s2;%% SQL table column used to store session. It has to be of `"CLOB`" 
format (able to store text of any length). Default value is loaded 
from `"session`_data`_column`" .ini parameter, which defaults 
to `"DATA`".&]
[s3; &]
[s4; &]
[s5;:SkylarkSessionConfig`:`:id`_column: [_^SqlId^ SqlId]_[* id`_column]&]
[s2;%% SQL table column used as session ID primary key, which is 
UUID based String. Columns should be able to store at least 32 
characters. Default value is loaded from `"session`_id`_column`" 
.ini parameter, which defaults to `"ID`".&]
[s3; &]
[s4; &]
[s5;:SkylarkSessionConfig`:`:lastwrite`_column: [_^SqlId^ SqlId]_[* lastwrite`_column]&]
[s2;%% SQL table column used to store last`-write time of session. 
Column must be able to store Time type. Default value is loaded 
from `"session`_lastwrite`_column`" .ini parameter, which defaults 
to `"LASTWRITE`".&]
[s3; &]
[s4; &]
[s5;:SkylarkSessionConfig`:`:table: [_^SqlId^ SqlId]_[* table]&]
[s2;%% The name of SQL table used to store sessions. If not empty, 
sessions are stored into SQL database rather than to filesystem. 
Default value is loaded from `"session`_table`" .ini parameter, 
which defaults to empty string (and thus default is to store 
sessions to filesystem).&]
[s3; &]
[s4; &]
[s5;:SkylarkSessionConfig`:`:expire: [@(0.0.255) int]_[* expire]&]
[s2;%% Number of seconds after which Skylark session expires and 
can be deleted. Default value is loaded from `"session`_expire`" 
.ini parameter, which defaults to 3600 `* 24 `* 365, one year.&]
[s2;%% &]
[s1;@(0.0.255)3 &]
[ {{10000@1 [s0; [*3 SkylarkConfig]]}}&]
[s3;%% &]
[s1;:SkylarkConfig`:`:struct: [@(0.0.255)3 struct][3 _][*3 SkylarkConfig]&]
[s2;%% Structure groups Skylark configuration parameters.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Member List]]}}&]
[s3; &]
[s5;:SkylarkConfig`:`:root: [_^String^ String]_[* root]&]
[s2;%% Application root URL directory.&]
[s3; &]
[s4; &]
[s5;:SkylarkConfig`:`:path: [_^String^ String]_[* path]&]
[s2;%% Set of directories, separated by `';`', used when searching 
for static files, e.g. witz templates. Default value is loaded 
from `"path`" .ini parameter, which defaults to `"UPP`_ASSEMBLY`_`_`" 
environment variable which is set by theide when starting application 
to current assembly `- that way in debug mode, static files can 
be referred the same way as include files (of other packages) 
`- `"package`_name/filename.witz`".&]
[s3; &]
[s4; &]
[s5;:SkylarkConfig`:`:session: [_^SkylarkSessionConfig^ SkylarkSessionConfig]_[* session]&]
[s2;%% Skylark session parameters. See above...&]
[s3; &]
[s4; &]
[s5;:SkylarkConfig`:`:threads: [@(0.0.255) int]_[* threads]&]
[s2;%% Number of handler threads in each Skylark process. Traditionally, 
the advantage of threads over spawning more processes is that 
threads can share (cached) data, disadvantage is that single 
thread crashing takes down the whole process (and thus possibly 
several requests being processed). Default value is loaded from 
`"threads`" .ini parameter, which defaults to 3 `* number`_of`_logical`_CPU 
cores `+ 1.&]
[s3; &]
[s4; &]
[s5;:SkylarkConfig`:`:port: [@(0.0.255) int]_[* port]&]
[s2;%% TCP/IP port of Skylark application. Skylark automatically 
detects SCGI and plain HTTP requests and replies accordingly. 
Default value is loaded from `"port`" .ini parameter, which defaults 
to 8001.&]
[s3; &]
[s4; &]
[s5;:SkylarkConfig`:`:ip: [_^String^ String]_[* ip]&]
[s2;%% Allows to specify the IP address Skylark listens on. Default 
is empty which makes Skylark listen on the default port.&]
[s3; &]
[s4; &]
[s5;:SkylarkConfig`:`:prefork: [@(0.0.255) int]_[* prefork]&]
[s2;%% Number of processes spawned to handle requests. Master process 
maintains this number of running processes, spawning additional 
ones in case that child process terminate or abort. Zero means 
that main process handles HTTP requests directly. Default value 
is loaded from `"prefork`" .ini parameter, which defaults to 
1 in release mode (so that main process acts basically as watchdog) 
and 0 in debug mode (it is easier to debug just one process).&]
[s3; &]
[s4; &]
[s5;:SkylarkConfig`:`:timeout: [@(0.0.255) int]_[* timeout]&]
[s2;%% Maximum processing time in seconds of single HTTP request 
before aborting the handler process. Zero means that there is 
no timeout.  Default value is loaded from `"timeout`" .ini parameter, 
which defaults to 300 seconds in release and zero (no timeout) 
in debug mode.&]
[s3; &]
[s4; &]
[s5;:SkylarkConfig`:`:use`_caching: [@(0.0.255) bool]_[* use`_caching]&]
[s2;%% If true, various things are cached, most importantly compiled 
witz templates. Default value is loaded from `"use`_caching`" 
.ini parameter, which defaults to true in release mode and false 
in debug mode.&]
[s0;%% ]]