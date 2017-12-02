topic "WithFactory";
[ $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9; $$1,0#37138531426314131252341829483380:structitem]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[i448;b42;O9;2 $$4,4#61672508125594000341940100500538:tparam]
[b42;2 $$5,5#13035079074754324216151401829390:normal]
[H6;0 $$6,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$7,0#37138531426314131252341829483370:codeitem]
[{_}%EN-US 
[ {{10000@(113.42.0) [s0; [*@7;4 WithFactory]]}}&]
[s0;*@3;4 &]
[s1;:noref:%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>&]
[s1;:WithFactory`:`:class:%- [@(0.0.255) class]_[* WithFactory]&]
[s4; [*C@4 T]-|Type of base class of polymorphic hierarchy.&]
[s5; This template class adds some RTTI (Run Time Type Identification) 
to your classes, and implements a so called `'class factory`', 
bringing the ability to register class hierarchies, construct 
them by name, check their type at runtime, list all classes in 
hierarchy and so on. &]
[s5; It`'s a base behaviour for most polymorphic class usage.&]
[s2; &]
[s3;%- &]
[s0;2 &]
[ {{10000F(128)G(128)@1 [s0; [* Basic usage]]}}&]
[s3;%- &]
[s5; To implement the factory behaviour in your base class, just 
derive from WithFactory :&]
[s0; &]
[s2; [* class MyBaseClass : public WithFactory<MyBaseClass>]&]
[s2; [* `{]&]
[s2; [* -|-|.........]&]
[s2; [* `};]&]
[s5; Then you can derive your class hierarchy from base, as usual 
:&]
[s0; &]
[s2; [* class MyDerivedClass : public MyBaseClass]&]
[s2; [* `{]&]
[s2; [* -|-|.........]&]
[s2; [* `};]&]
[s5; When you`'ve your class definition, you must register them; 
that`'s done inserting in a .cpp file (or in separate ones if 
you like, but NOT inside include files, the following statement 
:&]
[s5; &]
[s2; [* REGISTERCLASS(MyBaseClass `[, `"a class description`" `[, anIndex 
`[, `"an Iml image name`"`]`]`])]&]
[s2; [* REGISTERCLASS(MyDerivedClass `[, `"a class description`" `[, 
anIndex `[, `"an Iml image name`"`]`]`])]&]
[s5; &]
[s5; where you can insert an optional class description string, an 
index and and icon in Iml format; their usage will be clarified 
later on.&]
[s5; Class creation can be done by following ways :&]
[s5; &]
[s2; As a pointer by classic new operator:&]
[s2; -|-|[* MyBaseClass `*ptr `= new MyBaseClass;]&]
[s2; [* -|-|MyBaseClass `*ptr `= new MyDerivedClass;]&]
[s0; &]
[s2; As a pointer, by ascii class name :&]
[s2; -|-|[* MyBaseClass `*ptr `= MyBaseClass`::CreatePtr(`"MyBaseClass`");]&]
[s2; [* -|-|MyBaseClass `*ptr `= MyBaseClass`::CreatePtr(`"MyDerivedClass`");]&]
[s0; &]
[s2; As One<MyBaseClass> :&]
[s2; -|-|[* One<MyBaseClass> ptr `= MyBaseClass`::CreateInstance(`"MyBaseClass`");]&]
[s2; [* -|-|One<MyBaseClass> ptr `= MyBaseClass`::CreateInstance(`"MyDerivedClass`");]&]
[s0; &]
[s5; You can inquire the type of an object at runtime with the [* IsA()] 
member function `::&]
[s5; &]
[s2; [* MyBaseClass `*ptr `= new MyDerivedClass;]&]
[s2; [* String classType `= ptr`->IsA();    ]returns the string `"MyDerivedClass`"&]
[s0; &]
[s5; You can also list at runtime your class hierarchy by mean of 
static member [* Classes()]:&]
[s5; &]
[s2; [* Vector<String> const `&classList `= MyBaseClass`::Classes();]&]
[s0; &]
[s5; or get descriptions by class name with [* GetClassDescription()] 
static member :&]
[s5; &]
[s2; [* String classDesc `= MyBaseClass`::GetClassDescription(`"MyBaseClass`");]&]
[s2; [* String classDesc `= MyBaseClass`::GetClassDescription(`"MyDerivedClass`");]&]
[s0; &]
[s5; or get their integer `'index`' with [* GetClassIndex()] static 
member :&]
[s5; &]
[s2; [* int classIndex `= MyBaseClass`::GetClassIndex(`"MyBaseClass`");]&]
[s2; [* int classIndex `= MyBaseClass`::GetClassIndex(`"MyDerivedClass`");]&]
[s0;2 &]
[ {{10000F(128)G(128)@1 [s0; [* Class registering]]}}&]
[s3;%- &]
[s7;:REGISTERCLASS`(type`, `.`.`.`):%- [* REGISTERCLASS]([*@3 type])&]
[s2; Registers a class by its [%-*@3 type].&]
[s3; &]
[s6; &]
[s7;:REGISTERCLASS`(type`, `.`.`.`):%- [* REGISTERCLASS]([*@3 type], 
[*@3 description])&]
[s2; Registers a class by its [%-*@3 type] giving an optional [%-*@3 description 
]which can be queried later on. Main purpose of [%-*@3 description] 
is the presentation of class lists on menus and or dialogs when 
creating classes at runtime.&]
[s3; &]
[s7;:REGISTERCLASS`(type`, `.`.`.`):%- [* REGISTERCLASS]([*@3 type], 
[*@3 description, index])&]
[s2; Registers a class by its [%-*@3 type] giving an optional [%-*@3 description 
]and an integer [%-*@3 index] which both can be queried later on. 
Main purpose of [%-*@3 index] is to give the ability to sort at 
runtime the class list by importance. [%-*@3 Index] can be any 
integer number &]
[s6; &]
[s7;:REGISTERCLASS`(type`, `.`.`.`):%- [* REGISTERCLASS]([*@3 type], 
[*@3 description, index, icon])&]
[s2; Registers a class by its [%-*@3 type] giving an optional [%-*@3 description], 
an integer [%-*@3 index] and an [%-*@3 icon ]which can be queried 
later on. Main purpose of [%-*@3 index] is to give the ability 
to sort at runtime the class list by importance or to have sort 
of class grouping. [%-*@3 Index] can be any integer number [%-*@3 icon 
]should be a String containing a full Iml icon name, as `"MyIml`::MyImage`" 
&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0; [* Detailed member list]]}}&]
[s3;%- &]
[s6;%- &]
[s7;:WithFactory`:`:Create`(const String`&`):%- [@(0.0.255) static] 
[_^One^ One]<[*@4 T]>_[* CreateInstance]([@(0.0.255) const]_[_^String^ String]_`&[*@3 classNa
me])&]
[s2; Creates a class derived from hierarchy base T by its [%-*@3 className].&]
[s2; Returns a smart pointer to the base class T&]
[s3; &]
[s6;%- &]
[s7;:WithFactory`:`:CreatePtr`(String const`&`):%- [@(0.0.255) static] 
[*@4 T]_`*[* CreatePtr]([_^String^ String]_[@(0.0.255) const]_`&[*@3 className])&]
[s2; Creates a class derived from hierarchy base T by its [%-*@3 className].&]
[s2; Returns a traditional pointer to the base class T&]
[s3; &]
[s6;%- &]
[s7;:WithFactory`:`:Classes`(void`):%- [@(0.0.255) static] [_^Vector^ Vector]<[_^String^ St
ring]>_[@(0.0.255) const]_`&[* Classes]([@(0.0.255) void])&]
[s2; Returns Vector of strings containing all registered class names 
in hierarchy.&]
[s3;%- &]
[s6;%- &]
[s7;:WithFactory`:`:GetClassDescription`(const String`&`):%- [@(0.0.255) static] 
[_^String^ String]_[@(0.0.255) const]_`&[* GetClassDescription]([@(0.0.255) const]_[_^String^ S
tring]_`&[*@3 className])&]
[s2; Returns an ascii description of a class identified by [%-*@3 className].&]
[s2; If no description was given when registering the class, returns 
an empty string.&]
[s3; &]
[s6;%- &]
[s7;:WithFactory`:`:GetClassIndex`(const String`&`):%- [@(0.0.255) static] 
[@(0.0.255) int]_[@(0.0.255) const]_`&[* GetClassIndex]([@(0.0.255) const]_[_^String^ Strin
g]_`&[*@3 className])&]
[s2; Return an integer index assigned to class type identified by 
[%-*@3 className].&]
[s2; If no index was given when registering the class, returns 0.&]
[s3; &]
[s6;%- &]
[s7;:WithFactory`:`:GetClassIndex`(const String`&`):%- [@(0.0.255) static] 
Image_[* GetClassImage]([@(0.0.255) const]_[_^String^ String]_`&[*@3 className])&]
[s2; Return the Image object assigned to class type identified by 
[%-*@3 className].&]
[s2; If no image was given when registering the class, returns Null.&]
[s3; &]
[s6;%- &]
[s7;:WithFactory`:`:IsA`(void`):%- [_^String^ String]_[@(0.0.255) const]_`&[* IsA]([@(0.0.255) v
oid])&]
[s2; Returns a string containing the class name.&]
[s3;%- &]
[s6;%- &]
[s0; ]]