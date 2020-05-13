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
[s2;:Upp`:`:NonLinearOptimization`(Eigen`:`:VectorXd`&`,int`,Upp`:`:Function`<int`(const Eigen`:`:VectorXd`&`,Eigen`:`:VectorXd`&`)`>`):%- [@(0.0.255) b
ool]_[* NonLinearOptimization]([_^Eigen`:`:VectorXd^ Eigen`::VectorXd]_`&[*@3 y], 
[@(0.0.255) int]_[*@3 numData], [_^Upp`:`:Function^ Function]_<[@(0.0.255) int]([@(0.0.255) c
onst]_[_^Eigen`:`:VectorXd^ Eigen`::VectorXd]_`&[*@3 y], [_^Eigen`:`:VectorXd^ Eigen`::Ve
ctorXd]_`&[*@3 residual])>[*@3 Residual])&]
[s3; Given a number of [%-*@3 numData] records, the objective is to 
obtain in an iterative way the [%-*@3 y] unknown coefficients that 
minimizes, ideally to zero, the [%-*@3 residual] error of the [%-*@3 Residual] 
Function.&]
[s3; To improve the success of this calculation, [%-*@3 y] has to be 
initially filled with adequate initial guess of the unknowns.&]
[s3; [%-*@3 Residual ]Function has to fill in every call [%-*@3 residual]`[[%-*@3 numData]`]
 Vector by applying the unknowns provisionally guessed values 
in [%-*@3 y] to the model to be solved.&]
[s4; &]
[s1;%- &]
[s2;:Upp`:`:NonLinearSolver`(Eigen`:`:VectorXd`&`,Upp`:`:Function`<int`(const Eigen`:`:VectorXd`&b`,Eigen`:`:VectorXd`&residual`)`>`):%- [@(0.0.255) b
ool]_[* NonLinearSolver]([_^Eigen`:`:VectorXd^ VectorXd]_`&[*@3 y], 
[_^Upp`:`:Function^ Function]_<[@(0.0.255) int]([@(0.0.255) const]_[_^Eigen`:`:VectorXd^ E
igen`::VectorXd]_`&[*@3 y], [_^Eigen`:`:VectorXd^ Eigen`::VectorXd]_`&[*@3 residual])>_[*@3 R
esidual])&]
[s3; Given a system of equations defined by Function [%-*@3 Residual], 
and a set of initial values of the unknowns set in [%-*@3 y], this 
function tries to obtain the set of [%-*@3 y] that best comply 
with [%-*@3 Residual].&]
[s3; The dimension of [%-*@3 y] ([%-*@3 y.size()]) has to be equal to 
the number of equations ([%-*@3 residual.size()]).&]
[s4; ]]