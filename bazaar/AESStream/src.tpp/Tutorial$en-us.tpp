topic "AES Encoder/Decoder Tutorial";
[2 $$0,0#00000000000000000000000000000000:Default]
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[H4;b83;*4 $$3,5#07864147445237544204411237157677:title]
[i288;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;2 $$5,5#45413000475342174754091244180557:text]
[l288;b17;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;C@5;1 $$7,7#20902679421464641399138805415013:code]
[b2503;2 $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base]
[C2 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam]
[b167;C2 $$13,13#92430459443460461911108080531343:item1]
[i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128)2 $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7;2 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line]
[l160;*C+117 $$18,5#88603949442205825958800053222425:package`-title]
[2 $$19,0#53580023442335529039900623488521:gap]
[C2 $$20,20#70211524482531209251820423858195:class`-nested]
[b50;2 $$21,21#03324558446220344731010354752573:Par]
[{_}%EN-US 
[s2; AES Encoder/Decoder Tutorial&]
[s5; These classes manage following tasks:&]
[s0;i150;O2; Maximum protection of personal data while keeping their 
byte representation in shared container (memory, disks, database, 
etc.)&]
[s0;i150;O2; Ability to work with huge amounts of data (> 4 GB) without 
using large memory consumption (using streaming) and without 
lowering overall security level&]
[s0; &]
[s0; As a basement, OpenSSL library and AES (Rijndael) algorithms 
were chosen.&]
[s0; To works with classes, [* you must have OpenSSL library configured 
and installed] under TheIDE (please [^topic`:`/`/AESStream`/src`/Installing`_OpenSSL`$en`-us^ l
ook here] to see how to do it).&]
[s0; &]
[s0; SHA`-2 functions reference are also included [^topic`:`/`/AESStream`/src`/Sha`$en`-us^ h
ere].&]
[s2; &]
[s2; 1. Encryption&]
[s5;#i150;O1; Creating cryptographically stable encryption key. Key 
length must be 16, 24 or 32 bytes long (128/192/256 bits accordingly). 
Keys with another length are not supported. Please note that 
some countries forbid by law using keys with certain lengths.&]
[s7; String key(AESRandomString(32));&]
[s5;l192; [*_ NB] Do not use user password as key. This will bring 
to nothing cryptographic strength.&]
[s5; &]
[s5;i150;O1; Creating AESEncoderStream class object to start encrypting.&]
[s7; AESEncoderStream aesEncoder(dataSize, key);&]
[s5;l192; Arguments are length of data being encoded and the key 
just created.&]
[s5;l192; [*_ NB] While encrypting data using steaming, first argument 
is the [* overall length] of all data.&]
[s5;l192; Object created is intended to encrypt single data stream. 
To encrypt another stream of data, please create another AESEncoderStream 
object.&]
[s5;#l192; If key is not supported by classes, AESEncoderStream constructor 
will throw an exception of (const char `*) type which will contain 
error description.&]
[s5; &]
[s5;i150;O1; Encrypting data. Putting original data into AESEncoderStream 
input:&]
[s7; String someData;&]
[s7; //...&]
[s7; aesEncoder << someData;&]
[s5;l192; And getting encrypted bytes from AESEncoderStream object:&]
[s7; sOut << aesEncoder;&]
[s5; &]
[s5;i150;O1; Streaming. AESEncoderStream class will receive any chunks 
of bytes until their overall length is equal to the overall data 
size for object (first argument in AESEncoderStream constructor). 
Each `"putting`" chunk of data into AESEncoderStream leads to 
encrypting it inside of object. And the memory consumption of 
this process matches the size of this chunk. So one may encrypt 
very big amounts of data dividing it into small chunks and using 
streamed encrytpion. Maximum supported stream length is MAX`_QWORD 
which is VERY large number.&]
[s7; for (...) &]
[s7; `{&]
[s7; -|/`*...`*/&]
[s7; -|aesEncoder << someDataXXX;&]
[s7; -|/`*...`*/&]
[s7; -|sOut << aesEncoder;&]
[s7; `}&]
[s0;l224; Each time one requests getting data from encoder, it puts 
out data which was not processed in the iterations before. One 
must also notice that encryption is made by internal chunks. 
This may lead to empty output data in some iterations of streamed 
encoding process. When object has enough input data, an outputs 
data chunks are generated.&]
[s0;l224; The structure of output data doesn`'t depend on the sizes 
and the number of input chunks. Decrypting data with right key 
will always give original stream of bytes.&]
[s5; &]
[s5; &]
[s2; 2. Decryption&]
[s5; AESDecoderStream is used for decryption. To decrypt data one 
must have right key. If one uses wrong key and starts decryption 
with AESDecoderStream, an exception of (const char `*) type is 
thrown. An exception of the same type may also be thrown in the 
constructor of during the decoding process if any serious problems 
occur.&]
[s5; &]
[s5;i150;O1; Creating AESDecoderStream object for decryption. AESDecoderStream 
object decrypts single data stream. To decrypt another stream 
of data please create another AESDecoderStream object.&]
[s7; AESDecoderStream aesDecoder(key);&]
[s5; &]
[s5;i150;O1; Decryption. Putting enctypted bytes into AESDecoderStream 
input:&]
[s7; aesDecoder << encryptedData;&]
[s5;l160; And getting original (decrypted) bytes from it:&]
[s7; sDecoded << aesDecoder;&]
[s5;l160; One must also handle (const char `*) exception if wrong 
key caused one.&]
[s5; &]
[s5;i150;O1; Streaming. Streaming is organized in the manner of AESEncoderStream 
streaming. Encrypted stream is divided into chunks and put into 
the AESDecoderStream object input. Decrypted chunks are taken 
from AESDecoderStream output. One should also consider specifics 
of decryption algorithms which may lead to empty output in some 
iterations of streamed decryption process. Output data chunk 
is generated when enough bytes are got from input.&]
[s5;l160; Determining original data size is more serious task when 
using streamed decryption. The truth is that one may determine 
original data size only after certain decryption stage when AESDecoderStream 
object has enough input bytes to decrypt the header. So GetSize() 
member function returns zero input data size until header is 
decrypted. For that reason one may check this size with precaution:&]
[s7; if (!aesDecoder.GetSize())&]
[s7; `{ //still no answer...&]
[s7; `}&]
[s7; else&]
[s7; `{//now I know original data size!&]
[s7; `}&]
[s5;l160; Once should also notice an exception is thrown (see above) 
on the stage (streamed decryption cycle iteration) when header 
is just decrypted and the original key is compared to the one 
that is set for AESDecoderStream. The original key is not contained 
in the encrypted data in any kind.&]
[s5;l160; Streamed decryption process is close to the streamed encryption:&]
[s7; for (...) &]
[s7; `{&]
[s7; -|/`*...`*/&]
[s7; -|aesDecoder << encryptedDataXXX;&]
[s7; -|/`*...`*/&]
[s7; -|sOut << aesDecoder;&]
[s7; `}&]
[s5; &]
[s5; &]
[s2; 3. Remarks&]
[s5;i150;O2; In the current version of package and OpenSSL, encrypted 
data size is bigger than the original one as 32 bytes with regard 
to 16`-byte alignment of original data. E.g., original data size 
is 170 bytes. The encrypted data size is calculated as:&]
[s5;l192; 170 aligned to 16 byte chunks `= 176&]
[s5;l192; `+&]
[s5;l192; 32 bytes&]
[s5;l192; `= 208 bytes.&]
[s5; &]
[s5; &]
[s2; 4. Known issues&]
[s0;i150;O0; OpenSSL fails to compile with gcc/MSYS under Windows 
as static library.&]
[s0; &]
[s0; If you have any information on issues or any comments, please 
feel free to write [^http`:`/`/www`.ultimatepp`.org`/forum`/index`.php`?t`=msg`&th`=4640`&start`=0`&^ h
ere].&]
[s0; ]