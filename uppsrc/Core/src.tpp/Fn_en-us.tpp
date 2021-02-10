topic "Small utility functions";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 Small utility functions]]}}&]
[s0;%% &]
[s4; &]
[s5;:Upp`:`:min`(const T`&`,const T`&`,`.`.`.`): [@(0.0.255) template]_<[@(0.0.255) class
]_[*@4 T]>_[@(0.0.255) constexpr const]_[*@4 T][@(0.0.255) `&]_[* min]([@(0.0.255) const]_[*@4 T
][@(0.0.255) `&]_[*@3 a], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 b], 
...)&]
[s2;%% Returns the least of all arguments.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:max`(const T`&`,const T`&`,`.`.`.`): [@(0.0.255) template]_<[@(0.0.255) class
]_[*@4 T]>_[@(0.0.255) constexpr const]_[*@4 T][@(0.0.255) `&]_[* max]([@(0.0.255) const]_[*@4 T
][@(0.0.255) `&]_[*@3 a], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 b], 
...)&]
[s2;%% Returns the greatest of all arguments.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:clamp`(const T`&`,const T`&`,const T`&`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[@(0.0.255) constexpr]_[*@4 T]_[* clamp]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 x], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 min`_], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 max`_])&]
[s2;%% Returns [%-*@3 x] if it is between [%-*@3 min`_] and [%-*@3 max`_] 
.&]
[s2; [%% If ][*@3 x][%%  is lower than ][*@3 min`_][%%  it returns ][*@3 min`_][%%  
][*@3 .]&]
[s2;%% If [%-*@3 x] is higher than [%-*@3 max`_] it returns [%-*@3 max`_] 
[%-*@3 .].&]
[s3;%% &]
[s4; &]
[s5;:findarg`(const T`&`,const T1`&`,`.`.`.`): [@(0.0.255) template]_<[@(0.0.255) class]_
[*@4 T], [@(0.0.255) class]_[*@4 T1], ...>_[@(0.0.255) int]_[* findarg]([@(0.0.255) const]_[*@4 T
][@(0.0.255) `&]_[*@3 x], [@(0.0.255) const]_[*@4 T1][@(0.0.255) `&]_[*@3 p0], 
...)&]
[s2;%% Searches the list of arguments for the value of [%-*@3 x]. If 
it is found, function returns the index of argument (starting 
with 0). If not found, `-1 is returned.&]
[s3;%% &]
[s4; &]
[s5;:decode`(const T`&`,const T1`&`,const V1`&`,`.`.`.`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T], [@(0.0.255) class]_[*@4 T1], [@(0.0.255) class]_[*@4 V1], 
..., [@(0.0.255) class]_[*@4 D]>&]
[s5; [@(0.0.255) constexpr ][*@4 D]_[* decode]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x], 
[@(0.0.255) const]_[*@4 T1][@(0.0.255) `&]_[*@3 p0], [@(0.0.255) const]_[*@4 V1][@(0.0.255) `&]_
[*@3 v0], ..., [@(0.0.255) const]_[*@4 D][@(0.0.255) `&]_[*@3 def])&]
[s2;%% Searches the list of argument pairs for the value of [%-*@3 x] 
to be equal to the first argument in the pair. If found, returns 
the second argument of pair. If no match is found, returns the 
last argument [%-*@3 def].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:get`_i`(int`,const T`&`,const T1`&`,`.`.`.`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T], [@(0.0.255) class]_[*@4 T1], ...> T_[* get`_i]([@(0.0.255) int]_[*@3 i], 
[@(0.0.255) const]_T[@(0.0.255) `&]_[*/@3 p][*@3 0], [@(0.0.255) const]_T1[@(0.0.255) `&]_[*/@3 p
][*@3 1], ...)&]
[s5;:Upp`:`:get`_i`(int`,const char`*`,const char`*`,`.`.`.`): [@(0.0.255) const]_[@(0.0.255) c
har]_`*[* get`_i]([@(0.0.255) int]_[*@3 i], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 p0], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 p1], ...)&]
[s2;%% Returns parameter [%-*/@3 p][%-*@3 i]. If [%-*@3 i] is negative, 
returns [%-*/@3 p][%-*@3 0], if it is greater then the number of 
parameters, returns the last parameter.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:foreach`_arg`(F`,const T`&`,const T1`&`,`.`.`.`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 F], [@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 T1], 
...> [@(0.0.255) void]_[* foreach`_arg](F_[*@3 fn], [@(0.0.255) const]_T[@(0.0.255) `&]_[*@3 p
0], [@(0.0.255) const]_T1[@(0.0.255) `&]_[*@3 p1], ...)&]
[s2;%% Performs [%-*@3 fn] for each of arguments (argument is passed 
as [%-*@3 fn] parameter). Designed to be applied to a parameter 
pack.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:iter`_set`(I`,Args`&`&`.`.`.args`): [@(0.0.255) template]_<[@(0.0.255) class]_
[*@4 I], [@(0.0.255) typename...]_[*@4 Args]>_[@(0.0.255) void]_[* iter`_set]([*@4 I]_[*@3 t], 
[*@4 Args][@(0.0.255) `&`&]_...args)&]
[s2;%% Writes arguments to a sequence pointed to by iterator [%-*@3 t]. 
Designed to be applied to a parameter pack.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:iter`_get`(I`,Args`&`.`.`.args`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 I
], [@(0.0.255) typename...]_[*@4 Args]>_[@(0.0.255) void]_[* iter`_get]([*@4 I]_[*@3 s], 
[*@4 Args][@(0.0.255) `&]_...args)&]
[s2;%% Reads arguments from a sequence pointed to by iterator [%-*@3 t]. 
Designed to be applied to a parameter pack.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:gather`(Args`&`&`.`.`.args`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 C],
 [@(0.0.255) typename...]_[*@4 Args]>_[*@4 C]_[* gather]([*@4 Args][@(0.0.255) `&`&]_...args)
&]
[s2;%% Converts arguments into container. Designed to be applied 
to a parameter pack.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:scatter`_n`(int`,const C`&`,Args`&`.`.`.args`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 C], [@(0.0.255) typename...]_[*@4 Args]>_[@(0.0.255) int]_[* scatter`_n]([@(0.0.255) i
nt]_[*@3 n], [@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 c], [*@4 Args][@(0.0.255) `&]_...ar
gs)&]
[s2;%% If [%-*@3 n] is less than sizeof...(args), returns zero, otherwise 
writes first elements of container to arguments and returns sizeof...(args).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:scatter`(const C`&`,Args`&`.`.`.args`): [@(0.0.255) template]_<[@(0.0.255) cl
ass]_[*@4 C], [@(0.0.255) typename...]_[*@4 Args]>_[@(0.0.255) int]_[* scatter]([@(0.0.255) c
onst]_[*@4 C][@(0.0.255) `&]_[*@3 c], [*@4 Args][@(0.0.255) `&]_...args)&]
[s2;%% Same as scatter`_n(c.size(), c, args...).&]
[s2;%% &]
[s0;%% ]]