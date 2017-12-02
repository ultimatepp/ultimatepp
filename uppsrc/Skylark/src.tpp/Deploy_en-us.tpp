topic "Deploying Skylark application";
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
[s2; Deploying Skylark application&]
[s0; &]
[s0; This text consist of summarized experiences of a person with 
a very little web`-serving experience. Therefore, some of the 
described methods might not be the most efficient ones and the 
terminology might be slightly inaccurate. However, all of the 
solutions here were tested and proved to work.&]
[s0; &]
[s3; 1. Simplest `- all work is done by U`+`+&]
[s0; &]
[s0; If you want to have only simple web app that performs its task 
on dedicated address (i.e. there is nothing else, like web pages 
or other apps). Let`'s say we want to have [/ myapp] running on 
[/ http://example.com/]. For this you should configure [*@3 root] 
to empty string and set [*@3 port] to 80:&]
[s0; &]
[s7; port`=80;&]
[s7; root`=`"`";&]
[s0; &]
[s0; Now everything you need to do is compile the application, copy 
it anywhere on the [/ example.com] server. Also, copy all the static 
files your application needs and all the witz templates. Those 
should be placed in a directory with path matching that in the 
code (e.g. if you have `"[C@5 http.RenderResult(`"myapp/templates/index.witz`")]`" 
in your code, you should place the template in directory [/ myapp/templates/]). 
Make sure that the [*@3 path] variable is set correctly to point 
to the directories where you placed the witz templates and other 
static files. For the example above it would be to directory 
in which directory [/ myapp] can be found.&]
[s0; &]
[s0; Now log to this remote server, and simply run the binary:&]
[s0; &]
[s7; cd /path/to/myapp&]
[s7; nohup ./myapp `&> myapp.log `&&]
[s0; &]
[s0; The nohup command prevents the application to close when you 
log off. &]
[s0; &]
[s0; Depending on what you need from the application you might want 
to use some autoresurrecting mechanism that relaunches the application 
in case it crashes (there is many, search the web).&]
[s0; &]
[s3; 2. Using Nginx as a proxy&]
[s0; &]
[s0; Now lets assume that your application [/ myapp] is only part of 
larger website, that is running on Nginx server. Alternatively, 
you can use this also when you just want to make things little 
bit faster and simpler by serving the static files (images, scripts 
etc.) by Nginx without processing the requests in [/ myapp] itself.&]
[s0; &]
[s0; We will furthermore assume that you have Nginx already up and 
running (even the default settings in most of distributions is 
OK) and that your application is configured with&]
[s0; &]
[s7; port`=8001;&]
[s7; root`=`"myapp`";&]
[s0; &]
[s0; All you have to do to make Nginx proxy all the requests to it 
is to add following in the nginx configuration file (usually 
located at [/ /etc/nginx/nginx.conf]), in the [/ server] section:&]
[s0; &]
[s7; location /myapp `{&]
[s7; -|proxy`_pass http://localhost:8001;&]
[s7; -|proxy`_set`_header X`-Forwarded`-For `$remote`_addr;&]
[s7; `}&]
[s0; &]
[s0; To make it serve static files directly, you have to add one 
more location rule:&]
[s0; &]
[s7; location /myapp/static `{&]
[s7; -|alias /path/to/static;&]
[s7; `}&]
[s0; &]
[s0; This configuration assumes that all the static files are referenced 
as [/ /myapp/static/somepath/file.ext], if you have them scattered 
over multiple directories, just add one rule for each.&]
[s0; &]
[s0; Now make sure the application is running (see paragraph 1 on 
how to launch it) and reload nginx configuration (if it is running 
already, otherwise use `"start`" instead of `"reload`"):&]
[s0; &]
[s7; /etc/init.d/nginx reload&]
[s0; &]
[s3; 2. Using Apache as a proxy&]
[s0; &]
[s0; You can also use Apache web server in the same way as Nginx 
was used in previous paragraph. The only difference is in configuration. 
To make Apache work as proxy for [/ myapp], add following two lines 
in the apache configuration file (usually [/ /etc/httpd/conf/httpd.conf]):&]
[s0; &]
[s7; ProxyPass         /myapp http://localhost:8001/myapp&]
[s7; ProxyPassReverse  /myapp http://localhost:8001/myapp&]
[s0; &]
[s0; For direct serving of static files you can add this above the 
previous two lines:&]
[s0; &]
[s7; <Directory /tmp/static>&]
[s7;         Allow from all&]
[s7; </Directory>&]
[s7; &]
[s7; ProxyPass         /myapp/static/ !&]
[s7; Alias             /myapp/static /tmp/static&]
[s0; &]
[s0; Reloading the Apache configuration works similar to nginx (again, 
use `"start`" instead of `"reload`" if the server is not running 
yet):&]
[s0; &]
[s7; /etc/init.d/httpd reload&]
[s0; ]]