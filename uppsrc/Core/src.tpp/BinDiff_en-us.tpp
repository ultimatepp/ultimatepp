topic "BinDiff/UndoRedo";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 Binary diff / undiff]]}}&]
[s3; &]
[s5;:Upp`:`:BinDiff`(const String`&`,const String`&`): String [* BinDiff]([@(0.0.255) con
st] String[@(0.0.255) `&] [*@3 base], [@(0.0.255) const] String[@(0.0.255) `&] 
[*@3 data])&]
[s2;%% Efficiently encodes differences between [%-*@3 base] and [%-*@3 data]. 
&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:BinUndiff`(const String`&`,const String`&`): String [* BinUndiff]([@(0.0.255) c
onst] String[@(0.0.255) `&] [*@3 base], [@(0.0.255) const] String[@(0.0.255) `&] 
[*@3 bin`_diff])&]
[s2;%% With [%-*@3 bin`_diff] being the return value of [* BinDiff] restores 
[* data] with given [%-*@3 base].&]
[s3; &]
[s0; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 BinUndoRedo]]}}&]
[s0; &]
[s1;:Upp`:`:BinUndoRedo: [*@(0.0.255)3 class ][*3 BinUndoRedo]&]
[s2; Efficiently stores a list of similar String data. Standard usage 
for implementing Undo/Redo `- application would serialize the 
snapshot of content into binary String then use [* Commit ]to store 
data into BinUndoRedo. Lately, it can retrieve binary String 
with [* Undo]/[* Redo] a serialize it back into content.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:BinUndoRedo`:`:Reset`(const String`&`): [@(0.0.255) void] 
[* Reset]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 current])&]
[s2;%% Sets the initial data content. This is typically used after 
loading the data into application or creating a new content.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:BinUndoRedo`:`:Commit`(const String`&`,int`): [@(0.0.255) bool] 
[* Commit]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 current], [@(0.0.255) int] 
[*@3 limit] [@(0.0.255) `=] [@3 4096] [@(0.0.255) `*][@3 1024])&]
[s2;%% Adds a snapshot of content [%-*@3 current] effectively creating 
single undo step. [%-*@3 limit] represents maximum memory that 
can be used to store undo steps (when there is more, oldest steps 
are dropped).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:BinUndoRedo`:`:IsUndo`(`)const: [@(0.0.255) bool] [* IsUndo]() 
[@(0.0.255) const]&]
[s2;%% Returns true if undo steps are available.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:BinUndoRedo`:`:IsRedo`(`)const: [@(0.0.255) bool] [* IsRedo]() 
[@(0.0.255) const]&]
[s2;%% Returns true if undo steps are available..&]
[s3; &]
[s4; &]
[s5;:Upp`:`:BinUndoRedo`:`:Undo`(const String`&`): String [* Undo]([@(0.0.255) const] 
String[@(0.0.255) `&] [*@3 current])&]
[s2;%% Given snapshot of content [%-*@3 current], does one undo step 
and returns content snapshot after undo.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:BinUndoRedo`:`:Redo`(const String`&`): String [* Redo]([@(0.0.255) const] 
String[@(0.0.255) `&] [*@3 current])&]
[s2;%% Given snapshot of content [%-*@3 current], does one redo step 
and returns content snapshot after redo.&]
[s3; &]
[s0;%% ]]