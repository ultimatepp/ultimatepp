topic "Linter";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 Linter]]}}&]
[s3; &]
[s1;:Linter: [@(0.0.255) class ][*3 Linter]&]
[s2;%% The base class of TheIDE`'s static analyzer tools framework. 
This class provides a generic interface for integrating command`-line 
driven static analysis tools in TheIDE.&]
[s3;%% &]
[s0;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Enumerators]]}}&]
[s0;%% &]
[s0; enum_[@(0.0.255) class ][* Scope]&]
[s2;b17;a17;%% Constants used to indicate the scope of the analysis.&]
[s7;i1120;a17;:Ctrl`:`:CENTER:%% [%-*C@3 File]-|Denotes a single file 
analysis.&]
[s7;i1120;a17;:Ctrl`:`:LEFT:%% [%-*C@3 Package]-|Denotes a single package 
analysis&]
[s7;i1120;a17;:Ctrl`:`:RIGHT:%% [%-*C@3 Project]-|Denotes a complete 
(all packages `+ files) analysis.&]
[s0;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s3; &]
[s5;:Linter`:`:Linter`(const Upp`:`:String`&`): [* Linter]([@(0.0.255) const] 
String[@(0.0.255) `&] [*@3 name])&]
[s2;%% Constructor. Deriving class must provide a valid [%-*@3 name] 
for the module (e.g. `"CppCheck`").&]
[s3; &]
[s4; &]
[s5;:Linter`:`:`~`(`): [* `~Linter]()&]
[s2;%% Default destructor.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Linter`:`:GetConfigFilePath`(`)const: [@(0.0.255) virtual] String 
[* GetConfigFilePath]() [@(0.0.255) const ]`= 0&]
[s2;%% This method should return the path of the linter module`'s 
configuration file. .&]
[s3; &]
[s4; &]
[s5;:Linter`:`:LoadConfig`(`): [@(0.0.255) virtual] Value [* LoadConfig]() 
`= 0&]
[s2;%%  This method should load the linter module`'s configuration.&]
[s3; &]
[s4; &]
[s5;:Linter`:`:SaveConfig`(const Upp`:`:Value`&`): [@(0.0.255) virtual] 
[@(0.0.255) void] [* SaveConfig]([@(0.0.255) const] Value[@(0.0.255) `&] 
[*@3 cfg])&]
[s2;%% This method should save the linter module`'s configuration 
to file.&]
[s3; &]
[s4; &]
[s5;:Linter`:`:Exists`(`)const: [@(0.0.255) virtual] [@(0.0.255) bool] 
[* Exists]() [@(0.0.255) const ]`= 0&]
[s2;%% This method should return true if the linter backend (e.g. 
cppcheck, clang`-tidy) is present on the system. .&]
[s3; &]
[s4; &]
[s5;:Linter`:`:Settings`(`): [@(0.0.255) virtual] [@(0.0.255) void] [* Settings]() 
`= 0&]
[s2;%%  This method should open up the linter`'s configuration window.&]
[s3; &]
[s4; &]
[s5;:Linter`:`:GetName`(`)const: String [* GetName]() [@(0.0.255) const]&]
[s2;%% Returns the name of the linter module.&]
[s3; &]
[s4; &]
[s5;:Linter`:`:CanCheck`(`)const: [@(0.0.255) bool] [* CanCheck]() [@(0.0.255) const]&]
[s2;%%  Returns true if TheIDE is in editing state and is not in 
debug lock mode.&]
[s3; &]
[s4; &]
[s5;:Linter`:`:CheckFile`(`): [@(0.0.255) void] [* CheckFile]()&]
[s2;%% Checks and analyzes the current file.&]
[s3; &]
[s4; &]
[s5;:Linter`:`:CheckPackage`(`): [@(0.0.255) void] [* CheckPackage]()&]
[s2;%% Checks and analyzes the current package.&]
[s3; &]
[s4; &]
[s5;:Linter`:`:CheckProject`(`): [@(0.0.255) void] [* CheckProject]()&]
[s2;%% Checks and analyzes the whole project.&]
[s3; &]
[s4; &]
[s5;:Linter`:`:StdMenu`(Upp`:`:Bar`&`): [@(0.0.255) static] [@(0.0.255) void] 
[* StdMenu](Bar[@(0.0.255) `&] [*@3 menu])&]
[s2;%% Provides a standard linter menu for TheIDE.&]
[s3; &]
[s4; &]
[s5;:Linter`:`:FileMenu`(Upp`:`:Bar`&`): [@(0.0.255) static] [@(0.0.255) void] 
[* FileMenu](Bar[@(0.0.255) `&] [*@3 menu])&]
[s2;%% Provides a linter menu for individual TheIDE c/c`+`+ files&]
[s3;~~~256;%% &]
[s4; &]
[s5;:Linter`:`:PackageMenu`(Upp`:`:Bar`&`): [@(0.0.255) static] [@(0.0.255) void] 
[* PackageMenu](Bar[@(0.0.255) `&] [*@3 menu])&]
[s2;%% Provides a linter menu for individual TheIDE packages.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Protected Method List]]}}&]
[s3; &]
[s5;:Linter`:`:MakeCmdLine`(Linter`:`:Scope`,Vector`&`): [@(0.0.255) virtual] 
String [* MakeCmdLine]([@(0.0.255) enum] Scope [*@3 sc], Vector<String>[@(0.0.255) `&] 
[*@3 paths]) `= 0&]
[s2;%% The deriving module must define this function to create and 
return a [/ complete ]command line to be executed by TheIDE.  [%-*@3 paths] 
contain the path of one or more files to be analyzed, depending 
on the range of the given operation (file, package, project). 
[%-*@3 sc] indicates the scope of the operation.&]
[s3; &]
[s4; &]
[s5;:Linter`:`:OnResults`(const Upp`:`:String`&`): [@(0.0.255) virtual] 
[@(0.0.255) void] [* OnResults]([@(0.0.255) const] String[@(0.0.255) `&] 
[*@3 results]) `= 0&]
[s2;%% This method will deliver the [%-*@3 results] of the static analysis 
to the deriving module. The content of the input data depends 
on the underlying static analysis tool.&]
[s3; &]
[s4; &]
[s5;:Linter`:`:GetFileName`(`)const: String [* GetFileName]() [@(0.0.255) const]&]
[s2;%% Convenience method. Returns the name of TheIDE`'s active file.&]
[s3; &]
[s4; &]
[s5;:Linter`:`:GetFilePath`(`)const: String [* GetFilePath]() [@(0.0.255) const]&]
[s2;%% Convenience method. Return the path of TheIDE`'s active file.&]
[s3; &]
[s4; &]
[s5;:Linter`:`:GetPackageName`(`)const: String [* GetPackageName]() 
[@(0.0.255) const]&]
[s2;%% Convenience method. Returns the name of TheIDE`'s active package.&]
[s3; &]
[s4; &]
[s5;:Linter`:`:GetPackagePath`(`)const: String [* GetPackagePath]() 
[@(0.0.255) const]&]
[s2;%% Convenience method. Returns the path of TheIDE`'s active package.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Function List]]}}&]
[s3; &]
[s5;:RegisterLinterModule`(Linter`&`): [@(0.0.255) void] [* RegisterLinterModule](Linter[@(0.0.255) `&
] [*@3 linter`_module])&]
[s2;%% Registers a new [%-*@3 linter`_module] to TheIDE`'s static analyzers 
list.&]
[s3; &]
[s4; &]
[s5;:GetLinterModuleCount`(`): [@(0.0.255) int] [* GetLinterModuleCount]()&]
[s2;%% Returns the number of registered linter modules.&]
[s3; &]
[s4; &]
[s5;:GetLinterModule`(int`): Linter[@(0.0.255) `&] [* GetLinterModule]([@(0.0.255) int] 
[*@3 i])&]
[s2;%% Returns a reference to the linter module at index [%-*@3 i]. 
&]
[s3; &]
[s4; &]
[s5;:GetActiveLinterModulePtr`(`): Linter [@(0.0.255) `*][* GetActiveLinterModulePtr]()&]
[s2;%% Returns a pointer to the active (selected) linter module. 
Returns [%-@(0.0.255) nullptr] if there is no registered linter 
module&]
[s3; ]]