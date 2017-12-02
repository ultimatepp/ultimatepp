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
[s2; Initial setup wizard (overview)&]
[s0; [@6 (POSIX specific)]&]
[s0; &]
[s0; On the first start of TheIDE, user is presented with a wizard`-style 
dialog, which goal is to help user easily set up everything necessary 
to run TheIDE. This dialog can be also invoked later using command 
line option [/ `-install] or from the [/ Setup] menu. For more detailed 
and illustrated description see a [^topic`:`/`/ide`/SrcUpdater`/app`/LinuxInstallGuide`$en`-us^ s
tep by step description of the wizard].&]
[s0; &]
[s0; The wizard lets user select how the source codes should be managed 
and updated, currently supporting four methods:&]
[s0;i150;O0; Local copy of sources that reside in /usr/share/upp 
(or equivalent). It is assumed that those are maintained and 
updated by a package management software.&]
[s0;i150;O0; Read`-only assemblies in /usr/share/upp. Again, these 
files should be updated by package manager. This choice also 
assumes that user will never need to alter the U`+`+ sources.&]
[s0;i150;O0; SVN based assemblies. This option checks out the U`+`+ 
sources from SVN repository and sets up the assemblies on top 
of them.&]
[s0;i150;O0; No repositories. This option is for users who prefer 
to manage the sources manually.&]
[s0; &]
[s0; If SVN based assembly is chosen, user has further options to 
set up the SVN server: either the default mirror on code.google.com 
or custom setup. The custom setup allows to set up repository 
address, user and password. Also user can choose one of three 
modes of synchronization:&]
[s0;i150;O0; Always get the newest sources available.&]
[s0;i150;O0; Synchronize the sources with the version of TheIDE (useful 
for example when TheIDE is updated to stable release versions 
by package manager).&]
[s0;i150;O0; Manual updates.&]
[s0; &]
[s0; Last part of settings is dedicated to the assemblies to be created. 
The settings are fully editable. To save user some typing, substitution 
variables `$(SRC) and `$(OUT) can be set and used in settings.&]
[s0; &]
[s0; The entire process is designed with two main goals:&]
[s0;i150;O0; To be as simple as possible and user`-friendly even 
for newbies.&]
[s0;i150;O0; To be flexible enough, so that skilled users are not 
restricted in any way.&]
[s0; The author used all his imagination and common sense to meet 
those goals. For the first one, newbie user can just hit Finish 
and start exploring TheIDE right away. On the other hand, the 
dialog should be flexible enough to serve not only for managing 
U`+`+ sources, but also any other project, no matter if it is 
U`+`+ based or not.&]
[s0; &]
[s0; Clicking [/ Finish] button at any time results into applying and 
saving all the choices (and safe defaults for any skipped steps). 
The [/ Cancel] button results into leaving the wizard immediately 
without saving anything.&]
[s0; &]
[s2; [+150 Additional notes:]&]
[s0;i150;O0; It is safe to run installation wizard multiple times.&]
[s0;i150;O0; Only the latest setting is used for automatic updates.&]
[s0;i150;O0; If there already are some assemblies, they are not deleted, 
only if the name matches, then it is overwritten.&]
[s0;i150;O0; The effects of installation can be reverted using [/^topic`:`/`/ide`/SrcUpdater`/app`/Uninstall`$en`-us^ `-
uninstall][^topic`:`/`/ide`/SrcUpdater`/app`/Uninstall`$en`-us^  
command line option].&]
[s0; ]