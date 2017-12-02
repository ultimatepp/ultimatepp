topic "UnitTest++ in brief";
[2 $$0,0#00000000000000000000000000000000:Default]
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[H4;b83;*4 $$3,5#07864147445237544204411237157677:title]
[i288;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;2 $$5,5#45413000475342174754091244180557:text]
[l288;b17;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;C@5;1 $$7,7#20902679421464641399138805415013:code]
[b2503;2 $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base]
[C2 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam]
[b167;C2 $$13,13#92430459443460461911108080531343:item1]
[i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128)2 $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7;2 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line]
[l160;*C+117 $$18,5#88603949442205825958800053222425:package`-title]
[2 $$19,0#53580023442335529039900623488521:gap]
[C2 $$20,20#70211524482531209251820423858195:class`-nested]
[b50;2 $$21,21#03324558446220344731010354752573:Par]
[b83;a83;*R+117 $$22,22#15150040393549804420528261439571:caption2]
[{_}%EN-US 
[s2; UnitTest`+`+ in brief&]
[s0; &]
[s3; Introduction&]
[s21; For background, goals and license details, see:&]
[s21;i150;O0; [^http`:`/`/unittest`-cpp`.sourceforge`.net`/^ The UnitTest`+`+ 
home page]&]
[s0;i150;O0; [^http`:`/`/www`.gamesfromwithin`.com`/articles`/0603`/000108`.html^ Noel 
Llopis`' announcement]&]
[s21; The documentation, while sparse, aims to be practical, so it 
should give you enough info to get started using UnitTest`+`+ 
as fast as possible.&]
[s21; This text is almost identical copy of original [C docs/UnitTest`+`+.html] 
file made by Noel Llopis, I (Peter `"Ped`" Helcmanovsky) made 
only some U`+`+ related changes to it `- to make it more relevant 
for U`+`+ users.&]
[s0; &]
[s3; U`+`+ wrapper&]
[s5; The U`+`+ version of UnitTest`+`+ consists of two packages:&]
[s5;i150;O0; [* UnitTest`+`+] is the actual unit testing library. This 
is the package which you should add to your current project, 
if you want to use unit testing.&]
[s5;i150;O0; [* UnitTestTest ]is the package containing tests of UnitTest`+`+. 
This is standalone runnable package which was used to develop 
the UnitTest`+`+ (and verify U`+`+ wrapper). Use this one to 
test the main package works as expected, when you do custom changes 
to main library or just read the source to get some inspiration 
how the library can be used.&]
[s3; Using UnitTest`+`+&]
[s5; The U`+`+ version of UnitTest`+`+ comes with UnitTestTest package, 
which is a full test suite using UnitTest`+`+. This is a great 
place to learn techniques for testing. There is one sample .cpp 
file: [C^http`:`/`/code`.google`.com`/p`/upp`-mirror`/source`/browse`/trunk`/bazaar`/UnitTestTest`/TestUnitTest`%2B`%2B`.cpp^ U
nitTestTest/TestUnitTest`+`+.cpp]. It covers most of UnitTest`+`+`'s 
features in an easy`-to`-grasp context, so start there if you 
want a quick overview of typical usage.&]
[s22; Getting started&]
[s0; Create new package based on [C Core console project] (in an assembly 
where bazaar package nest is defined). Add bazaar package UnitTest`+`+ 
to it. Now adjust the cpp file to look like this:&]
[s0; &]
[s7; //some`_test.cpp&]
[s7; #include <Core/Core.h>&]
[s7; #include <UnitTest`+`+/UnitTest`+`+.h>&]
[s7; &]
[s7; TEST(FailSpectacularly)&]
[s7; `{&]
[s7; -|CHECK(false);&]
[s7; `}&]
[s7; &]
[s7; CONSOLE`_APP`_MAIN&]
[s7; `{&]
[s7; -|UPP`::SetExitCode( UnitTest`::RunAllTests() );&]
[s7; `}&]
[s21; [C UnitTest`+`+.h] is a facade header for UnitTest`+`+, so including 
that should get you all features of the library. All classes 
and free functions are placed in namespace [C UnitTest], so you 
need to either qualify their full names (as with [C RunAllTests()] 
in the example) or add a [C using namespace UnitTest;] statement 
in your .cpp files. Note that any mention of UnitTest`+`+ functions 
and classes in this document assume that the [C UnitTest] namespace 
has been opened.&]
[s0; &]
[s0; Building this application and running it should produce the 
following output (details may vary):&]
[s0; &]
[s7; .`\some`_test.cpp(5): error: Failure in FailSpectacularly: false&]
[s7; FAILED: 1 out of 1 tests failed (1 failures).&]
[s7; Test time: 0.00 seconds.&]
[s0; &]
[s0; UnitTest`+`+ attempts to report every failure in an IDE`-friendly 
format, depending on platform (e.g. you can double`-click it 
in Visual Studio`'s error list.) The exit code will be the number 
of failed tests, so that a failed test run always returns a non`-zero 
exit code.&]
[s22; Test macros&]
[s0; To add a test, simply put the following code in a .cpp file 
of your choice:&]
[s0; &]
[s7; TEST(YourTestName)&]
[s7; `{&]
[s7; `}&]
[s0; &]
[s0; The [C TEST] macro contains enough machinery to turn this slightly 
odd`-looking syntax into legal C`+`+, and automatically register 
the test in a global list. This test list forms the basis of 
what is executed by [C RunAllTests()].&]
[s0; &]
[s0; If you want to re`-use a set of test data for more than one 
test, or provide setup/teardown for tests, you can use the [C TEST`_FIXTURE] 
macro instead. The macro requires that you pass it a class name 
that it will instantiate, so any setup and teardown code should 
be in its constructor and destructor.&]
[s0; &]
[s7; struct SomeFixture&]
[s7; `{&]
[s7; -|SomeFixture() `{ /`* some setup `*/ `}&]
[s7; -|`~SomeFixture() `{ /`* some teardown `*/ `}&]
[s7; &]
[s7; -|int testData;&]
[s7; `};&]
[s7; &]
[s7; TEST`_FIXTURE(SomeFixture, YourTestName)&]
[s7; `{&]
[s7; -|int temp `= testData;&]
[s7; `}&]
[s0; &]
[s0; Note how members of the fixture are used as if they are a part 
of the test, since the macro`-generated test class derives from 
the provided fixture class.&]
[s22; Suite macros&]
[s0; Tests can be grouped into suites, using the [C SUITE] macro. A 
suite serves as a namespace for test names, so that the same 
test name can be used in two difference contexts.&]
[s0; &]
[s7; SUITE(YourSuiteName)&]
[s7; `{&]
[s7; -|TEST(YourTestName)&]
[s7; -|`{&]
[s7; -|`}&]
[s7; &]
[s7; -|TEST(YourOtherTestName)&]
[s7; -|`{&]
[s7; -|`}&]
[s7; `}&]
[s0; &]
[s0; This will place the tests into a C`+`+ namespace called [C YourSuiteName], 
and make the suite name available to UnitTest`+`+. [C RunAllTests()] 
can be called for a specific suite name, so you can use this 
to build named groups of tests to be run together.&]
[s22; Simple check macros&]
[s0; In test cases, we want to check the results of our system under 
test. UnitTest`+`+ provides a number of check macros that handle 
comparison and proper failure reporting.&]
[s0; &]
[s0; The most basic variety is the boolean [C CHECK] macro:&]
[s0; &]
[s7; CHECK(false); // fails&]
[s0; &]
[s0; It will fail if the boolean expression evaluates to false.&]
[s0; &]
[s0; For equality checks, it`'s generally better to use [C CHECK`_EQUAL]:&]
[s0; &]
[s7; CHECK`_EQUAL(10, 20); // fails&]
[s7; CHECK`_EQUAL(`"foo`", `"bar`"); // fails&]
[s0; &]
[s0; Note how [C CHECK`_EQUAL] is overloaded for C strings, so you 
don`'t have to resort to [C strcmp] or similar. There is no facility 
for case`-insensitive comparison or string searches, so you may 
have to drop down to a plain boolean [C CHECK] with help from the 
CRT:&]
[s0; &]
[s7; CHECK(std`::strstr(`"zaza`", `"az`") !`= 0); // succeeds&]
[s0; &]
[s0; For floating`-point comparison, equality isn`'t necessarily 
well`-defined, so you should prefer the [C CHECK`_CLOSE] macro:&]
[s0; &]
[s7; CHECK`_CLOSE(3.14, 3.1415, 0.01); // succeeds&]
[s0; &]
[s0; All of the macros are tailored to avoid unintended side`-effects, 
for example:&]
[s0; &]
[s7; TEST(CheckMacrosHaveNoSideEffects)&]
[s7; `{&]
[s7; -|int i `= 4;&]
[s7; -|CHECK`_EQUAL(5, `+`+i); // succeeds&]
[s7; -|CHECK`_EQUAL(5, i); // succeeds&]
[s7; `}&]
[s0; &]
[s0; The check macros guarantee that the [C `+`+i] expression isn`'t 
repeated internally, as demonstrated above.&]
[s22; Array check macros&]
[s0; There is a set of check macros for array comparison as well:&]
[s0; &]
[s7; const float oned`[2`] `= `{ 10, 20 `};&]
[s7; CHECK`_ARRAY`_EQUAL(oned, oned, 2); // succeeds&]
[s7; CHECK`_ARRAY`_CLOSE(oned, oned, 2, 0.00); // succeeds&]
[s7; &]
[s7; const float twod`[2`]`[3`] `= `{ `{0, 1, 2`}, `{2, 3, 4`} `};&]
[s7; CHECK`_ARRAY2D`_CLOSE(twod, twod, 2, 3, 0.00); // succeeds&]
[s0; &]
[s0; The array equal macro compares elements using [C operator`=`=], 
so [C CHECK`_ARRAY`_EQUAL] won`'t work for an array of C strings, 
for example.&]
[s0; &]
[s0; The array close macros are similar to the regular [C CHECK`_CLOSE] 
macro, and are really only useful for scalar types, that can 
be compared in terms of a difference between two array elements.&]
[s0; &]
[s0; Note that the one`-dimensional array macros work for [C std`::vector] 
as well, as it can be indexed just as a C array.&]
[s22; Exception check macros&]
[s0; Finally, there`'s a [C CHECK`_THROW] macro, which asserts that 
its enclosed expression throws the specified type:&]
[s0; &]
[s7; struct TestException `{`};&]
[s7; CHECK`_THROW(throw TestException(), TestException); // succeeds&]
[s0; &]
[s0; UnitTest`+`+ natively catches exceptions if your test code doesn`'t. 
So if your code under test throws any exception UnitTest`+`+ 
will fail the test and report either using the [C what()] method 
for [C std`::exception] derivatives or just a plain message for 
unknown exception types.&]
[s0; &]
[s0; Should your test or code raise an irrecoverable error (an Access 
Violation on Win32, for example, or a signal on Linux), UnitTest`+`+ 
will attempt to map them to an exception and fail the test, just 
as for other unhandled exceptions.&]
[s22; Time constraints&]
[s0; UnitTest`+`+ can fail a test if it takes too long to complete, 
using so`-called time constraints.&]
[s0; &]
[s0; They come in two flavors; local and global time constraints.&]
[s0; &]
[s0; Local time constraints are limited to the current scope, like 
so:&]
[s0; &]
[s7; TEST(YourTimedTest)&]
[s7; `{&]
[s7; -|// Lengthy setup...&]
[s7; &]
[s7; -|`{&]
[s7; -|-|UNITTEST`_TIME`_CONSTRAINT(50);&]
[s7; &]
[s7; -|-|// Do time`-critical stuff&]
[s7; -|`}&]
[s7; &]
[s7; -|// Lengthy teardown...&]
[s7; `}&]
[s0; &]
[s0; The test will fail if the `"Do time`-critical stuff`" block 
takes longer than 50 ms to complete. The time`-consuming setup 
and teardown are not measured, since the time constraint is scope`-bound. 
It`'s perfectly valid to have multiple local time constraints 
in the same test, as long as there is only one per block.&]
[s0; &]
[s0; A global time constraint, on the other hand, requires that all 
of the tests in a test run are faster than a specified amount 
of time. This allows you, when you run a suite of tests, to ask 
UnitTest`+`+ to fail it entirely if any test exceeds the global 
constraint. The max time is passed as a parameter to an overload 
of [C RunAllTests()].&]
[s0; &]
[s0; If you want to use a global time constraint, but have one test 
that is notoriously slow, you can exempt it from inspection by 
using the [C UNITTEST`_TIME`_CONSTRAINT`_EXEMPT] macro anywhere 
inside the test body.&]
[s0; &]
[s7; TEST(NotoriouslySlowTest)&]
[s7; `{&]
[s7; -|UNITTEST`_TIME`_CONSTRAINT`_EXEMPT();&]
[s7; &]
[s7; -|// Oh boy, this is going to take a while&]
[s7; -|...&]
[s7; `}&]
[s22; Test runners&]
[s0; The [C RunAllTests()] function has an overload that lets you customize 
the behavior of the runner, such as global time constraints, 
custom reporters, which suite to run, etc.&]
[s0; &]
[s7; int RunAllTests(TestReporter`& reporter, TestList const`& list, 
char const`* suiteName, int const maxTestTimeInMs);&]
[s0; &]
[s0; If you attempt to pass custom parameters to [C RunAllTests()], 
note that the list parameter should have the value [C Test`::GetTestList()].&]
[s0; &]
[s0; The parameterless [C RunAllTests()] is a simple wrapper for this 
one, with sensible defaults.&]
[s22; Example setup&]
[s0; How to create a new test project varies depending on your environment, 
but here are some directions on common file structure and usage.&]
[s0; &]
[s0; The general idea is that you keep one Main.cpp file with the 
entry`-point which calls RunAllTests().&]
[s0; &]
[s0; Then you can simply compile and link new .cpp files at will, 
typically one per test suite.&]
[s0; &]
[s0;    `+ ShaverTests/&]
[s0;    `|&]
[s0;    `+`- Main.cpp&]
[s0;    `|&]
[s0;    `+`- TestBrush.cpp   &]
[s0;    `+`- TestEngine.cpp&]
[s0;    `+`- TestRazor.cpp   &]
[s0; &]
[s0; Each of the Test`*.cpp files will contain one or more TEST macro 
incantations with the associated test code. There are no source`-level 
dependencies between Main.cpp and Test`*.cpp, as the TEST macro 
handles the registration and setup necessary for RunAllTests() 
to find all tests compiled into the same final executable.&]
[s0; &]
[s0; UnitTest`+`+ does not require this structure, even if this is 
how the library itself does it. As long as your test project 
contains one or more TESTs and calls RunAllTests() at one point 
or another, it will be handled by UnitTest`+`+.&]
[s0; &]
[s0; It`'s common to make the generated executable start as a post`-build 
step, so that merely building your test project will run the 
tests as well. Since the exit code is the count of failures, 
a failed test will generally break the build, as most build engines 
will fail a build if any step returns a non`-zero exit code.&]
[s0; &]
[s3; Best practices to use UnitTest`+`+ together with U`+`+&]
[s5; For a complex modular application using several packages as 
modules, I`'m following these patterns.&]
[s22; Packages organization&]
[s0; Each production package (module) has `"test`" package counterpart, 
for example with package [C MainAccounting] I do create package 
[C MainAccountingTests] as well. The test package contains at least 
one cpp file, for example:&]
[s7; //MainAccountingTests.cpp&]
[s7; #include <MainAccounting/MainAccounting.h>-|//MainAccounting 
package API definition&]
[s7; #include <UnitTest`+`+/UnitTest`+`+.h>&]
[s7; &]
[s7; void MainAccountingTestsCPP() `{`}-|//linking hack, each new 
CPP test file has it&]
[s7; &]
[s7; SUITE(MainAccounting`_tests) `{&]
[s7; -|TEST(MainAccounting`_Instantiate) `{&]
[s7; -|-|MainAccounting macc;&]
[s7; -|`}&]
[s7; `}&]
[s0; The test package also does add UnitTest`+`+ package from bazaar, 
and original MainAccounting package, and ideally nothing else. 
Because other non test packages should be already added in MainAccounting 
package.&]
[s0; &]
[s0; Then I do have a ProjectTestsAll package, where I add all the 
module`-test packages, Core package and UnitTest`+`+ package. 
This package contains ordinary console application [C main]:&]
[s7; //ProjectTestsAll.cpp&]
[s7; #include <Core/Core.h>&]
[s7; #include <UnitTest`+`+/UnitTest`+`+.h>&]
[s7; #define RUN`_IN`_RELEASE(x) extern void x(); x()&]
[s7; &]
[s7; CONSOLE`_APP`_MAIN&]
[s7; `{&]
[s7; -|//HACK: force all CPP files with tests to link and run tests 
in Release compilation mode.&]
[s7; -|RUN`_IN`_RELEASE( MainAccountingTestsCPP );&]
[s7; &]
[s7; -|UPP`::SetExitCode( UnitTest`::RunAllTests() );&]
[s7; `}&]
[s0; &]
[s0; There`'s a dummy empty function in each [C tests.cpp] file, because 
otherwise the tests are lost in release mode linking (in debug 
mode it works without the hack).&]
[s0; &]
[s22; Where is the main&]
[s0; The `"[C main]`" running tests is in the above mentioned piece 
of code, in ProjectTestsAll package. What about the main of the 
application itself?&]
[s0; &]
[s0; If you can have a single `"ProjectApp`" package adding all other 
modules (packages) and containing just a bare minimum of code 
to run the app, then by omitting this package from TestsAll you 
avoid any linking conflicts with multiple main entry points.&]
[s0; &]
[s0; If your main package is larger, and does contain also functions 
you want to test, you should add a `"[C UNITTEST]`" flag into main 
configuration of TestsAll package (Project/Main package configuration, 
edit the main line, or add first if none exists), then in the 
main package enclose the real application `"main`" inside [C #ifndef 
flagUNITTEST] / [C #endif] directives, and add also main package 
to TestsAll.&]
[s0; &]
[s0; This way you select the ProjectTestsAll package while doing 
TDD (test driven development) or when you want to run tests, 
hit run and the tests are failing (or running). Once you are 
satisfied with all tests and you want to run actual application, 
switch to main Project package containing the real main, and 
run it.&]
[s0; &]
[s22; Other remarks&]
[s0; Don`'t forget to run tests in all possible compiler configurations, 
which should be supported. I.e. both release and debug mode, 
and in case you use more compilers (MSCC vs GCC/MINGW), use both 
also to run tests. At least once per couple of days, but I think 
doing this daily is the right way (or having continuous integration 
server doing automated builds all the time). This will give you 
chance to catch subtle incompatibilities very early, and it will 
often help to discover nasty bugs which are hiding in particular 
mode/compiler combination, and will be revealed only in different 
combination.&]
[s0; &]
[s22; GUI testing&]
[s0; I`'m sorry if you find this too much `"console`" oriented: I 
didn`'t personally need to test any GUI parts of application 
(a mix of GUI main package solving layout and GUI functions `+ 
independent module doing the internals did solve this for me 
sufficiently), and so far I`'m not sure even where to start with 
GUI testing.&]
[s0; &]
[s0; This area is open for anyone who want to improve U`+`+ tools 
and has some great ideas.]