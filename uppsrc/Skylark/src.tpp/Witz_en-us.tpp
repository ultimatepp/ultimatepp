topic "Witz template language";
[2 $$0,0#00000000000000000000000000000000:Default]
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[H4;b83;*4 $$3,5#07864147445237544204411237157677:title]
[i288;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;2 $$5,5#45413000475342174754091244180557:text]
[l288;b17;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;C@5;1 $$7,7#20902679421464641399138805415013:code]
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
[{_}%EN-US 
[s2; Witz&]
[s3; Preprocessing&]
[s0; &]
[s0; The first phase in compilation of witz templates is preprocessing. 
Preprocessing itself can be broken to include phase, when other 
template or template snippets are combined to form single body 
and subblock replacement phase, when all references to subblocks 
are replaced with defined subblocks.&]
[s0; &]
[s0; [@5 #include] [/ path]&]
[s0; &]
[s0; This attempts to retrieve and include another witz template 
into current body. Witz preprocessor attempts to retrieve language 
specific version of template first, then defaults to common version.&]
[s0; &]
[s0; [@5 #define] [/ id]&]
[s0; &]
[s0; This starts a new template subblock which ends with another 
subblock. First subblock (the one before first #define is encountered) 
has [/ id] `"MAIN`". The value of `"MAIN`" subblock represents 
the template at the end of preprocessing process.&]
[s0; &]
[s0; [@5 #][/ id]&]
[s0; &]
[s0; Occurence of # followed by [/ id] that is neither `"include`" 
or `"define`" inserts a subblock defined by [@5 #define] [/ id]. 
This process is recursive `- subblock keep replacing until there 
are no more #[/ id] in the block, however any [/ id] can be replaced 
only once (to avoid infinite recursion). If [/ id ]is encountered 
for which there is no corresponding [@5 #define], original text 
is left unchanged. If there are more definition for single id, 
the last one is used. ## is always replaced with single #.&]
[s0; &]
[s0; Example:&]
[s0; &]
[s0; [* base.witz:]&]
[s0; &]
[s7; <html>&]
[s7; <title>#TITLE</title>&]
[s7; <body>#BODY</body>&]
[s7; </html>&]
[s7; &]
[s7; #define TITLE Generic title&]
[s0; &]
[s0; [* page.witz:]&]
[s0;* &]
[s7; #include base&]
[s7; #define TITLE The page title&]
[s7; #define BODY&]
[s7; <b><i>Hello world!</i></b>&]
[s0; &]
[s3; Witz code&]
[s0; &]
[s0; Witz code is marked by `'`$`' character (the end of code within 
template is determined by syntax rules, in rare situations you 
might have to enclose expressions in parenthesis). Character 
`'`$`' can be escaped to be inserted into text as `'`$`$`'.&]
[s0; &]
[s0; [* Values]&]
[s0; &]
[s0; The type of values processed by Witz is basically equal to U`+`+ 
Value. Skylark understands some of Value types and perform operations 
on them (like arithmetics for numbers, concatenating Strings), 
but generally any Value can be yielded by expression evaluation 
(either come from handler variables, or be created by function). 
Skylark always uses .ToString method of Value to convert final 
expression value before inserting it to the document.&]
[s0; &]
[s0; Witz primary values come from shared variable space of Skylark 
handler; the only other way to define a new variable is by using 
[/ for] statements. Variable names follow C/C`+`+ rules, `'.`' 
before variable designates session variable, `'`@`' designates 
cookie.&]
[s0; &]
[s0; There are also value literals, generally following JavaScript 
syntax:&]
[s0; &]
[s0; numbers: [@5 1], [@5 `-100], [@5 1.23]&]
[s0; strings: [@5 `"string`"]&]
[s0; arrays: [@5 `[1, `"hello`", 123`]]&]
[s0; maps: [@5 `{ `"key1`":`"value1`", `"key2`":123 `}]&]
[s0; &]
[s3;H0; [+150 Expressions]&]
[s0; &]
[s0; When expressions are used as logical values, then:&]
[s0; &]
[s0;i150;O0; if expression yields a number (int, int64, double, bool), 
it is false if it is Null or zero, true otherwise&]
[s0;i150;O0; if expression yields a ValueArray, it is false if ValueArray 
is empty, true otherwise&]
[s0;i150;O0; in other cases, it is false if expression is Null, true 
otherwise (note that this means that empty String is false, non`-empty 
true).&]
[s0; &]
[s0; In following table, thick lines divide operators with the same 
priority, with topmost items having the highest priority:&]
[s0; &]
[ {{4468:5532h1;@(216) [s0; Operator]
:: [s0; Comments]
::^@2 [s0; [/C map][*C `[][/C key][*C `]]]
:: [s0; Map value at [/ key]. If there is no such ]
:: [s0; [/C map][*C .][/C field]]
:: [s0; Same as [/ map]`[`"[/ field]`"`]. Note that there are 4 special 
pseudo`-fields with different meaning.]
:: [s0; [/C variable][*C .`_first]]
:: [s0; Variable must be control variable of for cycle. Returns true 
if current iteration is first.]
:: [s0; [/C variable][*C .`_last]]
:: [s0; Variable must be control variable of for cycle. Returns true 
if current iteration is last.]
:: [s0; [/C variable][*C .`_index]]
:: [s0; Variable must be control variable of for cycle. Returns zero`-based 
index of iteration.]
:: [s0; [/C variable][*C .`_key]]
:: [s0; Variable must be control variable of for cycle. Returns the 
key when iterating the map.]
:: [s0; [/C array][*C `[][/C index][*C `]]]
:: [s0; Array element at position [/ index].]
:: [s0; [/C function(value1, value2, ...)]]
:: [s0; Call to function (functions are defined in C`+`+).]
:: [s0; [/C handler(value1, value2, ...)]]
:: [s0; If the id before `'(`' represents Skylark handler, then what 
looks like function call gets actually translated to into the 
URL link to Skylark handler function, replacing [/ value] parameters 
at their corresponding positions in the URL, so they act as handler 
parameters.]
::t33/15 [s0; [*C `-][/C number]]
:: [s0; Unary minus.]
::t0/15 [s0; [*C `~][/C number]]
:: [s0; Bit`-wise complement.]
:: [s0; [*C !][/C value]]
:: [s0; Logical not. 1 when value represents false, 0 otherwise.]
::t33/15 [s0; [/C number][*C  `* ][/C number]]
:: [s0; Multiplication.]
::t0/15 [s0; [/C number][*C  / ][/C number]]
:: [s0; Division.]
:: [s0; [/C number][*C  % ][/C number]]
:: [s0; Modulo.]
::t33/15 [s0; [/C String][*C  `+ ][/C String]]
:: [s0; Concatenates [/ Strings].]
::t0/15 [s0; [/C number][*C  `+ ][/C number]]
:: [s0; Addition.]
:: [s0; [/C number][*C  `- ][/C number]]
:: [s0; Subtraction.]
::t33/15 [s0; [/C number][*C  << ][/C number]]
:: [s0; Shift left.]
::t0/15 [s0; [/C number][*C  >> ][/C number]]
:: [s0; Shift right.]
::t33/15 [s0; [/C number][*C  < ][/C number]&]
[s0; [/C number][*C  > ][/C number]&]
[s0; [/C number][*C  <`= ][/C number]&]
[s0; [/C number][*C  >`= ][/C number]]
:: [s0; Comparison of [/ number]s.]
::t0/15 [s0; [/C String][*C  < ][/C String]&]
[s0; [/C String ][*C > ][/C String]&]
[s0; [/C String][*C  <`= ][/C String]&]
[s0; [/C String][*C  >`= ][/C String]]
:: [s0; Comparison of [/ Strings].]
::t33/15 [s0; [/C value][*C  `=`= ][/C value]]
:: [s0; Equality.]
::t0/15 [s0; [/C value][*C  !`= ][/C value]]
:: [s0; Inequality.]
::t33/15 [s0; [/C number][*C  `& ][/C number]]
:: [s0; Binary and.]
:: [s0; [/C number][*C  `^ ][/C number]]
:: [s0; Binary xor.]
:: [s0; [/C number][*C  `| ][/C number]]
:: [s0; Binary or.]
:: [s0; [/C value][*C  `&`& ][/C value]]
:: [s0; Logical and. If first value is false, second value is not evaluated, 
just skipped.]
:: [s0; [/C value][*C  `|`| ][/C value]]
:: [s0; Logical or. If first value is true, second value is not evaluated, 
just skipped.]
:: [s0; [/C value ][*C ?][/C  value ][*C :][/C  value]]
:: [s0; Conditional expression. Only necessary expressions are evaluated.]}}&]
[s0; &]
[s0; &]
[s3; Statements&]
[s0; &]
[s0; Note that for structured statements, `'`$/`' is an alternative 
to `'`$endfor`' and `'`$endif`'.&]
[s0; &]
[ {{4508:5492h1;@(216) [s0; Statement]
:: [s0; Comments]
::^@2 [s0; [*C `$][/C expression]]
:: [s0; Expression. It gets evaluated and the result is put into the 
document.]
:: [s0; [*C `$if(][/C condition][*C )]&]
[s0; [*C ...]&]
[s0; [*C `$endif ][/C or][*C  `$/]]
:: [s0; Conditional statement.]
:: [s0; [*C `$if(][/C condition][*C )]&]
[s0; [*C ...]&]
[s0; [*C `$else ]&]
[s0; [*C ...]&]
[s0; [*C `$endif ][/C or][*C  `$/]]
:: [s0; Conditional statement with else clause.]
:: [s0; [*C `$for(][/C variable][*C  in ][/C array`_or`_map][*C )]&]
[s0; [*C ...]&]
[s0; [*C `$endfor ][/C or][*C  `$/]&]
[s0;*C &]
[s0;*C ]
:: [s0; Loop iterating through ValueMap or ValueArray. Variable is assigned 
all values of container while special pseudo`-fields are provided 
for this value:&]
[s0; &]
[s0; [*C .`_first ]true if element is first in the container&]
[s0; [*C .`_last ]true if element is last in the container&]
[s0; [*C .`_index] index of element&]
[s0; [*C .`_key] if container is a map, returns the key of current 
element&]
[s0; ]
:: [s0; [*C `$for(][/C variable][*C  in ][/C array`_or`_map][*C )]&]
[s0; [*C ...]&]
[s0; [*C `$else]&]
[s0; [*C ...]&]
[s0; [*C `$endfor ][/C or][*C  `$/]&]
[s0;*C ]
:: [s0; The optional else clause is invoked if no iteration is performed 
(map or array is empty).]}}&]
[s0; &]
[s3; [3 Standard library]&]
[s0;3 &]
[ {{4495:5505h1;@(216) [s0; Function]
:: [s0; Description]
::^@2 [s0; [*C cycle(i, p1, ..., pn)]]
:: [s0; Returns p`[i mod n`].]
:: [s0; [*C raw(text)]]
:: [s0; Returns argument as `"raw`" text that is not supposed to be 
html escaped.]
:: [s0; [*C count(array`_or`_map)]]
:: [s0; Returns the number of elements in ValueArray or ValueMap; 0 
if parameter is neither.]
:: [s0; [*C post`_identity()]]
:: [s0; Returns a complete definition of FORM hiddent field needed for 
CSRF attack prevention.]
:: [s0; [*C js`_identity()]]
:: [s0; Returns a script required that provides CSRF protection for 
AJAX POSTs.]
:: [s0; [*C set()]]
:: [s0; Returns html formatted list of current shared variables. Useful 
for debugging.]
:: [s0; [*C render(witz`_template, p1, ...)]]
:: [s0; Inserts another template into output. Current variables are 
copied to new template, parameters p1... are copied as `$`_1, 
`$`_2 etc. This is similar to using #include, but with it is 
easier to pass arguments to included template this way.]}}&]
[s0; ]]