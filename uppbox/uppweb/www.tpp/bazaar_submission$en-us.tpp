topic "Bazaar submission and maintenance";
[ $$0,0#00000000000000000000000000000000:Default]
[a83;*R6 $$1,0#31310162474203024125188417583966:caption]
[{_}%EN-US 
[s1; Bazaar submission and maintenance&]
[s0; [2 You have a package that consider could be interesting for Upp 
community. It has been made with your best skills. Code is clear 
and follows Upp style.]&]
[s0;2 &]
[s0; [2 So we can only answer ][/2 Thank you!][2 . ]&]
[s0;2 &]
[s0; [2 In this chapter you will find how to submit packages and do 
its maintenance.]&]
[s0;2 &]
[s0;%- &]
[ {{10000@(190.255.220) [s0; [*2 1. SUBVERSION INSTALL]]}}&]
[s0;*2 &]
[s0; [2 Upp project and Bazaar packages are hosted in Upp hosting system 
and latest releases are available through ][^http`:`/`/subversion`.tigris`.org`/^2 S
ubversion][2 . This is an Open Source version control system.]&]
[s0;2 &]
[s0; [2 So first of all you will have to install a Subversion (Svn) 
client like TortoiseSVN, SmartSVN, SlikSVN or just a `"apt`-get 
install subversion`" in Linux. Be sure that in Windows the svn 
client is in PATH environment variable although this is usually 
done by installer.]&]
[s0;2 &]
[s0;%- &]
[ {{10000@(190.255.220) [s0; [*2 2. UPLOAD PERMISSIONS]]}}&]
[s0;*2 &]
[s0; [2 Well. You have svn installed so that you could get the latest 
source code from Google Code mirror with just this command line: 
]&]
[s0;2 &]
[s0;l256; [C$7;2 svn checkout http://upp`-mirror.googlecode.com/svn/trunk/ 
my`-upp`-mirror]&]
[s0;2 &]
[s0;2 &]
[s0; [2 To get upload permissions you will have to send a post to Forum/Bazaar, 
explain as much details as possible about your project and ask 
for permission to upload it to Bazaar. ]&]
[s0;2 &]
[s0; [2 If the project is accepted (as usual), ][^http`:`/`/www`.ultimatepp`.org`/forum`/index`.php`?t`=usrinfo`&id`=3`&^2 U
pp administrator][2  will contact you and give you the address 
where you have to upload your new package, username (your Upp 
Forum nick) and password.]&]
[s0;2 &]
[s0;%- [%%2 For example, if the svn base address is `"][2 svn://90.176.237.17/upp`", 
you should have to upload your package to `"svn://90.176.237.17/upp/trunk/bazaar/M
y`_Cool`_Package`". ]&]
[s0;2%- &]
[s0; [%-2 Please do not put spaces in your package name. ][2 You also 
should take care to not modify others packages (you may have 
write access to them too).]&]
[s0;2 &]
[s0; [2 When you commit the changes do not forget to enter a comment 
following this format:]&]
[s0;2 &]
[s0;l256;%- [%%C$7;2 `"][C$7;2 My`_Cool`_Package: First release`"]&]
[s0;2%- &]
[s0;%- [2 or]&]
[s0;2%- &]
[s0;l256;%- [%%C$7;2 `"][C$7;2 My`_Cool`_Package: Added X an Y. Fixed 
bug Z`"]&]
[s0;2%- &]
[s0;2%- &]
[s0;%- [2 If in some hours you enter in ][^http`:`/`/code`.google`.com`/p`/upp`-mirror`/source`/list^2 h
ttp://code.google.com/p/upp`-mirror/source/list][2  you will see 
your new release into Bazaar so, congratulations, your first 
release is done!.]&]
[s0;2 &]
[s0;%- &]
[ {{10000@(190.255.220) [s0; [*2 3. PACKAGE WEB PUBLICATION]]}}&]
[s0;2 &]
[s0; [2 You are proud with your new package, you have informed all 
in Forum/Bazaar and you have uploaded it to svn so that anybody 
can make use of it.]&]
[s0;2 &]
[s0; [2 Now it is time to have your own page showing everybody what 
is your package for, technology used, examples and reference.]&]
[s0;2 &]
[s0; [2 It is very simple. Just send a post (private or public) to 
][^http`:`/`/www`.ultimatepp`.org`/forum`/index`.php`?t`=usrinfo`&id`=648`&^2 Bazaa
r page maintainer][2  including this information:]&]
[s0;2 &]
[s0;i150;O0; [2 Package name]&]
[s0;i150;O0; [2 Basic Description]&]
[s0;i150;O0; [2 Authors]&]
[s0;i150;O0; [2 Status]&]
[s0;l160;i150;O0; [2 Launch]&]
[s0;l160;i150;O0; [2 Alpha]&]
[s0;l160;i150;O0; [2 Beta]&]
[s0;l160;i150;O0; [2 Production]&]
[s0;l160;i150;O0; [2 Not Maintained]&]
[s0;i150;O0; [2 External Dependencies: Libraries required that are external 
to Upp and Bazaar]&]
[s0;i150;O0; [2 Supported OS]&]
[s0;i150;O0; [2 Image: A bitmap to put in ][^topic`:`/`/uppweb`/www`/bazaar`_`$en`-us^2 Ba
zaar introduction page]&]
[s0;i150;O0; [2 T`+`+ Help main page: This will be the first page to 
be linked from ][^topic`:`/`/uppweb`/www`/bazaar`_`$en`-us^2 Bazaar 
introduction page]&]
[s0;2 &]
[s0; [2 It is important to have a clear introduction page with links 
to the documentation and that will be linked from ][^topic`:`/`/uppweb`/www`/bazaar`_`$en`-us^2 B
azaar introduction page.]&]
[s0;2 &]
[s0; [2 In few days (usually two), the ][^topic`:`/`/uppweb`/www`/bazaar`_`$en`-us^2 Baza
ar introduction page][2  will be updated so your T`+`+ help will 
be available for the web. Upp hosting system will convert your 
T`+`+ help system into .html web pages.]&]
[s0;2 &]
[s0;%- &]
[ {{10000@(190.255.220) [s0; [*2 4. DOCUMENTATION AND EXAMPLES]]}}&]
[s0;2 &]
[s0; [2 Usually we think we are good programmers and our code is just 
crystal clear so with a quick read of .h and .c/.cpp files any 
user will know how to use our precious libraries.]&]
[s0;2 &]
[s0; [2 However real world programmers will require a certain effort 
to prepare simple and clear examples and documentation.]&]
[s0;2 &]
[s0; [2 This is worthwhile. The examples let the user to fast test 
your code and understand basic use. In the other side a classes 
and functions reference will help the user to get many of your 
code performance.]&]
[s0;2 &]
[s0; [2 So do not be shy. Think in your users and invest in T`+`+ help 
!. Remember that it will be available for everybody in this web 
page.]&]
[s0;2 &]
[s0;%- &]
[ {{10000@(190.255.220) [s0; [*2 5. PACKAGE RELEASES]]}}&]
[s0;2 &]
[s0; [2 Do not wait too much to do new releases. Do not wait to surpass 
important landmarks to pass from 2.0 to 3.0 version... Follow 
the advice in ][^http`:`/`/en`.wikipedia`.org`/wiki`/The`_Cathedral`_and`_the`_Bazaar^2 `"
The Cathedral and the Bazaar`"][2  book:]&]
[s0;2 &]
[s0;l288; [/$7;2 `"Release early. Release often. And listen to your 
customers`"]&]
[s0;2 &]
[s0;%- &]
[ {{10000@(190.255.220) [s0; [*2 6. PACKAGE LICENSE]]}}&]
[s0;2 &]
[s0; [2 All packages, documents and code in Bazaar are under ][^topic`:`/`/ide`/app`/About`$en`-us^2 B
SD license][2 .]&]
[s0;2 &]
[s0; [2 All software, documents or other media submitted to svn package 
should have to follow the same or a similar license.]&]
[s0;2 &]
[s0; [2 Do not forget to follow license requirements. Open Source and 
Creative Commons licenses are very easy to fulfill, but you will 
have to do it.]&]
[s0;2 &]
[s0;%- &]
[ {{10000@(190.255.220) [s0; [*2 7. DO NOT FORGET]]}}&]
[s0;2 &]
[s0; [2 A good package has to be available and maintained, so if:]&]
[s0;i150;O0; [2 You are not going to get package goals in finite time]&]
[s0;i150;O0; [2 You are tired]&]
[s0;i150;O0; [2 You are busy]&]
[s0;2 &]
[s0; [2 ...just ask for help. Post it in Forum/Bazaar, label your package 
`"Author`" with a `"Help Required`" or `"Status`" with a `"Not 
Maintained`". This will let other people the opportunity to help 
you or to get your relay run baton.]&]
[s0;2 &]
[s0; [2 And the community will acknowledge it.]&]
[s0; &]
[s0;%- &]
[ {{10000@(190.255.220) [s0; [2 Last edit: 2010`-01`-16]]}}]