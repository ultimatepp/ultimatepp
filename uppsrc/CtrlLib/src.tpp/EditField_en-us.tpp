topic "EditField";
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
[ {{10000@3 [s0;%% [*@(229)4 EditField]]}}&]
[s3; &]
[s1;:EditField`:`:class: [@(0.0.255)3 class][3 _][*3 EditField][3 _:_][@(0.0.255)3 public][3 _][*@3;3 C
trl][3 , ][@(0.0.255)3 private][3 _][*@3;3 TextArrayOps]&]
[s0;%% &]
[s2;%% 
@@image:406&143
(A0EAFwAAAAAAAAAAAHic7ZfBCYAwEASvcyuxEDuxBSE96MtH7uJhgoZd2GEfIeSxwwUx5diLgpFl3egSFU4qpOAwsw+7vUQKDkwF18pumjuACrGtW0eXKa0reqeQ7HMpNG/X0+G/GVBI5kKhkJSkUMgngqngPqFWE88AKvQihTH0p4pAU4Eu89+2SpILMkWx/w==)
&]
[s0; &]
[s9;%% This widget is used whenever there is a type of value that 
can be edited in its textual representation. Of course, the most 
evident type of value is (W)String, but EditField can be used 
to edit any other type like Date, int, double. Adaptation of 
EditField is achieved by using [^`:`:Convert^ Convert] based object 
to transform Value to text and back.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Member List]]}}&]
[s3; &]
[s5;:EditField`:`:GetViewHeight`(Font`): [@(0.0.255) static] [@(0.0.255) int]_[* GetViewHei
ght]([_^Font^ Font]_[*@3 font]_`=_StdFont())&]
[s2; Returns the correct height for EditField view with [*@3 font][%%  
used for edited text].&]
[s3; &]
[s4; &]
[s5;:EditField`:`:GetStdHeight`(Font`): [@(0.0.255) static] [@(0.0.255) int]_[* GetStdHeigh
t]([_^Font^ Font]_[*@3 font]_`=_StdFont())&]
[s2;%% [%- Returns the correct height for EditField with ][%-*@3 font] 
used for edited text, including the standard static frame.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:HighlightText`(Vector`<EditField`:`:Highlight`>`&`): [@(0.0.255) virt
ual] [@(0.0.255) void]_[* HighlightText]([_^Vector^ Vector]<[_^EditField`:`:Highlight^ Hi
ghlight]>`&_[*@3 hl])&]
[s2;%% Provides a chance to change the text color and background 
for individual characters.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:WhenBar: [_^Callback1^ Callback1]<Bar[@(0.0.255) `&]>_[* WhenBar]&]
[s2;%% This callback represents the context menu of EditField. The 
default is StdBar.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:WhenEnter: [_^Callback^ Callback]_[* WhenEnter]&]
[s2;%% This callback is invoked if user presses Enter key while in 
EditField. If not empty, EditField also consumes Enter key (so 
that it is not passed up in Ctrl hierarchy). Default is empty.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:WhenPasteFilter: [_^Callback1^ Callback1]<WString[@(0.0.255) `&]>_[* When
PasteFilter]&]
[s2;%% This callback is invoked when Paste operation is performed 
and can be used to alter the text to be pasted. Default is no 
change to the text.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:WhenHighlight: [_^Callback1^ Callback1]<[_^Vector^ Vector]<[_^EditField`:`:Highlight^ H
ighlight]>`&>_[* WhenHighlight]&]
[s2;%% Called by default implementation of HighlightText. Provides 
a chance to change the text color and background for individual 
characters.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:Insert`(int`,const WString`&`): [@(0.0.255) int]_[* Insert]([@(0.0.255) i
nt]_[*@3 pos], [@(0.0.255) const]_[_^WString^ WString][@(0.0.255) `&]_[*@3 text])&]
[s2;%% [%- Inserts ][%-*@3 text] at [%-*@3 pos].&]
[s3; &]
[s4; &]
[s5;:EditField`:`:Insert`(int`,const String`&`): [@(0.0.255) int]_[* Insert]([@(0.0.255) in
t]_[*@3 pos], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 text])&]
[s5;:EditField`:`:Insert`(int`,const char`*`): [@(0.0.255) int]_[* Insert]([@(0.0.255) int]_
[*@3 pos], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text])&]
[s2;%% [%- Inserts ][%-*@3 text] at [%-*@3 pos]. Text is converted to unicode 
using the default charset.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:Remove`(int`,int`): [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_[*@3 pos],
 [@(0.0.255) int]_[*@3 n])&]
[s2;%% [%- Removes ][%-*@3 n] characters at [%-*@3 pos].&]
[s3; &]
[s4; &]
[s5;:EditField`:`:Insert`(const WString`&`): [@(0.0.255) void]_[* Insert]([@(0.0.255) const
]_[_^WString^ WString][@(0.0.255) `&]_[*@3 text])&]
[s2;%% [%- Inserts] [%-*@3 text] at cursor position.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:Insert`(const String`&`): [@(0.0.255) void]_[* Insert]([@(0.0.255) const]_
[_^String^ String][@(0.0.255) `&]_[*@3 text])&]
[s5;:EditField`:`:Insert`(const char`*`): [@(0.0.255) void]_[* Insert]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 text])&]
[s2;%% [%- Inserts] [%-*@3 text] at cursor position. Text is converted 
to unicode using the default charset.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:Insert`(int`): [@(0.0.255) void]_[* Insert]([@(0.0.255) int]_[*@3 chr])&]
[s2;%% [%- Inserts a single unicode ][%-*@3 chr] at cursor position.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:Move`(int`,bool`): [@(0.0.255) void]_[* Move]([@(0.0.255) int]_[*@3 newpos],
 [@(0.0.255) bool]_[*@3 select]_`=_[@(0.0.255) false])&]
