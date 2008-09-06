topic "Algorithms";[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,7#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[l288;a17;*1 $$7,7#70004532496200323422659154056402:requirement]
[{_}%EN-US 
[s3; Algorithms&]
[s0; &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 T][%00-00 >__][%00-00@(0.0.255) v
oid][%00-00 _][%00-00@0 Swap][%00-00 (][%00-00^T^ T][%00-00 `&_][%00-00@3 a][%00-00 , 
][%00-00^T^ T][%00-00 `&_][%00-00@3 b][%00-00 )]&]
[s2; Swaps values. Specific types might specialize [* Swap] with more 
effective variants.&]
[s7; T must have either deep copy operator or pick operator.&]
[s4; [*C@4 T]-|Type of values.&]
[s4; [*C@3 a]-|First value to swap.&]
[s4; [*C@3 b]-|Second value to swap.&]
[s0; &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 I][%00-00 >__][%00-00@(0.0.255) v
oid][%00-00 _][%00-00@0 IterSwap][%00-00 (][%00-00^I^ I][%00-00 _][%00-00@3 a][%00-00 , 
][%00-00^I^ I][%00-00 _][%00-00@3 b][%00-00 )]&]
[s2; Swaps values pointed to by iterators. Specific types might specialize 
[* IterSwap] with more effective variants.&]
[s7; Swap must be defined for type pointed to by I.&]
[s4; [*C@4 I]-|Iterator type.&]
[s4; [*C@3 a]-|Iterator pointing to first value.&]
[s4; [*C@3 b]-|Iterator pointing to second value.&]
[s0; &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 C][%00-00 , 
][%00-00@(0.0.255) class][%00-00 _][%00-00@4 T][%00-00 , ][%00-00@(0.0.255) class][%00-00 _][%00-00@4 L
][%00-00 >__][%00-00@(0.0.255) int][%00-00 _][%00-00@0 FindLowerBound][%00-00 (][%00-00@(0.0.255) c
onst][%00-00 _][%00-00^C^ C][%00-00 `&_][%00-00@3 v][%00-00 , ][%00-00@(0.0.255) int][%00-00 _][%00-00@3 p
os][%00-00 , ][%00-00@(0.0.255) int][%00-00 _][%00-00@3 count][%00-00 , 
][%00-00@(0.0.255) const][%00-00 _][%00-00^T^ T][%00-00 `&_][%00-00@3 val][%00-00 , 
][%00-00@(0.0.255) const][%00-00 _][%00-00^L^ L][%00-00 `&_][%00-00@3 less][%00-00 )]&]
[s2; Finds first position in range of container sorted by [* less] 
predicate where [* val] can be inserted without breaking the ordering.&]
[s4; [*C@4 C]-|Type of container.&]
[s4; [*C@4 T]-|Type of value.&]
[s4; [*C@3 v]-|Container.&]
[s4; [*C@3 pos]-|Beginning of range.&]
[s4; [*C@3 count]-|Number of elements in range.&]
[s4; [*C@3 val]-|Value to find.&]
[s4; [*C@3 less]-|Ordering predicate.&]
[s4; [*/ Return value]-|Position in container.&]
[s0; &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 C][%00-00 , 
][%00-00@(0.0.255) class][%00-00 _][%00-00@4 T][%00-00 , ][%00-00@(0.0.255) class][%00-00 _][%00-00@4 L
][%00-00 >__][%00-00@(0.0.255) int][%00-00 _][%00-00@0 FindLowerBound][%00-00 (][%00-00@(0.0.255) c
onst][%00-00 _][%00-00^C^ C][%00-00 `&_][%00-00@3 v][%00-00 , ][%00-00@(0.0.255) const][%00-00 _
][%00-00^T^ T][%00-00 `&_][%00-00@3 val][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00^L^ L
][%00-00 `&_][%00-00@3 less][%00-00 )]&]
[s2; Finds first position in sorted by [* less] predicate where [* val] 
can be inserted without breaking the ordering.&]
[s4; [*C@4 C]-|Type of container.&]
[s4; [*C@4 T]-|Type of value.&]
[s4; [*C@3 v]-|Container.&]
[s4; [*C@3 val]-|Value to find.&]
[s4; [*C@3 less]-|Ordering predicate.&]
[s4; [*/ Return value]-|Position in container.&]
[s0; &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 C][%00-00 , 
][%00-00@(0.0.255) class][%00-00 _][%00-00@4 T][%00-00 >__][%00-00@(0.0.255) int][%00-00 _][%00-00@0 F
indLowerBound][%00-00 (][%00-00@(0.0.255) const][%00-00 _][%00-00^C^ C][%00-00 `&_][%00-00@3 v
][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00^T^ T][%00-00 `&_][%00-00@3 val][%00-00 )]&]
[s2; Finds first position in sorted by [*/ operator<] predicate where 
[* val] can be inserted without breaking the ordering.&]
[s4; [*C@4 C]-|Type of container.&]
[s4; [*C@4 T]-|Type of value.&]
[s4; [*C@3 v]-|Container.&]
[s4; [*C@3 val]-|Value to find.&]
[s4; [*/ Return value]-|Position in container.&]
[s0; &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 C][%00-00 , 
][%00-00@(0.0.255) class][%00-00 _][%00-00@4 T][%00-00 , ][%00-00@(0.0.255) class][%00-00 _][%00-00@4 L
][%00-00 >__][%00-00@(0.0.255) int][%00-00 _][%00-00@0 FindUpperBound][%00-00 (][%00-00@(0.0.255) c
onst][%00-00 _][%00-00^C^ C][%00-00 `&_][%00-00@3 v][%00-00 , ][%00-00@(0.0.255) int][%00-00 _][%00-00@3 p
os][%00-00 , ][%00-00@(0.0.255) int][%00-00 _][%00-00@3 count][%00-00 , 
][%00-00@(0.0.255) const][%00-00 _][%00-00^T^ T][%00-00 `&_][%00-00@3 val][%00-00 , 
][%00-00@(0.0.255) const][%00-00 _][%00-00^L^ L][%00-00 `&_][%00-00@3 less][%00-00 )]&]
[s2; Finds last position in range of container sorted by [* less] predicate 
where [* val] can be inserted without breaking the ordering.&]
[s4; [*C@4 C]-|Type of container.&]
[s4; [*C@4 T]-|Type of value.&]
[s4; [*C@3 v]-|Container.&]
[s4; [*C@3 pos]-|Beginning of range.&]
[s4; [*C@3 count]-|Number of elements in range.&]
[s4; [*C@3 val]-|Value to find.&]
[s4; [*C@3 less]-|Ordering predicate.&]
[s4; [*/ Return value]-|Position in container.&]
[s0; &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 C][%00-00 , 
][%00-00@(0.0.255) class][%00-00 _][%00-00@4 T][%00-00 , ][%00-00@(0.0.255) class][%00-00 _][%00-00@4 L
][%00-00 >__][%00-00@(0.0.255) int][%00-00 _][%00-00@0 FindUpperBound][%00-00 (][%00-00@(0.0.255) c
onst][%00-00 _][%00-00^C^ C][%00-00 `&_][%00-00@3 v][%00-00 , ][%00-00@(0.0.255) const][%00-00 _
][%00-00^T^ T][%00-00 `&_][%00-00@3 val][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00^L^ L
][%00-00 `&_][%00-00@3 less][%00-00 )]&]
[s2; Finds last position in sorted by [* less] predicate where [* val] 
can be inserted without breaking the ordering.&]
[s4; [*C@4 C]-|Type of container.&]
[s4; [*C@4 T]-|Type of value.&]
[s4; [*C@3 v]-|Container.&]
[s4; [*C@3 val]-|Value to find.&]
[s4; [*C@3 less]-|Ordering predicate.&]
[s4; [*/ Return value]-|Position in container.&]
[s0;:template`<class C`, class T`> int FindUpperBound`(const C`& v`, const T`& val`): &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 C][%00-00 , 
][%00-00@(0.0.255) class][%00-00 _][%00-00@4 T][%00-00 >__][%00-00@(0.0.255) int][%00-00 _][%00-00@0 F
indUpperBound][%00-00 (][%00-00@(0.0.255) const][%00-00 _][%00-00^C^ C][%00-00 `&_][%00-00@3 v
][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00^T^ T][%00-00 `&_][%00-00@3 val][%00-00 )]&]
[s2; Finds last position in sorted by [*/ operator<] predicate where 
[* val] can be inserted without breaking the ordering.&]
[s4; [*C@4 C]-|Type of container.&]
[s4; [*C@4 T]-|Type of value.&]
[s4; [*C@3 v]-|Container.&]
[s4; [*C@3 val]-|Value to find.&]
[s4; [*/ Return value]-|Position in container.&]
[s0; &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 C][%00-00 , 
][%00-00@(0.0.255) class][%00-00 _][%00-00@4 T][%00-00 , ][%00-00@(0.0.255) class][%00-00 _][%00-00@4 L
][%00-00 >__][%00-00@(0.0.255) int][%00-00 _][%00-00@0 FindBinary][%00-00 (][%00-00@(0.0.255) c
onst][%00-00 _][%00-00^C^ C][%00-00 `&_][%00-00@3 v][%00-00 , ][%00-00@(0.0.255) const][%00-00 _
][%00-00^T^ T][%00-00 `&_][%00-00@3 val][%00-00 , ][%00-00@(0.0.255) int][%00-00 _][%00-00@3 po
s][%00-00 , ][%00-00@(0.0.255) int][%00-00 _][%00-00@3 count][%00-00 , 
][%00-00@(0.0.255) const][%00-00 _][%00-00^L^ L][%00-00 `&_][%00-00@3 less][%00-00 )]&]
[s2; Finds position of element with specified value in a range of 
container sorted by [* less] predicate. If no such element exists, 
a negative value is returned.&]
[s4; [*C@4 C]-|Type of container.&]
[s4; [*C@4 T]-|Type of value.&]
[s4; [*C@3 v]-|Container.&]
[s4; [*C@3 pos]-|Beginning of range.&]
[s4; [*C@3 count]-|Number of elements in range.&]
[s4; [*C@3 val]-|Value to find.&]
[s4; [*C@3 less]-|Ordering predicate.&]
[s4; [*/ Return value]-|Position in container.&]
[s0; &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 C][%00-00 , 
][%00-00@(0.0.255) class][%00-00 _][%00-00@4 T][%00-00 , ][%00-00@(0.0.255) class][%00-00 _][%00-00@4 L
][%00-00 >__][%00-00@(0.0.255) int][%00-00 _][%00-00@0 FindBinary][%00-00 (][%00-00@(0.0.255) c
onst][%00-00 _][%00-00^C^ C][%00-00 `&_][%00-00@3 v][%00-00 , ][%00-00@(0.0.255) const][%00-00 _
][%00-00^T^ T][%00-00 `&_][%00-00@3 val][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00^L^ L
][%00-00 `&_][%00-00@3 less][%00-00 )]&]
[s2; Finds position of element with specified value in the container 
sorted by [* less] predicate. If no such element exists, a negative 
value is returned.&]
[s4; [*C@4 C]-|Type of container.&]
[s4; [*C@4 T]-|Type of value.&]
[s4; [*C@3 v]-|Container.&]
[s4; [*C@3 val]-|Value to find.&]
[s4; [*C@3 less]-|Ordering predicate.&]
[s4; [*/ Return value]-|Position in container.&]
[s0; &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 C][%00-00 , 
][%00-00@(0.0.255) class][%00-00 _][%00-00@4 T][%00-00 >__][%00-00@(0.0.255) int][%00-00 _][%00-00@0 F
indBinary][%00-00 (][%00-00@(0.0.255) const][%00-00 _][%00-00^C^ C][%00-00 `&_][%00-00@3 v][%00-00 ,
 ][%00-00@(0.0.255) const][%00-00 _][%00-00^T^ T][%00-00 `&_][%00-00@3 val][%00-00 )]&]
