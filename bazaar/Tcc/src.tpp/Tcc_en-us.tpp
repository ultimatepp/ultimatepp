topic "Reference";
[ $$0,0#00000000000000000000000000000000:Default]
[H6;0 $$1,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$2,0#37138531426314131252341829483370:codeitem]
[l288;2 $$3,0#27521748481378242620020725143825:desc]
[0 $$4,0#96390100711032703541132217272105:end]
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 Tcc]]}}&]
[s0; &]
[s0; &]
[ {{10000@1 [s0; [* Public method list]]}}&]
[s1;%- &]
[s2;:Tcc`:`:Tcc`(const char`*`):%- [* Tcc]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 dllNam
e])&]
[s3; Tcc constructor.  [%-*@3 dllName].contains the file with full 
path where .DLL file is located. In Posix implementation it is 
not necessary to include this file.&]
[s0; &]
[s1;%- &]
[s2;:Tcc`:`:Compile`(const char`*`):%- [@(0.0.255) void]_[* Compile]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 my`_program])&]
[s3; Compiles a C script. [%-*@3 my`_program].contains the full C script 
source code.&]
[s4; &]
[s1;%- &]
[s2;:Tcc`:`:AddSymbol`(const char`*`,void`*`):%- [@(0.0.255) void]_[* AddSymbol]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 funName], [@(0.0.255) void]_`*[*@3 fun])&]
[s3; Adds a function in the main program that can be called from 
the C script.&]
[s3;l480; [%-*@3 funName] is the function name as it will be used in 
the C script&]
[s3;l480; [%-*@3 fun].is a pointer to the function in the main program.&]
[s3; As many functions as necessary can be called from the main program.&]
[s3; Remember to declare them in the C script.&]
[s4; &]
[s1;%- &]
[s2;:Tcc`:`:Link`(const char`*`):%- [@(0.0.255) void]_[* Link]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 fileName]_`=_[@3 0])&]
[s3; Links all program symbols.&]
[s3; If defined SetOutputExe(), [%-*@3 fileName].defines the file name 
where the executable file is going to be copied.&]
[s3; Output executable format depends on the operating system where 
the main program has been compiled:&]
[s3;i150;O0; In Posix the format is ELF&]
[s3;i150;O0; In Windows the format is PE`-COFF&]
[s3; Both are the native program formats so that they can be run 
from a simple terminal window in the native OS&]
[s4;%- &]
[s1;%- &]
[s2;:Tcc`:`:GetProgram`(`):%- [_^String^ String]_[* GetProgram]()&]
[s3; Returns the C script source.&]
[s4;%- &]
[s1;%- &]
[s2;:Tcc`:`:GetSymbol`(const char`*`):%- [@(0.0.255) void]_`*[* GetSymbol]([@(0.0.255) cons
t]_[@(0.0.255) char]_`*[*@3 funName])&]
[s3; Returns the address of a C Script function to be called from 
the main program.&]
[s3;l480; [%-*@3 funName].contains the name of the function in the 
C script.&]
[s4; &]
[s1; &]
[s2;:Tcc`:`:AddIncludePath`(const char`*`):%- [@(0.0.255) bool]_[* AddIncludePath]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 path])&]
[s3; Sets a directory [%-*@3 path ]where include files referenced in 
C script source are located.&]
[s3; This function can be called as many times as necessary for different 
directories.&]
[s3; The original include files are located under Bazaar/plugin/tcc/include 
directory..&]
[s4; &]
[s1;%- &]
[s2;:Tcc`:`:AddLibraryPath`(const char`*`):%- [@(0.0.255) bool]_[* AddLibraryPath]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 path])&]
[s3; Sets a directory [%-*@3 path ]where library files referenced in 
C script source are located.&]
[s3; This function can be called as many times as necessary for different 
directories.&]
[s3; The original library files are located under Bazaar/plugin/tcc/lib 
directory..&]
[s1;%- &]
[s2;:Tcc`:`:SetOutputExe`(`):%- [@(0.0.255) void]_[* SetOutputExe]()&]
[s3; Defines that the program will be output to an executable file 
indicated in Link()&]
[s4;%- &]
[s1;%- &]
[s2;:Tcc`:`:SetOutputMemory`(`):%- [@(0.0.255) void]_[* SetOutputMemory]()&]
[s3; Defines that the program will be run in memory (option by default)&]
[s4;%- &]
[s0; &]
[ {{10000@1 [s0; [* C script function list. This functions can be called from C 
script]]}}&]
[s1;%- &]
[s0;%- [@(0.0.255)2 void][2 _][*2 throw][2 (][@(0.0.255)2 const char][2 _`*][*@3;2 path][2 )]&]
[s3; Throws an Exc exception with message [%-*@3 path].&]
[s4;%- &]
[s0;%- ]