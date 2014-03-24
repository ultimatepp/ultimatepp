topic "U++ development cycle guide";
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s0; U`+`+ usage and development cycle guide&]
[s0; &]
[s0; Forum&]
[s0; After you have installed U`+`+ and tried some examplas, you 
might have came to conclusion that you are going to give it a 
try. Now first thing that you should do at this moment is to 
register in the [^http`:`/`/www`.ultimatepp`.org`/forum^ Forum], 
where you will be able to post question, propose changes, report 
bugs etc. We strongly recommend to use the forum for all the 
communication, please do not try to post emails about U`+`+ features 
directly to authors. The main reason for this is that if you 
ask about your problem in the forum, the discussion will get 
recorded for future searches. Also note that that there is `"Newbie 
corner`".&]
[s0; Important note: If you feel like you are going to be involved 
in U`+`+ development, please provide an information about your 
real name (so that you can be later added to contributors lists 
without furthe asking etc...).&]
[s0; &]
[s0; &]
[s0; Reporting bugs&]
[s0; If you do think that you have found a problem with U`+`+ library, 
you should report it in the forum. In order to make dealing with 
the issue as efficient as possible please, following steps would 
be highly appreciated:&]
[s0; &]
[s0; `- specify the version of U`+`+ (also, you might want to try 
svn trunk or latest nightly build to find out whether the issue 
was alredy fixed)&]
[s0; `- specify your host platform (OS and its version, compiler 
and itspossibly also something about HW).&]
[s0; `- post a [/ testcase] that can be easily compiled. Testcase should 
be the minimal source code required to reproduce the problem. 
Usually, it takes a form of package, compressed as .zip and posted 
as attachement in the forum. Ideally, the name of package should 
start with your name, followed by `'`_`' and something that makes 
its name unque, like `'mirek`_bug25`'. You can also consider 
to add package description. The reason for this naming convention 
is so that it does not nameclash with other testcases.&]
[s0; `- describe a way how to reproduce a problem&]
[s0; &]
[s0; After posting a bug report, please let us time to process it. 
Serious bugs are usually fixed in days, but sometimes less serious 
issue can take weeks. If problem cannot be fixed immediately, 
it is usually moved to redmine issue tracking system. You can 
check the what issues are currently open here.&]
[s0; &]
[s0; &]
[s0; Getting involved (deeper)&]
[s0; SVN&]
[s0; &]
[s0; REDMINE&]
[s0; &]
[s0; Something about coding standards&]
[s0; I general, I believe that there will be a little problems with 
coding standard, as the reason that made you use U`+`+ was probably 
its way of doing things in the first place. However, to recapitulate:&]
[s0; &]
[s0; `- using `'delete`' is a big no`-no (of course there might be 
exception).&]
[s0; `- you can generally avoid `'new`' in U`+`+, so think twice 
before using it&]
[s0; `- generally, classes should have default constructor `- that 
is required to make in`-place construction possible&]
[s0; `- please do not propose adding/using shared (reference counted) 
smart pointers. We have worked hard to make possible to avoid 
them :)&]
[s0; `- use tabs to provide block indents, tabsize is 4 characters&]
[s0; `- use spaces for non`-block indents, e.g. to indent second 
line of long if condition&]
[s0; `- there are no recommendation about C`+`+ formating, put places 
and place `{ as you please&]
[s0; `- All C`+`+03 core language features are allowed, of course 
library wise U`+`+ Core has preference&]
[s0; `- For now, U`+`+ library code should be C`+`+11 compatible, 
but C`+`+11 should not be required. Parts intended to support 
C`+`+11 features should be condionally included using #ifdef 
CPP`_11]]