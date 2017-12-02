topic "Splitter";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[{_} 
[ {{10000@3 [s0;%% [*@(229)4 Splitter]]}}&]
[s3;%% &]
[s1;:Splitter`:`:class: [@(0.0.255)3 class][3 _][*3 Splitter][3 _:_][@(0.0.255)3 public][3 _][*@3;3 C
trl]&]
[s9;%% Splitter is a widget that contains two or more child widgets 
arranged either horizontally or vertically. User can then alter 
the distribution of area between individual widgets by dragging 
separator bars.&]
[s3;%% &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Splitter`:`:WhenSplitFinish: [_^Callback^ Callback]_[* WhenSplitFinish]&]
[s2;%% Invoked after use has finished moving the split.&]
[s3; &]
[s4; &]
[s5;:Splitter`:`:Set`(Ctrl`&`,Ctrl`&`): [@(0.0.255) void]_[* Set]([_^Ctrl^ Ctrl][@(0.0.255) `&
]_[*@3 l], [_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 r])&]
[s2;%% Sets t splitter to contain two child widgets.&]
[s3; &]
[s4; &]
[s5;:Splitter`:`:SetPos`(int`,int`): [_^Splitter^ Splitter][@(0.0.255) `&]_[* SetPos]([@(0.0.255) i
nt]_[*@3 newpos], [@(0.0.255) int]_[*@3 index]_`=_[@3 0])&]
[s2; [%% Sets the position of separator between widgets at ][*@3 index][%%  
and ][*@3 index][%%  `+ 1. Units of ][*@3 newpos] express ratio, 10000 
is the full width or height.&]
[s3;%% &]
[s4; &]
[s5;:Splitter`:`:GetPos`(int`)const: [@(0.0.255) int]_[* GetPos]([@(0.0.255) int]_[*@3 index]_
`=_[@3 0])_[@(0.0.255) const]&]
[s2;%% Returns the position of separator between widgets at [%-*@3 index] 
and [%-*@3 index] `+ 1.&]
[s3;%% &]
[s4; &]
[s5;:Splitter`:`:GetCount`(`)const: [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s2;%% Returns the number of child widgets.&]
[s3;%% &]
[s4; &]
[s5;:Splitter`:`:PosToClient`(int`)const: [@(0.0.255) int]_[* PosToClient]([@(0.0.255) int]_
[*@3 pos])_[@(0.0.255) const]&]
[s2;%% Converts position (where full width or height is 10000) into 
view coordinate.&]
[s3;%% &]
[s4; &]
[s5;:Splitter`:`:ClientToPos`(Point`)const: [@(0.0.255) int]_[* ClientToPos]([_^Point^ Poin
t]_[*@3 client])_[@(0.0.255) const]&]
[s2;%% Converts view coordinate (either x or y, based on orientaion) 
into position  (where full width or height is 10000).&]
[s3;%% &]
[s4; &]
[s5;:Splitter`:`:Zoom`(int`): [@(0.0.255) void]_[* Zoom]([@(0.0.255) int]_[*@3 i])&]
[s2;%% Makes widget [%-*@3 i] to cover whole splitter area (other widgets 
are hidden).&]
[s3;%% &]
[s4; &]
[s5;:Splitter`:`:NoZoom`(`): [@(0.0.255) void]_[* NoZoom]()&]
[s2;%% Reverts to normal operation after Zoom.&]
[s3;%% &]
[s4; &]
[s5;:Splitter`:`:GetZoom`(`)const: [@(0.0.255) int]_[* GetZoom]()_[@(0.0.255) const]&]
[s2;%% Returns the index of zoomed widget or negative value if none. 
 &]
