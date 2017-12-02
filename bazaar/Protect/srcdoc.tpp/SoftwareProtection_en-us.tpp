topic "About Software Protection methods";
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s0;%- [*R6 About Software Protection methods]&]
[s0;/%- &]
[s0;%- There are many ways to [_ try ]to protect your software from 
piracy... none of them is 100% effective.&]
[s0;%- Most protection schemes are usually composed by following 
parts :&]
[s0;%- &]
[s0;i150;O0;%- Check if user has a license to run the software&]
[s0;l160;%- This point can be accomplished by various means, like 
dongles, software licenses tied to one particular machine, lan/internet 
activation and original media checking.&]
[s0;%- &]
[s0;i150;O0;%- Make difficult for malicious user to defeat the protection 
scheme.&]
[s0;l160;%- This point can be achieved by various means, from simple 
code obfuscation, which makes debugging/disassembly difficult, 
up to code encryption, which makes disassembly virtually impossible 
if user hasn`'t a program license.&]
[s0;%- &]
[s0; First step is the easiest to achieve, and its security degree 
depends almost only on authentication media chosen.&]
[s0; In my opinion, a good dongle or, even better, an internet authentication 
method are the most secure; in particular, internet auth, which 
has the obvious caveat of the need of a connection available 
to run the application, can be the most flexible one.&]
[s0; &]
[s0; Second step presents most challenges.&]
[s0; If it`'s quite easy to make it impossible for an user to unlock 
a protected software without having a correct license, it`'s 
virtually impossible to have 100% protection against a malicious 
user with a software license; every software that can be run 
on a computer can also be copied.&]
[s0; &]
[s0; The solution proposed here doesn`'t want to be a commercial 
grade protection scheme; its purpose is just to make it impossible 
for an user [_ without ]a license to run the software, even if 
he has good skills in software debugging, and make is [_ somehow 
]difficult for an user in possession of a license to defeat the 
protection scheme.&]
[s0; This is accomplished by encryption of some software parts and 
on`-the`-fly decryption with a license key obtained in any usable 
way.&]
[s0; Beware that a mid`-skilled programmer in possession of a right 
license key can defeat the algorithm in short time, as the decryption 
process is quite trivial and placed exactly before the encrypted 
software parts; it can anyways make the process of reverse`-engineering 
not worth the time spent for it.&]
[s0; &]
[s0; [^topic`:`/`/Protect`/srcdoc`/HowItWorks`$en`-us^ How it works]&]
[s0; ]