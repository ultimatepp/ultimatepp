topic "Template Algorithms For Sorted Sets";
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
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 Template Algorithms For Sorted Sets]]}}&]
[s3;%- &]
[s5;:AppendSorted`(C`&`,const C`&`,const L`&`):%- [@(0.0.255) template]_<[@(0.0.255) clas
s]_[*@4 C], [@(0.0.255) class]_[*@4 L][@(0.0.255) >]_[*@4 C][@(0.0.255) `&]_[* AppendSorted]([*@4 C
][@(0.0.255) `&]_[*@3 dest], [@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 src], 
[@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 less])&]
[s2; Merges source NTL container to destination NTL container. Both 
containers must be sorted in ascending order. After the operation, 
destination container is sorted in ascending order and contains 
values from both containers. Duplicate values are preserved. 
Ordering predicate is determined by [* less].&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@3 dest]-|Destination container.&]
[s7; [*C@3 src]-|Source container.&]
[s7; [*C@3 less]-|Ordering predicate.&]
[s7; [*/ Return value]-|Destination container.[C template<class ][*C@4 C][C >]&]
[s3;%- &]
[s4;%- &]
[s5;:AppendSorted`(C`&`,const C`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 C][@(0.0.255) >
]_[*@4 C][@(0.0.255) `&]_[* AppendSorted]([*@4 C][@(0.0.255) `&]_[*@3 dest], 
[@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 src])&]
[s2;~~~.992; Merges source NTL container to destination NTL container. 
Both containers must be sorted in ascending order. After the 
operation, destination container is sorted in ascending order 
and contains values from both containers. Duplicate values are 
preserved. Ordering is determined by [*/ operator<].&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@3 dest]-|Destination container.&]
[s7; [*C@3 src]-|Source container.&]
[s7; [*/ Return value]-|Destination container.[C template<class ][*C@4 C][C >]&]
[s3;%- &]
[s4;%- &]
[s5;:UnionSorted`(C`&`,const C`&`,const L`&`):%- [@(0.0.255) template]_<[@(0.0.255) class
]_[*@4 C], [@(0.0.255) class]_[*@4 L][@(0.0.255) >]_[*@4 C][@(0.0.255) `&]_[* UnionSorted]([*@4 C
][@(0.0.255) `&]_[*@3 dest], [@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 src], 
[@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 less])&]
[s2; Merges source NTL container to destination NTL container. Both 
containers must be sorted in ascending order and values must 
be unique. After the operation, destination container is sorted 
in ascending order and contains unique values from both containers. 
Ordering is determined by [* less].&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@3 dest]-|Destination container.&]
[s7; [*C@3 src]-|Source container.&]
[s7; [*C@3 less]-|Ordering predicate.&]
[s7; [*/ Return value]-|Destination container.&]
[s3;%- &]
[s4;%- &]
[s5;:UnionSorted`(C`&`,const C`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 C][@(0.0.255) >
]_[*@4 C][@(0.0.255) `&]_[* UnionSorted]([*@4 C][@(0.0.255) `&]_[*@3 dest], 
[@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 src])&]
[s2; Merges the source NTL container to the destination NTL container. 
Both containers must be sorted in ascending order and values 
must be unique. After the operation, destination container is 
sorted in ascending order and contains unique values from both 
containers. Ordering is determined by [*/ operator<].&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@3 dest]-|Destination container.&]
[s7; [*C@3 src]-|Source container.&]
[s7; [*/ Return value]-|Destination container.&]
[s3;%- &]
[s4;%- &]
[s5;:RemoveSorted`(C`&`,const C`&`,const L`&`):%- [@(0.0.255) template]_<[@(0.0.255) clas
s]_[*@4 C], [@(0.0.255) class]_[*@4 L][@(0.0.255) >]_[*@4 C][@(0.0.255) `&]_[* RemoveSorted]([*@4 C
][@(0.0.255) `&]_[*@3 from], [@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 what], 
[@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 less])&]
[s2; Removes elements of source container from destination container. 
Both containers must be sorted in ascending order and values 
must be unique. Ordering is determined by [* less].&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@3 from]-|Destination container.&]
[s7; [*C@3 what]-|Source container.&]
[s7; [*C@3 less]-|Ordering predicate.&]
[s7; [*/ Return value]-|Destination container.&]
[s3;%- &]
[s4;%- &]
[s5;:RemoveSorted`(C`&`,const C`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 C][@(0.0.255) >
]_[*@4 C][@(0.0.255) `&]_[* RemoveSorted]([*@4 C][@(0.0.255) `&]_[*@3 from], 
[@(0.0.255) const]_[*@4 C][@(0.0.255) `&]_[*@3 what])&]
[s2; Removes elements of source container from destination container. 
Both containers must be sorted in ascending order and values 
must be unique. Ordering is determined by [*/ operator<].&]
[s7; [*C@4 C]-|Type of container.&]
[s7; [*C@3 from]-|Destination container.&]
[s7; [*C@3 what]-|Source container.&]
[s7; [*/ Return value]-|Destination container.&]
[s3;%- &]
[s4;%- &]
[s5;:IntersectSorted`(D`&`,const S`&`,const L`&`):%- [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 D], [@(0.0.255) class]_[*@4 S], [@(0.0.255) class]_[*@4 L][@(0.0.255) >]_[*@4 D][@(0.0.255) `&
]_[* IntersectSorted]([*@4 D][@(0.0.255) `&]_[*@3 dest], [@(0.0.255) const]_[*@4 S][@(0.0.255) `&
]_[*@3 src], [@(0.0.255) const]_[*@4 L][@(0.0.255) `&]_[*@3 less])&]
[s2; Removes elements from destination container that are not contained 
in source container. Both containers must be sorted in ascending 
order and values must be unique. Ordering is determined by [* less].&]
[s7; [*C@4 D]-|Type of destination container.&]
[s7; [*C@4 S]-|Type of source container.&]
[s7; [*C@3 dest]-|Destination container.&]
[s7; [*C@3 src]-|Source container.&]
[s7; [*C@3 less]-|Destination container.&]
[s3;%- &]
[s4;%- &]
[s5;:IntersectSorted`(D`&`,const S`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 D],
 [@(0.0.255) class]_[*@4 S][@(0.0.255) >]_[*@4 D][@(0.0.255) `&]_[* IntersectSorted]([*@4 D][@(0.0.255) `&
]_[*@3 dest], [@(0.0.255) const]_[*@4 S][@(0.0.255) `&]_[*@3 src])&]
[s2; Removes elements from destination container that are not contained 
in source container. Both containers must be sorted in ascending 
order and values must be unique. Ordering is determined by [*/ operator<].&]
[s7; [*C@4 D]-|Type of destination container.&]
[s7; [*C@4 S]-|Type of source container.&]
[s7; [*C@3 dest]-|Destination container.&]
[s7; [*C@3 src]-|Source container.&]
[s7; [*/ Return value]-|Destination container.&]
[s0; ]]