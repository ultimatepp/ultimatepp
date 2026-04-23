topic "Secure random data and nonce generation";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 Secure Random and Nonce Generators]]}}&]
[s2; &]
[s2;%% These functions provides a cryptographically secure random 
number and nonces compliant with NIST SP 800`-38D, tailored for 
high`-security applications that demand guaranteed uniqueness 
and strong collision resistance.  The implementation ensures 
process`-unique nonces and is fork`-safe on POSIX systems, automatically 
reseeding after a fork to avoid duplication. &]
[s2;%% &]
[s2;%% The implementation is [/ thread`-safe], supporting concurrent 
initialization and generation across threads without race conditions. 
It enforces a minimum nonce size of 12 bytes, aligning with cryptographic 
standards. &]
[s2;%% &]
[s2;%% The generator offers two distinct modes: one for producing 
unique, non`-repeating nonces,  and another for extracting purely 
random data suitable for general`-purpose cryptographic use.&]
[s2; &]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Function List]]}}&]
[s3; &]
[s5;:Upp`:`:SecureRandom`(int`): SecureBuffer<[@(0.255.0) byte]> [* SecureRandom]([@(0.0.255) i
nt] [*@3 n])&]
[s2;%% Generates [%-*@3 n] cryptographically secure random bytes. Enforces 
a minimum size of 1 bytes. Throws [^topic`:`/`/Core`/src`/Exc`_en`-us`#Exc`:`:class^ e
xception ]on failure.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SecureNonce`(int`): SecureBuffer<[@(0.255.0) byte]> [* SecureNonce]([@(0.0.255) i
nt] [*@3 n])&]
[s0;l288;%% Generates a secure nonce of [%-*@3 n] bytes. Enforces a 
minimum size of 12 bytes. Throws [^topic`:`/`/Core`/src`/Exc`_en`-us`#Exc`:`:class^ e
xception ]on failure. The returned value is a structured binary 
nonce produced from internal&]
[s2;%% domain, counter, and optional entropy components. All internal 
multi`-byte fields are encoded using little`-endian layout.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Standard Nonce Helpers]]}}&]
[s3; &]
[s5;:Upp`:`:GetAESGCMNonce`(`): SecureBuffer<[@(0.255.0) byte]> [* GetAESGCMNonce]()&]
[s5;:Upp`:`:GetChaChaPoly1305Nonce`(`): SecureBuffer<[@(0.255.0) byte]> 
[* GetChaChaPoly1305Nonce]()&]
[s5;:Upp`:`:GetTLSNonce`(`): SecureBuffer<[@(0.255.0) byte]> [* GetTLSNonce]()&]
[s5;:Upp`:`:GetAESCCMNonce`(`): SecureBuffer<[@(0.255.0) byte]> [* GetAESCCMNonce]()&]
[s5;:Upp`:`:GetJWTNonce`(`): SecureBuffer<[@(0.255.0) byte]> [* GetJWTNonce]()&]
[s5;:Upp`:`:GetOAuthNonce`(`): SecureBuffer<[@(0.255.0) byte]> [* GetOAuthNonce]()&]
[s5;:Upp`:`:GetOCSPNonce`(`): SecureBuffer<[@(0.255.0) byte]> [* GetOCSPNonce]()&]
[s5;:Upp`:`:GetECDSANonce`(`): SecureBuffer<[@(0.255.0) byte]> [* GetECDSANonce]()&]
[s5;:Upp`:`:GetDTLSCookie`(`): SecureBuffer<[@(0.255.0) byte]> [* GetDTLSCookie]()&]
[s2;%% These helper functions generate cryptographically secure nonces 
of commonly required lengths for widely used protocols and standards 
such as AES`-GCM, ChaCha20`-Poly1305, TLS, JWT, and ECDSA. Each 
helper ensures the nonce meets the expected size and uniqueness 
guarantees of its respective use case. Each helper throws [^topic`:`/`/Core`/src`/Exc`_en`-us`#Exc`:`:class^ e
xception ]on failure.&]
[s3; &]
[s0;%% ]]