topic "TextCtrl";
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
[ {{10000@3 [s0; [*@(229)4 TextCtrl]]}}&]
[s3; &]
[s1;:TextCtrl`:`:class: [@(0.0.255)3 class][3 _][*3 TextCtrl][3 _:_][@(0.0.255)3 public][3 _][*@3;3 C
trl][3 , ][@(0.0.255)3 protected][3 _][*@3;3 TextArrayOps]&]
[s9;%% A base class that implements operations common for [^`:`:DocEdit^ DocEdit] 
(raw text editor where lines `- LF delimited sections `- are 
displayed as paragraphs `- wrapped when they do not fit the width) 
and [^`:`:LineEdit^ LineEdit].(where lines are simply lines and 
are never wrapped). TextCtrl works in UNICODE, but is able performing 
conversions to any of predefined charsets. The Value of TextCtrl 
is contained text; setting it is the same as using Set method, 
reading is equivalent to Get method.&]
[s3;%% &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Protected Method List]]}}&]
[s3; &]
[s5;:TextCtrl`:`:SelectionChanged`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* SelectionC
hanged]()&]
[s2;%% Selection anchor or cursor has changed. (called even when 
there is no selection, just cursor). If you override this, you 
should call base`-class version in the body.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:ClearLines`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* ClearLines]()&]
[s2;%% The content was removed (e.g. Clear or Load operations). If 
you override this, you should call base`-class version in the 
body.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:InsertLines`(int`,int`): [@(0.0.255) virtual] [@(0.0.255) void]_[* InsertL
ines]([@(0.0.255) int]_[*@3 line], [@(0.0.255) int]_[*@3 count])&]
[s2;%% [%-*@3 count] lines at [%-*@3 line] position were inserted. If 
you override this, you should call base`-class version in the 
body.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:RemoveLines`(int`,int`): [@(0.0.255) virtual] [@(0.0.255) void]_[* RemoveL
ines]([@(0.0.255) int]_[*@3 line], [@(0.0.255) int]_[*@3 count])&]
[s2;%% [%-*@3 count] lines at [%-*@3 line] position were removed. If 
you override this, you should call base`-class version in the 
body.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:PreInsert`(int`,const WString`&`): [@(0.0.255) virtual] 
[@(0.0.255) void]_[* PreInsert]([@(0.0.255) int]_[*@3 pos], [@(0.0.255) const]_[_^WString^ WS
tring][@(0.0.255) `&]_[*@3 text])&]
[s2;%% Called before any [%-*@3 text] gets inserted at [%-*@3 pos.] If 
you override this, you should call base`-class version in the 
body.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:PostInsert`(int`,const WString`&`): [@(0.0.255) virtual] 
[@(0.0.255) void]_[* PostInsert]([@(0.0.255) int]_[*@3 pos], [@(0.0.255) const]_[_^WString^ W
String][@(0.0.255) `&]_[*@3 text])&]
[s2;%% Called after [%-*@3 text] was inserted at [%-*@3 pos.] If you 
override this, you should call base`-class version in the body.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:PreRemove`(int`,int`): [@(0.0.255) virtual] [@(0.0.255) void]_[* PreRemove
]([@(0.0.255) int]_[*@3 pos], [@(0.0.255) int]_[*@3 size])&]
[s2;%% Called before [%-*@3 size] characters get removed from the [%-*@3 pos]. 
If you override this, you should call base`-class version in 
the body.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:PostRemove`(int`,int`): [@(0.0.255) virtual] [@(0.0.255) void]_[* PostRemo
ve]([@(0.0.255) int]_[*@3 pos], [@(0.0.255) int]_[*@3 size])&]
[s2;%% Called after [%-*@3 size] characters was removed from the [%-*@3 pos]. 
 If you override this, you should call base`-class version in 
the body.&]
[s3;%% &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:TextCtrl`:`:RefreshLine`(int`): [@(0.0.255) virtual] [@(0.0.255) void]_[* RefreshLine](
[@(0.0.255) int]_[*@3 i])&]
[s2;%% This virtual method is called by TextCtrl when the content 
of line [%-*@3 i] is changed. If you override this, you should 
call base`-class version in the body.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:WhenBar: [_^Callback1^ Callback1][@(0.0.255) <]Bar[@(0.0.255) `&>]_[* WhenBa
r]&]
[s2;%% This callback is invoked when user right`-clicks the widget 
and should specify local menu. It is initialized by the constructor 
to the StdBar method.  &]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:WhenState: [_^Callback^ Callback]_[* WhenState]&]
[s2;%% This callback is invoked when the status of widget changes 
from not`-modified to modified (`"dirty`") or back.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:WhenSel: [_^Callback^ Callback]_[* WhenSel]&]
[s2;%% Called when cursor or selection changes.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:WhenViewLoading: [_^Upp`:`:Event^ Event]<[_^Upp`:`:int64^ int64]>
_[* WhenViewMapping]&]
[s2;%% Called during mapping view stream with current position in 
the stream.&]
[s3; &]
[s4; &]
[s5;:TextCtrl`:`:CachePos`(int`): [@(0.0.255) void]_[* CachePos]([@(0.0.255) int]_[*@3 pos])&]
[s2;%% This is specific optimization hint to the widget saying that 
following operations will be performed near [%-*@3 pos]. Unlikely 
to be used in the client code.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:CacheLinePos`(int`): [@(0.0.255) void]_[* CacheLinePos]([@(0.0.255) int]_[*@3 l
inei])&]
[s2;%% This is specific optimization hint to the widget saying that 
following operations will be performed near line [%-*@3 linei]. 
Unlikely to be used in the client code.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:Load`(Stream`&`,byte`): [@(0.0.255) int]_[* Load]([_^Stream^ Stream][@(0.0.255) `&
]_[*@3 s], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2;%% Loads the text from the stream with defined [%-*@3 charset]. 
Function returns the detected line endings mode `- LE`_CRLF, 
LE`_LF or LE`_DEFAULT if there were no line endings in the file. 
If file is bigger then the limit set by MaxLength, editor is 
put into `'truncated`' and read`-only mode.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:IsTruncated`(`)const: [@(0.0.255) bool]_[* IsTruncated]()_[@(0.0.255) cons
t]&]
[s2;%% Indicates that last Load had to truncate the size because 
of MaxLength limit.&]
[s3; &]
[s4; &]
[s5;:TextCtrl`:`:Save`(Stream`&`,byte`,int`)const: [@(0.0.255) void]_[* Save]([_^Stream^ St
ream][@(0.0.255) `&]_[*@3 s], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT, 
[@(0.0.255) int]_[*@3 line`_endings]_`=_LE`_DEFAULT)_[@(0.0.255) const]&]
[s2;%% Saves the text to the stream with defined [%-*@3 charset]. Characters 
that cannot be represented in suggested [%-*@3 charset] are saved 
as `'?`'. [%-*@3 line`_endings] parameter sets the line ending 
mode. LE`_DEFAULT uses platform specific line endings (CRLF in 
Windows, LF in POSIX), LE`_CRLF sets CRLF line endings, LE`_LF 
sets LF line endings). If IsTruncated is true, Save is blocked.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetInvalidCharPos`(byte`)const: [@(0.0.255) int]_[* GetInvalidCharPos]([_^byte^ b
yte]_[*@3 charset]_`=_CHARSET`_DEFAULT)_[@(0.0.255) const]&]
[s2;%% Returns the position of the first character that cannot be 
represented [%-*@3 charset]. If there is none, returns negative 
value.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:CheckCharset`(byte`)const: [@(0.0.255) bool]_[* CheckCharset]([_^byte^ byt
e]_[*@3 charset]_`=_CHARSET`_DEFAULT)_[@(0.0.255) const]&]
[s2; [%% Same as GetInvalidCharPos(]charset) < 0.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:Set`(const WString`&`): [@(0.0.255) void]_[* Set]([@(0.0.255) const]_[_^WString^ W
String][@(0.0.255) `&]_[*@3 s])&]
[s5;:TextCtrl`:`:Set`(const String`&`,byte`): [@(0.0.255) void]_[* Set]([@(0.0.255) const]_
[_^String^ String][@(0.0.255) `&]_[*@3 s], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT
)&]
[s2;%% Sets the text in the widget.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:Get`(byte`)const: [_^String^ String]_[* Get]([_^byte^ byte]_[*@3 charset]_`=
_CHARSET`_DEFAULT)_[@(0.0.255) const]&]
[s2;%% Gets the text in the widget.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:Get`(Upp`:`:int64`,int`,Upp`:`:byte`)const: [_^Upp`:`:String^ S
tring]_[* Get]([_^Upp`:`:int64^ int64]_[*@3 pos], [@(0.0.255) int]_[*@3 size], 
[_^Upp`:`:byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)_[@(0.0.255) const]&]
[s2;%% Gets the part of text in the widget.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:GetW`(Upp`:`:int64`,int`)const: [_^Upp`:`:WString^ WString]_[* Ge
tW]([_^Upp`:`:int64^ int64]_[*@3 pos], [@(0.0.255) int]_[*@3 size])_[@(0.0.255) const]&]
[s2;%% Gets the part of text in the widget in UNICODE.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetW`(`)const: [_^WString^ WString]_[* GetW]()_[@(0.0.255) const]&]
[s2;%% Gets the text in the widget in UNICODE.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:ClearDirty`(`): [@(0.0.255) void]_[* ClearDirty]()&]
[s2;%% Clears dirty flag. Note that the difference between `"dirty`" 
flag and `"modified`" flag of Ctrl interface is that `"dirty`" 
flag can get cleared by undo operation, when the text is restored 
to the original state (one after setting it or at ClearDirty).&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:IsDirty`(`)const: [@(0.0.255) bool]_[* IsDirty]()_[@(0.0.255) const]&]
[s2;%% Tests if text is different from `"original`" state (one after 
setting it or at ClearDirty).&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%% Empties the text.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:GetLine`(Upp`:`:int64`)const: [@(0.0.255) int]_[* GetLine]([_^Upp`:`:int64^ i
nt64]_[*@3 pos])_[@(0.0.255) const]&]
[s2;%% Similar to GetLinePos, but does not alter [%-*@3 pos] parameter.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetLinePos`(int`&`)const: [@(0.0.255) int]_[* GetLinePos]([@(0.0.255) int`&
]_[*@3 pos])_[@(0.0.255) const]&]
[s2;%% Returns the line where character at offset [%-*@3 pos] resides; 
[%-*@3 pos] is altered to contain the position withing the line.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetPos`(int`,int`)const: [@(0.0.255) int]_[* GetPos]([@(0.0.255) int]_[*@3 l
ine], [@(0.0.255) int]_[*@3 column])_[@(0.0.255) const]&]
[s2;%% Returns the offset of character at [%-*@3 line] and [%-*@3 column] 
index within the line.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetPos`(int`)const: [@(0.0.255) int]_[* GetPos]([@(0.0.255) int]_[*@3 line])
_[@(0.0.255) const]&]
[s2;%% Same as GetPos([%-*@3 line], 0).&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetUtf8Line`(int`)const: [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[* GetUtf8Line]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2;%% Returns the line [%-*@3 i] in UTF`-8 encoding.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetWLine`(int`)const: [_^WString^ WString]_[* GetWLine]([@(0.0.255) int]_[*@3 i
])_[@(0.0.255) const]&]
[s2;%% Returns the line [%-*@3 i] in UNICODE.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetEncodedLine`(int`,byte`)const: [_^String^ String]_[* GetEncodedLine](
[@(0.0.255) int]_[*@3 i], [_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)_[@(0.0.255) con
st]&]
[s2;%% Returns the line [%-*@3 i] required encoding.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetLineLength`(int`)const: [@(0.0.255) int]_[* GetLineLength]([@(0.0.255) i
nt]_[*@3 i])_[@(0.0.255) const]&]
[s2;%% Returns the length of line [%-*@3 i].&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetLineCount`(`)const: [@(0.0.255) int]_[* GetLineCount]()_[@(0.0.255) con
st]&]
[s2;%% Returns the number of lines.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:GetChar`(Upp`:`:int64`)const: [@(0.0.255) int]_[* GetChar]([_^Upp`:`:int64^ i
nt64]_[*@3 pos])_[@(0.0.255) const]&]
[s5;:Upp`:`:TextCtrl`:`:operator`[`]`(Upp`:`:int64`)const: [@(0.0.255) int]_[* operator`[
`]]([_^Upp`:`:int64^ int64]_[*@3 pos])_[@(0.0.255) const]&]
[s2;%% Returns the UNICODE character at [%-*@3 pos] offset.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:GetChar`(`)const: [@(0.0.255) int]_[* GetChar]()_[@(0.0.255) const]&]
[s2;%% Returns UNICODE character at cursor, or 0 if cursor is behind 
the last character.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetLength`(`)const: [@(0.0.255) int]_[* GetLength]()_[@(0.0.255) const]&]
[s2;%% Returns the total number of characters in the text.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetCursor`(`)const: [@(0.0.255) int]_[* GetCursor]()_[@(0.0.255) const]&]
[s2;%% Returns the position of cursor.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:GetCursorLine`(`)const: [@(0.0.255) int]_[* GetCursorLine]()_[@(0.0.255) c
onst]&]
[s2;%% Same as GetLine(GetCursor).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:SetSelection`(Upp`:`:int64`,Upp`:`:int64`): [@(0.0.255) void]_[* S
etSelection]([_^Upp`:`:int64^ int64]_[*@3 anchor]_`=_[@3 0], [_^Upp`:`:int64^ int64]_[*@3 c
ursor]_`=_[@3 2147483647]_)&]
[s2;%% Sets the selection. If [%-*@3 anchor] or [%-*@3 cursor] are out 
of range, they are `"fixed`". If they are equal, method changes 
the position of cursor.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:IsSelection`(`)const: [@(0.0.255) bool]_[* IsSelection]()_[@(0.0.255) cons
t]&]
[s2;%% Tests whether there is non`-empty normal selection.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:IsRectSelection`(`)const: [@(0.0.255) bool]_[* IsRectSelection]()_[@(0.0.255) c
onst]&]
[s2;%% Returns true if there is rectangular selection.&]
[s3; &]
[s4; &]
[s5;:TextCtrl`:`:IsAnySelection`(`)const: [@(0.0.255) bool]_[* IsAnySelection]()_[@(0.0.255) c
onst]&]
[s2;%% Returns true if there is either rectangular or normal selection.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetSelection`(int`&`,int`&`)const: [@(0.0.255) bool]_[* GetSelection]([@(0.0.255) i
nt`&]_[*@3 l], [@(0.0.255) int`&]_[*@3 h])_[@(0.0.255) const]&]
[s5;:Upp`:`:TextCtrl`:`:GetSelection`(Upp`:`:int64`&`,Upp`:`:int64`&`)const: [@(0.0.255) b
ool]_[* GetSelection]([_^Upp`:`:int64^ int64][@(0.0.255) `&]_[*@3 l], 
[_^Upp`:`:int64^ int64][@(0.0.255) `&]_[*@3 h])_[@(0.0.255) const]&]
[s2;%% Returns the selection lower and upper bounds. int64 version 
is only useful in view mode. In view mode, 32 bit version returns 
false if the range is larger than 2GB.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetSelection`(byte`)const: [_^String^ String]_[* GetSelection]([_^byte^ by
te]_[*@3 charset]_`=_CHARSET`_DEFAULT)_[@(0.0.255) const]&]
[s2;%% Returns the selected text in given [%-*@3 charset].&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetSelectionW`(`)const: [_^WString^ WString]_[* GetSelectionW]()_[@(0.0.255) c
onst]&]
[s2;%% Returns the selected text in UNICODE.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:ClearSelection`(`): [@(0.0.255) void]_[* ClearSelection]()&]
[s2;%% Cancels the selection.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:RemoveSelection`(`): [@(0.0.255) bool]_[* RemoveSelection]()&]
[s2;%% Deletes the selection text.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:SetCursor`(Upp`:`:int64`): [@(0.0.255) void]_[* SetCursor]([_^Upp`:`:int64^ i
nt64]_[*@3 cursor])&]
[s2;%% Places cursor at new position.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:Paste`(const WString`&`): [@(0.0.255) int]_[* Paste]([@(0.0.255) const]_[_^WString^ W
String][@(0.0.255) `&]_[*@3 text])&]
[s2;%% Pastes [%-*@3 text] at current cursor position.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:Insert`(int`,const WString`&`): [@(0.0.255) int]_[* Insert]([@(0.0.255) in
t]_[*@3 pos], [@(0.0.255) const]_[_^WString^ WString][@(0.0.255) `&]_[*@3 txt])&]
[s2;%% Inserts [%-*@3 txt] at [%-*@3 pos]. Returns a number of characters 
inserted.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:Insert`(int`,const String`&`,byte`): [@(0.0.255) int]_[* Insert]([@(0.0.255) i
nt]_[*@3 pos], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 txt], 
[_^byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2;%% Inserts [%-*@3 txt] encoded with [%-*@3 charset ]at [%-*@3 pos]. 
Returns a number of characters inserted.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:Insert`(int`,const char`*`): [@(0.0.255) int]_[* Insert]([@(0.0.255) int]_
[*@3 pos], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 txt])&]
[s2;%% Inserts [%-*@3 txt] at [%-*@3 pos] using default charset. Returns 
a number of characters inserted.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:Remove`(int`,int`): [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_[*@3 pos],
 [@(0.0.255) int]_[*@3 size])&]
[s2;%% Removes the text.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:Append`(const Upp`:`:WString`&`): [@(0.0.255) int]_[* Append]([@(0.0.255) c
onst]_[_^Upp`:`:WString^ WString][@(0.0.255) `&]_[*@3 txt])&]
[s2;%% Same as Insert(GetLength(), [%-*@3 txt]).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:Append`(const Upp`:`:String`&`,Upp`:`:byte`): [@(0.0.255) int]_
[* Append]([@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 txt], 
[_^Upp`:`:byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2;%% Same as Insert(GetLength(), [%-*@3 txt], [%-*@3 charset]).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:Append`(const char`*`): [@(0.0.255) int]_[* Append]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 txt])&]
[s2;%% Same as Append(WString([%-*@3 txt])).&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:NextUndo`(`): [@(0.0.255) void]_[* NextUndo]()&]
[s2;%% Marks the beginning of next undo block. All text altering 
operations (inserts and removes) are recorded into undo buffer 
in sections where each sections represents single user action. 
NextUndo tells TextCtrl that the next altering operation belongs 
to the new section. Note that NextUndo does not create a new 
section (yet); calling it twice without any altering operation 
in between is the same as calling it once.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:Undo`(`): [@(0.0.255) void]_[* Undo]()&]
[s2;%% Performs undo.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:Redo`(`): [@(0.0.255) void]_[* Redo]()&]
[s2;%% Performs redo.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:IsUndo`(`)const: [@(0.0.255) bool]_[* IsUndo]()_[@(0.0.255) const]&]
[s2;%% Tests whether there are some undo records.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:IsRedo`(`)const: [@(0.0.255) bool]_[* IsRedo]()_[@(0.0.255) const]&]
[s2;%% Tests whether there are some redo records.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:ClearUndo`(`): [@(0.0.255) void]_[* ClearUndo]()&]
[s2;%% Clears all undo and redo records.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:ClearRedo`(`): [@(0.0.255) void]_[* ClearRedo]()&]
[s2;%% Clears redo records.&]
[s3; &]
[s4; &]
[s5;:TextCtrl`:`:PickUndoData`(`): [_^TextCtrl`:`:UndoData^ UndoData]_[* PickUndoData]()&]
[s2;%% Picks undo and redo record. This is useful when single widget 
is used to edit various text, this allows to change undo/redo 
records with the text. Must be followed by either SetPickUndoData 
or ClearUndo before performing any text altering operation.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:SetPickUndoData`(Upp`:`:TextCtrl`:`:UndoData`&`&`): [@(0.0.255) v
oid]_[* SetPickUndoData]([_^Upp`:`:TextCtrl`:`:UndoData^ TextCtrl`::UndoData][@(0.0.255) `&
`&]_[*@3 data])&]
[s2;%% Sets the undo/redo records. [%-@3 data] is picked (destroyed) 
during the operation.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:Cut`(`): [@(0.0.255) void]_[* Cut]()&]
[s2;%% Performs Cut.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:Copy`(`): [@(0.0.255) void]_[* Copy]()&]
[s2;%% Performs Copy.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:Paste`(`): [@(0.0.255) void]_[* Paste]()&]
[s2;%% Performs Paste.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:SelectAll`(`): [@(0.0.255) void]_[* SelectAll]()&]
[s2;%% Selects the whole text.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:StdBar`(Bar`&`): [@(0.0.255) void]_[* StdBar]([_^Bar^ Bar][@(0.0.255) `&]_[*@3 m
enu])&]
[s2;%% Provides the standard local [%-*@3 menu].&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:SetCharset`(byte`): [@(0.0.255) void]_[* SetCharset]([_^byte^ byte]_[*@3 cs])
&]
[s2;%% Sets the input character encoding of widget. This character 
set is used to decide which characters can be entered by the 
user into the text.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetCharset`(`)const: [_^byte^ byte]_[* GetCharset]()_[@(0.0.255) const]&]
[s2;%% Returns the value set by SetCharset.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:SetColor`(int`,Color`): [@(0.0.255) void]_[* SetColor]([@(0.0.255) int]_[*@3 i
], [_^Color^ Color]_[*@3 c])&]
[s2;%% Sets one of colors used to display the text.&]
[s3;%% &]
[ {{4443:5557<288;>864; [s0; [+75 TextCtrl`::][*+75 INK`_NORMAL]]
:: [s0;%% [+75 Normal text color.]]
:: [s0; [+75 TextCtrl`::][*+75 INK`_DISABLED]]
:: [s0;%% [+75 Text color when widget is disabled. ]]
:: [s0; [+75 TextCtrl`::][*+75 INK`_SELECTED]]
:: [s0;%% [+75 Selected text color.]]
:: [s0; [+75 TextCtrl`::][*+75 PAPER`_NORMAL]]
:: [s0;%% [+75 Normal paper color.]]
:: [s0; [+75 TextCtrl`::][*+75 PAPER`_READONLY]]
:: [s0;%% [+75 Paper color when widget is read`-only.]]
:: [s0; [+75 TextCtrl`::][*+75 PAPER`_SELECTED]]
:: [s0;%% [+75 Selected paper color.]]
:: [s0; [+75 TextCtrl`::][*+75 COLOR`_COUNT]]
:: [s0;%% [+75 Total number of color indexes.]]}}&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:GetColor`(int`)const: [_^Color^ Color]_[* GetColor]([@(0.0.255) int]_[*@3 i])
_[@(0.0.255) const]&]
[s2;%% Gets the color used to display the text.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:View`(Upp`:`:Stream`&`,Upp`:`:byte`): [@(0.0.255) int]_[* View]([_^Upp`:`:Stream^ S
tream][@(0.0.255) `&]_[*@3 s], [_^Upp`:`:byte^ byte]_[*@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2;%% Initiates the view mode. In view mode, file is not loaded, 
but displayed while loading it from the stream as necessary. 
TextCtrl retains a reference to [%-*@3 s] so it must exist for 
the whole time it is displayed in TextCtrl. TextCtrl is in read`-only 
mode for view operations. View allows viewing of files >2GB, 
therefore some functions have 64`-bit counterparts working in 
view mode only. View actually needs to scan the file to create 
a map of file; this operation is performed in background (via 
PostCallbacks), application can use SerializeViewMap to store 
cache this map for particular file.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:WaitView`(int`,bool`): [@(0.0.255) void]_[* WaitView]([@(0.0.255) i
nt]_[*@3 line], [@(0.0.255) bool]_[*@3 progress])&]
[s2;%% While mapping of file for view is in progress, makes sure 
that the file is mapped up to [%-*@3 line] or EOF. If [%-*@3 progress] 
is true, progress is displayed.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:LockViewMapping`(`): [@(0.0.255) void]_[* LockViewMapping]()&]
[s2;%% Stops background mapping of file for view.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:UnlockViewMapping`(`): [@(0.0.255) void]_[* UnlockViewMapping]()&]
[s2; Continues mapping of file for view.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:SerializeViewMap`(Upp`:`:Stream`&`): [@(0.0.255) void]_[* Seriali
zeViewMap]([_^Upp`:`:Stream^ Stream][@(0.0.255) `&]_[*@3 s])&]
[s2;%% Serializes the file view map.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:IsView`(`)const: [@(0.0.255) bool]_[* IsView]()_[@(0.0.255) const]&]
[s2;%% Returns true if TextCtrl is in view mode.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:GetViewSize`(`)const: [_^Upp`:`:int64^ int64]_[* GetViewSize]()_[@(0.0.255) c
onst]&]
[s2;%% Returns GetSize of view stream.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:TextCtrl`:`:GetLinePos64`(Upp`:`:int64`&`)const: [@(0.0.255) int]_[* GetLineP
os64]([_^Upp`:`:int64^ int64][@(0.0.255) `&]_[*@3 pos])_[@(0.0.255) const]&]
[s5;:Upp`:`:TextCtrl`:`:GetPos64`(int`,int`)const: [_^Upp`:`:int64^ int64]_[* GetPos64]([@(0.0.255) i
nt]_[*@3 ln], [@(0.0.255) int]_[*@3 lpos])_[@(0.0.255) const]&]
[s5;:Upp`:`:TextCtrl`:`:GetLength64`(`)const: [_^Upp`:`:int64^ int64]_[* GetLength64]()_[@(0.0.255) c
onst]&]
[s5;:Upp`:`:TextCtrl`:`:GetCursor64`(`)const: [_^Upp`:`:int64^ int64]_[* GetCursor64]()_[@(0.0.255) c
onst]&]
[s2;%% These are variants of GetLinePos, GetPos, GetLength and GetCursor64 
for view mode (where values > INT`_MAX are possible).&]
[s3; &]
[s4; &]
[s5;:TextCtrl`:`:UndoSteps`(int`): [_^TextCtrl^ TextCtrl][@(0.0.255) `&]_[* UndoSteps]([@(0.0.255) i
nt]_[*@3 n])&]
[s2;%% Sets the maximum number of undo steps.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:ProcessTab`(bool`): [_^TextCtrl^ TextCtrl][@(0.0.255) `&]_[* ProcessTab]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% If active (default), widget accepts K`_TAB keystrokes (inserts 
`'`\t`').&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:NoProcessTab`(`): [_^TextCtrl^ TextCtrl][@(0.0.255) `&]_[* NoProcessTab]()
&]
[s2;%% Same as ProcessTab(false).&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:ProcessEnter`(bool`): [_^TextCtrl^ TextCtrl][@(0.0.255) `&]_[* ProcessEnte
r]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% If active (non`-default), widget accepts K`_ENTER keystrokes 
(inserts a new line).&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:NoProcessEnter`(`): [_^TextCtrl^ TextCtrl][@(0.0.255) `&]_[* NoProcessEnte
r]()&]
[s2;%% Same as ProcessEnter(false).&]
[s3; &]
[s4; &]
[s5;:TextCtrl`:`:NoBackground`(bool`): [_^TextCtrl^ TextCtrl][@(0.0.255) `&]_[* NoBackgroun
d]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% Sets the widget into transparent mode `- background is not 
painted and Transparent is activated `- a result, anything painted 
behind the widget is visible, allowing client code to provide 
any background it needs.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:MaxLength`(int`): [_^TextCtrl^ TextCtrl][@(0.0.255) `&]_[* MaxLength]([@(0.0.255) i
nt]_[*@3 len])&]
[s2;%% Sets the maximum size of text in unicode characters. Has to 
be less than 1Gchars. The default is 400Mchars.&]
[s3;%% &]
[s4; &]
[s5;:TextCtrl`:`:IsProcessTab`(`): [@(0.0.255) bool]_[* IsProcessTab]()&]
[s2;%% Tests whether ProcessTab mode is active.&]
[s3;%% &]
[s0;%% &]
[s0;%% &]
[s0;%% &]
[ {{10000@3 [s0; [*@(229)4 TextCtrl`::UndoData]]}}&]
[s3; &]
[s1;:TextCtrl`:`:UndoData`:`:struct: [@(0.0.255)3 struct][3 _][*3 UndoData]&]
[s9;%% This structure contains informations about undo/redo state 
of the widget. It can be with PickUndoData and SetPickUndoDate 
to store and restore undo/redo state.&]
[s3;%% &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:TextCtrl`:`:UndoData`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%% Clears the undo/redo state&]
[s3;%% &]
[s3;%% .]]