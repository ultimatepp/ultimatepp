topic "Tutorial";
[2 $$0,0#00000000000000000000000000000000:Default]
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[H4;b83;*4 $$3,5#07864147445237544204411237157677:title]
[i288;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;2 $$5,5#45413000475342174754091244180557:text]
[l288;b17;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;C@5;1 $$7,7#20902679421464641399138805415013:code]
[b2503;2 $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base]
[C2 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam]
[b167;C2 $$13,13#92430459443460461911108080531343:item1]
[i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128)2 $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7;2 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line]
[l160;*C+117 $$18,5#88603949442205825958800053222425:package`-title]
[2 $$19,0#53580023442335529039900623488521:gap]
[C2 $$20,20#70211524482531209251820423858195:class`-nested]
[b50;2 $$21,21#03324558446220344731010354752573:Par]
[{_}%EN-US 
[s2; Skylark Tutorial&]
[s0; &]
[s3; 1. SKYLARK handlers and path patterns&]
[s5; Skylark application is divided to `'handlers`' which provide 
response to various HTTP requests based on url pattern:&]
[s7; #include <Skylark/Skylark.h>&]
[s7; &]
[s7; using namespace Upp;&]
[s7; &]
[s7; [* SKYLARK](HomePage, `"`")&]
[s7; `{&]
[s7; -|http << `"<html><body>Hello world!</body></html>`";&]
[s7; `}&]
[s7; &]
[s7; [* SKYLARK](Param, `"`*/param`")&]
[s7; `{&]
[s7; -|http << `"<html><body>Parameter: `" << http`[0`] << `"</html></body>`";&]
[s7; `}&]
[s7; &]
[s7; [* SKYLARK](Params, `"params/`*`*`")&]
[s7; `{&]
[s7; -|http << `"<html><body>Parameters: `";&]
[s7; -|for(int i `= 0; i < http.GetParamCount(); i`+`+)&]
[s7; -|-|http << http`[i`] << `" `";&]
[s7; -|http << `"</html></body>`";&]
[s7; `}&]
[s7; &]
[s7; [* SKYLARK](CatchAll, `"`*`*`")&]
[s7; `{&]
[s7; -|http.[* Redirect](HomePage);&]
[s7; `}&]
[s7; &]
[s7; struct MyApp : SkylarkApp `{&]
[s7; -|MyApp() `{&]
[s7; -|-|[* root `= `"myapp`";]&]
[s7; -|#ifdef `_DEBUG&]
[s7; -|-|prefork `= 0;&]
[s7; -|-|use`_caching `= false;&]
[s7; -|#endif&]
[s7; -|`}&]
[s7; `};&]
[s7; &]
[s7; CONSOLE`_APP`_MAIN&]
[s7; `{&]
[s7; #ifdef `_DEBUG&]
[s7; -|StdLogSetup(LOG`_FILE`|LOG`_COUT);&]
[s7; -|Ini`::skylark`_log `= true;&]
[s7; #endif&]
[s7; &]
[s7; -|[* MyApp().Run();]-|&]
[s7; `}&]
[s5; Handlers are introduced by macro [@5 SKYLARK], first parameters 
is the name of handler (macro creates a function that is named 
this way and this id is also used in various places like page 
templates) and path pattern which handler is supposed to respond 
to.&]
[s5; Handler is represented by C`+`+ function with single parameter 
Http`& [/ http], which is used for request processing. [@5 SKYLARK] 
macro creates a header for this function and also registers it 
with Skylark so that requestes matching the path pattern are 
dispatched to this function. Important note: as [@5 SKYLARK] macro 
uses global constructors to register handlers, handlers should 
be put into .icpp files (unless they are in the same file with 
`'main`').&]
[s5; Path pattern can contain parameter placeholders `'`*`' (see 
[@5 Param] handler), which are then provided by [/ http] as parameters 
(through operator`[`](int)). Path is always split by `'/`' characters 
and each part must either be text or placeholder `- patterns 
like `"something`*/to`*avoid`" are not allowed&]
[s5; Special wildcard `'`*`*`' represents any other number of placeholders 
(see [@5 Params]); in special case it is useful to catch all unresolved 
requests to website and [@5 Redirect] them to the application homepage 
(see [@5 CatchAll]).&]
[s5; The priority of various handlers for particular request is determined 
by number of text parts matched, which means that for request 
`"foo/bar/32`" placeholder `"foo/bar/`*`" has priority over `"foo/`*/`*`".&]
[s5; Another concern is setting of root path for application, in 
code above done by `"[* root ]`= `"[@3 myapp]`";`". This defines 
the root path of application, in other words [@5 HomePage ]handler 
will react to `"[@3 myapp]`" path, Param handler to e.g. `"[@3 myapp]/test/param`" 
etc.&]
[s5; [@5 MyApp().Run()] starts a HTTP server (and also SCGI server) 
on default port 8001, so after starting application, you should 
be able to access it from your browser by entering &]
[s5; 127.0.0.1:8001/myapp&]
[s5; We then recommend to try e.g.&]
[s0; 127.0.0.1:8001/myapp/anything/param&]
[s0; 127.0.0.1:8001/myapp/params/1/2/3&]
[s0; 127.0.0.1:8001/myapp/anythingelse&]
[s0; 127.0.0.1:8001&]
[s5; (last one should result in error `"Page not found`", as no such 
handler is present)&]
[s5; &]
[s3; 2. Witz templates&]
[s5; To decouple presentation layer from application logic, Skylark 
features template language `'Witz`':&]
[s7; #include <Skylark/Skylark.h>&]
[s7; &]
[s7; using namespace Upp;&]
[s7; &]
[s7; SKYLARK(HomePage, `"`")&]
[s7; `{&]
[s7; -|ValueArray va;&]
[s7; -|va.Add(1);&]
[s7; -|va.Add(`"Hello`");&]
[s7; -|ValueMap m;&]
[s7; -|m.Add(`"key1`", `"first value`");&]
[s7; -|m.Add(`"key2`", `"second value`");&]
[s7; &]
[s7; -|[* http(`"MyValue`", `"some value`")]&]
[s7; [* -|    (`"MyRawValue`", Raw(`"<b>raw <u>html</u></b>`"))]&]
[s7; [* -|    (`"MyRawValue2`", `"<b>another raw <u>html</u></b>`")]&]
[s7; [* -|    (`"MyArray`", va)]&]
[s7; [* -|    (`"MyMap`", m)]&]
[s7; [* -|    .RenderResult(`"Skylark02/index`");]&]
[s7; `}&]
[s7; &]
[s7; struct MyApp : SkylarkApp `{&]
[s7; -|MyApp() `{&]
[s7; -|-|root `= `"myapp`";&]
[s7; -|#ifdef `_DEBUG&]
[s7; -|-|prefork `= 0;&]
[s7; -|-|[* use`_caching ]`= false;&]
[s7; -|#endif&]
[s7; -|`}&]
[s7; `};&]
[s7; &]
[s7; CONSOLE`_APP`_MAIN&]
[s7; `{&]
[s7; #ifdef `_DEBUG&]
[s7; -|StdLogSetup(LOG`_FILE`|LOG`_COUT);&]
[s7; -|Ini`::skylark`_log `= true;&]
[s7; #endif&]
[s7; &]
[s7; -|MyApp().Run();-|&]
[s7; `}&]
[s7; &]
[s7; &]
[s5; [* Skylark02/index.witz:]&]
[s7; <html>&]
[s7; <body>&]
[s7; MyValue: [* `$MyValue]<br>&]
[s7; MyRawValue: [* `$MyRawValue]<br>&]
[s7; MyRawValue2: [* `$raw(MyRawValue2)]<br>&]
[s7; <br>MyArray:<br>&]
[s7; [* `$for(i in MyArray)]&]
[s7; [* -|`$i.`_index. `$i<br>]&]
[s7; [* `$endfor]&]
[s7; <br>MyMap:<br>&]
[s7; [* `$for(i in MyMap)]&]
[s7; [* -|`$i.`_index. `$i.`_key: `$i<br>]&]
[s7; [* `$endfor]&]
[s7; </body>&]
[s7; </html>&]
[s7; &]
[s5; Witz template language loosely ressembles JavaScript. It is 
able to evaluate expression, supports `"if`" and looping through 
values with `"for`".&]
[s5; Witz templates are compiled at runtime. Settings configuration 
variable [* use`_caching] to false makes them to compile each time 
they are invoked (otherwise the compilation result is cached) 
`- this is usefull while debugging, as templates can be adjusted 
while application is running.&]
[s5; The search path for templates is set by another configuration 
variable [* path]. Default value of [* path] is set to environment 
variable `'UPP`_ASSEMBLY`_`_`', which is set by theide to current 
assembly path `- which means that for debugging you do not need 
to worry about [* path] as long as all templates are in U`+`+ packages 
and you start the application from theide. Note that [* path] is 
also used to search for static files.&]
[s5; String values are normally HTML escaped; if you need to pass 
raw html code as parameter, you have to either use [@5 Raw ]C`+`+ 
function in application logic or [@5 raw] function in witz.&]
[s5; ]]