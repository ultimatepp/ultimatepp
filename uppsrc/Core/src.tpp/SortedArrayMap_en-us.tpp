topic "SortedArrayMap";
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
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 SortedArrayMap]]}}&]
[s0;i448;a25;kKO9;@(0.0.255)%- &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 K][3 , 
][@(0.0.255)3 class][3 _][*@4;3 T][3 , ][@(0.0.255)3 class][3 _][*@4;3 Less][3 _`=_StdLess<][*@4;3 K
][3 >_>]&]
[s1;:SortedArrayMap`:`:class:%- [@(0.0.255) class]_[* SortedArrayMap]_:_[@(0.0.255) public]_
[*@3 MoveableAndDeepCopyOption]<[* SortedArrayMap]<[*@4 K], [*@4 T], 
[*@4 Less]>_>, [@(0.0.255) public]_[*@3 SortedAMap]<[*@4 K], [*@4 T], [*@4 Less], 
[_^Slaved`_InArray`_`_^ Slaved`_InArray`_`_]<[*@4 T]>_>_&]
[s2; Vector flavor of [^topic`:`/`/Core`/src`/SortedAMap`$en`-us^ SortedAMap]. 
It has default pick transfer semantics with optional deep`-copy. 
It is [^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ Moveable]. Keys 
have to be [^topic`:`/`/Core`/srcdoc`/Moveable`$en`-us^ Moveable] 
but there is no general restriction on values, they can even 
be derived from [%-*@4 T].&]
[s0;i448;a25;kKO9;@(0.0.255)%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:SortedArrayMap`:`:Add`(const K`&`,const T`&`):%- [*@4 T][@(0.0.255) `&]_[* Add]([@(0.0.255) c
onst]_[*@4 K][@(0.0.255) `&]_[*@3 k], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s5;:Upp`:`:SortedArrayMap`:`:Add`(const K`&`,T`&`&`):%- [*@4 T][@(0.0.255) `&]_[* Add]([@(0.0.255) c
onst]_[*@4 K][@(0.0.255) `&]_[*@3 k], [*@4 T][@(0.0.255) `&`&]_[*@3 x])&]
[s2; Inserts a copy of key`-value pair at upper bound key position 
and returns reference to element value.&]
[s3; &]
[s4; &]
[s5;:SortedArrayMap`:`:Add`(const K`&`):%- [*@4 T][@(0.0.255) `&]_[* Add]([@(0.0.255) const]_
[*@4 K][@(0.0.255) `&]_[*@3 k])&]
[s2; Inserts the key at upper bound key position and returns reference 
to the default constructed value at the same index.&]
[s3; &]
[s4; &]
[s5;:SortedArrayMap`:`:Add`(const K`&`,T`*`):%- [*@4 T][@(0.0.255) `&]_[* Add]([@(0.0.255) co
nst]_[*@4 K][@(0.0.255) `&]_[*@3 k], [*@4 T]_`*[*@3 newt])&]
[s2; Inserts the key at upper bound key position and sets key to 
[%-*@3 newt] `- SortedArrayMap takes ownership. Returns [%- `*][%-*@3 newt].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:SortedArrayMap`:`:Create`(const K`&`,Args`&`&`.`.`.args`):%- [@(0.0.255) te
mplate]_<[@(0.0.255) class]_[*@4 TT], [@(0.0.255) class...]_[*@4 Args]>_[*@4 TT][@(0.0.255) `&
]_[* Create]([@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[*@3 k], [*@4 Args][@(0.0.255) `&`&...]_
args)&]
[s2; Inserts the key at upper bound key position and creates a new 
value of type [%-*@4 TT] using args as constructor parameters. 
[%-*@4 TT] has to be derived from [%-*@4 T]. Returns a reference 
to this newly created value.&]
[s3; &]
[s4; &]
[s5;:SortedArrayMap`:`:FindAdd`(const K`&`):%- [@(0.0.255) int]_[* FindAdd]([@(0.0.255) con
st]_[*@4 K][@(0.0.255) `&]_[*@3 k])&]
[s2; If key [%-*@3 k] is present, returns index of its lower`-bound 
position. If not, inserts it at upper`-bound position and returns 
its index (value of element is default`-constructed).&]
[s3; &]
[s4; &]
[s5;:SortedArrayMap`:`:FindAdd`(const K`&`,const T`&`):%- [@(0.0.255) int]_[* FindAdd]([@(0.0.255) c
onst]_[*@4 K][@(0.0.255) `&]_[*@3 k], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 init])&]
[s2; If key [%-*@3 k] is present, returns index of its lower`-bound 
position. If not, inserts it at upper`-bound position with value 
of element copy`-constructed from [%-*@3 init] and returns its 
index.&]
[s3; &]
[s4; &]
[s5;:SortedArrayMap`:`:GetAdd`(const K`&`):%- [*@4 T][@(0.0.255) `&]_[* GetAdd]([@(0.0.255) c
onst]_[*@4 K][@(0.0.255) `&]_[*@3 k])&]
[s2; If key [%-*@3 k] is present, returns reference of value at its 
lower`-bound position. If not, inserts it at upper`-bound position 
and returns reference to coresponding value (value of element 
is default`-constructed).&]
[s3; &]
[s4; &]
[s5;:SortedArrayMap`:`:GetAdd`(const K`&`,const T`&`):%- [*@4 T][@(0.0.255) `&]_[* GetAdd](
[@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[*@3 k], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x
])&]
[s2; If key [%-*@3 k] is present, returns reference of value at its 
lower`-bound position. If not, inserts it at upper`-bound position 
and returns reference to coresponding value, which is copy`-constructed 
from [%-*@3 x].&]
[s3; &]
[s4; &]
[s5;:SortedArrayMap`:`:Detach`(int`):%- [*@4 T]_`*[* Detach]([@(0.0.255) int]_[*@3 i])&]
[s2; Removes element at [%-*@3 i] and returns the pointer to value. 
This pointer should be deleted by client code.&]
[s3; &]
[s4;%- &]
[s5;:SortedArrayMap`:`:operator`(`)`(const K`&`,const T`&`):%- [_^SortedArrayMap^ Sorte
dArrayMap][@(0.0.255) `&]_[* operator()]([@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[*@3 k], 
[@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 v])&]
[s2; Same as Add([%-*@3 k], [%-*@3 v]), returns `*this. Syntax sugar 
for creating map.&]
[s3; &]
[s4; &]
[s5;:SortedArrayMap`:`:SortedArrayMap`(const SortedArrayMap`&`,int`):%- [* SortedArrayM
ap]([@(0.0.255) const]_[* SortedArrayMap][@(0.0.255) `&]_[*@3 s], [@(0.0.255) int])&]
[s2; Deep copy constructor.&]
[s4;%- &]
[s5;:Upp`:`:SortedArrayMap`:`:SortedArrayMap`(std`:`:initializer`_list`<std`:`:pair`<K`,T`>`>`):%- [* S
ortedArrayMap]([_^http`:`/`/en`.cppreference`.com`/w`/cpp`/utility`/initializer`_list^ s
td`::initializer`_list]<[_^http`:`/`/en`.cppreference`.com`/w`/cpp`/utility`/pair^ s
td`::pair]<[*@4 K], [*@4 T]>>_[*@3 init])&]
[s2; C`+`+ 11 initialization.&]
[s3; &]
[s0; ]]