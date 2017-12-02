topic "Google Summer of Code 2010 Ideas";
[ $$0,0#00000000000000000000000000000000:Default]
[b117;*+117 $$1,2#27457433418004101424360058862402:Caption]
[b50;2 $$2,2#03324558446220344731010354752573:Par]
[{_}%EN-US 
[s1;#b0; [R6 Google Summer of Code 2010. Ideas]&]
[s1;#b0; &]
[s1;#b0; Introduction&]
[s0;2 &]
[s0; [2 Google Summer of Code 2010 has been announced and this year 
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
adding your personal touches.]&]
[s0;2 &]
[s0; [2 To prepare your proposal it is worth referencing a nice link 
in ][^http`:`/`/gcc`.gnu`.org`/wiki`/SummerOfCode^2 Gcc][2  from 
a how`-to in ][^http`:`/`/drupal`.org`/node`/59037^2 Drupal][2  project.]&]
[s0;2 &]
[s0; [2 This list is a work`-in`-progress. You can follow discussions 
in ][^http`:`/`/www`.ultimatepp`.org`/forums`/^2 Forum][2 .]&]
[s0;2 &]
[s1; Project Ideas&]
[s0;# &]
[s0;# [* U`+`+ port to KDE]&]
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
[s0;# [*2 We propose you this roadmap]&]
[s0;#i150;O0; [2 Learning to work in GNU/Linux U`+`+]&]
[s0;#i150;O0; [2 Analyzing Gtk`+ theming in U`+`+]&]
[s0;#i150;O0; [2 Applying the right patches to use Qt properly in KDE 
environments]&]
[s0;#2 &]
[s0;#2 &]
[s0;# [* SVG parser]&]
[s0;# &]
[s0;# [*2 Difficulty]&]
[s0;# [2 Medium.]&]
[s0;#2 &]
[s0;# [*2 Experience required]&]
[s0;# [2 U`+`+, SVG.]&]
[s0;#2 &]
[s0;# [*2 Description]&]
[s0;# [2 Scalable vector graphics (SVG) is a format that describes 
2D vectorial graphics. U`+`+ `"Painter`" package includes an 
antialiased renderer class enough versatile to implement main 
SVG requirements. However there is only a small SVG parser implementation 
included in Bazaar/PainterSvg package. The goal would be to implement 
a state of the art SVG parser.]&]
[s0;#2 &]
[s0;# [*2 We propose you this roadmap]&]
[s0;#i150;O0; [2 Reviewing PainterSvg package including U`+`+ antialiased 
rendering and Xml parser and generator]&]
[s0;#i150;O0; [2 SVG is a standard but not all the implementations 
are the same, We should have to look which are going to be our 
standards, including a set of test cases. We will define different 
project landmarks related to them.]&]
[s0;#2 &]
[s0;#2 &]
[s0;# [* Video Player Control]&]
[s0;# &]
[s0;# [*2 Difficulty]&]
[s0;# [2 Medium.]&]
[s0;#2 &]
[s0;# [*2 Skills]&]
[s0;# [2 U`+`+, ffmpeg, sdl, gcc/mingw compiling infrastructure.]&]
[s0;#2 &]
[s0;# [*2 Description]&]
[s0;# [2 This control would integrate ffmpeg video decoding and sdl 
to visualize video files.]&]
[s0;#2 &]
[s0;# [*2 We propose you this roadmap]&]
[s0;i150;O0; [2 Learning to work in U`+`+ and SDL]&]
[s0;i150;O0; [2 Learning to work with DHCtrl]&]
[s0;i150;O0; [2 Ffmpeg and ffplay are easy to compile alone. You should 
have to integrate them into a U`+`+ project. There is an excellent 
reference ][^http`:`/`/ffmpeg`.arrozcru`.org`/wiki`/index`.php`?title`=Main`_Page^2 h
ere]&]
[s0;# &]
[s0;# &]
[s0;# [* Web Browser Control]&]
[s0;# &]
[s0;# [*2 Difficulty]&]
[s0;# [2 High.]&]
[s0;#2 &]
[s0;# [*2 Experience required]&]
[s0;# [2 U`+`+, webkit, gcc/mingw compiling infrastructure.]&]
[s0;#2 &]
[s0;# [*2 Description]&]
[s0;# [2 This control would integrate a web browser into a control. 
Possibly the best option would be to use webkit.]&]
[s0;#2 &]
[s0;# [*2 We propose you this roadmap]&]
[s0;#i150;O0; [2 Learning to work in U`+`+ with DHCtrl]&]
[s0;#i150;O0; [2 Integrate Webkit in a U`+`+ project inside a DHCtrl 
derived class]&]
[s0;#2 &]
[s0;#2 &]
[s0;# [* U`+`+ as a library]&]
[s0;# &]
[s0;# [*2 Difficulty]&]
[s0;# [2 High]&]
[s0;#2 &]
[s0;# [*2 Experience required]&]
[s0;# [2 U`+`+, MSC, gcc/mingw compiling infrastructure.]&]
[s0;#2 &]
[s0;# [*2 Description]&]
[s0;# [2 U`+`+ is actually more than a C`+`+ library as it cannot be 
integrated easily in projects not developed in the U`+`+ ide 
TheIde.]&]
[s0;# [2 The goal would be to develop U`+`+ as a library using TheIde 
as a resource editor only.]&]
[s0;# [2 This would imply to create tools and processes that can be 
integrated with infrastructure server so that U`+`+ can be provided 
as pure library (or set of libraries).]&]
[s0;# [2 Create a special mode of TheIDE so that it can be used as 
resource editor only.]&]
[s0;#2 &]
[s0;# [*2 We propose you this roadmap]&]
[s0;#i150;O0; [2 Study U`+`+ TheIde compiling strategy]&]
[s0;#i150;O0; [2 Defining and developing the changes in U`+`+ to make 
projects using make or Visual Studio]&]
[s0;#i150;O0; [2 Changing TheIde to work with external projects as 
a resource editor]&]
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
[s0; ]]