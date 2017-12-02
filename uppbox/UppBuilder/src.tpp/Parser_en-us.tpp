topic "class Parser";
[ $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9; $$1,0#37138531426314131252341829483380:structitem]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:codeitem]
[{_} 
[s1;:Parser`:`:class: [@(0.0.255) class]_[* Parser]&]
[s2;%% Main class that does all the work: loads the main package and 
all its dependencies, evaluates the contents and outputs the 
results. The output writer is interchangeable to allow generating 
information to various build systems.&]
[s3; &]
[s4; &]
[s5;:Parser`:`:Parser`(const char`*`): [* Parser]([@(0.0.255) const]_[@(0.0.255) char`*]_[*@3 m
ainpkg])&]
[s2;%% Constructor, the parameter [%-*@3 mainpkg] specifies which package 
should be parsed, everything else is determined from [^topic`:`/`/UppBuilder`/srcdoc`/environment`$en`-us^ e
nvironment variables]&]
[s3;%% &]
[s4; &]
[s5;:Parser`:`:Process`(`): [@(0.0.255) void]_[* Process]()&]
[s2;%% The output function, called when everything is parsed. It 
can access all the information gathered in the parsing stage 
in order to do whatever necessary to build the package. This 
function is supplied by backend via macro definition, so it can 
do exactly what the backend needs, e.g. generate makefile or 
even call the compiler itself. &]
[s3; &]
[s0;%% ]