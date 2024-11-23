topic "Bazaar submission and maintenance";
[a83;*R6 $$1,0#31310162474203024125188417583966:caption]
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s1; Bazaar submission and maintenance&]
[s0; [2 You have a package that could be interesting for Upp community. 
It has been made with your best skills. Code is clear and follows 
Upp style.]&]
[s0;2 &]
[s0; [2 So we can only answer: ][/2 Thank you!]&]
[s0;2 &]
[s0; [2 In this chapter you will find how to submit packages and do 
its maintenance.]&]
[s0;2%- &]
[s0;2%- &]
[ {{10000f0;g0;^t/25b4/25 [s0;# [*+117 1. Subversion Install]]}}&]
[s0;2%- &]
[s0; [2 Upp project and Bazaar packages are hosted in Upp hosting system 
and latest releases are available through ][^http`:`/`/subversion`.tigris`.org`/^2 S
ubversion][2 . This is an Open Source version control system.]&]
[s0;2 &]
[s0; [2 So first of all you will have to install a Subversion (Svn) 
client like ][^http`:`/`/tortoisesvn`.net`/downloads`.html^2 TortoiseSVN][2 , 
][^http`:`/`/www`.smartsvn`.com`/^2 SmartSVN][2 , ][^http`:`/`/www`.sliksvn`.com`/en`/download`/^2 S
likSVN][2 , ][^https`:`/`/userbase`.kde`.org`/Kdesvn^2 kdesvn ][2 or 
just a]&]
[s0;2 &]
[s0;l256; [C$7;2 -|apt`-get install subversion-|]&]
[s0;2 &]
[s0; [2 in Linux. Be sure that in Windows the svn client is in PATH 
environment variable although this is usually done by installer.]&]
[s0;2 &]
[s0;2%- &]
[ {{10000f0;g0;^t/25b4/25 [s0;# [*+117 2. Upload Permissions]]}}&]
[s0;2%- &]
[s0; [2 Well. You have svn installed so that you could get the latest 
source code from our official repository:]&]
[s0;2 &]
[s0;l256; [C$7;2 svn checkout svn://www.ultimatepp.org/upp/trunk/ my`-upp`-mirror]&]
[s0;2 &]
[s0;2 &]
[s0; [2 To get upload permissions you will have to send a post to Forum/Bazaar, 
explain as much details as possible about your project and ask 
for permission to upload it to Bazaar. ]&]
[s0;2 &]
[s0; [2 If the project is accepted (as usual), ][^http`:`/`/www`.ultimatepp`.org`/forums`/index`.php`?t`=usrinfo`&id`=3`&^2 U
pp administrator][2  will contact you and give you the address 
where you have to upload your new package, username (your Upp 
Forum nick) and password.]&]
[s0;2 &]
[s0; [2 For example, if the svn base address is]&]
[s0;l256; [C$7;2 -|svn://192.168.0.1/upp-|]&]
[s0;%- [2 you should have to upload your package to]&]
[s0;l256; [C$7;2 -|svn://192.168.0.1/upp/trunk/bazaar/My`_Cool`_Package-|]&]
[s0;2%- &]
[s0; [%-2 Please do not put spaces in your package name. ][2 You also 
should take care to not modify others packages (you may have 
write access to them too).]&]
[s0;2 &]
[s0; [2 When you commit (upload to svn server) the changes do not forget 
to enter a comment following this format:]&]
[s0;2 &]
[s0;l256;%- [%%C$7;2 -|`"][C$7;2 My`_Cool`_Package: First release`"]&]
[s0;2%- &]
[s0;%- [2 or]&]
[s0;2%- &]
[s0;l256;%- [%%C$7;2 -|`"][C$7;2 My`_Cool`_Package: Added X an Y. Fixed 
bug Z`"]&]
[s0;2%- &]
[s0;2%- &]
[s0; [2 All Bazaar folders are open so take care not modifying directly 
contents in folders that you do not manage. If you want to change 
something follow ][^topic`:`/`/uppweb`/www`/bazaar`_support`$en`-us^2 Bazaar 
Support][2  instructions.]&]
[s0;2 &]
[s0; [2 And do not put many different features into single commit. 
It is better to separate them if they do different things.]&]
[s0;2 &]
[s0;2%- &]
[ {{10000f0;g0;^t/25b4/25 [s0;# [*+117 3. Package Web Publication]]}}&]
[s0;2%- &]
[s0; [*2 3.1 Introduction]&]
[s0;2 &]
[s0; [2 You are proud with your new package, you have informed all 
in Forum/Bazaar and you have uploaded it to SVN so that anybody 
can make use of it.]&]
[s0;2 &]
[s0; [2 Now it is time to have your own page showing everybody what 
is your package for, technology used, examples and reference.]&]
[s0;2 &]
[s0; [2 It is very simple. Just follow the next instructions.]&]
[s0;2 &]
[s0;2 &]
[s0; [*2 3.2 Upp web editing]&]
[s0;2 &]
[s0;i150;O0; [2 First get the latest SVN source. It is important as 
you are going to edit the web page.]&]
[s0;2 &]
[s0;i150;O0; [2 Open uppbox/uppweb package, choose www.tpp and bazaar`$en`-us 
document.]&]
[s0;2 &]
[s0;i150;O0; [2 There simply edit `"Maintained Packages List`" adding 
or editing your project data. Use exactly the same package name 
including type case. Maintain empty `"Last Release`" and `"Size`" 
columns. This way web infrastructure will update this data for 
yourself.]&]
[s0;2 &]
[s0;i150;O0;%- [%%2 Finally upload changes to Upp by choosing ][2 Project/SVN/Synchronize
 ...upp/uppbox/uppweb]&]
[s0;2 &]
[s0;2 &]
[s0; [2 Do it carefully and fast. As this is an index page somebody 
else could be editing it at the same time and some of the changes 
could get conflicts.]&]
[s0;2 &]
[s0; [2 Please do not modify contents made by other directly. It is 
better first to recommend the changes and ask for permission.]&]
[s0;2 &]
[s0; [2 After that the day after you will see the web page updated. 
The secret of this is that Upp hosting system will compile and 
run uppweb package converting this index package and your T`+`+ 
help system into .html web pages.]&]
[s0;2 &]
[s0;2 &]
[s0; [*2 3.3 Bazaar project basic data]&]
[s0;2 &]
[s0; [2 The data to fill in `"Maintained Packages List`" is:]&]
[s0;2 &]
[s0;i150;O0; [2 Package name: As you like, but try not to include empty 
spaces. Include as hyperlink your package T`+`+ Help main page. 
This will be the first page to be linked from ][^topic`:`/`/uppweb`/www`/bazaar`$en`-us^2 B
azaar introduction page]&]
[s0;i150;O0; [2 Basic Description]&]
[s0;i150;O0; [2 Authors]&]
[s0;i150;O0; [2 Status]&]
[s0;l160;i150;O0; [2 Launch]&]
[s0;l160;i150;O0; [2 Alpha]&]
[s0;l160;i150;O0; [2 Beta]&]
[s0;l160;i150;O0; [2 Production]&]
[s0;l160;i150;O0; [2 Not Maintained]&]
[s0;i150;O0; [2 External Dependencies: Libraries or programs required 
that are external to Upp and Bazaar]&]
[s0;i150;O0; [2 Supported OS]&]
[s0;i150;O0; [2 Image: A bitmap to put in.]&]
[s0;2 &]
[s0;2 &]
[s0; [*2 3.4 Your web page]&]
[s0;2 &]
[s0; [2 Now you have the opportunity to show everybody what your package 
does. It is easy.]&]
[s0;2 &]
[s0; [2 Prepare in T`+`+ your help as usual: Reference, examples, implementation, 
summary. ]&]
[s0;2 &]
[s0; [2 Be careful that all the pages are linked between them and from 
T`+`+ Help main page. If a page is left alone it will not appear 
in web.]&]
[s0;2 &]
[s0; [2 A good main page could be the Summary page. You will have to 
care that this page is linked from ][^topic`:`/`/uppweb`/www`/bazaar`$en`-us^2 Baza
ar introduction page][2  in your Package name hyperlink.]&]
[s0;2 &]
[s0; [2 After that the day after you will see the web page updated. 
The secret of this is that Upp hosting system will compile and 
run uppweb package converting this index package and your T`+`+ 
help system into .html web pages.]&]
[s0;2 &]
[s0;2 &]
[s0; [*2 3.5 If nothing works]&]
[s0;2 &]
[s0; [2 If nothing works or you do not feel comfortable in doing this 
you can send a post to Bazaar Forum asking for help or just send 
][^http`:`/`/www`.ultimatepp`.org`/forums`/index`.php`?t`=usrinfo`&id`=648`&^2 me][2  
a post. I will do it for you.]&]
[s0;2 &]
[s0;2%- &]
[ {{10000f0;g0;^t/25b4/25 [s0;# [*+117 4. Documentation and Examples]]}}&]
[s0;2%- &]
[s0; [2 Usually we think we are good programmers and our code is just 
crystal clear so with a quick read of .h and .c/.cpp files any 
user will know how to use our precious libraries.]&]
[s0;2 &]
[s0; [2 However real world programmers that are potential users of 
our libraries will require a certain effort to prepare simple 
and clear examples and documentation.]&]
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
[s0;2%- &]
[ {{10000f0;g0;^t/25b4/25 [s0;# [*+117 5. Package Releases]]}}&]
[s0;2%- &]
[s0; [2 Do not wait too much to do new releases. Do not wait to surpass 
important landmarks to pass from 2.0 to 3.0 version... Follow 
the advice in ][^http`:`/`/en`.wikipedia`.org`/wiki`/The`_Cathedral`_and`_the`_Bazaar^2 `"
The Cathedral and the Bazaar`"][2  book:]&]
[s0;2 &]
[ {{10000<576;>768;f0;g0;^t/25b4/25@(229) [s0;= [/2 `"Release early. Release often. And listen to your customers`"]]}}&]
[s0;l288;/$7;2 &]
[s0;# [2 And remember that if your package does not have an update 
in one year it will be considered as `"Not Maintained`".]&]
[s0;#2 &]
[s0;2%- &]
[ {{10000f0;g0;^t/25b4/25 [s0;# [*+117 6. Package License]]}}&]
[s0;2%- &]
[s0; [2 All packages, documents and code in Bazaar are under ][^topic`:`/`/ide`/app`/About`$en`-us^2 N
ew BSD license][2 .]&]
[s0;2 &]
[s0;# [2 All software, documents or other media attached to the package 
like plugins should have to follow the same or a similar license. 
]&]
[s0;#2 &]
[s0;# [2 For example, Tcc package has BSD license, but attached libtcc 
code and library has LGPL license.]&]
[s0;# [2 As Tcc is a libtcc wrapper, to use libtcc it has to follow 
LGPL license. However remember that Tcc package is BSD.]&]
[s0;#2 &]
[s0;# [2 Do not forget to follow license requirements. Open Source 
and Creative Commons licenses are very easy to fulfill, but you 
will have to do it.]&]
[s0;2 &]
[s0;2%- &]
[ {{10000f0;g0;^t/25b4/25 [s0;# [*+117 7. Do not Forget]]}}&]
[s0;2%- &]
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
[s0; ]]