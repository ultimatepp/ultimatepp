topic "TextCtrl";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[0 $$7,0#96390100711032703541132217272105:end]
[{_}%EN-US 
[s3;%- TextCtrl&]
[s1;:TextCtrl`:`:class:%- [*@(0.0.255)3 class][*@0;3 _][@0;3 TextCtrl][*@0;3 _:_][*@(0.0.255)3 p
ublic][*@0;3 _][@3;3 Ctrl][*@0;3 , ][*@(0.0.255)3 protected][*@0;3 _][@3;3 TextArrayOps]&]
[s2; &]
[s1;K:`:`:TextCtrl`:`:class:^`:`:Ctrl^%- &]
[s2; A base class that implements operations common for [^`:`:DocEdit^ DocEdit] 
(raw text editor where lines `- LF delimited sections `- are 
displayed as paragraphs `- wrapped when they do not fit the width) 
and [^`:`:LineEdit^ LineEdit].(where lines are simply lines and 
are never wrapped). TextCtrl works in UNICODE, but is able performing 
conversions to any of predefined charsets. The Value of TextCtrl 
is contained text; setting it is the same as using Set method, 
reading is equivalent to Get method.&]
[s2; &]
[s5;K%- [@(0.0.255) struct]_TextCtrl`::[@0 UndoData]&]
[s2;l320; This structure contains informations about undo/redo state 
of the widget. It can be with PickUndoData and SetPickUndoDate 
to store and restore undo/redo state.&]
[s0;l128; &]
[s5;l320;K:`:`:TextCtrl`:`:UndoData`:`:Clear`(`):%- [@(0.0.255) void]_[@0 Clear]()&]
[s2;l512; Clears the undo/redo state.&]
[s0;l320; &]
[s5;K:`:`:TextCtrl`:`:SelectionChanged`(`):%- virtual [@(0.0.255) void]_[@0 SelectionChan
ged]()[@0  `[protected`]]&]
[s2; Selection anchor or cursor has changed. (called even when there 
is no selection, just cursor). If you override this, you should 
call base`-class version in the body.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:ClearLines`(`):%- virtual [@(0.0.255) void]_[@0 ClearLines]()[@0  
`[protected`]]&]
[s2; The content was removed (e.g. Clear or Load operations). If 
you override this, you should call base`-class version in the 
body.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:InsertLines`(int`,int`):%- virtual [@(0.0.255) void]_[@0 InsertLine
s]([@(0.0.255) int]_[@3 line], [@(0.0.255) int]_[@3 count])[@0  `[protected`]]&]
[s2; [%-*@3 count] lines at [%-*@3 line] position were inserted. If you 
override this, you should call base`-class version in the body.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:RemoveLines`(int`,int`):%- virtual [@(0.0.255) void]_[@0 RemoveLine
s]([@(0.0.255) int]_[@3 line], [@(0.0.255) int]_[@3 count])[@0  `[protected`]]&]
[s2; [%-*@3 count] lines at [%-*@3 line] position were removed. If you 
override this, you should call base`-class version in the body.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:PreInsert`(int`,const`:`:WString`&`):%- virtual 
[@(0.0.255) void]_[@0 PreInsert]([@(0.0.255) int]_[@3 pos], [@(0.0.255) const]_[^`:`:WString^ W
String]`&_[@3 text])[@0  `[protected`]]&]
[s2; Called before any [%-*@3 text] gets inserted at [%-*@3 pos.] If 
you override this, you should call base`-class version in the 
body.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:PostInsert`(int`,const`:`:WString`&`):%- virtual 
[@(0.0.255) void]_[@0 PostInsert]([@(0.0.255) int]_[@3 pos], [@(0.0.255) const]_[^`:`:WString^ W
String]`&_[@3 text])[@0  `[protected`]]&]
[s2; Called after [%-*@3 text] was inserted at [%-*@3 pos.] If you override 
this, you should call base`-class version in the body.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:PreRemove`(int`,int`):%- virtual [@(0.0.255) void]_[@0 PreRemove]([@(0.0.255) i
nt]_[@3 pos], [@(0.0.255) int]_[@3 size])[@0  `[protected`]]&]
[s2; Called before [%-*@3 size] characters get removed from the [%-*@3 pos]. 
If you override this, you should call base`-class version in 
the body.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:PostRemove`(int`,int`):%- virtual [@(0.0.255) void]_[@0 PostRemove](
[@(0.0.255) int]_[@3 pos], [@(0.0.255) int]_[@3 size])[@0  `[protected`]]&]
[s2; Called after [%-*@3 size] characters was removed from the [%-*@3 pos]. 
 If you override this, you should call base`-class version in 
the body.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:RefreshLine`(int`):%- virtual [@(0.0.255) void]_[@0 RefreshLine]([@(0.0.255) i
nt]_[@3 i])&]
[s2; This virtual method is called by TextCtrl when the content of 
line [%-*@3 i] is changed. If you override this, you should call 
base`-class version in the body.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:WhenBar:%- [^`:`:Callback1^ Callback1]<[^`:`:Bar^@0 Bar]`&>_[@0 WhenB
ar]&]
[s2; This callback is invoked when user right`-clicks the widget 
and should specify local menu. It is initialized by the constructor 
to the StdBar method.  &]
[s0; &]
[s5;K:`:`:TextCtrl`:`:WhenState:%- [^`:`:Callback^ Callback]_[@0 WhenState]&]
[s2; This callback is invoked when the status of widget changes from 
not`-modified to modified (`"dirty`") or back.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:CachePos`(int`):%- [@(0.0.255) void]_[@0 CachePos]([@(0.0.255) int]_[@3 p
os])&]
[s2; This is specific optimization hint to the widget saying that 
following operations will be performed near after [%-*@3 pos]. 
Unlikely to be used in the client code.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:Load`(`:`:Stream`&`,`:`:byte`):%- [@(0.0.255) void]_[@0 Load]([^`:`:Stream^ S
tream]`&_[@3 s], [^`:`:byte^ byte]_[@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2; Loads the text from the stream with defined [%-*@3 charset].&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:Save`(`:`:Stream`&`,`:`:byte`)const:%- [@(0.0.255) void]_[@0 Save](
[^`:`:Stream^ Stream]`&_[@3 s], [^`:`:byte^ byte]_[@3 charset]_`=_CHARSET`_DEFAULT)_[@(0.0.255) c
onst]&]
[s2; Saves the text to the stream with defined [%-*@3 charset]. Characters 
that cannot be represented in suggested [%-*@3 charset] are saved 
as `'?`'.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:GetInvalidCharPos`(`:`:byte`)const:%- [@(0.0.255) int]_[@0 GetInval
idCharPos]([^`:`:byte^ byte]_[@3 charset]_`=_CHARSET`_DEFAULT)_[@(0.0.255) const]&]
[s2; Returns the position of the first character that cannot be represented 
[%-*@3 charset]. If there is none, returns negative value.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:CheckCharset`(`:`:byte`)const:%- [@(0.0.255) bool]_[@0 CheckCharset
]([^`:`:byte^ byte]_[@3 charset]_`=_CHARSET`_DEFAULT)_[@(0.0.255) const]&]
[s2;%- [%% Same as GetInvalidCharPos(]charset) < 0.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:Set`(const`:`:WString`&`):%- [@(0.0.255) void]_[@0 Set]([@(0.0.255) c
onst]_[^`:`:WString^ WString]`&_[@3 s])&]
[s5;K:`:`:TextCtrl`:`:Set`(const`:`:String`&`,`:`:byte`):%- [@(0.0.255) void]_[@0 Set]([@(0.0.255) c
onst]_[^`:`:String^ String]`&_[@3 s], [^`:`:byte^ byte]_[@3 charset]_`=_CHARSET`_DEFAULT)
&]
[s2; Sets the text in the widget.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:Get`(`:`:byte`)const:%- [^`:`:String^ String]_[@0 Get]([^`:`:byte^ by
te]_[@3 charset]_`=_CHARSET`_DEFAULT)_[@(0.0.255) const]&]
[s2; Gets the text in the widget.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:Get`(int`,int`,`:`:byte`)const:%- [^`:`:String^ String]_[@0 Get]([@(0.0.255) i
nt]_[@3 pos], [@(0.0.255) int]_[@3 size], [^`:`:byte^ byte]_[@3 charset]_`=_CHARSET`_DEFAUL
T)_[@(0.0.255) const]&]
[s2; Gets the part of text in the widget.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:GetW`(int`,int`)const:%- [^`:`:WString^ WString]_[@0 GetW]([@(0.0.255) i
nt]_[@3 pos], [@(0.0.255) int]_[@3 size])_[@(0.0.255) const]&]
[s2; Gets the part of text in the widget in UNICODE.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:GetW`(`)const:%- [^`:`:WString^ WString]_[@0 GetW]()_[@(0.0.255) cons
t]&]
[s2; Gets the text in the widget in UNICODE.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:ClearDirty`(`):%- [@(0.0.255) void]_[@0 ClearDirty]()&]
[s2; Clears dirty flag. Note that the difference between `"dirty`" 
flag and `"modified`" flag of Ctrl interface is that `"dirty`" 
flag can get cleared by undo operation, when the text is restored 
to the original state (one after setting it or at ClearDirty).&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:IsDirty`(`)const:%- [@(0.0.255) bool]_[@0 IsDirty]()_[@(0.0.255) cons
t]&]
[s2; Tests if text is different from `"original`" state (one after 
setting it or at ClearDirty).&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:Clear`(`):%- [@(0.0.255) void]_[@0 Clear]()&]
[s2; Empties the text.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:GetLinePos`(int`&`)const:%- [@(0.0.255) int]_[@0 GetLinePos]([@(0.0.255) i
nt]`&_[@3 pos])_[@(0.0.255) const]&]
[s2; Returns the line where character at offset [%-*@3 pos] resides; 
[%-*@3 pos] is altered to contain the position withing the line.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:GetPos`(int`,int`)const:%- [@(0.0.255) int]_[@0 GetPos]([@(0.0.255) i
nt]_[@3 line], [@(0.0.255) int]_[@3 column])_[@(0.0.255) const]&]
[s2; Returns the offset of character at [%-*@3 line] and [%-*@3 column] 
index within the line.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:GetPos`(int`)const:%- [@(0.0.255) int]_[@0 GetPos]([@(0.0.255) int]_[@3 l
ine])_[@(0.0.255) const]&]
[s2; Same as GetPos([%-*@3 line], 0).&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:GetLine`(int`)const:%- [@(0.0.255) int]_[@0 GetLine]([@(0.0.255) int]_
[@3 pos])_[@(0.0.255) const]&]
[s2; Similar to GetLinePos, but does not alter [%-*@3 pos] parameter.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:GetUtf8Line`(int`)const:%- [@(0.0.255) const]_[^`:`:String^ String]`&
_[@0 GetUtf8Line]([@(0.0.255) int]_[@3 i])_[@(0.0.255) const]&]
[s2; Returns the line [%-*@3 i] in UTF`-8 encoding.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:GetWLine`(int`)const:%- [^`:`:WString^ WString]_[@0 GetWLine]([@(0.0.255) i
nt]_[@3 i])_[@(0.0.255) const]&]
[s2; Returns the line [%-*@3 i] in UNICODE.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:GetEncodedLine`(int`,`:`:byte`)const:%- [^`:`:String^ String]_[@0 G
etEncodedLine]([@(0.0.255) int]_[@3 i], [^`:`:byte^ byte]_[@3 charset]_`=_CHARSET`_DEFAUL
T)_[@(0.0.255) const]&]
[s2; Returns the line [%-*@3 i] required encoding.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:GetLineLength`(int`)const:%- [@(0.0.255) int]_[@0 GetLineLength]([@(0.0.255) i
nt]_[@3 i])_[@(0.0.255) const]&]
[s2; Returns the length of line [%-*@3 i].&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:GetLineCount`(`)const:%- [@(0.0.255) int]_[@0 GetLineCount]()_[@(0.0.255) c
onst]&]
[s2; Returns the number of lines.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:GetChar`(int`)const:%- [@(0.0.255) int]_[@0 GetChar]([@(0.0.255) int]_
[@3 pos])_[@(0.0.255) const]&]
[s5;K:`:`:TextCtrl`:`:operator`[`]`(int`)const:%- [@(0.0.255) int]_[@0 operator`[`]]([@(0.0.255) i
nt]_[@3 pos])_[@(0.0.255) const]&]
[s2; Returns the UNICODE character at [%-*@3 pos] offset.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:GetLength`(`)const:%- [@(0.0.255) int]_[@0 GetLength]()_[@(0.0.255) c
onst]&]
[s2; Returns the total number of characters in the text.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:GetCursor`(`)const:%- [@(0.0.255) int]_[@0 GetCursor]()_[@(0.0.255) c
onst]&]
[s2; Returns the position of cursor.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:GetCursorLine`(`):%- [@(0.0.255) int]_[@0 GetCursorLine]()&]
[s2; Same as GetLine(GetCursor).&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:SetSelection`(int`,int`):%- [@(0.0.255) void]_[@0 SetSelection]([@(0.0.255) i
nt]_[@3 anchor]_`=_[@3 0], [@(0.0.255) int]_[@3 cursor]_`=_INT`_MAX)&]
[s2; Sets the selection. If [%-*@3 anchor] or [%-*@3 cursor] are out 
of range, they are `"fixed`". If they are equal, method changes 
the position of cursor.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:IsSelection`(`)const:%- [@(0.0.255) bool]_[@0 IsSelection]()_[@(0.0.255) c
onst]&]
[s2; Tests whether there is non`-empty selection.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:GetSelection`(int`&`,int`&`)const:%- [@(0.0.255) bool]_[@0 GetSelec
tion]([@(0.0.255) int]`&_[@3 l], [@(0.0.255) int]`&_[@3 h])_[@(0.0.255) const]&]
[s2; Returns the selection lower and upper bounds.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:GetSelection`(`:`:byte`):%- [^`:`:String^ String]_[@0 GetSelection](
[^`:`:byte^ byte]_[@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2; Returns the selected text in given [%-*@3 charset].&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:GetSelectionW`(`)const:%- [^`:`:WString^ WString]_[@0 GetSelectionW
]()[@0  const]&]
[s2; Returns the selected text in UNICODE.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:ClearSelection`(`)const:%- [@(0.0.255) void]_[@0 ClearSelection]()[@0  
const]&]
[s2; Cancels the selection.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:RemoveSelection`(`):%- [@(0.0.255) bool]_[@0 RemoveSelection]()&]
[s2; Deletes the selection text.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:SetCursor`(int`):%- [@(0.0.255) void]_[@0 SetCursor]([@(0.0.255) int]_
[@3 cursor])&]
[s2; Places cursor at new position.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:Paste`(const`:`:WString`&`):%- [@(0.0.255) int]_[@0 Paste]([@(0.0.255) c
onst]_[^`:`:WString^ WString]`&_[@3 text])&]
[s2; Pastes [%-*@3 text] at current cursor position.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:Insert`(int`,const`:`:WString`&`):%- [@(0.0.255) int]_[@0 Insert]([@(0.0.255) i
nt]_[@3 pos], [@(0.0.255) const]_[^`:`:WString^ WString]`&_[@3 txt])&]
[s2; Inserts [%-*@3 txt] at [%-*@3 pos].&]
[s0;@(64) &]
[s5;K:`:`:TextCtrl`:`:Insert`(int`,const`:`:String`&`,`:`:byte`):%- [@(0.0.255) int]_[@0 I
nsert]([@(0.0.255) int]_[@3 pos], [@(0.0.255) const]_[^`:`:String^ String]`&_[@3 txt], 
[^`:`:byte^ byte]_[@3 charset]_`=_CHARSET`_DEFAULT)&]
[s2; Inserts [%-*@3 txt] encoded with [%-*@3 charset ]at [%-*@3 pos].&]
[s0;@(64) &]
[s5;K:`:`:TextCtrl`:`:Insert`(int`,const char`*`):%- [@(0.0.255) int]_[@0 Insert]([@(0.0.255) i
nt]_[@3 pos], [@(0.0.255) const]_[@(0.0.255) char]_`*[@3 txt])&]
[s2; Inserts [%-*@3 txt] at [%-*@3 pos] using default charset.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:Remove`(int`,int`):%- [@(0.0.255) void]_[@0 Remove]([@(0.0.255) int]_
[@3 pos], [@(0.0.255) int]_[@3 size])&]
[s2; Removes the text.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:NextUndo`(`):%- [@(0.0.255) void]_[@0 NextUndo]()&]
[s2; Marks the beginning of next undo block. All text altering operations 
(inserts and removes) are recorded into undo buffer in sections 
where each sections represents single user action. NextUndo tells 
TextCtrl that the next altering operation belongs to the new 
section. Note that NextUndo does not create a new section (yet); 
calling it twice without any altering operation in between is 
the same as calling it once.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:Undo`(`):%- [@(0.0.255) void]_[@0 Undo]()&]
[s2; Performs undo.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:Redo`(`):%- [@(0.0.255) void]_[@0 Redo]()&]
[s2; Performs redo.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:IsUndo`(`)const:%- [@(0.0.255) bool]_[@0 IsUndo]()_[@(0.0.255) const]&]
[s2; Tests whether there are some undo records.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:IsRedo`(`)const:%- [@(0.0.255) bool]_[@0 IsRedo]()_[@(0.0.255) const]&]
[s2; Tests whether there are some redo records.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:ClearUndo`(`):%- [@(0.0.255) void]_[@0 ClearUndo]()&]
[s2; Clears all undo and redo records.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:PickUndoData`(`):%- [^`:`:TextCtrl`:`:UndoData^ UndoData]_[@0 PickU
ndoData]()&]
[s2; Picks undo and redo record. This is useful when single widget 
is used to edit various text, this allows to change undo/redo 
records with the text. Must be followed by either SetPickUndoData 
or ClearUndo before performing any text altering operation.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:SetPickUndoData`(pick`_`:`:TextCtrl`:`:UndoData`&`):%- [@(0.0.255) v
oid]_[@0 SetPickUndoData](pick`__[^`:`:TextCtrl`:`:UndoData^ UndoData]`&_[@3 data])&]
[s2; Sets the undo/redo records. [%-@3 data] is picked (destroyed) 
during the operation.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:Cut`(`):%- [@(0.0.255) void]_[@0 Cut]()&]
[s2; Performs Cut.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:Copy`(`):%- [@(0.0.255) void]_[@0 Copy]()&]
[s2; Performs Copy.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:Paste`(`):%- [@(0.0.255) void]_[@0 Paste]()&]
[s2; Performs Paste.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:SelectAll`(`):%- [@(0.0.255) void]_[@0 SelectAll]()&]
[s2; Selects the whole text.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:StdBar`(`:`:Bar`&`):%- [@(0.0.255) void]_[@0 StdBar]([^`:`:Bar^ Bar]`&
_[@3 menu])&]
[s2; Provides the standard local [%-*@3 menu].&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:SetCharset`(`:`:byte`):%- [@(0.0.255) void]_[@0 SetCharset]([^`:`:byte^ b
yte]_[@3 cs])&]
[s2; Sets the input character encoding of widget. This character 
set is used to decide which characters can be entered by the 
user into the text.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:GetCharset`(`)const:%- [^`:`:byte^ byte]_[@0 GetCharset]()_[@(0.0.255) c
onst]&]
[s2; Returns the value set by SetCharset.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:SetColor`(int`,`:`:Color`):%- [@(0.0.255) void]_[@0 SetColor]([@(0.0.255) i
nt]_[@3 i], [^`:`:Color^ Color]_[@3 c])&]
[s2; Sets one of colors used to display the text.&]
[s5;K:`:`:TextCtrl`:`:INK`_NORMAL:@0 &]
[ {{4443:5557<288;>864; [s0;%- [+75 TextCtrl`::][*+75 INK`_NORMAL]]
:: [s0; [+75 Normal text color.]]
:: [s0;%- [+75 TextCtrl`::][*+75 INK`_DISABLED]]
:: [s0; [+75 Text color when widget is disabled. ]]
:: [s0;%- [+75 TextCtrl`::][*+75 INK`_SELECTED]]
:: [s0; [+75 Selected text color.]]
:: [s0;%- [+75 TextCtrl`::][*+75 PAPER`_NORMAL]]
:: [s0; [+75 Normal paper color.]]
:: [s0;%- [+75 TextCtrl`::][*+75 PAPER`_READONLY]]
:: [s0; [+75 Paper color when widget is read`-only.]]
:: [s0;%- [+75 TextCtrl`::][*+75 PAPER`_SELECTED]]
:: [s0; [+75 Selected paper color.]]
:: [s0;%- [+75 TextCtrl`::][*+75 COLOR`_COUNT]]
:: [s0; [+75 Total number of color indexes.]]}}&]
[s0; &]
[s0; &]
[s5;K:`:`:TextCtrl`:`:GetColor`(int`)const:%- [^`:`:Color^ Color]_[@0 GetColor]([@(0.0.255) i
nt]_[@3 i])_[@(0.0.255) const]&]
[s2; Gets the color used to display the text.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:UndoSteps`(int`):%- [^`:`:TextCtrl^ TextCtrl]`&_[@0 UndoSteps]([@(0.0.255) i
nt]_[@3 n])&]
[s2; Sets the maximum number of undo steps.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:ProcessTab`(bool`):%- [^`:`:TextCtrl^ TextCtrl]`&_[@0 ProcessTab]([@(0.0.255) b
ool]_[@3 b]_`=_true)&]
[s2; If active (default), widget accepts K`_TAB keystrokes (inserts 
`'`\t`').&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:NoProcessTab`(`):%- [^`:`:TextCtrl^ TextCtrl]`&_[@0 NoProcessTab]()
&]
[s2; Same as ProcessTab(false).&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:NoBackground`(bool`):%- [^`:`:TextCtrl^ TextCtrl]`&_[@0 NoBackgroun
d]([@(0.0.255) bool]_[@3 b]_`=_true)&]
[s2; Sets the widget into transparent mode `- background is not painted 
and Transparent is activated `- a result, anything painted behind 
the widget is visible, allowing client code to provide any background 
it needs.&]
[s0; &]
[s5;K:`:`:TextCtrl`:`:IsProcessTab`(`):%- [@(0.0.255) bool]_[@0 IsProcessTab]()&]
[s2; Tests whether ProcessTab mode is active.&]
[s0; &]
[s0; ]