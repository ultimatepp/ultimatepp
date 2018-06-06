topic "String encryption";
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s0;%- [*R6 String encryption]&]
[s0;/%- &]
[s0; The Protect packages provides also a simple way to encrypt strings, 
in the form of character arrays, along as a function that can 
retrieve the encrypted data.&]
[s0; &]
[s0; Let`'s look at an example : suppose you want to encrypt the following 
text:&]
[s0; &]
[s0; `"I`'m a really really secret text`"&]
[s0; &]
[s0; To encrypt it, it`'s enough to surround it by 2 macros, PROTECT`_DATA`_START 
and PROTECT`_DATA`_END :&]
[s0; &]
[s0; const char `*MyEncryptedText `= PROTECT`_DATA`_START `"I`'m 
a really really secret text`" PROTECT`_DATA`_END;&]
[s0; &]
[s0; To retrieve it in your application, it`'s enough to do :&]
[s0; &]
[s0; String MyDecriptedText `= PROTECT`_DECRYPT`_DATA(GetCypher, 
MyEncryptedText);&]
[s0; &]
[s0; And you`'ll get the decrypted text in your string.&]
[s0; If you build your application without the PROTECT flag this 
will also work, just removing the start and end protection markers.&]
[s0;I5; ]]