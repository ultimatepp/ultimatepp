topic "OfficeSheet class reference";
[ $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9; $$1,0#37138531426314131252341829483380:structitem]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:codeitem]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 OfficeSheet]]}}&]
[s1;*4 &]
[s1;:OfficeSheet`:`:class: [@(0.0.255)4 class][4 _][*4 OfficeSheet]&]
[s2;l0;* &]
[s0; [2 OfficeSheet class serves to manage spreadsheets made with OpenOffice 
Calc or Microsoft Office Excel.]&]
[s0; [2 The class methods are designed to be as simple as possible 
and are exactly the same for both suites.]&]
[s0; [2 OfficeSheet handles the suites through `"Ole Automation`" so 
it requires either OpenOffice or Microsoft Office to be previosly 
installed in the computer.]&]
[s0;2 &]
[s0;2 &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s3; &]
[s5;:OfficeSheet`:`:OfficeSheet`(`): [* OfficeSheet]()&]
[s2;%% OfficeSheet constructor. It has no arguments as in function 
Init() it is defined which Office suite is going to be used.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:OfficeSheet`:`:`~OfficeSheet`(`): [@(0.0.255) `~][* OfficeSheet]()&]
[s2;%% OfficeSheet destructor. It closes spreadsheet application.&]
[s3; &]
[s4; &]
[s5;:OfficeSheet`:`:IsAvailable`(String`): [@(0.0.255) static] [@(0.0.255) bool]_[* IsAvail
able]([_^String^ String]_[*@3 type])&]
[s2;%% It checks if office suit of name [%-*@3 type].is available to 
be init. &]
[s2;%% Valid values are `"Open`" and `"Microsoft`".&]
[s3;%% &]
[s4; &]
[s5;:OfficeSheet`:`:Init`(String`): [@(0.0.255) bool]_[* Init]([_^String^ String]_[*@3 type])
&]
[s2;%% Initializes OfficeSheet to use [%-*@3 type].suite.&]
[s2;%% Valid values are `"Open`" and `"Microsoft`".&]
[s3;%% &]
[s4; &]
[s5;:OfficeSheet`:`:AddSheet`(bool`): [@(0.0.255) bool]_[* AddSheet]([@(0.0.255) bool]_[*@3 v
isible])&]
[s2;%% Opens a new spreadsheet. If [%-*@3 visible].is true the spreadsheet 
application will be visible. If not all the rest of operations 
made over the spreadsheet will be hidden for the user.&]
[s3;%% &]
[s4; &]
[s5;:OfficeSheet`:`:OpenSheet`(String`,bool`): [@(0.0.255) bool]_[* OpenSheet]([_^String^ S
tring]_[*@3 fileName], [@(0.0.255) bool]_[*@3 visible])&]
[s2;%% Opens an existing spreadsheet file whose name is [%-*@3 fileName. 
]If [%-*@3 visible ]is true the spreadsheet application will be 
visible. If not all the rest of operations made over the spreadsheet 
will be hidden for the user.&]
[s3;%% &]
[s4; &]
[s5;:OfficeSheet`:`:SetValue`(int`,int`,Value`): [@(0.0.255) bool]_[* SetValue]([@(0.0.255) i
nt]_[*@3 col], [@(0.0.255) int]_[*@3 row], [_^Value^ Value]_[*@3 value])&]
[s2;%% Fills cell [%-*@3 col,] [%-*@3 row ][%- with the value] [%-*@3 value]. 
[%-*@3 col,] [%-*@3 row ]origin is (1, 1). [%-*@3 value ]is inserted 
using the most adequate format depending on the [%-*@3 value] type.&]
[s3;%% &]
[s4; &]
[s5;:OfficeSheet`:`:SetValue`(String`,Value`): [@(0.0.255) bool]_[* SetValue]([_^String^ St
ring]_[*@3 cell], [_^Value^ Value]_[*@3 value])&]
[s2;%% Fills cell [%-*@3 cell] [%- with the value ][%-*@3 value]. [%-*@3 cell 
]is a string that indicates the cell location with letters to 
indicate the column and a number to indicate the row as in `"B7`", 
following the format used in spreadsheets. [%-*@3 value ]is inserted 
using the most adequate format depending on the [%-*@3 value] type.&]
[s3;%% &]
[s4; &]
[s5;:OfficeSheet`:`:SetValue`(Value`): [@(0.0.255) bool]_[* SetValue]([_^Value^ Value]_[*@3 v
alue])&]
[s2;%% Fills the range previously selected by Select() with [%-*@3 value]. 
[%-*@3 value ]is inserted using the most adequate format depending 
on the [%-*@3 value] type.&]
[s3;%% &]
[s4; &]
[s5;:OfficeSheet`:`:GetValue`(int`,int`): [_^Value^ Value]_[* GetValue]([@(0.0.255) int]_[*@3 c
ol], [@(0.0.255) int]_[*@3 row])&]
[s2;%% Returns the value got in cell [%-*@3 col,] [%-*@3 row]. [%-*@3 col,] 
[%-*@3 row ]origin is (1, 1).&]
[s3;%% &]
[s4; &]
[s5;:OfficeSheet`:`:GetValue`(String`): [_^Value^ Value]_[* GetValue]([_^String^ String]_[*@3 c
ell])&]
[s2;%% Returns the value got in cell [%-*@3 cell]. [%-*@3 cell ]is a 
string that indicates the cell location with letters to indicate 
the column and a number to indicate the row as in `"B7`", following 
the format used in spreadsheets.&]
[s3;%% &]
[s4; &]
[s5;:OfficeSheet`:`:Replace`(Value`,Value`): [@(0.0.255) bool]_[* Replace]([_^Value^ Value]_
[*@3 search], [_^Value^ Value]_[*@3 replace])&]
[s2;%% Replaces value [%-*@3 search] with value [%-*@3 replace].in the 
selected range by Select() or all the document.&]
[s3;%% &]
[s4; &]
[s5;:OfficeSheet`:`:SetBold`(int`,int`,bool`): [@(0.0.255) bool]_[* SetBold]([@(0.0.255) in
t]_[*@3 col], [@(0.0.255) int]_[*@3 row], [@(0.0.255) bool]_[*@3 bold])&]
[s2;%% Sets or unsets cell bold letters in [%-*@3 col,] [%-*@3 row. col,] 
[%-*@3 row ]origin is (1, 1).&]
[s3;%% &]
[s4; &]
[s5;:OfficeSheet`:`:SetBold`(String`,bool`): [@(0.0.255) bool]_[* SetBold]([_^String^ Strin
g]_[*@3 cell], [@(0.0.255) bool]_[*@3 bold])&]
[s2;%% Sets or unsets cell bold letters in [%-*@3 cell. cell ]is a 
string that indicates the cell location with letters to indicate 
the column and a number to indicate the row as in `"B7`", following 
the format used in spreadsheets.&]
[s3;%% &]
[s4; &]
[s5;:OfficeSheet`:`:SetBold`(bool`): [@(0.0.255) bool]_[* SetBold]([@(0.0.255) bool]_[*@3 bol
d])&]
[s2;%% Sets or unsets cell bold letters in the selected range by 
Select() or all the document.&]
[s3;%% &]
[s4; &]
[s5;:OfficeSheet`:`:SetFont`(int`,int`,String`,int`): [@(0.0.255) bool]_[* SetFont]([@(0.0.255) i
nt]_[*@3 col], [@(0.0.255) int]_[*@3 row], [_^String^ String]_[*@3 name], 
[@(0.0.255) int]_[*@3 size])&]
[s2;%% Sets in cell [%-*@3 col,] [%-*@3 row ]font [%-*@3 name ]with [%-*@3 size 
]in points. [%-*@3 cell ]is a string that indicates the cell location 
with letters to indicate the column and a number to indicate 
the row as in `"B7`", following the format used in spreadsheets.&]
[s3;%% &]
[s4; &]
[s5;:OfficeSheet`:`:SetFont`(String`,String`,int`): [@(0.0.255) bool]_[* SetFont]([_^String^ S
tring]_[*@3 cell], [_^String^ String]_[*@3 name], [@(0.0.255) int]_[*@3 size])&]
[s2;%% Sets in [%-*@3 cell ]font [%-*@3 name ]with [%-*@3 size ]in points. 
[%-*@3 col,] [%-*@3 row ]origin is (1, 1).&]
[s3;%% &]
[s4; &]
[s5;:OfficeSheet`:`:SetFont`(String`,int`): [@(0.0.255) bool]_[* SetFont]([_^String^ String
]_[*@3 name], [@(0.0.255) int]_[*@3 size])&]
[s2;%% Sets the selected range by Select() or all the document with 
font [%-*@3 name] with [%-*@3 size ]in points. &]
[s3;%% &]
[s4; &]
[s5;:OfficeSheet`:`:Select`(String`): [@(0.0.255) bool]_[* Select]([_^String^ String]_[*@3 ra
nge])&]
[s2;%% Selects [%-*@3 range]. [%-*@3 range ]is a string that indicates 
the range location with letters to indicate the column and a 
number to indicate the row of the top left and bottom right vertex 
as in `"B7:D9`", following the format used in spreadsheets.&]
[s3;%% &]
[s4; &]
[s5;:OfficeSheet`:`:Select`(int`,int`,int`,int`): [@(0.0.255) bool]_[* Select]([@(0.0.255) i
nt]_[*@3 fromX], [@(0.0.255) int]_[*@3 fromY], [@(0.0.255) int]_[*@3 toX], 
[@(0.0.255) int]_[*@3 toY])&]
[s2;%% Selects range from top left vertex [%-*@3 fromX,] [%-*@3 fromY 
]to right bottom[%-*@3  toX,] [%-*@3 toY]. Origin is (1, 1).&]
[s3;%% &]
[s4; &]
[s5;:OfficeSheet`:`:Select`(`): [@(0.0.255) bool]_[* Select]()&]
[s2;%% Selects full sheet.&]
[s3; &]
[s4; &]
[s5;:OfficeSheet`:`:Print`(`): [@(0.0.255) bool]_[* Print]()&]
[s2;%% Prints the selected spreadsheet.&]
[s3; &]
[s4; &]
[s5;:OfficeSheet`:`:SaveAs`(String`,String`): [@(0.0.255) bool]_[* SaveAs]([_^String^ Strin
g]_[*@3 fileName], [_^String^ String]_[*@3 type]_`=_`"xls`")&]
[s2;%% Saves the opened [%-*@3 fileName] with format [%-*@3 type]. &]
[s2;%% Formats admitted depend on the implementation but are basically:&]
[s2;i150;O0;%% -|OpenOffice: -|`"xls`", `"pxl`", `"html`", `"txt`", 
`"pdf`" and `"ods`".&]
[s2;i150;O0;%% -|Microsoft: -|`"csv`", `"html`", `"txt`", `"xlsx`" and 
`"xls`"&]
[s4; &]
[s5;:OfficeSheet`:`:Quit`(`): [@(0.0.255) bool]_[* Quit]()&]
[s2;%% Closes the spreadsheet application.&]
[s2;%% -|It is also called by the class destructor&]
[s3; &]
[s4; &]
[s5;:OfficeSheet`:`:InsertTab`(String`): [@(0.0.255) bool]_[* InsertTab]([_^String^ String]_
[*@3 name])&]
[s2;%% Inserts a new tab with [%-*@3 name].&]
[s3;%% &]
[s4; &]
[s5;:OfficeSheet`:`:ChooseTab`(String`): [@(0.0.255) bool]_[* ChooseTab]([_^String^ String]_
[*@3 name])&]
[s2;%% Makes active tab with [%-*@3 name].&]
[s3;%% &]
[s4; &]
[s5;:OfficeSheet`:`:ChooseTab`(int`): [@(0.0.255) bool]_[* ChooseTab]([@(0.0.255) int]_[*@3 i
ndex])&]
[s2;%% Makes active tab with [%-*@3 index], beginning from 1.&]
[s3;%% &]
[s4; &]
[s5;:OfficeSheet`:`:RemoveTab`(String`): [@(0.0.255) bool]_[* RemoveTab]([_^String^ String]_
[*@3 name])&]
[s2;%% Remove tab with [%-*@3 name].&]
[s3;%% &]
[s4; &]
[s5;:OfficeSheet`:`:RemoveTab`(int`): [@(0.0.255) bool]_[* RemoveTab]([@(0.0.255) int]_[*@3 i
ndex])&]
[s2;%% Removes tab with [%-*@3 index], beginning from 1.&]
[s3;%% &]
[s4; &]
[s5;:OfficeSheet`:`:GetNumTabs`(`): [@(0.0.255) int]_[* GetNumTabs]()&]
[s2;%% Gets the number of tabs.&]
[s3; &]
[s4; &]
[s5;:OfficeSheet`:`:GetType`(`): [_^String^ String]_[* GetType]()&]
[s2;%% Returns the spreadsheet type.&]
[s2;%% Valid values are:&]
[s2;i150;O0;%% `"Open`"&]
[s2;i150;O0;%% `"Microsoft`"&]
[s3; &]
[s4; &]
[s5;:OfficeSheet`:`:CellToColRow`(const char`*`,int`&`,int`&`): [@(0.0.255) static] 
[@(0.0.255) void]_[* CellToColRow]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 cell], 
[@(0.0.255) int]_`&[*@3 col], [@(0.0.255) int]_`&[*@3 row])&]
[s2;%% Converts [%-*@3 cell] into [%-*@3 col] and [%-*@3 row]. [%-*@3 cell 
]is a string that indicates the cell location with letters to 
indicate the column and a number to indicate the row as in `"B7`", 
following the format used in spreadsheets. [%-*@3 col,] [%-*@3 row 
]origin is (1, 1)&]
[s3;%% &]
[s4; &]
[s5;:OfficeSheet`:`:ColRowToCell`(const int`,const int`): [@(0.0.255) static] 
[_^String^ String]_[* ColRowToCell]([@(0.0.255) const]_[@(0.0.255) int]_[*@3 col], 
[@(0.0.255) const]_[@(0.0.255) int]_[*@3 row])&]
[s2;%% Returns the cell in text represented by [%-*@3 col], [%-*@3 row]. 
Cell returned is a string that indicates the cell location with 
letters to indicate the column and a number to indicate the row 
as in `"B7`", following the format used in spreadsheets. [%-*@3 col,] 
[%-*@3 row ]origin is (1, 1)&]
[s3;%% &]
[s0; ]