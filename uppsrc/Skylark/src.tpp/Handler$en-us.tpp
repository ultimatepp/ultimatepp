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
[ {{10000@(113.42.0) [s0; [*@7;4 HandlerId]]}}&]
[s3;%- &]
[s1;:HandlerId`:`:struct:%- [@(0.0.255)3 struct][3 _][*3 HandlerId]&]
[s2; This simple concrete type is able to store both function to 
pointer or pack instance method id to be used as URL link base. 
Can be created from pointer to handler or as result of THISLINK 
or LINK macros.&]
[s3; &]
[s0; &]
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
[s3;%- &]
[s4;%- &]
[s5;:SKYLARK`(name`, path`):%- [* SKYLARK`_PROGRESS]([*@3 name], [*@3 path], 
[*@3 progressCb])&]
[s2; Defines the handler function [%-*@3 name] (body follows the macro) 
supposed to handle [%-*@3 path] pattern, and setup a [%-*@3 progressCb 
]callback to handle progress feedback in lenghty operations.&]
[s2; [%-*@3 progressCb ]must be a pointer to a function with following 
prototype:&]
[s2; &]
[s2; -|-|int [%-*@3 progressCb](int [%-*@3 reason], Http`& [%-*@3 http], 
int [%-*@3 size]);&]
[s2; &]
[s2; [%-*@3 progressCb ]is called many times during http transferm, 
with different [%-*@3 reason] values :&]
[s2; -|-|[%-*@3 reason] `=`= PROGRESS`_HEADERS-|when headers have been 
parsed and GET variables are ready&]
[s2; -|-|[%-*@3 reason] `=`= PROGRESS`_CONTENT-|during content transfer, 
with a frequency depending on TcpSocket WaitTime value (50 Hz 
by default)&]
[s2; -|-|[%-*@3 reason] `=`= PROGRESS`_END-|when transfer is complete&]
[s2; &]
[s2; [%-*@3 http ]is the Http object passed to Skylark handler&]
[s2; [%-*@3 size ]has different meanings, depending on value :&]
[s2; -|-|if [%-*@3 reason] `=`= PROGRESS`_HEADERS, [%-*@3 size ] is the 
total contents size&]
[s2; -|-|if [%-*@3 reason] `=`= PROGRESS`_CONTENT, [%-*@3 size ] is the 
number of bytes transferred&]
[s2; -|-|if [%-*@3 reason] `=`= PROGRESS`_END, [%-*@3 size ]has no meaning.&]
[s2; &]
[s2; The handler must return a non`-zero value (true) if we want 
to continue transfer, a zero value (false) to abort current transfer.&]
[s2; You can see the SkylarkUpload example demo for usage.&]
[s3;%- &]
[s4;%- &]
[s5;:MakeLink`(const HandlerId`&`,const Vector`<Value`>`&`):%- [_^String^ String]_[* Make
Link]([@(0.0.255) const]_[_^HandlerId^ HandlerId][@(0.0.255) `&]_[*@3 id], 
[@(0.0.255) const]_[_^Vector^ Vector]<[_^Value^ Value]>`&_[*@3 arg])&]
[s2; Based on [%-*@3 handler] and [%-*@3 arg] list of arguments, creates 
URL would get handled by this [%-*@3 handler] with specified arguments.&]
[s3; &]
[s4; &]
[s5;:SkylarkPack`:`:struct:%- struct_[* SkylarkPack]&]
[s2; This is the base class of pack classed, which can be used to 
group and parametrize a set of handlers.&]
[s3; &]
[s4;%- &]
[s5;:SKYLARK`_USE`(cls`, id`, path`):%- [* SKYLARK`_USE]([*@3 cls], [*@3 id], 
[*@3 path])&]
[s2; Creates an instance of pack [%-*@3 cls] as global variable [%-*@3 id] 
with [%-*@3 path] prefix.&]
[s3; &]
[s4;%- &]
[s5;:SKYLARK`_METHOD`(method`, path`):%- [* SKYLARK`_METHOD]([*@3 method], 
[*@3 path])&]
[s2; Registers a pack handler [%-*@3 method] to handler [%-*@3 path] 
pattern (this is combined with path prefix of SKYLARK`_USE). 
Intended to be used in Use method of pack.&]
[s3; &]
[s4;%- &]
[s5;:THISLINK`(method`):%- [* THISLINK]([*@3 method])&]
[s2; Returns HandlerId of [%-*@3 method] `- to be used within pack 
class.&]
[s3; &]
[s4;%- &]
[s5;:LINK`(instance`, method`):%- [* LINK]([*@3 instance], [*@3 method])&]
[s2; Returns HandlerId of specific [%-*@3 instance] [%-*@3 method].&]
[s3; &]
[s4;%- &]
[s5;:SKYLARK`_MEMBER`(member`, path`):%- [* SKYLARK`_MEMBER]([*@3 member], 
[*@3 path])&]
[s2; Registers member variable subpack (in Use method).&]
[s0; ]]