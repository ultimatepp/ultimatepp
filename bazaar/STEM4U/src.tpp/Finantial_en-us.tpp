topic "Finantial";
[H6;0 $$1,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$2,0#37138531426314131252341829483370:codeitem]
[l288;2 $$3,0#27521748481378242620020725143825:desc]
[0 $$4,0#96390100711032703541132217272105:end]
[i448;a25;kKO9; $$5,0#37138531426314131252341829483380:structitem]
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[ {{10000@3 [s0; [*@7;4 Financial]]}}&]
[s1;%- &]
[s0;%- &]
[s0; [2 Some financial functions.]&]
[s0;2 &]
[s4; &]
[s1;%- &]
[s2;:Upp`:`:NetPresentValue`(double`,const Upp`:`:Vector`<double`>`&`):%- [@(0.0.255) d
ouble]_[* NetPresentValue]([@(0.0.255) double]_[*@3 discountRate], 
[@(0.0.255) const]_[_^Upp`:`:Vector^ Vector]<[@(0.0.255) double]>_`&[*@3 cf])&]
[s3; Calculates the [^https`:`/`/en`.wikipedia`.org`/wiki`/Net`_present`_value^ Net 
present value].with [%-*@3 discountRate] discount rate, and [%-*@3 cf] 
cash flow.&]
[s4; &]
[s1;%- &]
[s2;:Upp`:`:InternalRateOfReturn`(const Upp`:`:Vector`<double`>`&`,double`,double`,int`,double`):%- [_^Upp`:`:Vector^ V
ector]<[@(0.0.255) double]>_[* InternalRateOfReturn]([@(0.0.255) const]_[_^Upp`:`:Vector^ V
ector]<[@(0.0.255) double]>_`&[*@3 cf], [@(0.0.255) double]_[*@3 lowRate], 
[@(0.0.255) double]_[*@3 highRate], [@(0.0.255) int]_[*@3 maxIteration], 
[@(0.0.255) double]_[*@3 precisionReq])&]
[s3; Calculates the [^https`:`/`/en`.wikipedia`.org`/wiki`/Internal`_rate`_of`_return^ I
nternal rate of return] of an investment. The arguments are:&]
[s3;i150;O0; [%-*@3 cf] is the cash flow.&]
[s3;i150;O0; [%-*@3 lowRate] is the initial rate with which we compute 
the NPV.&]
[s3;i150;O0; [%-*@3 highRate] is the highest rate up to which we should 
consider for computing NPV.&]
[s3;i150;O0; [%-*@3 maxIteration]: There is always a possibility of 
not able to arrive at the rate for certain cash flows. This variable 
acts as a stopper for the number of iterations the code should 
check for NPV so as to ensure that the program does not go for 
an infinite loop&]
[s3;i150;O0; [%-*@3 precisionReq] : NPV value will not normally hit 
zero. We can find the NPV value with a precision upto certain 
value. When the computed NPV is below this value, the calculation 
stop and the rate used will be the IRR.&]
[s4; &]
[s1;%- &]
[s2;:Upp`:`:PMT`(double`,double`,double`): [%-@(0.0.255) double][%- _][%-* PMT][%- (][%-@(0.0.255) d
ouble][%- _][%-*@3 rate][%- , ][%-@(0.0.255) double][%- _][%-*@3 nper][%- , 
][%-@(0.0.255) double][%- _][%-*@3 pv][%- )].&]
[s3; Calculates the payment for a loan based on constant payments 
and a constant interest rate. The arguments are:&]
[s3;i150;O0; [%-*@3 rate] is the interest rate for the loan.&]
[s3;i150;O0; [%-*@3 nper] is the total number of payments for the loan.&]
[s3;i150;O0; [%-*@3 pv] is the present value, or the total amount that 
a series of future payments is worth now; also known as the principal.&]
[s4; ]]