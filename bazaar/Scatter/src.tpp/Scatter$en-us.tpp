topic "Scatter Reference";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 Scatter]]}}&]
[s0;i448;a25;kKO9;@(0.0.255) &]
[ {{10000F(128)G(128)@1 [s0;%% [* Function List]]}}&]
[s4; &]
[s5;:Scatter`:`:FitToData`(bool`): [@(0.0.255) void]_[* FitToData]([@(0.0.255) bool]_[*@3 Y]_
`=_[@(0.0.255) false])&]
[s0;l288;%% Changes X axis zoom to fit visible all data in Ctrl.&]
[s0;l288;i448;a25;kKO9;%% If [%-*@3 Y ]is true, Y axis zoom is also 
changed to fit the data.&]
[s4; &]
[s5;:Scatter`:`:SetRange`(double`,double`,double`): [_^Scatter^ Scatter][@(0.0.255) `&]_[* S
etRange]([@(0.0.255) double]_[*@3 rx], [@(0.0.255) double]_[*@3 ry], 
[@(0.0.255) double]_[*@3 ry2]_`=_`-[@3 1])&]
[s2;%% Sets visible data range for X axis in [%-*@3 rx], Y axis in 
[%-*@3 ry] and secondary Y axis in [%-*@3 ry2].&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:GetXRange`(`)const: [@(0.0.255) double]_[* GetXRange]()[@(0.0.255) const]&]
[s2;%% Gets X axis visible data range.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:GetYRange`(`)const: [@(0.0.255) double]_[* GetYRange]()[@(0.0.255) const]&]
[s2;%% Gets Y axis visible data range.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:GetY2Range`(`)const: [@(0.0.255) double]_[* GetY2Range]()[@(0.0.255) const]&]
[s2;%% Gets secondary Y axis visible data range.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:SetMajorUnits`(double`,double`): [_^Scatter^ Scatter][@(0.0.255) `&]_[* Set
MajorUnits]([@(0.0.255) double]_[*@3 ux], [@(0.0.255) double]_[*@3 uy])&]
[s2;%% Sets the space between grid lines, horizontal grid in [%-*@3 ux 
]and vertical grid in [%-*@3 uy].&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:GetMajorUnitsX`(`): [@(0.0.255) double]_[* GetMajorUnitsX]()&]
[s2;%% Gets the space between horizontal grid lines.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:GetMajorUnitsY`(`): [@(0.0.255) double]_[* GetMajorUnitsY]()&]
[s2;%% Gets the space between vertical grid lines.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:SetMinUnits`(double`,double`): [_^Scatter^ Scatter][@(0.0.255) `&]_[* SetMi
nUnits]([@(0.0.255) double]_[*@3 ux], [@(0.0.255) double]_[*@3 uy])&]
[s2;%% Sets the distance from the leftmost vertical grid to origin 
([%-*@3 ux]) and from bottommost horizontal grid to origin ([%-*@3 uy]).&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:SetXYMin`(double`,double`,double`): [_^Scatter^ Scatter][@(0.0.255) `&]_[* S
etXYMin]([@(0.0.255) double]_[*@3 xmin],[@(0.0.255) double]_[*@3 ymin],[@(0.0.255) double]_
[*@3 ymin2]_`=_[@3 0])&]
[s2;%% Sets the coordinate of the first visible point in the leftmost, 
bottommost corner of control in X axis coordinates as [%-*@3 xmin], 
Y axis coordinates as [%-*@3 ymin] and secondary Y axis coordinates 
as [%-*@3 ymin2].&]
[s3;%% &]
[s4; &]
[s5;:Scatter`:`:GetXMin`(`)const: [@(0.0.255) double]_[* GetXMin]_()_[@(0.0.255) const]&]
[s2;%% Gets X axis coordinate of the first visible point in the leftmost 
side of control.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:GetYMin`(`)const: [@(0.0.255) double]_[* GetYMin]_()_[@(0.0.255) const]&]
[s2;%% Gets Y axis coordinate of the first visible point in the bottommost 
side of control.&]
[s3; &]
[s4; &]
[s5;:Scatter`:`:GetYMin2`(`)const: [@(0.0.255) double]_[* GetYMin2]_()_[@(0.0.255) const]&]
[s2;%% Gets secondary Y axis coordinate of the first visible point 
in the bottommost side of control.&]
[s3; &]
[s0; ]