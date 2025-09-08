topic "PasteClip basic formats support functions";
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
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 PasteClip basic formats support functions.]]}}&]
[s3; &]
[s5;:ClipFmtsText`(`): [@(0.0.255) const]_[@(0.0.255) char]_`*[* ClipFmtsText]()&]
[s2;%% Returns a list of format ids associated with plain text (actually, 
returns `"text;wtext`").&]
[s3; &]
[s4; &]
[s5;:AcceptText`(PasteClip`&`): [@(0.0.255) bool]_[* AcceptText]([_^PasteClip^ PasteClip][@(0.0.255) `&
]_[*@3 clip])&]
[s2;%% Accepts [%-*@3 clip] if it is text (returns true for drop).&]
[s3;%% &]
[s4; &]
[s5;:GetString`(PasteClip`&`): [_^String^ String]_[* GetString]([_^PasteClip^ PasteClip][@(0.0.255) `&
]_[*@3 clip])&]
[s5;:GetWString`(PasteClip`&`): [_^WString^ WString]_[* GetWString]([_^PasteClip^ PasteClip
][@(0.0.255) `&]_[*@3 clip])&]
[s2;%% Gets text from [%-*@3 clip].&]
[s3;%% &]
[s4; &]
[s5;:GetTextClip`(const String`&`,const String`&`): [_^String^ String]_[* GetTextClip]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 text], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 fmt])&]
[s5;:GetTextClip`(const WString`&`,const String`&`): [_^String^ String]_[* GetTextClip]([@(0.0.255) c
onst]_[_^WString^ WString][@(0.0.255) `&]_[*@3 text], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 fmt])&]
[s2;%% Created ClipData binary data for [%-*@3 text] based on [%-*@3 fmt].&]
[s3;%% &]
[s4; &]
[s5;:Append`(VectorMap`<String`,ClipData`>`&`,const String`&`): [@(0.0.255) void]_[* Appe
nd]([_^VectorMap^ VectorMap]<[_^String^ String], [_^ClipData^ ClipData]>`&_[*@3 data], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 text])&]
[s5;:Append`(VectorMap`<String`,ClipData`>`&`,const WString`&`): [@(0.0.255) void]_[* App
end]([_^VectorMap^ VectorMap]<[_^String^ String], [_^ClipData^ ClipData]>`&_[*@3 data], 
[@(0.0.255) const]_[_^WString^ WString][@(0.0.255) `&]_[*@3 text])&]
[s2;%% Appends [%-*@3 text] as `"text`" and `"wtext`" formats to [%-*@3 data].&]
[s3;%% &]
[s4; &]
[s5;:ClipFmtsImage`(`): [@(0.0.255) const]_[@(0.0.255) char]_`*[* ClipFmtsImage]()&]
[s2;%% Returns host platform specific semicolon separated list of 
format ids associated with raster images.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:IsAvailableImage`(PasteClip`&`): [@(0.0.255) bool] [* IsAvailableImage](Paste
Clip[@(0.0.255) `&] [*@3 clip])&]
[s2;%% Returns true if [%-*@3 clip] contains an raster image.&]
[s3; &]
[s4; &]
[s5;:AcceptImage`(PasteClip`&`): [@(0.0.255) bool]_[* AcceptImage]([_^PasteClip^ PasteClip][@(0.0.255) `&
]_[*@3 clip])&]
[s2;%% Accepts [%-*@3 clip] if it contains an raster image (returns 
true for drop).&]
[s3;%% &]
[s4; &]
[s5;:GetImage`(PasteClip`&`): [_^Image^ Image]_[* GetImage]([_^PasteClip^ PasteClip][@(0.0.255) `&
]_[*@3 clip])&]
[s2;%% Extracts Image from [%-*@3 clip].&]
[s3;%% &]
[s4; &]
[s5;:GetImageClip`(const Image`&`,const String`&`): [_^String^ String]_[* GetImageClip]([@(0.0.255) c
onst]_[_^Image^ Image][@(0.0.255) `&]_[*@3 m], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 fmt])&]
[s2;%% Constructs binary clip data from [%-*@3 m] of format [%-*@3 fmt].&]
[s3;%% &]
[s4; &]
[s5;:Append`(VectorMap`<String`,ClipData`>`&`,const Image`&`): [@(0.0.255) void]_[* Appen
d]([_^VectorMap^ VectorMap]<[_^String^ String], [_^ClipData^ ClipData]>`&_[*@3 data], 
[@(0.0.255) const]_[_^Image^ Image][@(0.0.255) `&]_[*@3 img])&]
[s2;%% Appends platform specific raster image formats of [%-*@3 img] 
to [%-*@3 data].&]
[s3;%% &]
[s4; &]
[s5;:IsAvailableFiles`(PasteClip`&`): [@(0.0.255) bool]_[* IsAvailableFiles]([_^PasteClip^ P
asteClip][@(0.0.255) `&]_[*@3 clip])&]
[s2;%% Returns true if [%-*@3 clip].contains a list of OS shell files 
(drag operation from OS shell filelist to U`+`+ application).&]
[s3;%% &]
[s4; &]
[s5;:AcceptFiles`(PasteClip`&`): [@(0.0.255) bool]_[* AcceptFiles]([_^PasteClip^ PasteClip][@(0.0.255) `&
]_[*@3 clip])&]
[s2;%% Accepts [%-*@3 clip].if it contains a list of OS shell files.&]
[s3;%% &]
[s4; &]
[s5;:GetClipFiles`(const String`&`): [_^Vector^ Vector]<[_^String^ String]>_[* GetClipFiles
]([@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 data])&]
[s2;%% Extracts a list of OS shell files from raw Clipboard [%-*@3 data] 
(result of ClipData`::Get call).&]
[s3;%% &]
[s4; &]
[s5;:GetFiles`(PasteClip`&`): [_^Vector^ Vector]<[_^String^ String]>_[* GetFiles]([_^PasteClip^ P
asteClip][@(0.0.255) `&]_[*@3 clip])&]
[s2;%% Extracts a list of OS shell files from [%-*@3 clip].&]
[s3;%% &]
[s4; &]
[s5;:ClipFmt`(`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[_^String^ String]_[* Clip
Fmt]()&]
[s2;%% Constructs a special clip format id based on type. Useful 
when transferring data between U`+`+ applications&]
[s3; &]
[s4; &]
[s5;:Accept`(PasteClip`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) bo
ol]_[* Accept]([_^PasteClip^ PasteClip][@(0.0.255) `&]_[*@3 clip])&]
[s2;%% Accepts [%-*@3 clip ]if it contains a format id based on ClipFmt.&]
[s3;%% &]
[s4; &]
[s5;:InternalClip`(const T`&`,const char`*`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T
]>_[_^VectorMap^ VectorMap]<[_^String^ String], [_^ClipData^ ClipData]>_[* InternalClip](
[@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 i
d]_`=_`"`")&]
[s2;%% Creates `'internal`' clip for variable [%-*@3 x]. Internal clips 
only work within single process as they are using simple pointers 
to objects. [%-*@3 x] must exists as long as clip is available.&]
[s3;%% &]
[s4; &]
[s5;:IsAvailableInternal`(PasteClip`&`,const char`*`): [@(0.0.255) template]_<[@(0.0.255) c
lass]_[*@4 T]>_[@(0.0.255) bool]_[* IsAvailableInternal]([_^PasteClip^ PasteClip][@(0.0.255) `&
]_[*@3 d], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id]_`=_`"`")&]
[s2;%% Returns true if there is internal clip  of required type and 
[%-*@3 id ]in [%-*@3 d].&]
[s3;%% &]
[s4; &]
[s5;:AcceptInternal`(PasteClip`&`,const char`*`): [@(0.0.255) template]_<[@(0.0.255) clas
s]_[*@4 T]>_[@(0.0.255) bool]_[* AcceptInternal]([_^PasteClip^ PasteClip][@(0.0.255) `&]_[*@3 d
], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id]_`=_`"`")&]
[s2;%% Accepts internal clip of required type and [%-*@3 id].&]
[s3;%% &]
[s4; &]
[s5;:GetInternal`(PasteClip`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) c
onst]_[*@4 T][@(0.0.255) `&]_[* GetInternal]([_^PasteClip^ PasteClip][@(0.0.255) `&]_[*@3 d])
&]
[s2;%% Returns a reference to internal clip.&]
[s4; &]
[s5;:Upp`:`:GetInternalPtr`(Upp`:`:PasteClip`&`,const char`*`): [@(0.0.255) template]_<
[@(0.0.255) class]_[*@4 T]>_[@(0.0.255) const]_[*@4 T]_`*[* GetInternalPtr]([_^Upp`:`:PasteClip^ P
asteClip][@(0.0.255) `&]_[*@3 d], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 id]_`=_`"`")&]
[s2;%% If internal clip of required type and [%-*@3 id ]is available, 
returns a pointer to it, NULL otherwise.&]
[s3;%% ]]