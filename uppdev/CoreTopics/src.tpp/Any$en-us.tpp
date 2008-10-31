topic "Any";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a17;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[{_}%EN-US 
[s0; Any&]
[s0; &]
[s5;K%- [@(0.0.255) class][@(64) _]Any[@(64) _:_][@(0.0.255) private][@(64) _][^`:`:Moveable^@(64) M
oveable][@(64) <]Any[@(64) >_]&]
[s2; &]
[s0; Any is a special type of container capable of containing none 
or single element of [/ any] type. It also provides methods for 
querying the type stored and retrieving the content of specific 
type.&]
[s0; Any has pick semantics.&]
[s0; Members&]
[s0; &]
[s0;:`:`:Any`:`:Any`(pick`_`:`:Any`&`):%- [* Any](pick`__[* Any]`&_[*@3 s])&]
[s2; Pick constructor. Transfers content of source Any while destroying 
its content by picking.&]
[s7; [%-*C@3 s]-|Source Any.&]
[s0; &]
[s0;:Any`:`:Any`(`):%- [* Any]()&]
[s2; Constructs an empty Any.&]
[s0; &]
[s0;:`:`:Any`:`:`~Any`(`):%- `~[* Any]()&]
[s2; Destructor.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][^T^@(64) T][@(64) >__][^T^@(64) T][@(64) `&_]Create
[@(64) ()]&]
[s2; Creates content of type T inside Any.&]
[s7; [*C@4 T]-|Type of content.&]
[s7; [*/ Return value]-|Reference to the newly created content.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 T][@(64) >__][@(0.0.255) bool][@(64) _]Is[@(64) ()
_][@(0.0.255) const]&]
[s2; Tests whether Any contains content of type T.&]
[s7; [*C@4 T]-|Required type.&]
[s7; [*/ Return value]-|true if there is content with type T in Any.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][^T^@(64) T][@(64) >__][^T^@(64) T][@(64) `&_]Get[@(64) (
)]&]
[s2; Returns reference to content. Is<T> must be true, otherwise 
this operation is illegal.&]
[s7; [*C@4 T]-|Required type.&]
[s7; [*/ Return value]-|Reference to content.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][^T^@(64) T][@(64) >__][@(0.0.255) const][@(64) _][^T^@(64) T
][@(64) `&_]Get[@(64) ()_][@(0.0.255) const]&]
[s2; Returns constant reference to content. Is<T> must be true, otherwise 
this operation is illegal.&]
[s7; [*C@4 T]-|Required type.&]
[s7; [*/ Return value]-|Reference to content.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]Clear[@(64) ()]&]
[s2; Removes (and destroys) content.&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]IsEmpty[@(64) ()_][@(0.0.255) const]&]
[s7; [*/ Return value]-|true if there is no content.&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]IsPicked[@(64) ()_][@(0.0.255) const]&]
[s7; [*/ Return value]-|true if Any is picked.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]operator`=[@(64) (pick`__][^`:`:Any^@(64) Any][@(64) `&_][@3 s][@(64) )
]&]
[s2; Pick operator. Transfers content while destroying source.&]
[s7; [%-*C@3 s]-|Source Any.&]
[s0; ]