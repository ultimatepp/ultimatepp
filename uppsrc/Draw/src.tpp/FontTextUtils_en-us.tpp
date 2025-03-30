topic "Font and Text Routines";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 Font and Text Routines]]}}&]
[s3; &]
[s5;:Upp`:`:SetStdFont`(Upp`:`:Font`): [@(0.0.255) void]_[* SetStdFont]([_^Upp`:`:Font^ Fon
t]_[*@3 font])&]
[s2;%% Sets the standard font. This is the default font used to draw 
most GUI texts. Standard font is normally set during GUI startup 
and application rarely need to call this function.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:GetStdFont`(`): [_^Upp`:`:Font^ Font]_[* GetStdFont]()&]
[s2;%% Returns the standard font.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:GetStdFontCy`(`): [@(0.0.255) int]_[* GetStdFontCy]()&]
[s2;%% Returns the standard font height (in pixels or dots).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:GetStdFontCyA`(`): [@(0.0.255) int] [* GetStdFontCyA]()&]
[s2;%% Returns the standard font height (in pixels or dots), including 
bold and italic variants (maximum is used).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StdFont`(`): [_^Upp`:`:Font^ Font]_[* StdFont]()&]
[s2;%% Same as GetStdFont().&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StdFont`(int`): [_^Upp`:`:Font^ Font]_[* StdFont]([@(0.0.255) int]_[*@3 h])&]
[s2;%% Returns standard font with height changed to [%-*@3 h].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Serif`(int`): [_^Upp`:`:Font^ Font]_[* Serif]([@(0.0.255) int]_[*@3 n])&]
[s2;%% Returns default serif font with height [%-*@3 n].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SansSerif`(int`): [_^Upp`:`:Font^ Font]_[* SansSerif]([@(0.0.255) int]_[*@3 n]_`=
_`-[@3 32000])&]
[s2;%% Returns default sans`-serif font with height [%-*@3 n].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Monospace`(int`): [_^Upp`:`:Font^ Font]_[* Monospace]([@(0.0.255) int]_[*@3 n]_`=
_`-[@3 32000])&]
[s2;%% Returns default monospace font with height [%-*@3 n].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Compose`(Upp`:`:Font`,int`,Upp`:`:ComposedGlyph`&`): [@(0.0.255) bool]_[* Com
pose]([_^Upp`:`:Font^ Font]_[*@3 font], [@(0.0.255) int]_[*@3 chr], [_^Upp`:`:ComposedGlyph^ C
omposedGlyph][@(0.0.255) `&]_[*@3 cg])&]
[s2;%% This function is used by DrawText implementation when unicode 
point [%-*@3 chr] is missing in [%-*@3 font]. Function tries to find 
two glyphs, one from original font and second possibly from other 
font which, when composed, result in character corresponding 
to required codepoint (e.g. [*@(255.0.255) C] `+ [*@(255.0.255) ˇ] 
`= [*@(255.0.255) Č]). If such glyphs exist, they are returned 
in [%-*@3 cg] and function returns true.&]
[s3;%% &]
[s4; &]
[s5;:GetTextSize`(const wchar`*`,Font`,int`): [_^Size^ Size]_[* GetTextSize]([@(0.0.255) co
nst]_[_^wchar^ wchar]_`*[*@3 text], [_^Font^ Font]_[*@3 font], [@(0.0.255) int]_[*@3 n]_`=_`-
[@3 1])&]
[s5;:GetTextSize`(const WString`&`,Font`): [_^Size^ Size]_[* GetTextSize]([@(0.0.255) const
]_[_^WString^ WString][@(0.0.255) `&]_[*@3 text], [_^Font^ Font]_[*@3 font])&]
[s5;:GetTextSize`(const char`*`,byte`,Font`,int`): [_^Size^ Size]_[* GetTextSize]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 text], [_^byte^ byte]_[*@3 charset], [_^Font^ Font]_[*@3 font],
 [@(0.0.255) int]_[*@3 n]_`=_`-[@3 1])&]
