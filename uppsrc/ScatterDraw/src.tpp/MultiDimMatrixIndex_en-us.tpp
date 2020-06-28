topic "3.3 MultiDimMatrixIndex";
[H6;0 $$1,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$2,0#37138531426314131252341829483370:codeitem]
[l288;2 $$3,0#27521748481378242620020725143825:desc]
[0 $$4,0#96390100711032703541132217272105:end]
[i448;a25;kKO9; $$5,0#37138531426314131252341829483380:structitem]
[ $$0,0#00000000000000000000000000000000:Default]
[{_} 
[ {{10000@3 [s0;%% [*@(229)4 MultiDimMatrixIndex]]}}&]
[s4; &]
[s5;:Upp`:`:MultiDimMatrixIndex`:`:class: [@(0.0.255) class]_[* MultiDimMatrixIndex]&]
[s3;%% &]
[s0;%% [2 A class to access multidimensional matrices stored in simple 
vector containers.]&]
[s0;2%% &]
[s4;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor Detail]]}}&]
[s1; &]
[s2;:Upp`:`:MultiDimMatrixIndex`:`:MultiDimMatrixIndex`(int`): [* MultiDimMatrixIndex](
[@(0.0.255) int]_[*@3 numAxis])&]
[s3;%% Sets the number of dimensions with [%-*@3 numAxis].&]
[s4; &]
[ {{10000F(128)G(128)@1 [s0;%% [*2 Public Member List]]}}&]
[s1; &]
[s2;:Upp`:`:MultiDimMatrixIndex`:`:SetNumAxis`(int`): [@(0.0.255) void]_[* SetNumAxis]([@(0.0.255) i
nt]_[*@3 numAxis])&]
[s3;%% Sets the number of dimensions with [%-*@3 numAxis].&]
[s4;%% &]
[s1; &]
[s2;:Upp`:`:MultiDimMatrixIndex`:`:SetAxisDim`(int`,int`): [@(0.0.255) void]_[* SetAxisDi
m]([@(0.0.255) int]_[*@3 axis], [@(0.0.255) int]_[*@3 dim])&]
[s3;%% Sets the size [%-*@3 dim ]of the dimension [%-*@3 axis].&]
[s4;%% &]
[s1; &]
[s2;:Upp`:`:MultiDimMatrixIndex`:`:SetAxis1D`(int`): [@(0.0.255) void]_[* SetAxis1D]([@(0.0.255) i
nt]_[*@3 dimX])&]
[s3;%% Sets the size [%-*@3 dimX] of unidimensional vector.&]
[s4;%% &]
[s1; &]
[s2;:Upp`:`:MultiDimMatrixIndex`:`:SetAxis2D`(int`,int`): [@(0.0.255) void]_[* SetAxis2D](
[@(0.0.255) int]_[*@3 dimX], [@(0.0.255) int]_[*@3 dimY])&]
[s3;%% Sets with [%-*@3 dimX] and [%-*@3 dimY] the sizes of a bidimensional 
vector.&]
[s4;%% &]
[s1; &]
[s2;:Upp`:`:MultiDimMatrixIndex`:`:GetAxisDim`(`): [_^Upp`:`:Vector^ Upp`::Vector]<[@(0.0.255) i
nt]>_`&[* GetAxisDim]()&]
[s3;%% Gets the array of dimensions including the size of each.&]
[s4; &]
[s1; &]
[s2;:Upp`:`:MultiDimMatrixIndex`:`:GetIndex`(const Upp`:`:Vector`<int`>`&`)const: [@(0.0.255) i
nt]_[* GetIndex]([@(0.0.255) const]_[_^Upp`:`:Vector^ Vector]<[@(0.0.255) int]>_`&[*@3 inde
x])_[@(0.0.255) const]&]
[s3;%% Gets the index in the storage of a multidimensional matrix 
of index included in Vector [%-*@3 index.].&]
[s4;%% &]
[s1; &]
[s2;:Upp`:`:MultiDimMatrixIndex`:`:GetIndex`(T`,Args`.`.`.args`)const: [@(0.0.255) temp
late]_<[@(0.0.255) typename]_[*@4 T], [@(0.0.255) typename...]_[*@4 Args]>_[@(0.0.255) int]_
[* GetIndex]([*@4 T]_[*@3 t], [*@4 Args][@(0.0.255) ...]_args)_[@(0.0.255) const]&]
[s3; [%% Gets the index in the storage of a multidimensional matrix 
of indexes ][*@3 t (t0, t1, ...).]&]
[s4;%% &]
[s1; &]
[s2;:Upp`:`:MultiDimMatrixIndex`:`:GetIndex`(int`,int`)const: [@(0.0.255) int]_[* GetInde
x]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y])_[@(0.0.255) const]&]
[s3;%% Gets the index in the storage of a bidimensional matrix of 
index [%-*@3 x] and [%-*@3 y].&]
[s4;%% &]
[s1; &]
[s2;:Upp`:`:MultiDimMatrixIndex`:`:IsValid`(int`,int`)const: [@(0.0.255) bool]_[* IsValid
]([@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y])_[@(0.0.255) const]&]
[s3;%% Gets if the bidimensional matrix index [%-*@3 x] and [%-*@3 y 
]is inside bounds..&]
[s4;%% &]
[s1; &]
[s2;:Upp`:`:MultiDimMatrixIndex`:`:GetNumAxis`(`)const: [@(0.0.255) int]_[* GetNumAxis]()
_[@(0.0.255) const]&]
[s3;%% Gets the number of dimensions.&]
[s4; ]]