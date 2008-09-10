topic "BZ2 Compression";[ $$0,0#00000000000000000000000000000000:Default]
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[b83;*4 $$3,5#07864147445237544204411237157677:title]
[i288;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;2 $$5,5#45413000475342174754091244180557:text]
[l288;b17;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;tC@5;1 $$7,7#20902679421464641399138805415013:code]
[b2503;2 $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base]
[tC2 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam]
[b167;C2 $$13,13#92430459443460461911108080531343:item1]
[i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128)2 $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7;2 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line]
[l160;*tC+117 $$18,5#88603949442205825958800053222425:package`-title]
[2 $$19,0#53580023442335529039900623488521:gap]
[tC2 $$20,20#70211524482531209251820423858195:class`-nested]
[b50;2 $$21,21#03324558446220344731010354752573:Par]
[b117;*+117 $$22,21#27457433418004101424360058862402:Caption]
[i128;*C@(28.42.150)2 $$23,23#90519220486603166734501227306500:code]
[i448;a25;kKO9;*@(64)2 $$24,4#37138531426314131252341829483370:item]
[i448;a25;kKO9;*@(64) $$25,0#37138531426314131252341829483380:class]
[i448;a25;kO9;*@(64)2 $$26,26#46142079503553116794228416715415:item`_next]
[{_}%EN-US 
[s0;~~~0;%- Upp BZ2 Compression&]
[s0;~~~0;%- plugin`\bz2&]
[s0;~~~0;%- &]
[ {{10000F(128)G(128)@1 [s0; Method Detail]}}&]
[s0; &]
[s24;:`:`:BZ2Compress`(`:`:String`,`:`:Gate2`<int`,int`>`):%- [^`:`:String^ String]_[@0 BZ
2Compress]([^`:`:String^ String]_[@3 s], [^`:`:Gate2^ Gate2]<[^int^ int], 
[^int^ int]>_[@3 progress])&]
[s6;b0;a0; Compress the string [%-*@3 s] using bzip2 library. Run Gate 
function [%-*@3 progress] (usually used to update GUI progress 
bar) as compression continues. [%-*@3 progress] function is called 
each time a block of text is compressed (block size depends on 
compression buffer size). Return the compressed String.&]
[s0; &]
[s24;:`:`:BZ2Compress`(`:`:Stream`&`,`:`:Stream`&`,`:`:Gate2`<int`,int`>`):%- [@(0.0.255) v
oid]_[@0 BZ2Compress]([^`:`:Stream^ Stream]`&_[@3 out], [^`:`:Stream^ Stream]`&_[@3 in], 
[^`:`:Gate2^ Gate2]<[^int^ int], [^int^ int]>_[@3 progress]_`=_false)&]
[s6;b0;a0; Compress the Stream [%-*@3 in] while calling Gate function 
[%-*@3 progress]for each block that is compressed. Each compressed 
block is sent to Stream [%-*@3 out].&]
[s0; &]
[s24;:`:`:BZ2Decompress`(`:`:String`,`:`:Gate2`<int`,int`>`):%- [^`:`:String^ String]_[@0 B
Z2Decompress]([^`:`:String^ String]_[@3 s], [^`:`:Gate2^ Gate2]<[^int^ int], 
[^int^ int]>_[@3 progress]_`=_false)&]
[s6;b0;a0; Decompress the string [%-*@3 s] using bzip2 library. Run 
Gate function [%-*@3 progress] (usually used to update GUI progress 
bar) as decompression continues. [%-*@3 progress] function is called 
each time a block of text is decompressed (block size depends 
on decompression buffer size). Return the decompressed String.&]
[s0; &]
[s24;:`:`:BZ2Decompress`(`:`:Stream`&`,`:`:Stream`&`,`:`:Gate2`<int`,int`>`):%- [@(0.0.255) v
oid]_[@0 BZ2Decompress]([^`:`:Stream^ Stream]`&_[@3 out], [^`:`:Stream^ Stream]`&_[@3 in], 
[^`:`:Gate2^ Gate2]<[^int^ int], [^int^ int]>_[@3 progress]_`=_false)&]
[s6;b0;a0; Decompress the Stream [%-*@3 in] while calling Gate function 
[%-*@3 progress]for each block that is compressed. Each decompressed 
block is sent to Stream [%-*@3 out].&]
[s0; &]
[s24;:`:`:BZ2Decompress`(`:`:Stream`&`,`:`:Gate2`<int`,int`>`):%- [^`:`:String^ String]_
[@0 BZ2Decompress]([^`:`:Stream^ Stream]`&_[@3 stream], [^`:`:Gate2^ Gate2]<[^int^ int], 
[^int^ int]>_[@3 progress]_`=_false)&]
[s6; Decompress the Stream [%-*@3 stream] using bzip2 library. Run 
Gate function [%-*@3 progress] (usually used to update GUI progress 
bar) as decompression continues. [%-*@3 progress] function is called 
each time a block is decompressed (block size depends on decompression 
buffer size). Return the entire decompressed String.&]
[s0; &]
[s0; ]