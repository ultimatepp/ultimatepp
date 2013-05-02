topic "1.1 MouseBehaviour";
[ $$0,0#00000000000000000000000000000000:Default]
[0 $$1,0#96390100711032703541132217272105:end]
[i448;a25;kKO9;2 $$2,0#37138531426314131252341829483380:class]
[b42;2 $$3,3#13035079074754324216151401829390:normal]
[i448;a25;kKO9;2 $$4,0#37138531426314131252341829483370:item]
[l288;2 $$5,0#27521748481378242620020725143825:desc]
[H6;0 $$6,0#05600065144404261032431302351956:begin]
[{_} 
[s1; &]
[ {{10000@3 [s0;%% [*@(229)4 ScatterCtrl`::MouseBehavior]]}}&]
[s1;%% &]
[s2;:ScatterCtrl`:`:MouseBehaviour`:`:struct: [@(0.0.255)3 struct][3 _][*3 MouseBehavior]&]
[s3;%% This structure is used to describe the behavior of the mouse 
when used in ScatterCtrl. It includes the keyboard and mouse 
conditions that, when complied, will launch the indicated action.&]
[s3;%% It is used by SetMouseBehavior(MouseBehaviour `*`_mouseBehavior) 
function to set an array of MouseBehavior items that will be 
used to launch actions as zooming or scrolling when mouse is 
used.&]
[s3;%% The default array is in ScatterCtrl`::MouseBehaviour defaultMouse.&]
[s1;%% &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Member List]]}}&]
[s1;%% &]
[s4;:ScatterCtrl`:`:MouseBehaviour`:`:ctrl: [@(0.0.255) bool]_[* ctrl]&]
[s5;%% Set to true if Ctrl has to be pressed.&]
[s1;%% &]
[s6; &]
[s4;:ScatterCtrl`:`:MouseBehaviour`:`:alt: [@(0.0.255) bool]_[* alt]&]
[s5;%% Set to true if Alt has to be pressed.&]
[s1; &]
[s6; &]
[s4;:ScatterCtrl`:`:MouseBehaviour`:`:shift: [@(0.0.255) bool]_[* shift]&]
[s5;%% Set to true if Shift has to be pressed.&]
[s1; &]
[s6; &]
[s4;:ScatterCtrl`:`:MouseBehaviour`:`:left: [@(0.0.255) bool]_[* left]&]
[s5;%% Set to true if mouse left button has to be pressed.&]
[s1; &]
[s6; &]
[s4;:ScatterCtrl`:`:MouseBehaviour`:`:middle: [@(0.0.255) bool]_[* middle]&]
[s5;%% Set to true if mouse middle button has to be pressed.&]
[s1; &]
[s6; &]
[s4;:ScatterCtrl`:`:MouseBehaviour`:`:middleWheel: [@(0.0.255) int]_[* middleWheel]&]
[s5;%% Set to true if mouse middle wheel has to be rolled.&]
[s1; &]
[s6; &]
[s4;:ScatterCtrl`:`:MouseBehaviour`:`:right: [@(0.0.255) bool]_[* right]&]
[s5;%% Set to true if mouse right button has to be pressed.&]
[s1; &]
[s6; &]
[s4;:ScatterCtrl`:`:MouseBehaviour`:`:action: MouseAction_[* action]&]
[s5;%% Is the action to be launched if the previous conditions are 
complied. It can be:&]
[s5;i150;O0;~~~1248;%% NO`_ACTION-|No action. It serves to mark the 
end of MouseBehavior array.&]
[s5;i150;O0;~~~1248;%% SCROLL-|Scrolls the graphs.&]
[s5;i150;O0;~~~1248;%% ZOOM`_H`_ENL-|Zooms horizontally enlarging 
the graphs. &]
[s5;i150;O0;~~~1248;%% ZOOM`_H`_RED-|Zooms horizontally reducing the 
graphs.&]
[s5;i150;O0;~~~1248;%% ZOOM`_V`_ENL-|Zooms vertically enlarging the 
graphs.&]
[s5;i150;O0;~~~1248;%% ZOOM`_V`_RED-|Zooms horizontally reducing the 
graphs.&]
[s5;i150;O0;~~~1248;%% SHOW`_INFO-|Shows an info label including mouse 
real X and Y coordinates.&]
[s1;%% &]
[s0;%% ]]