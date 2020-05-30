topic "";
[H6;0 $$1,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$2,0#37138531426314131252341829483370:codeitem]
[l288;2 $$3,0#27521748481378242620020725143825:desc]
[0 $$4,0#96390100711032703541132217272105:end]
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@2;4 Nonlinear functions]]}}&]
[s0;*@3;4 &]
[s0; [2 Some functions have been include to ease nonlinear optimization 
and equation system solving.]&]
[s0;2 &]
[s1;%- &]
[s2;:Upp`:`:SolveNonLinearEquations`(Eigen`:`:VectorXd`&`,Upp`:`:Function`<int`(const Eigen`:`:VectorXd`&b`,Eigen`:`:VectorXd`&residual`)`>`):%- [@(0.0.255) b
ool]_[* SolveNonLinearEquations]([_^Eigen`:`:VectorXd^ Eigen`::VectorXd]_`&[*@3 y], 
[_^Upp`:`:Function^ Function]_<[@(0.0.255) int]([@(0.0.255) const]_[_^Eigen`:`:VectorXd^ E
igen`::VectorXd]_`&b, [_^Eigen`:`:VectorXd^ Eigen`::VectorXd]_`&residual)>_[*@3 Residua
l])&]
[s3; Given a system of non linear equations defined by Function [%-*@3 Residual], 
and a set of initial values of the unknowns set in [%-*@3 y], this 
function tries to obtain the set of [%-*@3 y] that zeroes the [%-*@3 Residual].&]
[s3; The dimension of [%-*@3 y] ([%-*@3 y.size()]) has to be equal to 
the number of equations ([%-*@3 residual.size()]).&]
[s3; It uses a modification of the [^https`:`/`/en`.wikipedia`.org`/wiki`/Powell`%27s`_method^ P
owell`'s hybrid method] (`"dogleg`"). The Jacobian is approximated 
using a forward`-difference method. &]
[s3; The algorithm was ported in [^http`:`/`/eigen`.tuxfamily`.org`/index`.php`?title`=Main`_Page^ E
igen] from [^https`:`/`/www`.math`.utah`.edu`/software`/minpack`/minpack`/hybrd`.html^ M
INPACK] library.&]
[s4; &]
[s1;%- &]
[s2;:Upp`:`:NonLinearOptimization`(Eigen`:`:VectorXd`&`,Eigen`:`:Index`,Upp`:`:Function`<int`(const Eigen`:`:VectorXd`&`,Eigen`:`:VectorXd`&`)`>`):%- [@(0.0.255) b
ool]_[* NonLinearOptimization]([_^Eigen`:`:VectorXd^ Eigen`::VectorXd]_`&[*@3 y], 
[_^Eigen`:`:Index^ Eigen`::Index]_[*@3 numData], [_^Upp`:`:Function^ Function]_<[@(0.0.255) i
nt]([@(0.0.255) const]_[_^Eigen`:`:VectorXd^ Eigen`::VectorXd]_`&[*@3 y], 
[_^Eigen`:`:VectorXd^ Eigen`::VectorXd]_`&[*@3 residual])>[*@3 residual])&]
[s3; Given a number of [%-*@3 numData] records, the objective is to 
obtain in an iterative way the [%-*@3 y] unknown coefficients that 
minimizes, ideally to zero, the [%-*@3 residual] error of the [%-*@3 Residual] 
Function.&]
[s3; To improve the success of this calculation, [%-*@3 y] has to be 
initially filled with adequate initial guess of the unknowns.&]
[s3; [%- Residual ]Function has to fill in every call [%- residual]`[[%- numData]`] 
Vector by applying the unknowns provisionally guessed values 
in [%- y] to the model to be solved.&]
[s3; The algorithm was ported in [^http`:`/`/eigen`.tuxfamily`.org`/index`.php`?title`=Main`_Page^ E
igen] from [^https`:`/`/www`.math`.utah`.edu`/software`/minpack`/minpack`/lmder`.html^ M
INPACK] library.&]
[s4; &]
[s4; ]]