[s2;%% [%- Moves the cursor to] [%-*@3 newpos]. If [%-*@3 select] is true, 
selection is set between recent cursor position and [%-*@3 newpos].&]
[s3; &]
[s4; &]
[s5;:EditField`:`:SetSelection`(int`,int`): [@(0.0.255) void]_[* SetSelection]([@(0.0.255) i
nt]_[*@3 l]_`=_[@3 0], [@(0.0.255) int]_[*@3 h]_`=_INT`_MAX)&]
[s2;%% [%- Sets the selection if ][%-*@3 l] < [%-*@3 h][%- . If ][%-*@3 l] 
or [%-*@3 h ]exceed the available range (that is zero to length 
of text), they are bound to it. I[%- f ][%-*@3 l] >`= [%-*@3 h], selection 
is canceled and [%-*@3 l] is the new cursor position.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:GetSelection`(int`&`,int`&`)const: [@(0.0.255) bool]_[* GetSelection]([@(0.0.255) i
nt`&]_[*@3 l], [@(0.0.255) int`&]_[*@3 h])_[@(0.0.255) const]&]
[s2;%% [%- Returns the current selection. If there is no selection, 
both] [%-*@3 l] [%-*@3 h] return the current cursor position.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:IsSelection`(`)const: [@(0.0.255) bool]_[* IsSelection]()_[@(0.0.255) con
st]&]
[s2;%% [%- Returns true if there is a selection. ] &]
[s3; &]
[s4; &]
[s5;:EditField`:`:RemoveSelection`(`): [@(0.0.255) bool]_[* RemoveSelection]()&]
[s2; If there is no selection, returns false, otherwise it removes 
selected text and returns true.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:CancelSelection`(`): [@(0.0.255) void]_[* CancelSelection]()&]
[s2;%% Any selection is canceled (no characters are selected after 
this operation).&]
[s3; &]
[s4; &]
[s5;:EditField`:`:Copy`(`): [@(0.0.255) void]_[* Copy]()&]
[s2; Copies selected text to clipboard.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:Undo`(`): [@(0.0.255) void]_[* Undo]()&]
[s2;%% Performs undo operation.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:Cut`(`): [@(0.0.255) void]_[* Cut]()&]
[s2;%% Cuts the selection.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:Paste`(`): [@(0.0.255) void]_[* Paste]()&]
[s2;%% Pastes the content of clipboard, if compatible, into the EditField.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:Erase`(`): [@(0.0.255) void]_[* Erase]()&]
[s2;%% Removes the content of EditField.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:SelectAll`(`): [@(0.0.255) void]_[* SelectAll]()&]
[s2;%% Selects the whole text in EditField.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:StdBar`(Bar`&`): [@(0.0.255) void]_[* StdBar]([_^Bar^ Bar][@(0.0.255) `&]_[*@3 m
enu])&]
[s2;%% Standard menu of EditField.&]
[s3;%% &]
[s4; &]
[s5;:EditField`:`:SetText`(const WString`&`): [@(0.0.255) void]_[* SetText]([@(0.0.255) con
st]_[_^WString^ WString][@(0.0.255) `&]_[*@3 text])&]
[s2;%% [%- Sets the] [%-*@3 text].&]
[s3; &]
[s4; &]
[s5;:EditField`:`:SetText`(const String`&`): [@(0.0.255) void]_[* SetText]([@(0.0.255) cons
t]_[_^String^ String][@(0.0.255) `&]_[*@3 t])&]
[s5;:EditField`:`:SetText`(const char`*`): [@(0.0.255) void]_[* SetText]([@(0.0.255) const]_
[@(0.0.255) char]_`*[*@3 t])&]
[s2;%% [%- Sets the text. ]Text is converted to unicode using the default 
charset.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:GetText`(`)const: [@(0.0.255) const]_[_^WString^ WString][@(0.0.255) `&]_
[* GetText]()_[@(0.0.255) const]&]
[s2; Returns the text.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:GetLength`(`)const: [@(0.0.255) virtual] [@(0.0.255) int]_[* GetLength]()
_[@(0.0.255) const]&]
[s2; Returns the length of text.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2; Sets the empty text.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:Reset`(`): [@(0.0.255) void]_[* Reset]()&]
[s2; Resets EditField to default state.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:Error`(bool`): [@(0.0.255) void]_[* Error]([@(0.0.255) bool]_[*@3 error])&]
[s2;%% If [%-*@3 error] is true, paints the content as if it was invalid 
(the same way as when assigned Convert returns ErrorValue).&]
[s3;%% &]
[s4; &]
[s5;:EditField`:`:Password`(bool`): [_^EditField^ EditField][@(0.0.255) `&]_[* Password]([@(0.0.255) b
ool]_[*@3 pwd]_`=_[@(0.0.255) true])&]
[s2;%% Sets the password mode. In password mode all characters are 
displayed as asterisk. Returns `*this.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:IsPassword`(`)const: [@(0.0.255) bool]_[* IsPassword]()_[@(0.0.255) const
]&]
[s2;%% Returns true if password mode is active.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:SetFilter`(int`(`*`)`(int`)`): [_^EditField^ EditField][@(0.0.255) `&]_
[* SetFilter]([@(0.0.255) int]_(`*[*@3 f])([@(0.0.255) int]))&]
[s2;%% [%- Sets the character filter] [%-*@3 f]. All characters keystrokes 
are first altered by this function and used only if the result 
is not zero. Returns `*this.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:SetConvert`(const Convert`&`): [_^EditField^ EditField][@(0.0.255) `&]_
[* SetConvert]([@(0.0.255) const]_[_^Convert^ Convert][@(0.0.255) `&]_[*@3 c])&]
[s2;%% Sets the Value converter. By using the specific converters, 
EditField can be adapted for editing any type of values with 
text representation. Note that EditField stores just reference 
to [%-@3 c], therefore its lifetime must exceed the lifetime of 
EditField. Returns `*this.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:SetInactiveConvert`(const Convert`&`): [_^EditField^ EditField][@(0.0.255) `&
]_[* SetInactiveConvert]([@(0.0.255) const]_[_^Convert^ Convert][@(0.0.255) `&]_[*@3 c])&]
[s2;%% Sets the inactive Value converter. Such converter is used 
to format displayed value when edit control does not own the 
focus. Returns `*this.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:SetCharset`(byte`): [_^EditField^ EditField][@(0.0.255) `&]_[* SetCharset
]([_^byte^ byte]_[*@3 cs])&]
[s2;%% Sets the encoding to [%-*@3 cs] `- only characters representable 
in given encoding are allowed in EditField. Note that this does 
not impact encoding of String values returned from EditField, 
only acts as another filter.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:AutoFormat`(bool`): [_^EditField^ EditField][@(0.0.255) `&]_[* AutoFormat
]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% [%- In AutoFormat mode, when focus goes out of editfield, text 
representation of value is reformated by current Convert `- text 
is converted to Value and back. This mode is active by default.] 
Returns `*this.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:NoAutoFormat`(`): [_^EditField^ EditField][@(0.0.255) `&]_[* NoAutoFormat
]()&]
[s2; Same as AutoFormat(false).&]
[s3; &]
[s4; &]
[s5;:EditField`:`:IsAutoFormat`(`)const: [@(0.0.255) bool]_[* IsAutoFormat]()_[@(0.0.255) c
onst]&]
[s2;%% Returns true is AutoFormat is active.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:SetFont`(Font`): [_^EditField^ EditField][@(0.0.255) `&]_[* SetFont]([_^Font^ F
ont]_[*@3 `_font])&]
[s2;%% [%- Sets the ][%-*@3 font] of displayed text. Returns `*this.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:SetColor`(Color`): [_^EditField^ EditField][@(0.0.255) `&]_[* SetColor]([_^Color^ C
olor]_[*@3 c])&]
[s2;%% [%- Sets the color] of displayed text. Returns `*this.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:EditField`:`:SetBackground`(Upp`:`:Color`): EditField[@(0.0.255) `&] 
[* SetBackground](Color [*@3 c])&]
[s2;%% Sets the background color. Returns `*this.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:ClickSelect`(bool`): [_^EditField^ EditField][@(0.0.255) `&]_[* ClickSele
ct]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; In ClickSelect mode, all the text of EditField is select when 
clicked by mouse. Returns `*this.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:IsClickSelect`(`)const: [@(0.0.255) bool]_[* IsClickSelect]()_[@(0.0.255) c
onst]&]
[s2;%% Returns true if ClickSelect is active.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:InitCaps`(bool`): [_^EditField^ EditField][@(0.0.255) `&]_[* InitCaps]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; In InitCaps mode, initial letters of entered text are uppercased. 
Returns `*this.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:IsInitCaps`(`)const: [@(0.0.255) bool]_[* IsInitCaps]()_[@(0.0.255) const
]&]
[s2;%% Returns true if InitCaps is active.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:NullText`(const Image`&`,const char`*`,Color`): [_^EditField^ EditFie
ld][@(0.0.255) `&]_[* NullText]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 icon],
 [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text]_`=_t`_(`"([@(0.0.255) default])`"), 
[_^Color^ Color]_[*@3 ink]_`=_SColorDisabled)&]
[s5;:EditField`:`:NullText`(const Image`&`,const char`*`,Font`,Color`): [_^EditField^ E
ditField][@(0.0.255) `&]_[* NullText]([@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 i
con], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], [_^Font^ Font]_[*@3 fnt], 
[_^Color^ Color]_[*@3 ink])&]
[s5;:EditField`:`:NullText`(const char`*`,Color`): [_^EditField^ EditField][@(0.0.255) `&
]_[* NullText]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text]_`=_t`_(`"([@(0.0.255) defa
ult])`"), [_^Color^ Color]_[*@3 ink]_`=_SColorDisabled)&]
[s5;:EditField`:`:NullText`(const char`*`,Font`,Color`): [_^EditField^ EditField][@(0.0.255) `&
]_[* NullText]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], [_^Font^ Font]_[*@3 fnt], 
[_^Color^ Color]_[*@3 ink])&]
[s2; In NullText mode, empty EditField displays[%%  ][*@3 text][%%  using 
][*@3 ink][%%  color and ][%%*@3 fnt][%%  as a font, with ][*@3 icon][%% .] 
Returns `*this.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:MaxChars`(int`): [_^EditField^ EditField][@(0.0.255) `&]_[* MaxChars]([@(0.0.255) i
nt]_[*@3 mc])&]
[s2; Limits maximum number of characters to[%%  ][*@3 mc][%% .] Returns 
`*this.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:GetMaxChars`(`)const: [@(0.0.255) int]_[* GetMaxChars]()_[@(0.0.255) cons
t]&]
[s2;%% Returns the maximum number of characters limit set by MaxChars.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:AutoSize`(int`): [_^EditField^ EditField][@(0.0.255) `&]_[* AutoSize]([@(0.0.255) i
nt]_[*@3 maxcx]_`=_INT`_MAX)&]
[s2;%% [%- Resizes the width of EditField according to the length of 
text up to] [%-*@3 maxcx] width.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:NoBackground`(bool`): [_^EditField^ EditField][@(0.0.255) `&]_[* NoBackgr
ound]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% Sets the widget into transparent mode `- background is not 
painted and Transparent is activated `- a result, anything painted 
behind the widget is visible, allowing client code to provide 
any background it needs.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:GetFilter`(`)const: [_^CharFilter^ CharFilter]_[* GetFilter]()_[@(0.0.255) c
onst]&]
[s2;%% Returns current character filter.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:GetConvert`(`)const: [@(0.0.255) const]_[_^Convert^ Convert][@(0.0.255) `&
]_[* GetConvert]()_[@(0.0.255) const]&]
[s2; Returns current converter.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:GetFont`(`)const: [_^Font^ Font]_[* GetFont]()_[@(0.0.255) const]&]
[s2; Returns current font.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:AlignRight`(bool`): [_^EditField^ EditField][@(0.0.255) `&]_[* AlignRight
]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% If [%-*@3 b].is true, the text will be displayed aligned to 
the right. Left alignment is default.&]
[s3;%% &]
[s4; &]
[s5;:EditField`:`:IsNoBackground`(`)const: [@(0.0.255) bool]_[* IsNoBackground]()_[@(0.0.255) c
onst]&]
[s2;%% Returns true if NoBackground is active.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:IsAlignRight`(`)const: [@(0.0.255) bool]_[* IsAlignRight]()_[@(0.0.255) c
onst]&]
[s2;%% Returns true if AlignRight is active.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:ShowSpaces`(bool`): [@(0.0.255) bool]_[* ShowSpaces]([@(0.0.255) bool]_[*@3 b
]_`=_[@(0.0.255) true])&]
[s2;%% When active, EditField paints blue dots to show spaces.&]
[s3;%% &]
[s4; &]
[s5;:EditField`:`:NoInternalMargin`(bool`): [_^EditField^ EditField][@(0.0.255) `&]_[* NoIn
ternalMargin]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% Normally, EditField has small internal margin between frame 
and edited text. Calling this method with [%-@(0.0.255) true] removes 
it.&]
[s3;%% &]
[s4; &]
[s5;:EditField`:`:GetChar`(int`)const: [@(0.0.255) virtual] [@(0.0.255) int]_[* GetChar]([@(0.0.255) i
nt]_[*@3 i])_[@(0.0.255) const]&]
[s2;%% Returns the character at [%-*@3 i] index.&]
[s3;%% &]
[s4; &]
[s5;:EditField`:`:GetCaretRect`(int`)const: [_^Rect^ Rect]_[* GetCaretRect]([@(0.0.255) int
]_[*@3 pos])_[@(0.0.255) const]&]
[s2;%% Returns the caret placement for character [%-*@3 pos] in EditField 
view coordinates.&]
[s3;%% &]
[s4; &]
[s5;:EditField`:`:GetCaretRect`(`)const: [_^Rect^ Rect]_[* GetCaretRect]()_[@(0.0.255) cons
t]&]
[s2;%% Same as GetCaretRect(cursor).&]
[s3; &]
[s4; &]
[s5;:EditField`:`:StyleDefault`(`): [@(0.0.255) static] [@(0.0.255) const]_[_^EditField`:`:Style^ S
tyle][@(0.0.255) `&]_[* StyleDefault]()&]
[s2;%% Returns the default style used to draw edit fields.&]
[s3; &]
[s4; &]
[s5;:EditField`:`:SetStyle`(const EditField`:`:Style`&`): [_^EditField^ EditField][@(0.0.255) `&
]_[* SetStyle]([@(0.0.255) const]_[_^EditField`:`:Style^ Style][@(0.0.255) `&]_[*@3 s])&]
[s2;%% Sets the style to an EditField`::Style [%-*@3 s].&]
[s3;%% &]
[s0; &]
[ {{10000@3 [s0;%% [*@(229)4 EditField`::Style]]}}&]
[s3; &]
[s1;:EditField`:`:Style`:`:struct: [@(0.0.255)3 struct][3 _][*3 Style][3 _:_][@(0.0.255)3 public
][3 _][*@3;3 ChStyle][@(0.0.255)3 <][*3 Style][@(0.0.255)3 >][3 _]&]
[s9;%% This structure defines the visual style of an EditField.&]
[s0; ]]