topic "3.1 Spline";
[i448;a25;kKO9; $$1,0#37138531426314131252341829483380:structitem]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:codeitem]
[ $$0,0#00000000000000000000000000000000:Default]
[{_} 
[ {{10000@3 [s0;%% [*@(229)4 Spline]]}}&]
[s3; &]
[s1;:Upp`:`:Spline`:`:class: [@(0.0.255) class]_[* Spline]&]
[s2;%% &]
[s0;%% [2 A natural Spline implementation.]&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [*2 Public Member List]]}}&]
[s4; &]
[s5;:Upp`:`:Spline`:`:Init`(const Upp`:`:Vector`<double`>`&`,const Upp`:`:Vector`<double`>`&`): [@(0.0.255) v
oid]_[* Init]([@(0.0.255) const]_[_^Upp`:`:Vector^ Vector]<[@(0.0.255) double]>_`&[*@3 x], 
[@(0.0.255) const]_[_^Upp`:`:Vector^ Vector]<[@(0.0.255) double]>_`&[*@3 y])&]
[s2;%% Gets the spline for a data series of [%-*@3 x] and [%-*@3 y].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Spline`:`:Init`(const Eigen`:`:VectorXd`&`,const Eigen`:`:VectorXd`&`): [@(0.0.255) v
oid]_[* Init]([@(0.0.255) const]_[_^Eigen`:`:VectorXd^ Eigen`::VectorXd]_`&[*@3 x], 
[@(0.0.255) const]_[_^Eigen`:`:VectorXd^ Eigen`::VectorXd]_`&[*@3 y])&]
[s2;%% Gets the spline for a data series of [%-*@3 x] and [%-*@3 y].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Spline`:`:Init`(const double`*`,const double`*`,int`): [@(0.0.255) void]_[* I
nit]([@(0.0.255) const]_[@(0.0.255) double]_`*[*@3 x], [@(0.0.255) const]_[@(0.0.255) doubl
e]_`*[*@3 y], [@(0.0.255) int]_[*@3 n])&]
[s2;%% Gets the spline for a data series of [%-*@3 x] and [%-*@3 y].with 
dimension [%-*@3 n].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Spline`:`:f`(double`)const: [@(0.0.255) double]_[* f]([@(0.0.255) double]_[*@3 x])
 const&]
[s2;%% Returns the y value for [%-*@3 x].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Spline`:`:df`(double`)const: [@(0.0.255) double]_[* df]([@(0.0.255) double]_[*@3 x
])_[@(0.0.255) const]&]
[s2;%% Returns the first derivative at [%-*@3 x] .&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Spline`:`:d2f`(double`)const: [@(0.0.255) double]_[* d2f]([@(0.0.255) double]_[*@3 x
])_[@(0.0.255) const]&]
[s2;%% Returns the second derivative at [%-*@3 x] .&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Spline`:`:Integral`(double`,double`)const: [@(0.0.255) double]_[* Integral]([@(0.0.255) d
ouble]_[*@3 from], [@(0.0.255) double]_[*@3 to])_[@(0.0.255) const]&]
[s2;%% Returns the definite integral from x `= [%-*@3 from] to x `= 
[%-*@3 to.].&]
[s3;%% &]
[s0;%% ]]