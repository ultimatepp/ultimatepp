topic "Code style";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[b117;*+117 $$10,0#27457433418004101424360058862402:Caption]
[a83;*R6 $$11,0#31310162474203024125188417583966:caption]
[b83;*4 $$12,13#07864147445237544204411237157677:title]
[b42;a42;ph2 $$13,13#45413000475342174754091244180557:text]
[{_}%EN-US 
[s11; Ultimate`+`+ code style&]
[s12; Introduction&]
[s13; Code style has always been subject to a lot of discussions, 
some of them being really passionate and emotional. Those of 
you already having seen the intimate details of Ultimate`+`+ 
in uppsrc and maybe bazaar surely have a concept in a certain 
degree of order, functionality and maybe, like I did, even a 
certain beauty (yes, I am married in real world :), all hard to 
describe. For me the code felt useful, well structured and quite 
easy to read and thus easier to understand. And this is maybe 
the essence of good code style. All the things mentioned below 
are tiny little details that can help to achieve this overall 
perception in U`+`+ code. Don`'t hesitate to look at the code 
in uppsrc to learn. I admit, that to `'learn`' U`+`+ style isn`'t 
done in a day because it isn`'t a fixed law. One needs to have 
worked with it for a while and feel some comfort with it. But 
this document should shorten the time.&]
[s13; If you like to contribute code or major fixes, you are encouraged 
to pay attention to these things. It makes life easier for the 
community, which usually keeps an eye on the changes made to 
the code base. It enables us (the community, of which you are 
about to be part of) to spot bugs fast and provide fast fixes. 
Having X coding styles mixed together in a project is just about 
the worst case. Though U`+`+ is split in packages it is still 
considered one single project. So, please make sure you consider 
overall goals that will be introduced in the next paragraph.&]
[s12; Design&]
[s13;i150;O0; Produce clean, `'intuitive`' and compact code, making 
[* usage ]easy as well as reading (and ideally understanding) the 
code.&]
[s13;i150;O0; [* Use Topic`+`+] (.tpp files) to comment your class usage, 
this is the prefered method of code commenting. Use separate 
files for API and implementation documentation. TheIDE can generate 
the appropriate files for you. Click on the little cyan rectangles 
next to functions, variables or classes. `'[/ New reference topic]`" 
adds to `'src.tpp`', `'[/ New implementation topic]`' adds to `'srcimp.tpp`'. 
[* Provide also the `"Big picture`"] design aspects of your package 
/ class, briefly outlined in the src.tpp as a header (see U`+`+ 
class documentation as an example). This relieves a lot of headache 
when studying the code.&]
[s13;i150;O0; [* Be familiar with U`+`+, use its facilities]. This is 
a general prerequisite anyway, but it means to have studied the 
existing code and its possibilities a bit. It might happen that 
you have invented things that U`+`+ already provides for you 
and you could have saved your time. It also means to know, understand, 
and thus stick to the design pattern, i.e. when implementing 
new Ctrl`'s, like knowing that there is a [C virtual Updated() 
]function to be overridden to recalculate the Ctrl`'s state only, 
without repaint, and is called with [C Ctrl`::Update() ]in a generalized 
manner. Another example is the Callback idea or the NTL Containers.&]
[s13;i150;O0; Each [* class has got its own .h and] (maybe multiple) 
[* .cpp] (or .hpp for template), except for classes that are tight 
together really closely and belong together.&]
[s13;i150;O0; Include package files using `"[/ Insert package directory 
file]`" mainly, instead of using `"[/ Insert any file]`". Otherwise 
[* use relative to package directory file paths] instead of absolute 
paths.&]
[s13;i150;O0; If you use a Bazaar package, include it to your Assembly 
doing this: `"[/ File > Set Main Package]`", `"[/ Assembly]`" area, 
right button, `"[/ Edit assembly...]`" option, `"[/ Package nests:]`" 
field, adding the absolute path to Bazaar at the end.&]
[s13;i150;O0; [* Include relative paths] instead of absolute in [C #include]s, 
like [C #include <Scatter/Scatter.h>] instead of [C #include `"/folderA/folderB/Scatt
er.h`"]&]
[s13;i150;O0; [* Use the package idea] of U`+`+. A package compounds 
one or more classes and its files, because they logically belong 
together and should all be available when using the package inside 
another package. A package should have an overall name and a 
.h with same name (which then can [C #include] the other class 
headers if any). Example: [C #include <Core/Core.h>]. This is the 
way you also should use other packages inside yours. (See `'Overview`' 
for more info on packages) &]
[s13;i150;O0; Provide a [* Test / Demo package] for your package. Generally 
it is a good idea to have a `'MyPackage`' and `'MyPackageTest`' 
package shipped together, where functionality and Testing / Presenting 
is separated. Others won`'t need to trouble with the code too 
much if the can simply [C #include <MyPackage/MyPackage.h>]. a 
package whose usage they have observed in the Test package.&]
[s13;i150;O0; [* Avoid usage of new / delete] as hell. [* Use U`+`+ NTL 
containers] for your data or encapsulate the allocation or deallocation 
of objects. Memory leaks can cause serious brain damage :). of 
course, don`'t mix new/delete with malloc/free. Pointers should 
only be used to really point to things and NOT to maintain the 
last reference to a dynamically allocated object (this should 
be done in containers, Array<>, Vector<>, One<>, Any<>, etc. 
[* everything belongs somewhere]...).&]
[s13;i150;O0; [* Avoid usage of  ][*C #define constants], use compile 
time safe constants inside your class where possible. Example: 
[C static const int MaxSize `= 100;]&]
[s13;i150;O0; Do not forget the [*C virtual `~Destructor()], As soon 
as your class is meant to be a base class (i.e. has any other 
virtual function) it should have a virtual destructor, even if 
it`'s empty.&]
[s13;i150;O0; Pay attention to implicit conversion. Some bugs come 
from constructors parametrized by default, which are used then 
as implicit conversion constructors. (you may protect with the 
`'explicit`' keyword)&]
[s13;i150;O0; Maintain the [* const correctness. ]Failing to do so opens 
doors to many bugs and makes the usage of your class difficult 
in classes that maintain it.&]
[s13;i150;O0; [* Use integer counter access]. Do not use iterators on 
the Containers. Example:&]
[s0;l320; [C for(int i `= 0; i < vec.GetCount(); i`+`+)]&]
[s0;l320; [C `{]&]
[s0;l320; [C     vec`[i`] `= 0;]&]
[s0;l320; [C `}]&]
[s13;i150;O0; User for each loop when you want to go over whole container.&]
[s13;i150;O0; Use positive action semantic. Think of general usage 
patterns, known to the user, like Add(), Remove(), Detach(). 
Example:&]
[s0;l320; [C void Enable(bool e `= true) `{ /`*your code`*/ `}]&]
[s0;l320; [C void Disable() `{ return Enable(false); `}]&]
[s0;l320; [C bool IsEnabled() const `{ /`*your code`*/ `}]&]
[s13;l160;~~~32; Upp code is both easier to read and to write because 
of little things like that.&]
[s13;i150;O0; [* Implement small functions in class definition itself]. 
Many functions only have 1 or 2 syntactically important statements 
(return `*this doesn`'t count) and are maybe important to understand 
class function. A developer will first look in header file. Rule 
of thumb: >3 statements should go into own implementation in .cpp 
file. Example:&]
[s13;l320; [C String Right(int count) const `{ return Mid(GetLength() 
`- count); `}]&]
[s13;i150;O0; Provide for Methods chaining in your class by returning 
`*this, where logically useful. This makes it possible to [C myControl.HSizePos().V
SizePos().Enable().]&]
[s13;i150;O0; Provide a [C String ToString() const; ]Users will thank 
you if they can inspect an object in a preformatted way with 
[C LOG(objectInstance);] without having to setup own functions 
to show content during debug. Maybe it should be virtual to allow 
derived classes to extend it. This is heavily used in object 
design orientated languages like C#.&]
[s13;i150;O0; Think of providing a [C unsigned GetHashValue() const; 
]If it is logical, such a function can enable your object to 
directly be used as a Key in Upp containers like Index.&]
[s12; Class layout&]
[s13; A class is made for someone to use it and should expose most 
important info first. So keep the class components`' order as 
following: (unneeded or unused parts can be left out)&]
[s0; &]
[s0; [C@(28.42.150) class tempc ]&]
[s0; [C@(28.42.150) -|: public EmptyClass]&]
[s0; [C@(28.42.150) -|, public Any]&]
[s0; [C@(28.42.150) `{]&]
[s0; [C@(28.42.150) -|//FRIENDS]&]
[s0; [C@(28.42.150) -|friend class Ctrl;]&]
[s0; [C@(28.42.150) -|friend void Vector<int>`::Clear();]&]
[s0;C@(28.42.150) &]
[s0; [C@(28.42.150) public:]&]
[s0; [C@(28.42.150) -|/`*]&]
[s0; [C@(28.42.150) -|`* all handling data structures and typedefs nesseccary 
for the usage of public classes API go here]&]
[s0; [C@(28.42.150) -|`*/]&]
[s0;C@(28.42.150) &]
[s0; [C@(28.42.150) -|typedef unsigned HANDLE;]&]
[s0; [C@(28.42.150) -|typedef tempc CLASSNAME;]&]
[s0;C@(28.42.150) &]
[s0; [C@(28.42.150) -|struct SubStruct]&]
[s0; [C@(28.42.150) -|`{`}]&]
[s0; [C@(28.42.150) -|]&]
[s0; [C@(28.42.150) -|static const int AnIntConstant `= 123;]&]
[s0; [C@(28.42.150) -|//static const float constf `= 1.2f; //NOT POSSIBLE, 
arch dependant.]&]
[s0;C@(28.42.150) &]
[s0; [C@(28.42.150) -|//CTORS, DTORS, all concerning this class generation]&]
[s0; [C@(28.42.150) -|//INTERFACE, defines how to operate it]&]
[s0;C@(28.42.150) &]
[s0; [C@(28.42.150) -|//explicit]&]
[s0; [C@(28.42.150) -|tempc(int a `= 0);]&]
[s0; [C@(28.42.150) -|virtual `~tempc() `{`}]&]
[s0;C@(28.42.150) &]
[s0; [C@(28.42.150) -|void AReadOnlyMethod() const `{ /`*do something, 
bot dont change things in class`*/ `}]&]
[s0; [C@(28.42.150) -|void AWritableMethod() `{ /`*here you can change 
your class state`*/ `}]&]
[s0;C@(28.42.150) &]
[s0; [C@(28.42.150) -|/`*]&]
[s0; [C@(28.42.150) -|`* Here follows the public available data, typically 
its place for a not close related members]&]
[s0; [C@(28.42.150) -|`*/]&]
[s0; [C@(28.42.150) -|]&]
[s0; [C@(28.42.150) protected:]&]
[s0; [C@(28.42.150) -|/`*]&]
[s0; [C@(28.42.150) -|`* all handling data structures and typedefs interesting 
for the deriving classes go here]&]
[s0; [C@(28.42.150) -|`*/]&]
[s0;C@(28.42.150) &]
[s0; [C@(28.42.150) -|/`*]&]
[s0; [C@(28.42.150) -|`* Here follows the interface, deriving classes 
might be interested in, accessing it directly.]&]
[s0; [C@(28.42.150) -|`*/]&]
[s0; [C@(28.42.150) -|]&]
[s0; [C@(28.42.150) -|/`*]&]
[s0; [C@(28.42.150) -|`* Here follows the data, which mostly for performance 
reasons should be made available to derived classes]&]
[s0; [C@(28.42.150) -|`*/]&]
[s0; [C@(28.42.150) -|]&]
[s0; [C@(28.42.150) private:]&]
[s0; [C@(28.42.150) -|/`*]&]
[s0; [C@(28.42.150) -|`* all handling data structures and typedefs which 
we use only internally, go here]&]
[s0; [C@(28.42.150) -|`*/]&]
[s0;C@(28.42.150) &]
[s0; [C@(28.42.150) -|/`*]&]
[s0; [C@(28.42.150) -|`* Here follows the private interface, which mostly 
are own helper classes and functions]&]
[s0; [C@(28.42.150) -|`*/]&]
[s0;C@(28.42.150) &]
[s0; [C@(28.42.150) -|/`*]&]
[s0; [C@(28.42.150) -|`* Here follows the private data, which only the 
class itself takes care of and none should ever know about.]&]
[s0; [C@(28.42.150) -|`* in case of wrapper class this is mostly the 
objects that are wrapped]&]
[s0; [C@(28.42.150) -|`*/]&]
[s0; [C@(28.42.150) `};]&]
[s0; &]
[s0; [*+117 Readability]&]
[s13;i150;O0; The [* default code style in TheIDE] is the first reference 
(Edit > Advanced > Format code in editor).&]
[s13;i150;O0; [* Use Pascal Case] for function names. (concatenated 
with capital letter at the beginning and on word boundaries, 
example:. [C void AddMemberElement(int a) ]). [* Use small letters 
anywhere else], especially in class members`' names. [* Don`'t 
use Hungarian style] (Linus: `"Brain damage`") or things like 
`'[C m`_myMember, `_anotherMember, yetAnotherMember]`'. Example: 
[C Font font; Size textsize; ]Assist`+`+ makes life so easy, that 
we do not need this to figure out if its a class member (Hit 
Ctrl`+Space)&]
[s13;i150;O0; Stick to[*  implicit variable names semantic] inside function 
bodies if possible and useful. i,j,k,m,n,q, u mostly are integer 
counters or indexes in arrays, p is probably a pointer, a,b,c 
are typically bool, d is double, f is a float. v is a Vector 
or a Value, s is a String etc.. this makes code less typographic 
and focuses more on the algorithm used. But don`'t use such variables 
exclusively, please. a1 a2 and a3 are quite the same at first 
look :)&]
[s13;i150;O0; Tend to [* avoid comments] in general, except for necessary 
hints on important sections, critical for understanding. instead, 
[* focus and pursue a good design structure] in your package / 
class, which makes most comments disposable.&]
[s13;i150;O0; [* Use Tabs to intend code], except in the header of a 
class, where many functions might have a single command body. 
This should be aligned with spaces (see container headers for 
example)&]
[s13;i150;O0; Put spaces around operators, example: [C if(a > 12) `{ 
/`*code`*/ `}]&]
[s13;i150;O0; The ampersand belongs to the object. example: [C Ctrl`& 
GetCtrlRef();]&]
[s13;i150;O0; Usually put static variable instantiation, CH`_STYLE 
and other housekeeping stuff at the bottom.&]
[s0; ]]