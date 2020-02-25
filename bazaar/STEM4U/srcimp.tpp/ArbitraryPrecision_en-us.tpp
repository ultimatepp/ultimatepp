topic "Rational. An arbitrary precision rational number";
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s0; [*R+184 Rational. An arbitrary precision rational number]&]
[s0; We use computers for doing floating point calculations. However 
even using 64 bit types, lack of precision in calculations produce 
inaccuracies that go accumulating over time, thus increasing 
the error.&]
[s0; &]
[s0; For example, lets compute y `= 2/1 `* 3/2 `* 4/3 `* ... If done 
n times, result has to be n.&]
[s0; &]
[s0; However, the code implementing this will fail:&]
[s0; &]
[s0; [C+92 double val `= 1;]&]
[s0; [C+92 for (double d `= 1; d < 100; `+`+d) ]&]
[s0; [C+92 -|val `*`= (d`+1)/d;]&]
[s0; [C+92 Cout() << `"double `=`= 100: `" << ((dval `=`= 100) ? `"true`" 
: `"false`") << `"`\n`";-|// It returns false!?!?]&]
[s0; &]
[s0; [^topic`:`/`/STEM4U`/src`/Rational`_en`-us^ Rational class] solves 
this drawback implementing an arbitrary precision integer rational 
number.]]