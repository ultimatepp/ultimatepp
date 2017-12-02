topic "NTL vs STL";
[2 $$0,0#00000000000000000000000000000000:Default]
[l288;i704;a17;O9;~~~.992;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[b83;*2 $$3,5#07864147445237544204411237157677:title]
[b167;a42;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;2 $$5,5#45413000475342174754091244180557:text]
[l288;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;t246;C@5;1 $$7,7#20902679421464641399138805415013:code]
[b2503;2 $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base]
[t4167;C2 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam]
[b167;C2 $$13,13#92430459443460461911108080531343:item1]
[i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128)2 $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7;2 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line]
[l160;t4167;*C+117 $$18,5#88603949442205825958800053222425:package`-title]
[{_}%EN-US 
[s2; NTL `- STL Comparison&]
[s5; NTL was created to solve following problems we used to encounter 
when using STL: &]
[s3; Copy`-constructible requirements&]
[s5; STL requires the elements stored in containers to meet the requirements 
of copy`-constructible and assignable types. This causes two 
problems:&]
[s5;i150;O2; Elements that do not satisfy these requirements cannot 
be directly stored in STL containers.&]
[s5;i150;O2; For many types of elements, including STL containers 
themselves, copy`-constructor and assign operator is a very expensive 
operation, that is why often they cannot be stored in STL containers 
effectively.&]
[s5; NTL addresses this problem by introducing [^topic`:`/`/Core`/src`/Vector`$en`-us^ V
ector] and [^topic`:`/`/Core`/src`/Array`$en`-us^ Array] flavors 
of containers.&]
[s3; Value transfer&]
[s5; STL provides deep copy of containers as default operation for 
(most) `'`=`' cases, with `&`& based move semantics for specific 
or explicit cases (std`::move). NTL requires to specify the mode 
of transfer with `'pick`' (corresponds to std`::move) or `'clone`' 
functions in all cases, except the assignment of temporary value 
(where the final semantics is the same).&]
[s3; Random access and random access notation&]
[s5; STL uses iterators as the preferred way how to access and identify 
elements in containers. While this is generally the most generic 
way, real`-life problems often require or at least benefit from 
random access using indices. NTL provides fast random access 
to all kinds of containers and NTL interfaces prefer notation 
using indices. As a side effect, NTL user can completely avoid 
using iterators in favor of indices, which in current C`+`+ results 
in much simpler and less verbose syntax (using modern optimizing 
compilers there is no difference in performance).&]
[s3; Index&]
[s5; A completely new type of associative container, [^topic`:`/`/Core`/src`/Indext`$en`-us^ I
ndex], is introduced, as an ideal building block for all kinds 
of associative operations.&]
[s5; [* InVector, Sorted maps]&]
[s5; NTL provides random access containers with fast insertion at 
arbitrary position, which scale well to milions of items. These 
containers then provide basis for `'sorted maps`' that are using 
binary search.&]
[s3; Algorithm requirements&]
[s5; Requirements of STL algorithms are very loosely defined. NTL 
tries to provide more specific requirements and also minimal 
ones. This allows e.g. direct sorting of Array of polymorphic 
elements.&]
[s3; Minor improvements&]
[s5; There are also some minor improvements:&]
[s5;i150;O2; Besides [* reserve] present in STL, NTL provides also 
[* Shrink] method to minimize a container`'s memory consumption.]]