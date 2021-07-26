topic "Charset and encoding issues";
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[b83;*2 $$3,5#07864147445237544204411237157677:title]
[i288;b167;a42;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;2 $$5,5#45413000475342174754091244180557:text]
[l288;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;t246;C@5;1 $$7,7#20902679421464641399138805415013:code]
[b2503;2 $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base]
[t4167;C2 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam]
[b167;C2 $$13,13#92430459443460461911108080531343:item1]
[i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128)2 $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7;2 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line]
[l160;t4167;*C+117 $$18,5#88603949442205825958800053222425:package`-title]
[2 $$19,0#53580023442335529039900623488521:gap]
[t4167;C2 $$20,20#70211524482531209251820423858195:class`-nested]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s2; Charset and encoding issues&]
[s3; [3 UTF`-8EE]&]
[s5; U`+`+ is not using pure UTF`-8, but our extension that we have 
named `"UTF`-8EE`" as for `"Error Escaped`".&]
[s5; The idea is that not every sequence of bytes is valid UTF`-8. 
Now the issues is how to react to this problem when loading file 
into TheIDE (or other editor). Of course, error message is one 
solution, but we sometimes have to process text files that have 
several sections with different encodings, UTF`-8 being one of 
them. It is of course cool to have editor capable of dealing 
with this.&]
[s5; So let us introduce UTF`-8EE. The idea is this `- when invalid 
input sequence is encountered, it is `"escaped`" into the unicode 
private area (0xe000 `- 0xf8ff) `- all bytes are escaped as 0xEE[@(0.0.255) xx] 
unicode characters (another source of that `"EE`" in the name). 
For this purpose, valid utf8 sequences that would yield 0xee00`-0xeeff 
values are also considered invalid in utf8ee and escaped (but 
as it is private area, no real characters can be there). Now 
when unicode text is converted back to utf`-8, those escaped 
bytes are simply interpreted with their original value. This 
basically means that any text can be converted from UTF`-8EE 
to 16bit unicode (UCS2) and back and the result is equal to the 
original text.&]
[s0; ]]