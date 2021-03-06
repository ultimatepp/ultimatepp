topic "2 DataSource";
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
[ {{10000@3 [s0; [*@(229)4 DataSource]]}}&]
[s3; &]
[s1;:DataSource`:`:class:%- [@(0.0.255)3 class][3 _][*3 DataSource]&]
[s0; &]
[s0; Some classes need sources of data for drawing, data analysis 
and statistics. However data series can be defined in different 
containers like C arrays, U`+`+ containers or even data grids 
like ArrayCtrl and GridCtrl.&]
[s0; &]
[s0; [^topic`:`/`/ScatterDraw`/src`/DataSource`$en`-us^ DataSource] 
abstracts many sources of data, like:&]
[s0; &]
[s0;i150;O0; [* Data series]&]
[s0; They are series of data with a number of columns (parameters) 
and rows (every parameter set case). For example:&]
[s0; &]
[ {{2500:2500:2500:2500<512;>928;h1;@(127)R2 [s0; [@2 X]]
:: [s0; [@2 Y]]
:: [s0; [@2 Z]]
:: [s0; [@2 Temperature]]
::@2R0 [s0; 1.1]
:: [s0; 45]
:: [s0; `-34]
:: [s0; 22]
:: [s0; 3.5]
:: [s0; 23]
:: [s0; 12]
:: [s0; 25]
:: [s0; 2.4]
:: [s0; 78]
:: [s0; 112]
:: [s0; 24]}}&]
[s0; &]
[s0;i150;O0; [* Explicit equation]&]
[s0; A function y `= f(x1, x2, ...) where y is the dependent variable 
and x1, x2 are the independent variables.&]
[s0; Examples are:&]
[s0; -|&]
[s0; -|y `= 4x `+ 3z&]
[s0; -|y `= 3`*x`^2 `+ 2`*x `- 1&]
[s0; &]
[s0;i150;O0; [* Parametric equation]&]
[s0; ([^http`:`/`/en`.wikipedia`.org`/wiki`/Parametric`_equation^ From 
Wikipedia]) A parametric equation of a curve is a representation 
of it through equations expressing the coordinates of the points 
of the curve as functions of a variable called parameter. For 
example,&]
[s0; &]
[s0; -|x `= cos(t)&]
[s0; -|y `= sin(t)&]
[s0; &]
[s0; is a parametric equation for the unit circle, where t is the 
parameter.&]
[s0; &]
[s0; These equations are useful to represent closed functions as 
circles, spirals and even epitrochoids.&]
[s0; &]
[s0; [^topic`:`/`/ScatterDraw`/src`/DataSource`$en`-us^ DataSource 
]classes can be used to interface data sources and containers 
and can be subclassed to be embedded in other classes like [^topic`:`/`/ScatterDraw`/src`/ExplicitEquation`$en`-us^ E
xplicitEquation].&]
[s0; &]
[s0; Examples of [^topic`:`/`/ScatterDraw`/src`/DataSource`$en`-us^ DataSource 
]classes are [^topic`:`/`/ScatterDraw`/src`/CArray`$en`-us^ CArray] 
and [^topic`:`/`/ScatterDraw`/src`/VectorY`$en`-us^ VectorY] .&]
[s0; &]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s4; &]
[s5;:DataSource`:`:DataSource`(`):%- [* DataSource]()&]
[s2; Default constructor where the data is defined as data series 
by default.&]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s4;%- &]
[s5;:DataSource`:`:y`(int64`):%- [@(0.0.255) virtual] [@(0.0.255) double]_[* y]([_^int64^ int
64]_[*@3 id])&]
[s2; Returns the first parameter of the data series [%-*@3 id] th value.&]
[s3; &]
[s4;%- &]
[s5;:DataSource`:`:x`(int64`):%- [@(0.0.255) virtual] [@(0.0.255) double]_[* x]([_^int64^ int
64]_[*@3 id])&]
[s2; Returns the second parameter of the data series [%-*@3 id] th 
value.&]
[s3; &]
[s4;%- &]
[s5;:DataSource`:`:xn`(int`,int64`):%- [@(0.0.255) virtual] [@(0.0.255) double]_[* xn]([@(0.0.255) i
nt]_[*@3 n], [_^int64^ int64]_[*@3 id])&]
[s2; Returns the [%-*@3 n] th parameter of the data series [%-*@3 id] 
th value.&]
[s3; &]
[s4;%- &]
[s5;:DataSource`:`:y`(double`):%- [@(0.0.255) virtual] [@(0.0.255) double]_[* y]([@(0.0.255) d
ouble]_[*@3 t])&]
[s2; Returns the first parameter of the parametric equation with 
independent value [%-*@3 t].&]
[s3; &]
[s4;%- &]
[s5;:DataSource`:`:x`(double`):%- [@(0.0.255) virtual] [@(0.0.255) double]_[* x]([@(0.0.255) d
ouble]_[*@3 t])&]
[s2; Returns the second parameter of the parametric equation with 
independent value [%-*@3 t].&]
[s3; &]
[s4;%- &]
[s5;:DataSource`:`:xn`(int`,double`):%- [@(0.0.255) virtual] [@(0.0.255) double]_[* xn]([@(0.0.255) i
nt]_[*@3 n], [@(0.0.255) double]_[*@3 t])&]
[s2; Returns the [%-*@3 n] th parameter of the parametric equation 
with independent value [%-*@3 t].&]
[s3; &]
[s4;%- &]
[s5;:DataSource`:`:f`(double`):%- [@(0.0.255) virtual] [@(0.0.255) double]_[* f]([@(0.0.255) d
ouble]_[*@3 x])&]
[s2; Returns the value of the explicit equation based on the independent 
value [%-*@3 x].&]
[s3; &]
[s4;%- &]
[s5;:DataSource`:`:f`(Vector`<double`>`):%- [@(0.0.255) virtual] [@(0.0.255) double]_[* f](
[_^Vector^ Vector]<[@(0.0.255) double]>_[*@3 xn])&]
[s2; Returns the value of the explicit equation based on the independent 
value set [%-*@3 xn].&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:DataSource`:`:Copy`(Upp`:`:DataSource`:`:Getdatafun`,Range`&`):%- [@(0.0.255) t
emplate]_<[@(0.0.255) class]_[*@4 Range]>_[@(0.0.255) void]_[* Copy]([_^Upp`:`:DataSource`:`:Getdatafun^ G
etdatafun]_[*@3 getdata], [*@4 Range]_`&[*@3 out])&]
[s2; Copies data series [%-*@3 getdata] into an Upp`::Vector<>, Eigen`::VectorXd 
or a std`::vector<> [%-*@3 out].&]
[s3; &]
[s4;%- &]
[s5;:DataSource`:`:GetCount`(`):%- [@(0.0.255) virtual] [_^int64^ int64]_[* GetCount]()&]
[s2; Returns the number of values in a data series or a parametric 
equation.&]
[s3;%- &]
[s4;%- &]
[s5;:DataSource`:`:IsParam`(`):%- [@(0.0.255) bool]_[* IsParam]()&]
[s2; Returns true if the data source is a parametric equation.&]
[s3;%- &]
[s4;%- &]
[s5;:DataSource`:`:IsExplicit`(`):%- [@(0.0.255) bool]_[* IsExplicit]()&]
[s2; Returns true if the data source is a explicit equation.&]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Functions]]}}&]
[s5;:Upp`:`:Convolution`(const Eigen`:`:MatrixBase`<T`>`&`,const Eigen`:`:MatrixBase`<T`>`&`,const double`):%- [@(0.0.255) t
emplate]_<[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) typename]_T`::PlainObject_[* Convolution
]([@(0.0.255) const]_[_^Eigen`:`:MatrixBase^ Eigen`::MatrixBase]<[*@4 T]>`&_[*@3 orig], 
[@(0.0.255) const]_[_^Eigen`:`:MatrixBase^ Eigen`::MatrixBase]<[*@4 T]>`&_[*@3 kernel], 
[@(0.0.255) const]_[@(0.0.255) double]_[*@3 factor]_`=_[@3 1])&]
[s2; Applies on [%-*@3 orig] the [^https`:`/`/en`.wikipedia`.org`/wiki`/Convolution^ conv
olution] [%-*@3 kernel], multiplying [%-*@3 factor] on each value 
of the convolution result.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Convolution2D`(const Eigen`:`:MatrixBase`<T`>`&`,const Eigen`:`:MatrixBase`<T`>`&`,const double`):%- [@(0.0.255) t
emplate]_<[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) typename]_T`::PlainObject_[* Convolution
2D]([@(0.0.255) const]_[_^Eigen`:`:MatrixBase^ Eigen`::MatrixBase]<[*@4 T]>`&_[*@3 orig], 
[@(0.0.255) const]_[_^Eigen`:`:MatrixBase^ Eigen`::MatrixBase]<[*@4 T]>`&_[*@3 kernel], 
[@(0.0.255) const]_[@(0.0.255) double]_[*@3 factor]_`=_[@3 1])&]
[s2; Applies on [%-*@3 orig] the 2D [^https`:`/`/en`.wikipedia`.org`/wiki`/Convolution^ c
onvolution] [%-*@3 kernel], multiplying [%-*@3 factor] on each value 
of the convolution result.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:SavitzkyGolay`_CheckParams`(int`,int`,int`,int`):%- [@(0.0.255) bool]_[* Savi
tzkyGolay`_CheckParams]([@(0.0.255) int]_[*@3 nleft], [@(0.0.255) int]_[*@3 nright], 
[@(0.0.255) int]_[*@3 deg], [@(0.0.255) int]_[*@3 der])&]
[s2;  Returns true if Savitzsky`-Golay filter arguments are correct. 
They are:&]
[s0;l288;i150;O0; [%-*@3 nleft]: Number of leftward data points&]
[s2;i150;O0; [%-*@3 nright]: Number of rightward data points&]
[s2;i150;O0; [%-*@3 deg]: Order of the smoothing polynomial&]
[s2;i150;O0; [%-*@3 der]: Order of the derivative. 0 means smoothed 
function, 1 means smoothed first derivative, ...&]
[s2; Valid arguments have to comply with nleft >`= 0 `&`& nright 
>`= 0 `&`& der <`= deg `&`& nleft `+ nright >`= deg.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SavitzkyGolay`_Coeff`(int`,int`,int`,int`):%- VectorXd_[* SavitzkyGolay`_Co
eff]([@(0.0.255) int]_[*@3 nleft], [@(0.0.255) int]_[*@3 nright], [@(0.0.255) int]_[*@3 deg],
 [@(0.0.255) int]_[*@3 der])&]
[s2;%- Savitzky–Golay is a digital filter applied by convolution 
of successive sub`-sets of adjacent data points with a low`-degree 
polynomial by the method of linear least squares. When the data 
points are equally spaced, an analytical solution to the least`-squares 
equations can be found, in the form of a single set of `"convolution 
coefficients`", to give estimates of the smoothed signal, (or 
derivatives of the smoothed signal) at the central point of each 
sub`-set. The method, was popularized by Abraham Savitzky and 
Marcel J. E. Golay in 1964. This function obtains the filter 
coefficients, and has to be applied just once for any dataset, 
as the coefficients do not depend on data but in the conditions 
defined by arguments. Arguments are explained in [*^Upp`:`:SavitzkyGolay`_CheckParams`(int`,int`,int`,int`)^ S
avitzkyGolay`_CheckParams][^Upp`:`:SavitzkyGolay`_CheckParams`(int`,int`,int`,int`)^ (
)]. Filter has to be appplied using [*^Upp`:`:Convolution`(const Eigen`:`:VectorXd`&`,const T`&`,int`,double`)^ C
onvolution][^Upp`:`:Convolution`(const Eigen`:`:VectorXd`&`,const T`&`,int`,double`)^ (
)] on input data.&]
[s2;%- &]
[s2;%- [%% References: ][%%^https`:`/`/en`.wikipedia`.org`/wiki`/Savitzky`%E2`%80`%93Golay`_filter^ S
avitzky–Golay filter (Wikipedia)][%% , Savitzky, A.; Golay, M.J.E. 
(1964). `"Smoothing and Differentiation of Data by Simplified 
Least Squares Procedures`". Analytical Chemistry. 36 (8): 1627–39, 
][%%^http`:`/`/www`.nrbook`.com`/a`/bookcpdf`.php^ Numerical Recipes 
in C, Second Edition (1992), ]Schafer, R.W. What [/ Is ]a Savitzky`-Golay 
Filter?, IEEE Signal Processing Magazine [@(153) `[]116`] (2011)[+59 .]&]
[s3; ]]