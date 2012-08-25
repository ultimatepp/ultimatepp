topic "Handler functions and macros";
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
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 Handler functions and macros]]}}&]
[s3;%- &]
[s5;:RegisterHandler`(void`(`*`)`(Http`&http`)`,const char`*`,const char`*`):%- [@(0.0.255) v
oid]_[* RegisterHandler]([@(0.0.255) void]_(`*[*@3 handler])(Http[@(0.0.255) `&]_http), 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 p
ath])&]
[s2; Registers handler function [%-*@3 handler] with the name [%-*@3 id] 
(id is usually the same as the name of function) that is supposed 
to handle [%-*@3 path] pattern.&]
[s3; &]
[s4;%- &]
[s5;:SKYLARK`(name`, path`):%- [* SKYLARK]([*@3 name], [*@3 path])&]
[s2; Defines the handler function [%-*@3 name] (body follows the macro) 
supposed to handle [%-*@3 path] pattern.&]
[s3; &]
[s4;%- &]
[s5;:MakeLink`(void`(`*`)`(Http`&`)`,const Vector`<Value`>`&`):%- [_^String^ String]_[* M
akeLink]([@(0.0.255) void]_(`*[*@3 handler])(Http[@(0.0.255) `&]), 
[@(0.0.255) const]_Vector<Value>`&_[*@3 arg])&]
[s2; Based on [%-*@3 handler] function pointer and [%-*@3 arg] list of 
arguments, creates URL would get handled by this [%-*@3 handler] 
with specified arguments.&]
[s3; &]
[s0; ]]