topic "Setup wizard classes";
[ $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9; $$1,0#37138531426314131252341829483380:structitem]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:codeitem]
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 Wizard]]}}&]
[s3;%- &]
[s1;:noref:%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T0],[@(0.0.255) class]_[*@4 T1],[@(0.0.255) c
lass]_[*@4 T2]_`=_NullStep,...>&]
[s1;:Wizard`:`:class:%- [@(0.0.255) class]_[* Wizard]_:_[@(0.0.255) public]_[*@3 TopWindow]&]
[s2; [%-* Wizard][%- _]is a template class aimed at simple creation of 
step`-by`-step procedures, often called `"wizards`". It supports 
variable number of template parameters, ranging from 2 to 10, 
which are then placed into the Wizard Layout as the steps. If 
you need more than 10 steps, you should really consider redesigning 
your wizard, since it might get annoying for the user.&]
[s2;*%- &]
[s2; The recommended usage is something like&]
[s2; [*C@5;1 class MyWiz : public Wizard< WithStep1Layout<ParentCtrl>,]&]
[s2; [*C@5;1                              WithStep2Layout<ParentCtrl>]&]
[s2; [*C@5;1                              ...]&]
[s2; [*C@5;1                            >`{]&]
[s2; [*C@5;1 public:]&]
[s2; [*C@5;1 -|-|MyWiz()`{]&]
[s2; [*C@5;1 -|-|-|WhenFinish`=THISBACK(Perform);]&]
[s2; [*C@5;1 -|-|-|// initialize the widgets in s0`-s9 as necessary]&]
[s2; [*C@5;1 -|-|`}]&]
[s2; [*C@5;1 -|-|void Perform()`{]&]
[s2; [*C@5;1 -|-|-|// perform actions based on how the user set the values 
of widgets in s0`-s9]&]
[s2; [*C@5;1 -|-|`}]&]
[s2; [*C@5;1 `}]&]
[s0; &]
[s3;%- &]
[s0;2 &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3;%- &]
[s4;%- &]
[s5;:Wizard`:`:s9:%- T0_[* s0 ]`-[*  ]T9_[* s9]&]
[s2; Those are instances of the steps, whose types were passed as 
the templates parameters for Wizard#. They can be used to modify 
the steps at runtime just like any other Ctrl.&]
[s3;%- &]
[s3;%- &]
[s4;%- &]
[s5;:Wizard`:`:WhenFinish:%- [_^topic`:`/`/Core`/src`/Callbacks`$en`-us^ Callback]_[* Whe
nFinish]&]
[s2; Function to execute when user clicks on Finish button. This 
function should apply all the changes that user made in the wizard.&]
[s3;%- &]
[s4;%- &]
[s5;:Wizard`:`:WhenCancel:%- [_^Callback^ Callback]_[* WhenCancel]&]
[s2; Function to execute when user clicks on Cancel button. This 
function should clean up every change the wizard performed so 
far, so that the resulting state is as if the wizard was never 
run.&]
[s3;%- &]
[s4;%- &]
[s5;:Wizard`:`:WhenStep:%- [_^topic`:`/`/Core`/src`/Callbacks`$en`-us^ Callback1]<[@(0.0.255) i
nt]>_[* WhenStep]&]
[s2; Function executed whenever step is changed. The single argument 
contains the index of step that is about to be shown. Useful 
for manipulating the flow of steps, e.g. based on users prior 
actions.&]
[s3;%- &]
[s4;%- &]
[s5;:Wizard`:`:prev:%- [_^topic`:`/`/CtrlLib`/src`/Button`$en`-us`#Button`:`:class^ But
ton]_[* prev]&]
[s5;:Wizard`:`:next:%- [_^topic`:`/`/CtrlLib`/src`/Button`$en`-us`#Button`:`:class^ But
ton]_[* next]&]
[s5;:Wizard`:`:finish:%- [_^topic`:`/`/CtrlLib`/src`/Button`$en`-us`#Button`:`:class^ B
utton]_[* finish]&]
[s5;:Wizard`:`:cancel:%- [_^topic`:`/`/CtrlLib`/src`/Button`$en`-us`#Button`:`:class^ B
utton]_[* cancel]&]
[s2; The buttons for navigation in the wizard. By default, they should 
behave very intuitively, but if necessary, their callbacks can 
be overridden to provide different behavior.&]
[s3;%- &]
[s4;%- &]
[s5;:Wizard`:`:GetCount`(`):%- [@(0.0.255) int]_[* GetCount]()&]
[s2; Returns the number of steps (that is the number of parameters 
passed to the template when creating the Wizard).&]
[s3;%- &]
[s4;%- &]
[s5;:Wizard`:`:NextStep`(`):%- [@(0.0.255) void]_[* NextStep]()&]
[s2; Loads next step. Default callback for the next button.&]
[s3;%- &]
[s4;%- &]
[s5;:Wizard`:`:PrevStep`(`):%- [@(0.0.255) void]_[* PrevStep]()&]
[s2; Loads previous step. Default callback for the prev button&]
[s3;%- &]
[s4;%- &]
[s5;:Wizard`:`:SetStep`(int`):%- [@(0.0.255) void]_[* SetStep]([@(0.0.255) int]_[*@3 n])&]
[s2; Loads step [%-*@3 n].&]
[s3; &]
[s4;%- &]
[s5;:Wizard`:`:Block`(int`,bool`):%- [@(0.0.255) void]_[* Block]([@(0.0.255) int]_[*@3 n], 
[@(0.0.255) bool]_[*@3 block][@(0.0.255) `=true])&]
[s2; Temporarily enables or disables (based on value of [%-*@3 block]) 
step [%-*@3 n].&]
[s3; &]
[s4;%- &]
[s5;:Wizard`:`:Unblock`(int`):%- [@(0.0.255) void]_[* Unblock]([@(0.0.255) int]_[*@3 n])&]
[s2; Enables step [%-*@3 n] if it was previously disabled by Block([%-*@3 n]).&]
[s3; &]
[s4;%- &]
[s5;:Wizard`:`:IsBlocked`(int`):%- [@(0.0.255) bool]_[* IsBlocked]([@(0.0.255) int]_[*@3 n])&]
[s2; Returns true if step [%-*@3 n] is blocked, false otherwise.&]
[s0; ]