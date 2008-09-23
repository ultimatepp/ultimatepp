topic "LineEdit";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[0 $$7,0#96390100711032703541132217272105:end]
[H6;0 $$8,0#05600065144404261032431302351956:begin]
[{_} 
[s1;K:`:`:LineEdit`:`:class: [@(0.0.255) class]_[@0 LineEdit]_:_[@(0.0.255) public]_[^`:`:TextCtrl^ T
extCtrl]&]
[s2; &]
[s1;l224;K:`:`:LineEdit`:`:Highlight`:`:struct: [@(0.0.255) struct]_[@0 LineEdit`::Highlig
ht]_:_[@(0.0.255) public]_[^`:`:Moveable^ Moveable]<[@0 Highlight]>&]
[s5;l224;K:`:`:LineEdit`:`:Highlight`:`:paper: [^`:`:Color^ Color]_[@0 paper]&]
[s5;l224;K:`:`:LineEdit`:`:Highlight`:`:ink: [^`:`:Color^ Color]_[@0 ink]&]
[s5;l224;K:`:`:LineEdit`:`:Highlight`:`:font: [^`:`:Font^ Font]_[@0 font]&]
[s5;l224;K:`:`:LineEdit`:`:Highlight`:`:chr: [^`:`:wchar^ wchar]_[@0 chr]&]
[s0;l224; &]
[s0;l224;i192; This structure defines syntax highlighting properties 
of single character cell. See description of &]
[s5;l224;K@(0.0.255) &]
[s5;l224;K:`:`:LineEdit`:`:Highlight`:`:operator`=`=`(const`:`:LineEdit`:`:Highlight`&`)const: [@(0.0.255) b
ool]_[@0 operator`=`=]([@(0.0.255) const]_[^`:`:LineEdit`:`:Highlight^ Highlight]`&_[@3 h
])_[@(0.0.255) const]&]
[s2;l224;i192;%% Equality comparison.&]
[s0; &]
[s0; &]
[s5;K:`:`:LineEdit`:`:HighlightLine`(int`,`:`:Vector`<`:`:LineEdit`:`:Highlight`>`&`,int`): v
irtual [@(0.0.255) void]_[@0 HighlightLine]([@(0.0.255) int]_[@3 line], 
[^`:`:Vector^ Vector]<[^`:`:LineEdit`:`:Highlight^ Highlight]>`&_[@3 h], 
[@(0.0.255) int]_[@3 pos]) [@0 `[protected`]]&]
[s2;%% This method can be overridden to add syntax highlighting to 
the LineEdit. It is called once for each line painted on the 
screen. [%-*@3 line] is the line index (first line is 0), [%-*@3 h] 
represents character cells in the line; client code should change 
this array to achieve the highlighting, [%-*@3 pos] is the index 
of character from the beginning of text. [%-*@3 h] initially contains 
one more space character than is the count of characters in the 
line; this additional character can be used to set the appearance 
of the rest of line after the last character. You also add more 
characters to the [%-*@3 h]. Default implementation is empty.&]
[s0; &]
[s5;K:`:`:LineEdit`:`:NewScrollPos`(`): virtual [@(0.0.255) void]_[@0 NewScrollPos]() 
[@0 `[protected`]]&]
[s2;%% Called when scrollbar gets new position. Default implementation 
is empty.&]
[s0; &]
[s5;K:`:`:LineEdit`:`:GetFontSize`(`)const: [^`:`:Size^ Size]_[@0 GetFontSize]()_[@(0.0.255) c
onst]&]
[s2;%% Returns the size of font character cell (LineEdit expects 
monospace glyphs `-  the width of character is constant).&]
[s0; &]
[s5;K:`:`:LineEdit`:`:GetGPos`(int`,int`)const: [@(0.0.255) int]_[@0 GetGPos]([@(0.0.255) i
nt]_[@3 ln], [@(0.0.255) int]_[@3 cl])_[@(0.0.255) const]&]
[s2;%% Returns `"graphical`" position of [%-*@3 ln] line and [%-*@3 cl] 
column. This takes into account any tabulator characters int 
the line.&]
[s0; &]
[s5;K:`:`:LineEdit`:`:GetMousePos`(`:`:Point`)const: [@(0.0.255) int]_[@0 GetMousePos]([^`:`:Point^ P
oint]_[@3 p])_[@(0.0.255) const]&]
[s2;%% Get the the offset of character placed at [%-*@3 p].&]
[s0; &]
[s5;K:`:`:LineEdit`:`:GetColumnLine`(int`)const: [^`:`:Point^ Point]_[@0 GetColumnLine]([@(0.0.255) i
nt]_[@3 pos])_[@(0.0.255) const]&]
[s2;%% Returns the line and column for the character at [%-*@3 pos] 
accounting for any tabulators. Column is x member of resulting 
Point, line is y.&]
[s0; &]
[s5;K:`:`:LineEdit`:`:GetColumnLinePos`(`:`:Point`)const: [@(0.0.255) int]_[@0 GetColumnL
inePos]([^`:`:Point^ Point]_[@3 pos])_[@(0.0.255) const]&]
[s2;%% Returns the position for given column and line [%-*@3 pos]. 
Does account for tabulators.&]
[s0; &]
[s5;K:`:`:LineEdit`:`:GetIndexLine`(int`)const: [^`:`:Point^ Point]_[@0 GetIndexLine]([@(0.0.255) i
nt]_[@3 pos])_[@(0.0.255) const]&]
[s2;%% Returns the line and index of character in the line for the 
given [%-*@3 pos]. Does not account for tabulators.&]
[s0; &]
[s5;K:`:`:LineEdit`:`:GetIndexLinePos`(`:`:Point`)const: [@(0.0.255) int]_[@0 GetIndexLin
ePos]([^`:`:Point^ Point]_[@3 pos])_[@(0.0.255) const]&]
[s2;%% Returns the position for given column and line [%-*@3 pos]. 
Does not account for tabulators.&]
[s0; &]
[s5;K:`:`:LineEdit`:`:ScrollUp`(`): [@(0.0.255) void]_[@0 ScrollUp]()&]
[s2;%% Scrolls the text single line up.&]
[s0; &]
[s5;K:`:`:LineEdit`:`:ScrollDown`(`): [@(0.0.255) void]_[@0 ScrollDown]()&]
[s2;%% Scrolls the text single line down.&]
[s0; &]
[s5;K:`:`:LineEdit`:`:GetLineScreenRect`(int`)const: [^`:`:Rect^ Rect]_[@0 GetLineScreenR
ect]([@(0.0.255) int]_[@3 line])_[@(0.0.255) const]&]
[s2;%% Gets the absolute screen rectangle position of [%-*@3 line].&]
[s0; &]
[s5;K:`:`:LineEdit`:`:TopCursor`(`): [@(0.0.255) void]_[@0 TopCursor]()&]
[s2;%% Scrolls the text to place the line with cursor is first in 
the view.&]
[s0; &]
[s5;K:`:`:LineEdit`:`:CenterCursor`(`): [@(0.0.255) void]_[@0 CenterCursor]()&]
[s2;%% Scrolls the text to place the line with cursor at the center 
of the view.&]
[s0; &]
[s5;K:`:`:LineEdit`:`:MoveUpDown`(int`,bool`): [@(0.0.255) void]_[@0 MoveUpDown]([@(0.0.255) i
nt]_[@3 n], [@(0.0.255) bool]_[@3 sel]_`=_false)&]
[s2;%% Moves the cursor [%-*@3 n] lines up ([%-*@3 n] is negative) or 
down. If [%-*@3 sel] is true, selects characters between starting 
and ending cursor position.&]
[s0; &]
[s5;K:`:`:LineEdit`:`:MoveLeft`(bool`): [@(0.0.255) void]_[@0 MoveLeft]([@(0.0.255) bool]_[@3 s
el]_`=_false)&]
[s5;K:`:`:LineEdit`:`:MoveRight`(bool`): [@(0.0.255) void]_[@0 MoveRight]([@(0.0.255) bool]_
[@3 sel]_`=_false)&]
[s5;K:`:`:LineEdit`:`:MoveUp`(bool`): [@(0.0.255) void]_[@0 MoveUp]([@(0.0.255) bool]_[@3 sel
]_`=_false)&]
[s5;K:`:`:LineEdit`:`:MoveDown`(bool`): [@(0.0.255) void]_[@0 MoveDown]([@(0.0.255) bool]_[@3 s
el]_`=_false)&]
[s5;K:`:`:LineEdit`:`:MovePageUp`(bool`): [@(0.0.255) void]_[@0 MovePageUp]([@(0.0.255) boo
l]_[@3 sel]_`=_false)&]
[s5;K:`:`:LineEdit`:`:MovePageDown`(bool`): [@(0.0.255) void]_[@0 MovePageDown]([@(0.0.255) b
ool]_[@3 sel]_`=_false)&]
[s5;K:`:`:LineEdit`:`:MoveHome`(bool`): [@(0.0.255) void]_[@0 MoveHome]([@(0.0.255) bool]_[@3 s
el]_`=_false)&]
[s5;K:`:`:LineEdit`:`:MoveEnd`(bool`): [@(0.0.255) void]_[@0 MoveEnd]([@(0.0.255) bool]_[@3 s
el]_`=_false)&]
[s5;K:`:`:LineEdit`:`:MoveTextBegin`(bool`): [@(0.0.255) void]_[@0 MoveTextBegin]([@(0.0.255) b
ool]_[@3 sel]_`=_false)&]
[s5;K:`:`:LineEdit`:`:MoveTextEnd`(bool`): [@(0.0.255) void]_[@0 MoveTextEnd]([@(0.0.255) b
ool]_[@3 sel]_`=_false)&]
[s0;%% [%- -|Moves the cursor in specified direction (Home/End are beginning/end 
of line). ]If [%-*@3 sel] is true, selects characters between starting 
and ending cursor position.&]
[s0; &]
[s5;K:`:`:LineEdit`:`:InsertChar`(`:`:dword`,int`,bool`): [@(0.0.255) bool]_[@0 InsertCha
r]([^`:`:dword^ dword]_[@3 key], [@(0.0.255) int]_[@3 count]_`=_[@3 1], 
[@(0.0.255) bool]_[@3 canoverwrite]_`=_false)&]
[s2;%% Inserts  [%-*@3 count ]characters [%-*@3 key] at cursor position. 
If [%-*@3 canoverwrite] is true, overwrite mode (as set by user 
pressing Insert key) can be used.&]
[s0; &]
[s5;K:`:`:LineEdit`:`:DeleteChar`(`): [@(0.0.255) void]_[@0 DeleteChar]()&]
[s2;%% Deletes character at cursor position.&]
[s0; &]
[s5;K:`:`:LineEdit`:`:Backspace`(`): [@(0.0.255) void]_[@0 Backspace]()&]
[s2;%% If character is not first in the text, deletes it and moves 
cursor one position back.&]
[s0; &]
[s5;K:`:`:LineEdit`:`:DeleteLine`(`): [@(0.0.255) void]_[@0 DeleteLine]()&]
[s2;%% Deletes a line with cursor (if it is not the only line in 
the text).&]
[s0; &]
[s5;K:`:`:LineEdit`:`:CutLine`(`): [@(0.0.255) void]_[@0 CutLine]()&]
[s2;%% Stores a line to clipboard and calls DeleteLine.&]
[s0; &]
[s5;K:`:`:LineEdit`:`:GetScrollPos`(`)const: [^`:`:Point^ Point]_[@0 GetScrollPos]()_[@(0.0.255) c
onst]&]
[s2;%% Returns the actual scroll position.&]
[s0; &]
[s5;K:`:`:LineEdit`:`:GetPageSize`(`): [^`:`:Size^ Size]_[@0 GetPageSize]()&]
[s2;%% Returns the current view size in character cells.&]
[s0; &]
[s5;K:`:`:LineEdit`:`:SetScrollPos`(`:`:Point`): [@(0.0.255) void]_[@0 SetScrollPos]([^`:`:Point^ P
oint]_[@3 p])&]
[s2;%% Sets the scroll position of view, e.g. to the value obtained 
by GetScrollPos.&]
[s0; &]
[s5;K:`:`:LineEdit`:`:GetEditPos`(`)const: [^`:`:LineEdit`:`:EditPos^ EditPos]_[@0 GetEdi
tPos]()_[@(0.0.255) const]&]
[s2;%% Returns the edit position in the text to be restored later. 
Edit position comprises cursor and scrollbar positions.&]
[s0; &]
[s5;K:`:`:LineEdit`:`:SetEditPos`(const`:`:LineEdit`:`:EditPos`&`): [@(0.0.255) void]_[@0 S
etEditPos]([@(0.0.255) const]_[^`:`:LineEdit`:`:EditPos^ LineEdit`::EditPos]`&_[@3 pos])
&]
[s2;%% Sets the edit position obtained by calling GetEditPos. If 
cursor is not in the view after restoring, view is scrolled so 
that it is by calling ScrollIntoCursor.&]
[s0; &]
[s5;K:`:`:LineEdit`:`:SetEditPosSb`(const`:`:LineEdit`:`:EditPos`&`): [@(0.0.255) void]_
[@0 SetEditPosSb]([@(0.0.255) const]_[^`:`:LineEdit`:`:EditPos^ LineEdit`::EditPos]`&_[@3 p
os])&]
[s2;%% Sets the edit position obtained by calling GetEditPos. Unlike 
SetEditPos, no scrolls are performed to get cursor into the view.&]
[s0; &]
[s5;K:`:`:LineEdit`:`:ScrollIntoCursor`(`): [@(0.0.255) void]_[@0 ScrollIntoCursor]()&]
[s2;%% Performs minimal scroll to get the cursor into the view. `"Minimal`" 
means that if cursor is `"before`" the view, view is scrolled 
to move cursor to the first line, if it is `"after`", it is moved 
to the last line.&]
[s0; &]
[s5;K:`:`:LineEdit`:`:GetCaretPoint`(`)const: [^`:`:Point^ Point]_[@0 GetCaretPoint]()_[@(0.0.255) c
onst]&]
[s2;%%   &]
[s0; &]
[s5;K:`:`:LineEdit`:`:Clear`(`): [@(0.0.255) void]_[@0 Clear]()&]
[s2;%% Sets the text empty.&]
[s0; &]
[s5;K:`:`:LineEdit`:`:OverWriteMode`(bool`): [@(0.0.255) void]_[@0 OverWriteMode]([@(0.0.255) b
ool]_[@3 o]_`=_true)&]
[s5;K:`:`:LineEdit`:`:IsOverWriteMode`(`)const: [@(0.0.255) bool]_[@0 IsOverWriteMode]()_
[@(0.0.255) const]&]
[s2;%% In OverWriteMode mode, input characters replace the content 
instead of being inserted.&]
[s0; &]
[s5;K:`:`:LineEdit`:`:RefreshChars`(bool`(`*`)`(int c`)`): [@(0.0.255) void]_[@0 RefreshC
hars]([@(0.0.255) bool]_(`*[@3 predicate])([@(0.0.255) int]_c))&]
[s2;%% Calls [%-*@3 predicate] for all characters in current view and 
when it returns true refreshes (schedules for repainting) character 
cell. It is useful for advanced code editors (e.g. refreshing 
color of braces in TheIDE editor).&]
[s0; &]
[s5;K:`:`:LineEdit`:`:TabSize`(int`): [^`:`:LineEdit^ LineEdit]`&_[@0 TabSize]([@(0.0.255) i
nt]_[@3 n])&]
[s2;%% Sets the tabulator size.&]
[s0; &]
[s5;K:`:`:LineEdit`:`:GetTabSize`(`)const: [@(0.0.255) int]_[@0 GetTabSize]()_[@(0.0.255) c
onst]&]
[s2;%% Returns current tabulator size.&]
[s0; &]
[s5;K:`:`:LineEdit`:`:BorderColumn`(int`,`:`:Color`): [^`:`:LineEdit^ LineEdit]`&_[@0 Bor
derColumn]([@(0.0.255) int]_[@3 col], [^`:`:Color^ Color]_[@3 c]_`=_SColorFace())&]
[s2;%%   [%-*@3 col] [%-*@3 c] &]
[s0; &]
[s5;K:`:`:LineEdit`:`:SetFont`(`:`:Font`): [^`:`:LineEdit^ LineEdit]`&_[@0 SetFont]([^`:`:Font^ F
ont]_[@3 f])&]
[s2;%% Sets the font. HighlightLine can replace this font, however, 
the size of character cell is determined by this font (note that 
CJK ideograms are two character cells wide). Font should be monospaced. 
Returns `*this. Default font is Courier(16).&]
[s0; &]
[s5;K:`:`:LineEdit`:`:GetFont`(`)const: [^`:`:Font^ Font]_[@0 GetFont]()_[@(0.0.255) const]&]
[s2;%% Returns the font.&]
[s0; &]
[s5;K:`:`:LineEdit`:`:NoHorzScrollbar`(bool`): [^`:`:LineEdit^ LineEdit]`&_[@0 NoHorzScro
llbar]([@(0.0.255) bool]_[@3 b]_`=_true)&]
[s2;%% In this mode horizontal scrollbar is never shown. Default 
is off.&]
[s0; &]
[s5;K:`:`:LineEdit`:`:ShowTabs`(bool`): [^`:`:LineEdit^ LineEdit]`&_[@0 ShowTabs]([@(0.0.255) b
ool]_[@3 st]_`=_true)&]
[s5;K:`:`:LineEdit`:`:IsShowTabs`(`)const: [@(0.0.255) bool]_[@0 IsShowTabs]()_[@(0.0.255) c
onst]&]
[s2;%% In this mode widget displays tabulators with faint graphics. 
Default is off.&]
[s0; &]
[s5;K:`:`:LineEdit`:`:WithCutLine`(bool`): [^`:`:LineEdit^ LineEdit]`&_[@0 WithCutLine]([@(0.0.255) b
ool]_[@3 b])&]
[s5;K:`:`:LineEdit`:`:NoCutLine`(`): [^`:`:LineEdit^ LineEdit]`&_[@0 NoCutLine]()&]
[s2;%% In this mode widget calls CutLine when user presses Ctrl`+Y 
or Ctrl`+L. Default is on.&]
[s0; ]