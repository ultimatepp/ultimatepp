topic "";
[ $$0,0#00000000000000000000000000000000:Default]
[H6;0 $$1,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$2,0#37138531426314131252341829483370:codeitem]
[l288;2 $$3,0#27521748481378242620020725143825:desc]
[0 $$4,0#96390100711032703541132217272105:end]
[i448;a25;kKO9; $$5,0#37138531426314131252341829483380:structitem]
[{_} 
[s1; &]
[s5;:CMeter`:`:CVMark`:`:class: [@(0.0.255) class]_[* CVMark]_:_[@(0.0.255) private]_[*@3 Move
able]<[* CVMark]>_&]
[s3;%% markers shown at side of the meter, independant from meter 
value&]
[s4; &]
[s2;:CMeter`:`:FindStep`(float`)const: [@(0.0.255) int]_[* FindStep]([@(0.0.255) float]_[*@3 v
])_[@(0.0.255) const]&]
[s3;%% find value [%-*@3 v].in the steps vector and return index if 
found&]
[s4;%% &]
[s1; &]
[s2;:CMeter`:`:GRADIENT: [@(0.0.255) enum]_[* GRADIENT]&]
[s3;%% a static gradient, a lice of which is displayed&]
[s4; &]
[s1; &]
[s2;:CMeter`:`:STATIC: [@(0.0.255) enum]_[* STATIC]&]
[s3;%% static color bar&]
[s4; &]
[s1; &]
[s2;:CMeter`:`:IMGCACHE: [@(0.0.255) enum]_[* IMGCACHE]_`=_[@3 0]&]
[s3;%% a fancy bar gradient with 2 colors, actually generated and 
stored in an Image&]
[s4; &]
[s5;:CMeter`:`:class: [@(0.0.255) class]_[* CMeter]_:_[@(0.0.255) public]_[*@3 Ctrl]&]
[s3;%% drawing a user manipulatable bar, can be used as meter, slider, 
progressbar, in every direction and orientation. additional behaviour: 
provide steps on which the bar should be changed only. specify 
some value marks to be drawn aside, with some values/ text. Linear 
stepping is also available, when provided steps, so you can specify 
own advancing behaviour. different styles supported.&]
[s4; &]
[s1; &]
[s2;:CMeter`:`:DYNGRADIENT: [@(0.0.255) enum]_[* DYNGRADIENT]&]
[s3;%% a gradient drawn over currently applied bar&]
[s4; &]
[s0; ]