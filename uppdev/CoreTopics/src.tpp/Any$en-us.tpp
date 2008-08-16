topic "Any";[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[l288;a17;*1 $$7,7#70004532496200323422659154056402:requirement]
[{_}%EN-US 
[s3; Any&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) class][%00-00 _][%00-00@0 Any][%00-00 _:_][%00-00@(0.0.255) private][%00-00 _
][%00-00^`:`:Moveable^ Moveable][%00-00 <][%00-00@0 Any][%00-00 >_]&]
[s2; &]
[s0; Any is a special type of container capable of containing none 
or single element of [/ any] type. It also provides methods for 
querying the type stored and retrieving the content of specific 
type.&]
[s0; Any has pick semantics.&]
[s0; Members&]
[s0;3 &]
[s0;:`:`:Any`:`:Any`(pick`_`:`:Any`&`): [%00-00* Any][%00-00 (pick`__][%00-00* Any][%00-00 `&
_][%00-00*@3 s][%00-00 )]&]
[s2; Pick constructor. Transfers content of source Any while destroying 
its content by picking.&]
[s4; [%00-00*C@3 s]-|Source Any.&]
[s0;3 &]
[s0;:`:`:Any`:`:Any`(`): [%00-00* Any][%00-00 ()]&]
[s2; Constructs an empty Any.&]
[s0;3 &]
[s0;:`:`:Any`:`:`~Any`(`): [%00-00 `~][%00-00* Any][%00-00 ()]&]
[s2; Destructor.&]
[s0;3 &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00^T^ T][%00-00 >__][%00-00^T^ T][%00-00 `&
_][%00-00@0 Create][%00-00 ()]&]
[s2; Creates content of type T inside Any.&]
[s4; [*C@4 T]-|Type of content.&]
[s4; [*/ Return value]-|Reference to the newly created content.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00@(64) template_<][%00-00 class][%00-00@(64) _][%00-00@4 T][%00-00@(64) >
__][%00-00 bool][%00-00@(64) _][%00-00@0 Is][%00-00@(64) ()_][%00-00 const]&]
[s2; Tests whether Any contains content of type T.&]
[s4; [*C@4 T]-|Required type.&]
[s4; [*/ Return value]-|true if there is content with type T in Any.&]
[s0;3 &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00^T^ T][%00-00 >__][%00-00^T^ T][%00-00 `&
_][%00-00@0 Get][%00-00 ()]&]
[s2; Returns reference to content. Is<T> must be true, otherwise 
this operation is illegal.&]
[s4; [*C@4 T]-|Required type.&]
[s4; [*/ Return value]-|Reference to content.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00@(64) template_<][%00-00 class][%00-00@(64) _][%00-00^T^@(64) T][%00-00@(64) >
__][%00-00 const][%00-00@(64) _][%00-00^T^@(64) T][%00-00@(64) `&_][%00-00@0 Get][%00-00@(64) (
)_][%00-00 const]&]
[s2; Returns constant reference to content. Is<T> must be true, otherwise 
this operation is illegal.&]
[s4; [*C@4 T]-|Required type.&]
[s4; [*/ Return value]-|Reference to content.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 Clear][%00-00 ()]&]
[s2; Removes (and destroys) content.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00 bool][%00-00@(64) _][%00-00@0 IsEmpty][%00-00@(64) ()_][%00-00 const]&]
[s4; [*/ Return value]-|true if there is no content.&]
[s0;3 &]
[s5;K@(0.0.255) [%00-00 bool][%00-00@(64) _][%00-00@0 IsPicked][%00-00@(64) ()_][%00-00 const]&]
[s4; [*/ Return value]-|true if Any is picked.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 operator`=][%00-00 (pick`__][%00-00^`:`:Any^ A
ny][%00-00 `&_][%00-00@3 s][%00-00 )]&]
[s2; Pick operator. Transfers content while destroying source.&]
[s4; [%00-00*C@3 s]-|Source Any.&]
[s0; ]