topic "2.1 ExplicitEquation";
[ $$0,0#00000000000000000000000000000000:Default]
[0 $$1,0#96390100711032703541132217272105:end]
[i448;a25;kKO9;2 $$2,0#37138531426314131252341829483380:class]
[H6;0 $$3,0#05600065144404261032431302351956:begin]
[l288;2 $$4,4#27521748481378242620020725143825:desc]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:codeitem]
[{_}%EN-US 
[ {{10000@3 [s0; [*@(229)4 ExplicitEquation]]}}&]
[s1; &]
[s2;:ExplicitEquation`:`:class: [@(0.0.255)3 class][3 _][*3 ExplicitEquation 
: ][@(0.0.255)3 public][3 _][*3 DataSource]&]
[s0;2 &]
[s0; [2 ExplicitEquation represents a generic explicit equation type, 
that is to say, a function y `= f(x1, x2, ..., a1, a2, ...) where 
y is the dependent variable, x1, x2 are the independent variables 
and a1, a2, ... are the specific coefficients. For example:]&]
[s0; [2 -|]&]
[s0; [2 -|y `= 3`*x`^2 `+ 2`*x `- 1]&]
[s0;2 &]
[s0; [2 where the numbers 3, 2 and `-1 are the equation coefficients.]&]
[s0;2 &]
[s0; [2 It can support any explicit equation so it is used to handle 
trend lines and non linear regression.]&]
[s0;2 &]
[s0; [2 Its normal use is subclassed to equation types like ][^topic`:`/`/ScatterDraw`/src`/LinearEquation`$en`-us^2 L
inearEquation][2 , ][^topic`:`/`/ScatterDraw`/src`/PolynomialEquation`$en`-us^2 Polyn
omialEquation, ][^topic`:`/`/ScatterDraw`/src`/FourierEquation`$en`-us^2 FourierEqu
ation, ][^topic`:`/`/ScatterDraw`/src`/Rational1Equation`$en`-us^2 Rational1Equatio
n][^topic`:`/`/ScatterDraw`/src`/FourierEquation`$en`-us^2 , ][^topic`:`/`/ScatterDraw`/src`/ExponentialEquation`$en`-us^2 E
xponentialEquation][^topic`:`/`/ScatterDraw`/src`/FourierEquation`$en`-us^2 , 
][^topic`:`/`/ScatterDraw`/src`/UserEquation`$en`-us^2 UserEquation][^topic`:`/`/ScatterDraw`/src`/FourierEquation`$en`-us^2 .
]&]
[s0;2 &]
[s1; &]
[ {{10000F(128)G(128)@1 [s0; [*2 Public Member List]]}}&]
[s3; &]
[s5;:ExplicitEquation`:`:SetDegree`(int`): [@(0.0.255) virtual] [@(0.0.255) void]_[* SetDeg
ree]([@(0.0.255) int]_[*@3 num])&]
[s4; Sets [*@3 num] as the degree of the equation. In a polynomial 
equation it is the exponent of the highest power of the independent 
variables.&]
[s4; It is related with the equation number of coefficients as the 
higher the degree, the higher the number of coefficients and 
thus the complexity of the equation.&]
[s1; &]
[s3; &]
[s5;:ExplicitEquation`:`:Fit`(DataSource`&`,double`&`): [@(0.0.255) FitError]_[* Fit]([_^DataSource^ D
ataSource]_`&[*@3 series], [@(0.0.255) double]_`&[*@3 r2])&]
[s4; Searches for the best combination of coefficients of the equation 
that matches [*@3 series] data series. The quality of the matching 
is returned in the [^http`:`/`/en`.wikipedia`.org`/wiki`/Coefficient`_of`_determination^ c
oefficient of determination] [*@3 r2].&]
[s4; It uses [^http`:`/`/eigen`.tuxfamily`.org`/index`.php`?title`=Main`_Page^ Eigen] 
implementation of [^http`:`/`/en`.wikipedia`.org`/wiki`/Levenberg`%E2`%80`%93Marquardt`_algorithm^ L
evenberg`-Marquardt] algorithm based on [^http`:`/`/www`.mcs`.anl`.gov`/`~more`/^ J
orge Moré] et al. [^http`:`/`/en`.wikipedia`.org`/wiki`/MINPACK^ MINPACK] 
original library.&]
[s4; It returns:&]
[s4;i150;O0; ExplicitEquation`::NoError&]
[s4;l448; Function returns succesfully. It is a return value bigger 
than zero.&]
[s4;i150;O0; ExplicitEquation`::InadequateDataSource&]
[s4;l448; Only data series sources are supported. Explicit and parametric 
functions are not supported.&]
[s4;i150;O0; ExplicitEquation`::SmallDataSource&]
[s4;l448; The number of values of the data set has to be bigger or 
equal than the number of coefficients to be obtained.&]
[s4;i150;O0; ExplicitEquation`::ImproperInputParameters&]
[s4;l448; There are problems in the input parameters. For example 
repeated data may avoid convergence.&]
[s4;i150;O0; ExplicitEquation`::TooManyFunctionEvaluation&]
[s4;l448; The methods converges to a solution too slowly.&]
[s1; &]
[s3; &]
[s5;:ExplicitEquation`:`:Fit`(DataSource`&`): [@(0.0.255) FitError]_[* Fit]([_^DataSource^ D
ataSource]_`&[*@3 series])&]
[s4; Simplified version of [@(0.0.255) bool]_[* Fit]([_^DataSource^ DataSource]_`&[*@3 series
], [@(0.0.255) double]_`&[*@3 r2]) that do not return [*@3 r2].&]
[s1; &]
[s3;%- &]
[s5;:ExplicitEquation`:`:GuessCoeff`(DataSource`&`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* GuessCoeff]([_^DataSource^ DataSource]_`&[*@3 series])_`=_[@3 0]&]
[s4; Guesses a set of initial values for the equation coefficients 
that matches [%-*@3 series], based on previous knowledge of equation.&]
[s1; &]
[s3; &]
[s5;:ExplicitEquation`:`:f`(double`): [@(0.0.255) virtual] [@(0.0.255) double]_[* f]([@(0.0.255) d
ouble]_[*@3 x1])&]
[s4; Returns the value of the explicit equation based on the independent 
value [*@3 x].&]
[s1; &]
[s3; &]
[s5;:ExplicitEquation`:`:f`(double`,double`): [@(0.0.255) virtual] 
[@(0.0.255) double]_[* f]([@(0.0.255) double]_[*@3 x1], [@(0.0.255) double]_[*@3 x2])&]
[s4; Returns the value of the explicit equation based on the independent 
values [*@3 x1 ]and [*@3 x2].&]
[s1; &]
[s3; &]
[s5;:ExplicitEquation`:`:f`(Vector`<double`>`): [@(0.0.255) virtual] 
[@(0.0.255) double]_[* f]([_^Vector^ Vector]_<[@(0.0.255) double]>_[*@3 x])&]
[s4; Returns the value of the explicit equation based on the independent 
value set [*@3 xn].&]
[s1; &]
[s3; &]
[s5;:ExplicitEquation`:`:GetName`(`): [@(0.0.255) virtual] [_^String^ String]_[* GetName]()
&]
[s4; Returns the equation name as `"Linear`", `"Polynomial`" or `"Fourier`".&]
[s1; &]
[s3; &]
[s5;:ExplicitEquation`:`:GetFullName`(`): [@(0.0.255) virtual] [_^String^ String]_[* GetFul
lName]()&]
[s4; Returns the equation name as `"Linear`", `"Polynomial n `= 2`" 
or `"Fourier n `= 3`". n represents the degree of the equation.&]
[s1; &]
[s3; &]
[s5;:ExplicitEquation`:`:GetEquation`(`): [@(0.0.255) virtual] [_^String^ String]_[* GetEqu
ation]()&]
[s4; Returns the equation in plain text, as y `= `-34 `+ 12`*x `- 
3`*x`^2&]
[s1; &]
[s3;%- &]
[s5;:ExplicitEquation`:`:SetNumDigits`(int`):%- [@(0.0.255) void]_[* SetNumDigits]([@(0.0.255) i
nt]_[*@3 n])&]
[s4; Sets with [%-*@3 n] the equation coefficients number of digits 
when returned by [_^String^ String]_[* GetEquation]().&]
[s1; &]
[s3;%- &]
[s5;:ExplicitEquation`:`:GetNumDigits`(`):%- [@(0.0.255) int]_[* GetNumDigits]()&]
[s4; Returns the equation coefficients number of digits when returned 
by [_^String^ String]_[* GetEquation]().&]
[s1;%- &]
[s3;%- &]
[s5;:ExplicitEquation`:`:SetMaxFitFunctionEvaluations`(int`):%- [@(0.0.255) void]_[* SetM
axFitFunctionEvaluations]([@(0.0.255) int]_[*@3 n])&]
[s4; Sets with [%-*@3 n] the maximum number of equation evaluations 
done by [* Fit]() searching for the best coefficients values.&]
[s1; &]
[s3;%- &]
[s5;:ExplicitEquation`:`:GetMaxFitFunctionEvaluations`(`):%- [@(0.0.255) int]_[* GetMaxFi
tFunctionEvaluations]()&]
[s4; Returns the maximum number of equation evaluations done by [* Fit]() 
searching for the best coefficients values.&]
[s1;%- &]
[s3; ]]