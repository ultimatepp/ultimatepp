topic "OfficeDoc class reference";
[ $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9; $$1,0#37138531426314131252341829483380:structitem]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:codeitem]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 OfficeDoc]]}}&]
[s0;2 &]
[s0; [@(0.0.255)4 class][4 _][*4 OfficeDoc ][4 : ][@(0.0.255)4 public][4  ][*4 DocPlugin][4 , 
][@(0.0.255)4 public][4  ][*4 StaticPlugin]&]
[s0;*2 &]
[s0; [2 OfficeDoc class serves to manage documents made with OpenOffice 
Doc or Microsoft Office Word.]&]
[s0; [2 The class methods are designed to be as simple as possible 
and are exactly the same for both suites.]&]
[s0; [2 OfficeDoc handles the suites through `"Ole Automation`" so 
it requires either OpenOffice or Microsoft Office to be previosly 
installed in the computer.]&]
[s0;2 &]
[s0;2 &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s3; &]
[s5;:OfficeDoc`:`:OfficeDoc`(`): [* OfficeDoc]()&]
[s2;%% OfficeDoc constructor. It has no arguments as in function 
Init() it is defined which Office suite is going to be used.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:OfficeDoc`:`:`~OfficeDoc`(`): [@(0.0.255) `~][* OfficeDoc]()&]
[s2;%% OfficeDoc destructor. It closes document editor application.&]
[s3; &]
[s4;* &]
[s5;:OfficeDoc`:`:IsAvailable`(const char`*`): [@(0.0.255) bool]_[* IsAvailable]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 type])&]
[s2;%% It checks if office suit of name [%-*@3 type ]is available to 
be init. &]
[s2;%% Valid values are `"Open`" and `"Microsoft`".&]
[s3;%% &]
[s4; &]
[s5;:OfficeDoc`:`:Init`(const char`*`): [@(0.0.255) bool]_[* Init]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 type])&]
[s2;%% Initializes OfficeSheet to use [%-*@3 type].suite.&]
[s2;%% Valid values are `"Open`" and `"Microsoft`".&]
[s3;%% &]
[s4; &]
[s5;:OfficeDoc`:`:AddDoc`(bool`): [@(0.0.255) bool]_[* AddDoc]([@(0.0.255) bool]_[*@3 visible
])&]
[s2;%% Opens a new document. If [%-*@3 visible].is true the document 
application will be visible. If not all the rest of operations 
made over the document will be hidden for the user.&]
[s3;%% &]
[s4; &]
[s5;:OfficeDoc`:`:OpenDoc`(String`,bool`): [@(0.0.255) bool]_[* OpenDoc]([_^String^ String]_
[*@3 fileName], [@(0.0.255) bool]_[*@3 visible])&]
[s2;%% Opens an existing document file whose name is [%-*@3 fileName. 
]If [%-*@3 visible ]is true the document application will be visible. 
If not all the rest of operations made over the document will 
be hidden for the user.&]
[s3;%% &]
[s4; &]
[s5;:OfficeDoc`:`:SetFont`(String`,int`): [@(0.0.255) bool]_[* SetFont]([_^String^ String]_
[*@3 font], [@(0.0.255) int]_[*@3 size])&]
[s2;%% Sets the [%-*@3 font] type and [%-*@3 size ]in points. From now 
all the text entered will use the font and size defined.&]
[s3;%% &]
[s4; &]
[s5;:OfficeDoc`:`:SetBold`(bool`): [@(0.0.255) bool]_[* SetBold]([@(0.0.255) bool]_[*@3 bold])
&]
[s2;%% Sets or unsets text [%-*@3 bold ]letters. From now all the text 
entered will be bold or not depending on value [%-*@3 bold].&]
[s3;%% &]
[s4; &]
[s5;:OfficeDoc`:`:SetItalic`(bool`): [@(0.0.255) bool]_[* SetItalic]([@(0.0.255) bool]_[*@3 i
talic])&]
[s2;%% Sets or unsets text [%-*@3 italic].letters. From now all the 
text entered will be italic or not depending on value [%-*@3 italic].&]
[s3;%% &]
[s4; &]
[s5;:OfficeDoc`:`:WriteText`(String`): [@(0.0.255) bool]_[* WriteText]([_^String^ String]_[*@3 v
alue])&]
[s2;%% Writes the text [%-*@3 value].&]
[s3;%% &]
[s4; &]
[s5;:OfficeDoc`:`:Select`(`): [@(0.0.255) bool]_[* Select]()&]
[s2;%% Selects all document.&]
[s3; &]
[s4; &]
[s5;:OfficeDoc`:`:Replace`(String`,String`): [@(0.0.255) bool]_[* Replace]([_^String^ Strin
g]_[*@3 search], [_^String^ String]_[*@3 replace])&]
[s2;%% Replaces value [%-*@3 search] with value [%-*@3 replace].in all 
the document.&]
[s3;%% &]
[s4; &]
[s5;:OfficeDoc`:`:Print`(`): [@(0.0.255) bool]_[* Print]()&]
[s2;%% Prints the document.&]
[s3; &]
[s4; &]
[s5;:OfficeDoc`:`:SetSaved`(bool`): [@(0.0.255) bool]_[* SetSaved]([@(0.0.255) bool 
][*@3 saved])&]
[s2;%% Sets or unsets the flag [%-*@3 saved], so that the application 
did not ask for saving the document if the application receives 
the Quit() order.&]
[s3; &]
[s4; &]
[s5;:OfficeDoc`:`:SaveAs`(String`,String`): [@(0.0.255) bool]_[* SaveAs]([_^String^ String]_
[*@3 fileName], [_^String^ String]_[*@3 type]_`=_`"doc`")&]
[s2;%% Saves the opened [%-*@3 fileName] with format [%-*@3 type]. &]
[s2;%% Formats admitted depend on the implementation but are basically:&]
[s2;i160;O0;%% -|OpenOffice: -|`"doc`", `"rtf`", `"psw`", `"html`", 
`"txt`", `"pdf`" and `"odt`".&]
[s2;i150;O0;%% -|Microsoft: -|`"rtf`", `"html`", `"txt`", `"docx`" and 
`"doc`".&]
[s3;%% &]
[s4; &]
[s5;:OfficeDoc`:`:Quit`(`): [@(0.0.255) bool]_[* Quit]()&]
[s2;%% Closes the spreadsheet application.&]
[s2;%% -|It is also called by the class destructor&]
[s3; &]
[s4; &]
[s5;:OfficeDoc`:`:GetType`(`): [_^String^ String]_[* GetType]()&]
[s2;%% Returns the spreadsheet type.&]
[s2;%% Valid values are:&]
[s2;i150;O0;%% `"Open`"&]
[s2;i150;O0;%% `"Microsoft`"&]
[s0; ]