topic "Font and Text Routines";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 Font and Text Routines]]}}&]
[s3; &]
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
[s0; ]