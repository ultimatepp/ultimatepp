topic "Butterworth IIR filter";
[H6;0 $$1,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$2,0#37138531426314131252341829483370:codeitem]
[l288;2 $$3,0#27521748481378242620020725143825:desc]
[0 $$4,0#96390100711032703541132217272105:end]
[ $$0,0#00000000000000000000000000000000:Default]
[{_} 
[ {{10000@3 [s0;%% [*@7;4 Butterworth IIR filter]]}}&]
[s1; &]
[s0; &]
[s0;%% [2 The ][^https`:`/`/en`.wikipedia`.org`/wiki`/Butterworth`_filter^2 Butterworth 
IIR filter][2  is a type of signal processing filter designed to 
have a frequency response as flat as possible in the passband. 
It is also referred to as a maximally flat magnitude filter. 
It was first described in 1930 by the British engineer and physicist 
Stephen Butterworth in his paper entitled ][^https`:`/`/www`.changpuak`.ch`/electronics`/downloads`/On`_the`_Theory`_of`_Filter`_Amplifiers`.pdf^2 `"
On the Theory of Filter Amplifiers`"]&]
[s0; [2 Other references:]&]
[s0;i150;O0; [^https`:`/`/en`.wikipedia`.org`/wiki`/Digital`_filter`#Difference`_equation^2 D
igital filter (Wikipedia)]&]
[s0;i150;O0; [^https`:`/`/en`.wikipedia`.org`/wiki`/Infinite`_impulse`_response^2 Infin
ite impulse response]&]
[s0;i150;O0; [^https`:`/`/www`.mathworks`.com`/help`/signal`/ref`/butter`.html^2 butter
 function (Matlab)]&]
[s0;i150;O0; [^https`:`/`/www`.mathworks`.com`/help`/matlab`/ref`/filter`.html^2 filter
 function (Matlab)]&]
[s0;i150;O0; [^https`:`/`/es`.mathworks`.com`/help`/signal`/ref`/filtfilt`.html`?lang`=en^2 f
iltfilt function (Matlab)]&]
[s4; &]
[s1; &]
[s2;:Upp`:`:Butter`(int`,typename value`_type`,typename value`_type`,Range`&`,Range`&`): [@(0.0.255) t
emplate]_<[@(0.0.255) class]_[*@4 Range]>_[@(0.0.255) void]_[* Butter]([@(0.0.255) int]_[*@3 o
rder], [@(0.0.255) typename]_Range`::value`_type_[*@3 lcutoff], [@(0.0.255) typename]_Ra
nge`::value`_type_[*@3 ucutoff], [*@4 Range]_`&[*@3 cnum], [*@4 Range]_`&[*@3 cden])&]
[s3;%% [%- Returns the transfer function coefficients (feedforward 
coefficients ][%-*@3 cnum] (B) and feedback coefficients [%-*@3 cden] 
(A)[%- ) of an nth`-][%-*@3 order] [%- bandpass digital Butterworth 
filter with normalized lower cutoff ][%-*@3 lcutoff] [%- and higher 
cutoff u][%-*@3 cutoff ]frequencies.&]
[s3;%% For digital filters, the transfer function is expressed in 
terms of B and A as:&]
[s0;=%% [2 
@@image:2071&309
(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)
]&]
[s4;%% &]
[s1; &]
[s2;:Upp`:`:Filter`(const Range`&`,const Range`&`,const Range`&`,Range`&`): [@(0.0.255) t
emplate]_<[@(0.0.255) class]_[*@4 Range]>_[@(0.0.255) void]_[* Filter]([@(0.0.255) const]_[*@4 R
ange]_`&[*@3 x], [@(0.0.255) const]_[*@4 Range]_`&[*@3 cnum], [@(0.0.255) const]_[*@4 Range]_
`&[*@3 cden], [*@4 Range]_`&[*@3 filtered])&]
[s3;%% Filters the input data [%-*@3 x] using a rational transfer function 
defined by the numerator [%- feedforward coefficients] [%-*@3 cnum] 
and denominator [%- feedback coefficients ][%-*@3 cden].&]
[s4;%% &]
[s0;%% ]]