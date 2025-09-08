topic "ClipData";
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
[ {{10000@(113.42.0) [s0; [*@7;4 ClipData]]}}&]
[s3; &]
[s1;:ClipData`:`:struct: [@(0.0.255)3 struct][3 _][*3 ClipData][3 _:_][@(0.0.255)3 public][3 _][*@3;3 M
oveable][3 <][*3 ClipData][3 >_]&]
[s0; &]
[s0;%% This structure represents single datatype assigned to clip 
operation (clipboard or drag`&drop). The final data transfered 
via clip has always to be binary string, but as usually application 
provides multiple formats (e.g. it can provide .png, .jpg and 
.gif form Image) and conversion of source to final format can 
be expensive, ClipData allows to store the source data in original 
format accompanied with `'rendering`' function used to convert 
it to final format.&]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Members]]}}&]
[s3; &]
[s5;:ClipData`:`:data: [_^Value^ Value]_[* data]&]
[s2;%% Source data.&]
[s3;%% &]
[s4; &]
[s5;:ClipData`:`:render: [_^String^ String]_(`*[* render])([@(0.0.255) const]_Value[@(0.0.255) `&
]_data)&]
[s2;%% Function to convert source [* data] to binary string.&]
[s3; &]
[s4; &]
[s5;:ClipData`:`:Render`(`)const: [_^String^ String]_[* Render]()_[@(0.0.255) const]&]
[s2;%% Equivalent to (`*render)(data).&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s4; &]
[s5;:ClipData`:`:ClipData`(const Value`&`,String`(`*`)`(const Value`&`)`): [* ClipData](
[@(0.0.255) const]_[_^Value^ Value][@(0.0.255) `&]_[*@3 data], [_^String^ String]_(`*[*@3 ren
der])([@(0.0.255) const]_Value[@(0.0.255) `&]_[*@3 data]))&]
[s2;%% Assigns [%-*@3 data] and [%-*@3 render] to respective member variables.&]
[s3;%% &]
[s4; &]
[s5;:ClipData`:`:ClipData`(const String`&`): [* ClipData]([@(0.0.255) const]_[_^String^ Str
ing][@(0.0.255) `&]_[*@3 data])&]
[s2;%% Assigns [%-*@3 data].to [* data ]and trivial `"string to string`" 
(no conversion) rendering function to [* render.]&]
[s3;%% &]
[s4; &]
[s5;:ClipData`:`:ClipData`(`): [* ClipData]()&]
[s2;%% Assigns trivial `"string to string`" (no conversion) rendering 
function to [* render.]&]
[s3; &]
[s1;@(0.0.255)3 &]
[s0; &]
[ {{10000@(113.42.0) [s0; [*@7;4 ClipData]]}}&]
[s0; &]
[s1;:PasteClip`:`:class: [@(0.0.255)3 class][3 _][*3 PasteClip]&]
[s2;%% This class represents data source and operation status in 
clipboard or drag and drop operations (and also in X11 selection 
operations).&]
[s3; &]
[s0;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Member List]]}}&]
[s0; &]
[s5;:PasteClip`:`:IsAvailable`(const char`*`)const: [@(0.0.255) bool]_[* IsAvailable]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 fmt])_[@(0.0.255) const]&]
[s2;%% Returns true when data source offers data in format [%-*@3 fmt].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:PasteClip`:`:IsAnyAvailable`(const char`*`)const: [@(0.0.255) bool] 
[* IsAnyAvailable]([@(0.0.255) const] [@(0.0.255) char] [@(0.0.255) `*][*@3 fmt]) 
[@(0.0.255) const]&]
[s2;%% Parameter [%-*@3 fmt] contains a list of format identifiers 
separated by semicolon. If any of them is available in the PasteClip, 
function returns true.&]
[s3; &]
[s4; &]
[s5;:PasteClip`:`:Get`(const char`*`)const: [_^String^ String]_[* Get]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 fmt])_[@(0.0.255) const]&]
[s2;%% Returns binary data rendered in [%-*@3 fmt].&]
[s3;%% &]
[s4; &]
[s5;:PasteClip`:`:Accept`(`): [@(0.0.255) bool]_[* Accept]()&]
[s2;%% Sets the status of operation to accepted (target accepts data) 
and returns true when source should [/ paste ]the source data into 
target `- during drag`&drop operation, it returns false during 
`'drag`' phase to alter appearance of mouse cursor (it should 
show `'stop`' sign when drop is not possible) and `'true`' to 
indicate drop operation.&]
[s3; &]
[s4; &]
[s5;:PasteClip`:`:Accept`(const char`*`): [@(0.0.255) bool]_[* Accept]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 fmt])&]
[s2;%% Parameter [%-*@3 fmt] contains a list of format identifiers 
separated by semicolon. If any of them is available in the PasteClip, 
it is set to accepted status and accepted format is stored in 
PasteClip (to be retrieved by GetFormat method later). If PasteClip 
is in paste mode, method stores source data rendered in accepted 
format and returns true. Note that Accept can return false even 
if format is accepted `- this happens in `'drag`' stage of drag`&drop 
operation to indicate that target is willing to accept the data.&]
[s3;%% &]
[s4; &]
[s5;:PasteClip`:`:GetFormat`(`): [_^String^ String]_[* GetFormat]()&]
[s2;%% Returns a format accepted by the last Accept(fmt) method returning 
true.&]
[s3; &]
[s4; &]
[s5;:PasteClip`:`:Get`(`)const: [_^String^ String]_[* Get]()_[@(0.0.255) const]&]
[s2;%% Returns data rendered by the last Accept(fmt) method returning 
true.&]
[s3; &]
[s4; &]
[s5;:PasteClip`:`:operator String`(`)const: [* operator_String]()_[@(0.0.255) const]&]
[s2;%% Same as Get().&]
[s3; &]
[s4; &]
[s5;:PasteClip`:`:operator`~`(`)const: [_^String^ String]_[@(0.0.255) operator]_`~()_[@(0.0.255) c
onst]&]
[s2;%% Same as Get().&]
[s3; &]
[s4; &]
[s5;:PasteClip`:`:Reject`(`): [@(0.0.255) void]_[* Reject]()&]
[s2;%% Sets PasteClip to rejected status. This can be called after 
setting it to accepted status to revert the decision. Usually 
this is used when decision has to take into account the content 
of actual data to be pasted `- in that case data is Accepted 
first, then after examination of data content, it can be rejected.&]
[s3; &]
[s4; &]
[s5;:PasteClip`:`:GetAction`(`)const: [@(0.0.255) int]_[* GetAction]()_[@(0.0.255) const]&]
[s2;%% Gets the proposed action of drag`&drop operation (one of DND`_COPY, 
DND`_MOVE).&]
[s3; &]
[s4; &]
[s5;:PasteClip`:`:GetAllowedActions`(`)const: [@(0.0.255) int]_[* GetAllowedActions]()_[@(0.0.255) c
onst]&]
[s2;%% Returns the bitmask of allowed operations of drag`&drop operation 
(DND`_COPY, DND`_MOVE).&]
[s3; &]
[s4; &]
[s5;:PasteClip`:`:SetAction`(int`): [@(0.0.255) void]_[* SetAction]([@(0.0.255) int]_[*@3 x])
&]
[s2;%% Changes drop operation to [%-*@3 x]. This is used when proposed 
operation is no suitable for particular drop target.&]
[s3;%% &]
[s4; &]
[s5;:PasteClip`:`:IsAccepted`(`)const: [@(0.0.255) bool]_[* IsAccepted]()_[@(0.0.255) const
]&]
[s2;%% Returns true if status is accepted.&]
[s3; &]
[s4; &]
[s5;:PasteClip`:`:IsQuery`(`)const: [@(0.0.255) bool]_[* IsQuery]()_[@(0.0.255) const]&]
[s2;%% Returns true if PasteClip represents drag phase (no pasting, 
Accept returns false).&]
[s3; &]
[s4; &]
[s5;:PasteClip`:`:IsPaste`(`)const: [@(0.0.255) bool]_[* IsPaste]()_[@(0.0.255) const]&]
[s2;%% Returns true if PasteClip represents drop/paste phase (Accept 
returns true).&]
[s3; &]
[s4; &]
[s5;:PasteClip`:`:PasteClip`(`): [* PasteClip]()&]
[s2;%% Default constructor.&]
[s3; &]
[s0;%% ]]