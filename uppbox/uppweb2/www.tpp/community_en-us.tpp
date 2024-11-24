topic "U++ development cycle guide";
[a83;*R6 $$1,3#31310162474203024125188417583966:caption]
[H4;b83;t300;*4 $$2,3#07864147445237544204411237157677:title]
[b42;a42;ph2 $$3,3#45413000475342174754091244180557:text]
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s1; U`+`+ usage and development cycle guide&]
[s2; Table of contents&]
[s0; &]
[s0; [^topic`:`/`/uppweb`/www`/community`_en`-us`#1^2 1. Forum]&]
[s0; [^topic`:`/`/uppweb`/www`/community`_en`-us`#2^2 2. Reporting 
bugs]&]
[s0; [^topic`:`/`/uppweb`/www`/community`_en`-us`#3^2 3. GIT]&]
[s0; [^topic`:`/`/uppweb`/www`/community`_en`-us`#4^2 4. Issue tracking]&]
[s0; [^topic`:`/`/uppweb`/www`/community`_en`-us`#5^2 5. Something 
about coding standards]&]
[s0; &]
[s2;:1: 1. Forum&]
[s3; After you have installed U`+`+ and tried some examples, you 
might have came to conclusion that you are going to give it a 
try. Now first thing that you should do at this moment is to 
register in the [^http`:`/`/www`.ultimatepp`.org`/forums^ Forum], 
where you will be able to post question, propose changes, report 
bugs etc. We strongly recommend to use the forum for all the 
communication, please do not try to post emails about U`+`+ features 
directly to authors. The main reason for this is that if you 
ask about your problem in the forum, the discussion will get 
recorded for future searches. Also note that that there is `"Newbie 
corner`".&]
[s3; Important note: If you feel like you are going to be involved 
in U`+`+ development, please provide an information about your 
real name (so that you can be later added to contributors lists 
without further asking etc...).&]
[s2;:2: 2. Reporting bugs&]
[s3; If you do think that you have found a problem with U`+`+ library, 
you should report it in the forum. In order to make dealing with 
the issue as efficient as possible please, following steps would 
be highly appreciated:&]
[s3;l160;i150;O2; Specify the version of U`+`+ (also, you might want 
to try svn trunk or latest nightly build to find out whether 
the issue was already fixed)&]
[s3;l160;i150;O2; Specify your host platform (OS and its version, 
compiler and possibly also something about HW).&]
[s3;l160;i150;O2; Post a [/ testcase] that can be easily compiled. 
Testcase should be the minimal source code required to reproduce 
the problem. Usually, it takes a form of package, compressed 
as .zip and posted as attachment in the forum. Ideally, the name 
of package should start with your name, followed by `'`_`' and 
something that makes its name unique, like `'mirek`_bug25`'. 
The reason for this naming convention is so that it does not 
name`-clash with other testcases.&]
[s3;l160;i150;O2; Describe a way how to reproduce a problem&]
[s3; After posting a bug report, please let us time to process it. 
Serious bugs are usually fixed in days, but sometimes less serious 
issue can take weeks. If problem cannot be fixed immediately, 
it is usually moved to redmine issue tracking system. You can 
check the what issues are currently open [^http`:`/`/www`.ultimatepp`.org`/redmine`/projects`/upp`/issues^ h
ere].&]
[s2;:3: 3. GIT&]
[s3; U`+`+ currently uses git as their main version control tool. 
Main git repository are located under [^https`:`/`/github`.com`/ultimatepp`/ultimatepp^ h
ttps://github.com/ultimatepp/ultimatepp]. You could learn more 
about U`+`+ framework development with git from following documentation 
[^topic`:`/`/uppweb`/www`/GITRepository`_en`-us^ site].&]
[s3; The GIT contains following sub`-directories (U`+`+ nests):&]
[s0;2 &]
[ {{1995:8005^ [s3;b0;a0; [* uppsrc]]
::= [s3;b0;a0; This contains `"canonical`" U`+`+ library packages, albeit 
not all packages of this nest are exported to U`+`+ releases 
at this time (sort of flaw).]
::^ [s3;b0;a0; [* reference]]
::= [s3;b0;a0; Reference examples. These examples are meant as sort of 
`"documentation by example`". This gets exported into U`+`+ releases.]
::^ [s3;b0;a0; [* examples]]
::= [s3;b0;a0; Demonstration examples. This gets exported into U`+`+ 
releases.]
::^ [s3;b0;a0; [* tutorial]]
::= [s3;b0;a0; Companion examples for tutorials. This gets exported into 
U`+`+ releases.]
::^ [s3;b0;a0; [* upptst]]
::= [s3;b0;a0; Manual tests.]
::^ [s3;b0;a0; [* autotest]]
::= [s3;b0;a0; Automated tests.]
::^ [s3;b0;a0; [* benchmarks]]
::= [s3;b0;a0; Benchmarking code.]
::^ [s3;b0;a0; [* autotst]]
::= [s3;b0;a0; Special packages to test the autotest system.]
::^ [s3;b0;a0; [* rainbow]]
::= [s3;b0;a0; Testing area for developing new GUI backends.]}}&]
[s0; &]
[s2;:4: 4. Issue tracking&]
[s3; All longer term U`+`+ issues should be tracked. We are using 
[* Redmine] issue tracking system for this, which is [^http`:`/`/www`.ultimatepp`.org`/redmine`/projects`/upp`/issues^ h
ere].&]
[s2;:5: 5. Something about coding standards&]
[s3; General, I believe that there will be a little problems with 
coding standard, as the reason that made you use U`+`+ was probably 
its way of doing things in the first place. However, to recapitulate:&]
[s3;l160;i150;O2; Using [*@5 delete] is a big no`-no (of course there 
might be exception).&]
[s3;l160;i150;O2; You can generally avoid [*@5 new] in U`+`+, so think 
twice before using it&]
[s3;l160;i150;O2; Generally, classes should have default constructor 
`- that is required to make in`-place construction possible&]
[s3;l160;i150;O2; Please do not propose adding/using shared (reference 
counted) smart pointers. We have worked hard to make possible 
to avoid them :)&]
[s3;l160;i150;O2; Use tabs to provide block indents, tabsize is 4 
characters&]
[s3;l160;i150;O2; Use spaces for non`-block indents, e.g. to indent 
second line of long if condition&]
[s3;l160;i150;O2; There are no recommendation about C`+`+ formatting, 
put spaces and place `{ as you please&]
[s3;l160;i150;O2; All C`+`+14 core language features are allowed 
and it is minimal standard to compile U`+`+&]
[s3;l160;i150;O2; For now, U`+`+ library code is C`+`+20 compatible, 
but C`+`+20 is not required. Parts intended to support C`+`+20 
features should be conditionally included using #ifdef CPP`_20&]
[s3;l160;i150;O2; Prefer U`+`+ features over equivalents in the standard 
library for example use Upp`::Index instead of std`::set&]
[s0;2 ]]