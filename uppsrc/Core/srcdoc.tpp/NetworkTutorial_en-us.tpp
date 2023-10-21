topic "U++ Network Tutorial";
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[H4;b83;*4 $$3,5#07864147445237544204411237157677:title]
[i288;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;ph2 $$5,5#45413000475342174754091244180557:text]
[l288;b17;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;C@5+75 $$7,7#20902679421464641399138805415013:code]
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
[b83;* $$22,22#78EB85B566C7E078B53494D0FD53D992:subtitle]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_} 
[s2;%% Network Tutorial&]
[s0;b42;a42;ph%% Welcome to the Network Tutorial! In this tutorial, 
you will learn how to use U`+`+ Core components to enrich your 
applications with network support. You will also learn how to 
build server applications that can run locally or be hosted in 
any public cloud, for example, AWS or Microsoft Azure.&]
[s5;%% Moreover, the examples attached to this tutorial are bundled 
with the U`+`+ standard distribution, and they are localized 
in the tutorial assembly. So, you don`'t need to rewrite it yourself 
to launch them or experiment with the code. Good luck!&]
[s5;%% This is the preview version of the tutorial. All planned content 
is not yet available.&]
[s3;%% Table of contents&]
[s0;%% &]
[s0;%% [^topic`:`/`/Core`/srcdoc`/NetworkTutorial`_en`-us`#1^ 1. HTTP 
Request]&]
[s0;%%     [^topic`:`/`/Core`/srcdoc`/NetworkTutorial`_en`-us`#1`_1^ 1.1 
Obtaining data from REST API]&]
[s0;^topic`:`/`/CtrlLib`/srcdoc`/Tutorial`_en`-us`#1`_2^%% &]
[s3;:1:%% 1. HTTP Request&]
[s22;:1`_1:%% 1.1 Obtaining data from REST API&]
[s5;%% One of the most popular APIs available now on the internet 
is the REST API. In this tutorial, you will learn how to use 
the [* HTTPRequest] class to communicate with such an API and use 
the obtained data directly in the application.&]
[s5;%% [* HTTPRequest] is a class that performs synchronous or asynchronous 
HTTP and [* HTTPS] requests. HTTPS requests are very common today. 
More than 80% of services use this kind of connection. To support 
HTTPS in your application, you need to add the [* Core/SSL] package 
to the project. Without this package, your request will fail 
during runtime.&]
[s5; [%% In this tutorial will use ][^https`:`/`/restcountries`.com`/^ https://restcountri
es.com] publicly available API. This API allows you to  explore 
remote server that holds information about world countries. The 
API returns data in JSON format. When you open the mentioned 
page you will have access to the the detailed documentation with 
all available options. I encourage you to visit it.&]
[s5; For simplification, we will use the [^https`:`/`/gitlab`.com`/restcountries`/restcountries`#name^ /
name] resource path to search for countries with the given name. 
In our case, we will look for Germany and information about its 
name and capital city.&]
[s5;%% To perform a request, you need to construct an HttpRequest 
object and pass a [* URL] as a parameter. After constructing the 
object, there is a need to indicate the HTTP method that should 
be used. In our case, we will be using [^https`:`/`/developer`.mozilla`.org`/en`-US`/docs`/Web`/HTTP`/Methods`/GET^ G
ET]. To select a method, there is a dedicated method called [* Method] 
that accepts an integer as a parameter. The integers that can 
be passed are defined within the HttpRequest class. So, the value 
to perform a GET request should be [* HttpRequest`::METHOD`_GET].&]
[s5; main.cpp&]
[s0;l320; [C@(128.0.255) #include][C  ][C@(0.0.255) <][C Core][C@(0.0.255) /][C Core][C@(0.0.255) .
][C h][C@(0.0.255) >]&]
[s0;l320;C &]
[s0;l320; [*C@(0.0.255) using][C  ][*C@(0.0.255) namespace][C  Upp;]&]
[s0;l320;C &]
[s0;l320; [C CONSOLE`_APP`_MAIN `{]&]
[s0;l320; [C     ][*_C HttpRequest http][C (]&]
[s0;l320; [C         ][C@3 `"https://restcountries.com/v3.1/name/germany`&fields`=name,cap
ital`"][C );]&]
[s0;l320; [C     ][*C@(0.0.255) auto][C  content ][C@(0.0.255) `=][C  ][*_C http][*_C@(0.0.255) .][*_C M
ethod(HttpRequest][*_C@(0.0.255) `::][*_C METHOD`_GET)][*_C@(0.0.255) .][*_C Execute()][C ;]&]
[s0;l320; [C     ][*C@(0.0.255) if][C (content][C@(0.0.255) .][C IsVoid()) 
`{]&]
[s0;l320; [C         Cout() ][C@(0.0.255) <<][C  ][C@3 `"Failed to execute 
GET request wit error code `"][C  ][C@(0.0.255) <<][C  http][C@(0.0.255) .][C GetStatusCode
()]&]
[s0;l320; [C                ][C@(0.0.255) <<][C  ][C@3 `".][C@(0.0.255) `\n][C@3 `"][C ;]&]
[s0;l320; [C         ][*_C@(128.0.255) return][C ;]&]
[s0;l320; [C     `}]&]
[s0;l320;C &]
[s0;l320; [C     ][*C@(0.0.255) auto][C  json ][C@(0.0.255) `=][C  ParseJSON(content);]&]
[s0;l320; [C     ][*C@(0.0.255) if][C (json][C@(0.0.255) .][C IsError()) `{]&]
[s0;l320; [C         Cout() ][C@(0.0.255) <<][C  ][C@3 `"Failed to parse 
JSON response.`"][C ;]&]
[s0;l320; [C         ][*_C@(128.0.255) return][C ;]&]
[s0;l320; [C     `}]&]
[s0;l320;C &]
[s0;l320; [C     ][*C@(0.0.255) if][C (json][C@(0.0.255) .][C GetCount() ][C@(0.0.255) `=`=][C  
][C@3 0][C ) `{]&]
[s0;l320; [C         Cout() ][C@(0.0.255) <<][C  ][C@3 `"The JSON is empty. 
HTTP request returns empty countries list.][C@(0.0.255) `\n][C@3 `"][C ;]&]
[s0;l320; [C         ][*_C@(128.0.255) return][C ;]&]
[s0;l320; [C     `}]&]
[s0;l320;C &]
[s0;l320; [C     ][/C@4 // Let`'s parse the search results and display 
them on the terminal.]&]
[s0;l320; [C     Cout() ][C@(0.0.255) <<][C  ][C@3 `"Found countries:][C@(0.0.255) `\n][C@3 `"][C ;]&]
[s0;l320; [C     ][*C@(0.0.255) for][C (][*C@(0.0.255) const][C  ][*C@(0.0.255) auto][C@(0.0.255) `&
][C  result ][C@(0.0.255) :][C  json) `{]&]
[s0;l320; [C         String common`_name;]&]
[s0;l320; [C         String capital;]&]
[s0;l320;C &]
[s0;l320; [C         common`_name ][C@(0.0.255) `=][C  result][C@(0.0.255) `[][C@3 `"name`"][C@(0.0.255) `]
`[][C@3 `"common`"][C@(0.0.255) `]][C ;]&]
[s0;l320; [C         ][*C@(0.0.255) for][C (][*C@(0.0.255) const][C  ][*C@(0.0.255) auto][C@(0.0.255) `&
][C  result`_capital ][C@(0.0.255) :][C  result][C@(0.0.255) `[][C@3 `"capital`"][C@(0.0.255) `]
][C ) `{]&]
[s0;l320; [C             capital ][C@(0.0.255) `=][C  result`_capital;]&]
[s0;l320;C &]
[s0;l320; [C             ][/C@4 // Some countries like South Africa might 
have more than one capital city. For this]&]
[s0;l320; [C             ][/C@4 // tutorial, let`'s ignore it and display 
only the first result on the list.]&]
[s0;l320; [C             ][*_C@(128.0.255) break][C ;]&]
[s0;l320; [C         `}]&]
[s0;l320;C &]
[s0;l320; [C         Cout() ][C@(0.0.255) <<][C  ][C@3 `"`- `"][C  ][C@(0.0.255) <<][C  
common`_name ][C@(0.0.255) <<][C  ][C@3 `" with `"][C  ][C@(0.0.255) <<][C  
capital ][C@(0.0.255) <<][C  ][C@3 `" as a capital city.][C@(0.0.255) `\n][C@3 `"][C ;]&]
[s0;l320; [C     `}]&]
[s0;l320; [C `}]&]
[s5; In this case, the status code should be 200 OK, and the JSON 
response from the server should look as follows:&]
[s0;l320; [C@(0.0.255) `[][C `{]&]
[s0;l320; [C     name ][C@(0.0.255) :][C  `{]&]
[s0;l320; [C         common ][C@(0.0.255) :][C  Germany,]&]
[s0;l320; [C         official ][C@(0.0.255) :][C  Federal Republic of Germany,]&]
[s0;l320; [C         nativeName ][C@(0.0.255) :][C  `{deu ][C@(0.0.255) :][C  
`{official ][C@(0.0.255) :][C  Bundesrepublik Deutschland, common 
][C@(0.0.255) :][C  Deutschland`}`}]&]
[s0;l320; [C     `},]&]
[s0;l320; [C     capital ][C@(0.0.255) :][C  ][C@(0.0.255) `[][C Berlin][C@(0.0.255) `]]&]
[s0;l320; [C `}][C@(0.0.255) `]]&]
[s5;%% Also, please note that the [* .Method(HttpRequest`::METHOD`_GET)] 
can be simplified to [* .GET()]. The same applies to other methods 
as well. For example .Method(HttpRequest`::METHOD`_POST) can be 
replaced with .POST(). So the following line of code:&]
[s0;l320; [*C@(0.0.255) auto][C  content ][C@(0.0.255) `=][C  http][C@(0.0.255) .][*_C Method(Htt
pRequest][*_C@(0.0.255) `::][*_C METHOD`_GET)][C@(0.0.255) .][C Execute();]&]
[s5; Is it exactly the same as:&]
[s0;l320; [*C@(0.0.255) auto][C  content ][C@(0.0.255) `=][C  http][C@(0.0.255) .][*_C GET()][C@(0.0.255) .
][C Execute();]&]
[s5; ]]