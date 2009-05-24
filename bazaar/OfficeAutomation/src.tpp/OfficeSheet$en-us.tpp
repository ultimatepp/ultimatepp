topic "class OfficeSheet";
[ $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9; $$1,0#37138531426314131252341829483380:structitem]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:codeitem]
[{_} 
[s1;:OfficeSheet`:`:class: [@(0.0.255) class]_[* OfficeSheet]&]
[s2;%% &]
[s0; OfficeSheet class serves to manage spreadsheets made with OpenOffice 
or Microsoft Office.&]
[s0; The class methods are designed to be as simple as possible and 
are exactly the same for using both suites.&]
[s0; OfficeSuite handles the suites through `"Ole Automation`" so 
it requires either OpenOffice or Microsoft Office to be previosly 
installed in the computer.&]
[s0;2 &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s4; &]
[s5;:OfficeSheet`:`:OfficeSheet`(`): [* OfficeSheet]()&]
[s2;%% OfficeSheet constructor. It has no arguments as in function 
Init() it is defined which Office suit is going to be used.&]
[s0;i448;a25;kKO9;:noref:@(0.0.255)2 &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s4; &]
[s5;:OfficeSheet`:`:IsAvailable`(String`): [@(0.0.255) static] [@(0.0.255) bool]_[* IsAvail
able]([_^String^ String]_[*@3 type])&]
[s2;%% It checks if office suit of name [%-*@3 type].is availoable 
to be init. &]
[s2;%% Valid values are `"Open`" and `"Microsoft`".&]
[s3;%% &]
[s4; &]
[s5;:OfficeSheet`:`:Init`(String`): [@(0.0.255) bool]_[* Init]([_^String^ String]_[*@3 type])
&]
[s2;%% Initializes OfficeSheet to use [%-*@3 type].suite.&]
[s2;%% Valid values are `"Open`" and `"Microsoft`".&]
[s0;%% &]
[s4; &]
[s5;:OfficeSheet`:`:AddSheet`(bool`): [@(0.0.255) bool]_[* AddSheet]([@(0.0.255) bool]_[*@3 v
isible])&]
[s2;%% Opens a new spreadsheet. If [%-*@3 visible].is true the spreadsheet 
will be visible. If not all the rest of operations made over 
the spreadsheet will be hidden for the user.&]
[s3;%% &]
[s4; &]
[s5;:OfficeSheet`:`:OpenSheet`(String`,bool`): [@(0.0.255) bool]_[* OpenSheet]([_^String^ S
tring]_[*@3 fileName], [@(0.0.255) bool]_[*@3 visible])&]
[s2;%% Opens an existing spreadsheet file which name is [%-*@3 fileName. 
]If [%-*@3 visible ]is true the spreadsheet will be visible. If 
not all the rest of operations made over the spreadsheet will 
be hidden for the user..&]
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
[s2;%% Set or unset cell bold letters in [%-*@3 col,] [%-*@3 row. col,] 
[%-*@3 row ]origin is (1, 1).&]
[s3;%% &]
[s4; &]
[s5;:OfficeSheet`:`:SetBold`(String`,bool`): [@(0.0.255) bool]_[* SetBold]([_^String^ Strin
g]_[*@3 cell], [@(0.0.255) bool]_[*@3 bold])&]
[s2;%% Set or unset cell bold letters in [%-*@3 cell. cell ]is a string 
that indicates the cell location with letters to indicate the 
column and a number to indicate the row as in `"B7`", following 
the format used in spreadsheets.&]
[s3;%% &]
[s4; &]
[s5;:OfficeSheet`:`:SetBold`(bool`): [@(0.0.255) bool]_[* SetBold]([@(0.0.255) bool]_[*@3 bol
d])&]
[s2;%% Set or unset cell bold letters in the selected range by Select() 
or all the document.&]
[s3;%% &]
[s4; &]
[s5;:OfficeSheet`:`:Print`(`): [@(0.0.255) bool]_[* Print]()&]
[s2;%% Prints the selected spreadsheet.&]
[s3; &]
[s0; ]