topic "DataPusher";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[ {{10000@3 [s0; [*@(229)4 DataPusher]]}}&]
[s3; &]
[s1;:DataPusher`:`:class:%- [@(0.0.255)3 class][3 _][*3 DataPusher][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 Pusher]&]
[s0; &]
[s2; 
@@image:1906&143
(AzEBFwAAAOn/AAAAAHic7ZzRrcIwDEU7MXu8ZZiHFZ7EDuUPRbF947aUmvYc3Q+SJrYbcmkkEM//xxMhhBBCsW5/d4TQgbKWnAHgILAkQCmwJEApsCRAKT5ryWma1o18N6cGPdcOiPpXlBdFzpS3PenqOuEEDC250/ZwLZnpFP1u5OQljSgvOXdRrkWWh5NR2ZJRASe2ZJsIV16TjCXtqbJrtj3RyC6m24ws2Z4b53jH2uNllNTehVu2Ls+WEcXsemwifWvuYrpP0mExUJ/8U9K+3VGPuzHcmCKFuDq0pJtoGCcz0fpdLIv1jl63NouoJMqeKQbqs7clLcNdFI3fYslM0uGnR35AxlOttd3HYvs6GtyZNH9fUJZrWlKEFdUuGpBcNzeyXkxR0rAYqM+XD64rmtstGWWJ4g8nRtHcgPl1m71lzEzPFwP1yXwvaQ9RXdN1TdTvRm57urlzvG+7wbZfZ7FX9cRoV7v7XzhdJ3JvfxgwWQzUZ8tPBXijd4KFvTJYEqAU/MYVoBRYEqAUriURQgfq+/8lghBCCP2cXgKYpno=)
&]
[s0; &]
[s9; DataPusher is a class that is able to Convert and Display some 
Value and also allows user to click this value (usually in order 
to change it). Note that both Convert and Display lifetime must 
exceed the lifetime of DataPusher (DataPusher stores just the 
reference to them).&]
[s2; &]
[s9; Usual usage pattern is that DataPusher displays some data that 
can be changed by some complex dialog. This pattern can be implemented 
either by deriving new class from DataPusher, overriding DoAction 
method and using SetDataAction at the end of it to both change 
the value of DataPusher and invoke repainting and WhenAction, 
or it can use default DoAction implementation and WhenAction 
callback (do not call SetDataAction in this case).&]
[s2; &]
[s0;%- [%%/ Derived from][%%  ][^topic`:`/`/CtrlLib`/src`/Pusher`$en`-us^ Pusher]&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3;%- &]
[s5;:DataPusher`:`:DataPusher`(`):%- [* DataPusher]()&]
[s2; Initializes the data pusher.&]
[s3; &]
[s4; &]
[s5;:DataPusher`:`:DataPusher`(const Convert`&`,const Display`&`):%- [* DataPusher]([@(0.0.255) c
onst]_[_^Convert^ Convert][@(0.0.255) `&]_[*@3 convert], [@(0.0.255) const]_[_^Display^ Dis
play][@(0.0.255) `&]_[*@3 display]_`=_StdDisplay())&]
[s2; Creates DataPusher with specified Convert and Display used to 
Convert a then Display the Value of DataPusher. Note that both 
convert and display lifetime must exceed the lifetime of DataPusher 
(DataPusher stores just the reference to them).&]
[s7; [%-*C@3 convert]-|Convert.&]
[s7; [%-*C@3 display]-|Display.&]
[s3; &]
[s4;%- &]
[s5;:DataPusher`:`:DataPusher`(const Display`&`):%- [* DataPusher]([@(0.0.255) const]_[_^Display^ D
isplay][@(0.0.255) `&]_[*@3 display])&]
[s2; Creates DataPusher with specified Display.&]
[s7; [%-*C@3 display]-|Display.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Protected Method List]]}}&]
[s3;%- &]
[s5;:DataPusher`:`:DoAction`(`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* DoAction]()&]
[s2; This virtual method is invoked when user clicks DataPusher. 
Default action is to call WhenAction callback.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Member List]]}}&]
[s3;%- &]
[s5;:DataPusher`:`:SetConvert`(const Convert`&`):%- [_^DataPusher^ DataPusher][@(0.0.255) `&
]_[* SetConvert]([@(0.0.255) const]_[_^Convert^ Convert][@(0.0.255) `&]_[*@3 `_convert])&]
[s2; Sets the new Convert.&]
[s7; [%-*C@3 `_convert]-|New Convert.&]
[s7; [*/ Return value]-|`*this for chaining.&]
[s3; &]
[s4;%- &]
[s5;:DataPusher`:`:GetConvert`(`)const:%- [@(0.0.255) const]_[_^Convert^ Convert][@(0.0.255) `&
]_[* GetConvert]()_[@(0.0.255) const]&]
[s2; Returns current Convert.&]
[s7; [*/ Return value]-|Current Convert.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:DataPusher`:`:ConvertBy`(Function`):%- DataPusher[@(0.0.255) `&] 
[* ConvertBy](Function<Value ([@(0.0.255) const] Value[@(0.0.255) `&])> 
[*@3 cv])&]
[s2; Sets a conversion function. This is applied before [%-*@3 `_convert].&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:DataPusher`:`:Format`(const Value`&`):%- Value [* Format]([@(0.0.255) const] 
Value[@(0.0.255) `&] [*@3 v])&]
[s2; Returns [%-*@3 v] converted by ConvertBy and SetConvert exactly 
the same as in DataPusher`::Paint..&]
[s3;%- &]
[s4;%- &]
[s5;:DataPusher`:`:SetDisplay`(const Display`&`):%- [_^DataPusher^ DataPusher][@(0.0.255) `&
]_[* SetDisplay]([@(0.0.255) const]_[_^Display^ Display][@(0.0.255) `&]_[*@3 `_display])&]
[s2; Sets the new Display.&]
[s7; [%-*C@3 `_display]-|New Display.&]
[s7; [*/ Return value]-|`*this for chaining.&]
[s3; &]
[s4;%- &]
[s5;:DataPusher`:`:GetDisplay`(`)const:%- [@(0.0.255) const]_[_^Display^ Display][@(0.0.255) `&
]_[* GetDisplay]()_[@(0.0.255) const]&]
[s2; Returns current Display.&]
[s7; [*/ Return value]-|Current Display.&]
[s3; &]
[s4;%- &]
[s5;:DataPusher`:`:SetDataAction`(const Value`&`):%- [@(0.0.255) void]_[* SetDataAction](
[@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 value])&]
[s2; Invokes SetData([%-*C@3 value]) and then UpdateActionRefresh(). 
Useful when building DataPusher derived class.&]
[s3; &]
[s4;%- &]
[s5;:DataPusher`:`:Set`(const Value`&`):%- [@(0.0.255) void]_[* Set]([@(0.0.255) const]_[_^Value^ V
alue][@(0.0.255) `&]_[*@3 value])&]
[s2; Alternative way to set data, decoupled from SetData interface. 
Sets the data to be displayed. Intended to be used by derived 
classes that implement own SetData/GetData.&]
[s3; &]
[s4;%- &]
[s5;:DataPusher`:`:SetData`(const Value`&`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* SetData]([@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 value])
&]
[s2; Sets the control`'s data to [%-*@3 value].&]
[s3; &]
[s4;%- &]
[s5;:DataPusher`:`:GetData`(`)const:%- [@(0.0.255) virtual] [_^Value^ Value]_[* GetData]()_
[@(0.0.255) const]&]
[s2; Returns the control`'s data.&]
[s3;%- &]
[s4;%- &]
[s5;:DataPusher`:`:NullText`(const char`*`,Color`):%- [_^DataPusher^ DataPusher][@(0.0.255) `&
]_[* NullText]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text]_`=_t`_(`"([@(0.0.255) defa
ult])`"), [_^Color^ Color]_[*@3 ink]_`=_Brown)&]
[s5;:DataPusher`:`:NullText`(const char`*`,Font`,Color`):%- [_^DataPusher^ DataPusher][@(0.0.255) `&
]_[* NullText]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 text], [_^Font^ Font]_[*@3 fnt], 
[_^Color^ Color]_[*@3 ink])&]
[s2; Sets the default text that will be displayed if no data is supplied.&]
[s7;%- [*@3 text]-|default text&]
[s7;%- [*@3 ink]-|color of the text&]
[s7;%- [*@3 fnt]-|font of the text&]
[s3; &]
[s4;%- &]
[s5;:DataPusher`:`:WhenPreAction:%- [_^Callback^ Callback]_[* WhenPreAction]&]
[s2; This callback is invoked before DoAction.&]
[s3; &]
[s0; ]]