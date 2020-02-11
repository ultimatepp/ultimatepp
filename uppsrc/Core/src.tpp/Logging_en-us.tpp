topic "U++ Logging";
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
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 U`+`+ Logging]]}}&]
[s0;i448;a25;kKO9;@(0.0.255)%- &]
[s0; U`+`+ logging infrastructure is based on output stream, which 
is capable of outputting log lines into decided output facilities 
(files, console output, syslog).&]
[s0; &]
[s0; Logging itself is then based on set o macros that output values 
to this stream, usually using AsString or operator<<(Stream`&, 
...) to convert values.&]
[s0; &]
[s0; This stream is accessible through UppLog() function and its 
default version is provided by U`+`+, however it can be replaced 
(using SetUppLog).&]
[s0; &]
[s0; Multithreading note: Standard log stream handles serialization 
of output in its implementation  (and any possible replacement 
should do that too), so it is possible to use single output stream 
in all threads.&]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Entity List]]}}&]
[s0;%- &]
[s5;:StdLogSetup`(dword`,const char`*`,int`):%- [@(0.0.255) void]_[* StdLogSetup]([_^dword^ d
word]_[*@3 options], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 filepath]_`=_NULL, 
[@(0.0.255) int]_[*@3 filesize`_limit]_`=_[@3 10]_`*_[@3 1024]_`*_[@3 1024])&]
[s2; This function setups standard U`+`+ logging. [%-*@3 filepath] 
is the path of .log file (if logging to file is active), [%-*@3 filesize`_limit] 
is maximum size of log (if LOG`_FILE options is active), if achieved, 
a new log file is created. [%-*@3 options] is a combination of 
bit flags:&]
[s2; &]
[ {{2939:7061<288;^ [s0; LOG`_FILE ]
:: [s0; Output log to file (this is default). The default path of file 
is ConfigFile(`"[/ program`_name].log`").]
:: [s0; LOG`_COUT ]
:: [s0; Output log to standard output, using plain putchar.]
:: [s0; LOG`_CERR ]
:: [s0; Output log to error output, using plain putc.]
:: [s0; LOG`_DBG ]
:: [s0; Output log to debugger (Win32 specific).]
:: [s0; LOG`_SYS ]
:: [s0; Output log to syslog (POSIX specific).]
:: [s0; LOG`_ELAPSED]
:: [s0; Prepend a number of miliseconds to each log line since the previous 
log line.]
:: [s0; LOG`_TIMESTAMP ]
:: [s0; Prepend local time timestamp to each log line (not with LOG`_SYS, 
as syslog already does this).]
:: [s0; LOG`_TIMESTAMP`_UTC]
:: [s0; Prepend universal time timestamp to each log line (not with 
LOG`_SYS).]
:: [s0; LOG`_APPEND ]
:: [s0; When starting a program, append log to existing file instead 
of replacing it.]
:: [s0; LOG`_ROTATE([*@3 x])]
:: [s0; When starting a new log file, up to [*@3 x] older logs is preserved 
(renamed with extension `'.1`', `'.2`' etc...).]
:: [s0; LOG`_ROTATE`_GZIP]
:: [s0; Older preserved log files are compressed using gzip (except 
the most recent log `'.1`'.]
:: [s0; LOG`_COUTW]
:: [s0; Output log to standard output, using Cout. This provides eventual 
conversion of UTF8 characters, at the price of using heap (so 
cannot be used to e.g. LOG inside heap routines).]
:: [s0; LOG`_CERRW]
:: [s0; Output log to standard output, using Cerr. This provides eventual 
conversion of UTF8 characters, at the price of using heap (so 
cannot be used to e.g. LOG inside heap routines).]}}&]
[s3; &]
[s4;%- &]
[s5;:StdLog`(`):%- [_^Stream^ Stream][@(0.0.255) `&]_[* StdLog]()&]
[s2; Returns a reference to standard log stream.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:GetStdLogPath`(`):%- [_^Upp`:`:String^ String]_[* GetStdLogPath]()&]
[s2; Returns the path of current log, if any.&]
[s3;%- &]
[s4;%- &]
[s5;:LOG`_BEGIN:%- [@(0.0.255) const]_[@(0.0.255) char]_[* LOG`_BEGIN `= 
`'`\x1e`';]&]
[s2; Putting this character into standard log stream adds one tabulator 
of indentation to all subsequent lines (moves text `"right`").&]
[s3;%- &]
[s4;%- &]
[s5;:LOG`_END:%- [@(0.0.255) const]_[@(0.0.255) char]_[* LOG`_END `= `'`\x1f`';]&]
[s2; Putting this character into standard log stream removes one 
tabulator of indentation to all subsequent lines (moves text 
`"left`").&]
[s3;%- &]
[s4;%- &]
[s5;:UppLog`(`):%- [_^Stream^ Stream][@(0.0.255) `&]_[* UppLog]()&]
[s2; Returns a reference to [/ current] log stream.&]
[s3; &]
[s4;%- &]
[s5;:SetUppLog`(Stream`&`):%- [@(0.0.255) void]_[* SetUppLog]([_^Stream^ Stream][@(0.0.255) `&
]_[*@3 log])&]
[s2; Sets the current log stream to [%-*@3 log] (must be an object 
with global lifetime).&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:SetUppLog`(Upp`:`:LogLineFn`):%- [_^Upp`:`:LogLineFn^ LogLineFn]_[* SetUppLog
]([_^Upp`:`:LogLineFn^ LogLineFn]_[*@3 log`_line])&]
[s0;l288; Similar to SetUppLog with Stream parameter, but instead 
of replacing the whole stream, it just replaces the final routine 
to write the single line, while returning its previous definition 
(as function pointer). LogLineFn is defined as&]
[s0;l288; &]
[s0;l288; [C typedef void (`*LogLineFn)(const char `*buffer, int len, 
int depth);]&]
[s2; &]
[s2; where dept is incremented by LOG`_BEGIN character and decremented 
by LOG`_END `- standard logging stream inserts equivalent number 
of tabs at the beginning of line.&]
[s3; &]
[s0; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Logging macros]]}}&]
[s0; &]
[s0; Standard logging macros come in 3 basic flavors. `"Normal`" 
macros (LOG, DUMP, ...) only emit code in debug mode, `"Debugging`" 
macros (DLOG, DDUMP, ..., first letter is `'D`') only [*/ compile] 
in debug mode and their presence causes release mode compilation 
to fail (this is to prevent any forgotten debugging macros in 
code) and finally `"Release`" macros (RLOG, RDUMP, ..., first 
letter is `'R`') produce output even in release mode.&]
[s0; &]
[ {{5000:5000^ [s0; LOG(x), DLOG(x), RLOG(x)]
:: [s0; Outputs a single line to log.]
:: [s0; DUMP(x), DDUMP(x), RDUMP(x)]
:: [s0; Outputs a variable `- adds a variable name before value `-  
defined as LOG(#x << `"`=`" << x)]
:: [s0; DUMPC(x), DDUMPC(x), RDUMPC(x)]
:: [s0; Outputs a Vector or Array or Index of values.]
:: [s0; DUMPM(x), DDUMPM(x), RDUMPM(x)]
:: [s0; Outputs a VectorMap or ArrayMap.]
:: [s0; TIMING(x), DTIMING(x), RTIMING(x)]
:: [s0; Establishes profiling timing inspector which profiles since 
definition till the end of block, profiling values are printed 
to log at the program exit.]
:: [s0; LOGHEX(x), DLOGHEX(x), RLOGHEX(x)]
:: [s0; Outputs value as hexadecimal dump, currently works with String.]
:: [s0; DUMPHEX(x), DDUMPHEX(x), RDUMPHEX(x)]
:: [s0; Outputs variable as hexadecimal dump, like LOGHEX but puts variable 
name into log.]}}&]
[s0; &]
[s0; In addition to this it is common practice to use `'local`' macros 
that can be switched on/off for given modules files; standard 
approach is to add&]
[s0; &]
[s0; #define LLOG(x)    // DLOG(x)&]
[s0; &]
[s0; at the start of files with local logging, then activate logging 
by uncommenting the DLOG.&]
[s0; &]
[s0; U`+`+ has also concept of `"modular`" logging, where specific 
logging macros can be created that are bound to some global boolean 
entity (usually INI`_BOOL). Creation of these modular logs is 
simplified by macro:&]
[s0; &]
[s0; #define [* LOG`_](flag, x)     do `{ if(flag) RLOG(x); `} while(false)&]
[s0; &]
[s0; One such predefined type exists, USRLOG, that is indended to 
log user actions (like opening windows, pressing keyboard keys) 
and can be activated by IniBool Ini`::user`_log (and also by setting 
user`_log`=true in .ini file):&]
[s0; &]
[s0; #define [* USRLOG](x)         LOG`_(Ini`::user`_log, x)&]
[s0; ]]