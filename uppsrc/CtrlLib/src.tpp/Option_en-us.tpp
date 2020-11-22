topic "Option";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;ph2 $$9,9#13035079074754324216151401829390:normal]
[l321;C@5+75 $$10,10#20902679421464641399138805415013:code]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[ {{10000@3 [s0; [*@(229)4 Option]]}}&]
[s3;%- &]
[s1;:Option`:`:class:%- [@(0.0.255)3 class][3 _][*3 Option][3 _:_][@(0.0.255)3 public][3 _][*@3;3 Pu
sher]&]
[s0;%- &]
[s2; 
@@image:712&131
(A3IAFQAAAOv/AAAAAHic7ZhtioNADIa9U++059hTLsMigiwiYhFZytKfDQkVm8xk4li7FBJeZD7UZB6T/PA8hLPL5XK5DtDp9GHRv8f5XgJiX2ghhG+0pmnatv252zAMTvWJVIEnTKNUq0c7Ljx6+fr6AklHW10DsYAGAOu6Xidq9VmlqJb52nmWl1Hdf1IgRilKSJfCB6REdRxHp1pAFXgCPWDIkIIgUed5tlNlPUGOoyvyZrbCOoDSeeQ7ZbQpv8ydJWaFKqQoUSWwa6RbqbIt5XT6iuSgPBidWrykgrQ/q1ClDrDAJEHhA89ftIJctVBN3ZylKlMo5SWKLhukQjXlWlKVhT+iAc8LmpGqJQ2iMSvfKEo1dRYjVd2LTlUhKal2Xdf3PdQ7Q/qHdjTVrblqD8AyLqOazdUejbroUviE9Iq2vwPIrfVKllKqiqMhyS05ZWEog2zMWaqAdJomO9Uy2evofXXcfwCWtMav7HK5XC7XohsrJCxc)
 
@@image:1543&475
(A/cATAAAAAAAAAAAAHic7dpBbptAAIZR36mSz8NtOIzv4l3XXXWfG7Q2xYChhDBJBP55TyyIzeBJ9BkNKG9vAAAAAAAA0PvxMVtPE8rdAv6zROS8tNWRX6pTq7psN+2xa30+nevr1tNgl9ZFfm/p0fa1rjeu/PZ9EzbL1kW+r6z2NRt2a+Vy5b5YGYd1v7yPVjBNfpf29duL3SH92H7UZNmzdMLmJP2yqTloEPzc8Loaz4EgP3/9Hm3dW+tvPNuGBv089vuFcVPgZHdw7KDJS/Vc3dIJn17shnb788O7T37+Wk3/OLattk8WPvdu4dOVpvR7K8NVelNQ88N/8xvsDy6242v58gnf/ZR182FHijtfHFj8CLGtqTzymYczyyfsjhB5mrLOF4esvPGsh6V1a+PB8mCyGplfVMykNnfCx/ELn7JqPuzO9pGPb/ca/drj/bCf94crltFFfeaEVXUeHd7OZvbGc3E+7M4OIt+KMo/imyL/+P+unCZKfo0SIj+K74j8RYj8KA4cOUchcuKJnHgiJ57IiSdy4omceCInnsiJJ3LiiZx4IieeyIkncuKJnHgiJ57IiSdy4omceCInnsiJJ3LiiZx4IieeyIkncuKJnHgiJ57IiSdy4omceCInnsiJJ3LiiZx4IieeyIkncuKJnHgiJ57IiSdy4pVFrnNeRXGuOuclfDLUf8Nttp1vX/iVAQAAAACASH8BOEK7+g==)
&]
[s0; &]
[s9; Widget providing the selection of 2 or alternatively 3 states 
(true, false, Null). Value of Option is either 0, 1, or Null. 
When setting Value to Option, string `"1`" is interpreted as 
true state, other non`-Null strings as false. Depending on whether 
Option is in NotNull mode, Null is interpreted as false or Null 
state. 0 is always interpreted as false, Null number as either 
Null (NotNull mode) or false (NoNotNull mode), other numbers 
as true. The control may be also known under [^https`:`/`/en`.wikipedia`.org`/wiki`/Checkbox^ c
hecbox] name.&]
[s9;%- [%%/ Derived from ][/ Pusher.]&]
[s3;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Exemplary usage]]}}&]
[s9; Below is the exemplary usage of Option control with the detailed 
commentary:&]
[s10; &]
[s0;l321; [C@5+75 #include <CtrlLib/CtrlLib.h>]&]
[s10; &]
[s10; using namespace Upp;&]
[s10; &]
[s10; struct MyAppWindow : TopWindow `{&]
[s10; -|Option option;&]
[s10; -|&]
[s10; -|MyAppWindow() `{&]
[s10; -|-|SetRect(0, 0, 640, 480);&]
[s10; -|-|Add(option.SetLabel(`"Option`").LeftPosZ(20).TopPosZ(20));&]
[s10; -|-|&]
[s10; -|-|[@(28.127.0) // By default option is disabled `- to change that]&]
[s10; [@(28.127.0) -|-|// let`'s enable it using Set() method.]&]
[s10; [@(28.127.0) -|-|//]&]
[s10; [@(28.127.0) -|-|// Alternatives:]&]
[s10; [@(28.127.0) -|-|//     `- option.Set(true);]&]
[s10; [@(28.127.0) -|-|//     `- option.Set(static`_cast<int>(true));]&]
[s10; [@(28.127.0) -|-|//     `- option.SetData(`"1`");]&]
[s10; -|-|option.Set(1);&]
[s10; -|-|&]
[s10; -|-|[@(28.127.0) // Let`'s react when option value will be change 
by the user.]&]
[s10; -|-|option << `[`=`] `{&]
[s10; -|-|-|Title(Format(`"Option (Check`-box) state changed to %d`", 
option.Get()));&]
[s10; -|-|`};&]
[s10; -|`}&]
[s10; `};&]
[s10; &]
[s10; GUI`_APP`_MAIN&]
[s10; `{&]
[s10; -|MyAppWindow app;&]
[s10; -|app.Run();&]
[s10; &]
[s10; -|if(app.option)&]
[s10; -|-|PromptOK(`"The option was enabled!`");&]
[s10; -|else&]
[s10; -|-|PromptOK(`"The option was disabled!`");&]
[s10; -|&]
[s10; -|[@(28.127.0) // Alternatives:]&]
[s10; [@(28.127.0) -|//     `- app.option `=`= 1                     
 // enabled]&]
[s10; [@(28.127.0) -|//     `- app.option `=`= 0                     
 // disabled]&]
[s10; [@(28.127.0) -|//     `- app.option `=`= static`_cast<int>(Null) 
// mixed (tri`-state mode)]&]
[s10; [@(28.127.0) -|//]&]
[s10; [@(28.127.0) -|//     `- bool enabled `= static`_cast<bool>(app.option);]&]
[s10; [@(28.127.0) -|//]&]
[s10; [@(28.127.0) -|// Also to obtain value Get() method could be used:]&]
[s10; [@(28.127.0) -|//     `- app.option.Get() `=`= 1]&]
[s10; [@(28.127.0) -|//     `- ...]&]
[s10; `}&]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3;%- &]
[s5;:Option`:`:Option`(`):%- [* Option]()&]
[s2; Initializes Option into NotNull, 2`-state mode, false value 
and standard appearance.&]
[s3; &]
[s4;%- &]
[s5;:Option`:`:`~Option`(`):%- [@(0.0.255) `~][* Option]()&]
[s2; Default destructor.&]
[s3;%- &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:Option`:`:Set`(int`):%- [_^Option^ Option][@(0.0.255) `&]_[* Set]([@(0.0.255) int]_[*@3 b])
&]
[s2; Sets Option to the specified state (0, 1 or Null).&]
[s7; [%-*C@3 b]-|State.&]
[s3;%- &]
[s4;%- &]
[s5;:Option`:`:Get`(`)const:%- [@(0.0.255) int]_[* Get]()_[@(0.0.255) const]&]
[s7; [*/ Return value]-|Current state of Option.&]
[s3; &]
[s4;%- &]
[s5;:Option`:`:operator int`(`)const:%- [* operator_int]()_[@(0.0.255) const]&]
[s7; [*/ Return value]-|[^topic`:`/`/CtrlLib`/src`/Option`$en`-us`#Option`:`:Get`(`)const^ G
et]()&]
[s3; &]
[s4;%- &]
[s5;:Option`:`:operator`=`(int`):%- [@(0.0.255) void]_[* operator`=]([@(0.0.255) int]_[*@3 b])
&]
[s2; Same as [^topic`:`/`/CtrlLib`/src`/Option`$en`-us`#Option`:`:Set`(int`)^ Set](b).&]
[s7; [%-*C@3 b]-|State.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Option`:`:EnableBox`(bool`):%- [@(0.0.255) void]_[* EnableBox]([@(0.0.255) bool
]_[*@3 b])&]
[s2; Enables/disables all widgets that are children of Option`'s 
parent and are intersecting Option rectangle.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Option`:`:EnableBox`(`):%- [@(0.0.255) void]_[* EnableBox]()&]
[s2; Enables/disables all widgets that are children of Option`'s 
parent and are intersecting Option rectangle based on the status 
of the Option.&]
[s3;%- &]
[s4;%- &]
[s5;:Option`:`:BlackEdge`(bool`):%- [_^Option^ Option][@(0.0.255) `&]_[* BlackEdge]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Activates visual appearance suitable for placing Option on white 
background (SColorPaper), like in list, as opposed to default 
representation suitable for placing it on dialog (SColorFace).&]
[s7; [%-*C@3 b]-|true to activate.&]
[s7; [*/ Return value]-|`*this for chaining.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Option`:`:IsBlackEdge`(`)const:%- [@(0.0.255) bool]_[* IsBlackEdge]()_[@(0.0.255) c
onst]&]
[s2; Returns true if BlackEdge is active.&]
[s3;%- &]
[s4;%- &]
[s5;:Option`:`:SwitchImage`(bool`):%- [_^Option^ Option][@(0.0.255) `&]_[* SwitchImage]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Activates visual representation that paints the Option with 
the Switch appearance.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Option`:`:IsSwitchImage`(`)const:%- [@(0.0.255) bool]_[* IsSwitchImage]()_[@(0.0.255) c
onst]&]
[s2; Returns true if SwitchImage is active.&]
[s3;%- &]
[s4;%- &]
[s5;:Option`:`:ThreeState`(bool`):%- [_^Option^ Option][@(0.0.255) `&]_[* ThreeState]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Activates three`-state mode. Also activates NoNotNull mode.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Option`:`:IsThreeState`(`)const:%- [@(0.0.255) bool]_[* IsThreeState]()_[@(0.0.255) c
onst]&]
[s2; Returns true if ThreeState is active.&]
[s3;%- &]
[s4;%- &]
[s5;:Option`:`:NotNull`(bool`):%- [_^Option^ Option][@(0.0.255) `&]_[* NotNull]([@(0.0.255) b
ool]_[*@3 nn]_`=_[@(0.0.255) true])&]
[s2; Activates NotNull mode `- Null Value assigned to Option (via 
SetData) is interpreted as false.&]
[s3; &]
[s4;%- &]
[s5;:Option`:`:NoNotNull`(`):%- [_^Option^ Option][@(0.0.255) `&]_[* NoNotNull]()&]
[s2; Activates NoNotNull mode `- Null Value assigned to Option is 
interpreted as Null.&]
[s7; [*/ Return value]-|`*this for chaining.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Option`:`:IsNotNull`(`)const:%- [@(0.0.255) bool]_[* IsNotNull]()_[@(0.0.255) c
onst]&]
[s2; Returns true if NotNull is active.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:Option`:`:SetColor`(Upp`:`:Color`):%- [_^Upp`:`:Option^ Option][@(0.0.255) `&
]_[* SetColor]([_^Upp`:`:Color^ Color]_[*@3 c])&]
[s2; Sets the color of text of Option label. Setting Null restores 
the default color.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Option`:`:Box`(bool`):%- [_^Upp`:`:Option^ Option][@(0.0.255) `&]_[* Box]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Changes the visual representation to `"checked box`".&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Option`:`:AutoBox`(bool`):%- [_^Upp`:`:Option^ Option][@(0.0.255) `&]_[* AutoBo
x]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Calls Box([%-*@3 b]) and activates mode where all widgets that 
are children of Option`'s parent and are intersecting Option 
rectangle are enabled / disabled based on the status of the Option.&]
[s3; &]
[s4;%- &]
[s5;:Option`:`:ShowLabel`(bool`):%- [_^Option^ Option][@(0.0.255) `&]_[* ShowLabel]([@(0.0.255) b
ool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Toggles the display of the option`'s label.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:Option`:`:IsShowLabel`(`)const:%- [@(0.0.255) bool]_[* IsShowLabel]()_[@(0.0.255) c
onst]&]
[s2; Returns true if ShowLabel is active.&]
[s0;%- &]
[s0;%- &]
[ {{10000@3 [s0; [*@(229)4 OptionBox]]}}&]
[s0;%- &]
[s1;:Upp`:`:OptionBox`:`:class:%- [@(0.0.255)3 class][3 _][*3 OptionBox][3 _:_][@(0.0.255)3 publ
ic][3 _][*@3;3 Option]&]
[s2; Option with Box modifier active.&]
[s0;%- ]]