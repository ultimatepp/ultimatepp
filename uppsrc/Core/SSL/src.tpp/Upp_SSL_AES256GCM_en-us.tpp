topic "AES256 Encryption & Decryption";
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
[ {{10000@(113.42.0) [s0; [*@7;4 Aes256Gcm]]}}&]
[s1;%- &]
[s1;:Upp`:`:Aes256Gcm:%- [*3 Aes256Gcm][3  ][@(0.0.255)3 :][3  NoCopy]&]
[s0;l288; Provides AES`-256`-GCM authenticated encryption and decryption 
with OpenSSL 3.0 or later.&]
[s2; This class supports both stream`-based and in`-memory encryption, 
deriving a strong key from a password using PBKDF2`-HMAC`-SHA256. 
Intended for secure, verifiable data encryption using modern 
cryptographic primitives. This class is non`-copyable.&]
[s2; &]
[s2; [* Format]&]
[s2;* &]
[s2; Encrypted data is stored in a binary envelope with the following 
layout:&]
[s2; &]
[s0;=l288; [C `[GCMv1`_`_`]`[SALT`]`[IV`]`[CIPHERTEXT`]`[TAG`]]&]
[s0;=l288;C &]
[s0;l288;i150;O0; [C GCMv1`_`_]: 7`-byte version identifier (AES`_GCM`_FORMAT`_PREFIX).&]
[s0;l288;i150;O0; [C SALT]: 16`-byte random salt for PBKDF2.&]
[s0;l288;i150;O0; [C IV]: 12`-byte random initialization vector for 
AES`-GCM.&]
[s0;l288;i150;O0; [C CIPHERTEXT]: AES`-GCM encrypted payload.&]
[s0;l288;i150;O0; [C TAG]: 16`-byte authentication tag appended at the 
end.&]
[s0; &]
[s2; The version prefix allows future format changes without ambiguity. 
Authentication is enforced via the GCM tag; tampering or wrong 
passwords result in decryption failure.&]
[s2; &]
[s2; [* Thread Safety]&]
[s2; &]
[s0;l288;i150;O0; Instances of Aes256Gcm are [/_ not thread`-safe].&]
[s2;i150;O0; Use separate instances for parallel encryption/decryption.&]
[s2; &]
[s2; [* Performance Considerations]&]
[s2; &]
[s0;l288;i150;O0; Key derivation (PBKDF2) is intentionally CPU`-expensive 
to resist brute`-force attacks. The default iteration count balances 
security and responsiveness.&]
[s2;i150;O0; Use [^topic`:`/`/Core`/SSL`/src`/Upp`_SSL`_AES256GCM`_en`-us`#Upp`:`:Aes256Gcm`:`:Iteration`(int`)^ I
teration()] method to configure the number of PBKDF2 rounds.&]
[s2;i150;O0; [^topic`:`/`/Core`/SSL`/src`/Upp`_SSL`_AES256GCM`_en`-us`#Upp`:`:Aes256Gcm`:`:Chunksize`(int`)^ C
hunksize()] method can be used to control the size of internal 
I/O buffers for streaming scenarios.&]
[s2; &]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor detail]]}}&]
[s3;%- &]
[s5;:Upp`:`:Aes256Gcm`:`:Aes256Gcm`(`):%- [* Aes256Gcm]()&]
[s2; Default constructor. Initializes context and AES`-256`-GCM cipher.&]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0;%- Constants]}}&]
[s3;%- &]
[s5;:Upp`:`:AES`_GCM`_DEFAULT`_ITERATION:%- [@(0.0.255) constexpr] 
[@(0.0.255) int] [* AES`_GCM`_DEFAULT`_ITERATION]&]
[s2; Default PBKDF2 iteration count (100000).&]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:Upp`:`:Aes256Gcm`:`:Iteration`(int`):%- Aes256Gcm[@(0.0.255) `&] 
[* Iteration]([@(0.0.255) int] [*@3 n])&]
[s2; Sets the PBKDF2 iteration count. The value is clamped to a safe 
range. Default is implementation`-defined. Returns `*this for 
method chaining.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Aes256Gcm`:`:ChunkSize`(int`):%- Aes256Gcm[@(0.0.255) `&] 
[* ChunkSize]([@(0.0.255) int] [*@3 sz])&]
[s2; Sets the processing chunk size (for streaming I/O) to [%-*@3 sz]. 
Must be at least 128 bytes. Returns `*this for method chaining.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Aes256Gcm`:`:Encrypt`(Stream`&`,const String`&`,Stream`&`):%- [@(0.0.255) b
ool] [* Encrypt](Stream[@(0.0.255) `&] [*@3 in], [@(0.0.255) const] String[@(0.0.255) `&] 
[*@3 password], Stream[@(0.0.255) `&] [*@3 out])&]
[s2; Encrypts data from an input stream  [%-*@3 in ]using the provided 
[%-*@3 password] . Writes the result to the output stream [%-*@3 out]. 
Returns true on success. On failure the error description can 
be get using [^topic`:`/`/Core`/SSL`/src`/Upp`_SSL`_AES256GCM`_en`-us`#Upp`:`:Aes256Gcm`:`:GetErrorDesc`(`)const^ G
etErrorDesc()] method.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Aes256Gcm`:`:Encrypt`(const String`&`,const String`&`,String`&`):%- [@(0.0.255) b
ool] [* Encrypt]([@(0.0.255) const ]String[@(0.0.255) `&] [*@3 in], [@(0.0.255) const] 
String[@(0.0.255) `&] [*@3 password], String[@(0.0.255) `&] [*@3 out])&]
[s2; Encrypts the input string [%-*@3 in]. Writes the result to [%-*@3 out]. 
Returns true on success. On failure the error description can 
be get using [^topic`:`/`/Core`/SSL`/src`/Upp`_SSL`_AES256GCM`_en`-us`#Upp`:`:Aes256Gcm`:`:GetErrorDesc`(`)const^ G
etErrorDesc()] method.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Aes256Gcm`:`:Decrypt`(Stream`&`,const String`&`,Stream`&`):%- [@(0.0.255) b
ool] [* Decrypt](Stream[@(0.0.255) `&] [*@3 in], [@(0.0.255) const] String[@(0.0.255) `&] 
[*@3 password], Stream[@(0.0.255) `&] [*@3 out])&]
[s2; Decrypts data from an input stream  [%-*@3 in ]using the provided 
[%-*@3 password] . Writes the result to the output stream [%-*@3 out]. 
Returns true on success. On failure the error description can 
be get using [^topic`:`/`/Core`/SSL`/src`/Upp`_SSL`_AES256GCM`_en`-us`#Upp`:`:Aes256Gcm`:`:GetErrorDesc`(`)const^ G
etErrorDesc()] method.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Aes256Gcm`:`:Decrypt`(const String`&`,const String`&`,String`&`):%- [@(0.0.255) b
ool] [* Decrypt]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 in], [@(0.0.255) const] 
String[@(0.0.255) `&] [*@3 password], String[@(0.0.255) `&] [*@3 out])&]
[s2; Decrypts the input string [%-*@3 in]. Writes the result to [%-*@3 out]. 
Returns true on success. On failure the error description can 
be get using [^topic`:`/`/Core`/SSL`/src`/Upp`_SSL`_AES256GCM`_en`-us`#Upp`:`:Aes256Gcm`:`:GetErrorDesc`(`)const^ G
etErrorDesc()] method.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Aes256Gcm`:`:WhenProgress:%- Gate<int64, int64> [* WhenProgress]&]
[s2; Optional event for progress tracking. Called with (processed, 
total) values during streaming encryption/decryption. Returning 
true will abort the operation.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Aes256Gcm`:`:GetErrorDesc`(`)const:%- String [* GetErrorDesc]() 
[@(0.0.255) const]&]
[s2; Returns the last error message as a string or empty if no error 
occured.&]
[s3;%- &]
[s0;i448;a25;kKO9;@(0.0.255)%- &]
[ {{10000F(128)G(128)@1 [s0; [* Function List]]}}&]
[s3;%- &]
[s5;:Upp`:`:AES256Encrypt`(const String`&`,const String`&`,Gate`):%- String 
[* AES256Encrypt]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 in], 
[@(0.0.255) const] String[@(0.0.255) `&] [*@3 password], Gate<int64, 
int64> WhenProgress [@(0.0.255) `=] [*@3 Null])&]
[s2;%- [%% Encrypts the input string using AES`-256`-GCM and returns 
the result. Returns a void string on failure. ][*@3 password ][%% can 
be empty or null. ]WhenProgress event can be used to track or 
abort the process.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:AES256Decrypt`(const String`&`,const String`&`,Gate`):%- String 
[* AES256Decrypt]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 in], 
[@(0.0.255) const] String[@(0.0.255) `&] [*@3 password], Gate<int64, 
int64> WhenProgress [@(0.0.255) `=] [*@3 Null])&]
[s2;%- [%% Decrypts the AES`-256`-GCM encrypted input string and returns 
the result. Returns a void string on failure. ][*@3 password ][%% can 
be empty or null. ]WhenProgress event can be used to track or 
abort the process.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:AES256Encrypt`(Stream`&`,const String`&`,Stream`&`,Gate`):%- [@(0.0.255) bo
ol] [* AES256Encrypt](Stream[@(0.0.255) `&] [*@3 in], [@(0.0.255) const] 
String[@(0.0.255) `&] [*@3 password], Stream[@(0.0.255) `&] [*@3 out], 
Gate<int64, int64> WhenProgress [@(0.0.255) `=] [*@3 Null])&]
[s2;%- [%% Encrypts the input stream with AES`-256`-GCM and writes 
the encrypted chunks into ][*@3 out][%%  stream. Returns true on 
success. ][*@3 password ][%% can be empty or null. ]WhenProgress 
event can be used to track or abort the process.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:AES256Decrypt`(Stream`&`,const String`&`,Stream`&`,Gate`):%- [@(0.0.255) bo
ol] [* AES256Decrypt](Stream[@(0.0.255) `&] [*@3 in], [@(0.0.255) const] 
String[@(0.0.255) `&] [*@3 password], Stream[@(0.0.255) `&] [*@3 out], 
Gate<int64, int64> WhenProgress [@(0.0.255) `=] [*@3 Null])&]
[s2;%- [%% Decrypts the input stream encrypted with AES`-256`-GCM and 
writes the decrypted chunks into ][*@3 out][%%  stream. Returns true 
on success. ][*@3 password ][%% can be empty or null. ]WhenProgress 
event can be used to track or abort the process.&]
[s3;%- &]
[s0; ]]