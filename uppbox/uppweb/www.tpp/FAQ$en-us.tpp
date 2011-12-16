topic "Ultimate++ FAQ";
[2 $$0,0#00000000000000000000000000000000:Default]
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[b83;*4 $$3,5#07864147445237544204411237157677:title]
[i288;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;2 $$5,5#45413000475342174754091244180557:text]
[l288;b17;a17;2 $$6,6#27521748481378242620020725143825:desc]
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
[2 $$19,0#53580023442335529039900623488521:gap]
[t4167;C2 $$20,20#70211524482531209251820423858195:class`-nested]
[b50;2 $$21,21#03324558446220344731010354752573:Par]
[l128;2 $$22,22#15420321421851854042230329115330:answer]
[/2 $$23,23#56850243442053430284230945304012:question]
[{_}%EN-US 
[s0; [*R6 Ultimate`+`+ FAQ]&]
[s0;*4 &]
[s0; [*4 Glossary]&]
[s0;*4 &]
[s0; [* Blitz]&]
[s0;* &]
[s22; Technology to speedup compilation times by combining multiple 
C`+`+ source files into single one, thus saving time to parse 
headers (which get only parsed once). Complementary to Hydra.&]
[s0;* &]
[s0;* &]
[s0; [* Hydra]&]
[s0;* &]
[s22; Technology to speedup compilation by using multiple cores, running 
several compilers in parallel. Complementary to Blitz.&]
[s0;* &]
[s0;* &]
[s0; [* Chameleon]&]
[s0;* &]
[s22; GUI skinning system. U`+`+ normally sets the application skin 
to host platform look`&fell.&]
[s0;* &]
[s0;* &]
[s0; [* Rainbow]&]
[s0;* &]
[s22; Compile`-time mostly preprocessor macro based technology to 
decouple GUI host platform from U`+`+ GUI libraries. It allows 
creating a new GUI backend (e.g. MacOSX or Wayland) without actually 
changing U`+`+ sources.&]
[s0;*4 &]
[s0;*4 &]
[s0; [*4 Installation]&]
[s0; &]
[s0; [* Win32]&]
[s0; &]
[s23; Do I need to uninstall Ultimate first or the installer will 
do it for me?&]
[s0;/ &]
[s22; If you try to install U`+`+ in the same dir as previous version, 
installer will detect that and will invoke un`-install first.&]
[s0; &]
[s0; &]
[s0; [/ What will happen with my sources or programs if I had placed 
them inside Ultimate directory?]&]
[s0; &]
[s22; During un`-installation, the whole directory is deleted, sorry. 
That is why your sources are put to separate place (default C:`\MyApps.). 
(Un`-install warns about this before proceeding.). If you have 
any valuable files inside the installation directory, backup 
them first.&]
[s0; &]
[s0; &]
[s0; [/ How do I un`-install U`+`+ ?]&]
[s0; &]
[s22; Use normal Add/Remove programs.&]
[s0; &]
[s0; &]
[s0; &]
[s0; [*4 U`+`+ Library]&]
[s0; &]
[s0; [/ Is there a complete list of U`+`+ widgets?]&]
[s0; &]
[s22; [^topic`:`/`/CtrlLib`/src`/index`$en`-us^ Yes.]&]
[s0; &]
[s0; &]
[s0; [/ What is the minimal size of U`+`+ GUI application?]&]
[s0; &]
[s22; The size of application executable is not the primary design 
goal. U`+`+ prefers simple maintainance, speed and rapid development 
of complex applications. We believe that these goals are more 
important in the age when there are no hard`-drives on the market 
with less than 20GB.&]
[s22; &]
[s22; Anyway: the size of [^topic`:`/`/uppweb`/examples`/Puzzle`$en`-us^ Puzzle] 
example, compiled with Microsoft C`+`+ 7.1 with size optimizations 
is [* 980KB]. This is not great, but you have to consider that 
this represents self`-contained executable with most U`+`+ features 
compiled in and that past this point the size tends to grow quite 
slowly.&]
[s22; &]
[s22; Typical [* complex] U`+`+ application is [* 2`-3MB] long. TheIDE, 
which is a VERY complex application, has [* 3.5MB].&]
[s0; &]
[s0; &]
[s0; [/ Can I create OCX Control using U`+`+?]&]
[s0; &]
[s22; [^topic`:`/`/Ole`/srcdoc`/ocx`$en`-us^ Yes you can.]&]
[s0; &]
[s0; &]
[s0; [/ When compiling using Visual C`+`+ 2005 compiler, I am getting 
errors like:]&]
[s0; [/C1 F:`\upp`\uppsrc`\CtrlLib/CtrlLib.t : warning C4819: The file 
contains a character that cannot be represented in the current 
code page (932). Save the file in Unicode format to prevent data 
loss]&]
[s0; [/C1 F:`\upp`\uppsrc`\CtrlLib/CtrlLib.t(29) : error C2001: newline 
in constant]&]
[s0; &]
[s22; This compiler for some reasons checks whether the content of 
string literals is correct with respect to default windows character 
set. For CJK, default character set uses multi`-byte encoding 
and some of sequences in this encoding can be wrong.&]
[s22; &]
[s22; This should be fixed by #pragma setlocal(`"C`"), but for some 
reason it does not seem to work.&]
[s22; &]
[s22; The only known solution at the moment is to switch the default 
encoding of Windows to some single`-byte character set (like 
Windows 1250).&]
[s0; &]
[s0; &]
[s0; &]
[s0; [*4 TheIDE]&]
[s0; &]
[s0; [/ Why compilation of simple examples like `"HelloWorld`" takes 
so long? Is not BLITZ supposed to be fast?]&]
[s0;/ &]
[s22; The very first build of any GUI example has to build complete 
U`+`+ library, something you cannot expect to happen in seconds. 
After this first rebuild, results (object and/or library files) 
are cached for future similar builds which should be significantly 
faster.&]
[s0; &]
[s0; &]
[s0; [/ What is <prj`-aux>, <ide`-aux> and <temp`-aux> ?]&]
[s0; &]
[s22; In order to be able to work with files that are not part of 
project (aka main package), we have `"pseudopackages`". There 
are three:&]
[s22; &]
[s22; <prj`-aux> list of files is stored with project&]
[s22; <ide`-aux> list of files is global `- is available in any project&]
[s22; <temp`-aux> list of files is temporary, list is empty after 
TheIde starts.&]
[s22; &]
[s22; `"Open file...`" (Ctrl`+O) places the file into <temp`-aux> 
pseudo`-package.&]
[s22; &]
[s22; &]
[s23; TheIDE ignores international characters. Is it a bug?&]
[s22; &]
[s22; Check the code`-page of your file. TheIDE does not allow you 
to enter the character that is not representable in the code`-page 
of your file. You can setup code`-page for the file, for the 
whole package (in package organizer) and also default code`-page 
for TheIDE (default of this default is Win1252).&]
[s22; &]
[s22; &]
[s23; How to insert a new file into the package?&]
[s22; &]
[s22; Right`-click on the file list and invoke `"Insert package directory 
file(s)`". This allows you both to insert existing files, but 
also to insert (create) new files `- just type in the name of 
file (or even several files) into the File field.&]
[s22; &]
[s22; &]
[s0; [/ How to start the layout (image) designer?]&]
[s22;/ &]
[s22; Insert a file with the .lay (or .iml) extension to your project. 
By clicking this file layout (image) designer will be invoked 
to edit it.&]
[s22; &]
[s22; &]
[s23; File with layout (.lay) or image list (.iml) switched to the 
text format. How do I switch it back?&]
[s23; &]
[s23; &]
[s22; Please invoke menu Edit / Edit using designer, or press hotkey 
which defaults to Ctrl`+T.&]
[s0; &]
[s0; &]
[s23; What is the meaning of `"Optimal`", `"Optimize speed`" and `"Optimize 
size`" release settings?&]
[s0; &]
[s22; Generally, you are not supposed to use `"Optimize speed`" or 
`"Optimize size`".&]
[s22; &]
[s22; The thing is that most of application code does not need to 
be speed optimized. And speed optimized code is generally significantly 
longer than size optimized code (mostly because all inline functions 
are really inlined).&]
[s22; &]
[s22; Therefore theide / U`+`+ is able to mark files with `"F`" flag 
that are supposed to be speed optimized; rest is considered a 
size optimized code. Usually, only 5`-10% of code really needs 
to be speed optimized.&]
[s22; &]
[s22; In the `"Optimal`" mode, these file flags are used. In `"force 
speed`" / `"force size`", they are ignored. Those two `"force`" 
modes in fact are mostly development utilities to allow you comparison 
between fully speed optimized code and `"optimal`" version. Anyway, 
your reaction if `"Speed optimized`" code is significantly faster 
than `"Optimal`" should be to identify more files or packages 
to have `"F`" flag and to get the performance on par.&]
[s0; &]
[s0; &]
[s0; [/ Could someone point me to a detailed, step by step, description 
how to build the IDE from the fresh SVN sources? (I do have theide 
installed.)]&]
[s0;/ &]
[s22; Get fresh sources from [^http`:`/`/code`.google`.com`/p`/upp`-mirror`/source`/checkout^ h
ttp://code.google.com/p/upp`-mirror/source/checkout].&]
[s22; &]
[s22; `"trunk/uppsrc`" contains U`+`+ library and theide.&]
[s22; &]
[s22; Start theide.&]
[s22; &]
[s22; Now click on the left `"Assembly`" pane. Rightclick, choose 
`"New`".&]
[s22; &]
[s22; To the `"Package nests`" fill the path to uppsrc from svn (e.g. 
`"c:`\uppsrc.svn`"). `"Output....`" fill any nice output path 
(should not be the same as the path for 2008.1 uppsrc). `"Assembly`" 
`- fill `"uppsrc.svn`".&]
[s22; &]
[s22; Click OK. Make sure `"uppsrc.svn`" is selected. In the right 
pane, choose `"ide`", push `"OK`".&]
[s22; &]
[s22; Now make sure you build in optimal mode `- there is a `"weird`" 
combo box (DropList) with left and right drop buttons in the 
toolbar. Click the right button and choose Optimal.&]
[s22; &]
[s22; Then click the bomb in the toolbar. (`"Rebuild all`").&]
[s22; &]
[s22; It should build. Then, if you are in win32, you can choose `"Build/Open 
output directory`" to fetch the result. Replace theide.exe in 
your upp directory. I recommend keeping existing theide.exe under 
different name as backup.&]
[s22; &]
[s22; Note: You might want to use current U`+`+ library too. You can 
do this by putting it to appropriate assemblies `- without much 
further explanation, try e.g. right clicking Edit on MyApps in 
the initial theide dialog and see paths...&]
[s22; ]