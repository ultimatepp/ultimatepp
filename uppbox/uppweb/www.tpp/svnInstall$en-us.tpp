topic "Subversion Install and Permissions";
[ $$0,0#00000000000000000000000000000000:Default]
[a83;*R6 $$1,2#31310162474203024125188417583966:caption]
[b42;a42;2 $$2,2#45413000475342174754091244180557:text]
[{_}%EN-US 
[s1;a0; Subversion Install and Permissions&]
[s2;>b0;a83;*R^topic`:`/`/uppweb`/www`/comparison`$ru`-ru^1 &]
[s0; [*+117 Subversion Install]&]
[s0;*2 &]
[s0; [2 Upp project and Bazaar packages are hosted in Upp hosting system 
and latest releases are available through ][^http`:`/`/subversion`.tigris`.org`/^2 S
ubversion][2 . This is an Open Source version control system.]&]
[s0;2 &]
[s0; [2 So first of all you will have to install a Subversion (Svn) 
client like TortoiseSVN, SmartSVN, SlikSVN, kdesvn or just a 
`"apt`-get install subversion`" in Linux. Be sure that in Windows 
the svn client is in PATH environment variable although this 
is usually done by installer.]&]
[s0;2 &]
[s0;%- &]
[s0; [*+117 Upload Permissions]&]
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
[s0;%- [%%2 For example, if the svn base address is `"][2 svn://192.168.0.1/upp`", 
you should have to upload your package to `"svn://192.168.0.1/upp/trunk/bazaar/My`_
Cool`_Package`". ]&]
[s0;2%- &]
[s0; [%-2 Please do not put spaces in your package name. ][2 You also 
should take care to not modify others packages (you may have 
write access to them too).]&]
[s0;2 &]
[s0; [2 When you commit (upload to svn server) the changes do not forget 
to enter a comment following this format:]&]
[s0;2 &]
[s0;l256;%- [%%C$7;2 `"][C$7;2 My`_Cool`_Package: First release`"]&]
[s0;2%- &]
[s0;%- [2 or]&]
[s0;2%- &]
[s0;l256;%- [%%C$7;2 `"][C$7;2 My`_Cool`_Package: Added X an Y. Fixed 
bug Z`"]&]
[s0;2%- &]
[s0;2%- &]
[s0;%- [2 The ][^http`:`/`/code`.google`.com`/p`/upp`-mirror`/source`/list^2 http://code.g
oogle.com/p/upp`-mirror/source/list][2  will mirror it within an 
hour. Then you will see your new release in Bazaar, congratulations, 
your first release is done!]&]
[s0;2 &]
[s0; [2 And do not put many different features into single commit. 
It is better to separate them if they do different things.]&]
[s0; ]