topic "Assist++ C++ parser directives";
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
[b83;*4 $$10,11#07864147445237544204411237157677:title]
[b42;a42;2 $$11,11#45413000475342174754091244180557:text]
[l321;t246;C@5;1 $$12,12#20902679421464641399138805415013:code]
[a83;*R6 $$13,11#31310162474203024125188417583966:caption]
[{_}%EN-US 
[s13; Specifics of TheIDE C`+`+ parser&]
[s11; Assist`+`+ C`+`+ parser does not follow C/C`+`+ standards exactly, 
for performance and practical reasons. This documents provides 
information about deviations, heuristics and tricks that we use 
to make the machinery fast and highly error resistant.&]
[s10; Handling of source files, headers, macros and namespaces&]
[s11; The main difference between C`+`+ compiler and theide C`+`+ 
parser is that theide is handling any source file separately. 
This is an absolute performance requirement if global database 
is to be maintained while editing files.&]
[s11; For this reason, declaration and definition must be be in single 
file. For example&]
[s11; [* File1.h]&]
[s12; struct Foo `{&]
[s12; &]
[s11; [* File2.h]&]
[s12; #include `"File1.h`"&]
[s12; -|int bar;&]
[s12; `};&]
[s11; is [*/ NOT] supported.&]
[s11; Parser preprocessor rules:&]
[s11;i150;O0; All #if/#ifdef conditions are considered true. This 
is useful e.g. when there is platform specific code  `- parser 
is then able to pick all variants. #else parts are excluded. 
For example&]
[s12; #ifdef PLATFORM`_WIN32&]
[s12; void CloseWindow(void `*handle) `{&]
[s12; ....&]
[s12; `}&]
[s12; #endif&]
[s12; #ifdef PLATFORM`_GTK&]
[s12; void CloseWindow(void `*handle) `{&]
[s12; .....&]
[s12; `}&]
[s12; #endif&]
[s11; both CloseWindow definitions will be in codebase.&]
[s11;i150;O0; When expanding macro, last #define directive is used.&]
[s12; #define FOO 1&]
[s12; #define FOO 2&]
[s12; FOO&]
[s11; the last line will be expanded to `"2`".&]
[s11;i150;O0; #undef cancels the last definition of the same macro, 
if it is defined in the same file. This is useful to handle special 
defines used for handling name clashes when including external 
libraries:&]
[s12; #define byte win32`_byte`_ // RpcNdr defines byte `-> class 
with Upp`::byte&]
[s12; #define CY win32`_CY`_&]
[s12; #include <objidl.h>&]
[s12; #include <winnetwk.h>&]
[s12; #undef byte&]
[s12; #undef CY&]
[s11;i150;O0; #include in file adds all macros that are (recursively) 
defined by included file and also all `"using namespace`" directives. 
It [* DOES NOT] use namespace block definitions, for example this 
abomination is not supported:&]
[s11; [* StartNamespace.h]&]
[s12; namespace MyNamespace `{&]
[s12; &]
[s11; [* EndNamespace.h]&]
[s12; `};&]
[s12; &]
[s11; [* File.cpp]&]
[s12; #include `"StartNamespace.h`"&]
[s12; void Foo();&]
[s12; #include `"EndNamespace.h`"&]
[s11; is [*/ NOT] supported.&]
[s11;i150;O0; However, if file gets into the project through #include, 
all macros, usings and namespace block definitions are correctly 
included/used. Consider&]
[s11; [* MasterHeader.h]&]
[s12; #define FOO 1&]
[s12; using namespace Bar;&]
[s12; namespace Foo `{&]
[s12; #include `"SubHeader.h`"&]
[s12; `};&]
[s12; &]
[s11; [* SubHeader.h]&]
[s12; &]
[s12; void Fn() `{ return FOO; `}&]
[s12; &]
[s11; This [* IS] supported: Fn will be in Foo namespace and will return 
1.&]
[s10; Macro overrides, namespace macros&]
[s11; It is possible to tell TheIDE overriding definition of specific 
macros in special .defs files. One global.defs file resides in 
TheIDE configuration and is accessible through `"<meta>`" package. 
Also, .defs files can be put into packages. If macro is defined 
in .defs files, it overrides all occurrences of the identifier 
in all cases.&]
[s11; Note that after changing .defs file, it is necessary to rescan 
the code.&]
[s11; .defs files are also used to amend one issue that has no better 
solution in the framework. Consider&]
[s11; File.h&]
[s12; #include `"Core.h`"&]
[s12; NAMESPACE`_UPP&]
[s12; #include `"subheader.h`"&]
[s12; END`_UPP`_NAMESPACE&]
[s12; &]
[s11; where NAMESPACE`_UPP and END`_UPP`_NAMESPACE are defined somewhere 
in Core.h as&]
[s12; &]
[s12; #define NAMESPACE`_UPP namespace Upp `{&]
[s12; #define END`_UPP`_NAMESPACE `};&]
[s12; &]
[s11; Unfortunately, withing parser logic we have not found a fast 
way how to detect and resolve this situation (we do not know 
meaning of NAMESPACE`_UPP/END`_UPP`_NAMESPACE when extracting 
macros from File.h). The solution is simple, putting those #defines 
into .defs file fixes the issue, as such macros are detected 
by special code and used when handling File.h.&]
[s10; Grounding heuristics&]
[s11; It is a good idea to make parser highly resistant to bugs, including 
code that it does not understand. For this reason parser is using 
somewhat strange but very effective simple heuristics: If a line 
starts (at character zero) with identifier which is not followed 
by single `':`' character (to exclude labels), it is a definition 
on `'global`' level. Consider&]
[s12; void Fn1() `{&]
[s12; -|for(int i `= 0; i < 10; i`+`+) `{&]
[s12; -|-|Cout() << i;&]
[s12; `}&]
[s12; &]
[s12; void Fn2() `{&]
[s12; `}&]
[s12; &]
[s11; There is missing `'`}`' in Fn1, which would likely make parser 
miss the rest of file (because all would be considered to be 
part of Fn1 body). However Fn2 definition invokes grounding heuristics 
and parsing restarts at that point, not missing Fn2.&]
[s10; Assist`+`+ C`+`+ parser directives&]
[s11; Assist supports simple directives that are passed to it via 
`'//`' comments that can be used in cases that original code 
is confusing:&]
[ {{3850:6150 [s12;l32; [*+75 //`$`-]]
:: [s11; Code past this directive will [*/ not] be passed to parser.]
:: [s12;l32; [*+75 //`$`+]]
:: [s11; Code past this directive will be passed to parser (stops [*C@5+75 //`$`-]).]
:: [s12;l32; [*+75 //`$_][*/+75 code]&]
[s12;l32; [A@0+75 example:][*+75  //`$ void Foo(Bar`&)]]
:: [s11; Code will be passed to parser (adds code that is not part of 
compiled sources)]}}&]
[s0; ]]