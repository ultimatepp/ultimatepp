topic "class EvalExpr";
[ $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9; $$1,0#37138531426314131252341829483380:structitem]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:codeitem]
[{_}%EN-US 
[ {{10000@3 [s0; [*@(229)4 EvalExpr]]}}&]
[s3; &]
[s1;:EvalExpr`:`:class:%- [@(0.0.255) class]_[* EvalExpr]&]
[s2; &]
[s0;%- [2 It implements a simple expression evaluator including variable 
handling.]&]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s0;%- &]
[s5;:EvalExpr`:`:EvalExpr`(`):%- [* EvalExpr]()&]
[s2; EvalExpr constructor.&]
[s3;%- &]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [*2 Public Member List]]}}&]
[s0;%- &]
[s5;:EvalExpr`:`:Eval`(String`):%- [@(0.0.255) double]_[* Eval]([_^String^ String]_[*@3 line])
&]
[s2; Evals [%-*@3 line] expression using declared constants and variables 
with previously assigned values returning numerical result.&]
[s2; Eval() can be called as many times as needed.&]
[s2; For example:&]
[s2;l448; EvalExpr expr;&]
[s2;l448; double res1 `= expr.Eval(`"x `= 2`");-|-|// Returns 2&]
[s2;l448; double res2 `= expr.Eval(`"23 `+ x`^2`");-|// Returns 27&]
[s2;l448; double res3 `= expr.Eval(`"y `= 2 `+ x`");/-|// Returns 
4&]
[s3; &]
[s4;%- &]
[s5;:EvalExpr`:`:EvalStr`(String`,int`):%- [_^String^ String]_[* EvalStr]([_^String^ String
]_[*@3 line], [@(0.0.255) int]_[*@3 numDigits]_`=_[@3 3])&]
[s2; Evals [%-*@3 line] expression using declared variables previously 
assigned values. It returns an expression including variable 
values with [%-*@3 numDigits] number of digits. If [%-*@3 numDigits] 
is Null, variables will not be replaced by its numerical value&]
[s3; &]
[s4;%- &]
[s5;:EvalExpr`:`:constants:%- [_^VectorMap^ VectorMap]<[_^String^ String], 
[@(0.0.255) double]>_[* constants]&]
[s2; [%-* constants] VectorMap includes all constants. Instead of with 
variables, the value of a constant is not shown with EvalStr(). 
It  will be converted private soon.&]
[s3;%- &]
[s4;%- &]
[s5;:EvalExpr`:`:variables:%- [_^VectorMap^ VectorMap]<[_^String^ String], 
[@(0.0.255) double]>_[* variables]&]
[s2; [%-* variables] VectorMap includes all variables. It  will be 
converted private soon.&]
[s3;%- &]
[s3; ]]