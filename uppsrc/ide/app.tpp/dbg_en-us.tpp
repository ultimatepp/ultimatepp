topic "Debugger pretty printing scripts";
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[H4;b83;*4 $$3,5#07864147445237544204411237157677:title]
[i288;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;ph2 $$5,5#45413000475342174754091244180557:text]
[l288;b17;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;C@5+75 $$7,7#20902679421464641399138805415013:code]
[b2503;2 $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base]
[C2 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam]
[b167;C2 $$13,13#92430459443460461911108080531343:item1]
[i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128)2 $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7;2 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line]
[l160;*C+117 $$18,5#88603949442205825958800053222425:package`-title]
[2 $$19,0#53580023442335529039900623488521:gap]
[C2 $$20,20#70211524482531209251820423858195:class`-nested]
[b50;2 $$21,21#03324558446220344731010354752573:Par]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s2; Debugger pretty printing scripts&]
[s0; This feature is currently only available in Windows PDB debugger.&]
[s0; &]
[s0; TheIDE PDB debugger can use scripts, stored in .dbg files in 
packages, to display user defined types nicely in the debugger. 
Scripts from .dbg files are loaded at the start of debugging.&]
[s0; &]
[s0; Scripts are based on [^topic`:`/`/Esc`/srcdoc`/Esc`_en`-us^ Esc] 
scripting language.&]
[s0; &]
[s0; Scripts can contain two kinds top`-level elements&]
[s0; &]
[s0; [*@(0.0.255) fn ][/ function(params) ]`{ .. `}&]
[s0; &]
[s0; defines Esc function.&]
[s0; &]
[s0; [*@(0.0.255) typename ][/ type ]`{ .. `}&]
[s0; &]
[s0; defines script for pretty printing of [/ type]. Script receives 
following implicit input parameters&]
[s0; &]
[ {{3447:6553^ [s0; [*@(0.0.255) value]]
:: [s0; Map of `"address`" and `"type`" numbers, representing the value 
to print.]
:: [s0; [*@(0.0.255) template`_param]]
:: [s0; If the type is template, array of template parameters (types). 
These usually go right away to ITEM`_TYPE.]
:: [s0; [*@(0.0.255) from]]
:: [s0; If there are multiple values (ITEM`_COUNT was used), first value 
to return (e.g. with ITEM`_PTR)]
:: [s0; [*@(0.0.255) items]]
:: [s0; If there are multiple values (ITEM`_COUNT was used), number 
of values to return.]}}&]
[s0; &]
[s0; Data and type manipulation functions&]
[s0; &]
[ {{3462:6538^ [s0; [*@(0.0.255) SizeOf](x)]
:: [s0; Returns the size of type. Parameter can be string, type number 
or map with `"type`" number attribute]
:: [s0; [*@(0.0.255) TypeName](x)]
:: [s0; Returns the name of type. Parameter can be type number or map 
with `"type`" number attribute.]
:: [s0; [*@(0.0.255) TypeNo](s)]
:: [s0; Convertes type string to type number.]
:: [s0; [*@(0.0.255) PeekPtr](x)]
:: [s0; Reads a pointer value from debugee. Parameter is either number 
or map with `"address`".]
:: [s0; [*@(0.0.255) Peek8](x)]
:: [s0; Reads a byte from debugee. Parameter is either number or map 
with `"address`".]
:: [s0; [*@(0.0.255) Peek16](x)]
:: [s0; Reads 16`-bit integer from debugee. Parameter is either number 
or map with `"address`".]
:: [s0; [*@(0.0.255) Peek32](x)]
:: [s0; Reads 32`-bit integer from debugee. Parameter is either number 
or map with `"address`".]
:: [s0; [*@(0.0.255) Peek64](x)]
:: [s0; Reads 64`-bit integer from debugee. Parameter is either number 
or map with `"address`".]
:: [s0; [*@(0.0.255) Peek32f](x)]
:: [s0; Reads 32`-bit FP number (float) from debugee. Parameter is either 
number or map with `"address`".]
:: [s0; [*@(0.0.255) Peek64f](x)]
:: [s0; Reads 64`-bit FP number (double) from debugee. Parameter is 
either number or map with `"address`".]
:: [s0; [*@(0.0.255) SizeOfPtr]()]
:: [s0; Returns the size of pointer (in bytes) in debugee. 4 for 32`-bit 
mode, 8 for 64`-bit.]
:: [s0; [*@(0.0.255) NestedType](t, id)]
:: [s0; Returns type number of nested type id. t can be type number 
or map with `"type`".]
:: [s0; [*@(0.0.255) DeRef](x)]
:: [s0; Parameter must be map with `"address`". Returns the same with 
`"address`" replaced with pointer value read from debugee. Similar 
to x.address `= PeekPtr(x.address).]
:: [s0; [*@(0.0.255) Field](x, id)]
:: [s0; Parameter must be map with `"address`" and `"type`". id should 
be the name of member variable of `"type`". Returns new map with 
`"address`" and `"type`" corresponding to the given member variable.]}}&]
[s0; &]
[s0; &]
[s0; Output functions&]
[s0; &]
[ {{3472:6528^ [s0; [*@(0.0.255) TEXT](x, color `= 1)]
:: [s0; Adds text output to pretty printing. x can be string or number. 
color is specified by number from the following set:&]
[ {{625:625:625:625:625:625:625:625:625:625:625:625:625:625:625:625 [s0;= 0]
:: [s0;= 1]
:: [s0;= 2]
:: [s0;= 3]
:: [s0;= 4]
:: [s0;= 5]
:: [s0;= 6]
:: [s0;= 7]
:: [s0;= 8]
:: [s0;= 9]
:: [s0;= 10]
:: [s0;= 11]
:: [s0;= 12]
:: [s0;= 13]
:: [s0;= 14]
:: [s0;= 15]
::@0 [s0; ]
::@3 [s0; ]
::@4 [s0; ]
::@(128.128.0) [s0; ]
::@5 [s0; ]
::@(128.0.255) [s0; ]
::@(0.128.128) [s0; ]
::@1 [s0; ]
::@6 [s0; ]
::@(0.255.0) [s0; ]
::@(255.255.192) [s0; ]
::@(0.0.255) [s0; ]
::@(255.0.255) [s0; ]
::@8 [s0; ]
::@7 [s0; ]
::@2 [s0; ]}}]
:: [s0; [*@(0.0.255) ITEM`_COUNT](x)]
:: [s0; Reports the number of items in the container. If not called, 
the value is displayed as `"single`" (no number of elements and 
no list of elements). However, even single value [/ can] contain 
ITEM`_TYPE and ITEM`_PTR, which designate single value to be 
displayed in debugger.]
:: [s0; [*@(0.0.255) ITEM`_TYPE](x)]
:: [s0; Returns the type of value whose address is returned with one 
of ITEM`_PTR variants. Can be called multiple times (debugger 
then displays tuples `- useful for maps).]
:: [s0; [*@(0.0.255) ITEM`_PTR](adr)]
:: [s0; Adds a value address to be displayed in the debugger. Printing 
script should call this exactly [*@(0.0.255) items] times for each 
ITEM`_TYPE. As Esc scripting language is not exactly fast, it 
is better to use ITEM`_PTRS or ITEM`_DEREF`_PTRS instead.]
:: [s0; [*@(0.0.255) ITEM`_PTRS](adr, sz, from, items)]
:: [s0; Adds multiple value addresses `- much faster variant of script 
code&]
[s7;l0; for(i `= 0; i < items; i`+`+)&]
[s7;l0;      ITEM`_PTR(address `+ (i `+ from) `* sz)]
:: [s0; [*@(0.0.255) ITEM`_DEREF`_PTRS](adr, from, items)]
:: [s0; Adds multiple value addresses by derefencing pointers`- much 
faster variant of script code&]
[s7;l0; sz `= SizeOfPtr();&]
[s7;l0; for(i `= 0; i < items; i`+`+)&]
[s7;l0;      ITEM`_PTR(PeekPtr(address `+ (i `+ from) `* sz));]
:: [s0; [*@(0.0.255) STRING]()]
:: [s0; Signals to debugger that value should be displayed as string. 
ITEM`_TYPE must be some kind of integer type that is then interpreted 
as UNICODE characters.]
:: [s0; [*@(0.0.255) CHUNK](x)]
:: [s0; Debugger normally tries to load and display value items by 10000 
chunks, which might be hard for scripts to manage at reasonable 
speed and with set processing steps limit, especially if using 
ITEM`_PTR function. CHUNK can reduce this number to something 
more manageable.]}}&]
[s0; &]
[s0; Debugger helper function to create script skeleton&]
[s0; &]
[s0; There is a special menu entry that helps with creating pretty 
printing scripts:&]
[s0; &]
[s0; ]]