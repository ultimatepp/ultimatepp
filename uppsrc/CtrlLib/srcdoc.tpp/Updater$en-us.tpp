topic "About Updater";
[2 $$0,0#00000000000000000000000000000000:Default]
[l288;i704;a17;O9;~~~.992;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[b83;*2 $$3,5#07864147445237544204411237157677:title]
[i288;b167;a42;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;2 $$5,5#45413000475342174754091244180557:text]
[l288;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;t246;C@5;1 $$7,7#20902679421464641399138805415013:code]
[b2503;2 $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base]
[t4167;C2 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam]
[b167;C2 $$13,13#92430459443460461911108080531343:item1]
[i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128)2 $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7;2 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line]
[l160;t4167;*C+117 $$18,5#88603949442205825958800053222425:package`-title]
[{_}%EN-US 
[s2; About Updater&]
[s0; Updater.exe and related CtrlLib routines can save a lot of maintainance 
costs when deploying new application versions at large client`'s 
side.&]
[s0; &]
[s0; The ideas is this: New version is placed to the network drive. 
When user starts his local application, it checks network directory 
for the new version. If there is any, application starts updater 
and exits. Updater overwrites current version with new one and 
restarts it. Note that the need for updater is caused by fact 
that .exe cannot overwrite itself.&]
[s0; &]
[s0; See CtrlLib/Update for details.&]
[s0; &]
[s0; Now the funny part is that it can update even more than itself 
`- it is used to update any files in app directory. So updater.exe 
itself is updated by application. Last development has gone even 
so far, that Oracle client installation is `"updated`", that 
in fact means that deploying our Oracle apps can be done by copying 
two files to clients directory `- app itself and `"q.ini`" file, 
which basically contains information about network directory 
in UPDATE`=[/ path ]entry.&]
[s0; &]
[s0; Example `- this is how some of our app GUI`_APP`_MAIN looks 
like:&]
[s0; &]
[s7; GUI`_APP`_MAIN&]
[s7; `{&]
[s7; -|SelfUpdate(); // SelfUpdate updates updater.exe first and then 
app itself&]
[s7; -|UpdateFile(`"cs`-cz.scd`"); // Update czech spelling checker&]
[s7; -|UpdateFile(`"logo.bmp`"); // Update company logo to be used 
in reports&]
[s0; &]
[s0; &]
[s0; ]