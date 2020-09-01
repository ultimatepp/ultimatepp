topic "Numerical Integration";
[H6;0 $$1,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$2,0#37138531426314131252341829483370:codeitem]
[l288;2 $$3,0#27521748481378242620020725143825:desc]
[0 $$4,0#96390100711032703541132217272105:end]
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[ {{10000@3 [s0; [*@7;4 Integral]]}}&]
[s1;%- &]
[s0;%- &]
[s0; [2 Numerical integration of a series of points.]&]
[s0; [2 Integration schemes included are:]&]
[s0;2 &]
[ {{2098:7902<288;h1; [s0;%- [*2 TRAPEZOIDAL]]
:: [s0;%- [2 Trapezoidal integration]]
:: [s0;%- [*2 SIMPSON`_1`_3]]
:: [s0;%- [2 Simpson`'s 1/3 rule with a correction  if the number of points 
is even.]]
:: [s0;%- [*2 SIMPSON`_3`_8]]
:: [s0;%- [2 Simpson`'s 3/8 rule with a correction if the number of points 
is not a multiple of three.]]
:: [s0;%- [*2 HERMITE`_3]]
:: [s0;%- [2 Hermite`'s rule with a three points difference scheme to 
approximate endpoints derivatives.]]
:: [s0;%- [*2 HERMITE`_5]]
:: [s0;%- [2 Hermite`'s rule with a five points difference scheme to approximate 
endpoints derivatives.]]}}&]
[s0;2 &]
[s2; References:&]
[s3;i150;O0; [^https`:`/`/en`.wikipedia`.org`/wiki`/Simpson`%27s`_rule^ Wikipedia]&]
[s3;i150;O0; [^https`:`/`/epubs`.siam`.org`/doi`/pdf`/10`.1137`/S0036144502416308^ `"An
 Invitation to Hermite’s Integration and Summation: A Comparison 
between Hermite’s and Simpson’s Rules`", V. Lampret. SIAM 
REVIEW Vol. 46, No. 2, pp. 311–328 (2004)]&]
[s3;i150;O0;%- [^https`:`/`/www`.semanticscholar`.org`/paper`/Hermite`-versus`-Simpson`-`%3A`-the`-Geometry`-of`-Numerical`-Long`-Long`/9f1d1ccfafc604428397ffbe6855a1686bba7a41^ `"
Hermite versus Simpson: the Geometry of Numerical Integration`", 
Andy Long and Cliff Long., (2010)]&]
[s0;^https`:`/`/en`.wikipedia`.org`/wiki`/Simpson`%27s`_rule^ &]
[s1;%- &]
[s2;:Upp`:`:Integral`(const Range`&`,const Range`&`,IntegralType`):%- [@(0.0.255) templ
ate]_<[@(0.0.255) class]_[*@4 Range], [@(0.0.255) class]_[*@4 T]>_[*@4 T]_[* Integral]([@(0.0.255) c
onst]_[*@4 Range]_`&[*@3 y], [@(0.0.255) const]_[*@4 Range]_`&[*@3 x], 
IntegralType_[*@3 type]_`=_TRAPEZOIDAL)&]
[s3; Obtains the numerical integration of data series defined by 
points ([%-*@3 x], [%-*@3 y]), using the integration schema defined 
with [%-*@3 type].&]
[s4; &]
[s1;%- &]
[s2;:Upp`:`:Integral`(Range`&`,T`,IntegralType`):%- [@(0.0.255) template]_<[@(0.0.255) cl
ass]_[*@4 Range], [@(0.0.255) class]_[*@4 T]>_[*@4 T]_[* Integral]([*@4 Range]_`&[*@3 y], 
[*@4 T]_[*@3 dx], IntegralType_[*@3 type]_`=_TRAPEZOIDAL)&]
[s3; Obtains the numerical integration of data series defined by 
points [%-*@3 y], separated [%-*@3 dx] between them, using the integration 
schema defined with [%-*@3 type].&]
[s4; &]
[s0; ]]