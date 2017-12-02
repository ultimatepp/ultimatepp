topic "Code Obfuscation";
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s0;%- [*R6 Code Obfuscation]&]
[s0;/%- &]
[s0; As you can easily understand, the [^topic`:`/`/Protect`/srcdoc`/HowItWorks`$en`-us^ e
ncryption method proposed earlier] has 2 flaws :&]
[s0; &]
[s0;i150;O0; The decrypting code is visible and traceable&]
[s0; &]
[s0;i150;O0; You must obtain the key [*/ before ]the decryption phase&]
[s0; &]
[s0; The first point is not so important: even if that`'s true that 
user can step the decryptor and get the real code, he must do 
it for every function you did protect, record the code, repatch 
the executable in many places (they can be hundreds, if needed), 
disable the decryption parts, and so on.... an huge work, often 
not worth the time spent. And, of course, if your app is so valuable 
you can resort to a commercial tool !&]
[s0; &]
[s0; Second point is somehow more sensitive : if the cracker gets 
the key, he can write a tool that automatically decrypts and 
patch your application in short time, even if it contains hundred 
of protected functions. Is not so easy, but can be done in a 
reasonable time.&]
[s0; A perfect solution for that doesn`'t exist... of course. But, 
we can make cracker`'s live a bit more difficult if we encrypt 
somehow also sensitive parts of code with a random key.&]
[s0; This is done by another couple of macros :&]
[s0; &]
[s0;l160; [/ void MyObfuscatedFunction(void)]&]
[s0;l160; [/ `{]&]
[s0;l160; [/ -|][*/ OBFUSCATE`_START`_FUNC;]&]
[s0;l160;/ &]
[s0;l160; [/ -|<here my function code>]&]
[s0;l160;/ &]
[s0;l160; [/ -|][*/ OBFUSCATE`_END`_FUNC;]&]
[s0;l160;/ &]
[s0;l160; [/ `}]&]
[s0; &]
[s0; You can notice that the [* decrFunc ]parameter is gone. The macro 
generates a random, 16 byte long key, different for each obfuscated 
function. The key is stored into the executable and automatically 
used to decrypt the code on the fly when needed [*/ and ]to reencrypt 
it at function exit.&]
[s0; The code will run anyways, it won`'t check any key, so what`'s 
the purpose ?&]
[s0; Is simply to defeat  disassemblers and to make function tracing 
quite hard.&]
[s0; The disassembler would see just garbage bytes, as the function 
is stored in encrypted form. If you try to trace it step by step 
with a debugger, the function will be unencrypted just when needed, 
and re`-encrypted on exit, which confuses many debuggers (TheIde 
GDB frontend is `*very`* confused about it :`-) )&]
[s0; You`'d normally use [* Obfuscation ]when obtaining your key used 
to decrypt the [* Protected ]parts, mostly on code startup and 
on some sensitive parts which needs to be executed [/ before ]the 
key is available.&]
[s0; &]
[s0; A note : as opposite as [* PROTECT`_xxx`_FUNC()] macro block, the 
[* OBFUSCATE`_xxx`_FUNC] one [/ doesn`'t] create an enclosed code 
block, so the visibility of variable is [/ global ]to function; 
more important, the code [*/ MUST ]reach the end macro in order 
to [*/ re`-obfuscate] itself. That means that you [*/ can`'t] put 
[*/ any] return statement inside the obfuscation block. If you 
do, your app will crash if the obfuscated function is executed 
twice. So :&]
[s0; &]
[s0;l160; [/ -|][*/ OBFUSCATE`_START`_FUNC;]&]
[s0;l160; [*/ -|][/ <some code here>]&]
[s0;l160; [/ -|return;]&]
[s0;l160; [/ -|][*/ OBFUSCATE`_END`_FUNC;]&]
[s0; &]
[s0; Is [*/ WRONG ]and will make your app crash on second function 
invocation; the right approach is :&]
[s0; &]
[s0;l160; [/ -|][*/ OBFUSCATE`_START`_FUNC;]&]
[s0;l160; [*/ -|][/ <some code here>]&]
[s0;l160; [/ -|][*/ OBFUSCATE`_END`_FUNC;]&]
[s0;l160; [/ -|return;]&]
[s0; &]
[s0; A note : [* OBFUSCATE`_START`_FUNC ]is, by now, [* NOT ]thread safe, 
so [* DON`'T] obfuscate multithreaded routines.]