[s5;:GetTextSize`(const char`*`,Font`,int`): [_^Size^ Size]_[* GetTextSize]([@(0.0.255) con
st]_[@(0.0.255) char]_`*[*@3 text], [_^Font^ Font]_[*@3 font], [@(0.0.255) int]_[*@3 n]_`=_`-
[@3 1])&]
[s5;:GetTextSize`(const String`&`,Font`): [_^Size^ Size]_[* GetTextSize]([@(0.0.255) const]_
[_^String^ String][@(0.0.255) `&]_[*@3 text], [_^Font^ Font]_[*@3 font])&]
[s2;%% Computes the graphical size of [%-*@3 text] if drawn with [%-*@3 font], 
using font metrics. Returned height is the height of font (ascent`+descent), 
returned with is the total advance width of text. No overhangs 
are considered. [%-*@3 n] is a number of characters; if negative 
(default value), it is obtained using `[w`]strlen. Text can either 
be unicode or in 8`-bit encoding; in that case either [%-*@3 charset] 
is provided or it is assumed to be default charset.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:DrawTextEllipsis`(Upp`:`:Draw`&`,int`,int`,int`,const char`*`,const char`*`,Upp`:`:Font`,Upp`:`:Color`,int`): [@(0.0.255) v
oid]_[* DrawTextEllipsis]([_^Upp`:`:Draw^ Draw][@(0.0.255) `&]_[*@3 w], 
[@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 e
llipsis], [_^Upp`:`:Font^ Font]_[*@3 font]_`=_StdFont(), [_^Upp`:`:Color^ Color]_[*@3 ink
]_`=_SColorText(), [@(0.0.255) int]_[*@3 n]_`=_`-[@3 1])&]
[s5;:Upp`:`:DrawTextEllipsis`(Upp`:`:Draw`&`,int`,int`,int`,const Upp`:`:wchar`*`,const char`*`,Upp`:`:Font`,Upp`:`:Color`,int`): [@(0.0.255) v
oid]_[* DrawTextEllipsis]([_^Upp`:`:Draw^ Draw][@(0.0.255) `&]_[*@3 w], 
[@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], 
[@(0.0.255) const]_[_^Upp`:`:wchar^ wchar]_`*[*@3 text], [@(0.0.255) const]_[@(0.0.255) cha
r]_`*[*@3 ellipsis], [_^Upp`:`:Font^ Font]_[*@3 font]_`=_StdFont(), 
[_^Upp`:`:Color^ Color]_[*@3 ink]_`=_SColorText(), [@(0.0.255) int]_[*@3 n]_`=_`-[@3 1])&]
[s2;%% Draws a [%-*@3 text] line limited in width to [%-*@3 cx]. If the 
text is longer than what could be fitted to [%-*@3 cx] , it is 
it is truncated so that it fits together with [%-*@3 ellipsis] 
text appended into [%-*@3 cx], then drawn with [%-*@3 ellipsis] appended. 
[%-*@3 n] is the number of characters to be drawn, if `-1, `[w`]strlen(text) 
is used instead.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:DrawTLText`(Upp`:`:Draw`&`,int`,int`,int`,const Upp`:`:wchar`*`,Upp`:`:Font`,Upp`:`:Color`,int`): [@(0.0.255) v
oid]_[* DrawTLText]([_^Upp`:`:Draw^ Draw][@(0.0.255) `&]_[*@3 draw], 
[@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y], [@(0.0.255) int]_[*@3 cx], 
[@(0.0.255) const]_[_^Upp`:`:wchar^ wchar]_`*[*@3 text], [_^Upp`:`:Font^ Font]_[*@3 font]_`=
_StdFont(), [_^Upp`:`:Color^ Color]_[*@3 ink]_`=_SColorText(), [@(0.0.255) int]_[*@3 acce
sskey]_`=_[@3 0])&]
[s2;%% This function draws multiline text with tabs (contains `'`\n`' 
and `'`\t`'), possibly underline single character [%-*@3 accesskey 
]in the text. Width is limited by [%-*@3 cx] and text is word `- 
wrapped if line is longer.&]
[s0;%% &]
[s4; &]
[s5;:Upp`:`:GetTLTextSize`(const Upp`:`:wchar`*`,Upp`:`:Font`): [_^Upp`:`:Size^ Size]_[* G
etTLTextSize]([@(0.0.255) const]_[_^Upp`:`:wchar^ wchar]_`*[*@3 text], 
[_^Upp`:`:Font^ Font]_[*@3 font]_`=_StdFont())&]
[s2;%% Returns the size of multiline text with tabs.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:GetTLTextHeight`(const Upp`:`:wchar`*`,Upp`:`:Font`): [@(0.0.255) int]_[* Get
TLTextHeight]([@(0.0.255) const]_[_^Upp`:`:wchar^ wchar]_`*[*@3 s], 
[_^Upp`:`:Font^ Font]_[*@3 font])&]
[s2;%% Same as GetTLTextSize(s, font).cy.&]
[s3;%% &]
[s0; ]]