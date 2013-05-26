topic "SkylarkApp";
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
[ {{10000@(113.42.0) [s0; [*@7;4 Title]]}}&]
[s1;@(0.0.255)3%- &]
[s1;:SkylarkApp`:`:class:%- [@(0.0.255)3 class][3 _][*3 SkylarkApp][3 _:_][@(0.0.255)3 protected
][3 _][*@3;3 SkylarkConfig]&]
[s2; SkylarkApp class represents the web application and allows adjusting 
of various aspects of application by overriding virtual methods 
and changing Skylark configuration, which should be only done 
in SkylarkApp constructor.&]
[s0;i448;a25;kKO9;:noref:@(0.0.255)%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3; &]
[s5;:SkylarkApp`:`:SigUsr1`(`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* SigUsr1]()&]
[s2; Invoked when application recieves SIGUSR1 signal. Default implementation 
is empty.&]
[s3; &]
[s4; &]
[s5;:SkylarkApp`:`:NotFound`(Http`&`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* NotFound
]([_^Http^ Http][@(0.0.255) `&]_[*@3 http])&]
[s2; Overriding this method can change the response in case that 
there no handler was found to handle the request. Default implementation 
is empty.&]
[s3; &]
[s4;%- &]
[s5;:SkylarkApp`:`:SqlError`(Http`&`,const SqlExc`&`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* SqlError]([_^Http^ Http][@(0.0.255) `&]_[*@3 http], 
[@(0.0.255) const]_[_^SqlExc^ SqlExc][@(0.0.255) `&]_[*@3 e])&]
[s2; Overriding this method can change the response in case that 
there was SQL error in handler. Default implementation is empty.&]
[s3; &]
[s4;%- &]
[s5;:SkylarkApp`:`:InternalError`(Http`&`,const Exc`&`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* InternalError]([_^Http^ Http][@(0.0.255) `&]_[*@3 http], 
[@(0.0.255) const]_[_^Exc^ Exc][@(0.0.255) `&]_[*@3 e])&]
[s2; Overriding this method can change the response in case that 
there was internal error in handler. Default implementation is 
empty.&]
[s3; &]
[s4;%- &]
[s5;:SkylarkApp`:`:Unauthorized`(Http`&`,const AuthExc`&`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* Unauthorized]([_^Http^ Http][@(0.0.255) `&]_[*@3 http], 
[@(0.0.255) const]_[_^AuthExc^ AuthExc][@(0.0.255) `&]_[*@3 e])&]
[s2; Overriding this method can change the response in case that 
unauthorized request was detected. Default implementation is 
empty.&]
[s3; &]
[s4;%- &]
[s5;:SkylarkApp`:`:BadRequest`(Http`&`,const BadRequestExc`&`):%- [@(0.0.255) virtual] 
[@(0.0.255) void]_[* BadRequest]([_^Http^ Http][@(0.0.255) `&]_[*@3 http], 
[@(0.0.255) const]_[_^BadRequestExc^ BadRequestExc][@(0.0.255) `&]_[*@3 e])&]
[s2; Overriding this method can change the response in case that 
invalid request was detected. Default implementation is empty.&]
[s3; &]
[s4;%- &]
[s5;:SkylarkApp`:`:TemplateError`(Http`&`,const TemplateErrorExc`&`):%- [@(0.0.255) vir
tual] [@(0.0.255) void]_[* TemplateError]([_^Http^ Http][@(0.0.255) `&]_[*@3 http], 
[@(0.0.255) const]_TemplateErrorExc[@(0.0.255) `&]_[*@3 e])&]
[s2; Overriding this method can change the response in case that 
there is a bug in witz template. Default implementation calls 
InternalError with empty Exc.&]
[s3; &]
[s4; &]
[s5;:SkylarkApp`:`:WorkThread`(`):%- [@(0.0.255) virtual] [@(0.0.255) void]_[* WorkThread](
)&]
[s2; This method represents the main work thread body. It should 
be override to add any application specific initialization of 
thread, most importantly creating the per`-thread database session 
(as stack object) and establishing connection to database. When 
all initialization work is done, it should call RunThread method 
to start actual handling of requests.&]
[s3; &]
[s4; &]
[s5;:SkylarkApp`:`:RunThread`(`):%- [@(0.0.255) void]_[* RunThread]()&]
[s2; This method should be called from WorkThread as it represents 
the loop used to process Http requests.&]
[s3; &]
[s4; &]
[s5;:SkylarkApp`:`:Run`(`):%- [@(0.0.255) void]_[* Run]()&]
[s2; This method should be called from APP`_MAIN to start Skylark 
application. There can be only one SkylarkApp in single binary.&]
[s3; &]
[s4;%- &]
[s5;:SkylarkApp`:`:Quit`(`):%- [@(0.0.255) void]_[* Quit]()&]
[s2; Quits the Skylark application (can be called from another thread).&]
[s3;%- &]
[s4; &]
[s5;:SkylarkApp`:`:TheApp`(`):%- [@(0.0.255) static] [_^SkylarkApp^ SkylarkApp][@(0.0.255) `&
]_[* TheApp]()&]
[s2; Returns a reference to the Skylark application running.&]
[s3; &]
[s4; &]
[s5;:SkylarkApp`:`:Config`(`):%- [@(0.0.255) static] [@(0.0.255) const]_[_^SkylarkConfig^ S
kylarkConfig][@(0.0.255) `&]_[* Config]()&]
[s2; Returns a reference of Skylark application configuration. Note 
that configuration parameters should be changed in the code only 
in application constructor.&]
[s0; ]]