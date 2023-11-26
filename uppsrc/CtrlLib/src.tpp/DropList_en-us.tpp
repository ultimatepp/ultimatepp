topic "DropList";
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
[ {{10000@3 [s0;%% [*@(229)4 DropList]]}}&]
[s3; &]
[s1;:DropList`:`:class: [@(0.0.255)3 class][3 _][*3 DropList][3 _:_][@(0.0.255)3 public][3 _][*@3;3 M
ultiButton][3 , ][@(0.0.255)3 public][3 _][*@3;3 Convert]&]
[s0;%% &]
[s2;%% 
@@image:337&143
(AzYAFwAAAOn/AAAAAHic7dfPCcIwGIfh7OcOjuDVGdzAKbx51YqVKlGxqBg8qRWxGMQNNPBBKAniVy3+ghjeQ5IW+tBD/+hc6X9V1Gh1AswR3gMb3xfK7a3dPXAyZ0KEdGlO5kyUcKKunJhCIYSzpPGJcLzRnF4KfcyzeVlhsr5weuMeViUcrXKqVm/aub8DFMbLs8lgKFr6O0DhMD1RluTMKaBwsDjaLIwqHgIKo3lWzPKcfaCwP9s7GZ6/CXwe9qY7TsB3SiQVJ6AwliknlDD8b5uywxcGGPzX42d6AOPWczE=)
&]
[s0;%% &]
[s9;%% This widget provides the selection of values from the drop`-down 
 list. List is organized in key`-value pairs; when item is selected, 
the key of items becomes the new DropList value (as used by SetData/GetData 
interface). The value of DropList does not necessarily have to 
be in the list as key, you can e.g. assign a value to DropList 
with empty list and fill the list later.&]
[s9;%% Both key and value are of generic Value type. You can associate 
specific Display and Convert with the list and that way use DropList 
to represent any type of value (colors, line width, fonts etc..).&]
[s9;%% DropList also implements the single`-direction Convert interface, 
where Format is defined as key`->value translation. When DisplayAll() 
mode is on, Format returns key values for keys missing in the 
key / value table; when DisplayAll() mode is off, such values 
are considered invalid and Format returns Value().&]
[s9;%% Note: DropBox is implementation helper class used to share 
some code between DropList and similar classes. DropBox is derived 
from Ctrl.&]
[s3;%% &]
[s0;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* DropList Public Members]]}}&]
[s3; &]
[s5;:DropList`:`:Style`:`:typedef: [@(0.0.255) typedef]_[_^MultiButton`:`:Style^ MultiBut
ton`::Style]_[* Style]&]
[s2;%% The chameleon style of DropList is in fact the Style of MultiButton 
base class.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:WhenDrop: [_^Callback^ Callback]_[* WhenDrop]&]
[s2;%% This callback is called [* before] the list is displayed. Client 
code can (re)create the list in the callback.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DropList`:`:Add`(const Upp`:`:Value`&`,const Upp`:`:Value`&`,bool`): [_^Upp`:`:DropList^ D
ropList][@(0.0.255) `&]_[* Add]([@(0.0.255) const]_[_^Upp`:`:Value^ Value][@(0.0.255) `&]_[*@3 k
ey], [@(0.0.255) const]_[_^Upp`:`:Value^ Value][@(0.0.255) `&]_[*@3 value], 
[@(0.0.255) bool]_[*@3 enable] `= [@(0.0.255) true])&]
[s2;%% Adds a new item to the list, [%-*@3 key] determines the value 
of item, [%-*@3 value] is displayed in the value area and in the 
list. If [%-*@3 enable] is false, DropList can be assigned this 
[%-*@3 key]and displays corresponding [%-*@3 value], but does not 
include it in drop list popup.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DropList`:`:Add`(std`:`:initializer`_list`<std`:`:pair`<Upp`:`:Value`,Upp`:`:Value`>`>`): [_^Upp`:`:DropList^ D
ropList][@(0.0.255) `&]_[* Add]([_^std`:`:initializer`_list^ std`::initializer`_list]<[_^std`:`:pair^ s
td`::pair]<[_^Upp`:`:Value^ Value], [_^Upp`:`:Value^ Value]>>_[*@3 init])&]
[s2;%% Adds [%-*@3 init] to the list.&]
[s3;%% &]
[s4; &]
[s5;:DropList`:`:Add`(const Value`&`): [_^DropList^ DropList][@(0.0.255) `&]_[* Add]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 value])&]
[s2;%% Same as Add(value, value).&]
[s3; &]
[s4; &]
[s5;:DropList`:`:Remove`(int`): [@(0.0.255) void]_[* Remove]([@(0.0.255) int]_[*@3 i])&]
[s2;%% Removes drop list item [%-*@3 i].&]
[s3;%% &]
[s4; &]
[s5;:DropList`:`:ClearList`(`): [@(0.0.255) void]_[* ClearList]()&]
[s2;%% Clears the list.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%% Clears the list and sets DropList to Null.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:AddSeparator`(`): [_^DropList^ DropList][@(0.0.255) `&]_[* AddSeparator]()
&]
[s2;%% Adds horizontal ruler separator item. This item cannot be 
selected.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:Drop`(`): [@(0.0.255) void]_[* Drop]()&]
[s2;%% Forces the list to `"drop`".&]
[s3;%% &]
[s4; &]
[s5;:DropList`:`:SetIndex`(int`): [@(0.0.255) void]_[* SetIndex]([@(0.0.255) int]_[*@3 i])&]
[s2;%% Sets the value of list item [%-*@3 i] to be the current value.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:GetIndex`(`)const: [@(0.0.255) int]_[* GetIndex]()_[@(0.0.255) const]&]
[s2;%% Returns the index of current value of DropList in the list. 
Note that the current value is not required to be in the list, 
in that case `-1 is returned.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:GoBegin`(`): [@(0.0.255) void]_[* GoBegin]()&]
[s2;%% If there are any items, selects the first one.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:GoEnd`(`): [@(0.0.255) void]_[* GoEnd]()&]
[s2;%% If there are any items, selects the last one.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:HasKey`(const Value`&`)const: [@(0.0.255) bool]_[* HasKey]([@(0.0.255) con
st]_[_^Value^ Value][@(0.0.255) `&]_[*@3 k])_[@(0.0.255) const]&]
[s2;%% Tests whether key [%-*@3 k] is in the list. The search is fast 
([^topic`:`/`/Core`/src`/Indext`$en`-us`#`:`:Index`:`:Index`(`)^ Index] 
based).&]
[s3; &]
[s4; &]
[s5;:DropList`:`:FindKey`(const Value`&`)const: [@(0.0.255) int]_[* FindKey]([@(0.0.255) co
nst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 k])_[@(0.0.255) const]&]
[s2;%% Returns the index of key [%-*@3 k] in the list; returns `-1 
if not found. The search is fast ([^topic`:`/`/Core`/src`/Indext`$en`-us`#`:`:Index`:`:Index`(`)^ I
ndex] based).&]
[s3; &]
[s4; &]
[s5;:DropList`:`:Find`(const Value`&`)const: [@(0.0.255) int]_[* Find]([@(0.0.255) const]_[_^Value^ V
alue][@(0.0.255) `&]_[*@3 k])_[@(0.0.255) const]&]
[s2;%% Same as FindKey(k).&]
[s3; &]
[s4; &]
[s5;:DropList`:`:FindValue`(const Value`&`)const: [@(0.0.255) int]_[* FindValue]([@(0.0.255) c
onst]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v])_[@(0.0.255) const]&]
[s2;%% Finds the index of item with value [%-*@3 v]. If there is none, 
returns `-1.&]
[s3;%% &]
[s4; &]
[s5;:DropList`:`:GetCount`(`)const: [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) const]&]
[s2;%% Returns the count of items in the list.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:Trim`(int`): [@(0.0.255) void]_[* Trim]([@(0.0.255) int]_[*@3 n])&]
[s2;%% Trims the count of items in the list to [%-*@3 n].&]
[s3; &]
[s4; &]
[s5;:DropList`:`:GetKey`(int`)const: [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[* Ge
tKey]([@(0.0.255) int]_[*@3 i])_[@(0.0.255) const]&]
[s2;%% Returns the key at index [%-*@3 i].&]
[s3; &]
[s4; &]
[s5;:DropList`:`:GetValue`(int`)const: [_^Value^ Value]_[* GetValue]([@(0.0.255) int]_[*@3 i])
_[@(0.0.255) const]&]
[s2;%% Returns the value at index [%-*@3 i].&]
[s3; &]
[s4; &]
[s5;:DropList`:`:GetValue`(`)const: [_^Value^ Value]_[* GetValue]()_[@(0.0.255) const]&]
[s2;%% Returns the first value from the list that has the key equal 
to the current value of DropList. (Note that the GetData/SetData 
interface of DropList uses the key).&]
[s3; &]
[s4; &]
[s5;:DropList`:`:SetValue`(int`,const Value`&`): [@(0.0.255) void]_[* SetValue]([@(0.0.255) i
nt]_[*@3 i], [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v])&]
[s2;%% Replaces the value at index [%-*@3 i].&]
[s3; &]
[s4; &]
[s5;:DropList`:`:SetValue`(const Value`&`): [@(0.0.255) void]_[* SetValue]([@(0.0.255) cons
t]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v])&]
[s2;%% Sets the first value in the list that has the key equal to 
the current value of DropList.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:operator`[`]`(int`)const: [_^Value^ Value]_[* operator`[`]]([@(0.0.255) in
t]_[*@3 i])_[@(0.0.255) const]&]
[s2;%% Same as GetValue(i).&]
[s3; &]
[s4; &]
[s5;:DropList`:`:Adjust`(`): [@(0.0.255) void]_[* Adjust]()&]
[s2;%% Checks whether the current DropList value is present as key 
in the list. If it is not and list is not empty, first list item 
is selected.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:Adjust`(const Value`&`): [@(0.0.255) void]_[* Adjust]([@(0.0.255) const]_[_^Value^ V
alue][@(0.0.255) `&]_[*@3 k])&]
[s2;%% Checks whether the current DropList value is present as key 
in the list. If it is not and list is not empty, DropList is 
assigned [%-*@3 k].&]
[s3; &]
[s4; &]
[s5;:DropList`:`:SetDropLines`(int`): [_^DropList^ DropList][@(0.0.255) `&]_[* SetDropLines
]([@(0.0.255) int]_[*@3 d])&]
[s2;%% Sets the number of lines in dropped list.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:SetValueConvert`(const Convert`&`): [_^DropList^ DropList][@(0.0.255) `&
]_[* SetValueConvert]([@(0.0.255) const]_[_^Convert^ Convert][@(0.0.255) `&]_[*@3 cv])&]
[s2;%% Sets the converting object for the value in view area.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:SetConvert`(const Convert`&`): [_^DropList^ DropList][@(0.0.255) `&]_[* Se
tConvert]([@(0.0.255) const]_[_^Convert^ Convert][@(0.0.255) `&]_[*@3 cv])&]
[s2;%% Sets the converting object for list values `- list values 
are converted first before being displayed in either view area 
of DropList or in list. Returns `*this.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:SetDisplay`(int`,const Display`&`): [_^DropList^ DropList][@(0.0.255) `&
]_[* SetDisplay]([@(0.0.255) int]_[*@3 i], [@(0.0.255) const]_[_^Display^ Display][@(0.0.255) `&
]_[*@3 d])&]
[s2;%% Sets the display for item [%-*@3 i] in the list. Returns `*this.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:SetDisplay`(const Display`&`): [_^DropList^ DropList][@(0.0.255) `&]_[* Se
tDisplay]([@(0.0.255) const]_[_^Display^ Display][@(0.0.255) `&]_[*@3 d])&]
[s2; [%% Sets the common display ][*@3 d][%%  for items in the list and 
view area. Returns `*this. If specific displays for list items 
(]SetDisplay[@(64) (][@(0.0.255) int][@(64) _][@3 i][@(64) , ][@(0.0.255) const][@(64) _][^`:`:Display^@(64) D
isplay][@(64) `&_][@3 d][@(64) )]) or view area (ValueDisplay[@(64) (][@(0.0.255) const][@(64) _
][^`:`:Display^@(64) Display][@(64) `&_][@3 d][@(64) )]) are defined, 
they take the precedence.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:SetLineCy`(int`): [_^DropList^ DropList][@(0.0.255) `&]_[* SetLineCy]([@(0.0.255) i
nt]_[*@3 lcy])&]
[s2;%% Sets the line height of the list item in pixels. Returns `*this.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:SetDisplay`(const Display`&`,int`): [_^DropList^ DropList][@(0.0.255) `&
]_[* SetDisplay]([@(0.0.255) const]_[_^Display^ Display][@(0.0.255) `&]_[*@3 d], 
[@(0.0.255) int]_[*@3 lcy])&]
[s2;%% Same as SetDisplay(d), SetLineCy(lcy). Returns `*this.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:ValueDisplay`(const Display`&`): [_^DropList^ DropList][@(0.0.255) `&]_[* V
alueDisplay]([@(0.0.255) const]_[_^Display^ Display][@(0.0.255) `&]_[*@3 d])&]
[s2;%% Sets the display object to be used in view area of DropList. 
Returns `*this.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:DisplayAll`(bool`): [_^DropList^ DropList][@(0.0.255) `&]_[* DisplayAll]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% In this mode, if the current value of DropList is not present 
as the key in the list, the value itself is used to be displayed 
in the view area. Default is off. Returns `*this. When DisplayAll() 
mode is on, Convert`::Format returns key value when a given key 
is missing in the key / value table; when DisplayAll() mode is 
off, Convert`::Format returns Value() for unknown key values.&]
[s0;%% &]
[s4; &]
[s5;:DropList`:`:DropFocus`(bool`): [_^DropList^ DropList][@(0.0.255) `&]_[* DropFocus]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% In this mode, dropping down the list and closing it (either 
by selecting the item or canceling the selection) brings the 
keyboard focus to the widget. Default is on. Returns `*this.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:NoDropFocus`(`): [_^DropList^ DropList][@(0.0.255) `&]_[* NoDropFocus]()&]
[s2;%% Same as DropFocus(false).&]
[s3; &]
[s4; &]
[s5;:DropList`:`:AlwaysDrop`(bool`): [_^DropList^ DropList][@(0.0.255) `&]_[* AlwaysDrop]([@(0.0.255) b
ool]_[*@3 e]_`=_[@(0.0.255) true])&]
[s2;%% If this mode is active, drop button is enabled even when the 
list is empty, allowing the client code to deffer the list creation 
until the WhenDrop callback.&]
[s3; &]
[s4; &]
[s5;:DropList`:`:SetStyle`(const DropList`:`:Style`&`): [_^DropList^ DropList][@(0.0.255) `&
]_[* SetStyle]([@(0.0.255) const]_[_^DropList`:`:Style^ Style][@(0.0.255) `&]_[*@3 s])&]
[s2;%% Sets the chameleon style of DropList.&]
[s3;%% &]
[s4; &]
[s5;:DropList`:`:NotNull`(bool`): [_^DropList^ DropList][@(0.0.255) `&]_[* NotNull]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% Specifies that Null value is illegal for DropList. If active, 
Null value is not [^Ctrl`:`:Accept^ accepted].&]
[s3;%% &]
[s4; &]
[s5;:DropList`:`:DropWidth`(int`): [_^DropList^ DropList][@(0.0.255) `&]_[* DropWidth]([@(0.0.255) i
nt]_[*@3 w])&]
[s2;%% Override the with of dropped PopUpTable drop`-down list (which 
by default is the same as the width of DropList).&]
[s3;%% &]
[s4; &]
[s5;:DropList`:`:DropWidthZ`(int`): [_^DropList^ DropList][@(0.0.255) `&]_[* DropWidthZ]([@(0.0.255) i
nt]_[*@3 w])&]
[s2;%% Same as DropWidth(HorzLayoutZoom([%-*@3 w])) `- this adjusts 
the width according to current standard GUI font zooming.&]
[s3;%% &]
[s4; &]
[s5;:DropList`:`:Wheel`(bool`): [_^DropList^ DropList][@(0.0.255) `&]_[* Wheel]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2;%% If active, the value of DropList can be changed by the mouse 
wheel. Default is active.&]
[s3;%% &]
[s4; &]
[s5;:DropList`:`:NoWheel`(`): [_^DropList^ DropList][@(0.0.255) `&]_[* NoWheel]()&]
[s2;%% Same as Wheel(false).&]
[s3; &]
[s4; &]
[s5;:DropList`:`:SetScrollBarStyle`(const ScrollBar`:`:Style`&`): [_^DropList^ DropList
][@(0.0.255) `&]_[* SetScrollBarStyle]([@(0.0.255) const]_[_^ScrollBar`:`:Style^ ScrollBa
r`::Style][@(0.0.255) `&]_[*@3 s])&]
[s2;%% Sets the chameleon style of Scrollbar of PopUpTable drop`-down 
list.&]
[s3;%% &]
[s4; &]
[s5;:DropList`:`:operator`=`(const Value`&`): [@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&
]_[* operator`=]([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 v])&]
[s5;:DropList`:`:operator Value`(`)const: [@(0.0.255) operator]_Value()_[@(0.0.255) const
]&]
[s2;%% These methods are deprecated in favor of operator`~ and operator<<`=.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* DropList Related Global Functions]]}}&]
[s9;%% These are simple utility function for filling DropList`'s 
list and MapConvert.&]
[s3;%% &]
[s3;%% &]
[s5;:Append`(DropList`&`,const VectorMap`<Value`,Value`>`&`): [@(0.0.255) void]_[* Append
]([_^DropList^ DropList][@(0.0.255) `&]_[*@3 list], [@(0.0.255) const]_[_^VectorMap^ Vector
Map]<[_^Value^ Value], [_^Value^ Value]>`&_[*@3 values])&]
[s5;:Append`(DropList`&`,const VectorMap`<int`,String`>`&`): [@(0.0.255) void]_[* Append](
[_^DropList^ DropList][@(0.0.255) `&]_[*@3 list], [@(0.0.255) const]_[_^VectorMap^ VectorMa
p]<[@(0.0.255) int], [_^String^ String]>`&_[*@3 values])&]
[s2;%% Fills the DropList from VectorMap.&]
[s3;%% &]
[s4; &]
[s5;:Append`(MapConvert`&`,const VectorMap`<Value`,Value`>`&`): [@(0.0.255) void]_[* Appe
nd]([_^MapConvert^ MapConvert][@(0.0.255) `&]_[*@3 convert], [@(0.0.255) const]_[_^VectorMap^ V
ectorMap]<[_^Value^ Value], [_^Value^ Value]>`&_[*@3 values])&]
[s5;:Append`(MapConvert`&`,const VectorMap`<int`,String`>`&`): [@(0.0.255) void]_[* Appen
d]([_^MapConvert^ MapConvert][@(0.0.255) `&]_[*@3 convert], [@(0.0.255) const]_[_^VectorMap^ V
ectorMap]<[@(0.0.255) int], [_^String^ String]>`&_[*@3 values])&]
[s2;%% Fills the MapConvert from VectorMap.&]
[s3;%% &]
[s4; &]
[s5;:Append`(DropList`&`,const MapConvert`&`): [@(0.0.255) void]_[* Append]([_^DropList^ Dr
opList][@(0.0.255) `&]_[*@3 list], [@(0.0.255) const]_[_^MapConvert^ MapConvert][@(0.0.255) `&
]_[*@3 convert])&]
[s2;%% Fills the DropList from MapConvert.&]
[s3;%% &]
[s4; &]
[s5;:operator`*`=`(DropList`&`,const VectorMap`<Value`,Value`>`&`): [@(0.0.255) void]_[* o
perator`*`=]([_^DropList^ DropList][@(0.0.255) `&]_[*@3 list], [@(0.0.255) const]_[_^VectorMap^ V
ectorMap]<[_^Value^ Value], [_^Value^ Value]>`&_[*@3 values])&]
[s5;:operator`*`=`(DropList`&`,const VectorMap`<int`,String`>`&`): [@(0.0.255) void]_[* o
perator`*`=]([_^DropList^ DropList][@(0.0.255) `&]_[*@3 list], [@(0.0.255) const]_[_^VectorMap^ V
ectorMap]<[@(0.0.255) int], [_^String^ String]>`&_[*@3 values])&]
[s2;%% Clears and fills the DropList from VectorMap.&]
[s3;%% &]
[s4; &]
[s5;:operator`*`=`(MapConvert`&`,const VectorMap`<Value`,Value`>`&`): [@(0.0.255) void]_
[* operator`*`=]([_^MapConvert^ MapConvert][@(0.0.255) `&]_[*@3 convert], 
[@(0.0.255) const]_[_^VectorMap^ VectorMap]<[_^Value^ Value], [_^Value^ Value]>`&_[*@3 valu
es])&]
[s5;:operator`*`=`(MapConvert`&`,const VectorMap`<int`,String`>`&`): [@(0.0.255) void]_
[* operator`*`=]([_^MapConvert^ MapConvert][@(0.0.255) `&]_[*@3 convert], 
[@(0.0.255) const]_[_^VectorMap^ VectorMap]<[@(0.0.255) int], [_^String^ String]>`&_[*@3 va
lues])&]
[s2;%% Clears and fills the MapConvert from VectorMap.&]
[s3;%% &]
[s4; &]
[s5;:operator`*`=`(DropList`&`,const MapConvert`&`): [@(0.0.255) void]_[* operator`*`=]([_^DropList^ D
ropList][@(0.0.255) `&]_[*@3 list], [@(0.0.255) const]_[_^MapConvert^ MapConvert][@(0.0.255) `&
]_[*@3 convert])&]
[s2;%% Clears and fills the DropList from MapConvert.&]
[s3;%% &]
[s0;%% ]]