topic "Google Summer of Code 2011 Ideas";
[ $$0,0#00000000000000000000000000000000:Default]
[b117;*+117 $$1,2#27457433418004101424360058862402:Caption]
[b50;2 $$2,2#03324558446220344731010354752573:Par]
[{_}%EN-US 
[s1;#b0; [R^topic`:`/`/uppweb`/gsoc`/gsoc2011`$en`-us^6 GSoC 2011][R6 . 
Ideas]&]
[s0;%- &]
[ {{10000f0;g0;^t/25b4/25 [s0;# [*+117 Introduction]]}}&]
[s0;2 &]
[s0; [2 Google Summer of Code 2011 has been announced and this year 
Ultimate`+`+ project really wants to participate as a mentoring 
organization. ]&]
[s0;2 &]
[s0; [2 If you like C`+`+ and you want to use a radical focus to more 
effective programming we would be happy if you come with us.]&]
[s0;2 &]
[s0; [2 Here we will describe you our proposals. We hope you will like 
them! However you can ask for additional information and we would 
also like to hear your ideas.]&]
[s0;2 &]
[s0; [2 You can introduce yourself in our ][^http`:`/`/www`.ultimatepp`.org`/forums`/^2 F
orum][2 , discuss with the community these ideas and your proposals 
adding your personal touches. You can also contact the ][^topic`:`/`/uppweb`/gsoc`/mentor2011`$en`-us^2 m
entors][2  whose skills can match with your project ideas. This 
way you can do your own proposals with more possibilities to 
be approved.]&]
[s0;2 &]
[s0; [2 This list is a work`-in`-progress. You can follow discussions 
in ][^http`:`/`/www`.ultimatepp`.org`/forums`/^2 Forum][2 .]&]
[s0;2 &]
[s0;%- &]
[ {{10000f0;g0;^t/25b4/25 [s0;# [*+117 Project Ideas]]}}&]
[s0;#* &]
[s0;# [* Web Browser Control]&]
[s0;# &]
[s0;# [*2 Proposed by]&]
[s0;# [^http`:`/`/www`.ultimatepp`.org`/forum`/index`.php`?t`=usrinfo`&id`=647`&^2 Jan 
Dolinár]&]
[s0;# &]
[s0;# [*2 Difficulty]&]
[s0;# [2 High]&]
[s0;#2 &]
[s0;# [*2 Experience required]&]
[s0;# [2 U`+`+, webkit (or any other engine), gcc compiling infrastructure.]&]
[s0;#2 &]
[s0;# [*2 Description]&]
[s0;# [2 This tasks main goal is to create a GUI widget allowing to 
integrate a web browser engine into U`+`+ applications. Webkit 
engine is a good candidate for this task as it is fast and reliable, 
written in C`+`+ and actively developed by large community, but 
other engines might be used as well.]&]
[s0;#2 &]
[s0;# [*2 Mentors]&]
[s0;# [^http`:`/`/www`.ultimatepp`.org`/forum`/index`.php`?t`=usrinfo`&id`=647`&^2 Jan 
Dolinár]&]
[s0;#2 &]
[s0;# [*2 Roadmap]&]
[s0;#i150;O0; [2 Explore possibility to use libwebkit]&]
[s0;#i150;O0; [2 Decide how to integrate Webkit in a U`+`+ project 
(most straightforward possibilities are direct rendering in DHCtrl 
or rendering into buffer first)]&]
[s0;#i150;O0; [2 Write a widget binding U`+`+ events to the rendering 
engine]&]
[s0;#2 &]
[s0;#2 &]
[s0;# [* U`+`+ as a library]&]
[s0;# &]
[s0;# [*2 Proposed by]&]
[s0;# [^http`:`/`/www`.ultimatepp`.org`/forum`/index`.php`?t`=usrinfo`&id`=3`&^2 Mirek 
Fidler][2  and others.]&]
[s0;#*2 &]
[s0;# [*2 Difficulty]&]
[s0;# [2 High]&]
[s0;#2 &]
[s0;# [*2 Experience required]&]
[s0;# [2 U`+`+, MSC, gcc/mingw compiling infrastructure.]&]
[s0;#2 &]
[s0;# [*2 Description]&]
[s0;# [2 U`+`+ is actually more than a C`+`+ library. However it cannot 
be integrated easily in projects not developed in the U`+`+ ide 
TheIde.]&]
[s0;# [2 The goal would be to develop U`+`+ as a library using TheIde 
as a resource editor only.]&]
[s0;# [2 This would imply to create tools and processes that can be 
integrated with infrastructure server so that U`+`+ can be provided 
as pure library (or set of libraries).]&]
[s0;# [2 Create a special mode of TheIDE so that it can be used as 
resource editor only.]&]
[s0;#2 &]
[s0;# [*2 Mentors]&]
[s0;# [2 TBD]&]
[s0;#2 &]
[s0;# [*2 We propose you this roadmap]&]
[s0;#i150;O0; [2 Study U`+`+ TheIde compiling strategy]&]
[s0;#i150;O0; [2 Defining and developing the changes in U`+`+ to make 
projects using make or Visual Studio]&]
[s0;#i150;O0; [2 Changing TheIde to work with external projects as 
a resource editor]&]
[s0;#2 &]
[s0;#2 &]
[s0;# [* Selecting theme engine at runtime]&]
[s0;# &]
[s0;# [*2 Proposed by]&]
[s0;# [^http`:`/`/www`.ultimatepp`.org`/forum`/index`.php`?t`=usrinfo`&id`=906`&^2 Andr
ei Natanael Cosma (Student)]&]
[s0;#2 &]
[s0;# [*2 Difficulty]&]
[s0;# [2 Low to medium]&]
[s0;#2 &]
[s0;# [*2 Experience required]&]
[s0;# [2 C`+`+]&]
[s0;# [2 May help knowing to work with TheIDE]&]
[s0;#2 &]
[s0;# [*2 Description]&]
[s0;# [2 You`'ll have to take U`+`+ code which provide theme for Gnome 
(gtk`+), make it buildable as a `*.so file so it may be linked 
at runtime but also keep the current functionality of being linked 
statically at compile time (embed in application). Create a portable 
interface so further theme engines (i.e. for KDE) may be added 
later.]&]
[s0;#2 &]
[s0;# [*2 Mentors]&]
[s0;# [2 TBD]&]
[s0;#2 &]
[s0;# [*2 Roadmap]&]
[s0;#i150;O0; [2 Get to know U`+`+ basic  theming internals]&]
[s0;#i150;O0; [2 Learn about U`+`+ runtime dynamic linking]&]
[s0;#i150;O0; [2 Refactor the code]&]
[s0;#i150;O0; [2 Create new API (glue)]&]
[s0;#2 &]
[s0;#2 &]
[s0;# [* Simple GUI frontend for Git]&]
[s0;# &]
[s0;# [*2 Proposed by]&]
[s0;# [^http`:`/`/www`.ultimatepp`.org`/forum`/index`.php`?t`=usrinfo`&id`=647`&^2 Jan 
Dolinár]&]
[s0;#2 &]
[s0;# [*2 Difficulty]&]
[s0;# [2 Low]&]
[s0;#2 &]
[s0;# [*2 Experience required]&]
[s0;# [2 U`+`+, Git.]&]
[s0;#2 &]
[s0;# [*2 Description]&]
[s0;# [2 Create a package wrapping standard git command line into U`+`+ 
GUI. The package should allow standalone usage as well as integration 
into other applications.]&]
[s0;#2 &]
[s0;# [*2 Mentors]&]
[s0;# [^http`:`/`/www`.ultimatepp`.org`/forum`/index`.php`?t`=usrinfo`&id`=647`&^2 Jan 
Dolinár][2 , ][^http`:`/`/www`.ultimatepp`.org`/forum`/index`.php`?t`=usrinfo`&id`=595`&^2 I
on Lupascu (backup)]&]
[s0;#2 &]
[s0;# [*2 Roadmap]&]
[s0;#i150;O0; [2 Get to know usvn, as it provides the very same functionality 
for SVN version control system]&]
[s0;#i150;O0; [2 Create a GUI widget providing the basic git functionality 
(create repository, pull, push, display changes, ...)]&]
[s0;#i150;O0; [2 Integrate the widget into TheIDE (to allow using it 
directly with U`+`+ git mirror)]&]
[s0;#i150;O0; [2 If there is any time left, the functionality can be 
extended to support more advanced features of distributed versioning 
systems]&]
[s0;#2 &]
[s0;#2 &]
[s0;# [* U`+`+ port to KDE]&]
[s0;#*2 &]
[s0;# [*2 Proposed by]&]
[s0;# [^http`:`/`/www`.ultimatepp`.org`/forum`/index`.php`?t`=usrinfo`&id`=906`&^2 Andr
ei Natanael Cosma (Student)]&]
[s0;# &]
[s0;# [*2 Difficulty]&]
[s0;# [2 High.]&]
[s0;#2 &]
[s0;# [*2 Experience required]&]
[s0;# [2 U`+`+, Qt.]&]
[s0;#2 &]
[s0;# [*2 Description]&]
[s0;# [2 The goal is to provide a library which wraps Qt theming API 
and draw U`+`+ widgets using it.]&]
[s0;# [2 Right now U`+`+ is using gtk`+ theming API which make applications 
look ugly under KDE and gtk`+ libs have to be available in order 
for application to run.]&]
[s0;# [2 This library will make U`+`+ non`-dependent on a theming API 
but rather selecting proper theming engine at runtime so application 
look will be changed based on running context `- KDE(Qt based) 
or Gnome(gtk`+ based). If Qt or gtk`+ do not exist and user have 
only X11 server U`+`+ will use its own theming API for drawing 
widgets.]&]
[s0;#2 &]
[s0;# [*2 Roadmap]&]
[s0;#i150;O0; [2 Learning to work in GNU/Linux U`+`+]&]
[s0;#i150;O0; [2 Analyzing Gtk`+ theming in U`+`+]&]
[s0;#i150;O0; [2 Applying the right patches to use Qt properly in KDE 
environments]&]
[s0;#2 &]
[s0;# [*2 Mentors]&]
[s0;# [2 TBD]&]
[s0;#^http`:`/`/www`.ultimatepp`.org`/forum`/index`.php`?t`=usrinfo`&id`=906`&^2 &]
[s0;#^http`:`/`/www`.ultimatepp`.org`/forum`/index`.php`?t`=usrinfo`&id`=906`&^2 &]
[s0;# [* Distributed build server]&]
[s0;#*2 &]
[s0;# [*2 Proposed by]&]
[s0;# [^http`:`/`/www`.ultimatepp`.org`/forum`/index`.php`?t`=usrinfo`&id`=836`&^2 Razv
an Oprea]&]
[s0;# &]
[s0;# [*2 Difficulty]&]
[s0;# [2 High.]&]
[s0;#2 &]
[s0;# [*2 Experience required]&]
[s0;# [2 U`+`+,Servers programming, TheIDE build methodology.]&]
[s0;#2 &]
[s0;# [*2 Description]&]
[s0;# [2 A standalone server that can build source files and return 
the result to the client. The system would allow N clients to 
be connected to N servers. The client should be integrated in 
TheIDE and Visual Studio 2008/2010, with a nice interface displaying 
some overall statistics regarding the status of the servers.]&]
[s0;# [2 The hard part is to keep the workload balanced and have the 
system work with a minimum human interference.]&]
[s0;# [2 In this project the server can take advantage of the current 
TheIDE infrastructure creating the possibility for Visual Studio 
to build with GCC.]&]
[s0;# [2 The main purpose of the app is to use the computing power 
of idle workstations in order to speed up the compiling speed 
for a project. Usually most of the time, the CPU of a machine 
is not used 100% therefore when other people need to compile 
a big project the extra power can speed up this process significantly.]&]
[s0;# [2 Also another advantage of having such a system is the ability 
to compile and run on other remote platforms making the development 
for multiple operating systems much easier. The programmer can 
use the development environment of his choice and compile for 
any OS that has a build server running. This will increases productivity 
many times over just because the developer will use TheIDE he 
is used with (constantly switching between Window and Mac is 
really a pain).]&]
[s0;# [2 Having a server with the ability to compile code remotely 
allows users to create a really powerful build machine keeping 
the costs of other computers down just because the user has to 
only run a simple editor. Also, if we take for example the package 
system of TheIDE, the server can dynamically choose already built 
sources increasing the build speed even further.]&]
[s0;# [2 The main purpose of the Distributed Build System is to allow 
programmers to build projects on multiple machines, but this 
can be extended even further. For example the server can keep 
track of the source repository, automated nightly builds, or 
last stable binary files.]&]
[s0;#+92 &]
[s0;# [*2 Roadmap]&]
[s0;#i150;O0; [2 TBD]&]
[s0;#2 &]
[s0;# [*2 Mentors]&]
[s0;# [2 TBD]&]
[s0;#2 &]
[s0;#2 &]
[s0;# [* Google V8 JavaScript in U`+`+]&]
[s0;#2 &]
[s0; [*2 Proposed by]&]
[s0; [^http`:`/`/www`.ultimatepp`.org`/forum`/index`.php`?t`=usrinfo`&id`=595`&^2 Ion 
Lupascu]&]
[s0;2 &]
[s0; [*2 Difficulty]&]
[s0; [2 Medium]&]
[s0;2 &]
[s0; [*2 Experience required]&]
[s0; [2 UPP, V8 JavaScript, JavaScript]&]
[s0;2 &]
[s0; [*2 Description]&]
[s0; [2 V8 implements ECMAScript as specified in ECMA`-262, 3rd edition, 
and runs on Windows XP and Vista, Mac OS X 10.5 (Leopard), and 
Linux systems that use IA`-32 or ARM processors. In U`+`+ V8 
will be a good choise to create possibility for extensions.]&]
[s0;2 &]
[s0; [*2 Roadmap]&]
[s0; [2 TBD]&]
[s0;2 &]
[s0; [*2 Mentors]&]
[s0;# [^http`:`/`/www`.ultimatepp`.org`/forum`/index`.php`?t`=usrinfo`&id`=595`&^2 Ion 
Lupascu]&]
[s0;#2 &]
[s0;#2 &]
[s0;# [* T`+`+ preview inside TheIDE]&]
[s0;#*2 &]
[s0; [*2 Proposed by]&]
[s0; [^http`:`/`/www`.ultimatepp`.org`/forum`/index`.php`?t`=usrinfo`&id`=21`&^2 Peter 
Helcmanovsky]&]
[s0;2 &]
[s0; [*2 Difficulty]&]
[s0; [2 Easy to Medium]&]
[s0;2 &]
[s0; [*2 Experience required]&]
[s0; [2 C`+`+]&]
[s0;2 &]
[s0; [*2 Description]&]
[s0; [2 Add into TheIDE a way to view edited T`+`+ file without whitespace 
marks/etc, ie. `"preview`" mode (Eventually add different preview 
modes like Qtf/Pdf/Web).]&]
[s0;2 &]
[s0; [*2 Roadmap]&]
[s0;i150;O0; [2 Find and learn about how T`+`+ files are edited and 
processed for view (there are some examples including Qtf Designer 
in TheIDE, UWord and uppweb generator).]&]
[s0;i150;O0; [2 Check if Qtf Designer can be connected to T`+`+ file 
editing and what benefits would bring such solution.]&]
[s0;i150;O0; [2 Add preview mode to TheIDE.]&]
[s0;#i150;O0; [2 Eventually add different modes of preview.]&]
[s0;# &]
[s0; [*2 Mentors]&]
[s0;# [2 TBD]&]
[s0;#2 &]
[s0;#2 &]
[s0;# [* Advanced T`+`+ help system]&]
[s0;#*2 &]
[s0; [*2 Proposed by]&]
[s0; [^http`:`/`/www`.ultimatepp`.org`/forum`/index`.php`?t`=usrinfo`&id`=6`&^2 Graeme 
Prentice]&]
[s0;2 &]
[s0; [*2 Difficulty]&]
[s0; [2 Medium to High.]&]
[s0;2 &]
[s0; [*2 Experience required]&]
[s0; [2 C`+`+]&]
[s0;2 &]
[s0; [*2 Description]&]
[s0; [2 To create a standalone help package possibly based on ][^http`:`/`/www`.ultimatepp`.org`/forum`/index`.php`?t`=usrinfo`&id`=472`&^2 m
delfede][2 `'s help viewer. Features:]&]
[s0;2 &]
[s0;#i150;O0; [2 Structured table of contents in parallel with existing 
`"package reference`" help. Anyone can add things or add their 
own pages locally.]&]
[s0;#i150;O0; [2 Existing topic`+`+ pages can be linked to and links 
within those pages work whether they`'re viewed from the `"new`" 
help or existing.]&]
[s0;#i150;O0; [2 Provide history, favorites, bookmarks and retrace.]&]
[s0;#i150;O0; [2 Press F1 in source code and the help window shows 
help for word under cursor i.e. add a mechanism that supports 
this.]&]
[s0;#i150;O0; [2 Click on a blue square and you see the topic page 
in non`-edit mode i.e. go straight to Mr Ped `"preview mode`".]&]
[s0;#i150;O0; [2 Improve the help search so it takes you to the exact 
line.]&]
[s0;#i150;O0; [2 Add `"tag word`" mechanism so that tag words can be 
assigned to a topic`+`+ page and searched for.]&]
[s0;#2 &]
[s0;# [2 For extra credit:]&]
[s0;#i150;O0; [2 Generate pdf with a clickable table of contents and 
index.]&]
[s0;#i150;O0; [2 Convert from topic`+`+ format to Google Docs format 
and vice versa. Google Docs allows simultaneous online changes 
by multiple users so possibly people could write and maintain 
an individual help page as a Google doc and import as a topic 
page when convenient. With a proper help infrastructure, people 
might become more interested in updating the help and the easier 
it is, the more likely people are to contribute.]&]
[s0;#i150;O0; [2 Improve/automate the mechanism for users to submit 
changes/improvements to the help. Related to the last point, 
automating the process so that people are more likely to contribute.]&]
[s0;2 &]
[s0; [*2 Roadmap]&]
[s0;i150;O0; [2 Find and learn about how T`+`+ files are edited and 
processed for view (there are some examples including Qtf Designer 
in TheIDE, UWord and uppweb generator).]&]
[s0;i150;O0; [2 Study ][^http`:`/`/www`.ultimatepp`.org`/forum`/index`.php`?t`=usrinfo`&id`=472`&^2 m
delfede][2 `'s help viewer.]&]
[s0;i150;O0; [2 TBD]&]
[s0;# &]
[s0; [*2 Mentors]&]
[s0;# [2 TBD]&]
[s0;#2 &]
[s0;#2 &]
[s0;# [* Inter`-Process Communication (IPC)]&]
[s0;#2 &]
[s0; [*2 Proposed by]&]
[s0; [^http`:`/`/www`.ultimatepp`.org`/forum`/index`.php`?t`=usrinfo`&id`=595`&^2 Ion 
Lupascu]&]
[s0;2 &]
[s0; [*2 Difficulty]&]
[s0; [2 Medium]&]
[s0;2 &]
[s0; [*2 Experience required]&]
[s0; [2 U`+`+, socket, named pipes (POSIX, WINDOWS).]&]
[s0;2 &]
[s0; [*2 Description]&]
[s0; [2 Using IPC can create a stable plugin manager with crash prevent 
(ex: Google Chrome), running parallel scripts of V8 JavaScript, 
etc.]&]
[s0; [2 A good experience result is chromium project (Google Chrome).]&]
[s0;2 &]
[s0; [*2 Roadmap]&]
[s0; [2 TBD]&]
[s0;2 &]
[s0; [*2 Mentors]&]
[s0;# [^http`:`/`/www`.ultimatepp`.org`/forum`/index`.php`?t`=usrinfo`&id`=595`&^2 Ion 
Lupascu]&]
[s0;#2 &]
[s0;#2 &]
[s0;# [* Your project HERE]&]
[s0;#2 &]
[s0;# [2 Remember you as student can propose your own projects. We 
advice you to read our ][^http`:`/`/www`.ultimatepp`.org`/forum`/^2 Forum][2  
and contact the ][^topic`:`/`/uppweb`/gsoc`/mentor2011`$en`-us^2 mentors][2  
that can match with your project.]&]
[s0;#* &]
[s0;%- &]
[ {{10000f0;g0;^t/25b4/25 [s0;# [*+117 Project requirements]]}}&]
[s0;# &]
[s0;# [2 All U`+`+ community projects have to comply with certain basic 
requirements. These are included in Bazaar pages.]&]
[s0;2 &]
[s0; [2 Package submission and maintenance is described ][^topic`:`/`/uppweb`/www`/bazaar`_submission`$en`-us^2 h
ere][2 .]&]
[s0;2 &]
[s0; [2 If you need to use community packages you would have to do 
][^topic`:`/`/uppweb`/www`/bazaar`_user`$en`-us^2 this][2 .]&]
[s0;2 &]
[s0;# [2 And if you find any bug or you would like to get any additional 
feature or improvement, please go ][^topic`:`/`/uppweb`/www`/bazaar`_support`$en`-us^2 h
ere][2 .]&]
[s0;#2 &]
[s0;# [2 As the main U`+`+ platforms are Windows and GNU/Linux it would 
be important that all projects would support both operating systems 
leaving open the possibility to, in a future, be implemented 
in OSX.]&]
[s0;#2 &]
[s0;# [2 Ultimate`+`+ project is proud of its BSD license so we would 
really prefer that all external libraries to be linked with U`+`+ 
code would be BSD licensed or similar.]&]
[s0; &]
[s0;> [^topic`:`/`/uppweb`/gsoc`/gsoc2011`$en`-us^+75 Return to GSoC 
2011 Home page]&]
[s0; ]]