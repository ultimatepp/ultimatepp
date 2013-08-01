topic "Serving static files";
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
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 Serving static files]]}}&]
[s0;i448;a25;kKO9;@(0.0.255) &]
[ {{10000F(128)G(128)@1 [s0;%% [* Function List]]}}&]
[s4; &]
[s5;:ServeStaticPage`(Http`&`): [@(0.0.255) void]_[* ServeStaticPage]([_^Http^ Http][@(0.0.255) `&
]_[*@3 http])&]
[s2;%% Skylark handler for serving static files placed on [^topic`:`/`/Skylark`/src`/Config`$en`-us`#SkylarkConfig`:`:path^ p
ath] given in configuration. Files will be served with mime`-type 
based on the extension. Recognized extensions are jpg, png, gif, 
js and css. Files with unknown extensions are served as text. 
The Ini variable [*@3 caching] can be used to set behavior of the 
caching. Currently supported values are 0`=no caching, 1`=use 
ETag header (allows browser to check if the content was modified, 
without downloading the file), 2`=use versioned path like [/ /static/ver1345496224/
favicon.png] that allows to use very strong caching while ensuring 
that when the file is change it will be immediately updated on 
clients browser as well. Both ETag header value and the version 
string are the timestamp of the served file.&]
[s3;%% &]
[s4; &]
[s5;:CachedStatic`(const Vector`<Value`>`&`,const Renderer`*`): [_^Value^ Value]_[* Cache
dStatic](Value filename)&]
[s2;%% A [^topic`:`/`/Skylark`/src`/Witz`$en`-us^ witz function] intended 
as a helper to allow advanced caching of static files through 
the ServeStaticPage handler. It takes only one argument, the 
file to be served.&]
[s3;%% &]
[s0;%% ]]