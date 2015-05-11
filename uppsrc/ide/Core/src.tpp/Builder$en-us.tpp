topic "";
[ $$0,0#00000000000000000000000000000000:Default]
[H6;0 $$1,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$2,0#37138531426314131252341829483370:codeitem]
[l288;2 $$3,0#27521748481378242620020725143825:desc]
[0 $$4,0#96390100711032703541132217272105:end]
[{_}%EN-US 
[s1;%- &]
[s2;:Builder`:`:BuildPackage`(const String`&`,Vector`<String`>`&`,Vector`<String`>`&`,String`&`,const Vector`<String`>`&`,const Vector`<String`>`&`,int`):%- [@(0.0.255) v
irtual] [@(0.0.255) bool]_[* BuildPackage]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 package], [_^Vector^ Vector]<[_^String^ String]>`&_[*@3 linkfile], 
[_^Vector^ Vector]<[_^String^ String]>`&_[*@3 immfile], [_^String^ String][@(0.0.255) `&]_[*@3 l
inkoptions], [@(0.0.255) const]_[_^Vector^ Vector]<[_^String^ String]>`&_[*@3 all`_uses],
 [@(0.0.255) const]_[_^Vector^ Vector]<[_^String^ String]>`&_[*@3 all`_libraries], 
[@(0.0.255) int]_[*@3 optimize])&]
[s3; Called for each package during building.&]
[s3; [%-*@3 package] name of package.&]
[s3; [%-*@3 linkfile] builder puts here files to link, for Link method&]
[s3; [%-*@3 immfile] builder puts here all intermediate files (.a, 
.o, .lib ...) so that theide knows them and can touch them (set 
current time) as needed&]
[s3; [%-*@3 linkoptions] again, like linkfile, a place to gather link 
options so that Link method recieves them&]
[s3; [%-*@3 all`_uses] all packages that package uses (including indirect). 
important for .so builds only&]
[s3; [%-*@3 all`_libraries] all libraries that package uses (including 
indirect). important for .so builds only&]
[s3; [%-*@3 optimize] global optimization setting `- R`_SPEED, R`_SIZE, 
R`_OPTIMAL&]
[s4; &]
[s0; ]]