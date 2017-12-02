topic "U++ development cycle guide";
[ $$0,0#00000000000000000000000000000000:Default]
[a83;*R6 $$1,3#31310162474203024125188417583966:caption]
[b83;*4 $$2,3#07864147445237544204411237157677:title]
[b42;a42;ph2 $$3,3#45413000475342174754091244180557:text]
[{_}%EN-US 
[s1; U`+`+ usage and development cycle guide&]
[s2; Forum&]
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
[s2; Reporting bugs&]
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
[s2; SVN&]
[s3; U`+`+ currently uses svn as cvs. Main svn is at svn://www.ultimatepp.org/upp.&]
[s3; SVN has anonymous read`-only access, to commit you need credentials. 
U`+`+ is using quite complex per`-directory system of rights; 
rights to some directories are very restricted and to change 
the code there you need to post a patch (for code review). To 
obtain commit rights, please send forum PM message to user `'mirek`'.&]
[s3; The SVN contains following subdirectories (U`+`+ nests):&]
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
::^ [s3;b0;a0; [* bazaar]]
::= [s3;b0;a0; Community contributed packages. This gets exported into 
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
::= [s3;b0;a0; Testing area for developing new GUI backends.]
::^ [s3;b0;a0; [* uppsrc2]]
::= [s3;b0;a0; Obsoleted packages.]
::^ [s3;b0;a0; [* archive]]
::= [s3;b0;a0; Archived abandoned code.]
::^ [s3;b0;a0; [* uppdev]]
::= [s3;b0;a0; This is testing area, total mess. Best ignored.]}}&]
[s2; Redmine&]
[s3; All longer term U`+`+ issues should be tracked. We are using 
Redmine issue tracking system for this, which is [^http`:`/`/www`.ultimatepp`.org`/redmine`/projects`/upp`/issues^ h
ere].&]
[s2; Something about coding standards&]
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
[s3;l160;i150;O2; All C`+`+03 core language features are allowed, 
of course library wise U`+`+ has preference over standard library&]
[s3;l160;i150;O2; For now, U`+`+ library code should be C`+`+11 compatible, 
but C`+`+11 should not be required. Parts intended to support 
C`+`+11 features should be conditionally included using #ifdef 
CPP`_11&]
[s0;2 ]]