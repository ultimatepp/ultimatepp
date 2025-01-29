topic "LineEdit";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_} 
[ {{10000@3 [s0;%% [*@(229)4 LineEdit]]}}&]
[s3; &]
[s1;:LineEdit`:`:class: [@(0.0.255)3 class][3 _][*3 LineEdit][3 _:_][@(0.0.255)3 public][3 _][*@3;3 T
extCtrl]&]
[s9;%% A widget used to edit a line of a document one at a time.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Protected Method List]]}}&]
[s3; &]
[s5;:LineEdit`:`:HighlightLine`(int`,Vector`<LineEdit`:`:Highlight`>`&`,int`): [@(0.0.255) v
irtual] [@(0.0.255) void]_[* HighlightLine]([@(0.0.255) int]_[*@3 line], 
[_^Vector^ Vector][@(0.0.255) <][_^LineEdit`:`:Highlight^ Highlight][@(0.0.255) >`&]_[*@3 h
], [@(0.0.255) int]_[*@3 pos])&]
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
[s3; &]
[s4; &]
[s5;:LineEdit`:`:NewScrollPos`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* NewScrollPos](
)&]
[s2;%% Called when scrollbar gets new position. Default implementation 
is empty.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Member List]]}}&]
[s3;%% &]
[s5;:Upp`:`:LineEdit`:`:WhenScroll: [_^Upp`:`:Event^ Event]<>_[* WhenScroll]&]
[s2;%% Invoked when scrollbar gets new position.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:LineEdit`:`:WhenLeftUp: Event<> [* WhenLeftUp]&]
[s2;%% Invoked when left mouse button is released.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:GetFontSize`(`)const: [_^Size^ Size]_[* GetFontSize]()_[@(0.0.255) const]&]
[s2;%% Returns the size of font character cell (LineEdit expects 
monospace glyphs `-  the width of character is constant).&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:GetGPos`(int`,int`)const: [@(0.0.255) int]_[* GetGPos]([@(0.0.255) int]_[*@3 l
n], [@(0.0.255) int]_[*@3 cl])_[@(0.0.255) const]&]
[s2;%% Returns `"graphical`" position of [%-*@3 ln] line and [%-*@3 cl] 
column. This takes into account any tabulator characters in the 
line.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:GetMousePos`(Point`)const: [@(0.0.255) int]_[* GetMousePos]([_^Point^ Poin
t]_[*@3 p])_[@(0.0.255) const]&]
[s2;%% Get the the offset of character placed at [%-*@3 p].&]
[s3; &]
[s4; &]
[s5;:Upp`:`:LineEdit`:`:GetColumnLine`(int64`)const: Point [* GetColumnLine](int64 
[*@3 pos]) [@(0.0.255) const]&]
[s2;%% Returns the line and column for the character at [%-*@3 pos] 
accounting for any tabulators. Column is x member of resulting 
Point, line is y.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:GetColumnLinePos`(Point`)const: [@(0.0.255) int]_[* GetColumnLinePos]([_^Point^ P
oint]_[*@3 pos])_[@(0.0.255) const]&]
[s2;%% Returns the position for given column and line [%-*@3 pos]. 
Does account for tabulators.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:LineEdit`:`:GetIndexLine`(int64`)const: Point [* GetIndexLine](int64 
[*@3 pos]) [@(0.0.255) const]&]
[s2;%% Returns the line and index of character in the line for the 
given [%-*@3 pos]. Does not account for tabulators.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:GetIndexLinePos`(Point`)const: [@(0.0.255) int]_[* GetIndexLinePos]([_^Point^ P
oint]_[*@3 pos])_[@(0.0.255) const]&]
[s2;%% Returns the position for given column and line [%-*@3 pos]. 
Does not account for tabulators.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:LineEdit`:`:SetRectSelection`(int64`,int64`): [@(0.0.255) void] 
[* SetRectSelection](int64 [*@3 l], int64 [*@3 h])&]
[s2;%% Sets rectangular selection.&]
[s3;%% &]
[s4; &]
[s5;:LineEdit`:`:SetRectSelection`(const Rect`&`): [@(0.0.255) void]_[* SetRectSelection](
[@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 rect])&]
[s2;%% Same as SetRectSelection(GetGPos(rect.top, rect.left), GetGPos(rect.bottom, 
rect.right));&]
[s3;%% &]
[s4; &]
[s5;:LineEdit`:`:GetRectSelection`(`)const: [_^Rect^ Rect]_[* GetRectSelection]()_[@(0.0.255) c
onst]&]
[s2;%% Returns rectangular selection (as `"graphical`").&]
[s3; &]
[s4; &]
[s5;:Upp`:`:LineEdit`:`:GetRectSelection`(const Rect`&`,int`,int64`&`,int64`&`): [@(0.0.255) b
ool] [* GetRectSelection]([@(0.0.255) const] Rect[@(0.0.255) `&] [*@3 rect], 
[@(0.0.255) int] [*@3 line], int64[@(0.0.255) `&] [*@3 l], int64[@(0.0.255) `&] 
[*@3 h])&]
[s2;%% Returns lower and upper limits [%-*@3 l] [%-*@3 h] of characters 
of [%-*@3 line] that are in rectangular selection [%-*@3 rect]. Returns 
false when line is not in selection.&]
[s3;%% &]
[s4; &]
[s5;:LineEdit`:`:ScrollUp`(`): [@(0.0.255) void]_[* ScrollUp]()&]
[s2;%% Scrolls the text single line up.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:ScrollDown`(`): [@(0.0.255) void]_[* ScrollDown]()&]
[s2;%% Scrolls the text single line down.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:ScrollPageUp`(`): [@(0.0.255) void]_[* ScrollPageUp]()&]
[s2;%% Scrolls the text one page up.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:ScrollPageDown`(`): [@(0.0.255) void]_[* ScrollPageDown]()&]
[s2;%% Scrolls the text one page down.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:ScrollBegin`(`): [@(0.0.255) void]_[* ScrollBegin]()&]
[s2;%% Scrolls the text to the begin.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:ScrollEnd`(`): [@(0.0.255) void]_[* ScrollEnd]()&]
[s2;%% Scrolls the text to the end.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:GetLineScreenRect`(int`)const: [_^Rect^ Rect]_[* GetLineScreenRect]([@(0.0.255) i
nt]_[*@3 line])_[@(0.0.255) const]&]
[s2;%% Gets the absolute screen rectangle position of [%-*@3 line].&]
[s5;K:`:`:LineEdit`:`:TopCursor`(`): [@(0.0.255) void][@(64) _]TopCursor[@(64) ()]&]
[s2;%% Scrolls the text to place the line with cursor is first in 
the view.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:TopCursor`(int`): [@(0.0.255) void]_[* TopCursor]([@(0.0.255) int]_[*@3 line
s]_`=_[@3 0])&]
[s2;%% Scrolls the text to place the line with cursor placed [%-*@3 lines] 
from the top.&]
[s3;%% &]
[s4; &]
[s5;:LineEdit`:`:CenterCursor`(`): [@(0.0.255) void]_[* CenterCursor]()&]
[s2;%% Scrolls the text to place the line with cursor at the center 
of the view.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:MoveUpDown`(int`,bool`): [@(0.0.255) void]_[* MoveUpDown]([@(0.0.255) int]_
[*@3 n], [@(0.0.255) bool]_[*@3 sel]_`=_[@(0.0.255) false])&]
[s2;%% Moves the cursor [%-*@3 n] lines up ([%-*@3 n] is negative) or 
down. If [%-*@3 sel] is true, selects characters between starting 
and ending cursor position.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:MoveLeft`(bool`): [@(0.0.255) void]_[* MoveLeft]([@(0.0.255) bool]_[*@3 sel]_
`=_[@(0.0.255) false])&]
[s5;:LineEdit`:`:MoveRight`(bool`): [@(0.0.255) void]_[* MoveRight]([@(0.0.255) bool]_[*@3 se
l]_`=_[@(0.0.255) false])&]
[s5;:LineEdit`:`:MoveUp`(bool`): [@(0.0.255) void]_[* MoveUp]([@(0.0.255) bool]_[*@3 sel]_`=_
[@(0.0.255) false])&]
[s5;:LineEdit`:`:MoveDown`(bool`): [@(0.0.255) void]_[* MoveDown]([@(0.0.255) bool]_[*@3 sel]_
`=_[@(0.0.255) false])&]
[s5;:LineEdit`:`:MovePageUp`(bool`): [@(0.0.255) void]_[* MovePageUp]([@(0.0.255) bool]_[*@3 s
el]_`=_[@(0.0.255) false])&]
[s5;:LineEdit`:`:MovePageDown`(bool`): [@(0.0.255) void]_[* MovePageDown]([@(0.0.255) bool]_
[*@3 sel]_`=_[@(0.0.255) false])&]
[s5;:LineEdit`:`:MoveHome`(bool`): [@(0.0.255) void]_[* MoveHome]([@(0.0.255) bool]_[*@3 sel]_
`=_[@(0.0.255) false])&]
[s5;:LineEdit`:`:MoveEnd`(bool`): [@(0.0.255) void]_[* MoveEnd]([@(0.0.255) bool]_[*@3 sel]_`=
_[@(0.0.255) false])&]
[s5;:LineEdit`:`:MoveTextBegin`(bool`): [@(0.0.255) void]_[* MoveTextBegin]([@(0.0.255) boo
l]_[*@3 sel]_`=_[@(0.0.255) false])&]
[s5;:LineEdit`:`:MoveTextEnd`(bool`): [@(0.0.255) void]_[* MoveTextEnd]([@(0.0.255) bool]_[*@3 s
el]_`=_[@(0.0.255) false])&]
[s2;%% [%- Moves the cursor in specified direction (Home/End are beginning/end 
of line). ]If [%-*@3 sel] is true, selects characters between starting 
and ending cursor position.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:InsertChar`(dword`,int`,bool`): [@(0.0.255) bool]_[* InsertChar]([_^dword^ d
word]_[*@3 key], [@(0.0.255) int]_[*@3 count]_`=_[@3 1], [@(0.0.255) bool]_[*@3 canoverwrite]_
`=_[@(0.0.255) false])&]
[s2;%% Inserts  [%-*@3 count ]characters [%-*@3 key] at cursor position. 
If [%-*@3 canoverwrite] is true, overwrite mode (as set by user 
pressing Insert key) can be used.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:DeleteChar`(`): [@(0.0.255) void]_[* DeleteChar]()&]
[s2;%% Deletes character at cursor position.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:Backspace`(`): [@(0.0.255) void]_[* Backspace]()&]
[s2;%% If character is not first in the text, deletes it and moves 
cursor one position back.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:DeleteLine`(`): [@(0.0.255) void]_[* DeleteLine]()&]
[s2;%% Deletes a line with cursor (if it is not the only line in 
the text).&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:CutLine`(`): [@(0.0.255) void]_[* CutLine]()&]
[s2;%% Stores a line to clipboard and calls DeleteLine.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:PasteColumn`(const WString`&`): [@(0.0.255) void]_[* PasteColumn]([@(0.0.255) c
onst]_[_^WString^ WString][@(0.0.255) `&]_[*@3 text])&]
[s2;%% Pastes lines of [%-*@3 text] into actual graphical column of 
text. Returns final cursor position.&]
[s3;%% &]
[s4; &]
[s5;:LineEdit`:`:PasteColumn`(`): [@(0.0.255) void]_[* PasteColumn]()&]
[s2;%% PasteColumn with clipboard text.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:Sort`(`): [@(0.0.255) void]_[* Sort]()&]
[s2;%% Sorts lines based on rectangular selection.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:GetScrollPos`(`)const: [_^Point^ Point]_[* GetScrollPos]()_[@(0.0.255) con
st]&]
[s2;%% Returns the actual scroll position.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:GetPageSize`(`): [_^Size^ Size]_[* GetPageSize]()&]
[s2;%% Returns the current view size in character cells.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:SetScrollPos`(Point`): [@(0.0.255) void]_[* SetScrollPos]([_^Point^ Point]_
[*@3 p])&]
[s2;%% Sets the scroll position of view, e.g. to the value obtained 
by GetScrollPos.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:GetEditPos`(`)const: [_^LineEdit`:`:EditPos^ EditPos]_[* GetEditPos]()_[@(0.0.255) c
onst]&]
[s2;%% Returns the edit position in the text to be restored later. 
Edit position comprises cursor and scrollbar positions.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:LineEdit`:`:SetEditPos`(const Upp`:`:TextCtrl`:`:EditPos`&`): [@(0.0.255) v
oid]_[* SetEditPos]([@(0.0.255) const]_[_^Upp`:`:TextCtrl`:`:EditPos^ EditPos][@(0.0.255) `&
]_[*@3 pos])&]
[s2;%% Sets the edit position obtained by calling GetEditPos. If 
cursor is not in the view after restoring, view is scrolled so 
that it is by calling ScrollIntoCursor.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:SetEditPosSb`(const LineEdit`:`:EditPos`&`): [@(0.0.255) void]_[* SetEdi
tPosSb]([@(0.0.255) const]_[_^LineEdit`:`:EditPos^ LineEdit`::EditPos][@(0.0.255) `&]_[*@3 p
os])&]
[s2;%% Sets the edit position obtained by calling GetEditPos. Unlike 
SetEditPos, no scrolls are performed to get cursor into the view.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:LineEdit`:`:SetEditPosSbOnly`(const Upp`:`:LineEdit`:`:EditPos`&`): [@(0.0.255) v
oid]_[* SetEditPosSbOnly]([@(0.0.255) const]_[_^Upp`:`:LineEdit`:`:EditPos^ LineEdit`::E
ditPos][@(0.0.255) `&]_[*@3 pos])&]
[s2;%% Sets only scrollbar position of [%-*@3 pos].&]
[s3;%% &]
[s4; &]
[s5;:LineEdit`:`:ScrollIntoCursor`(`): [@(0.0.255) void]_[* ScrollIntoCursor]()&]
[s2;%% Performs minimal scroll to get the cursor into the view. `"Minimal`" 
means that if cursor is `"before`" the view, view is scrolled 
to move cursor to the first line, if it is `"after`", it is moved 
to the last line.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:GetCaretPoint`(`)const: [_^Point^ Point]_[* GetCaretPoint]()_[@(0.0.255) c
onst]&]
[s2;%% Returns the current caret position.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%% Sets the text empty.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:OverWriteMode`(bool`): [@(0.0.255) void]_[* OverWriteMode]([@(0.0.255) boo
l]_[*@3 o]_`=_[@(0.0.255) true])&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:IsOverWriteMode`(`)const: [@(0.0.255) bool]_[* IsOverWriteMode]()_[@(0.0.255) c
onst]&]
[s2;%% In OverWriteMode mode, input characters replace the content 
instead of being inserted.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:RefreshChars`(bool`(`*`)`(int c`)`): [@(0.0.255) void]_[* RefreshChars](
[@(0.0.255) bool]_(`*[*@3 predicate])([@(0.0.255) int]_c))&]
[s2;%% Calls [%-*@3 predicate] for all characters in current view and 
when it returns true refreshes (schedules for repainting) character 
cell. It is useful for advanced code editors (e.g. refreshing 
color of braces in TheIDE editor).&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:TabSize`(int`): [_^LineEdit^ LineEdit][@(0.0.255) `&]_[* TabSize]([@(0.0.255) i
nt]_[*@3 n])&]
[s2;%% Sets the tabulator size.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:GetTabSize`(`)const: [@(0.0.255) int]_[* GetTabSize]()_[@(0.0.255) const]&]
[s2;%% Returns current tabulator size.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:BorderColumn`(int`,Color`): [_^LineEdit^ LineEdit][@(0.0.255) `&]_[* Borde
rColumn]([@(0.0.255) int]_[*@3 col], [_^Color^ Color]_[*@3 c]_`=_SColorFace())&]
[s2;%% Displays a border column of width [%-*@3 col] and color [%-*@3 c].&]
[s3; &]
[s4; &]
[s5;:Upp`:`:LineEdit`:`:GetBorderColumn`(`)const: [@(0.0.255) int]_[* GetBorderColumn]()_
[@(0.0.255) const]&]
[s2;%% Return border column position.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:SetFont`(Font`): [_^LineEdit^ LineEdit][@(0.0.255) `&]_[* SetFont]([_^Font^ F
ont]_[*@3 f])&]
[s2;%% Sets the font. HighlightLine can replace this font, however, 
the size of character cell is determined by this font (note that 
CJK ideograms are two character cells wide). Font should be monospaced. 
Returns `*this. Default font is Courier(16).&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:GetFont`(`)const: [_^Font^ Font]_[* GetFont]()_[@(0.0.255) const]&]
[s2;%% Returns the font.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:NoHorzScrollbar`(bool`): [_^LineEdit^ LineEdit][@(0.0.255) `&]_[* NoHorzSc
rollbar]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% In this mode horizontal scrollbar is never shown. Default 
is off.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:IsNoHorzScrollbar`(`)const: [@(0.0.255) bool]_[* IsNoHorzScrollbar]()_[@(0.0.255) c
onst]&]
[s2;%% Returns status of NoHrozScrollbar.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:ShowTabs`(bool`): [_^LineEdit^ LineEdit][@(0.0.255) `&]_[* ShowTabs]([@(0.0.255) b
ool]_[*@3 st]_`=_[@(0.0.255) true])&]
[s2;%% In this mode widget displays tabulators with faint graphics. 
Default is off.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:IsShowTabs`(`)const: [@(0.0.255) bool]_[* IsShowTabs]()_[@(0.0.255) const]&]
[s2;%% Returns status of ShowTabs..&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:ShowSpaces`(bool`): [_^LineEdit^ LineEdit][@(0.0.255) `&]_[* ShowSpaces]([@(0.0.255) b
ool]_[*@3 ss]_`=_[@(0.0.255) true])&]
[s2;%% In this mode widget displays spaces with faint dots. Default 
is off.&]
[s3;%% &]
[s4; &]
[s5;:LineEdit`:`:IsShowSpaces`(`)const: [@(0.0.255) bool]_[* IsShowSpaces]()_[@(0.0.255) co
nst]&]
[s2;%% Returns status of ShowSpaces.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:LineEdit`:`:ShowLineEndings`(bool`): [_^Upp`:`:LineEdit^ LineEdit][@(0.0.255) `&
]_[* ShowLineEndings]([@(0.0.255) bool]_[*@3 sl]_`=_[@(0.0.255) true])&]
[s2;%% In this mode widget displays line endings with faint dots. 
Default is off.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:LineEdit`:`:IsShowLineEndings`(`)const: [@(0.0.255) bool]_[* IsShowLineEnding
s]()_[@(0.0.255) const]&]
[s2;%% Returns status of ShowLineEndings.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:LineEdit`:`:WarnWhiteSpace`(bool`): [_^Upp`:`:LineEdit^ LineEdit][@(0.0.255) `&
]_[* WarnWhiteSpace]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% In this mode widget shows whitespaces that seem to be misplaced, 
like tabs after spaces or if line ends with spaces. Default is 
off.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:LineEdit`:`:IsWantWhiteSpace`(`)const: [@(0.0.255) bool]_[* IsWantWhiteSpace](
)_[@(0.0.255) const]&]
[s2;%% Returns status of WarnWhiteSpace.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:WithCutLine`(bool`): [_^LineEdit^ LineEdit][@(0.0.255) `&]_[* WithCutLine](
[@(0.0.255) bool]_[*@3 b])&]
[s2;%% In this mode widget calls CutLine when user presses Ctrl`+Y 
or Ctrl`+L. Default is on.&]
[s0; &]
[s4; &]
[s5;:LineEdit`:`:NoCutLine`(`): [_^LineEdit^ LineEdit][@(0.0.255) `&]_[* NoCutLine]()&]
[s2;%% Switches WithCutLine off.&]
[s3;%% &]
[s4; &]
[s5;:LineEdit`:`:IsWithCutLine`(`)const: [@(0.0.255) bool]_[* IsWithCutLine]()_[@(0.0.255) c
onst]&]
[s2;%% Returns status of WithCutLine.&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:SetFilter`(int`(`*`)`(int c`)`): [_^LineEdit^ LineEdit][@(0.0.255) `&]_[* S
etFilter]([@(0.0.255) int]_(`*[*@3 f])([@(0.0.255) int]_c))&]
[s2;%% Sets the filter of characters processed. Filter function can 
alter characters before they are inserted into the text. If filter 
function returns zero, character is rejected.&]
[s0; &]
[s0; &]
[ {{10000@3 [s0;%% [*@(229)4 LineEdit`::Highlight]]}}&]
[s3; &]
[s1;:LineEdit`:`:Highlight`:`:struct: [@(0.0.255)3 struct][3 _][*3 Highlight][3 _:_][@(0.0.255)3 p
ublic][3 _][*@3;3 Moveable][@(0.0.255)3 <][*3 Highlight][@(0.0.255)3 >][3 _]&]
[s9; This structure defines syntax highlighting properties of single 
character cell. &]
[s3;%% &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Member List]]}}&]
[s3; &]
[s5;:LineEdit`:`:Highlight`:`:paper: [_^Color^ Color]_[* paper]&]
[s3;%% &]
[s4; &]
[s5;:LineEdit`:`:Highlight`:`:ink: [_^Color^ Color]_[* ink]&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:Highlight`:`:font: [_^Font^ Font]_[* font]&]
[s3; &]
[s4; &]
[s5;:LineEdit`:`:Highlight`:`:chr: [_^wchar^ wchar]_[* chr]&]
[s3;%% &]
[s4; &]
[s5;:LineEdit`:`:Highlight`:`:operator`=`=`(const LineEdit`:`:Highlight`&`)const: [@(0.0.255) b
ool]_[* operator`=`=]([@(0.0.255) const]_[_^LineEdit`:`:Highlight^ Highlight][@(0.0.255) `&
]_[*@3 h])_[@(0.0.255) const]&]
[s2;%% Equality comparison.&]
[s3; &]
[s0; ]]