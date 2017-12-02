topic "2008.1 changelog";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[l288;a17;*1 $$7,7#70004532496200323422659154056402:requirement]
[{_}%EN-US 
[s0; [4 Ultimate 2008.1]&]
[s0;1 &]
[s0; [1 Full list of all changes:]&]
[s0; [0 `* new Core with better threading support. More here  http://www.ultimatepp.org/
forum/index.php?t`=msg`&th`=2189`& amp; amp; amp;start`=0`&]&]
[s0; [0 `* init`_ and GLOBAL`_ changes. More here http://www.ultimatepp.org/forum/index.
php?t`=msg`&th`=2202`& amp; amp; amp;start`=0`&]&]
[s0; [0 `* RichEdit now support overwrite mode (Insert key...)]&]
[s0; [0 `* new methods Ctrl `*GetOwnerCtrl() and const Ctrl GetOwnerCtrl() 
const]&]
[s0; [0 `* first iteration of Drag and Drop in Ctrl (currently only 
for win32) More here http://www.ultimatepp.org/forum/index.php?t`=msg`&th`=2222`& 
amp; amp; amp;start`=0`&]&]
[s0; [0 `* added Ctrl`::OverrideCursor(const Image`& m) `- Overrides 
the image of mouse cursor.]&]
[s0; [0 `* Ctrl now detects and delivers some new mouse events. More 
here http://www.ultimatepp.org/forum/index.php?t`=msg`&th`=2228`& 
amp; amp; amp;start`=0`&]&]
[s0; [0 `* modal loop handling changes. More here http://www.ultimatepp.org/forum/index.
php?t`=msg`&th`=2230`& amp; amp; amp;start`=0`&]&]
[s0; [0 `* added Ctrl`::ChildFrameMouseEvent(Ctrl `*child, int event, 
Point p, int zdelta, dword keyflags) for conroling child`'s frame 
mouse events]&]
[s0; [0 `* GridCtrl now supports merged cells (see JoinCells and JoinFixedCells 
methods). Fixed some bugs.]&]
[s0; [0 `* PostgreSQL support! (thanks to ZSolt)]&]
[s0; [0 `* many Drag `& Drop improvements. All edit controls as well 
as TreeCtrl and ArrayCtrl now support it.]&]
[s0; [0 `* ArrayCtrlDnd reference was added]&]
[s0; [0 `* fixed MaxLen restriction in edits.]&]
[s0; [0 `* PostgreSQL now supports EnumView, EnumSequences and EnumTables. 
GetColumns should also work.]&]
[s0; [0 `* InnerJoin, LeftJoin, RightJoin, FullJoin and On methods 
were added to SqlExp interface to allow do joins in sql`-92 way. 
Old way (which work only in Oracle (and probably in Firebird)) 
are naturally still available.]&]
[s0; [0 `* as MinGW does not support TLS variables, it now compiles 
as `"USEMALLOC`" in MT mode]&]
[s0; [0 `* IconDes: Insert from file operation was added]&]
[s0; [0 `* TheIDE now accepts file drops]&]
[s0; [0 `* ColumnList supports drag `& drop]&]
[s0; [0 `* fixed rotated text problem (DrawText with angle) in X11]&]
[s0; [0 `* fixed problem with Ctrl`+1 `- Ctrl`+0 keys in X11]&]
[s0; [0 `* ArrayCtrl and TreeCtrl support showing small popup if cell`'s 
width is not enough to display the text. It can by switched off 
by NoPopUpEx()]&]
[s0; [0 `* U`+`+ is now able to produce Win64 code (ise MSC8`_64 builder) 
(Artur Bác)]&]
[s0; [0 `* Code editor highlights Sql functions, boolean expression, 
U`+`+ macros and text in upper case. Default color is black. 
Change it see the effect]&]
[s0; [0 `* Code editor marks edited lines as well as lines with warnings 
and errors (Ralf `- Zardos)]&]
[s0; [0 `* inital support for drag `& drop in X11. Small stability 
fixes to win32 drag `& drop.]&]
[s0; [0 `* Markers of changes in code editor! (by Ralf) `+ highlighting 
error/warnings/found in files lines.]&]
[s0; [0 `* New QuickTabs]&]
[s0; [0 `* Windows installer should work properly now (theide.exe is 
not hanging on process list)]&]
[s0; [0 `* Postgres and As() fixes (by ZSolt)]&]
[s0; [0 `* Fixed some problems with TreeCtrl (by Mrjt) More here: http://www.ultimatepp.o
rg/forum/index.php?t`=msg`&goto`=959 2`&#msg`_9592]&]
[s0; [0 `* HomeBudget crashed when sql exception was thrown. Fixed.]&]
[s0; [0 `* Some GridCtrl fixes (see internal changelog)]&]
[s0; [0 `* Drag and Drop interface slightly refactored]&]
[s0; [0 `* Delayed clipboard rendering support]&]
[s0; [0 `* Topic`+`+ integrated into main Ide window]&]
[s0; [0 `* Reduced code size produced by png plugin]&]
[s0; [0 `* GridCtrl `- fixed text wrapping, added HeaderAlign`* methods]&]
[s0; [0 `* MYSQLDLL `- This new MySql package flag forces linking with 
.dll version of MySql client library.]&]
[s0; [0 `* IconDes import file is now able to import multiple files]&]
[s0; [0 `* Maximum number of `"varargs`" extended to 40]&]
[s0; [0 `* Added EditField`::AlignRight]&]
[s0; [0 `* Extended DateTimeCtrl customization abilities]&]
[s0; [0 `* Fixed crashing Run() in PostgreSQL]&]
[s0; [0 `* Added ArrayCtrl`::SetCtrl()]&]
[s0; [0 `* New Image cache ops. More here http://www.ultimatepp.org/forum/index.php?t`=m
sg`&th`=2468`& amp; amp; amp;start`=0`&]&]
[s0; [0 `* Small ToolBar improvements (Wrap(), NoWrap(), SetButtonMinSize(Size)]&]
[s0; [0 `* Some macro enhancements for TheIde. More here http://www.ultimatepp.org/forum
/index.php?t`=msg`&th`=2513`& amp; amp; amp;start`=0`&]&]
[s0; [0 `* GetHomeDirectory, GetHomeDirFile now working in Win32]&]
[s0; [0 `* Grouping in QuickTabs (not fully finished)]&]
[s0; [0 `* SqlExp operator `| now produces correct output for PostgreSQL]&]
[s0; [0 `* fixed compiling SQL`_PostgreSql reference (Bas)]&]
[s0; [0 `* optimized painting when there is a lot of out of screen 
widgets]&]
[s0; [0 `* TabCtrl now supports Insert and Remove of tabs]&]
[s0; [0 `* FileSel is now using icons from windows]&]
[s0; [0 `* multidisplay support (in Windows)]&]
[s0; [0 `* LOG improvementes More here: http://www.ultimatepp.org/forum/index.php?t`=msg`&
th`=2670`& amp; amp;start`=0`&]&]
[s0; [0 `* fixed ScrollBar`::MinThumb(int sz) `- there was not return 
value]&]
[s0; [0 `* many DropGrid improovements (but still no documentation)]&]
[s0; [0 `* fixed couple of bugs of GridCtrl (see changelog.txt)]&]
[s0; [0 `* PostgreSQL: Dialect now renamed to PGSQL, PgSequence now 
represents PGSQL SEQUENCE object.]&]
[s0; [0 `* PostgreSQL: DUAL`_PRIMARY`_KEY was not working, added support 
for binary columns]&]
[s0; [0 `* SqlExp Select statement now has `"Get`" variant to be used 
instead of `"From`" to get `"table`-less`" select, like `"select 
nextval(`'seq`')`". In Oracle, Select(NextVal(SEQ)).Get gets 
translated to `"select seq.nextval from DUAL`", in PGSQL to `"select 
nextval(`'SEQ`')`".]&]
[s0; [0 `* `-M theide commandline option generates Makefile]&]
[s0; [0 `* Help Window is no longer a Window, but is now integrated 
into theide main frame.]&]
[s0; [0 `* `"Close all tabs`" and `"Close non`-project tabs`" feature 
of tabs is now implemented]&]
[s0; [0 `* DropList dropfocus mode is now default, added NotNull, fixed 
refreshing right button after closing popuped list]&]
[s0; [0 `* Fixed memory allocator connected with limit on a total number 
of virtual memory blocks]&]
[s0; [0 `* Fixed QuickTabs crash related to group change, reduced space 
between icon and tab name]&]
[s0; [0 `* GridCtrl now supports inserting after/before and duplicating 
rows]&]
[s0; [0 `* Change in Image interface http://www.ultimatepp.org/forum/index.php?t`=msg`&t
h`=2579`& amp; amp;start`=0`&]&]
[s0; [0 `* Added simple static widget DisplayCtrl]&]
[s0; [0 `* Added FileSel preview]&]
[s0; [0 `* Added SqlArray`::ReQuery()]&]
[s0; [0 `* Added CPU`_Cores() `- returns the number of CPU cores in 
system.]&]
[s0; [0 `* New widget DropGrid http://www.ultimatepp.org/forum/index.php?t`=msg`&th`=261
0`& amp; amp;start`=0`&]&]
[s0; [0 `* Improved build performance]&]
[s0; [0 `* MingGW updated from 3.4.2 to 4.2.1]&]
[s0; [0 `* SDL updated from 1.2.7 to 1.2.12]&]
[s0; [0 `* Sqlite updated from 3.3.0 to 3.4.2]&]
[s0; [0 `* PCRE updated from 7.0 to 7.3]&]
[s0; [0 `* added close group to quick tabs popuped menu]&]
[s0; [0 `* `"Insert any file`" in TheIDE hotkey changed to Ctrl`+Shift`+O(because 
of conflict with Italic in T`+`+ editor)]&]
[s0; [0 `* those pesky icon.ico files wrong exports are no more generated 
from .iml]&]
[s0; [0 `* fixed a problem with unnamed image in .iml causing iml loss]&]
[s0; [0 `* added `-gstabs flag for mingw `- this is supposed to solve 
the problem with mingw 4.2 debugging]&]
[s0; [0 `* fixed potential crash in ArrayCtrl`::AddColumn]&]
[s0; [0 `* FileSel types change if there is an empty `"DefaultExt`", 
extension is now taken from the `"file type`" droplist, from 
the first mask.]&]
[s0; [0 `* fixed clipping in Ctrl`::GetTopRect]&]
[s0; [0 `* fixed painting LabelBox image at incorrect y`-coord (mrjt)]&]
[s0; [0 `* fixed ArrayCtrl`::NoBackground (mrjt)]&]
[s0; [0 `* fixed segmentation fault on Ubuntu Feisty (mdelfede)]&]
[s0; [0 `* fixed problems with GUI and the new sqlite3 (thanks to dmcgeoch)]&]
[s0; [0 `* Sql`::Begin and SqlSession`::Begin now perform ClearErrors]&]
[s0; [0 `* added void Sql`::Get(Fields fo) More here: http://www.ultimatepp.org/forum/inde
x.php?t`=msg`&th`=2719`& amp; amp; amp; amp;start`=0`&]&]
[s0; [0 `* added String SqlSession`::GetErrorCodeString(). Some databases 
like Postgres return `"stringed`" error codes.]&]
[s0; [0 `* added SqlSession`::WhenDatabaseActivity(ActivityStatus status) 
callback. Status can be (for now) EXECUTING, END`_EXECUTING, 
FETCHING, END`_FETCHING. It can be used to react on database 
activity (for example mouse cursor can change then, progress 
bar can be displayed and so on)]&]
[s0; [0 `* fixed QuickTabs highlighting problems]&]
[s0; [0 `* added popuped menu with Close item to QuickTabs]&]
[s0; [0 `* added network browsing class NetNode (for now, in Win32)]&]
[s0; [0 `* EditField`::NullText now has variant with Font parameter]&]
[s0; [0 `* added EditField`::SetInactiveConvert(Convert `&c) `- allow 
to set convert class used when focus is out of edit field]&]
[s0; [0 `* added Image Win32DllIcon(const char `*dll, int ii, bool 
large) to retrieve icon as image from given dll file.]&]
[s0; [0 `* fixed Splitter SetMin which wrongly used the 10000 ratio 
instead of pixels. This new method should use directly pixels....]&]
[s0; [0 `* almost all calls to Win32 API in Core are now implemented 
to use unicode variant (like CreateFileW)]&]
[s0; [0 `* painting Image for low`-level machines was optimized]&]
[s0; [0 `* fixed 2 regression bugs of DateTimeCtrl. More here: http://www.ultimatepp.org/
forum/index.php?t`=msg`&th`=2773`& amp; amp; amp; amp;start`=0`&]&]
[s0; [0 `* TheIDE Ctrl`+F1 now searches help for the current word]&]
[s0; [0 `* added value parameter to ArrayCtrl`::SetCtrl. More here: http://www.ultimatepp.
org/forum/index.php?t`=msg`&th`=2779`& amp; amp; amp; amp;start`=0`&]&]
[s0; [0 `* added IdCtrls `- a new very small utility class, modeled 
after SqlCtrls. More here: http://www.ultimatepp.org/forum/index.php?t`=msg`&th`=27
80`& amp; amp; amp; amp;start`=0`&]&]
[s0; [0 `* fixed a problem in CtrlCore which prevented closing the 
window with long operations and many timer events]&]
[s0; [0 `* many DropGrid fixes (resizing problems, null value caused 
selecting first item on list) and GridCtrl fixes (multiline highlighing 
is now supported, item editable state is now respected when ctrls 
are shown)]&]
[s0; [0 `* Going into Help (pressing F1 or icon/menu) now does not 
change the tree or topic.]&]
[s0; [0 `* F1 in help now has the same effect as pressing Ctrl`+Tab]&]
[s0; [0 `* Win64 support (library only)]&]
[s0; [0 `* Automatic setup in TheIDE now supports (auto`-installs) 
MSC8 `- x64 compiler]&]
[s0; [0 `* Drawing now supports Null]&]
[s0; [0 `* Fixed GetMinSize in .usc]&]
[s0; [0 `* Layout Font property height now zoomed]&]
[s0; [0 `* `"init`" files in packages. More here http://www.ultimatepp.org/forum/index.p
hp?t`=msg`&th`=2834`& amp; amp;start`=0`&]&]
[s0; [0 `* Fixed `"Writes to freed blocks`" bug in ide]&]
[s0; [0 `* Added ProgessIndicator`::SetColor]&]
[s0; [0 `* Added ASSERT`_ macro `- ASSERT with `"message`" parameter. 
U`+`+ will now tell you what is wrong with your Value conversions]&]
[s0; [0 `* New widget MultiButton]&]
[s0; [0 `* added void Ctrl`::LayoutId(const char `*s) to put the name 
of layout variable into the Ctrl]&]
[s0; [0 `* There is now much improved chameleon support for DropList, 
DropChoice, DropGrid, DropDate, DropTime and EditField for Windows 
Vista]&]
[s0; [0 `* StaticRect enhanced to support Ch Values]&]
[s0; [0 `* Added chameleon utility functions to add and remove margins 
from rectangle]&]
[s0; [0 `* Chameleon can set now style for individual componenets of 
composite widgets]&]
[s0; [0 `* IconDes / .iml changed to premultiplied format]&]
[s0; [0 `* Added BackPaintHint. More here http://www.ultimatepp.org/forum/index.php?t`=m
sg`&th`=2867`& amp; amp;start`=0`&]&]
[s0; [0 `* Fixed Upp to work in Linux again]&]
[s0; [0 `* DateTimeCtrl replaced by DropDate and DropTime]&]
[s0; [0 `* Improvements to painting system]&]
[s0; [0 `* Many fixes to GridCtrl and DropGrid]&]
[s0; [0 `* Going into Help (pressing F1 or icon/menu) now does not 
change the tree or topic.]&]
[s0; [0 `* F1 in help now has the same effect as pressing Ctrl`+Tab]&]
[s0; [0 `* Win64 support (library only)]&]
[s0; [0 `* Automatic setup in TheIDE now supports (auto`-installs) 
MSC8 `- x64 compiler]&]
[s0; [0 `* Drawing now supports Null]&]
[s0; [0 `* Fixed GetMinSize in .usc]&]
[s0; [0 `* Layout Font property height now zoomed]&]
[s0; [0 `* `"init`" files in packages. More here http://www.ultimatepp.org/forum/index.p
hp?t`=msg`&th`=2834`& amp; amp;start`=0`&]&]
[s0; [0 `* Fixed `"Writes to freed blocks`" bug in ide]&]
[s0; [0 `* Added ProgessIndicator`::SetColor]&]
[s0; [0 `* Added ASSERT`_ macro `- ASSERT with `"message`" parameter. 
U`+`+ will now tell you what is wrong with your Value conversions]&]
[s0; [0 `* New widget MultiButton]&]
[s0; [0 `* added void Ctrl`::LayoutId(const char `*s) to put the name 
of layout variable into the Ctrl]&]
[s0; [0 `* There is now much improved chameleon support for DropList, 
DropChoice, DropGrid, DropDate, DropTime and EditField for Windows 
Vista]&]
[s0; [0 `* StaticRect enhanced to support Ch Values]&]
[s0; [0 `* Added chameleon utility functions to add and remove margins 
from rectangle]&]
[s0; [0 `* Chameleon can set now style for individual componenets of 
composite widgets]&]
[s0; [0 `* IconDes / .iml changed to premultiplied format]&]
[s0; [0 `* Added BackPaintHint. More here http://www.ultimatepp.org/forum/index.php?t`=m
sg`&th`=2867`& amp; amp;start`=0`&]&]
[s0; [0 `* Fixed Upp to work in Linux again]&]
[s0; [0 `* DateTimeCtrl replaced by DropDate and DropTime]&]
[s0; [0 `* Improvements to painting system]&]
[s0; [0 `* Many fixes to GridCtrl and DropGrid]&]
[s0; [0 `* QuickTabs refactored to use system theme to paint tabs]&]
[s0; [0 `* Implemetation of DateTimeCtrl moved to header]&]
[s0; [0 `* Added Build/Clean package to packages popup menu of TheIDE]&]
[s0; [0 `* U`+`+ translated to Finish (Tom1)]&]
[s0; [0 `* Chameleon styles now have `"Standard`" method which returns 
the `"default`" initial value of the style]&]
[s0; [0 `* HeaderCtrl can now move (reorganize) tabs]&]
[s0; [0 `* ArrayCtrl sorting]&]
[s0; [0 `* simplify translating in TheIDE. More here http://www.ultimatepp.org/forum/ind
ex.php?t`=msg`&th`=2905`& amp; amp; amp;start`=0`&]&]
[s0; [0 `* ConvertInt64, EditInt64]&]
[s0; [0 `* Toolbar buttons now can have labels]&]
[s0; [0 `* some Hydra fixes]&]
[s0; [0 `* Visual Studion 9.0 (2008) compiler supported]&]
[s0; [0 `* fixed Debugger going to wrong file at startup]&]
[s0; [0 `* EditField, WithDropChoice, Bar, MenuBar, ToolBar, StatusBar, 
MenuBar visual improvements (Vista, GTK)]&]
[s0; [0 `* BLITZ heurestics altered. More here http://www.ultimatepp.org/forum/index.php
?t`=msg`&th`=2986`& amp; amp; amp;start`=0`&]&]
[s0; [0 `* DropGrid can now display clear button]&]
[s0; [0 `* added MultiButton`::SetLabel(), DropGrid`::AddText, DropGrid`::AddClear]&]
[s0; [0 `* added MulitButton`::RemoveButton(int)]&]
[s0; [0 `* Some Sql addidtions. More here http://www.ultimatepp.org/forum/index.php?t`=m
sg`&th`=3575`& amp;start`=0`&]&]
[s0; [0 `* fixed DrawArc bug (mdelfede)]&]
[s0; [0 `* fixed big font issue in gnome (cbpporter)]&]
[s0; [0 `* fixed most Win64 warnings]&]
[s0; [0 `* ide: added underlining of recently modified files]&]
[s0; [0 `* ide: added export project ]&]
[s0; ]]