[s2; Finds position of element with specified value in the container 
sorted by [*/ operator<] predicate. If no such element exists, 
a negative value is returned.&]
[s4; [*C@4 C]-|Type of container.&]
[s4; [*C@4 T]-|Type of value.&]
[s4; [*C@3 v]-|Container.&]
[s4; [*C@3 val]-|Value to find.&]
[s4; [*/ Return value]-|Position in container.&]
[s0;~~~.992; &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00^C^ C][%00-00 , 
][%00-00@(0.0.255) class][%00-00 _][%00-00@4 L][%00-00 >__][%00-00^C^ C][%00-00 `&_][%00-00@0 A
ppendSorted][%00-00 (][%00-00^C^ C][%00-00 `&_][%00-00@3 dest][%00-00 , 
][%00-00@(0.0.255) const][%00-00 _][%00-00^C^ C][%00-00 `&_][%00-00@3 src][%00-00 , 
][%00-00@(0.0.255) const][%00-00 _][%00-00^L^ L][%00-00 `&_][%00-00@3 less][%00-00 )]&]
[s2; Merges source NTL container to destination NTL container. Both 
containers must be sorted in ascending order. After the operation, 
destination container is sorted in ascending order and contains 
values from both containers. Duplicate values are preserved. 
Ordering predicate is determined by [* less].&]
[s4; [*C@4 C]-|Type of container.&]
[s4; [*C@3 dest]-|Destination container.&]
[s4; [*C@3 src]-|Source container.&]
[s4; [*C@3 less]-|Ordering predicate.&]
[s4;C [*/A Return value][A -|Destination container.]template<class [*@4 C]>&]
[s0;~~~.992; &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00^C^ C][%00-00 >__][%00-00^C^ C][%00-00 `&
_][%00-00@0 AppendSorted][%00-00 (][%00-00^C^ C][%00-00 `&_][%00-00@3 dest][%00-00 , 
][%00-00@(0.0.255) const][%00-00 _][%00-00^C^ C][%00-00 `&_][%00-00@3 src][%00-00 )]&]
[s2;~~~.992; Merges source NTL container to destination NTL container. 
Both containers must be sorted in ascending order. After the 
operation, destination container is sorted in ascending order 
and contains values from both containers. Duplicate values are 
preserved. Ordering is determined by [*/ operator<].&]
[s4; [*C@4 C]-|Type of container.&]
[s4; [*C@3 dest]-|Destination container.&]
[s4; [*C@3 src]-|Source container.&]
[s4;C [*/A Return value][A -|Destination container.]template<class [*@4 C]>&]
[s0; &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00^C^ C][%00-00 , 
][%00-00@(0.0.255) class][%00-00 _][%00-00@4 L][%00-00 >__][%00-00^C^ C][%00-00 `&_][%00-00@0 U
nionSorted][%00-00 (][%00-00^C^ C][%00-00 `&_][%00-00@3 dest][%00-00 , 
][%00-00@(0.0.255) const][%00-00 _][%00-00^C^ C][%00-00 `&_][%00-00@3 src][%00-00 , 
][%00-00@(0.0.255) const][%00-00 _][%00-00^L^ L][%00-00 `&_][%00-00@3 less][%00-00 )]&]
[s2; Merges source NTL container to destination NTL container. Both 
containers must be sorted in ascending order and values must 
be unique. After the operation, destination container is sorted 
in ascending order and contains unique values from both containers. 
Ordering is determined by [* less].&]
[s4; [*C@4 C]-|Type of container.&]
[s4; [*C@3 dest]-|Destination container.&]
[s4; [*C@3 src]-|Source container.&]
[s4; [*C@3 less]-|Ordering predicate.&]
[s4; [*/ Return value]-|Destination container.&]
[s0; &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00^C^ C][%00-00 >__][%00-00^C^ C][%00-00 `&
_][%00-00@0 UnionSorted][%00-00 (][%00-00^C^ C][%00-00 `&_][%00-00@3 dest][%00-00 , 
][%00-00@(0.0.255) const][%00-00 _][%00-00^C^ C][%00-00 `&_][%00-00@3 src][%00-00 )]&]
[s2; Merges the source NTL container to the destination NTL container. 
Both containers must be sorted in ascending order and values 
must be unique. After the operation, destination container is 
sorted in ascending order and contains unique values from both 
containers. Ordering is determined by [*/ operator<].&]
[s4; [*C@4 C]-|Type of container.&]
[s4; [*C@3 dest]-|Destination container.&]
[s4; [*C@3 src]-|Source container.&]
[s4; [*/ Return value]-|Destination container.&]
[s0; &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00^C^ C][%00-00 , 
][%00-00@(0.0.255) class][%00-00 _][%00-00@4 L][%00-00 >__][%00-00^C^ C][%00-00 `&_][%00-00@0 R
emoveSorted][%00-00 (][%00-00^C^ C][%00-00 `&_][%00-00@3 from][%00-00 , 
][%00-00@(0.0.255) const][%00-00 _][%00-00^C^ C][%00-00 `&_][%00-00@3 what][%00-00 , 
][%00-00@(0.0.255) const][%00-00 _][%00-00^L^ L][%00-00 `&_][%00-00@3 less][%00-00 )]&]
[s2; Removes elements of source container from destination container. 
Both containers must be sorted in ascending order and values 
must be unique. Ordering is determined by [* less].&]
[s4; [*C@4 C]-|Type of container.&]
[s4; [*C@3 from]-|Destination container.&]
[s4; [*C@3 what]-|Source container.&]
[s4; [*C@3 less]-|Ordering predicate.&]
[s4; [*/ Return value]-|Destination container.&]
[s0; &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00^C^ C][%00-00 >__][%00-00^C^ C][%00-00 `&
_][%00-00@0 RemoveSorted][%00-00 (][%00-00^C^ C][%00-00 `&_][%00-00@3 from][%00-00 , 
][%00-00@(0.0.255) const][%00-00 _][%00-00^C^ C][%00-00 `&_][%00-00@3 what][%00-00 )]&]
[s2; Removes elements of source container from destination container. 
Both containers must be sorted in ascending order and values 
must be unique. Ordering is determined by [*/ operator<].&]
[s4; [*C@4 C]-|Type of container.&]
[s4; [*C@3 from]-|Destination container.&]
[s4; [*C@3 what]-|Source container.&]
[s4; [*/ Return value]-|Destination container.&]
[s0; &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00^D^ D][%00-00 , 
][%00-00@(0.0.255) class][%00-00 _][%00-00@4 S][%00-00 , ][%00-00@(0.0.255) class][%00-00 _][%00-00@4 L
][%00-00 >__][%00-00^D^ D][%00-00 `&_][%00-00@0 IntersectSorted][%00-00 (][%00-00^D^ D][%00-00 `&
_][%00-00@3 dest][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00^S^ S][%00-00 `&_][%00-00@3 s
rc][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00^L^ L][%00-00 `&_][%00-00@3 less][%00-00 )
]&]
[s2; Removes elements from destination container that are not contained 
in source container. Both containers must be sorted in ascending 
order and values must be unique. Ordering is determined by [* less].&]
[s4; [*C@4 D]-|Type of destination container.&]
[s4; [*C@4 S]-|Type of source container.&]
[s4; [*C@3 dest]-|Destination container.&]
[s4; [*C@3 src]-|Source container.&]
[s4; [*C@3 less]-|Destination container.&]
[s0; &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00^D^ D][%00-00 , 
][%00-00@(0.0.255) class][%00-00 _][%00-00@4 S][%00-00 >__][%00-00^D^ D][%00-00 `&_][%00-00@0 I
ntersectSorted][%00-00 (][%00-00^D^ D][%00-00 `&_][%00-00@3 dest][%00-00 , 
][%00-00@(0.0.255) const][%00-00 _][%00-00^S^ S][%00-00 `&_][%00-00@3 src][%00-00 )]&]
[s2; Removes elements from destination container that are not contained 
in source container. Both containers must be sorted in ascending 
order and values must be unique. Ordering is determined by [*/ operator<].&]
[s4; [*C@4 D]-|Type of destination container.&]
[s4; [*C@4 S]-|Type of source container.&]
[s4; [*C@3 dest]-|Destination container.&]
[s4; [*C@3 src]-|Source container.&]
[s4; [*/ Return value]-|Destination container.&]
[s0; &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 T][%00-00 , 
][%00-00@(0.0.255) class][%00-00 _][%00-00@4 Less][%00-00 >__][%00-00@(0.0.255) void][%00-00 _
][%00-00@0 Sort][%00-00 (][%00-00^T^ T][%00-00 `&_][%00-00@3 c][%00-00 , 
][%00-00@(0.0.255) const][%00-00 _][%00-00^Less^ Less][%00-00 `&_][%00-00@3 less][%00-00 )]&]
[s2; Sorts container. Ordering is determined by [* less].&]
[s7; IterSwap must be defined for T`::Iterator.&]
[s4; [*C@4 T]-|Type of container.&]
[s4; [*C@3 c]-|Container.&]
[s4; [*C@3 less]-|Ordering predicate.&]
[s0; &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 T][%00-00 >__][%00-00@(0.0.255) v
oid][%00-00 _][%00-00@0 Sort][%00-00 (][%00-00^T^ T][%00-00 `&_][%00-00@3 c][%00-00 )]&]
[s2; Sorts container. Ordering is determined by [*/ operator<].&]
[s2;*1 IterSwap must be defined for T`::Iterator.&]
[s4; [*C@4 T]-|Type of container.&]
[s4; [*C@3 c]-|Container.&]
[s0; &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 KC][%00-00 , 
][%00-00@(0.0.255) class][%00-00 _][%00-00@4 VC][%00-00 , ][%00-00@(0.0.255) class][%00-00 _][%00-00@4 L
ess][%00-00 >__][%00-00@(0.0.255) void][%00-00 _][%00-00@0 IndexSort][%00-00 (][%00-00^KC^ KC
][%00-00 `&_][%00-00@3 keys][%00-00 , ][%00-00^VC^ VC][%00-00 `&_][%00-00@3 values][%00-00 , 
][%00-00@(0.0.255) const][%00-00 _][%00-00^Less^ Less][%00-00 `&_][%00-00@3 less][%00-00 )]&]
[s2; Sorts pair of containers. Both containers must have same number 
of items. Resulting order is determined by the [* keys] container. 
Ordering is determined by [* less].&]
[s4; [*C@4 KC]-|Type of keys container.&]
[s4; [*C@4 VC]-|Type of values.&]
[s4; [*C@3 keys]-|Container of keys.&]
[s4; [*C@3 values]-|Container of values.&]
[s4; [*C@3 less]-|Ordering predicate.&]
[s0; &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 KC][%00-00 , 
][%00-00@(0.0.255) class][%00-00 _][%00-00@4 VC][%00-00 >__][%00-00@(0.0.255) void][%00-00 _][%00-00@0 I
ndexSort][%00-00 (][%00-00^KC^ KC][%00-00 `&_][%00-00@3 keys][%00-00 , 
][%00-00^VC^ VC][%00-00 `&_][%00-00@3 values][%00-00 )]&]
[s2; Sorts pair of containers. Both containers must have same number 
of items. Resulting order is determined by the [* keys] container. 
Ordering is determined by [*/ operator<].&]
[s4; [*C@4 KC]-|Type of keys container.&]
[s4; [*C@4 VC]-|Type of values container.&]
[s4; [*C@3 keys]-|Container of keys.&]
[s4; [*C@3 values]-|Container of values.&]
[s0; &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 C][%00-00 , 
][%00-00@(0.0.255) class][%00-00 _][%00-00@4 Less][%00-00 >__][%00-00^`:`:Vector^ Vector][%00-00 <
][%00-00^int^ int][%00-00 >_][%00-00@0 GetSortOrder][%00-00 (][%00-00@(0.0.255) const][%00-00 _
][%00-00^C^ C][%00-00 `&_][%00-00@3 container][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00^Less^ L
ess][%00-00 `&_][%00-00@3 less][%00-00 )]&]
[s2; Creates ascending order of values in container. Ordering is 
determined by [* less].&]
[s4; [*C@4 C]-|Type of container.&]
[s4; [*C@3 container]-|Source container.&]
[s4; [*C@3 less]-|Ordering predicate.&]
[s4; [*/ Return value]-|Vector of positions of source container in sorted 
order.&]
[s0; &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 C][%00-00 >__][%00-00^`:`:Vector^ V
ector][%00-00 <][%00-00^int^ int][%00-00 >_][%00-00@0 GetSortOrder][%00-00 (][%00-00@(0.0.255) c
onst][%00-00 _][%00-00^C^ C][%00-00 `&_][%00-00@3 container][%00-00 )]&]
[s2; Creates ascending order of values in container. Ordering is 
determined by [*/ operator<].&]
[s4; [*C@4 C]-|Type of container.&]
[s4; [*C@3 container]-|Source container.&]
[s4; [*/ Return value]-|Vector of positions of source container in sorted 
order.&]
[s0; &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00^O^ O][%00-00 , 
][%00-00@(0.0.255) class][%00-00 _][%00-00^T^ T][%00-00 , ][%00-00@(0.0.255) class][%00-00 _][%00-00^R^ R
][%00-00 >__][%00-00^`:`:FieldRelationCls^ FieldRelationCls][%00-00 <][%00-00^O^ O][%00-00 ,
 ][%00-00^T^ T][%00-00 , ][%00-00^R^ R][%00-00 >_][%00-00@0 FieldRelation][%00-00 (][%00-00^O^ O
][%00-00 _(T`::`*][%00-00@3 member][%00-00 ), ][%00-00@(0.0.255) const][%00-00 _][%00-00^R^ R][%00-00 `&
_][%00-00@3 relation][%00-00 )]&]
[s2; Creates ordering predicate for [* T] based on the value of member 
variable of[*  T].&]
[s4; [*C@4 T]-|Type of element.&]
[s4; [*C@3 member]-|Member variable of T.&]
[s4; [*C@3 relation]-|Ordering relation for [* member].&]
[s4; [*/ Return value]-|Ordering predicate.&]
[s0; &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 O][%00-00 , 
][%00-00@(0.0.255) class][%00-00 _][%00-00^T^ T][%00-00 , ][%00-00@(0.0.255) class][%00-00 _][%00-00^R^ R
][%00-00 >__][%00-00^`:`:MethodRelationCls^ MethodRelationCls][%00-00 <][%00-00@4 O][%00-00 _
(T`::`*)(), ][%00-00^T^ T][%00-00 , ][%00-00^R^ R][%00-00 >_][%00-00@0 MethodRelation][%00-00 (
][%00-00^O^ O][%00-00 _(T`::`*][%00-00@3 method][%00-00 )(), ][%00-00@(0.0.255) const][%00-00 _
][%00-00^R^ R][%00-00 `&_][%00-00@3 relation][%00-00 )]&]
[s2; Creates ordering predicate for [* T] based on the value returned 
by non`-const method of [* T].&]
[s4; [*C@4 T]-|Type of element.&]
[s4; [*C@3 method]-|Method of T.&]
[s4; [*C@3 relation]-|Ordering relation for value returned by method.&]
[s4; [*/ Return value]-|Ordering predicate.&]
[s0; &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 O][%00-00 , 
][%00-00@(0.0.255) class][%00-00 _][%00-00^T^ T][%00-00 , ][%00-00@(0.0.255) class][%00-00 _][%00-00^R^ R
][%00-00 >__][%00-00^`:`:MethodRelationCls^ MethodRelationCls][%00-00 <][%00-00@4 O][%00-00 _
(T`::`*)()_][%00-00@(0.0.255) const][%00-00 , ][%00-00^T^ T][%00-00 , 
][%00-00^R^ R][%00-00 >_][%00-00@0 MethodRelation][%00-00 (][%00-00^O^ O][%00-00 _(T`::`*][%00-00@3 m
ethod][%00-00 )()_][%00-00@(0.0.255) const][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00^R^ R
][%00-00 `&_][%00-00@3 relation][%00-00 )]&]
[s2; Creates ordering predicate for [* T] based on the value returned 
by const method of [* T].&]
[s4; [*C@4 T]-|Type of element.&]
[s4; [*C@3 method]-|Method of T.&]
[s4; [*C@3 relation]-|Ordering relation for value returned by method.&]
[s4; [*/ Return value]-|Ordering predicate.&]
[s0; ]