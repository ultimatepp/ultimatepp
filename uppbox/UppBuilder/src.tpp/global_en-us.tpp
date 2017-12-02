topic "global functions";
[ $$0,0#00000000000000000000000000000000:Default]
[H6;0 $$1,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$2,0#37138531426314131252341829483370:codeitem]
[l288;2 $$3,0#27521748481378242620020725143825:desc]
[0 $$4,0#96390100711032703541132217272105:end]
[i448;a25;kKO9; $$5,0#37138531426314131252341829483380:structitem]
[{_} 
[s2;:NEVER`(T`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) void]_[* NEVER](
[*@4 T]_[*@3 s])&]
[s3;%% Safely terminates the parser when [%-*@3 s] evaluates to true. 
Used to end the execution in case of nonrecoverable state.&]
[s4;%% &]
[s1; &]
[s2;:Slashes`(String`&`): [@(0.0.255) void]_[* Slashes]([_^String^ String][@(0.0.255) `&]_[*@3 s
])&]
[s3;%% Converts all backslashes in [%-*@3 s] to forward slashes.&]
[s4;%% &]
[s1; &]
[s2;:BaseName`(const String`&`): [_^String^ String]_[* BaseName]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 s])&]
[s3;%% Returns everything after last `'/`' character in [%-*@3 s].&]
[s4;%% &]
[s1; &]
[s2;:DirName`(const String`&`): [_^String^ String]_[* DirName]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 s])&]
[s3;%% Returns everything before last `'/`' character in [%-*@3 s].&]
[s4;%% &]
[s1; &]
[s2;:LoadFile`(const String`&`): [_^String^ String]_[* LoadFile]([@(0.0.255) const]_[_^String^ S
tring][@(0.0.255) `&]_[*@3 fn])&]
[s3;%% Loads content of file [%-*@3 fn] into string.&]
[s4;%% &]
[s1; &]
[s2;:GetEnv`(const char`*`,const char`*`): [_^String^ String]_[* GetEnv]([@(0.0.255) const]_
[@(0.0.255) char`*]_[*@3 n],[@(0.0.255) const]_[@(0.0.255) char`*]_[*@3 s][@(0.0.255) `=]`"`"
)&]
[s3;%% Reads environment variable UPP`_[%-*@3 n], if no such variable 
is found then [%-*@3 s] is returned.&]
[s4;%% &]
[s1; &]
[s2;:Split`(String`,Vector`<String`>`&`): [@(0.0.255) void]_[* Split]([_^String^ String]_[*@3 s
],[_^Vector^ Vector]<[_^String^ String]>`&_[*@3 v])&]
[s3;%% Splits string [%-*@3 s] at spaces and adds each chunk into vector 
[%-*@3 v].&]
[s4;%% &]
[s1; &]
[s2;:FlagDir`(const Vector`<String`>`&`): [_^String^ String]_[* FlagDir]([@(0.0.255) const]_
[_^Vector^ Vector]<[_^String^ String]>`&_[*@3 v])&]
[s3;%% Sorts contents of [%-*@3 v] alphabetically and concatenates 
it into string with `'.`' as delimiters&]
[s4;%% &]
[s0;%% ]