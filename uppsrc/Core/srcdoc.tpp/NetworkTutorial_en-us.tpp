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
[s0;%% [^topic`:`/`/Core`/srcdoc`/NetworkTutorial`_en`-us`#1^ 1. Obtaining 
data from REST API via HTTP Request]&]
[s0; [^topic`:`/`/Core`/srcdoc`/NetworkTutorial`_en`-us`#2^ 2. Building 
RESTful server]&]
[s0;^topic`:`/`/CtrlLib`/srcdoc`/Tutorial`_en`-us`#1`_2^%% &]
[s3;:1:%% 1. Obtaining data from REST API via HTTP Request&]
[s5;%% One of the most popular APIs available now on the internet 
is the REST API. In this tutorial, you will learn how to use 
the [*^topic`:`/`/Core`/src`/HttpRequest`_en`-us`#HttpRequest`:`:class^ HTTPRequest
] class to communicate with such an API and use the obtained 
data directly in the application.&]
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
[s5; &]
[s3;:2: 2. Building RESTful server&]
[s5; Now, it`'s time to build our own RESTful server, and don`'t 
look at others. In this tutorial, we will build a simple service 
with one node [* /countries] that will return predefined countries. 
We need to start by creating a server that will listen on any 
port that is not yet taken. To do it, let`'s create a [*^topic`:`/`/Core`/src`/TcpSocket`_en`-us`#TcpSocket`:`:class^ T
cpSocket] instance and then call the [*^topic`:`/`/Core`/src`/TcpSocket`_en`-us`#TcpSocket`:`:Listen`(int`,int`,bool`,bool`,void`*`)^ L
isten] method. This method, in its simplest form, requires providing 
a port as an argument. The safe value in this case for development 
purposes is 8080. Please keep in mind that the default port for 
[* HTTP is 80] and for [* HTTPS is 443]. In the production enviromenet, 
these ports must be used. However, in some operating systems, 
these ports may be blocked by default, and to unlock them, some 
additional actions might be needed. So, to save ourselves the 
trouble, we will use a different port.&]
[s5; After that, we will create a loop for our server logic. In each 
loop iteration, we will handle one request from the client. To 
do that, there is a need for the creation of a separate [* TcpSocket] 
instance dedicated to handling that request. To accept an incoming 
connection from the client, there is a need to call the socket 
[*^topic`:`/`/Core`/src`/TcpSocket`_en`-us`#TcpSocket`:`:class^ Accept][*  
]method. The arugmenet to this method is socket. In our case, 
we need to pass a server socket. The code for this section is 
implemented in the RunServerLoop() function in the code sample 
attached to this paragraph.&]
[s5; In the very final step, we will handle the client request. To 
do that, we need to read the HTTP header provided by the client. 
The header contains many valuable pieces of information, such 
as the URI that was passed by the client and the HTTP method 
that was used. To obtain the header, let`'s create an [*^topic`:`/`/Core`/src`/HttpHeader`_en`-us`#HttpHeader`:`:struct^ H
ttpHeader] class instance and then call the [*^topic`:`/`/Core`/src`/HttpHeader`_en`-us`#HttpHeader`:`:Read`(TcpSocket`&`)^ R
ead] method, with the socket responsible for handling connections 
to the client as a parameter. In order to handle client requests, 
we will need to use the following methods of the HttpHeader class:&]
[s5;l320;i150;O0; [* GetURI()] `- returns URI associated with the header. 
We will use this information to process client request. Moreover, 
URI can also contain additional parameteres to the query.&]
[s5;l320;i150;O0; [* GetMethod()] `- returns HTTP method associated 
with the header.&]
[s5; In the end, to send the response to the client, the [*^topic`:`/`/Core`/src`/Inet`_en`-us`#HttpResponse`(TcpSocket`&`,bool`,int`,const char`*`,const char`*`,const String`&`,const char`*`,bool`)^ H
ttpResponse] function must be used. To use it properly, you need 
to provide a client socket, a status code, and optionally, if 
you don`'t return error content type and data. After providing 
parameters, the function will return all specified information 
directly back to the client.&]
[s5; main.cpp&]
[s0;l320; [C@(128.0.255) #include][C  ][C@(0.0.255) <][C Core][C@(0.0.255) /][C Core][C@(0.0.255) .
][C h][C@(0.0.255) >]&]
[s0;l320;C &]
[s0;l320; [*C@(0.0.255) using][C  ][*C@(0.0.255) namespace][C  Upp;]&]
[s0;l320;C &]
[s0;l320; [*C@(0.0.255) constexpr][C  ][*C@(0.0.255) int][C  SERVER`_PORT 
][C@(0.0.255) `=][C  ][C@3 8080][C ;]&]
[s0;l320;C &]
[s0;l320; [*C@(0.0.255) void][C  ProcessHttpRequest(TcpSocket][C@(0.0.255) `&][C  
client)]&]
[s0;l320; [C `{]&]
[s0;l320; [C     ][*_C HttpHeader header;]&]
[s0;l320; [C     ][*C@(0.0.255) if][C (][C@(0.0.255) !][*_C header][*_C@(0.0.255) .][*_C Read(clien
t)][C ) `{]&]
[s0;l320; [C         Cerr() ][C@(0.0.255) <<][C  ][C@3 `"Failed to read HttpHeader.][C@(0.0.255) `\
n][C@3 `"][C ;]&]
[s0;l320; [C         HttpResponse(client, ][*C@(0.0.255) false][C , HttpStatus][C@(0.0.255) `:
:][C BAD`_REQUEST);]&]
[s0;l320; [C         ][*_C@(128.0.255) return][C ;]&]
[s0;l320; [C     `}]&]
[s0;l320;C &]
[s0;l320; [C     ][*C@(0.0.255) auto][C  path ][C@(0.0.255) `=][C  ][*_C header][*_C@(0.0.255) .][*_C G
etURI()][C ;]&]
[s0;l320; [C     ][*C@(0.0.255) if][C (][*_C header][*_C@(0.0.255) .][*_C GetMethod()][C  
][C@(0.0.255) `=`=][C  ][C@3 `"GET`"][C ) `{]&]
[s0;l320; [C         ][*C@(0.0.255) if][C (path ][C@(0.0.255) `=`=][C  ][C@3 `"/countries`"][C ) 
`{]&]
[s0;l320; [C             JsonArray ja;]&]
[s0;l320; [C             ja ][C@(0.0.255) <<][C  ][C@3 `"Czech Republic`"]&]
[s0;l320; [C                ][C@(0.0.255) <<][C  ][C@3 `"Indonesia`"]&]
[s0;l320; [C                ][C@(0.0.255) <<][C  ][C@3 `"Brazil`"]&]
[s0;l320; [C                ][C@(0.0.255) <<][C  ][C@3 `"France`"][C ;]&]
[s0;l320;C &]
[s0; [C                    ][*C@(0.0.255) auto][C  code ][C@(0.0.255) `=][C  
HttpStatus][C@(0.0.255) `::][C OK;]&]
[s0; [C                    ][*_C HttpResponse(client, ][*_C@(0.0.255) false][*_C , 
code, HttpStatus][*_C@(0.0.255) `::][*_C ToString(code), ][*_C@3 `"application/json`"][*_C ,
]&]
[s0;l320; [*_C                           ja][*_C@(0.0.255) .][*_C ToString())][C ;]&]
[s0;l320; [C         `}]&]
[s0;l320; [C     `}]&]
[s0;l320;C &]
[s0;l320; [C     HttpResponse(client, ][*C@(0.0.255) false][C , HttpStatus][C@(0.0.255) `::][C N
OT`_FOUND);]&]
[s0;l320; [C `}]&]
[s0;l320;C &]
[s0;l320; [*C@(0.0.255) void][C  RunServerLoop(TcpSocket][C@(0.0.255) `&][C  
server)]&]
[s0;l320; [C `{]&]
[s0;l320; [C     ][*C@(0.0.255) for][C (;;) `{]&]
[s0;l320; [C         TcpSocket client;]&]
[s0;l320;C &]
[s0;l320; [C         Cout() ][C@(0.0.255) <<][C  ][C@3 `"Waiting for incoming 
connection from the client...][C@(0.0.255) `\n][C@3 `"][C ;]&]
[s0;l320; [C         ][*C@(0.0.255) if][C (][C@(0.0.255) !][*_C client][*_C@(0.0.255) .][*_C Accept
(server)][C ) `{]&]
[s0;l320; [C             Cerr() ][C@(0.0.255) <<][C  ][C@3 `"Connection from 
the client not accepted.][C@(0.0.255) `\n][C@3 `"][C ;]&]
[s0;l320; [C             ][*_C@(128.0.255) continue][C ;]&]
[s0;l320; [C         `}]&]
[s0;l320;C &]
[s0;l320; [C         ProcessHttpRequest(client);]&]
[s0;l320; [C     `}]&]
[s0;l320; [C `}]&]
[s0;l320;C &]
[s0;l320; [C CONSOLE`_APP`_MAIN]&]
[s0;l320; [C `{]&]
[s0;l320; [C     ][*_C TcpSocket server;]&]
[s0;l320; [C     ][*C@(0.0.255) if][C (][C@(0.0.255) !][*_C server][*_C@(0.0.255) .][*_C Listen(SER
VER`_PORT)][C ) `{]&]
[s0;l320; [C         Cerr() ][C@(0.0.255) <<][C  ][C@3 `"Cannot open server 
port for listening with error ][C@(0.0.255) `\`"][C@3 `"][C  ][C@(0.0.255) <<][C  
server][C@(0.0.255) .][C GetErrorDesc()]&]
[s0;l320; [C                ][C@(0.0.255) <<][C  ][C@3 `"][C@(0.0.255) `\`"][C@3 .][C@(0.0.255) `\n
][C@3 `"][C ;]&]
[s0;l320; [C         ][*_C@(128.0.255) return][C ;]&]
[s0;l320; [C     `}]&]
[s0;l320;C &]
[s0;l320; [C     RunServerLoop(server);]&]
[s0;l320; [C `}]&]
[s5; In the recent version of the U`+`+ framework (2024.1), the HttpStatus 
codes were introduced. It means that you don`'t longer need to 
provide an explicit status code and prhase to HttpRepsonse. So,&]
[s0; &]
[s0;l320; [C HttpResponse(client, ][*C@(0.0.255) false][C , HttpStatus][C@(0.0.255) `::][C NOT`_
FOUND);]&]
[s0; &]
[s5; is the eqivalent to:&]
[s0; &]
[s0;l320; [C HttpResponse(client, ][*C@(0.0.255) false][C@5 , ][C@3 404][C@5 , 
][C@3 `"Not Found`"][C@5 );]&]
[s0; &]
[s5; To test the above example a [*^https`:`/`/curl`.se`/^ curl] terminal 
application can be used. This application is bundled with most 
Linux distributions, and it can be easily downloaded for Windows. 
The command that should be run in the terminal is as follows:&]
[s0;l320;~~~32; [C$2 curl `-v http://127.0.0.1:8080/countries]&]
[s5; The output of the command should be:&]
[s0;l320;~~~288; [C$2+93 `* __Trying 127.0.0.1:8080...]&]
[s0;l320;~~~288; [C+93 `* Connected to 127.0.0.1 (127.0.0.1) port 8080]&]
[s0;l320;~~~288; [C+93 > GET /countries HTTP/1.1]&]
[s0;l320;~~~288; [C+93 > Host: 127.0.0.1:8080]&]
[s0;l320;~~~288; [C+93 > User`-Agent: curl/8.4.0]&]
[s0;l320;~~~288; [C+93 > Accept: `*/`*]&]
[s0;l320;~~~288; [C+93 > _]&]
[s0;l320;~~~288; [C+93 < HTTP/1.1 200 OK]&]
[s0;l320;~~~288; [C+93 < Date: Sun, 3 Dec 2023 12:19:02 `+0100]&]
[s0;l320;~~~288; [C+93 < Server: U`+`+ based server]&]
[s0;l320;~~~288; [C+93 < Connection: close]&]
[s0;l320;~~~288; [C+93 < Content`-Length: 48]&]
[s0;l320;~~~288; [C+93 < Content`-Type: application/json]&]
[s0;l320;~~~288; [C+93 < _]&]
[s0;l320;~~~288; [C+93 `* Closing connection]&]
[s0;l320;~~~288; [C+93 `[`"Czech Republic`",`"Indonesia`",`"Brazil`",`"France`"`]]&]
[s0;l320;~~~288;C &]
[s5; As you can see in the above output, we received our desirable 
response. The [* `-v] parameter stands for verbose mode, and it 
is supposed to show additional information. It is very helpful 
for debugging purposes. Without this parameter, the output is:&]
[s0;l320; [C$2 `[`"Czech Republic`",`"Indonesia`",`"Brazil`",`"France`"`]]&]
[s5; ]]