[s3;%% &]
[s4; &]
[s5;:Splitter`:`:SetMin`(int`,int`): [@(0.0.255) void]_[* SetMin]([@(0.0.255) int]_[*@3 i], 
[@(0.0.255) int]_[*@3 w])&]
[s2;%% Sets the minimal width of widget  [%-*@3 i] to [%-*@3 w] `* total 
width / 10000.&]
[s3;%% &]
[s4; &]
[s5;:Splitter`:`:SetMinPixels`(int`,int`): [@(0.0.255) void]_[* SetMinPixels]([@(0.0.255) i
nt]_[*@3 i], [@(0.0.255) int]_[*@3 w])&]
[s2;%% Sets the minimal width of widget  [%-*@3 i] to [%-*@3 w] pixels.&]
[s3;%% &]
[s4; &]
[s5;:Splitter`:`:GetSplitWidth`(`)const: [@(0.0.255) int]_[* GetSplitWidth]()_[@(0.0.255) c
onst]&]
[s2;%% Width of spllitter handle in pixels.&]
[s3; &]
[s4; &]
[s5;:Splitter`:`:Vert`(`): [_^Splitter^ Splitter][@(0.0.255) `&]_[* Vert]()&]
[s2;%% Sets splitter to vertical mode `- separator bars in this mode 
are horizontal. Returns `*this.&]
[s3;%% &]
[s4; &]
[s5;:Splitter`:`:Horz`(`): [_^Splitter^ Splitter][@(0.0.255) `&]_[* Horz]()&]
[s2;%% Sets splitter to horizontal mode `- separator bars in this 
mode are vertical. Returns `*this.&]
[s3;%% &]
[s4; &]
[s5;:Splitter`:`:Add`(Ctrl`&`): [@(0.0.255) virtual] [@(0.0.255) void]_[* Add]([_^Ctrl^ Ctrl][@(0.0.255) `&
]_[*@3 pane])&]
[s5;:Splitter`:`:operator`<`<`(Ctrl`&`): [@(0.0.255) virtual] [_^Splitter^ Splitter][@(0.0.255) `&
]_[* operator<<]([_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 pane])&]
[s2;%% Adds the new [%-*@3 pane].&]
[s3;%% &]
[s4; &]
[s5;:Splitter`:`:Insert`(int`,Ctrl`&`): [@(0.0.255) void]_[* Insert]([@(0.0.255) int]_[*@3 po
s], [_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 pane])&]
[s2;%% Inserts a new [%-*@3 pane] at [%-*@3 pos].&]
[s3;%% &]
[s4; &]
[s5;:Splitter`:`:Remove`(Ctrl`&`): [@(0.0.255) void]_[* Remove]([_^Ctrl^ Ctrl][@(0.0.255) `&]_
[*@3 pane])&]
[s2;%% Removes [%-*@3 pane] from splitter.&]
[s3;%% &]
[s4; &]
[s5;:Splitter`:`:Swap`(Ctrl`&`,Ctrl`&`): [@(0.0.255) void]_[* Swap]([_^Ctrl^ Ctrl][@(0.0.255) `&
]_[*@3 pane], [_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 newpane])&]
[s2;%% Replaces [%-*@3 pane] with [%-*@3 newpane] .&]
[s3;%% &]
[s4; &]
[s5;:Splitter`:`:Horz`(Ctrl`&`,Ctrl`&`): [_^Splitter^ Splitter][@(0.0.255) `&]_[* Horz]([_^Ctrl^ C
trl][@(0.0.255) `&]_[*@3 left], [_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 right])&]
[s2;%% Same as Set([%-*@3 left], [%-*@3 right]); return Vert();&]
[s3;%% &]
[s4; &]
[s5;:Splitter`:`:Vert`(Ctrl`&`,Ctrl`&`): [_^Splitter^ Splitter][@(0.0.255) `&]_[* Vert]([_^Ctrl^ C
trl][@(0.0.255) `&]_[*@3 top], [_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 bottom])&]
[s2;%% Same as Set([%-*@3 top] [%-*@3 bottom]); return Horz();&]
[s3;%% &]
[s4; &]
[s5;:Splitter`:`:BarWidth`(int`): [_^Splitter^ Splitter][@(0.0.255) `&]_[* BarWidth]([@(0.0.255) i
nt]_[*@3 w])&]
[s2;%% Sets the width of separator bar to [%-*@3 w].&]
[s3;%% &]
[s4; &]
[s5;:Splitter`:`:IsHorz`(`)const: [@(0.0.255) bool]_[* IsHorz]()_[@(0.0.255) const]&]
[s5;:Splitter`:`:IsVert`(`)const: [@(0.0.255) bool]_[* IsVert]()_[@(0.0.255) const]&]
[s2;%% Tests whether Splitter is horizontal or vertical.&]
[s3;%% &]
[s4; &]
[s5;:Splitter`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%% Removes panes from Splitter.&]
[s3; &]
[s4; &]
[s5;:Splitter`:`:Reset`(`): [@(0.0.255) void]_[* Reset]()&]
[s2;%% Resets Splitter to default state.&]
[s3;%% &]
[s0;%% ]]