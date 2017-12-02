topic "StarIndicator";
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
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 StarIndicator]]}}&]
[s3; &]
[s1;:StarIndicator`:`:class: [@(0.0.255)3 class][3 _][*3 StarIndicator][3 _:_][@(0.0.255)3 publi
c][3 _][*@3;3 ProgressIndicator]&]
[s0;i448;a25;kKO9;@(0.0.255) &]
[s9; [/ Derived from ][/^topic`:`/`/CtrlLib`/src`/ProgressInd`$en`-us`#ProgressIndicator`:`:class^ P
rogressIndicator][/ . Part of ][/^topic`:`/`/Controls4U`/src`/Controls4U`$en`-us^ Con
trols4U][/  library.]&]
[s9; &]
[s9; StarIndicator is a kind of ProgressIndicator allowing to show 
the popularity of something, as we can see on some merchant web 
sites. It indicates the popularity with stars, and shows with 
text the exact rate (with one decimal) of the popularity and 
the number of votes.&]
[s9; &]
[s9; Colors are used to show the confidence we can give to the grade, 
according to the number of votes :&]
[s9;i150;O0; Light blue if undefined,&]
[s9;i150;O0; Red if less than the value defined in the variable [*/ m`_nVotesLow],&]
[s9;i150;O0; Yellow if more than the value defined in the variable 
[*/ m`_nVotesLow] and less than the value defined in the variable 
[*/ m`_nVotesHigh],&]
[s9;i150;O0; Light Green if more than the value defined in the variable 
[* m`_nVotesHigh]&]
[s9; &]
[s9; As seen in layout designer :&]
[s9; -|
@@PING:3150&633
(iVBORw0KGgoAAAANSUhEUgAAAXoAAABMCAYAAABj9HyUAAAACXBIWXMAAAsTAAALFAFP2wrfAAABY0lEQVR4nO3ZsW0DMRQFQdK4wq40lfY7kwMDDhT7QGA9EzJ6YLABuWfmvQDI+jo9AIBnXWutNTOHZwDwhPu+f0K/1lqv1+vgFACeMDOebgDqrs+Dt6/ZA1w68Pf23mstn7EAeUIPECf0AHFCDxAn9ABxQg8QJ/QAcUIPECf0AHFCDxAn9ABxQg8QJ/QAcUIPECf0AHFCDxAn9ABxQg8QJ/QAcUIPECf0AHFCDxAn9ABxQg8QJ/QAcUIPECf0AHFCDxAn9ABxQg8QJ/QAcUIPECf0AHFCDxAn9ABxQg8QJ/QAcUIPECf0AHFCDxAn9ABxQg8QJ/QAcUIPECf0AHFCDxAn9ABxQg8QJ/QAcUIPECf0AHFCDxAn9ABxQg8QJ/QAcUIPECf0AHFCDxAn9ABxQg8QJ/QAcUIPECf0AHFCDxAn9ABxQg8QJ/QAcUIPEHd9Hux9YsZ/59KB5/yGfmYOzgDgKd9Iww/k/zAUKQAAAABJRU5ErkJggg==)
&]
[s9; As seen in program :&]
[s9; -|
@@PING:3108&567
(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)
&]
[s9; -|
@@PING:3108&567
(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)
&]
[s9; &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor detail]]}}&]
[s4; &]
[s5;:StarIndicator`:`:StarIndicator`(`): [* StarIndicator()]&]
[s2; Initializes StarIndicator with the default behavior :&]
[s2;i150;O0; The text will be displayed when the mouse goes over 
the control,&]
[s2;i150;O0; The text will be black,&]
[s2;i150;O0; The total will be shown,&]
[s2;i150;O0; The number of votes will be shown,&]
[s2;i150;O0; The number of votes is 0,&]
[s2;i150;O0; The minimal number of votes is 5 (for yellow stars),&]
[s2;i150;O0; The optimal number of votes is 50 (for green stars).&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s4; &]
[s5;:StarIndicator`:`:AlwaysShowValue`(bool`): [@(0.0.255) virtual] 
[_ StarIndicator][@(0.0.255) `&] [* AlwaysShowValue]([@(0.0.255) bool]_[*@3 b])&]
[s2; If [*@3 b] is [/ true], the text will always appear. If not, it 
will appear only when the mouse is over the control.&]
[s7;i1120;a17; [*C@3 b]-|Indicates if the text must be always shown 
or not.&]
[s3; &]
[s4; &]
[s5;:StarIndicator`:`:Layout`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* Layout]()&]
[s2; This method is invoked when layout of StarIndicator has to be 
refreshed. This is usually either before window is displayed 
or when the StarIndicator is resized.&]
[s3; &]
[s4; &]
[s5;:StarIndicator`:`:MouseEnter`(Point`,dword`): [@(0.0.255) virtual] 
[@(0.0.255) void]_[* MouseEnter]([_^Point^ Point]_[*@3 p], [_^dword^ dword]_[*@3 keyflags])&]
[s2; This method is called by default implementation of MouseEvent 
when mouse cursor enters the view area of StarIndicator.&]
[s7;i1120;a17; [*C@3 p]-|Point of mouse cursor in view coordinates.&]
[s7;i1120;a17; [*C@3 keyflags]-|Combination of key flags for Shift, 
Ctrl and Alt keys.&]
[s3;  &]
[s4; &]
[s5;:StarIndicator`:`:MouseLeave`(`): [@(0.0.255) virtual] [@(0.0.255) void]_[* MouseLeave](
)&]
[s2;b17;a17; This method is called by default implementation of MouseEvent 
when mouse cursor leaves view area of StarIndicator.&]
[s3; &]
[s4; &]
[s5; [@(0.0.255) virtual] [@(0.0.255) void]_[* MouseMove]([_^`:`:Point^ Point]_[*@3 p], 
[_^`:`:dword^ dword]_[*@3 keyflags])&]
[s2;b17;a17; This method is called by default implementation of MouseEvent 
when mouse cursor hovers above view area of StarIndicator.&]
[s7;i1120;a17; [*C@3 p]-|Position of mouse cursor in view coordinates.&]
[s7;i1120;a17; [*C@3 keyflags]-|Combination of key flags for Shift, 
Ctrl and Alt keys.&]
[s3; &]
[s4; &]
[s5;:StarIndicator`:`:Paint`(Draw`&`): [@(0.0.255) virtual] [@(0.0.255) void]_[* Paint]([_^`:`:Draw^ D
raw][@(0.0.255) `&]_[*@3 draw])&]
[s2;b17;a17; This method is called when StarIndicator`'s view area 
is about to be repainted.&]
[s7;i1120;a17; [*C@3 draw]-|Target of draw operations.&]
[s0; &]
[s4; &]
[s5;:StarIndicator`:`:Set`(double`): [@(0.0.255) virtual] [@(0.0.255) void]_[* Set]([@(0.0.255) d
ouble]_[*@3 n])&]
[s2;b17;a17; Sets the new rate.&]
[s7;i1120;a17; [*C@3 n]-|New value.&]
[s3; &]
[s4; &]
[s5;:StarIndicator`:`:Set`(int`): [@(0.0.255) virtual] [@(0.0.255) void]_[* Set]([@(0.0.255) i
nt]_[*@3 n])&]
[s2;b17;a17; Sets the new rate as integer value.&]
[s7;i1120;a17; [*C@3 n]-|New value.&]
[s3; &]
[s4; &]
[s5;:StarIndicator`:`:Set`(int`,int`): [@(0.0.255) virtual] [@(0.0.255) void]_[* Set]([@(0.0.255) i
nt]_[*@3 n],[*@3  ][@(0.0.255) int]_[*@3 tot])&]
[s2;b17;a17; Sets the new rate as integer value and the maximum possible 
rate.&]
[s7;i1120;a17; [*C@3 n]-|New value.&]
[s7;i1120;a17; [*C@3 tot]-|New maximum rate.&]
[s3; &]
[s4; &]
[s5;:StarIndicator`:`:SetFontColor`(Color`): [@(0.0.255) virtual] [_ StarIndicator][@(0.0.255) `&
] [* SetFontColor]([_^`:`:Point^ Color]_[*@3 c])&]
[s2; Sets the color of the text.&]
[s7;i1120;a17; [*C@3 c]-|Color of the text.&]
[s3; &]
[s4; &]
[s5;:StarIndicator`:`:SetVotes`(int`): [@(0.0.255) virtual] [_ StarIndicator][@(0.0.255) `&
] [* SetVotes]([@(0.0.255) int]_[*@3 n])&]
[s2; Sets the number of votes.&]
[s7;i1120;a17; [*C@3 n]-|Number of votes.&]
[s3; &]
[s4; &]
[s5;:StarIndicator`:`:SetVotesHigh`(int`): [@(0.0.255) virtual] [_ StarIndicator][@(0.0.255) `&
] [* SetVotesHigh]([@(0.0.255) int]_[*@3 n])&]
[s2; Sets the minimal number of votes giving full confidence to the 
rate. The stars will be colored in light green if the number 
of votes is more than [*@3 n].&]
[s7;i1120;a17; [*C@3 n]-|Number of votes for full confidence.&]
[s3; &]
[s4; &]
[s5;:StarIndicator`:`:SetVotesLow`(int`): [@(0.0.255) virtual] [_ StarIndicator][@(0.0.255) `&
] [* SetVotesLow]([@(0.0.255) int]_[*@3 n])&]
[s2; Sets the minimal number of votes giving half confidence to the 
rate. The stars will be colored in yellow if the number of votes 
is more than [*@3 n], in red if lower than [*@3 n].&]
[s7;i1120;a17; [*C@3 n]-|Number of votes for half confidence.&]
[s3; &]
[s4; &]
[s5;:StarIndicator`:`:ShowPercent`(bool`): [@(0.0.255) virtual] [_ StarIndicator][@(0.0.255) `&
] [* ShowPercent]([@(0.0.255) bool]_[*@3 b])&]
[s2; Changes the way the text will display the rate : in percents 
if_[*@3 b] is true or in value if false. It will be displayed according 
to the choice set by the call of [* AlwaysShowValue]([@(0.0.255) bool]_[*@3 b]).&]
[s7;i1120;a17; [*C@3 b]-|Indicates if the text will display the rate 
in percents or in value.&]
[s3; &]
[s4; &]
[s5;:StarIndicator`:`:ShowTotal`(bool`): [@(0.0.255) virtual] [_ StarIndicator][@(0.0.255) `&
] [* ShowTotal]([@(0.0.255) bool]_[*@3 b])&]
[s2; The maximum rate value can be displayed or not, according to 
[*@3 b]. It will be displayed according to the choice set by the 
call of [* AlwaysShowValue]([@(0.0.255) bool]_[*@3 b]).&]
[s7;i1120;a17; [*C@3 b]-|If [/ true], the total (maximum rate value) will 
be displayed.&]
[s3; &]
[s4; &]
[s5;:StarIndicator`:`:ShowVotes`(bool`): [@(0.0.255) virtual] [_ StarIndicator][@(0.0.255) `&
] [* ShowVotes]([@(0.0.255) bool]_[*@3 b])&]
[s2; The number of votes can be displayed or not, according to [*@3 b]. 
It will be displayed according to the choice set by the call 
of [* AlwaysShowValue]([@(0.0.255) bool]_[*@3 b]).&]
[s7;i1120;a17; [*C@3 b]-|If [/ true], the number of votes will be displayed.&]
[s3;%- ]]