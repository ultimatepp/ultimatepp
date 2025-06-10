topic "Frame";
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
[ {{10000t/25b/25@3 [s0;%- [*@(229)4 Frame]]}}&]
[s9; [^topic`:`/`/CtrlCore`/srcdoc`/AboutFrames`$en`-us^ Frames] are 
objects derived from CtrlFrame class that form appearance and 
functionality of area between outer Ctrl border and its view.&]
[s3; &]
[s0; &]
[s0; &]
[s0;%- &]
[ {{10000t/25b/25@3 [s0;%- [*@(229)4 CtrlFrame]]}}&]
[s3; &]
[s1;:CtrlFrame`:`:class:%- [@(0.0.255)3 class][3 _][*3 CtrlFrame]&]
[s9; Interface definition of frame classes.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3;%- &]
[s5;:CtrlFrame`:`:`~CtrlFrame`(`):%- [@(0.0.255) `~][* CtrlFrame]()&]
[s2; Empty virtual destructor.&]
[s3; &]
[s4;%- &]
[s5;:CtrlFrame`:`:FrameLayout`(Rect`&`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* FrameL
ayout]([_^Rect^ Rect][@(0.0.255) `&]_[*@3 r])_`=_[@3 0]&]
[s2; Frame reacts to this method by defining its own layout (if needed) 
and reducing the size of Ctrl view rectangle.&]
[s7; [%-*C@3 r]-|Reference to current Ctrl rectangle. When Ctrl recomputes 
its layout, it starts with Rect equivalent to its external size 
(GetRect().Size()). Then it calls FrameLayout of all its frames 
(starting with frame 0) and resulting Rect is the size of Ctrl`'s 
view.&]
[s3; &]
[s4;%- &]
[s5;:CtrlFrame`:`:FrameAddSize`(Size`&`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* Frame
AddSize]([_^Size^ Size][@(0.0.255) `&]_[*@3 sz])_`=_[@3 0]&]
[s2; Adds size of the frame to the current external size of Ctrl. 
This is used to compute the external size of Ctrl for given size 
of view.&]
[s7; [%-*C@3 sz]-|Reference to actual size of Ctrl.&]
[s3; &]
[s4;%- &]
[s5;:CtrlFrame`:`:FramePaint`(Draw`&`,const Rect`&`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* FramePaint]([_^Draw^ Draw][@(0.0.255) `&]_[*@3 w], 
[@(0.0.255) const]_[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r])&]
[s2; Paint the frame. Default implementation is empty.&]
[s7; [%-*C@3 w]-|Draw.&]
[s7; [%-*C@3 r]-|Outer rectangle of the frame (this is the same rectangle 
as was given in last FrameLayout).&]
[s3; &]
[s4;%- &]
[s5;:CtrlFrame`:`:FrameAdd`(Ctrl`&`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* FrameAdd](
[_^Ctrl^ Ctrl][@(0.0.255) `&]_[*@3 parent])&]
[s2; Called when frame is added to the Ctrl. Frame can use it to 
add its subctrls to the parent. Default implementation is empty.&]
[s7; [%-*C@3 parent]-|Parent Ctrl.&]
[s3; &]
[s4;%- &]
[s5;:CtrlFrame`:`:FrameRemove`(`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* FrameRemove](
)&]
[s2; Called when frame is removed from the Ctrl. Frame can use it 
to remove subctrls from its parent. Default implementation is 
empty. &]
[s3; &]
[s4;%- &]
[s5;:CtrlFrame`:`:OverPaint`(`)const:%- [@(0.0.255) virtual] [@(0.0.255) int]_[* OverPaint](
)_[@(0.0.255) const]&]
[s2; This method can returns non`-zero number that represents paint 
extension margin of Ctrl rectangle `- frame can paint over this 
margin despite that fact that it does not belong to the Ctrl 
rectangle. This is useful to represent some specific skinning 
effect (like glare around the EditField). Default implementation 
returns zero.  &]
[s3; &]
[s0; &]
[s0; &]
[s0; &]
[ {{10000t/25b/25@3 [s0;%- [*@(229)4 Standard static frames]]}}&]
[s9; U`+`+ defines several standard static frames. Those frames are 
mostly used to define (or alter) appearance of border of Ctrls. 
All of them are obtained as a reference to single global instance 
by single global function and can be assigned to unlimited number 
of Ctrls.&]
[s9; Appearance of some of them can be altered by current OS look`&feel.&]
[s3; &]
[s0; &]
[ {{3967:1202:1908:2923h1;@(204) [s0; Function]
:: [s0; altered by look`&feel]
:: [s0; Appearance]
:: [s0; Comment]
::@2 [s0; CtrlFrame`&_[* NullFrame]()]
:: [s0; No.]
:: [s0; 
@@image:612&237
(A2IAJgAAANr/AAAAAHic7dBBCQAgAMDA/g2tINhBKwzBlzcuwdYcCwAAntmf5VLJpZJLJZdKLpVcKrlUcqnkUsmlkksll0oulVwquVRyqeRSyaWSS6W7SwAAEB2VfOL1)
]
:: [s0; [1 Default Frame for Ctrl.]]
:: [s0; CtrlFrame`&_[* InsetFrame]()]
:: [s0; No.]
:: [s0; 
@@image:612&237
(A2IAJgAAANr/AAAAAHic7dHBDYAgFERB++/ADjxYkC2Y0ANawgaiRJnNXDnwfjmPAgAAj9m3dR611uZKyxy7P9tZqU4wlZKplEylZColUymZSslUSqZSMpWSqZRMpWQqJVMpmUrJVErWWant4Rc1Vxp94bc3/FIAAPzbBXs9SD0=)
]
:: [s0; ]
:: [s0; CtrlFrame`&_[* OutsetFrame]()]
:: [s0; No.]
:: [s0; 
@@image:612&237
(A2IAJgAAANr/AAAAAHic7dKxDYNAEEXB678Dd0DggtwCEj1gOuDpILt5mnSTrz323wEAALxhjDF9e67Rd/tY6TYrlaxUslLJSiUrlaxUslLJSiUrlaxUslLJSiUrlaxUslLp4UrX+SKmVxqLNf1LAABQ/AG7lTfP)
]
:: [s0; ]
:: [s0; CtrlFrame`&_[* ButtonFrame]()]
:: [s0; No.]
:: [s0; 
@@image:612&237
(A2IAJgAAANr/AAAAAHic7dGxDYAwEATB778DOiCgIDJiJPcAlHCyBQSe06Sf7LdzbwAA8JprplVVd6Vjjm3rMljp5x9/MpWSqZRMpWQqJVMpmUrJVEqmUjKVkqmUTKVkKiVTKZlKyVRKNljpOZ9Ed6WabH2VAAAgdAN14TM/)
]
:: [s0; ]
:: [s0; CtrlFrame`&_[* ThinInsetFrame]()]
:: [s0; No.]
:: [s0; 
@@image:612&237
(A2IAJgAAANr/AAAAAHic7dBBDYAwAARB/xJwwANBWCCph2Jh0wR4MJcRcNlxnQMAAB5z7Nt/zDmXK80/TaUylcpUKlOpTKUylcpUKlOpTKUylcpUKlOpTKUylcpUKlOpTKWytUpfv357a5UAACC6AbZWe+U=)
]
:: [s0; ]
:: [s0; CtrlFrame`&_[* ThinOutsetFrame]()]
:: [s0; No.]
:: [s0; 
@@image:612&237
(A2IAJgAAANr/AAAAAHic7dAxDYBAAARB/xJwQIEgLJC8B8ABm0+ofi7TXrPjOgcAAPzmXmnHvqn0OZXKVCpTqUylMpXKVCpTqUylMpXKVCpTqUylMpXKVCpTqUylMpXKpiu9x6XMVQIAgOgB/DB5xQ==)
]
:: [s0; ]
:: [s0; CtrlFrame`&_[* BlackFrame]()]
:: [s0; No.]
:: [s0; 
@@image:612&237
(A2IAJgAAANr/AAAAAHic7dBRDYAwFMBA/CvEAsk8gIVmC1/vmlPQ9dwLAAB+cw1r+9I7JpdKLpVcKrlUcqnkUsmlkksll0oulVwquVRyqeRSyaWSSyWXSieXRrV3CQAAog9J6pkj)
]
:: [s0; ]
:: [s0; CtrlFrame`&_[* FieldFrame]()]
:: [s0; Yes.]
:: [s0; 
@@image:612&237
(A2IAJgAAANr/AAAAAHic7dBBDYAwAMBAx/jADHqwQDIPzEIzwmvXnIKO5x4AAPCb47y2snzp3SaXSi6VXCq5VHKp5FLJpZJLJZdKLpVcKrlUcqnkUsmlkksll0pfLm1l7RIAAEQTozHbhg==)
]
:: [s0; [1 Good for borders of all Ctrls that display somthing, like EditField 
or ArrayCtrl.]]
:: [s0; CtrlFrame`&_[* TopSeparatorFrame]()]
:: [s0; Yes.]
:: [s0; 
@@image:612&237
(A2IAJgAAANr/AAAAAHic7dBRCQAhEEDB/hGugR9XwwjCXQTBDlpBBGFh5zEJ3ujfAACAa97ypHJ2aSbr7NLfaiouueSSS9G45JJLLkXjkksuuRSNSy655FI0Lt27BAAAmxZrvoWI)
]
:: [s0; ]
:: [s0; CtrlFrame`&_[* BottomSeparatorFrame]()]
:: [s0; Yes.]
:: [s0; 
@@image:612&237
(A2IAJgAAANr/AAAAAHic7dBRCQAhEEDB/hGugR9XwwjCXQTBDlpBBGFh5zEJ3ujfAACAa/5WU3HJJZdcisYll1xyKRqXXHLJpWhccskll6Jx6d6ltzypnF2ayTq7BAAAmxaTOIWI)
]
:: [s0; ]
:: [s0; CtrlFrame`&_[* LeftSeparatorFrame]()]
:: [s0; Yes.]
:: [s0; 
@@image:612&237
(A2IAJgAAANr/AAAAAHic7dBBCcAwFAUw/xLqYIfamITCKmFQD78Wdhk8aCAKst5nAQDAb/rVqmqO+xCWLFmylMaSJUuW0liyZMlSGkuWLFlKY8mSpfwlAAD4aAPkY4SY)
]
:: [s0; ]
:: [s0; CtrlFrame`&_[* RightSeparatorFrame]()]
:: [s0; Yes.]
:: [s0; 
@@image:612&237
(A2IAJgAAANr/AAAAAHic7dBRCYAwGAbA/hFssAdrLMJAIwzW4V8FX4RPPLgEt+a1AADgNffoP3G2o6osWbJkKYclS5YspbFkyZKlNJYsWbKUxpIlS19ZAgCAhzabQ4SY)
]
:: [s0; ]}}&]
[s3; &]
[s0; &]
[s0; &]
[s0;%- &]
[ {{10000t/25b/25@3 [s0;%- [*@(229)4 Simple Ctrl frame templates]]}}&]
[s3;%- &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@3;3 T][3 >]&]
[s1;:FrameCtrl`:`:class:%- [@(0.0.255) class]_[* FrameCtrl]_:_[@(0.0.255) public]_[*@3 T], 
[@(0.0.255) public]_[*@3 CtrlFrame]&]
[s2; This is the base class of simple Ctrl frames `- frames that 
place single Ctrl at some edge of parent Ctrl.&]
[s2; &]
[s2; This class basically overloads FrameAdd and FrameRemove virtual 
methods of [^topic`:`/`/CtrlCore`/src`/Frame`$en`-us`#`:`:CtrlFrame`:`:class CtrlFrame^ C
trlFrame] so that they add/remove `'this`' from parent`'s children`-list.&]
[s2; &]
[s7; [%-*C@4 T]-|Ctrl type.&]
[s0; &]
[s0;%- [%%/ Derived from][%%  ]T, [^topic`:`/`/CtrlCore`/src`/Frame`$en`-us`#`:`:CtrlFrame`:`:class CtrlFrame^@(0.0.255) C
trlFrame]&]
[s0;^topic`:`/`/CtrlCore`/src`/Frame`$en`-us`#`:`:CtrlFrame`:`:class CtrlFrame^@(0.0.255)%- &]
[s0;^topic`:`/`/CtrlCore`/src`/Frame`$en`-us`#`:`:CtrlFrame`:`:class CtrlFrame^@(0.0.255)%- &]
[s5;:FrameCtrl`:`:GetParent`(`):%- [_^Ctrl^ Ctrl]_`*[* GetParent]()&]
[s2; Returns a parent to which the Ctrl is attached.&]
[s3;%- &]
[s4; &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 T][3 >]&]
[s1;:FrameLR`:`:class:%- [@(0.0.255) class]_[* FrameLR]_:_[@(0.0.255) public]_[*@3 FrameCtrl]<
[*@4 T]>_&]
[s2; This class extends CtrlFrame class with width attribute and 
serves as base class to classes placing Ctrl at the left or right 
size of parent Ctrl frame. Width is initialized to Null. Null 
as width indicates that width is equal to FrameButtonWidth().&]
[s2; &]
[s7; [%-*C@4 T]-|Ctrl type.&]
[s0; &]
[s0;%- [%%/ Derived from][%%  ][^topic`:`/`/CtrlCore`/src`/Frame`$en`-us`#`:`:FrameCtrl`:`:template `<class T`> class FrameCtrl^ F
rameCtrl<T>] &]
[s0; &]
[s0; &]
[s0; &]
[s5;:FrameLR`:`:Width`(int`):%- [_^FrameLR^ FrameLR][@(0.0.255) `&]_[* Width]([@(0.0.255) int
]_[*@3 `_cx])&]
[s2; Sets the new width.&]
[s7; [%-*C@3 `_cx]-|Width.&]
[s7; [*/ Return value]-|`*this.&]
[s0;3 &]
[s5;:FrameLR`:`:GetWidth`(`)const:%- [@(0.0.255) int]_[* GetWidth]()_[@(0.0.255) const]&]
[s7; [*/ Return value]-|Current width.&]
[s3; &]
[s4; &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 T][3 >]&]
[s1;:FrameLeft`:`:class:%- [@(0.0.255) class]_[* FrameLeft]_:_[@(0.0.255) public]_[*@3 FrameLR
]<[*@4 T]>_&]
[s2; This class places Ctrl to the parent`'s left side as frame.&]
[s7; [%-*C@4 T]-|Ctrl type.&]
[s0; &]
[s0;%- [%%/ Derived from][%%  ][^topic`:`/`/CtrlCore`/src`/Frame`$en`-us`#`:`:FrameLR`:`:template `<class T`> class FrameLR^ F
rameLR<T>] &]
[s3; &]
[s4; &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 T][3 >]&]
[s1;:FrameRight`:`:class:%- [@(0.0.255) class]_[* FrameRight]_:_[@(0.0.255) public]_[*@3 Frame
LR]<[*@4 T]>_&]
[s2; This class places Ctrl to the parent`'s right side as frame.&]
[s7; [%-*C@4 T]-|Ctrl type.&]
[s0; &]
[s0;%- [%%/ Derived from][%%  ][^topic`:`/`/CtrlCore`/src`/Frame`$en`-us`#`:`:FrameLR`:`:template `<class T`> class FrameLR^ F
rameLR<T>] &]
[s3; &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 T][3 >]&]
[s1;:FrameTB`:`:class:%- [@(0.0.255) class]_[* FrameTB]_:_[@(0.0.255) public]_[*@3 FrameCtrl]<
[*@4 T]>_&]
[s2; This class extends CtrlFrame class with height attribute and 
serves as base class to classes placing Ctrl as the top or bottom 
side of parent Ctrl frame. Height is initialized to Null. Null 
as height indicates that height is equal to the width.&]
[s7; [%-*C@4 T]-|Ctrl type.&]
[s0; &]
[s0;%- [%%/ Derived from][%%  ][^topic`:`/`/CtrlCore`/src`/Frame`$en`-us`#`:`:FrameCtrl`:`:template `<class T`> class FrameCtrl^ F
rameCtrl<T>] &]
[s3; &]
[s0; &]
[s0; &]
[s5;:FrameTB`:`:Height`(int`):%- [_^FrameTB^ FrameTB][@(0.0.255) `&]_[* Height]([@(0.0.255) i
nt]_[*@3 `_cy])&]
[s2; Sets the new height.&]
[s7; [%-*C@3 `_cy]-|Height.&]
[s7; [*/ Return value]-|`*this.&]
[s0; &]
[s5;:FrameTB`:`:GetHeight`(`)const:%- [@(0.0.255) int]_[* GetHeight]()_[@(0.0.255) const]&]
[s7; [*/ Return value]-|Current height.&]
[s3; &]
[s4; &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 T][3 >]&]
[s1;:FrameTop`:`:class:%- [@(0.0.255) class]_[* FrameTop]_:_[@(0.0.255) public]_[*@3 FrameTB]<
[*@4 T]>_&]
[s2; This class places Ctrl to the parent`'s top side as frame.&]
[s7; [%-*C@4 T]-|Ctrl type.&]
[s0; &]
[s0;%- [%%/ Derived from][%%  ][^topic`:`/`/CtrlCore`/src`/Frame`$en`-us`#`:`:FrameTB`:`:template `<class T`> class FrameTB^ F
rameTB<T>] &]
[s3; &]
[s4; &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 T][3 >]&]
[s1;:FrameBottom`:`:class:%- [@(0.0.255) class]_[* FrameBottom]_:_[@(0.0.255) public]_[*@3 Fra
meTB]<[*@4 T]>_&]
[s2; This class places Ctrl to the parent`'s bottom side as frame.&]
[s7; [%-*C@4 T]-|Ctrl type.&]
[s0; &]
[s0;%- [%%/ Derived from][%%  ][^topic`:`/`/CtrlCore`/src`/Frame`$en`-us`#`:`:FrameTB`:`:template `<class T`> class FrameTB^ F
rameTB<T>] &]
[s3; &]
[s0; &]
[s0; &]
[s0;%- &]
[ {{10000t/25b/25@3 [s0;%- [*@(229)4 Frame utility functions]]}}&]
[s9; Following functions are intended as helpers to for implementation 
of FrameLayout method of CtrlFrame, placing some Ctrl to the 
side of parent Ctrl. They adjust given Rect (parameter of FrameLayout) 
and also alter position of given Ctrl.&]
[s3; &]
[s0; &]
[s5;:LayoutFrameLeft`(Rect`&`,Ctrl`*`,int`):%- [@(0.0.255) void]_[* LayoutFrameLeft]([_^Rect^ R
ect][@(0.0.255) `&]_[*@3 r], [_^Ctrl^ Ctrl]_`*[*@3 ctrl], [@(0.0.255) int]_[*@3 cx])&]
[s2; Places ctrl at the left side of parent Ctrl.&]
[s7; [%-*C@3 r]-|Current parent Ctrl rect.&]
[s7; [%-*C@3 ctrl]-|Ctrl to be placed.&]
[s7; [%-*C@3 cx]-|Required width.&]
[s3; &]
[s4;%- &]
[s5;:LayoutFrameRight`(Rect`&`,Ctrl`*`,int`):%- [@(0.0.255) void]_[* LayoutFrameRight]([_^Rect^ R
ect][@(0.0.255) `&]_[*@3 r], [_^Ctrl^ Ctrl]_`*[*@3 ctrl], [@(0.0.255) int]_[*@3 cx])&]
[s2; Places ctrl at the right side of parent Ctrl.&]
[s7; [%-*C@3 r]-|Current parent Ctrl rect.&]
[s7; [%-*C@3 ctrl]-|Ctrl to be placed.&]
[s7; [%-*C@3 cx]-|Required width.&]
[s3; &]
[s4;%- &]
[s5;:LayoutFrameTop`(Rect`&`,Ctrl`*`,int`):%- [@(0.0.255) void]_[* LayoutFrameTop]([_^Rect^ R
ect][@(0.0.255) `&]_[*@3 r], [_^Ctrl^ Ctrl]_`*[*@3 ctrl], [@(0.0.255) int]_[*@3 cy])&]
[s2; Places ctrl at the top side of parent Ctrl.&]
[s7; [%-*C@3 r]-|Current parent Ctrl rect.&]
[s7; [%-*C@3 ctrl]-|Ctrl to be placed.&]
[s7; [%-*C@3 cy]-|Required height.&]
[s3; &]
[s4;%- &]
[s5;:LayoutFrameBottom`(Rect`&`,Ctrl`*`,int`):%- [@(0.0.255) void]_[* LayoutFrameBottom](
[_^Rect^ Rect][@(0.0.255) `&]_[*@3 r], [_^Ctrl^ Ctrl]_`*[*@3 ctrl], [@(0.0.255) int]_[*@3 cy])&]
[s2; Places ctrl at the bottom side of parent Ctrl.&]
[s7; [%-*C@3 r]-|Current parent Ctrl rect.&]
[s7; [%-*C@3 ctrl]-|Ctrl to be placed.&]
[s7; [%-*C@3 cy]-|Required height.&]
[s3; &]
[s0; ]]