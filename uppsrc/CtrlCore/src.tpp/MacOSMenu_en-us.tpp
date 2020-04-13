topic "MacOS menu support";
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
[ {{10000@(113.42.0) [s0; [*@7;4 MacOS menu support]]}}&]
[s0; &]
[s0; U`+`+ applications without changes to support MacOS application 
menu will run with menu inside the window.&]
[s0; &]
[s0; To properly support MacOS menu, application needs to provide 
different code for MacOS (conditional compilation depending on 
PLATFORM`_COCOA macro). Usually, the changes are minimal, e.g. 
in Uword application the only change is&]
[s0; &]
[s0; [*C #ifdef PLATFORM`_COCOA]&]
[s0; [*C -|SetMainMenu(THISBACK(MainMenu));]&]
[s0; [*C #else]&]
[s0; [*C -|AddFrame(menubar);]&]
[s0; [*C #endif]&]
[s0; &]
[s0; replacing the call to AddFrame to add the main menu bar with 
the call to TopWindow`::SetMainMenu.&]
[s0; &]
[s0; Other possibility is to omit AddFrame when PLATFORM`_COCOA is 
defined and change menu setting code (which is called on any 
main menu changes) to something like&]
[s0; &]
[s0; [*C #ifdef PLATFORM`_COCOA]&]
[s0; [*C -|SetMainMenu(THISBACK(MainMenu));]&]
[s0; [*C #else]&]
[s0; [*C -|menubar.Set(THISBACK(MainMenu));]&]
[s0; [*C #endif]&]
[s0; &]
[s0; Another MacOS feature is the dock menu. This can be set using 
TopWindow`::WhenDockMenu.&]
[s0; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* TopWindow members for MacOS style menu support]]}}&]
[s3;%- &]
[s5;:Upp`:`:TopWindow`:`:WhenDockMenu:%- [_^Upp`:`:Event^ Event]<Bar[@(0.0.255) `&]>_[* Whe
nDockMenu]&]
[s2; Represents application dock menu. Gets called if TopWindow is 
on top and user invokes the dock menu for application (e.g. by 
right`-click on application icon).&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:TopWindow`:`:SetMainMenu`(Upp`:`:Event`<Upp`:`:Bar`&`>`):%- [@(0.0.255) voi
d]_[* SetMainMenu]([_^Upp`:`:Event^ Event]<Bar[@(0.0.255) `&]>_[*@3 menu])&]
[s2; Sets the main application menu. This menu will be active if 
given toplevel TopWindow is on the top.&]
[s0; ]]