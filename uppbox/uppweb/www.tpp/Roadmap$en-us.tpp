topic "Changelog";
[2 $$0,0#00000000000000000000000000000000:Default]
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[b83;*4 $$3,5#07864147445237544204411237157677:title]
[i288;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;2 $$5,5#45413000475342174754091244180557:text]
[l288;b17;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;t246;C@5;1 $$7,7#20902679421464641399138805415013:code]
[b2503;2 $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base]
[t4167;C2 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam]
[b167;C2 $$13,13#92430459443460461911108080531343:item1]
[i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128)2 $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7;2 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line]
[l160;t4167;*C+117 $$18,5#88603949442205825958800053222425:package`-title]
[2 $$19,0#53580023442335529039900623488521:gap]
[t4167;C2 $$20,20#70211524482531209251820423858195:class`-nested]
[b50;2 $$21,21#03324558446220344731010354752573:Par]
[{_}%EN-US 
[s0; [*R6 Roadmap]&]
[s0;*3 &]
[s0; [*_3 Current release]&]
[s0;3 &]
[s0; [*_4 2015.1 (rev 8227)][*3  (Mar 2015)]&]
[s0;4 &]
[s0; [* Core]&]
[s0;* &]
[s0;i150;O0; U`+`+ abandoned `"default pick`" semantics for containers. 
The transfer semantics now has to be specified explicitly, either 
as `'[* pick]`' or `'[* clone]`' (this is enforced in C`+`+11 mode, 
in legacy mode old behaviour is maintained and pick/clone are 
optional).&]
[s0;i150;O0; New containers, based on fast`-insertion arrays: [* InVector, 
InArray, SortedIndex, SortedVectorMap, SortedArrayMap]. SortedIndex, 
SortedVectorMap and SortedArrayMap provide binary searches, while 
maintaining random access to the array, with performance comparable 
to binary tree based containers (e.g. std`::map).&]
[s0;i150;O0; New [* Core/POP3] package `- support for retrieving emails 
form POP3 servers.&]
[s0;i150;O0; Initial round of C`+`+11 support (lambda callbacks, 
minor fixes)&]
[s0;i150;O0; A set of small utility functions [* findarg], [* decode], 
[* min]/[* max] now supporting more arguments. [* SplitTo ]and [* Merge] 
for splitting/merging strings.&]
[s0;i150;O0; [* InFilterStream], [* OutFilterStream] classes that can 
provide some sort of processing (e.g. compression) to regular 
streams.&]
[s0;i150;O0; New Date related functions [* EasterDay], [* GetMonths], 
[* GetWeek], [* GetWeekDate].&]
[s0;i150;O0; MIME / fileextension mapping functions [* FileExtToMIME 
]and [* MIMEToFileExt].&]
[s0;i150;O0; Some ARMv7 related optimization (unaligned access, byte 
swaps).&]
[s0;i150;O0; [* SpinLock] class.&]
[s0;i150;O0; Stream optimization methods [* GetPtr], [* PutPtr], [* GetSzPtr] 
provide safe access to Stream internal buffer.&]
[s0;i150;O0; String/WString`::[* Find](const String`&) significantly 
speed optimized.&]
[s0;i150;O0; [* Value] invalid casts now throwing exception [* ValueTypeError] 
instead of panic.&]
[s0;i150;O0; [* WebSocket] class.&]
[s0;i150;O0; `[[* Stable]`][* SortBy]`[[* Keys]`|[* Values]`] functions for 
sorting VectorMap, ArrayMap.&]
[s0;i150;O0; [* plugin]/[* lz4 ]`- adapted fast compression library&]
[s0;i150;O0; [* plugin]/[* lzma ]`- adapted high ratio compression library&]
[s0;i150;O0; [* plugin]/[* glew] `- popular OpenGL utility library adapted 
as package&]
[s0; &]
[s0; [* GUI programming `& graphics]&]
[s0;* &]
[s0;i150;O0; [* Gtk ]backend (U`+`+ now using Gtk for host platform 
interface)&]
[s0;i150;O0; [* EditDateDlg], [* EditNumber ]simple function for entering 
single value (equivalent to EditText)&]
[s0;i150;O0; ArrayCtrl new methods to add/retrieve ValueMaps and 
ValueArrays, new [* CreateCtrl]<T> method.&]
[s0;i150;O0; LineEdit heavily optimized to be able to work with huge 
files (>100MB).&]
[s0;i150;O0; LineEdit now able to work with [* rectangular selections.]&]
[s0;i150;O0; [* DDARasterizer] and [* SDraw] are new tools providing 
Draw with minimal host platform support (only two operations, 
paint Image and paint colored rectangle, are required).&]
[s0;i150;O0; Image rescaling now supports varios filters, like bicubic 
or lanczos.&]
[s0;i150;O0; AttrText is now rich comparable Value: it is now possible 
to sort ArrayCtrl by AttrText filled columns.&]
[s0;i150;O0; [* GLDraw] `- Draw implemented using OpenGL (ES).&]
[s0;i150;O0; [* SVG parser] (to Painter interface).&]
[s0;i150;O0; [* plugin]/[* ppm ]`- trivial image format support.&]
[s0;i150;O0; [* RichText]/RichEdit [* header]/[* footer ]support, QTF now 
using BASE64 for binary data (instead of 7`-bit raw encoding).&]
[s0;i150;O0; [* Turtle] is another web framework of U`+`+ `- this one 
allows to convert existing GUI applications into Web application: 
simple javascript terminal connects to server where GUI application 
runs.&]
[s0;i150;O0; [* ScatterDraw/ScatterCtrl] `- Added legend table, linked 
scroll controls, units, data fitting to equations, run time user 
equations, zoom window, improved text drawing, huge datasets 
handling and key/mouse handling.&]
[s0;i150;O0; X11 backend: Multimonitor support, removed flickering 
when opening window&]
[s0;i150;O0; [* Error]`[[* OK]`|[* Cancel]`|[* Yes]`|[* No]...`] `- new Prompt 
variants with Error decorations&]
[s0; &]
[s0; [* Sql]&]
[s0; &]
[s0;i150;O0; New variants of Insert/Select/Update commands (now roughly 
supporting all important combinations of SQL standard).&]
[s0;i150;O0; [* SqlSetFrom ]functions creates a SqlSet from any container.&]
[s0;i150;O0; S`_ structures refactored, now provide an improved interface 
with better introspection.&]
[s0;i150;O0; Sql console redesigned.&]
[s0; &]
[s0; [* TheIDE]&]
[s0;* &]
[s0;i150;O0; Win64 PDB (Visual C`+`+ compiled code) debugger.&]
[s0;i150;O0; Find/replace GUI refactored, added support for incremental 
finds, added regexp support.&]
[s0;i150;O0; Block replace now has grep`-like feature to remove matching/non`-matchin
g lines (instead of replacing matches)&]
[s0;i150;O0; Case insensitive replace now has `"mimic case`" option 
(mimics the case of first and next characters, so replacing `"vector`" 
`-> `"array`", when match is `"Vector`", result is `"Array`").&]
[s0;i150;O0; Compilation errors now parsed and displayed nicely.&]
[s0;i150;O0; Assist`+`+ context goto (Alt`+J) now supports local 
variables (goes to declaration).&]
[s0;i150;O0; Layout/Icon designers now have find Search filter.&]
[s0;i150;O0; Ide now displays long numbers with thousands separators.&]
[s0;i150;O0; Syntax highlighting refactored, added highlighting of 
HTML, XML, .log files, .xsd files, .diff files, C`+`+11 keywords 
highlighted, TODO and FIXME comments highlighted&]
[s0;i150;O0; Directory comparison tool.&]
[s0;i150;O0; Simple Json/XML viewers.&]
[s0;i150;O0; Comment/Uncomment block.&]
[s0;i150;O0; Selected identifier is now highlighted in all the text.&]
[s0;i150;O0; Current line is highlighted with faint lines.&]
[s0;i150;O0; Precompiled headers support for release mode (BLITZ 
still far better for debug builds).&]
[s0;i150;O0; Insert file path (plain or as CString) function.&]
[s0;i150;O0; Layout code generator now can place double`-quotes around 
elements, generate handlers for buttons.&]
[s0;i150;O0; Layout designer now can sort widgets based on their 
graphical position.&]
[s0;i150;O0; Code navigator is completely redesigned.&]
[s0;i150;O0; Icon designer supports various rescaling filters and 
new icon smoothing function.&]
[s0;i150;O0; Ide now has line duplicate feature (CTRL`+D).&]
[s0; &]
[s0;3 &]
[s0; [*_4 5485][*3  (Oct 2012)]&]
[s0;3 &]
[s0; The focus of this release was server programming. The main new 
feature is `"[* Skylark]`", lean and mean [^http`:`/`/www`.ultimatepp`.org`/srcdoc`$Skylark`$Tutorial`$en`-us`.html^ U
`+`+ based web development framework].&]
[s0; &]
[s0; [* Core]&]
[s0; &]
[s0;i150;O0; String class has been further optimized for improved 
Cat performance.&]
[s0;i150;O0; Id class was refactored to better support multithreaded 
programming.&]
[s0;i150;O0; STATIC`_ASSERT introduced for compile`-time checks.&]
[s0;i150;O0; TCP/IP and HTTP was refactored and integrated into Core, 
providing new TcpSocket, IpAddrInfo, HttpHeader and HttpRequest 
classed (effectively obsoleting Web package).&]
[s0;i150;O0; Value was refactored and optimized, now using small 
value optimization technique.&]
[s0;i150;O0; Value got Xmlize (conversion to/from XML) and Jsonize 
(conversion to/from JSON) support.&]
[s0;i150;O0; Date now has operator`+`+/`-`-.&]
[s0;i150;O0; Exit is now equivalent of C exit function, implemented 
using exception trow to `*`_APP`_MAIN and C`+`+ friendly.&]
[s0;i150;O0; Thread now has AtExit support.&]
[s0;i150;O0; .ini parameters now can be optionally encapsulated using 
INI`_`* helpers.&]
[s0;i150;O0; Sort algorithm was refactored, works about 10% faster.&]
[s0;i150;O0; LOGHEX and DUMPHEX logging macros provided for logging 
Strings in hexadecimal form.&]
[s0;i150;O0; LoadFromJsonFile and StoreToJsonFile jsonize object 
from/to files.&]
[s0; &]
[s0; &]
[s0; [* Sql]&]
[s0; &]
[s0;i150;O0; `'Of`' method, used to qualify SQL column names, now 
has new alternative notation using operator().&]
[s0;i150;O0; Schema files information is now accessible application 
providing a form of database schema introspection.&]
[s0;i150;O0; JoinRef method of Select provides automatic join where 
join condition is synthetized by framework based on schema introspection.&]
[s0;i150;O0; SqlCtrls now support automated mode based on schema 
introspection.&]
[s0;i150;O0; SqlSession now has ThrowOnError option, when active 
all SQL errors throw SqlError exception.&]
[s0;i150;O0; Sql: Sql now supports fetching ValueMap of row (using 
Fetch variant or GetRowMap after Fetch).&]
[s0;i150;O0; MassInsert now has `"remove`" option to remove rows 
inserted before inserting, NoUseTransaction option and is using 
RDBM specific SQL syntax (instead generic UNION ALL trick) when 
possible.&]
[s0;i150;O0; Secondary SQLR default session intended for `'read`-only`' 
sessions.&]
[s0;i150;O0; Default sessions SQL and SQLR now can be per`-thread 
(useful for multithreaded server applications).&]
[s0;i150;O0; Columns and table names now can be optionally quoted 
(new mode).&]
[s0;i150;O0; Sql: SqlLoadTable and SqlLoadColumn helper function can 
read whole tables into C`+`+ VectorMaps.&]
[s0;i150;O0; New Sqls`::operator`^ fetches single row from select 
and returns it as ValueMap.&]
[s0; &]
[s0; &]
[s0; [* RichText, RichEdit]&]
[s0;i150;O0; Paragraph ruler now can have dotted or dashed styles&]
[s0; &]
[s0; [* CtrlLib]&]
[s0; &]
[s0;i150;O0; EditField now has WhenPasteFilter that can be used to 
change pasted clipboard text before inserting it.&]
[s0;i150;O0; FileSel in Win32 is now using separate thread to lazy`-load 
icons.&]
[s0;i150;O0; ColorPopup checks clipboard and when it founds a text 
resembling color definition (like #12aa33 or 123 55 6), adds 
this color to predefined colors in selection.&]
[s0; &]
[s0; [* ide]&]
[s0;* &]
[s0;i150;O0; New, vastly improved GDB frontend.&]
[s0;i150;O0; Startup times should be significantly improved.&]
[s0;i150;O0; C#, JavaScript and css syntax highlighting.&]
[s0;i150;O0; Ide now can view .png, .jpg, .gif and .bmp files and 
even edit smaller .png files.&]
[s0;i150;O0; New save file on window deactivation option is helpful 
when editing scripts or web templates `- no need to save file 
when going to another window to test.&]
[s0;i150;O0; Insert clipboard as C string, convert text to C string 
functions.&]
[s0;i150;O0; New find in files dialog option to filter files according 
to their `'read only`' status.&]
[s0;4 &]
[s0;4 &]
[s0; [*_3 Previous releases]&]
[s0; &]
[s0; [*_4 4193][*3  (24 Nov 2011)]&]
[s0;* &]
[s0; `"Rainbow`" technology decouples U`+`+ GUI from host platform. 
It is now possible to develop new GUI&]
[s0; implementation without changing core U`+`+ files. `"Framebuffer`" 
package uses Rainbow to implement&]
[s0; GUI for trivial RGBA framebuffer (with keyboard and mouse inputs).&]
[s0; &]
[s0; New command line utility `"umk`" `- GUI independent, U`+`+ package 
builder: umk.&]
[s0; &]
[s0; Docking package was adopted to main source tree.&]
[s0;*3 &]
[s0; [* TheIDE]&]
[s0;i150;O0; Support for UTF8`-BOM encoding.&]
[s0;i150;O0; Layout files are now syntax highlighted, Assist`+`+ 
now knows correct line position info.&]
[s0;i150;O0; Database schema files (.sch) are now syntax highlighted. 
Assist`+`+ now knows to correct line position info.&]
[s0;i150;O0; Ide now has minimal Objective C and C`+`+/CLR support.&]
[s0;i150;O0; New function to export documentation to html or pdf.&]
[s0;i150;O0; Layout designer now has `'struct`' option in code generator&]
[s0;i150;O0; Image (.iml) designer now can import (insert) whole 
another .iml and also export .iml to png files.&]
[s0;i150;O0; There is now Assist`+`+ for #include.&]
[s0;i150;O0; InsertColor function now supports QTF colors.&]
[s0;i150;O0; Local menu on package file items now has `"Open file 
directory`" item.&]
[s0;* &]
[s0; [* Library]&]
[s0;* &]
[s0; Core&]
[s0;i150;O0; U`+`+ now has Value compatible Complex type.&]
[s0;i150;O0; CParser has new Unicode Escape option to understand 
`"`\u`" escape codes (as in JavaScript).&]
[s0;i150;O0; ValueArray has now Insert, Append and Remove methods.&]
[s0;i150;O0; ValueMap has now Set(key, value) method.&]
[s0;i150;O0; Value now has operator`[`] (key) and operator`[`] (index) 
methods to directly access contained ValueMap and ValueArray 
elements.&]
[s0;i150;O0; double formatting (Format, Convert) now has support 
to easily replace decimal point with `',`' character.&]
[s0;i150;O0; Core now has JSON (JavaScript Object Notation) parsing 
and creation support.&]
[s0;i150;O0; String now has GetCharCount() that is able to return 
correct number of characters (code`-points) in String even if 
encoding is UTF`-8.&]
[s0; &]
[s0; CtrlLib and CtrlCore (GUI library)&]
[s0;i150;O0; ArrayCtrl now has AsText, SetClipboard, AsQtf and AsCsv 
methods.&]
[s0;i150;O0; EditField now supports limited text highlighting.&]
[s0;i150;O0; New SetLayout`_`[layout name`] template function generated 
from .lay (used to change compatible layout).&]
[s0; &]
[s0; Draw/PdfDraw/Report (graphical output)&]
[s0;i150;O0; Draw/PdfDraw/Report got minimalistic PDF/A support.&]
[s0;i150;O0; AttrText now has Bold/Italic/Underline/Strikeout methods.&]
[s0;i150;O0; New QtfReport for simplified reporting of Qtf.&]
[s0; &]
[s0; Other library changes&]
[s0;i150;O0; In SQL trace log, questionmark parameter placeholders 
are now replaced with actual parameters.&]
[s0;i150;O0; plugin`\Sqlite3 is upgraded to 3.7.8.&]
[s0;i150;O0; plugin/tif is upgraded to version 1.43.&]
[s0;i150;O0; plugin/jpg now supports EXIM image orientation metadata.&]
[s0;i150;O0; New plugin/wincert package supports basic Win32 certificate 
manipulation and PDF digital signing.&]
[s0;#i150;O0; HttpClient now supports PUT method.&]
[s0;3 &]
[s0; [*_4 3211][*3  (16 Feb 2011)]&]
[s0;*3 &]
[s0; [* TheIDE]&]
[s0;i150;O0; SSE2 as Option in main config, templates now include 
SSE2 as default (RM #9).&]
[s0;i150;O0; new function `- convert selection to ASCII.&]
[s0;i150;O0; Main package configuration editor shows structured flags 
accepted by packages.&]
[s0;i150;O0; Replaced QuickTabs with TabBar, reorganized Environment 
dialog.&]
[s0;i150;O0; Allow conditional breakpoints.&]
[s0;i150;O0; Support for CLOB columns in .sch file (#14).&]
[s0;* &]
[s0; [* Library]&]
[s0;* &]
[s0; Core&]
[s0;i150;O0;  Added missing Gate4`::Execute.&]
[s0;i150;O0; Tuples now Moveable if all elements moveable.&]
[s0;i150;O0; Added Xmlize ValueArray and ValueMap.&]
[s0;i150;O0; Added DayOfYear, StrToTime; Draw: Rotate180.&]
[s0;i150;O0; Thread threadid support.&]
[s0;i150;O0; FormatIntBase added `'upper`' option. Added FormatIntHexUpper 
function.&]
[s0; &]
[s0; Draw&]
[s0;i150;O0; Added ImageOps hotspots.&]
[s0;i150;O0; RASTER`_8ALPHA now considered non`-premultiplied (fixes 
issue in some .png files).&]
[s0; &]
[s0; Painter&]
[s0;i150;O0; Painter now depends on Draw.&]
[s0; &]
[s0; plugin/gif:&]
[s0;i150;O0; Support for LCT.&]
[s0; &]
[s0; CtrlCore&]
[s0;i150;O0; EncodeRTF improved to support page size, margins and 
table cell borders.&]
[s0;i150;O0; GetOpaqueRect, GetVoidRect now const&]
[s0;i150;O0; TopWindow FullScreen for open window in Win32&]
[s0; &]
[s0; CtrlLib&]
[s0;i150;O0; FileSel: Support for Terminal Services client shares 
(Win32).&]
[s0;i150;O0; FileSel lazy and optimized file icon loading.&]
[s0;i150;O0; Automatic landscape mode for RichText print.&]
[s0;i150;O0; LabelBox visually supports Disabled state, LabelBase 
has got disabledink.&]
[s0;i150;O0; Splitter can now have any width.&]
[s0;i150;O0; ArrayCtrl with embedded Ctrls optimized to work well 
up to 10000 widgets.&]
[s0; &]
[s0; RichEdit&]
[s0;i150;O0; Added SingleLine, Filter.&]
[s0;i150;O0; Improved process of searching for speller dictionaries.&]
[s0;i150;O0; Added some missing czech translations.&]
[s0; &]
[s0; GridCtrl&]
[s0;i150;O0; Added ClearChanged(), added missing row`_order setting 
in Remove0&]
[s0; &]
[s0; Sql&]
[s0;i150;O0; operator`[`](SqlId) now invokes NEVER (in debug) when 
column is not found&]
[s0;i150;O0; Oracle: SQLT`_TIMESTAMP support (thanks alendar)&]
[s0; &]
[s0; Web&]
[s0;i150;O0; SmtpMail: Refactored, added support for character encoding 
in headers, added recipient/sender names, &]
[s0; &]
[s0; [* Bazaar]&]
[s0;# &]
[s0; Many packages added and improved: Scatter, DeEncrypter, CMeter, 
Protect, Tree, Dispatcher, Gen, SDLCtrl, PolyXML, PlotLib,PlotCtrl, 
SysInfo, Media, MediaPlayer, Functions4U, plugin/portaudio, Sound, 
SoundExample, AutoScroller, CtrlFinder, CtrlProp, LogPosCtrl, 
PointCtrl, PopUpC, RectCtrl, Styler, ValueCtrl, Docking , TabBar, 
SysExec, Protect, Updater, SysExecGui, OfficeAutomation, Dispatcher, 
Styler, Cypher, ProductVersion, Uniq, SplashSV and HelpViewer.&]
[s0;4 &]
[s0; [*_4 2791][*3  (18 Oct 2010)]&]
[s0;*3 &]
[s0; [* TheIDE]&]
[s0;i150;O0; Now it has direct support for Windows SDK 7.1.&]
[s0;* &]
[s0; [* Library]&]
[s0;* &]
[s0; Core&]
[s0;i150;O0; There are now new templates Tuple2 `- Tuple4.&]
[s0;i150;O0; Stream class got new methods Peek and PutPtr intended 
for optional optimizations.&]
[s0;i150;O0; In Vector and Array template classes, interface was 
changed so that all Add methods are now returning the reference 
to the element inserted.&]
[s0;i150;O0; String and WString has new Replace method.&]
[s0; &]
[s0; Draw&]
[s0;i150;O0; plugin/gif now has support for animated gifs, Raster 
and Raster encoder have new GetDPI and SetDPI methods. DPI information 
handling for png, tif and jpg was fixed.&]
[s0;i150;O0; AttrText utility class has new NormalInk method and 
supports conversion from Value.&]
[s0; &]
[s0; CtrlCore&]
[s0;i150;O0; There is a new IsPainting method.&]
[s0; &]
[s0; CtrlLib&]
[s0;i150;O0; Progress has now blocked open if invoked during Draw 
method. IdCtrls have now Reset method.&]
[s0;i150;O0; New utility function SelectDirectory. &]
[s0;i150;O0; LineEdit now reacts to Shift`+MouseWheel does horizontal 
scroll with horizontal scrolling.&]
[s0;i150;O0; RichTextView got new WhenMouseMove callback.&]
[s0; &]
[s0; GridCtrl&]
[s0;i150;O0; Has now WhenBeforeChangeRow and WhenBeforeChangeCol 
callbacks, CancelDuplicate, SetCtrl, FindCol, ReadCol and CopyColumnNames 
methods and SetValues, GetValues and Xmlize template specialization.&]
[s0; &]
[s0; Sql&]
[s0;i150;O0; SqlSelect now can add individual fetch values using 
operator(). &]
[s0;i150;O0; New SqlMassInsert provides generic acceleration of mass 
inserts using `'union all`' trick. &]
[s0;i150;O0; IsSame is a new SqlExp equality test which is null safe 
(null `=`= null yields true).&]
[s0;i150;O0; Oracle8 has new BulkExecute interface to OCI8 array 
binds repeated command execution with varying parameter sets; 
speeds up inserts / updates drastically over networks with long 
roundtrip.&]
[s0; &]
[s0; XmlRpc&]
[s0;i150;O0; XmlRpcCall has now Proxy and ProxyAuth methods.&]
[s0; &]
[s0; New plugin/Box2D package was added.&]
[s0; &]
[s0; [* Bazaar]&]
[s0; &]
[s0; Added packages&]
[s0;i150;O0; Protect: Software encryption system.&]
[s0;i150;O0; Cypher, StreamCypher, CypherTest.&]
[s0;i150;O0; DeEncrypter: A `*simple`* AESStream based Encrypter / 
Decrypter.&]
[s0;i150;O0; BufferStream: Stream based on Vector<byte> buffer, self 
growing.&]
[s0;i150;O0; CMeter: A progressbar / meter / slider with some versatile 
settings Ctrl &]
[s0;i150;O0; Dispatcher: Generic class to dispatch data to arbitrary 
recipients.&]
[s0;i150;O0; HelpViewer, TestHelpViewer.&]
[s0; &]
[s0; MtAlt&]
[s0;i150;O0; Some improvements&]
[s0; &]
[s0; Scatter&]
[s0;i150;O0; Some fixes&]
[s0; &]
[s0; Docking&]
[s0;i150;O0; Some fixes&]
[s0; &]
[s0; PixRaster&]
[s0;i150;O0; Updated Leptonica library to version 1.65 &]
[s0; &]
[s0; TabBar&]
[s0;i150;O0; Minor changes and fixes&]
[s0; &]
[s0; Timer&]
[s0;i150;O0; Minor changes&]
[s0; &]
[s0; ExpandFrame&]
[s0;i150;O0; Minor changes&]
[s0; &]
[s0; AESStream&]
[s0;i150;O0; Added SHA2 functions&]
[s0; &]
[s0; LEDCtrl&]
[s0;i150;O0; New LED control by Christian&]
[s0; &]
[s0; Functions4U&]
[s0;i150;O0; Added functions, GatherTpp, improved doc and fixed bugs&]
[s0; &]
[s0; SysInfo&]
[s0;i150;O0; Added GetAdapterInfo&]
[s0; &]
[s0; Control4U&]
[s0;i150;O0; Added Knob&]
[s0; &]
[s0; OfficeAutomation&]
[s0;i150;O0; Cyrillic characters support&]
[s0; &]
[s0; UltimateBook&]
[s0;i150;O0; Updated doc &]
[s0;4 &]
[s0; [*_4 2467][*3  (10 Jun 2010)]&]
[s0; &]
[s0; [* TheIDE]&]
[s0;i150;O0; Now has QTF designer. &]
[s0; &]
[s0; [* Library]&]
[s0; &]
[s0; Core&]
[s0;i150;O0; `[W`]String got new variant of ReverseFind method.&]
[s0; &]
[s0; CtrlCore&]
[s0;i150;O0; There is a new static method Ctrl`::GetEventId to help 
identify individual GUI events, intended for use in caching schemes.&]
[s0; &]
[s0; CtrlLib&]
[s0;i150;O0; Update system was parametrised.&]
[s0;i150;O0; Handling of active file type in FileSel was improved.&]
[s0;i150;O0; Splitter now has WhenSplitFinish.&]
[s0; &]
[s0; PdfDraw&]
[s0;i150;O0; Got support for fill patterns. &]
[s0;* &]
[s0; [* Bazaar]&]
[s0;i150;O0; SysInfo: Added CPU temperature monitoring and improved 
battery status.&]
[s0;i150;O0; Functions4U: Added new SVG color functions and doc.&]
[s0;i150;O0; Controls4U: Added ActiveX controls (VLC and web browsing).&]
[s0;i150;O0; MtAlt: Added support for fast 5`-argument callbacks (copy 
and pick version).&]
[s0;i150;O0; Form: Form core package, Editor and sample.&]
[s0;i150;O0; Scatter: Added SetSequentialX and SetFastViewX for faster 
refresh.&]
[s0;i150;O0; Docking: Bug fixes, some small feature additions.&]
[s0;i150;O0; ExpandFrame/ExpanderCtrl: Fixes fopr horizontal alignment.&]
[s0;i150;O0; AESStream: Added SHA2 functions.&]
[s0;4 &]
[s0; [*_4 2361][*3  (4 May 2010)]&]
[s0; &]
[s0; [* TheIDE]&]
[s0;i150;O0; Packages now sorted by directory closeness to main package 
directory, names later `- puts main package nest or main package 
subpackages first in the list&]
[s0; &]
[s0; [* Library]&]
[s0; &]
[s0; Core:&]
[s0;i150;O0; ArrayIndex: Added Detach, PopDetach&]
[s0;i150;O0; XmlRpc: Added bool XmlRpcPerform(Socket`& http, const 
char `*group);&]
[s0; &]
[s0; CtrlLib:&]
[s0;i150;O0; Added ArrayCtrl WhenScroll&]
[s0;i150;O0; ColumnList: AutoHideSb&]
[s0;i150;O0; FileSel: list is autohide&]
[s0;i150;O0; EditField ReadOnly Ctrl`+C fix&]
[s0;i150;O0; DocEdit: NoEofLine&]
[s0;i150;O0; RichEdit now supports dropping Image files&]
[s0;i150;O0; SqlCtrl: SqlArray has new WhenFilter Gate (able to filter 
records being fetched)&]
[s0; &]
[s0; Sql:&]
[s0;i150;O0; Optional SqlSession errorhandler routing is now installable&]
[s0;i150;O0; SqlSession: First SQL error is recorded (until ClearError) 
instead of last&]
[s0; &]
[s0; Uppsrc:&]
[s0; New IT`-IT translation&]
[s0;4 &]
[s0; [*_4 2232][*3  (17 March 2010)]&]
[s0; &]
[s0; [* TheIDE]&]
[s0;i150;O0; Sorting of package has been improved, placing project 
packages first&]
[s0;i150;O0; Insert menu now has typedef ... CLASSNAME entry.&]
[s0;i150;O0; FileSel was improved by adding Places with common directories.&]
[s0; &]
[s0; [* Library]&]
[s0;* &]
[s0; Core:&]
[s0;i150;O0; XmlRpc has improved error handling.&]
[s0;i150;O0; Xmlize now supports all Core Value types and has got 
support for Value serialization.&]
[s0;i150;O0; XmlParser has got new Peek methods. Socket has new GetPeerAddr 
method.&]
[s0;i150;O0; CParser now provides the information about current column.&]
[s0; &]
[s0; CtrlLib:&]
[s0;i150;O0; TabCtrl has new Find/Set/Remove/Insert methods using 
reference to slave instead of index.&]
[s0; &]
[s0; RichText:&]
[s0;i150;O0; RichTextView now have `"ShrinkOversizedObjects`" option.&]
[s0;4 &]
[s0; [*_4 2070][*3  (16 February 2010)]&]
[s0; &]
[s0; [* TheIDE]&]
[s0;i150;O0; User interface for Find and Replace was significantly 
reworked&]
[s0;i150;O0; Current editor line is now highlighted in the left bar.&]
[s0; &]
[s0; [* Library]&]
[s0;* &]
[s0; Core:&]
[s0;i150;O0; Support for Win32 .lnk files (GetSymLinkPath function, 
FindFile`::IsSymLink method) `- same interface as for Posix symlinks.&]
[s0;i150;O0; ConvertDate has new Truncate option (truncates Time 
to Date), corresponding StdConvertDateTruncate global Convert 
added.&]
[s0; &]
[s0; CtrlLib:&]
[s0;i150;O0; SliderCtrl widget, has new Jump method. &]
[s0;i150;O0; ArrayCtrl class has new WhenMouseMove Callback. &]
[s0;i150;O0; EditField has new WhenEnter callback.&]
[s0;i150;O0; TabCtrl has new NoAccept option&]
[s0; &]
[s0; Draw:&]
[s0;i150;O0; The search for missing glyphs (i18n issue) was optimized.&]
[s0;4 &]
[s0; [*_4 1952][*3  (27 January 2010)]&]
[s0; &]
[s0; [* TheIde]&]
[s0;i150;O0; Improved installation in Win32 with support for the 
latest Win32 SDK. &]
[s0;i150;O0; Helpsystem has vastly improved searching capabilities.&]
[s0;i150;O0; Build process now supports new link options `- Posix 
releases now use `"gc`-sections`" linker option to dramatically 
reduce the size of resulting binaries (up to 40%). &]
[s0; &]
[s0; [* Library]&]
[s0;* &]
[s0; CtrlLib:&]
[s0;i150;O0; File selector (FileSel class) got networking support 
in Win32.&]
[s0;i150;O0; TrayIcon in X11 now supports notifications. &]
[s0;i150;O0; TopWindow in X11 now supports SetAlphs. &]
[s0; &]
[s0; Painter:&]
[s0;i150;O0; Image rescaling algorithm was improved for better downscaling. 
&]
[s0; &]
[s0; Core: &]
[s0;i150;O0; Containers now have a new debugging utility to resolve 
pick semantic errors.&]
[s0;4 &]
[s0; [*_4 1824][*3  (21 December 2009)]&]
[s0; &]
[s0; [* TheIDE]&]
[s0;i150;O0; Autosetup now recognizes Win32 SDK 7.0.&]
[s0; &]
[s0; [* Library]&]
[s0;* &]
[s0; Core:&]
[s0;i150;O0; XmlRpc package adds support for XML`-RPC based webservices.&]
[s0;i150;O0; LanguageInfo has been completely refactored.&]
[s0; &]
[s0; Bazaar:&]
[s0;i150;O0; GoogleTranslator: New version.&]
[s0;i150;O0; Functions4U: New package with additional functions to 
complement Core and CtrlLib.&]
[s0;i150;O0; Controls4U: New package with additional graphic controls. 
&]
[s0;i150;O0; SysInfo: Improved Unicode support.&]
[s0;4 &]
[s0; [*_4 1713][*3  (30 November 2009)]&]
[s0; &]
[s0; [* TheIDE]&]
[s0;i150;O0; .t files are now saved with strings converted by escaping 
>127 characters (and back on loading). This fixes venerable problem 
of UTF`-8 string literals in CJK Windows and MSC compiler.&]
[s0; &]
[s0; [* Library]&]
[s0;* &]
[s0; CtrlLib:&]
[s0;i150;O0; TreeCtrl InsertDrop now returns ids of inserted nodes.&]
[s0;i150;O0; A new set of utility functions that simplify selecting 
files introduced (see http://www.ultimatepp.org/src`$CtrlLib`$FileSelUtil`$en`-us.
html)&]
[s0;i150;O0; HeaderCtrl columns now can have a tooltip&]
[s0;i150;O0; SliderCtrl now allows reversed orientation (thanks avpavp!)&]
[s0; &]
[s0; RichEdit:&]
[s0;i150;O0; New much more effective speller dictionary format, utilities 
required to convert OpenOffice speller dictionaries into U`+`+ 
speller files.&]
[s0;i150;O0; Labeled paragraphs now marked with arrow displayed in 
the left bar.&]
[s0;i150;O0; Previously private StyleManager class capable of managing 
paragraph styles has been made public.&]
[s0; &]
[s0; Report:&]
[s0;i150;O0; New methods ChoosePrinter, ChooseDefault printer allow 
selecting printer BEFORE report is created, setting the physical 
page size based on chosen printer.&]
[s0; &]
[s0; Core:&]
[s0;i150;O0; In Win32, console applications no longer pop up a MessageBox 
when panicking, instead they write the panic string on the error 
output and terminate.&]
[s0; &]
[s0; GridCtrl:&]
[s0;i150;O0; Multiline popups now allowed, using GridDisplay to paint 
the content.&]
[s0; &]
[s0; Bazaar:&]
[s0;i150;O0; New functions to SysInfo&]
[s0; Urr `- UDP request`-response protocol added &]
[s0;4 &]
[s0; [*_4 1659][*3  (26 October 2009)]&]
[s0; &]
[s0; [* Library]&]
[s0;* &]
[s0;i150;O0; Core&]
[s0;l160;i160;O3; GetUtcTime&]
[s0;l160;i160;O3; New Split variant with String delimiter&]
[s0;l160;i160;O3; ConvertTime Min, Max&]
[s0;i150;O0; Core, CtrlLib: ConvertDate, ConvertTime now have Default 
method `-> propagates to date/time widgets as well...&]
[s0;i150;O0; PostgreSQL: GetInsertedId now supported for tables with 
primary key `'id`'&]
[s0;i150;O0; CtrlLib: MenuBar WhenOpenSubMenu, WhenCloseSubMenu callbacks&]
[s0;i150;O0; Sql, SqlExp: Sql`::InsertNoKey(Fields) ommits first column 
`- intended for key with autoincrement&]
[s0; &]
[s0; [* TheIDE]&]
[s0;* &]
[s0;i150;O0; Minor IDE speed improvements `- removed unnecessary 
Assist`+`+ package check when inserting / removing separators 
and auxiliary package contents&]
[s0; Package selector now shows app icons if available &]
[s0;4 &]
[s0; [*_4 1607][*3  (07 October 2009)]&]
[s0; &]
[s0; [* Library]&]
[s0; &]
[s0;i150;O0; Draw&]
[s0;l160;i160;O3; DisplayWithIcon (Display that adds an icon to any 
other Display)&]
[s0;l160;i160;O3; AttrText now has `'SetImage`' (places the icon 
to the left of  text)&]
[s0;i150;O0; CtrlLib: DropTree widget&]
[s0;i150;O0; Web: void AttachSocket(Socket`& socket, SOCKET s, bool 
blocking)&]
[s0;i150;O0; MSSQL: IdentityInsert helper (bypass of IDENTITY column 
insertion)&]
[s0;i150;O0; CtrlCore: RegisterSystemHotKey (system`-wide hotkey) 
&]
[s0;4 &]
[s0; [*_4 1579][*3  (23 September 2009)]&]
[s0; &]
[s0; [* Library]&]
[s0;* &]
[s0;i150;O0; Core: Added support for many various 8`-bit encodings&]
[s0;i150;O0; Geom: Added new Computational Geometry algorithm: 2D convex 
hull calculation (ConvexHullOrder)&]
[s0;i150;O0; Sql: SqlExp, SqlSelect AsTable, Joins accept SqlSet&]
[s0;i150;O0; RichText: Optimized by caching paragraph data and layout&]
[s0;i150;O0; Draw, plugin/tif: Added GetActivePage support&]
[s0;i150;O0; Fixed a lot of minor bugs&]
[s0;*4 &]
[s0; [*_4 1517][*3  (19 August 2009)]&]
[s0; &]
[s0; Major overhaul graphics infrastructure is finished, U`+`+ is 
now capable of headless drawing operations&]
[s0; (e.g. drawing graphics and text into .png files in console applications, 
without X11)&]
[s0; (example: [^http`:`/`/www`.ultimatepp`.org`/reference`$ConsoleDraw`.html^ http://www
.ultimatepp.org/reference`$ConsoleDraw.html])&]
[s0; &]
[s0; Minor improvements:&]
[s0; &]
[s0; [* Library]&]
[s0;* &]
[s0;i150;O0; RegExp fixed to work with mingw, added GetMatchPos method&]
[s0;i150;O0; SetSurface functions for optimized raw output of RGBA 
data to screen (example: [^http`:`/`/www`.ultimatepp`.org`/reference`$SetSurface`.html^ h
ttp://www.ultimatepp.org/reference`$SetSurface.html])&]
[s0;i150;O0; Added Blackfin CPU support into Core&]
[s0;i150;O0; EditField`::NullText method now can provide an icon&]
[s0;i150;O0; RichText, Report: RichTextLayoutTracer ability (example: 
[^http`:`/`/www`.ultimatepp`.org`/reference`$RichTextLayoutTracer`.html^ http://www.
ultimatepp.org/reference`$RichTextLayoutTracer.html])&]
[s0;i150;O0; Core: Added StringStream`::Reserve&]
[s0; &]
[s0; [* TheIDE]&]
[s0; &]
[s0;i150;O0; Added package internal include paths&]
[s0; Ctrl`+mouse wheel changes font size&]
[s0;*4 &]
[s0; [*_4 1314][*3  (17 June 2009)]&]
[s0; &]
[s0; [* Library]&]
[s0; &]
[s0;i150;O0; CtrlCore&]
[s0;l160;i160;O3; Fixed several X11/Compiz/SCIM compatibility issues&]
[s0;i150;O0; RichText&]
[s0;l160;i150;O3; QTF: Added text`-type rich object format&]
[s0;i150;O0; GridCtrl&]
[s0;l160;i150;O3; Added WhenMoveRow, CancelMove&]
[s0;l160;i150;O3; Improved search results highlighting&]
[s0; &]
[s0; [* TheIDE]&]
[s0; &]
[s0;i150;O0; Import directory source tree into package&]
[s0;i150;O0; Support for multiline svn commit messages&]
[s0;i150;O0; Assist`+`+ now supports SQL schema files&]
[s0;i150;O0; FileTabs now persistent&]
[s0;i150;O0; Non`-fixed pitch fonts allowed in editor (used as fixed...)&]
[s0;4 &]
[s0; [*_4 1254][*3  (03 June 2009)]&]
[s0;*4 &]
[s0; [* Library]&]
[s0; &]
[s0;i150;O0; Various improvements in SQL related stuff (SqlPerformScript, 
SqlBinary enable for PGSQL) &]
[s0; &]
[s0; [* TheIDE]&]
[s0; &]
[s0;i150;O0; Highlighting of .sch and .sql files &]
[s0;i150;O0; Find in files now has separate output console &]
[s0;i150;O0; Select package dialog `'All`' option replaced by the 
selection of more understandable options, main packages now displayed 
bold&]
[s0;4 &]
[s0; [*_4 1205][*3  (18 May 2009)]&]
[s0;*4 &]
[s0; [* Library]&]
[s0; &]
[s0;i150;O0; New GUI multithreading architecture &]
[s0; &]
[s0; [* TheIDE ]&]
[s0; &]
[s0;i150;O0; Layout designer: option to resize dialog without springs 
&]
[s0;i150;O0; No more confusing switches of .lay, .iml to text mode 
(instead designer) &]
[s0;i150;O0; One more redesign of code navigator &]
[s0;i150;O0; Ctrl`+Click onto symbol jumps to its definition &]
[s0;i150;O0; Package selector now shows the nest&]
[s0;*4 &]
[s0; [*_4 1126][*3  (5 May 2009)]&]
[s0;*4 &]
[s0;%- [* Library]&]
[s0;*%- &]
[s0;i150;O0; Change in SetDateFilter format, cleanup in related docs 
&]
[s0;i150;O0; Improved translation docs &]
[s0;i150;O0; Added runtime check for serializing RichValue with missing 
Register &]
[s0;i150;O0; Docking: CreateDockable<T> now returns type T &]
[s0;i150;O0; mrjt`'s MultiList now merged into ColumnList &]
[s0;i150;O0; TabBar: Tab Stacking and sorting added. Other changes. 
&]
[s0;i150;O0; Drawing refactored &]
[s0;*%- &]
[s0;*%- &]
[s0;%- [* TheIDE ]&]
[s0;*%- &]
[s0;i150;O0; Refactored CodeNavigator, search symbol (Ctrl`+Q) now 
copies current id search field &]
[s0;*4 &]
[s0; [%-*_4 1096][*3  (26 April 2009)]&]
[s0;*4%- &]
[s0;%- [* Library]&]
[s0;*%- &]
[s0;i150;O0;%- Painter: High quality, `"SVG/PDF strength`" software 
renderer&]
[s0;i150;O0;%- HttpClient improvements&]
[s0;i150;O0;%- JPGRaster now can read EXIF thumbnails&]
[s0;i150;O0;%- ODBC SQL interface&]
[s0;i150;O0;%- Microsoft SQL Server interface improved&]
[s0;i150;O0;%- Multithreading: ConditionVarable and LazyUpdate classes&]
[s0;i150;O0;%- LocalProcess class&]
[s0;i150;O0;%- RichText: EncodeHTML support for superscript, subscript, 
strikeout&]
[s0;i150;O0;%- and smallcaps&]
[s0;i150;O0;%- Mersenne twister based Random&]
[s0;%- &]
[s0;%- [* TheIDE]&]
[s0;*%- &]
[s0;i150;O0;%- Win32 .pdb debugger:&]
[s0;l160;i150;O3;%- shows tooltip with values of variables&]
[s0;l160;i150;O3;%- improved displaying of strings in pdb debugger&]
[s0;l160;i150;O3;%- watches are remembered, Clear all watches function, 
Drag`&Drop to watches&]
[s0;i150;O0;%- Packages can be sorted by name&]
[s0;i150;O0;%- Packages now can be colored and assigned bold/italic 
font&]
[s0;i150;O0;%- Package files underlined if recently changed&]
[s0;i150;O0;%- SVN support, SVN history of file&]
[s0;i150;O0;%- Compare with file, compare with patch&]
[s0;i150;O0;%- Assist`+`+&]
[s0;l160;i150;O3;%- New heurestic error recovery of C`+`+ parser&]
[s0;l160;i150;O3;%- New code navigator bar&]
[s0;l160;i150;O3;%- Position of Assist parameter info improved&]
[s0;l160;i150;O3;%- Context go to (Alt`+J) now jumps to layout designer 
when invoked on layout class template&]
[s0;i150;O0;%- Topic`+`+ finished; documentation now visible (and 
editable) as tooltip annotations of sources&]
[s0;i150;O0;%- Abbreviations: e.g. type `"i`" and press Ctrl`+`[.`] 
to expand it to if() ;&]
[s0;i150;O0;%- Icon editor: free angle rotation&]
[s0;i150;O0;%- Insert`" color or U`+`+ specific includes (.lay, .iml, 
.tpp group)&]
[s0;i150;O0;%- Print command (of source files)&]
[s0;i150;O0;%- Editor: ToUpper/Lower/InitCaps/SwapCase &]
[s0;4 &]
[s0; [*_4 2008.1][*3  (28 July 2008)]&]
[s0; &]
[s0; [* Library]&]
[s0; &]
[s0;i150;O0; New very fast String/WString implementation&]
[s0;i150;O0; New high performance (mostly) lock`-free heap allocator, 
&]
[s0;i150;O0; Improved multithreading support&]
[s0;i150;O0; CoWork multicore loop paralelization class&]
[s0;i150;O0; Unified Drag`&Drop support&]
[s0;i150;O0; PostgreSQL support&]
[s0;i150;O0; Win64 support (except debugger)&]
[s0;i150;O0; QuickTabs class&]
[s0;i150;O0; HeaderCtrl can now move (reorganize) tabs&]
[s0;i150;O0; ArrayCtrl automated column sorting&]
[s0;i150;O0; Chameleon Look`&feel improvements under GTK and Vista&]
[s0; &]
[s0; [* TheIDE]&]
[s0; &]
[s0;i150;O0; Topic`+`+ documentation editor now integrated into main 
window&]
[s0;i150;O0; Help Help integrated into main window&]
[s0;4 &]
[s0; [*_4 2007.1][*3  (11 April 2007)]&]
[s0; &]
[s0; [* Library]&]
[s0;* &]
[s0;i150;O0; Pdf export and printing in Linux finished&]
[s0;i150;O0; Raster image infrastructure completely reworked&]
[s0;i150;O0; `"Chameleon`" skinning system which allows both native 
look of U`+`+ applications across platforms AND custom skins 
to be designed/used.&]
[s0;i150;O0; New useful widgets: GridCtrl, CalendarCtrl and DateTimeCtrl&]
[s0;i150;O0; Sql expressions are now adjusted before being executed 
on particular RDBMS target&]
[s0;i150;O0; Library moved to Upp namespace&]
[s0;i150;O0; FreeBSD port is now regular part of U`+`+&]
[s0;i150;O0; Experimental PocketPC support was introduced&]
[s0;i150;O0; Library fixed to support ARM and PowerPC CPUs&]
[s0;i150;O0; A lot of new documentation and tutorials&]
[s0;i150;O0; Many minor improvements and bug fixes&]
[s0; &]
[s0; [* TheIDE]&]
[s0; &]
[s0;i150;O0; New Icon designer (supports alpha channel and has tools 
to produce antialiased icons)&]
[s0; &]
[s0; [* Distribution]&]
[s0;* &]
[s0;i150;O0; FreeBSD port available&]
[s0;i150;O0; Ubuntu .deb packages released&]
[s0;4 &]
[s0; [*_4 605][*3  (15 May 2006)]&]
[s0;* &]
[s0; [* Library]&]
[s0;* &]
[s0;i150;O0; XML parser refinements (faster, easier to use)&]
[s0;* &]
[s0; [* TheIDE]&]
[s0; &]
[s0;i150;O0; `"Hydra build`" `- multi`-CPU building&]
[s0;i150;O0; further Assist`+`+ improvements&]
[s0; &]
[s0; [* Distribution]&]
[s0;* &]
[s0;i150;O0; new Windows installer `- download size reduced by 50%&]
[s0; &]
[s0;*3 &]
[s0; [*_4 602][*3  (28 February 2006)]&]
[s0; &]
[s0; [* Library]&]
[s0;* &]
[s0;i150;O0; New Report package.&]
[s0;i150;O0; Fixed multi`-threading issues in Linux.&]
[s0; &]
[s0; [* TheIDE]&]
[s0; &]
[s0;i150;O0; Assist`+`+&]
[s0;l160;i150;O2; Fixed many C`+`+ parsing issues and optimized&]
[s0;l160;i150;O2; `"Virtual methods`" tool&]
[s0;l160;i150;O2; `"THISBACKs`" tool&]
[s0;i150;O0; `"Generate code..`" tool of Layout designer able to 
generate useful code snippets&]
[s0;i150;O0; `"Show opposite`" (Alt`+O) switches .h/.cpp files&]
[s0;i150;O0; `"Complete identifier`" (Alt`+,) `- based on identifiers 
in the file (no C`+`+ parsing intelligence)&]
[s0;i150;O0; Fixed support for global variables in PDB debugger&]
[s0;i150;O0; Package organizer redesigned, `"When`" conditions now 
support logical operators, `"uses`" can be conditional&]
[s0;i150;O0; New fine`-grained optimization system with `"Optimize 
for speed`" / `"Optimize for size`" / `"Optimal`" (only explicitly 
marked files are optimized for speed) modes&]
[s0;i150;O0; New TheIDE command`-line mode, including compilation 
(via umk utility)&]
[s0;*_3 &]
[s0; [*_4 511][*3  (20 November 2005)]&]
[s0; &]
[s0; [* U`+`+ Library]&]
[s0; &]
[s0;i150;O0; New XML support in Core&]
[s0;i150;O0; Linux/AMD64 is now supported&]
[s0;i150;O0; X11 fixes improve stability with less common window 
managers, input method problem fixed as well&]
[s0;i150;O0; New kind of callbacks `- pteback `- to avoid dangling 
callback situation problem&]
[s0;i150;O0; Sqlite was added as source`-based plugin together with 
U`+`+ adapter&]
[s0;i150;O0; A lot of new documentation&]
[s0; &]
[s0; [* TheIDE]&]
[s0; &]
[s0;i150;O0; File tabs are redesigned&]
[s0;i150;O0; Source editor has now scripting language&]
[s0;i150;O0; `"Calculator`" console&]
[s0;i150;O0; Layout of IDE now redesigned, bottom pane now contains 
browser/console/debugger/calculator switched by tabs&]
[s0;i150;O0; ASSIST`+`+:&]
[s0;l192;i150;O2; Improved C`+`+ code analysis now able to resolve 
class hierarchy issues, code browser now displays base classes, 
methods, virtual methods, overrides etc...&]
[s0;l192;i150;O2; New assist function now suggests list of methods 
and attributes after typing `'`->`' or `'.`' or after Ctrl`+Space.&]
[s0;l192;i150;O2; Navigate in the file and Navigate functions simplify 
moving to symbol with specified name&]
[s0;l192;i150;O2; Copy definition/declaration is able to convert 
method declaration (inside class declaration) to definition and 
back (works with more than one method as well)&]
[s0;l192;i150;O2; Goto definition/declaration moves between definitions 
and declarations of current method&]
[s0; &]
[s0; [* Distribution]&]
[s0; &]
[s0;i150;O0; U`+`+/Win now ships with SDL library and contains nice 
SDLExample as well as SDL project templates.&]
[s0; ]]