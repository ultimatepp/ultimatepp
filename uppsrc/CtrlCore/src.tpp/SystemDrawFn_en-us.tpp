topic "SystemDraw utility functions";
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
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 SystemDraw utility functions]]}}&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Function List]]}}&]
[s4;H0; &]
[s5;:SetSurface`(Draw`&`,const Rect`&`,const RGBA`*`,Size`,Point`): [@(0.0.255) void]_[* S
etSurface]([_^Draw^ Draw][@(0.0.255) `&]_[*@3 w], [@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&
]_[*@3 dest], [@(0.0.255) const]_[_^RGBA^ RGBA]_`*[*@3 pixels], [_^Size^ Size]_[*@3 srcsz], 
[_^Point^ Point]_[*@3 poff])&]
[s5;:SetSurface`(Draw`&`,int`,int`,int`,int`,const RGBA`*`): [@(0.0.255) void]_[* SetSurf
ace]([_^Draw^ Draw][@(0.0.255) `&]_[*@3 w], [@(0.0.255) int]_[*@3 x], [@(0.0.255) int]_[*@3 y],
 [@(0.0.255) int]_[*@3 cx], [@(0.0.255) int]_[*@3 cy], [@(0.0.255) const]_[_^RGBA^ RGBA]_`*[*@3 p
ixels])&]
[s2;%% This function transfers RGBA pixels from buffer to target 
Draw. If Draw represents window on the screen, function uses 
optimized algorithm, otherwise transfer is performed by copying 
pixels to Image and using DrawImage.&]
[s2;%% In the first variant, target area is [%-*@3 dest], [%-*@3 srcsz] 
is the size of source [%-*@3 pixels] buffer, [%-*@3 poff].is offset 
in source buffer (means, only part of source buffer can be copied).&]
[s2; [%% In the second variant, target area is ][*@3 x],[*@3 y],[*@3 cx],[*@3 cy] 
and the source buffer has to have the same size as target.&]
[s3; &]
[s0; ]]