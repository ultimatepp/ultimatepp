topic "Utilities";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[b42;a42;ph2 $$10,10#E71D914572A169C91BE52E3FB27F810F:text]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 Utilities]]}}&]
[s10; There are several useful functions related to RichText. Below 
is the list of the most important ones.&]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Functions]]}}&]
[s3;%- &]
[s5;:Upp`:`:DeQtf`(const char`*`):%- [_^Upp`:`:String^ String]_[* DeQtf]([@(0.0.255) const]_
[@(0.0.255) char]_`*[*@3 s])&]
[s2;%- The function converts given C string [*@3 s] into QTF format. 
All symbols that causes conflicts will be escaped. It should 
be used in situation where function require string in QTF format, 
but provided string could contain symbols that have meaning in 
QTF (like `'`[`'). Note that alternative is [^topic`:`/`/RichText`/srcdoc`/QTF`$en`-us^ u
sing `'`\1`' escape character].&]
[s7; [%-*C@3 s]-|C string.&]
[s7; [*/ Return value]-|String converted to Qtf format.&]
[s3; ]]