topic "sdfgsdfg sdfjkl ghlskdjfh glskdhj fglksjd hfglkjshdfglkjhds fgkl dslk skldjfh glksdjh glksdj glskdj kldsjfh glksdjh gjklds";
[ $$0,0#00000000000000000000000000000000:Default]
[H6;0 $$1,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$2,0#37138531426314131252341829483370:codeitem]
[l288;2 $$3,0#27521748481378242620020725143825:desc]
[0 $$4,0#96390100711032703541132217272105:end]
[{_}%EN-US 
[s1;%- &]
[s0;%- &]
[s0;%- &]
[s2;:FormatInt`(int`):%- [_^String^ String]_[* FormatInt]([@(0.0.255) int]_[*@3 i])&]
[s3; Outputs decimally formatted signed integer without `+ or any 
paddings. Supposed to be quite fast. Equivalent to FormatIntBase(i, 
10).&]
[s3; &]
[s0;l288;O9;~~~.640; [* i]-|the number to format.&]
[s4;%- &]
[s1;%- &]
[s2;:FormatInt`(int`):%- [_^String^ String]_[* FormatInt]([@(0.0.255) int]_[*@3 i])&]
[s3; Outputs decimally formatted signed integer without `+ or any 
paddings. Supposed to be quite fast. Equivalent to FormatIntBase(i, 
10).&]
[s3; &]
[s0;l288;O9;~~~.640; [* i]-|the number to format.&]
[s1;%- &]
[s2;:sgn`(T`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T][@(0.0.255) >]_[@(0.0.255) in
t]_[* sgn]([*@4 T]_[*@3 a])&]
[s3;  [%-*@3 a].&]
[s4; &]
[s1; &]
[s2;:tabs`(T`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T][@(0.0.255) >]_[*@4 T]_[* tabs
]([*@4 T]_[*@3 a])&]
[s3;  [%-*@3 a].&]
[s4; &]
[s1; &]
[s2;:cmp`(const T`&`,const T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T][@(0.0.255) >
]_[@(0.0.255) int]_[* cmp]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 a], 
[@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 b])&]
[s3;  [%-*@3 a] [%-*@3 b].&]
[s4; &]
[s1; &]
[s2;:Reverse`(I`,I`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 I][@(0.0.255) >]_[@(0.0.255) v
oid]_[* Reverse]([*@4 I]_[*@3 start], [*@4 I]_[*@3 end])&]
[s3;  [%-*@3 start] [%-*@3 end].&]
[s4; &]
[s1; &]
[s2;:Reverse`(C`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 C][@(0.0.255) >]_[@(0.0.255) v
oid]_[* Reverse]([*@4 C][@(0.0.255) `&]_[*@3 container])&]
[s3;  [%-*@3 container].&]
[s4; &]
[s1; &]
[s2;:Sum`(V`&`,T`,T`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T], 
[@(0.0.255) class]_[*@4 V][@(0.0.255) >]_[@(0.0.255) void]_[* Sum]([*@4 V][@(0.0.255) `&]_[*@3 su
m], [*@4 T]_[*@3 ptr], [*@4 T]_[*@3 end])&]
[s3;  [%-*@3 sum] [%-*@3 ptr] [%-*@3 end].&]
[s4; &]
[s1; &]
[s2;:Sum`(const T`&`,const typename ValueType`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T][@(0.0.255) >]_[@(0.0.255) typename]_T`::ValueType_[* Sum]([@(0.0.255) const]_[*@4 T
][@(0.0.255) `&]_[*@3 c], [@(0.0.255) const]_[@(0.0.255) typename]_T`::ValueType[@(0.0.255) `&
]_[*@3 init]_`=_[@(0.0.255) typename]_T`::ValueType())&]
[s3;  [%-*@3 c] [%-*@3 init].&]
[s4; &]
[s1; &]
[s2;:Sum0`(const T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T][@(0.0.255) >]_[@(0.0.255) t
ypename]_T`::ValueType_[* Sum0]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 c])&]
[s3;  [%-*@3 c].&]
[s4; &]
[s1; &]
[s2;:MinElement`(T`,T`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T][@(0.0.255) >]_[*@4 T
]_[* MinElement]([*@4 T]_[*@3 ptr], [*@4 T]_[*@3 end])&]
[s3;  [%-*@3 ptr] [%-*@3 end].&]
[s4; &]
[s1; &]
[s2;:MinIndex`(const C`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 C][@(0.0.255) >]_
[@(0.0.255) int]_[* MinIndex]([@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 c])&]
[s3;  [%-*@3 c].&]
[s4; &]
[s1; &]
[s2;:MaxIndex`(const C`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 C][@(0.0.255) >]_
[@(0.0.255) int]_[* MaxIndex]([@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 c])&]
[s3;  [%-*@3 c].&]
[s4; &]
[s1; &]
[s2;:Min`(const T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T][@(0.0.255) >]_[@(0.0.255) c
onst]_[@(0.0.255) typename]_T`::ValueType[@(0.0.255) `&]_[* Min]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 c])&]
[s3;  [%-*@3 c].&]
[s4; &]
[s1; &]
[s2;:MaxElement`(T`,T`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T][@(0.0.255) >]_[*@4 T
]_[* MaxElement]([*@4 T]_[*@3 ptr], [*@4 T]_[*@3 end])&]
[s3;  [%-*@3 ptr] [%-*@3 end].&]
[s4; &]
[s1; &]
[s2;:Max`(const T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T][@(0.0.255) >]_[@(0.0.255) c
onst]_[@(0.0.255) typename]_T`::ValueType[@(0.0.255) `&]_[* Max]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 c])&]
[s3;  [%-*@3 c].&]
[s4; &]
[s1; &]
[s2;:IsEqual`(T`,T`,T`,T`,const C`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T],
 [@(0.0.255) class]_[*@4 C][@(0.0.255) >]_[@(0.0.255) bool]_[* IsEqual]([*@4 T]_[*@3 ptr1], 
[*@4 T]_[*@3 end1], [*@4 T]_[*@3 ptr2], [*@4 T]_[*@3 end2], [@(0.0.255) const]_[*@4 C][@(0.0.255) `&
]_[*@3 equal])&]
[s3;  [%-*@3 ptr1] [%-*@3 end1] [%-*@3 ptr2] [%-*@3 end2] [%-*@3 equal].&]
[s4; &]
[s1; &]
[s2;:IsEqual`(const T`&`,const T`&`,const C`&`):%- [@(0.0.255) template]_<[@(0.0.255) cla
ss]_[*@4 T], [@(0.0.255) class]_[*@4 C][@(0.0.255) >]_[@(0.0.255) bool]_[* IsEqual]([@(0.0.255) c
onst]_[*@4 T][@(0.0.255) `&]_[*@3 c1], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 c2], 
[@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 equal])&]
[s3;  [%-*@3 c1] [%-*@3 c2] [%-*@3 equal].&]
[s4; &]
[s1; &]
[s2;:IsEqual`(const T`&`,const T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T][@(0.0.255) >
]_[@(0.0.255) bool]_[* IsEqual]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 c1], 
[@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 c2])&]
[s3;  [%-*@3 c1] [%-*@3 c2].&]
[s4; &]
[s1; &]
[s2;:Find`(T`,T`,const V`&`,const C`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T
], [@(0.0.255) class]_[*@4 V], [@(0.0.255) class]_[*@4 C][@(0.0.255) >]_[*@4 T]_[* Find]([*@4 T]_
[*@3 ptr], [*@4 T]_[*@3 end], [@(0.0.255) const]_[*@4 V][@(0.0.255) `&]_[*@3 value], 
[@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 equal])&]
[s3;  [%-*@3 ptr] [%-*@3 end] [%-*@3 value] [%-*@3 equal].&]
[s4; &]
[s1; &]
[s2;:Find`(T`,T`,const V`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T], 
[@(0.0.255) class]_[*@4 V][@(0.0.255) >]_[*@4 T]_[* Find]([*@4 T]_[*@3 ptr], 
[*@4 T]_[*@3 end], [@(0.0.255) const]_[*@4 V][@(0.0.255) `&]_[*@3 value])&]
[s3;  [%-*@3 ptr] [%-*@3 end] [%-*@3 value].&]
[s4; &]
[s1; &]
[s2;:FindIndex`(const T`&`,const V`&`,const C`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T], [@(0.0.255) class]_[*@4 V], [@(0.0.255) class]_[*@4 C][@(0.0.255) >]_[@(0.0.255) i
nt]_[* FindIndex]([@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 cont], 
[@(0.0.255) const]_[*@4 V][@(0.0.255) `&]_[*@3 value], [@(0.0.255) const]_[*@4 C][@(0.0.255) `&
]_[*@3 equal])&]
[s3;  [%-*@3 cont] [%-*@3 value] [%-*@3 equal].&]
[s4; &]
[s1; &]
[s2;:FindIndex`(const T`&`,const V`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T],
 [@(0.0.255) class]_[*@4 V][@(0.0.255) >]_[@(0.0.255) int]_[* FindIndex]([@(0.0.255) const]_[*@4 T
][@(0.0.255) `&]_[*@3 cont], [@(0.0.255) const]_[*@4 V][@(0.0.255) `&]_[*@3 value])&]
[s3;  [%-*@3 cont] [%-*@3 value].&]
[s4; &]
[s1; &]
[s2;:BinFindIndex`(I`,I`,const K`&`,const L`&`):%- [@(0.0.255) template]_<[@(0.0.255) cla
ss]_[*@4 I], [@(0.0.255) class]_[*@4 K], [@(0.0.255) class]_[*@4 L][@(0.0.255) >]_[@(0.0.255) i
nt]_[* BinFindIndex]([*@4 I]_[*@3 begin], [*@4 I]_[*@3 end], [@(0.0.255) const]_[*@4 K][@(0.0.255) `&
]_[*@3 key], [@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 less])&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 key] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:BinFindIndex`(const C`&`,const K`&`,const L`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 C], [@(0.0.255) class]_[*@4 K], [@(0.0.255) class]_[*@4 L][@(0.0.255) >]_[@(0.0.255) i
nt]_[* BinFindIndex]([@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 container], 
[@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[*@3 key], [@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_
[*@3 less])&]
[s3;  [%-*@3 container] [%-*@3 key] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:BinFindIndex`(const C`&`,const K`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 C
], [@(0.0.255) class]_[*@4 K][@(0.0.255) >]_[@(0.0.255) int]_[* BinFindIndex]([@(0.0.255) con
st]_[*@4 C][@(0.0.255) `&]_[*@3 container], [@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[*@3 key])
&]
[s3;  [%-*@3 container] [%-*@3 key].&]
[s4; &]
[s1; &]
[s2;:BinFind`(I`,I`,const K`&`,const L`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 I
], [@(0.0.255) class]_[*@4 K], [@(0.0.255) class]_[*@4 L][@(0.0.255) >]_[*@4 I]_[* BinFind]([*@4 I
]_[*@3 begin], [*@4 I]_[*@3 end], [@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[*@3 key], 
[@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 less])&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 key] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:BinFind`(const C`&`,const K`&`,const L`&`):%- [@(0.0.255) template]_<[@(0.0.255) cla
ss]_[*@4 C], [@(0.0.255) class]_[*@4 K], [@(0.0.255) class]_[*@4 L][@(0.0.255) >]_[@(0.0.255) t
ypename]_C`::ConstIterator_[* BinFind]([@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 contai
ner], [@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[*@3 key], [@(0.0.255) const]_[*@4 L][@(0.0.255) `&
]_[*@3 less])&]
[s3;  [%-*@3 container] [%-*@3 key] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:BinFind`(const C`&`,const K`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 C], 
[@(0.0.255) class]_[*@4 K][@(0.0.255) >]_[@(0.0.255) typename]_C`::ConstIterator_[* BinFind](
[@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 container], [@(0.0.255) const]_[*@4 K][@(0.0.255) `&
]_[*@3 key])&]
[s3;  [%-*@3 container] [%-*@3 key].&]
[s4; &]
[s1; &]
[s2;:IterCompare`(I`,I`,I`,I`,const C`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 I
], [@(0.0.255) class]_[*@4 C][@(0.0.255) >]_[@(0.0.255) int]_[* IterCompare]([*@4 I]_[*@3 a], 
[*@4 I]_[*@3 a`_end], [*@4 I]_[*@3 b], [*@4 I]_[*@3 b`_end], [@(0.0.255) const]_[*@4 C][@(0.0.255) `&
]_[*@3 compare])&]
[s3;  [%-*@3 a] [%-*@3 a`_end] [%-*@3 b] [%-*@3 b`_end] [%-*@3 compare].&]
[s4; &]
[s1; &]
[s2;:FindLowerBound`(const C`&`,int`,int`,const T`&`,const L`&`):%- [@(0.0.255) templat
e]_<[@(0.0.255) class]_[*@4 C], [@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 L][@(0.0.255) >
]_[@(0.0.255) int]_[* FindLowerBound]([@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 v], 
[@(0.0.255) int]_[*@3 pos], [@(0.0.255) int]_[*@3 count], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 val], [@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 less])&]
[s3;  [%-*@3 v] [%-*@3 pos] [%-*@3 count] [%-*@3 val] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:FindLowerBoundIter`(I`,I`,const T`&`,const L`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 I], [@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 L][@(0.0.255) >]_[*@4 I]_[* Fi
ndLowerBoundIter]([*@4 I]_[*@3 begin], [*@4 I]_[*@3 end], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 val], [@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 less])&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 val] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:FindLowerBoundIter`(I`,I`,const T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 I
], [@(0.0.255) class]_[*@4 T][@(0.0.255) >]_[*@4 I]_[* FindLowerBoundIter]([*@4 I]_[*@3 begin],
 [*@4 I]_[*@3 end], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 val])&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 val].&]
[s4; &]
[s1; &]
[s2;:FindLowerBound`(const C`&`,const T`&`,const L`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 C], [@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 L][@(0.0.255) >]_[@(0.0.255) i
nt]_[* FindLowerBound]([@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 v], 
[@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 val], [@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_
[*@3 less])&]
[s3;  [%-*@3 v] [%-*@3 val] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:FindLowerBound`(const C`&`,const T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_
[*@4 C], [@(0.0.255) class]_[*@4 T][@(0.0.255) >]_[@(0.0.255) int]_[* FindLowerBound]([@(0.0.255) c
onst]_[*@4 C][@(0.0.255) `&]_[*@3 v], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 val])&]
[s3;  [%-*@3 v] [%-*@3 val].&]
[s4; &]
[s1; &]
[s2;:FindUpperBound`(const C`&`,int`,int`,const T`&`,const L`&`):%- [@(0.0.255) templat
e]_<[@(0.0.255) class]_[*@4 C], [@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 L][@(0.0.255) >
]_[@(0.0.255) int]_[* FindUpperBound]([@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 v], 
[@(0.0.255) int]_[*@3 pos], [@(0.0.255) int]_[*@3 count], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 val], [@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 less])&]
[s3;  [%-*@3 v] [%-*@3 pos] [%-*@3 count] [%-*@3 val] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:FindUpperBoundIter`(I`,I`,const T`&`,const L`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 I], [@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 L][@(0.0.255) >]_[*@4 I]_[* Fi
ndUpperBoundIter]([*@4 I]_[*@3 begin], [*@4 I]_[*@3 end], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&
]_[*@3 val], [@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 less])&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 val] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:FindUpperBoundIter`(I`,I`,const T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 I
], [@(0.0.255) class]_[*@4 T][@(0.0.255) >]_[*@4 I]_[* FindUpperBoundIter]([*@4 I]_[*@3 begin],
 [*@4 I]_[*@3 end], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 val])&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 val].&]
[s4; &]
[s1; &]
[s2;:FindUpperBound`(const C`&`,const T`&`,const L`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 C], [@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 L][@(0.0.255) >]_[@(0.0.255) i
nt]_[* FindUpperBound]([@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 v], 
[@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 val], [@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_
[*@3 less])&]
[s3;  [%-*@3 v] [%-*@3 val] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:FindUpperBound`(const C`&`,const T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_
[*@4 C], [@(0.0.255) class]_[*@4 T][@(0.0.255) >]_[@(0.0.255) int]_[* FindUpperBound]([@(0.0.255) c
onst]_[*@4 C][@(0.0.255) `&]_[*@3 v], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 val])&]
[s3;  [%-*@3 v] [%-*@3 val].&]
[s4; &]
[s1; &]
[s2;:FindBinary`(const C`&`,const T`&`,int`,int`,const L`&`):%- [@(0.0.255) template]_<
[@(0.0.255) class]_[*@4 C], [@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 L][@(0.0.255) >
]_[@(0.0.255) int]_[* FindBinary]([@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 v], 
[@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 val], [@(0.0.255) int]_[*@3 pos], 
[@(0.0.255) int]_[*@3 count], [@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 less])&]
[s3;  [%-*@3 v] [%-*@3 val] [%-*@3 pos] [%-*@3 count] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:FindBinaryIter`(I`,I`,const T`&`,const L`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 I], [@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 L][@(0.0.255) >]_[*@4 I]_[* Fi
ndBinaryIter]([*@4 I]_[*@3 begin], [*@4 I]_[*@3 end], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_
[*@3 val], [@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 less])&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 val] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:FindBinaryIter`(I`,I`,const T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 I],
 [@(0.0.255) class]_[*@4 T][@(0.0.255) >]_[*@4 I]_[* FindBinaryIter]([*@4 I]_[*@3 begin], 
[*@4 I]_[*@3 end], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 val])&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 val].&]
[s4; &]
[s1; &]
[s2;:FindBinary`(const C`&`,const T`&`,const L`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 C], [@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 L][@(0.0.255) >]_[@(0.0.255) i
nt]_[* FindBinary]([@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 v], [@(0.0.255) const]_[*@4 T
][@(0.0.255) `&]_[*@3 val], [@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 less])&]
[s3;  [%-*@3 v] [%-*@3 val] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:FindBinary`(const C`&`,const T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 C
], [@(0.0.255) class]_[*@4 T][@(0.0.255) >]_[@(0.0.255) int]_[* FindBinary]([@(0.0.255) const
]_[*@4 C][@(0.0.255) `&]_[*@3 v], [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 val])&]
[s3;  [%-*@3 v] [%-*@3 val].&]
[s4; &]
[s1; &]
[s2;:BinFindCompIndex`(I`,I`,const K`&`,const X`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 I], [@(0.0.255) class]_[*@4 K], [@(0.0.255) class]_[*@4 X][@(0.0.255) >]_[@(0.0.255) i
nt]_[* BinFindCompIndex]([*@4 I]_[*@3 begin], [*@4 I]_[*@3 end], [@(0.0.255) const]_[*@4 K][@(0.0.255) `&
]_[*@3 key], [@(0.0.255) const]_[*@4 X][@(0.0.255) `&]_[*@3 comp])&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 key] [%-*@3 comp].&]
[s4; &]
[s1; &]
[s2;:BinFindCompIndex`(const C`&`,const K`&`,const X`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 C], [@(0.0.255) class]_[*@4 K], [@(0.0.255) class]_[*@4 X][@(0.0.255) >]_[@(0.0.255) i
nt]_[* BinFindCompIndex]([@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 container], 
[@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[*@3 key], [@(0.0.255) const]_[*@4 X][@(0.0.255) `&]_
[*@3 comp])&]
[s3;  [%-*@3 container] [%-*@3 key] [%-*@3 comp].&]
[s4; &]
[s1; &]
[s2;:BinFindComp`(I`,I`,const K`&`,const X`&`):%- [@(0.0.255) template]_<[@(0.0.255) clas
s]_[*@4 I], [@(0.0.255) class]_[*@4 K], [@(0.0.255) class]_[*@4 X][@(0.0.255) >]_[*@4 I]_[* BinFi
ndComp]([*@4 I]_[*@3 begin], [*@4 I]_[*@3 end], [@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[*@3 ke
y], [@(0.0.255) const]_[*@4 X][@(0.0.255) `&]_[*@3 comp])&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 key] [%-*@3 comp].&]
[s4; &]
[s1; &]
[s2;:BinFindComp`(const C`&`,const K`&`,const X`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 C], [@(0.0.255) class]_[*@4 K], [@(0.0.255) class]_[*@4 X][@(0.0.255) >]_[@(0.0.255) t
ypename]_C`::ConstIterator_[* BinFindComp]([@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 co
ntainer], [@(0.0.255) const]_[*@4 K][@(0.0.255) `&]_[*@3 key], [@(0.0.255) const]_[*@4 X][@(0.0.255) `&
]_[*@3 comp])&]
[s3;  [%-*@3 container] [%-*@3 key] [%-*@3 comp].&]
[s4; &]
[s1; &]
[s2;:Append`(T`&`,V`,V`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T], 
[@(0.0.255) class]_[*@4 V][@(0.0.255) >]_[@(0.0.255) void]_[* Append]([*@4 T][@(0.0.255) `&]_[*@3 d
st], [*@4 V]_[*@3 ptr], [*@4 V]_[*@3 end])&]
[s3;  [%-*@3 dst] [%-*@3 ptr] [%-*@3 end].&]
[s4; &]
[s1; &]
[s2;:Append`(T`&`,V`,int`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T], 
[@(0.0.255) class]_[*@4 V][@(0.0.255) >]_[@(0.0.255) void]_[* Append]([*@4 T][@(0.0.255) `&]_[*@3 d
st], [*@4 V]_[*@3 ptr], [@(0.0.255) int]_[*@3 n])&]
[s3;  [%-*@3 dst] [%-*@3 ptr] [%-*@3 n].&]
[s4; &]
[s1; &]
[s2;:Append`(T`&`,const V`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T], 
[@(0.0.255) class]_[*@4 V][@(0.0.255) >]_[@(0.0.255) void]_[* Append]([*@4 T][@(0.0.255) `&]_[*@3 d
st], [@(0.0.255) const]_[*@4 V][@(0.0.255) `&]_[*@3 src])&]
[s3;  [%-*@3 dst] [%-*@3 src].&]
[s4; &]
[s1; &]
[s2;:FindAppend`(C`&`,I`,I`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 C], 
[@(0.0.255) class]_[*@4 I][@(0.0.255) >]_[*@4 C][@(0.0.255) `&]_[* FindAppend]([*@4 C][@(0.0.255) `&
]_[*@3 dest], [*@4 I]_[*@3 begin], [*@4 I]_[*@3 end])&]
[s3;  [%-*@3 dest] [%-*@3 begin] [%-*@3 end].&]
[s4; &]
[s1; &]
[s2;:FindAppend`(C`&`,const S`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 C], 
[@(0.0.255) class]_[*@4 S][@(0.0.255) >]_[*@4 C][@(0.0.255) `&]_[* FindAppend]([*@4 C][@(0.0.255) `&
]_[*@3 dest], [@(0.0.255) const]_[*@4 S][@(0.0.255) `&]_[*@3 source])&]
[s3;  [%-*@3 dest] [%-*@3 source].&]
[s4; &]
[s1; &]
[s2;:AppendSorted`(C`&`,const C`&`,const L`&`):%- [@(0.0.255) template]_<[@(0.0.255) clas
s]_[*@4 C], [@(0.0.255) class]_[*@4 L][@(0.0.255) >]_[*@4 C][@(0.0.255) `&]_[* AppendSorted]([*@4 C
][@(0.0.255) `&]_[*@3 dest], [@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 src], 
[@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 less])&]
[s3;  [%-*@3 dest] [%-*@3 src] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:AppendSorted`(C`&`,const C`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 C][@(0.0.255) >
]_[*@4 C][@(0.0.255) `&]_[* AppendSorted]([*@4 C][@(0.0.255) `&]_[*@3 dest], 
[@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 src])&]
[s3;  [%-*@3 dest] [%-*@3 src].&]
[s4; &]
[s1; &]
[s2;:UnionSorted`(C`&`,const C`&`,const L`&`):%- [@(0.0.255) template]_<[@(0.0.255) class
]_[*@4 C], [@(0.0.255) class]_[*@4 L][@(0.0.255) >]_[*@4 C][@(0.0.255) `&]_[* UnionSorted]([*@4 C
][@(0.0.255) `&]_[*@3 dest], [@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 src], 
[@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 less])&]
[s3;  [%-*@3 dest] [%-*@3 src] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:UnionSorted`(C`&`,const C`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 C][@(0.0.255) >
]_[*@4 C][@(0.0.255) `&]_[* UnionSorted]([*@4 C][@(0.0.255) `&]_[*@3 dest], 
[@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 src])&]
[s3;  [%-*@3 dest] [%-*@3 src].&]
[s4; &]
[s1; &]
[s2;:RemoveSorted`(C`&`,const C`&`,const L`&`):%- [@(0.0.255) template]_<[@(0.0.255) clas
s]_[*@4 C], [@(0.0.255) class]_[*@4 L][@(0.0.255) >]_[*@4 C][@(0.0.255) `&]_[* RemoveSorted]([*@4 C
][@(0.0.255) `&]_[*@3 from], [@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 what], 
[@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 less])&]
[s3;  [%-*@3 from] [%-*@3 what] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:RemoveSorted`(C`&`,const C`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 C][@(0.0.255) >
]_[*@4 C][@(0.0.255) `&]_[* RemoveSorted]([*@4 C][@(0.0.255) `&]_[*@3 from], 
[@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 what])&]
[s3;  [%-*@3 from] [%-*@3 what].&]
[s4; &]
[s1; &]
[s2;:IntersectSorted`(D`&`,const S`&`,const L`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 D], [@(0.0.255) class]_[*@4 S], [@(0.0.255) class]_[*@4 L][@(0.0.255) >]_[*@4 D][@(0.0.255) `&
]_[* IntersectSorted]([*@4 D][@(0.0.255) `&]_[*@3 dest], [@(0.0.255) const]_[*@4 S][@(0.0.255) `&
]_[*@3 src], [@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 less])&]
[s3;  [%-*@3 dest] [%-*@3 src] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:IntersectSorted`(D`&`,const S`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 D],
 [@(0.0.255) class]_[*@4 S][@(0.0.255) >]_[*@4 D][@(0.0.255) `&]_[* IntersectSorted]([*@4 D][@(0.0.255) `&
]_[*@3 dest], [@(0.0.255) const]_[*@4 S][@(0.0.255) `&]_[*@3 src])&]
[s3;  [%-*@3 dest] [%-*@3 src].&]
[s4; &]
[s1; &]
[s2;:StreamContainer`(Stream`&`,T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T][@(0.0.255) >
]_[@(0.0.255) void]_[* StreamContainer](Stream[@(0.0.255) `&]_[*@3 s], 
[*@4 T][@(0.0.255) `&]_[*@3 cont])&]
[s3;  [%-*@3 s] [%-*@3 cont].&]
[s4; &]
[s1; &]
[s2;:ForwardSort`(I`,I`,const Less`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 I],
 [@(0.0.255) class]_[*@4 Less][@(0.0.255) >]_[@(0.0.255) void]_[* ForwardSort]([*@4 I]_[*@3 beg
in], [*@4 I]_[*@3 end], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:ForwardSort`(T`&`,const Less`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T],
 [@(0.0.255) class]_[*@4 Less][@(0.0.255) >]_[@(0.0.255) void]_[* ForwardSort]([*@4 T][@(0.0.255) `&
]_[*@3 c], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s3;  [%-*@3 c] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:ForwardSort`(T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T][@(0.0.255) >]_[@(0.0.255) v
oid]_[* ForwardSort]([*@4 T][@(0.0.255) `&]_[*@3 c])&]
[s3;  [%-*@3 c].&]
[s4; &]
[s1; &]
[s2;:`_`_SORT`_THRESHOLD:%- [@(0.0.255) enum]_[* `_`_SORT`_THRESHOLD]_`=_[@3 16]&]
[s3; &]
[s4; &]
[s1; &]
[s2;:`_`_SORT`_MEDIAN`_PASSES:%- [@(0.0.255) enum]_[* `_`_SORT`_MEDIAN`_PASSES]_`=_[@3 2]&]
[s3; &]
[s4; &]
[s1; &]
[s2;:Sort`(I`,I`,const Less`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 I], 
[@(0.0.255) class]_[*@4 Less][@(0.0.255) >]_[@(0.0.255) void]_[* Sort]([*@4 I]_[*@3 begin], 
[*@4 I]_[*@3 end], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:Sort`(T`&`,const Less`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T], 
[@(0.0.255) class]_[*@4 Less][@(0.0.255) >]_[@(0.0.255) void]_[* Sort]([*@4 T][@(0.0.255) `&]_[*@3 c
], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s3;  [%-*@3 c] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:Sort`(T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T][@(0.0.255) >]_[@(0.0.255) v
oid]_[* Sort]([*@4 T][@(0.0.255) `&]_[*@3 c])&]
[s3;  [%-*@3 c].&]
[s4; &]
[s1; &]
[s2;:StableSort`_`(I`,I`,const Less`&`,const T`*`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 I], [@(0.0.255) class]_[*@4 Less], [@(0.0.255) class]_[*@4 T][@(0.0.255) >]_[@(0.0.255) v
oid]_[* StableSort`_]([*@4 I]_[*@3 begin], [*@4 I]_[*@3 end], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&
]_[*@3 less], [@(0.0.255) const]_[*@4 T]_`*)&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:StableSort`(I`,I`,const Less`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 I],
 [@(0.0.255) class]_[*@4 Less][@(0.0.255) >]_[@(0.0.255) void]_[* StableSort]([*@4 I]_[*@3 begi
n], [*@4 I]_[*@3 end], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:StableSort`(T`&`,const Less`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T], 
[@(0.0.255) class]_[*@4 Less][@(0.0.255) >]_[@(0.0.255) void]_[* StableSort]([*@4 T][@(0.0.255) `&
]_[*@3 c], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s3;  [%-*@3 c] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:StableSort`(T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T][@(0.0.255) >]_[@(0.0.255) v
oid]_[* StableSort]([*@4 T][@(0.0.255) `&]_[*@3 c])&]
[s3;  [%-*@3 c].&]
[s4; &]
[s1; &]
[s2;:StableSortCmp`_`(I`,I`,const Cmp`&`,const T`*`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 I], [@(0.0.255) class]_[*@4 Cmp], [@(0.0.255) class]_[*@4 T][@(0.0.255) >]_[@(0.0.255) v
oid]_[* StableSortCmp`_]([*@4 I]_[*@3 begin], [*@4 I]_[*@3 end], [@(0.0.255) const]_[*@4 Cmp][@(0.0.255) `&
]_[*@3 cmp], [@(0.0.255) const]_[*@4 T]_`*)&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 cmp].&]
[s4; &]
[s1; &]
[s2;:StableSortCmp`(I`,I`,const Cmp`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 I
], [@(0.0.255) class]_[*@4 Cmp][@(0.0.255) >]_[@(0.0.255) void]_[* StableSortCmp]([*@4 I]_[*@3 b
egin], [*@4 I]_[*@3 end], [@(0.0.255) const]_[*@4 Cmp][@(0.0.255) `&]_[*@3 cmp])&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 cmp].&]
[s4; &]
[s1; &]
[s2;:StableSortCmp`(T`&`,const Cmp`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T],
 [@(0.0.255) class]_[*@4 Cmp][@(0.0.255) >]_[@(0.0.255) void]_[* StableSortCmp]([*@4 T][@(0.0.255) `&
]_[*@3 c], [@(0.0.255) const]_[*@4 Cmp][@(0.0.255) `&]_[*@3 cmp])&]
[s3;  [%-*@3 c] [%-*@3 cmp].&]
[s4; &]
[s1; &]
[s2;:StableSortCmp`(T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T][@(0.0.255) >]_
[@(0.0.255) void]_[* StableSortCmp]([*@4 T][@(0.0.255) `&]_[*@3 c])&]
[s3;  [%-*@3 c].&]
[s4; &]
[s1; &]
[s2;:`_`_IndexSort`(II`,II`,VI`,const Less`&`,const K`*`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 II], [@(0.0.255) class]_[*@4 VI], [@(0.0.255) class]_[*@4 K], 
[@(0.0.255) class]_[*@4 Less][@(0.0.255) >]_[@(0.0.255) void]_[* `_`_IndexSort]([*@4 II]_[*@3 b
egin], [*@4 II]_[*@3 end], [*@4 VI]_[*@3 pair], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 l
ess], [@(0.0.255) const]_[*@4 K]_`*)&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 pair] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:IndexSort`(II`,II`,VI`,const Less`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 I
I], [@(0.0.255) class]_[*@4 VI], [@(0.0.255) class]_[*@4 Less][@(0.0.255) >]_[@(0.0.255) void
]_[* IndexSort]([*@4 II]_[*@3 begin], [*@4 II]_[*@3 end], [*@4 VI]_[*@3 pair], 
[@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 pair] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:IndexSort`(KC`&`,VC`&`,const Less`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 K
C], [@(0.0.255) class]_[*@4 VC], [@(0.0.255) class]_[*@4 Less][@(0.0.255) >]_[@(0.0.255) void
]_[* IndexSort]([*@4 KC][@(0.0.255) `&]_[*@3 keys], [*@4 VC][@(0.0.255) `&]_[*@3 values], 
[@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s3;  [%-*@3 keys] [%-*@3 values] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:IndexSort`(KC`&`,VC`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 KC], 
[@(0.0.255) class]_[*@4 VC][@(0.0.255) >]_[@(0.0.255) void]_[* IndexSort]([*@4 KC][@(0.0.255) `&
]_[*@3 keys], [*@4 VC][@(0.0.255) `&]_[*@3 values])&]
[s3;  [%-*@3 keys] [%-*@3 values].&]
[s4; &]
[s1; &]
[s2;:`_`_StableIndexSort`(II`,II`,VI`,const Less`&`,const K`*`):%- [@(0.0.255) template
]_<[@(0.0.255) class]_[*@4 II], [@(0.0.255) class]_[*@4 VI], [@(0.0.255) class]_[*@4 K], 
[@(0.0.255) class]_[*@4 Less][@(0.0.255) >]_[@(0.0.255) void]_[* `_`_StableIndexSort]([*@4 II
]_[*@3 begin], [*@4 II]_[*@3 end], [*@4 VI]_[*@3 pair], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&
]_[*@3 less], [@(0.0.255) const]_[*@4 K]_`*)&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 pair] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:StableIndexSort`(II`,II`,VI`,const Less`&`):%- [@(0.0.255) template]_<[@(0.0.255) cl
ass]_[*@4 II], [@(0.0.255) class]_[*@4 VI], [@(0.0.255) class]_[*@4 Less][@(0.0.255) >]_[@(0.0.255) v
oid]_[* StableIndexSort]([*@4 II]_[*@3 begin], [*@4 II]_[*@3 end], [*@4 VI]_[*@3 pair], 
[@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 pair] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:StableIndexSort`(KC`&`,VC`&`,const Less`&`):%- [@(0.0.255) template]_<[@(0.0.255) cl
ass]_[*@4 KC], [@(0.0.255) class]_[*@4 VC], [@(0.0.255) class]_[*@4 Less][@(0.0.255) >]_[@(0.0.255) v
oid]_[* StableIndexSort]([*@4 KC][@(0.0.255) `&]_[*@3 keys], [*@4 VC][@(0.0.255) `&]_[*@3 value
s], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s3;  [%-*@3 keys] [%-*@3 values] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:StableIndexSort`(KC`&`,VC`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 KC], 
[@(0.0.255) class]_[*@4 VC][@(0.0.255) >]_[@(0.0.255) void]_[* StableIndexSort]([*@4 KC][@(0.0.255) `&
]_[*@3 keys], [*@4 VC][@(0.0.255) `&]_[*@3 values])&]
[s3;  [%-*@3 keys] [%-*@3 values].&]
[s4; &]
[s1; &]
[s2;:`_`_StableIndexSortCmp`(II`,II`,VI`,const Cmp`&`,const K`*`):%- [@(0.0.255) templa
te]_<[@(0.0.255) class]_[*@4 II], [@(0.0.255) class]_[*@4 VI], [@(0.0.255) class]_[*@4 K], 
[@(0.0.255) class]_[*@4 Cmp][@(0.0.255) >]_[@(0.0.255) void]_[* `_`_StableIndexSortCmp]([*@4 I
I]_[*@3 begin], [*@4 II]_[*@3 end], [*@4 VI]_[*@3 pair], [@(0.0.255) const]_[*@4 Cmp][@(0.0.255) `&
]_[*@3 cmp], [@(0.0.255) const]_[*@4 K]_`*)&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 pair] [%-*@3 cmp].&]
[s4; &]
[s1; &]
[s2;:StableIndexSortCmp`(II`,II`,VI`,const Cmp`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 II], [@(0.0.255) class]_[*@4 VI], [@(0.0.255) class]_[*@4 Cmp][@(0.0.255) >]_[@(0.0.255) v
oid]_[* StableIndexSortCmp]([*@4 II]_[*@3 begin], [*@4 II]_[*@3 end], [*@4 VI]_[*@3 pair], 
[@(0.0.255) const]_[*@4 Cmp][@(0.0.255) `&]_[*@3 cmp])&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 pair] [%-*@3 cmp].&]
[s4; &]
[s1; &]
[s2;:StableIndexSortCmp`(KC`&`,VC`&`,const Cmp`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 KC], [@(0.0.255) class]_[*@4 VC], [@(0.0.255) class]_[*@4 Cmp][@(0.0.255) >]_[@(0.0.255) v
oid]_[* StableIndexSortCmp]([*@4 KC][@(0.0.255) `&]_[*@3 keys], [*@4 VC][@(0.0.255) `&]_[*@3 va
lues], [@(0.0.255) const]_[*@4 Cmp][@(0.0.255) `&]_[*@3 cmp])&]
[s3;  [%-*@3 keys] [%-*@3 values] [%-*@3 cmp].&]
[s4; &]
[s1; &]
[s2;:StableIndexSortCmp`(KC`&`,VC`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 KC],
 [@(0.0.255) class]_[*@4 VC][@(0.0.255) >]_[@(0.0.255) void]_[* StableIndexSortCmp]([*@4 KC][@(0.0.255) `&
]_[*@3 keys], [*@4 VC][@(0.0.255) `&]_[*@3 values])&]
[s3;  [%-*@3 keys] [%-*@3 values].&]
[s4; &]
[s1; &]
[s2;:`_`_IndexSort2`(II`,II`,VI`,WI`,const Less`&`,const K`*`):%- [@(0.0.255) template]_
<[@(0.0.255) class]_[*@4 II], [@(0.0.255) class]_[*@4 VI], [@(0.0.255) class]_[*@4 WI], 
[@(0.0.255) class]_[*@4 K], [@(0.0.255) class]_[*@4 Less][@(0.0.255) >]_[@(0.0.255) void]_[* `_
`_IndexSort2]([*@4 II]_[*@3 begin], [*@4 II]_[*@3 end], [*@4 VI]_[*@3 pair1], 
[*@4 WI]_[*@3 pair2], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less], 
[@(0.0.255) const]_[*@4 K]_`*)&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 pair1] [%-*@3 pair2] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:IndexSort2`(II`,II`,VI`,WI`,const Less`&`):%- [@(0.0.255) template]_<[@(0.0.255) cla
ss]_[*@4 II], [@(0.0.255) class]_[*@4 VI], [@(0.0.255) class]_[*@4 WI], 
[@(0.0.255) class]_[*@4 Less][@(0.0.255) >]_[@(0.0.255) void]_[* IndexSort2]([*@4 II]_[*@3 begi
n], [*@4 II]_[*@3 end], [*@4 VI]_[*@3 pair1], [*@4 WI]_[*@3 pair2], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&
]_[*@3 less])&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 pair1] [%-*@3 pair2] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:IndexSort2`(KC`&`,VC`&`,WC`&`,const Less`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 KC], [@(0.0.255) class]_[*@4 VC], [@(0.0.255) class]_[*@4 WC], 
[@(0.0.255) class]_[*@4 Less][@(0.0.255) >]_[@(0.0.255) void]_[* IndexSort2]([*@4 KC][@(0.0.255) `&
]_[*@3 keys], [*@4 VC][@(0.0.255) `&]_[*@3 values1], [*@4 WC][@(0.0.255) `&]_[*@3 values2], 
[@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s3;  [%-*@3 keys] [%-*@3 values1] [%-*@3 values2] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:IndexSort2`(KC`&`,VC`&`,WC`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 KC], 
[@(0.0.255) class]_[*@4 VC], [@(0.0.255) class]_[*@4 WC][@(0.0.255) >]_[@(0.0.255) void]_[* Ind
exSort2]([*@4 KC][@(0.0.255) `&]_[*@3 keys], [*@4 VC][@(0.0.255) `&]_[*@3 values1], 
[*@4 WC][@(0.0.255) `&]_[*@3 values2])&]
[s3;  [%-*@3 keys] [%-*@3 values1] [%-*@3 values2].&]
[s4; &]
[s1; &]
[s2;:`_`_StableIndexSort2`(II`,II`,VI`,WI`,const Less`&`,const K`*`):%- [@(0.0.255) tem
plate]_<[@(0.0.255) class]_[*@4 II], [@(0.0.255) class]_[*@4 VI], [@(0.0.255) class]_[*@4 WI],
 [@(0.0.255) class]_[*@4 K], [@(0.0.255) class]_[*@4 Less][@(0.0.255) >]_[@(0.0.255) void]_[* `_
`_StableIndexSort2]([*@4 II]_[*@3 begin], [*@4 II]_[*@3 end], [*@4 VI]_[*@3 pair1], 
[*@4 WI]_[*@3 pair2], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less], 
[@(0.0.255) const]_[*@4 K]_`*)&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 pair1] [%-*@3 pair2] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:StableIndexSort2`(II`,II`,VI`,WI`,const Less`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 II], [@(0.0.255) class]_[*@4 VI], [@(0.0.255) class]_[*@4 WI], 
[@(0.0.255) class]_[*@4 Less][@(0.0.255) >]_[@(0.0.255) void]_[* StableIndexSort2]([*@4 II]_[*@3 b
egin], [*@4 II]_[*@3 end], [*@4 VI]_[*@3 pair1], [*@4 WI]_[*@3 pair2], [@(0.0.255) const]_[*@4 Le
ss][@(0.0.255) `&]_[*@3 less])&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 pair1] [%-*@3 pair2] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:StableIndexSort2`(KC`&`,VC`&`,WC`&`,const Less`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 KC], [@(0.0.255) class]_[*@4 VC], [@(0.0.255) class]_[*@4 WC], 
[@(0.0.255) class]_[*@4 Less][@(0.0.255) >]_[@(0.0.255) void]_[* StableIndexSort2]([*@4 KC][@(0.0.255) `&
]_[*@3 keys], [*@4 VC][@(0.0.255) `&]_[*@3 values1], [*@4 WC][@(0.0.255) `&]_[*@3 values2], 
[@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s3;  [%-*@3 keys] [%-*@3 values1] [%-*@3 values2] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:StableIndexSort2`(KC`&`,VC`&`,WC`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 K
C], [@(0.0.255) class]_[*@4 VC], [@(0.0.255) class]_[*@4 WC][@(0.0.255) >]_[@(0.0.255) void]_
[* StableIndexSort2]([*@4 KC][@(0.0.255) `&]_[*@3 keys], [*@4 VC][@(0.0.255) `&]_[*@3 values1],
 [*@4 WC][@(0.0.255) `&]_[*@3 values2])&]
[s3;  [%-*@3 keys] [%-*@3 values1] [%-*@3 values2].&]
[s4; &]
[s1; &]
[s2;:`_`_StableIndexSort2Cmp`(II`,II`,VI`,WI`,const Cmp`&`,const K`*`):%- [@(0.0.255) t
emplate]_<[@(0.0.255) class]_[*@4 II], [@(0.0.255) class]_[*@4 VI], [@(0.0.255) class]_[*@4 W
I], [@(0.0.255) class]_[*@4 K], [@(0.0.255) class]_[*@4 Cmp][@(0.0.255) >]_[@(0.0.255) void]_
[* `_`_StableIndexSort2Cmp]([*@4 II]_[*@3 begin], [*@4 II]_[*@3 end], [*@4 VI]_[*@3 pair1], 
[*@4 WI]_[*@3 pair2], [@(0.0.255) const]_[*@4 Cmp][@(0.0.255) `&]_[*@3 cmp], 
[@(0.0.255) const]_[*@4 K]_`*)&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 pair1] [%-*@3 pair2] [%-*@3 cmp].&]
[s4; &]
[s1; &]
[s2;:StableIndexSort2Cmp`(II`,II`,VI`,WI`,const Cmp`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 II], [@(0.0.255) class]_[*@4 VI], [@(0.0.255) class]_[*@4 WI], 
[@(0.0.255) class]_[*@4 Cmp][@(0.0.255) >]_[@(0.0.255) void]_[* StableIndexSort2Cmp]([*@4 II]_
[*@3 begin], [*@4 II]_[*@3 end], [*@4 VI]_[*@3 pair1], [*@4 WI]_[*@3 pair2], 
[@(0.0.255) const]_[*@4 Cmp][@(0.0.255) `&]_[*@3 cmp])&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 pair1] [%-*@3 pair2] [%-*@3 cmp].&]
[s4; &]
[s1; &]
[s2;:StableIndexSort2Cmp`(KC`&`,VC`&`,WC`&`,const Cmp`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 KC], [@(0.0.255) class]_[*@4 VC], [@(0.0.255) class]_[*@4 WC], 
[@(0.0.255) class]_[*@4 Cmp][@(0.0.255) >]_[@(0.0.255) void]_[* StableIndexSort2Cmp]([*@4 KC][@(0.0.255) `&
]_[*@3 keys], [*@4 VC][@(0.0.255) `&]_[*@3 values1], [*@4 WC][@(0.0.255) `&]_[*@3 values2], 
[@(0.0.255) const]_[*@4 Cmp][@(0.0.255) `&]_[*@3 cmp])&]
[s3;  [%-*@3 keys] [%-*@3 values1] [%-*@3 values2] [%-*@3 cmp].&]
[s4; &]
[s1; &]
[s2;:StableIndexSort2Cmp`(KC`&`,VC`&`,WC`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_
[*@4 KC], [@(0.0.255) class]_[*@4 VC], [@(0.0.255) class]_[*@4 WC][@(0.0.255) >]_[@(0.0.255) vo
id]_[* StableIndexSort2Cmp]([*@4 KC][@(0.0.255) `&]_[*@3 keys], [*@4 VC][@(0.0.255) `&]_[*@3 va
lues1], [*@4 WC][@(0.0.255) `&]_[*@3 values2])&]
[s3;  [%-*@3 keys] [%-*@3 values1] [%-*@3 values2].&]
[s4; &]
[s1; &]
[s2;:`_`_IndexSort3`(II`,II`,VI`,WI`,XI`,const Less`&`,const K`*`):%- [@(0.0.255) templ
ate]_<[@(0.0.255) class]_[*@4 II], [@(0.0.255) class]_[*@4 VI], [@(0.0.255) class]_[*@4 WI], 
[@(0.0.255) class]_[*@4 XI], [@(0.0.255) class]_[*@4 K], [@(0.0.255) class]_[*@4 Less][@(0.0.255) >
]_[@(0.0.255) void]_[* `_`_IndexSort3]([*@4 II]_[*@3 begin], [*@4 II]_[*@3 end], 
[*@4 VI]_[*@3 pair1], [*@4 WI]_[*@3 pair2], [*@4 XI]_[*@3 pair3], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&
]_[*@3 less], [@(0.0.255) const]_[*@4 K]_`*)&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 pair1] [%-*@3 pair2] [%-*@3 pair3] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:IndexSort3`(II`,II`,VI`,WI`,XI`,const Less`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 II], [@(0.0.255) class]_[*@4 VI], [@(0.0.255) class]_[*@4 WI], 
[@(0.0.255) class]_[*@4 XI], [@(0.0.255) class]_[*@4 Less][@(0.0.255) >]_[@(0.0.255) void]_[* I
ndexSort3]([*@4 II]_[*@3 begin], [*@4 II]_[*@3 end], [*@4 VI]_[*@3 pair1], 
[*@4 WI]_[*@3 pair2], [*@4 XI]_[*@3 pair3], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 les
s])&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 pair1] [%-*@3 pair2] [%-*@3 pair3] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:IndexSort3`(KC`&`,VC`&`,WC`&`,XC`&`,const Less`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 KC], [@(0.0.255) class]_[*@4 VC], [@(0.0.255) class]_[*@4 WC], 
[@(0.0.255) class]_[*@4 XC], [@(0.0.255) class]_[*@4 Less][@(0.0.255) >]_[@(0.0.255) void]_[* I
ndexSort3]([*@4 KC][@(0.0.255) `&]_[*@3 keys], [*@4 VC][@(0.0.255) `&]_[*@3 values1], 
[*@4 WC][@(0.0.255) `&]_[*@3 values2], [*@4 XC][@(0.0.255) `&]_[*@3 values3], 
[@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s3;  [%-*@3 keys] [%-*@3 values1] [%-*@3 values2] [%-*@3 values3] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:IndexSort3`(KC`&`,VC`&`,WC`&`,XC`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 K
C], [@(0.0.255) class]_[*@4 VC], [@(0.0.255) class]_[*@4 WC], [@(0.0.255) class]_[*@4 XC][@(0.0.255) >
]_[@(0.0.255) void]_[* IndexSort3]([*@4 KC][@(0.0.255) `&]_[*@3 keys], 
[*@4 VC][@(0.0.255) `&]_[*@3 values1], [*@4 WC][@(0.0.255) `&]_[*@3 values2], 
[*@4 XC][@(0.0.255) `&]_[*@3 values3])&]
[s3;  [%-*@3 keys] [%-*@3 values1] [%-*@3 values2] [%-*@3 values3].&]
[s4; &]
[s1; &]
[s2;:`_`_StableIndexSort3`(II`,II`,VI`,WI`,XI`,const Less`&`,const K`*`):%- [@(0.0.255) t
emplate]_<[@(0.0.255) class]_[*@4 II], [@(0.0.255) class]_[*@4 VI], [@(0.0.255) class]_[*@4 W
I], [@(0.0.255) class]_[*@4 XI], [@(0.0.255) class]_[*@4 K], [@(0.0.255) class]_[*@4 Less][@(0.0.255) >
]_[@(0.0.255) void]_[* `_`_StableIndexSort3]([*@4 II]_[*@3 begin], [*@4 II]_[*@3 end], 
[*@4 VI]_[*@3 pair1], [*@4 WI]_[*@3 pair2], [*@4 XI]_[*@3 pair3], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&
]_[*@3 less], [@(0.0.255) const]_[*@4 K]_`*)&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 pair1] [%-*@3 pair2] [%-*@3 pair3] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:StableIndexSort3`(II`,II`,VI`,WI`,XI`,const Less`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 II], [@(0.0.255) class]_[*@4 VI], [@(0.0.255) class]_[*@4 WI], 
[@(0.0.255) class]_[*@4 XI], [@(0.0.255) class]_[*@4 Less][@(0.0.255) >]_[@(0.0.255) void]_[* S
tableIndexSort3]([*@4 II]_[*@3 begin], [*@4 II]_[*@3 end], [*@4 VI]_[*@3 pair1], 
[*@4 WI]_[*@3 pair2], [*@4 XI]_[*@3 pair3], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 les
s])&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 pair1] [%-*@3 pair2] [%-*@3 pair3] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:StableIndexSort3`(KC`&`,VC`&`,WC`&`,XC`&`,const Less`&`):%- [@(0.0.255) template]_
<[@(0.0.255) class]_[*@4 KC], [@(0.0.255) class]_[*@4 VC], [@(0.0.255) class]_[*@4 WC], 
[@(0.0.255) class]_[*@4 XC], [@(0.0.255) class]_[*@4 Less][@(0.0.255) >]_[@(0.0.255) void]_[* S
tableIndexSort3]([*@4 KC][@(0.0.255) `&]_[*@3 keys], [*@4 VC][@(0.0.255) `&]_[*@3 values1], 
[*@4 WC][@(0.0.255) `&]_[*@3 values2], [*@4 XC][@(0.0.255) `&]_[*@3 values3], 
[@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s3;  [%-*@3 keys] [%-*@3 values1] [%-*@3 values2] [%-*@3 values3] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:StableIndexSort3`(KC`&`,VC`&`,WC`&`,XC`&`):%- [@(0.0.255) template]_<[@(0.0.255) cla
ss]_[*@4 KC], [@(0.0.255) class]_[*@4 VC], [@(0.0.255) class]_[*@4 WC], 
[@(0.0.255) class]_[*@4 XC][@(0.0.255) >]_[@(0.0.255) void]_[* StableIndexSort3]([*@4 KC][@(0.0.255) `&
]_[*@3 keys], [*@4 VC][@(0.0.255) `&]_[*@3 values1], [*@4 WC][@(0.0.255) `&]_[*@3 values2], 
[*@4 XC][@(0.0.255) `&]_[*@3 values3])&]
[s3;  [%-*@3 keys] [%-*@3 values1] [%-*@3 values2] [%-*@3 values3].&]
[s4; &]
[s1; &]
[s2;:`_`_StableIndexSort3Cmp`(II`,II`,VI`,WI`,XI`,const Cmp`&`,const K`*`):%- [@(0.0.255) t
emplate]_<[@(0.0.255) class]_[*@4 II], [@(0.0.255) class]_[*@4 VI], [@(0.0.255) class]_[*@4 W
I], [@(0.0.255) class]_[*@4 XI], [@(0.0.255) class]_[*@4 K], [@(0.0.255) class]_[*@4 Cmp][@(0.0.255) >
]_[@(0.0.255) void]_[* `_`_StableIndexSort3Cmp]([*@4 II]_[*@3 begin], 
[*@4 II]_[*@3 end], [*@4 VI]_[*@3 pair1], [*@4 WI]_[*@3 pair2], [*@4 XI]_[*@3 pair3], 
[@(0.0.255) const]_[*@4 Cmp][@(0.0.255) `&]_[*@3 cmp], [@(0.0.255) const]_[*@4 K]_`*)&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 pair1] [%-*@3 pair2] [%-*@3 pair3] [%-*@3 cmp].&]
[s4; &]
[s1; &]
[s2;:StableIndexSort3Cmp`(II`,II`,VI`,WI`,XI`,const Cmp`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 II], [@(0.0.255) class]_[*@4 VI], [@(0.0.255) class]_[*@4 WI], 
[@(0.0.255) class]_[*@4 XI], [@(0.0.255) class]_[*@4 Cmp][@(0.0.255) >]_[@(0.0.255) void]_[* St
ableIndexSort3Cmp]([*@4 II]_[*@3 begin], [*@4 II]_[*@3 end], [*@4 VI]_[*@3 pair1], 
[*@4 WI]_[*@3 pair2], [*@4 XI]_[*@3 pair3], [@(0.0.255) const]_[*@4 Cmp][@(0.0.255) `&]_[*@3 cmp])
&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 pair1] [%-*@3 pair2] [%-*@3 pair3] [%-*@3 cmp].&]
[s4; &]
[s1; &]
[s2;:StableIndexSort3Cmp`(KC`&`,VC`&`,WC`&`,XC`&`,const Cmp`&`):%- [@(0.0.255) template
]_<[@(0.0.255) class]_[*@4 KC], [@(0.0.255) class]_[*@4 VC], [@(0.0.255) class]_[*@4 WC], 
[@(0.0.255) class]_[*@4 XC], [@(0.0.255) class]_[*@4 Cmp][@(0.0.255) >]_[@(0.0.255) void]_[* St
ableIndexSort3Cmp]([*@4 KC][@(0.0.255) `&]_[*@3 keys], [*@4 VC][@(0.0.255) `&]_[*@3 values1],
 [*@4 WC][@(0.0.255) `&]_[*@3 values2], [*@4 XC][@(0.0.255) `&]_[*@3 values3], 
[@(0.0.255) const]_[*@4 Cmp][@(0.0.255) `&]_[*@3 cmp])&]
[s3;  [%-*@3 keys] [%-*@3 values1] [%-*@3 values2] [%-*@3 values3] [%-*@3 cmp].&]
[s4; &]
[s1; &]
[s2;:StableIndexSort3Cmp`(KC`&`,VC`&`,WC`&`,XC`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 KC], [@(0.0.255) class]_[*@4 VC], [@(0.0.255) class]_[*@4 WC], 
[@(0.0.255) class]_[*@4 XC][@(0.0.255) >]_[@(0.0.255) void]_[* StableIndexSort3Cmp]([*@4 KC][@(0.0.255) `&
]_[*@3 keys], [*@4 VC][@(0.0.255) `&]_[*@3 values1], [*@4 WC][@(0.0.255) `&]_[*@3 values2], 
[*@4 XC][@(0.0.255) `&]_[*@3 values3])&]
[s3;  [%-*@3 keys] [%-*@3 values1] [%-*@3 values2] [%-*@3 values3].&]
[s4; &]
[s1; &]
[s2;:`_`_SortOrder`(int`*`,int`*`,I`,const Less`&`,const V`*`):%- [@(0.0.255) template]_
<[@(0.0.255) class]_[*@4 I], [@(0.0.255) class]_[*@4 V], [@(0.0.255) class]_[*@4 Less][@(0.0.255) >
]_[@(0.0.255) void]_[* `_`_SortOrder]([@(0.0.255) int]_`*[*@3 begin], 
[@(0.0.255) int]_`*[*@3 end], [*@4 I]_[*@3 data], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_
[*@3 less], [@(0.0.255) const]_[*@4 V]_`*)&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 data] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:GetSortOrder`(I`,I`,const Less`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 I
], [@(0.0.255) class]_[*@4 Less][@(0.0.255) >]_Vector[@(0.0.255) <int>]_[* GetSortOrder]([*@4 I
]_[*@3 begin], [*@4 I]_[*@3 end], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:GetSortOrder`(const C`&`,const Less`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_
[*@4 C], [@(0.0.255) class]_[*@4 Less][@(0.0.255) >]_Vector[@(0.0.255) <int>]_[* GetSortOrder
]([@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 container], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&
]_[*@3 less])&]
[s3;  [%-*@3 container] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:GetSortOrder`(const C`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 C][@(0.0.255) >
]_Vector[@(0.0.255) <int>]_[* GetSortOrder]([@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 co
ntainer])&]
[s3;  [%-*@3 container].&]
[s4; &]
[s1; &]
[s2;:`_`_StableSortOrder`(int`*`,int`*`,I`,const Less`&`,const T`*`):%- [@(0.0.255) tem
plate]_<[@(0.0.255) class]_[*@4 I], [@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 Less][@(0.0.255) >
]_[@(0.0.255) void]_[* `_`_StableSortOrder]([@(0.0.255) int]_`*[*@3 ibegin], 
[@(0.0.255) int]_`*[*@3 iend], [*@4 I]_[*@3 data], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_
[*@3 less], [@(0.0.255) const]_[*@4 T]_`*)&]
[s3;  [%-*@3 ibegin] [%-*@3 iend] [%-*@3 data] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:GetStableSortOrder`(I`,I`,const Less`&`):%- [@(0.0.255) template]_<[@(0.0.255) class
]_[*@4 I], [@(0.0.255) class]_[*@4 Less][@(0.0.255) >]_Vector[@(0.0.255) <int>]_[* GetStableS
ortOrder]([*@4 I]_[*@3 begin], [*@4 I]_[*@3 end], [@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_
[*@3 less])&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:GetStableSortOrder`(const C`&`,const Less`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 C], [@(0.0.255) class]_[*@4 Less][@(0.0.255) >]_Vector[@(0.0.255) <int>]_[* GetSta
bleSortOrder]([@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 container], 
[@(0.0.255) const]_[*@4 Less][@(0.0.255) `&]_[*@3 less])&]
[s3;  [%-*@3 container] [%-*@3 less].&]
[s4; &]
[s1; &]
[s2;:GetStableSortOrder`(const C`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 C][@(0.0.255) >
]_Vector[@(0.0.255) <int>]_[* GetStableSortOrder]([@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_
[*@3 container])&]
[s3;  [%-*@3 container].&]
[s4; &]
[s1; &]
[s2;:`_`_StableSortOrderCmp`(int`*`,int`*`,I`,const Cmp`&`,const T`*`):%- [@(0.0.255) t
emplate]_<[@(0.0.255) class]_[*@4 I], [@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 Cmp
][@(0.0.255) >]_[@(0.0.255) void]_[* `_`_StableSortOrderCmp]([@(0.0.255) int]_`*[*@3 ibegin
], [@(0.0.255) int]_`*[*@3 iend], [*@4 I]_[*@3 data], [@(0.0.255) const]_[*@4 Cmp][@(0.0.255) `&
]_[*@3 cmp], [@(0.0.255) const]_[*@4 T]_`*)&]
[s3;  [%-*@3 ibegin] [%-*@3 iend] [%-*@3 data] [%-*@3 cmp].&]
[s4; &]
[s1; &]
[s2;:GetStableSortOrderCmp`(I`,I`,const Cmp`&`):%- [@(0.0.255) template]_<[@(0.0.255) cla
ss]_[*@4 I], [@(0.0.255) class]_[*@4 Cmp][@(0.0.255) >]_Vector[@(0.0.255) <int>]_[* GetStable
SortOrderCmp]([*@4 I]_[*@3 begin], [*@4 I]_[*@3 end], [@(0.0.255) const]_[*@4 Cmp][@(0.0.255) `&
]_[*@3 cmp])&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 cmp].&]
[s4; &]
[s1; &]
[s2;:GetStableSortOrderCmp`(const C`&`,const Cmp`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 C], [@(0.0.255) class]_[*@4 Cmp][@(0.0.255) >]_Vector[@(0.0.255) <int>]_[* GetStab
leSortOrderCmp]([@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 container], 
[@(0.0.255) const]_[*@4 Cmp][@(0.0.255) `&]_[*@3 cmp])&]
[s3;  [%-*@3 container] [%-*@3 cmp].&]
[s4; &]
[s1; &]
[s2;:GetStableSortOrderCmp`(const C`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 C
][@(0.0.255) >]_Vector[@(0.0.255) <int>]_[* GetStableSortOrderCmp]([@(0.0.255) const]_[*@4 C
][@(0.0.255) `&]_[*@3 container])&]
[s3;  [%-*@3 container].&]
[s4; &]
[s1; &]
[s2;:GetFieldContainer`(DC`&`,I`,I`,F`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 D
C], [@(0.0.255) class]_[*@4 I], [@(0.0.255) class]_[*@4 F][@(0.0.255) >]_[@(0.0.255) void]_[* G
etFieldContainer]([*@4 DC][@(0.0.255) `&]_[*@3 dest], [*@4 I]_[*@3 begin], 
[*@4 I]_[*@3 end], [*@4 F]_[*@3 field])&]
[s3;  [%-*@3 dest] [%-*@3 begin] [%-*@3 end] [%-*@3 field].&]
[s4; &]
[s1; &]
[s2;:GetFieldContainer`(DC`&`,const SC`&`,F`):%- [@(0.0.255) template]_<[@(0.0.255) class
]_[*@4 DC], [@(0.0.255) class]_[*@4 SC], [@(0.0.255) class]_[*@4 F][@(0.0.255) >]_[@(0.0.255) v
oid]_[* GetFieldContainer]([*@4 DC][@(0.0.255) `&]_[*@3 dest], [@(0.0.255) const]_[*@4 SC][@(0.0.255) `&
]_[*@3 src], [*@4 F]_[*@3 field])&]
[s3;  [%-*@3 dest] [%-*@3 src] [%-*@3 field].&]
[s4; &]
[s1; &]
[s2;:FindField`(I`,I`,F`,const O`&`,const E`&`):%- [@(0.0.255) template]_<[@(0.0.255) cla
ss]_[*@4 I], [@(0.0.255) class]_[*@4 F], [@(0.0.255) class]_[*@4 O], [@(0.0.255) class]_[*@4 E][@(0.0.255) >
]_[*@4 I]_[* FindField]([*@4 I]_[*@3 begin], [*@4 I]_[*@3 end], [*@4 F]_[*@3 field], 
[@(0.0.255) const]_[*@4 O][@(0.0.255) `&]_[*@3 object], [@(0.0.255) const]_[*@4 E][@(0.0.255) `&
]_[*@3 equal])&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 field] [%-*@3 object] [%-*@3 equal].&]
[s4; &]
[s1; &]
[s2;:FindField`(I`,I`,F`,const O`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 I], 
[@(0.0.255) class]_[*@4 F], [@(0.0.255) class]_[*@4 O][@(0.0.255) >]_[*@4 I]_[* FindField]([*@4 I
]_[*@3 begin], [*@4 I]_[*@3 end], [*@4 F]_[*@3 field], [@(0.0.255) const]_[*@4 O][@(0.0.255) `&]_
[*@3 object])&]
[s3;  [%-*@3 begin] [%-*@3 end] [%-*@3 field] [%-*@3 object].&]
[s4; &]
[s1; &]
[s2;:FindFieldIndex`(const C`&`,F`,const O`&`,const E`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 C], [@(0.0.255) class]_[*@4 F], [@(0.0.255) class]_[*@4 O], 
[@(0.0.255) class]_[*@4 E][@(0.0.255) >]_[@(0.0.255) int]_[* FindFieldIndex]([@(0.0.255) cons
t]_[*@4 C][@(0.0.255) `&]_[*@3 container], [*@4 F]_[*@3 field], [@(0.0.255) const]_[*@4 O][@(0.0.255) `&
]_[*@3 object], [@(0.0.255) const]_[*@4 E][@(0.0.255) `&]_[*@3 equal])&]
[s3;  [%-*@3 container] [%-*@3 field] [%-*@3 object] [%-*@3 equal].&]
[s4; &]
[s1; &]
[s2;:FindFieldIndex`(const C`&`,F`,const O`&`):%- [@(0.0.255) template]_<[@(0.0.255) clas
s]_[*@4 C], [@(0.0.255) class]_[*@4 F], [@(0.0.255) class]_[*@4 O][@(0.0.255) >]_[@(0.0.255) in
t]_[* FindFieldIndex]([@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 container], 
[*@4 F]_[*@3 field], [@(0.0.255) const]_[*@4 O][@(0.0.255) `&]_[*@3 object])&]
[s3;  [%-*@3 container] [%-*@3 field] [%-*@3 object].&]
[s4; &]
[s1; &]
[s2;:FieldRelation`(O`(`*`)`,const R`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 O
], [@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 R][@(0.0.255) >]_FieldRelationCls[@(0.0.255) <
][*@4 O], [*@4 T], [*@4 R][@(0.0.255) >]_[* FieldRelation]([*@4 O]_(T`::[@(0.0.255) `*][*@3 member
]), [@(0.0.255) const]_[*@4 R][@(0.0.255) `&]_[*@3 relation])&]
[s3;  [%-*@3 member] [%-*@3 relation].&]
[s4; &]
[s1; &]
[s2;:MethodRelation`(O`(`*`)`(`)`,const R`&`):%- [@(0.0.255) template]_<[@(0.0.255) class
]_[*@4 O], [@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 R][@(0.0.255) >]_MethodRelatio
nCls[@(0.0.255) <][*@4 O]_(T`::[@(0.0.255) `*])(), [*@4 T], [*@4 R][@(0.0.255) >]_[* MethodRelat
ion]([*@4 O]_(T`::[@(0.0.255) `*][*@3 method])(), [@(0.0.255) const]_[*@4 R][@(0.0.255) `&]_[*@3 r
elation])&]
[s3;  [%-*@3 method] [%-*@3 relation].&]
[s4; &]
[s1; &]
[s2;:MethodRelation`(O`(`*`)`(`)const`,const R`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 O], [@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 R][@(0.0.255) >]_MethodRel
ationCls[@(0.0.255) <][*@4 O]_(T`::[@(0.0.255) `*])()_[@(0.0.255) const], 
[*@4 T], [*@4 R][@(0.0.255) >]_[* MethodRelation]([*@4 O]_(T`::[@(0.0.255) `*][*@3 method])()_[@(0.0.255) c
onst], [@(0.0.255) const]_[*@4 R][@(0.0.255) `&]_[*@3 relation])&]
[s3;  [%-*@3 method] [%-*@3 relation].&]
[s4; &]
[s1; &]
[s2;:LruAdd`(C`&`,T`,int`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 C], 
[@(0.0.255) class]_[*@4 T][@(0.0.255) >]_[@(0.0.255) void]_[* LruAdd]([*@4 C][@(0.0.255) `&]_[*@3 l
ru], [*@4 T]_[*@3 value], [@(0.0.255) int]_[*@3 limit]_`=_[@3 10])&]
[s3;  [%-*@3 lru] [%-*@3 value] [%-*@3 limit].&]
[s4; &]
[s0; ]