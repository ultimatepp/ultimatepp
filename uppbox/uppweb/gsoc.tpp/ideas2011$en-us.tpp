topic "Google Summer of Code 2011 Ideas";
[ $$0,0#00000000000000000000000000000000:Default]
[b117;*+117 $$1,2#27457433418004101424360058862402:Caption]
[b50;2 $$2,2#03324558446220344731010354752573:Par]
[{_}%EN-US 
[s1;#b0; [R^topic`:`/`/uppweb`/gsoc`/gsoc2011`$en`-us^6 GSoC 2011][R6 . 
Ideas]&]
[s1;#b0; &]
[s1;#b0; Introduction&]
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
[s0; [2 You can introduce yourself in our ][^http`:`/`/www`.ultimatepp`.org`/forum`/^2 Fo
rum][2 , discuss with the community these ideas and your proposals 
adding your personal touches.]&]
[s0;2 &]
[s0; [2 This list is a work`-in`-progress. You can follow discussions 
in ][^http`:`/`/www`.ultimatepp`.org`/forum`/^2 Forum][2 .]&]
[s0;2 &]
[s1; Project Ideas&]
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
[s0;# [*2 We propose you this roadmap]&]
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
[s0;# [^http`:`/`/www`.ultimatepp`.org`/forum`/index`.php`?t`=usrinfo`&id`=906`&^2 Andr
ei Natanael]&]
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
[s0;# [^http`:`/`/www`.ultimatepp`.org`/forum`/index`.php`?t`=usrinfo`&id`=906`&^2 Andr
ei Natanael]&]
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
ei Natanael]&]
[s0;#2 &]
[s0;# [*2 Difficulty]&]
[s0;# [2 TBD]&]
[s0;#2 &]
[s0;# [*2 Experience required]&]
[s0;# [2 TBD]&]
[s0;#2 &]
[s0;# [*2 Description]&]
[s0;# [2 A .so file which provide theming for KDE and another for Gnome 
`- part of that will be solved by my GSoC application `- if user 
doesn`'t have any DM based on gtk/qt then U`+`+ style will be 
used.]&]
[s0;#2 &]
[s0;# [*2 Mentors]&]
[s0;# [^http`:`/`/www`.ultimatepp`.org`/forum`/index`.php`?t`=usrinfo`&id`=906`&^2 Andr
ei Natanael]&]
[s0;#2 &]
[s0;# [*2 We propose you this roadmap]&]
[s0;# [2 TBD]&]
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
[s0;# [2 Git]&]
[s0;#2 &]
[s0;# [*2 Description]&]
[s0;# [2 Create a package wrapping standard git command line into U`+`+ 
GUI. The package should allow standalone usage as well as integration 
into other applications.]&]
[s0;#2 &]
[s0;# [*2 Mentors]&]
[s0;# [^http`:`/`/www`.ultimatepp`.org`/forum`/index`.php`?t`=usrinfo`&id`=647`&^2 Jan 
Dolinár]&]
[s0;#2 &]
[s0;# [*2 We propose you this roadmap]&]
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
[s1; Project requirements&]
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
[s0; ]