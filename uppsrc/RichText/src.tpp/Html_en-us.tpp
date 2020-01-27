topic "RichText to HTML output";
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
[ {{10000@(113.42.0) [s0; [*@7;4 HtmlObjectSaver]]}}&]
[s3;%- &]
[s1;:Upp`:`:HtmlObjectSaver`:`:class:%- [@(0.0.255)3 class][3 _][*3 HtmlObjectSaver]&]
[s2; This is a helper abstract class used to save RichObject instances 
when exporting RichText to HTML.&]
[s0;i448;a25;kKO9;:noref:@(0.0.255)%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:Upp`:`:HtmlObjectSaver`:`:GetHtml`(const Upp`:`:RichObject`&`):%- [@(0.0.255) virt
ual] [_^Upp`:`:String^ String]_[* GetHtml]([@(0.0.255) const]_[_^Upp`:`:RichObject^ RichO
bject][@(0.0.255) `&]_[*@3 object])&]
[s2; Exports [%-*@3 object]. Default implementation is empty.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:HtmlObjectSaver`:`:GetHtml`(const Upp`:`:RichObject`&`,const Upp`:`:String`&`):%- [@(0.0.255) v
irtual] [_^Upp`:`:String^ String]_[* GetHtml]([@(0.0.255) const]_[_^Upp`:`:RichObject^ Ri
chObject][@(0.0.255) `&]_[*@3 object], [@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&
]_[*@3 link])&]
[s2; Exports [%-*@3 object] which currently has anchor [%-*@3 link]. 
Default implementation calls single parameter GetHtml.&]
[s3; &]
[s0;%- &]
[ {{10000@(113.42.0) [s0; [*@7;4 Export functions]]}}&]
[s3;%- &]
[s5;:Upp`:`:EncodeHtml`(const Upp`:`:RichText`&`,Upp`:`:Index`<Upp`:`:String`>`&`,const Upp`:`:VectorMap`<Upp`:`:String`,Upp`:`:String`>`&`,const Upp`:`:VectorMap`<Upp`:`:String`,Upp`:`:String`>`&`,const Upp`:`:String`&`,const Upp`:`:String`&`,Upp`:`:Zoom`,const Upp`:`:VectorMap`<Upp`:`:String`,Upp`:`:String`>`&`,int`):%- [_^Upp`:`:String^ S
tring]_[* EncodeHtml]([@(0.0.255) const]_[_^Upp`:`:RichText^ RichText][@(0.0.255) `&]_[*@3 t
ext], [_^Upp`:`:Index^ Index]<[_^Upp`:`:String^ String]>`&_[*@3 css], 
[@(0.0.255) const]_[_^Upp`:`:VectorMap^ VectorMap]<[_^Upp`:`:String^ String], 
[_^Upp`:`:String^ String]>`&_[*@3 links], [@(0.0.255) const]_[_^Upp`:`:VectorMap^ VectorM
ap]<[_^Upp`:`:String^ String], [_^Upp`:`:String^ String]>`&_[*@3 labels], 
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 path], 
[@(0.0.255) const]_[_^Upp`:`:String^ String][@(0.0.255) `&]_[*@3 base]_`=_Null, 
[_^Upp`:`:Zoom^ Zoom]_[*@3 z]_`=_[_^Upp`:`:Zoom^ Zoom]([@3 8], [@3 40]), 
[@(0.0.255) const]_[_^Upp`:`:VectorMap^ VectorMap]<[_^Upp`:`:String^ String], 
[_^Upp`:`:String^ String]>`&_[*@3 escape]_`=_[_^Upp`:`:VectorMap^ VectorMap]<[_^Upp`:`:String^ S
tring], [_^Upp`:`:String^ String]>(), [@(0.0.255) int]_[*@3 imtolerance]_`=_[@3 0])&]
[s5;:Upp`:`:EncodeHtml`(const Upp`:`:RichText`&`,Upp`:`:Index`<Upp`:`:String`>`&`,const Upp`:`:VectorMap`<Upp`:`:String`,Upp`:`:String`>`&`,const Upp`:`:VectorMap`<Upp`:`:String`,Upp`:`:String`>`&`,Upp`:`:HtmlObjectSaver`&`,Upp`:`:Zoom`,const Upp`:`:VectorMap`<Upp`:`:String`,Upp`:`:String`>`&`):%- [_^Upp`:`:String^ S
tring]_[* EncodeHtml]([@(0.0.255) const]_[_^Upp`:`:RichText^ RichText][@(0.0.255) `&]_[*@3 t
ext], [_^Upp`:`:Index^ Index]<[_^Upp`:`:String^ String]>`&_[*@3 css], 
[@(0.0.255) const]_[_^Upp`:`:VectorMap^ VectorMap]<[_^Upp`:`:String^ String], 
[_^Upp`:`:String^ String]>`&_[*@3 links], [@(0.0.255) const]_[_^Upp`:`:VectorMap^ VectorM
ap]<[_^Upp`:`:String^ String], [_^Upp`:`:String^ String]>`&_[*@3 labels], 
[_^Upp`:`:HtmlObjectSaver^ HtmlObjectSaver][@(0.0.255) `&]_[*@3 object`_saver], 
[_^Upp`:`:Zoom^ Zoom]_[*@3 z]_`=_[_^Upp`:`:Zoom^ Zoom]([@3 8], [@3 40]), 
[@(0.0.255) const]_[_^Upp`:`:VectorMap^ VectorMap]<[_^Upp`:`:String^ String], 
[_^Upp`:`:String^ String]>`&_[*@3 escape]_`=_[_^Upp`:`:VectorMap^ VectorMap]<[_^Upp`:`:String^ S
tring], [_^Upp`:`:String^ String]>())&]
[s2; Exports RichText [%-*@3 text] in html format. &]
[s2; [%-*@3 css] is set of CSS definitions. The CSS class name is index 
of definition formatted with FormatIntAlpha.&]
[s2; [%-*@3 links] is optional set of link translations. Whole link 
translation is attempted at first, then only part before `'#`' 
is attempted.&]
[s2; [%-*@3 labels] is optional set of label translations.&]
[s2; [%-*@3 object`_saver] instance of HtmlObjectSaver used to store 
RichObject instances.&]
[s2; [%-*@3 z] Zoom factor.&]
[s2; [%-*@3 escape] optional map that translates RichText index entries 
to raw html code to be embedded into the body. &]
[s2; [%-*@3 path] overload with this parameter is using default HtmlObjectSaver 
to export RichObject instances as .png into [%-*@3 path] directory, 
in both zoomed size and original size as long as there is no 
link associated with the image.&]
[s2; [%-*@3 base] this is prefix added to .png name potentially used 
to make it unique if there is a chance of conflict in case the 
output dir is common for more .html pages.&]
[s2; [%-*@3 imtolerance] maximum tolerance of image size error before 
the image is rescaled in percents.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:AsCss`(Upp`:`:Index`<Upp`:`:String`>`&`):%- [_^Upp`:`:String^ String]_[* AsCs
s]([_^Upp`:`:Index^ Index]<[_^Upp`:`:String^ String]>`&_[*@3 ss])&]
[s2; Converts set of CSS class definitions produced by EncodeHtml 
to css block of HTML. The purpose of having this process separated 
from EncodeHtml body production is to have ability to combine 
multiple bodies (multiple EncodeHtml calls) into single HTML 
page.&]
[s2; &]
[s3; &]
[s0; ]]