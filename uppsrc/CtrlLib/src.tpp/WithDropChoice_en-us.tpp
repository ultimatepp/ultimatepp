topic "WithDropChoice";
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
[ {{10000@3 [s0; [*@(229)4 WithDropChoice]]}}&]
[s3; &]
[s1;:noref: [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@3;3 T][@(0.0.255)3 >]&]
[s1;:WithDropChoice`:`:class: [@(0.0.255) class]_[* WithDropChoice]_:_[@(0.0.255) public]_[*@3 T
]&]
[s0;%% &]
[s2;%% 
@@image:1350&331
(A9gANQAAAMv/AAAAAHic7ZyxTttAGIDzSjxH36EP0KFrK8YGhi6gMmUoZUFCLJ0oyogKQUItIkIIVVUpDjE1hEilhbaj+0MU1707J2fHds7o+/QNtrGTIH3c5SCmf/mpj4iIQ5++fItYssYOQ4ASoUMogg+frxvvzmyUM0M6hGKQwM77f2yUM0M6hGKQuvyr3zbad1gbMqXvCaqH1NXt/bLRssN4fvmmaP9o/AhUDqnr7PLWxgwd5gsdPmDuO7wZ+Ojxs2hbPzJhh8pkHd9VJvGkaT3p8viTGh8Q3Efq6lzciJLcwMGufmSS94f69oiEjFcZL0l6NCKsHFKXF/wcGIWnbA/MsF62Lyf8fygzDon6YKiPkPrTQSW47/BHZJTfMMJ/Xyqtw6SXOjozOqw0Utfpt+u4UYTK8UnWy8zLMBqp6+v5d0WJUD844e8PR68slI2khYblOiUkxaph7NAof0+B4rjrsHthIx1Ccdx16Hk20iEUB5+3gSqS1CFiyeod1gBKx9jhzJMWYmnSIbrg2A6V95NTf8H4ILUcD9MWSLGYSjpEF5TkvJN9v9MO/KNecGzfoTJTKxtM4phKSW5vt3l4sHX65aOkaNlhfDcKz7iLaKMkt7baaG6uS4pdr23fob54MQ6PiDZKcosLdUlRRkWZoDOMh/GDdIjZlOTm52ZXlpda2xsyNTMv41SU5Oovnr95/WrnfYoOZ8atU4yXICZp2SFiodIhumBSh+k/QQaQHToEF6BDcAE6BBegQ3CBsR3WuJMdiofxEFyADsEF6BBcgA7BBVingAswHoIL0CG4QFKHACWjd6jonezv7TbXVhuLC/X5uVk5HzF39ftTFP1O+/Bgq7m5LqetLC9JtIi5q9+vpxj4RzJUygnSamt7Q0ZOxNzV719W7AXH8iWpVAZMOQ2xCOP/z+EvcWfmOg==)
&]
[s0;%% &]
[s9;%% This template class adds a list of choices, with drop down 
button placed at right side of the frame, to any widget with 
associated value accessible via SetData/GetData interface.&]
[s3;%% &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Member List]]}}&]
[s3; &]
[s5;:WithDropChoice`:`:WhenDrop: [_^Callback^ Callback]_[* WhenDrop]&]
[s2;%% This callback is invoked when the drop button is pushed (before 
drop down list appears). Client code can use it to create or 
alter the list.&]
[s3;%% &]
[s4; &]
[s5;:WithDropChoice`:`:WhenSelect: [_^Callback^ Callback]_[* WhenSelect]&]
[s2;%% This callback is called after user selected an item from drop`-down 
list and this value was assigned to the base widget.&]
[s3;%% &]
[s4; &]
[s5;:WithDropChoice`:`:ClearList`(`): [@(0.0.255) void]_[* ClearList]()&]
[s2;%% Clears the drop`-down list.&]
[s3;%% &]
[s4; &]
[s5;:WithDropChoice`:`:AddList`(const Value`&`): [@(0.0.255) void]_[* AddList]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 data])&]
[s2;%% Adds [%-*@3 data] to the drop`-down list.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:WithDropChoice`:`:FindAddList`(const Upp`:`:Value`&`): [@(0.0.255) void]_[* F
indAddList]([@(0.0.255) const]_[_^Upp`:`:Value^ Value][@(0.0.255) `&]_[*@3 data])&]
[s2;%% Adds [%-*@3 data] to the drop`-down list, by only if it is not 
already there.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:WithDropChoice`:`:Find`(const Upp`:`:Value`&`)const: [@(0.0.255) int]_[* Find
]([@(0.0.255) const]_[_^Upp`:`:Value^ Value][@(0.0.255) `&]_[*@3 data])_[@(0.0.255) const]&]
[s2;%% Finds the index of first list entry equal to [%-*@3 data].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:WithDropChoice`:`:Set`(int`,const Upp`:`:Value`&`): [@(0.0.255) void]_[* Set](
[@(0.0.255) int]_[*@3 i], [@(0.0.255) const]_[_^Upp`:`:Value^ Value][@(0.0.255) `&]_[*@3 data
])&]
[s2;%% Sets entry [%-*@3 i] in drop`-down list to [%-*@3 data] .&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:WithDropChoice`:`:Remove`(int`): [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_
[*@3 i])&]
[s2;%% Removes entry [%-*@3 i] in drop`-down list.&]
[s3;%% &]
[s4; &]
[s5;:WithDropChoice`:`:SerializeList`(Stream`&`): [@(0.0.255) void]_[* SerializeList]([_^Stream^ S
tream][@(0.0.255) `&]_[*@3 s])&]
[s2;%% Serializes the drop`-down list content.&]
[s3;%% &]
[s4; &]
[s5;:WithDropChoice`:`:AddHistory`(int`): [@(0.0.255) void]_[* AddHistory]([@(0.0.255) int]_
[*@3 max]_`=_[@3 12])&]
[s2;%% This method puts the value to the begin of drop`-down list. 
If value is already in the list, it is removed first (values 
in the list are unique). If there is more than [%-*@3 max] items 
in the list, list is trimmed. This method is ideal when you want 
to keep the history of previous values.&]
[s3;%% &]
[s4; &]
[s5;:WithDropChoice`:`:Dropping`(bool`): [_^WithDropChoice^ WithDropChoice][@(0.0.255) `&
]_[* Dropping]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% In this mode, drop button is visible. Can be used to temporarily 
remove the drop`-down list capability. Default is on.&]
[s3;%% &]
[s4; &]
[s5;:WithDropChoice`:`:NoDropping`(`): [_^WithDropChoice^ WithDropChoice][@(0.0.255) `&]_
[* NoDropping]()&]
[s2;%% Same as Dropping(false).&]
[s3;%% &]
[s4; &]
[s5;:WithDropChoice`:`:NoDropFocus`(`): [_^WithDropChoice^ WithDropChoice][@(0.0.255) `&]_
[* NoDropFocus]()&]
[s2;%% If NoDropFocus is active, focus is not moved to base widget 
when drop button is pushed. Default is to move the focus.&]
[s3;%% &]
[s4; &]
[s5;:WithDropChoice`:`:Appending`(const String`&`,`"`): [_^WithDropChoice^ WithDropChoi
ce][@(0.0.255) `&]_[* Appending]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s]_
`=_`", `")&]
[s2; [*@3 s]&]
[s3;%% &]
[s4; &]
[s5;:WithDropChoice`:`:SetDropLines`(int`): [_^WithDropChoice^ WithDropChoice][@(0.0.255) `&
]_[* SetDropLines]([@(0.0.255) int]_[*@3 n])&]
[s2;%% Sets the maximum height of drop`-down list to accommodate 
[%-*@3 n] lines (scrollbar appears if there is more than that).&]
[s3;%% &]
[s4; &]
[s5;:WithDropChoice`:`:SetDisplay`(int`,const Display`&`): [_^WithDropChoice^ WithDropC
hoice][@(0.0.255) `&]_[* SetDisplay]([@(0.0.255) int]_[*@3 i], [@(0.0.255) const]_[_^Display^ D
isplay][@(0.0.255) `&]_[*@3 d])&]
[s2;%% Sets the Display to be used with drop`-down list item [%-*@3 i].&]
[s3;%% &]
[s4; &]
[s5;:WithDropChoice`:`:SetDisplay`(const Display`&`): [_^WithDropChoice^ WithDropChoice
][@(0.0.255) `&]_[* SetDisplay]([@(0.0.255) const]_[_^Display^ Display][@(0.0.255) `&]_[*@3 d
])&]
[s2;%% Sets the Display to be used with drop`-down list, unless item`-specific 
Display is assigned.&]
[s3;%% &]
[s4; &]
[s5;:WithDropChoice`:`:SetLineCy`(int`): [_^WithDropChoice^ WithDropChoice][@(0.0.255) `&
]_[* SetLineCy]([@(0.0.255) int]_[*@3 lcy])&]
[s2;%% Sets the line height of drop`-down display.&]
[s3;%% &]
[s4; &]
[s5;:WithDropChoice`:`:SetDisplay`(const Display`&`,int`): [_^WithDropChoice^ WithDropC
hoice][@(0.0.255) `&]_[* SetDisplay]([@(0.0.255) const]_[_^Display^ Display][@(0.0.255) `&]_
[*@3 d], [@(0.0.255) int]_[*@3 lcy])&]
[s2;%% Same as SetDisplay(d).SetLineCy(lcy).&]
[s3;%% &]
[s4; &]
[s5;:WithDropChoice`:`:SetConvert`(const Convert`&`): [_^WithDropChoice^ WithDropChoice
][@(0.0.255) `&]_[* SetConvert]([@(0.0.255) const]_[_^Convert^ Convert][@(0.0.255) `&]_[*@3 d
])&]
[s2;%% Sets the Convert to be used with drop`-down list item [%-*@3 i].&]
[s3;%% &]
[s4; &]
[s5;:WithDropChoice`:`:AlwaysDrop`(bool`): [_^WithDropChoice^ WithDropChoice][@(0.0.255) `&
]_[* AlwaysDrop]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% If active, drop button is visible even if the list is empty 
(client code can fill it using WhenDrop callback).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:WithDropChoice`:`:HideDrop`(bool`): [_^Upp`:`:WithDropChoice^ WithDropChoic
e][@(0.0.255) `&]_[* HideDrop]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% Instead of disabling the drop arrow when the list is empty 
and AlwaysDrop is false, the drop arrow is hidden.&]
[s3;%% &]
[s4; &]
[s5;:WithDropChoice`:`:RdOnlyDrop`(bool`): [_^WithDropChoice^ WithDropChoice][@(0.0.255) `&
]_[* RdOnlyDrop]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% If active, allows changing value even by drop even if ReadOnly.&]
[s3;%% &]
[s4; &]
[s5;:WithDropChoice`:`:WithWheel`(bool`): [_^WithDropChoice^ WithDropChoice][@(0.0.255) `&
]_[* WithWheel]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% Widget reacts to mousewheel. This is active by default.&]
[s3;%% &]
[s4; &]
[s5;:WithDropChoice`:`:NoWithWheel`(`): [_^WithDropChoice^ WithDropChoice][@(0.0.255) `&]_
[* NoWithWheel]()&]
[s2;%% Same as WithWheel(false).&]
[s3; &]
[s4; &]
[s5;:WithDropChoice`:`:DropWidth`(int`): [_^WithDropChoice^ WithDropChoice][@(0.0.255) `&
]_[* DropWidth]([@(0.0.255) int]_[*@3 w])&]
[s2;%% Set the width of the drop down list to [%-*@3 w].&]
[s3;%% &]
[s4; &]
[s5;:WithDropChoice`:`:DropWidthZ`(int`): [_^WithDropChoice^ WithDropChoice][@(0.0.255) `&
]_[* DropWidthZ]([@(0.0.255) int]_[*@3 w])&]
[s2;%% Set the width of the drop down list to a value obtained by 
zooming [%-*@3 w] in accordance with font scaling.&]
[s3;%% &]
[s4; &]
[s5;:WithDropChoice`:`:UpDownKeys`(bool`): [_^WithDropChoice^ WithDropChoice][@(0.0.255) `&
]_[* UpDownKeys]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% If active, Up and Down keys cycle through the history. Default 
is active..&]
[s3;%% &]
[s4; &]
[s5;:WithDropChoice`:`:NoUpDownKeys`(`): [_^WithDropChoice^ WithDropChoice][@(0.0.255) `&
]_[* NoUpDownKeys]()&]
[s2;%% Same as UpDownKeys(false).&]
[s3;%% &]
[s4; &]
[s5;:WithDropChoice`:`:GetCount`(`)const: [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const
]&]
[s2;%% Returns the number of items in the drop down list.&]
[s3;%% &]
[s4; &]
[s5;:WithDropChoice`:`:Get`(int`)const: [_^Value^ Value]_[* Get]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) c
onst]&]
[s2;%% Returns the item with the index [%-*@3 i ]from the drop down 
list.&]
[s3;%% &]
[s4; &]
[s5;:WithDropChoice`:`:StyleDefault`(`): [@(0.0.255) const]_[_^MultiButton`:`:Style^ Mult
iButton`::Style][@(0.0.255) `&]_[* StyleDefault]()&]
[s2;%% Returns the default style of the drop down frame added to 
the widget.&]
[s3; &]
[s4; &]
[s5;:WithDropChoice`:`:SetStyle`(const MultiButton`:`:Style`&`): [_^WithDropChoice^ Wit
hDropChoice][@(0.0.255) `&]_[* SetStyle]([@(0.0.255) const]_[_^MultiButton`:`:Style^ Mult
iButton`::Style][@(0.0.255) `&]_[*@3 s])&]
[s2;%% Sets the frame style to [%-_^MultiButton`:`:Style^ MultiButton`::Style] 
[%-*@3 s].&]
[s3;%% &]
[s0; ]]