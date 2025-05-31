topic "SecureBuffer";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 SecureBuffer]]}}&]
[s3; &]
[s1;:noref: [@(0.0.255)3 template][3  ]<[@(0.0.255) class]_[*@4 T][@(0.0.255) >]&]
[s1;:Upp`:`:SecureBuffer: [@(0.0.255) class ][* SecureBuffer ][*@(0.0.255) :][*  
][*@3 Moveable][*@(0.0.255) <][* _SecureBuffer][*@(0.0.255) <][*@4 T][*@(0.0.255) >][* _>_, 
NoCopy]&]
[s6; [@4 T] must be trivially copyable and destructible.&]
[s0;l288;%% A dynamic buffer designed for storing sensitive cryptographic 
data such as private keys, symmetric keys, passwords, certificates, 
nonces, and other security`-critical material. SecureBuffer attempts 
to lock its memory region in RAM (using mlock/VirtualLock) to 
prevent it from being swapped to disk. This locking is best`-effort 
and [/ may ]fail on systems with limited permissions or resource 
limits. Regardless of locking success, the buffer is securely 
zeroed before deallocation to reduce the risk of sensitive data 
lingering in memory. SecureBuffer is [/ not ]thread safe. External 
synchronization is required for concurrent access.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s3; &]
[s5;:Upp`:`:SecureBuffer`:`:SecureBuffer`(`): [* SecureBuffer]()&]
[s2;%% Default constructor. Creates an empty secure buffer.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SecureBuffer`:`:SecureBuffer`(size`_t`): [* SecureBuffer](size`_t 
[*@3 size])&]
[s2;%% Constructs a buffer of given [%-*@3 size], allocates memory 
and locks it.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SecureBuffer`:`:SecureBuffer`(SecureBuffer`&`&`): [* SecureBuffer]([* SecureB
uffer][@(0.0.255) `&`&] [*@3 src])&]
[s2;%% Pick constructor. Destroys source container [%-*@3 src].&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SecureBuffer`:`:`~SecureBuffer`(`): [* `~SecureBuffer]()&]
[s2;%% Destructor. Frees memory, securely zeroes the content, and 
unlocks it from physical memory.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:SecureBuffer`:`:operator`=`(SecureBuffer`&`&`): SecureBuffer[@(0.0.255) `&] 
operator[@(0.0.255) `=]([* SecureBuffer][@(0.0.255) `&`&] [*@3 src])&]
[s2;%% Pick operator. Destroys source buffer [%-*@3 src].&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SecureBuffer`:`:Alloc`(size`_t`): [@(0.0.255) void] [* Alloc](size`_t 
[*@3 size])&]
[s2;%% Clears existing buffer and allocates a new one of specified 
[%-*@3 size].&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SecureBuffer`:`:Clear`(`): [@(0.0.255) void] [* Clear]()&]
[s2;%% Releases the buffer and securely zeroes its contents.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SecureBuffer`:`:Zero`(`): [@(0.0.255) void] [* Zero]()&]
[s2;%% Explicitly zeroes the contents of the buffer in a secure way. 
Doesn`'t release or destroy the buffer.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SecureBuffer`:`:GetSize`(`)const: size`_t [* GetSize]() 
[@(0.0.255) const]&]
[s2;%% Returns the size of buffer. Return 0 if the buffer is empty.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SecureBuffer`:`:IsEmpty`(`)const: [@(0.0.255) bool] [* IsEmpty]() 
[@(0.0.255) const]&]
[s2;%% Returns true if the buffer is empty.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SecureBuffer`:`:operator T`*`(`): operator T [@(0.0.255) `*]()&]
[s5;:Upp`:`:SecureBuffer`:`:operator const T`*`(`)const: operator 
[@(0.0.255) const] T [@(0.0.255) `*]() [@(0.0.255) const]&]
[s5;:Upp`:`:SecureBuffer`:`:operator`~`(`): T [@(0.0.255) `*]operator[@(0.0.255) `~]()&]
[s5;:Upp`:`:SecureBuffer`:`:operator`~`(`)const: [@(0.0.255) const] 
T [@(0.0.255) `*]operator[@(0.0.255) `~]() [@(0.0.255) const]&]
[s5;:Upp`:`:SecureBuffer`:`:Get`(`): T [@(0.0.255) `*][* Get]()&]
[s5;:Upp`:`:SecureBuffer`:`:Get`(`)const: [@(0.0.255) const] T [@(0.0.255) `*][* Get]() 
[@(0.0.255) const]&]
[s5;:Upp`:`:SecureBuffer`:`:begin`(`): T [@(0.0.255) `*][* begin]()&]
[s5;:Upp`:`:SecureBuffer`:`:begin`(`)const: [@(0.0.255) const] T [@(0.0.255) `*][* begin]()
 [@(0.0.255) const]&]
[s5;:Upp`:`:SecureBuffer`:`:Begin`(`): T [@(0.0.255) `*][* Begin]()&]
[s5;:Upp`:`:SecureBuffer`:`:Begin`(`)const: [@(0.0.255) const] T [@(0.0.255) `*][* Begin]()
 [@(0.0.255) const]&]
[s2;%% Returns a pointer to the first element of the buffer or [@(0.0.255) nullptr 
]if the buffer is empty.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SecureBuffer`:`:end`(`): T [@(0.0.255) `*][* end]()&]
[s5;:Upp`:`:SecureBuffer`:`:end`(`)const: [@(0.0.255) const] T [@(0.0.255) `*][* end]() 
[@(0.0.255) const]&]
[s5;:Upp`:`:SecureBuffer`:`:End`(`): T [@(0.0.255) `*][* End]()&]
[s5;:Upp`:`:SecureBuffer`:`:End`(`)const: [@(0.0.255) const] T [@(0.0.255) `*][* End]() 
[@(0.0.255) const]&]
[s2;%% Returns a pointer to the last element of the buffer or [@(0.0.255) nullptr 
]if the buffer is empty.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SecureBuffer`:`:operator`[`]`(size`_t`): T[@(0.0.255) `&] 
operator[@(0.0.255) `[`]](size`_t i)&]
[s5;:Upp`:`:SecureBuffer`:`:operator`[`]`(size`_t`)const: [@(0.0.255) const] 
T[@(0.0.255) `&] operator[@(0.0.255) `[`]](size`_t i) [@(0.0.255) const]&]
[s2;%% Provides indexed access to elements. Checks bounds (and asserts) 
in DEBUG mode.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Function List]]}}&]
[s3; &]
[s5;:Upp`:`:SecureZero`(T`*`,size`_t`): [@(0.0.255) template] <[@(0.0.255) class] 
T>&]
[s5;:Upp`:`:SecureZero`(T`*`,size`_t`): [@(0.0.255) void] [* SecureZero](T 
[@(0.0.255) `*][*@3 ptr], size`_t [*@3 count])&]
[s0;:Upp`:`:SecureZero`(T`& obj`): [@(0.0.255) void] [* SecureZero](T[@(0.0.255) `&] 
[*@3 obj])&]
[s6; [@4 T] must be trivially copyable and destructible.&]
[s2;%% A secure memory zeroing function used internally by SecureBuffer. 
This function overwrites memory contents in a way that is [/ not] 
optimized away by the compiler.&]
[s0;%% ]]