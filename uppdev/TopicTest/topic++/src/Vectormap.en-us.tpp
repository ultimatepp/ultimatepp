TITLE("VectorMap class template")
REF("T Pop()")
REF("VectorMap(const VectorMap& s, int)")
REF("VectorMap(pick_ Index<K>& ndx, pick_ Vector<T>& val)")
REF("VectorMap(pick_ Vector<K>& ndx, pick_ Vector<T>& val)")
REF("VectorMap()")
TOPIC_TEXT(
"[ $$0,0#00000000000000000000000000000000:Default][l288;i704;a17;O9;~~~.992; $$1,"
"0#10431211400427159095818037425705:param][a83;*R6 $$2,5#3131016247420302412518841"
"7583966:caption][b83;* $$3,5#07864147445237544204411237157677:title][b167;a42;C $"
"$4,6#40027414424643823182269349404212:item][b42;a42; $$5,5#4541300047534217475409"
"1244180557:text][l288;a17; $$6,6#27521748481378242620020725143825:desc][l321;t246"
";C@5;1 $$7,7#20902679421464641399138805415013:code][b2503; $$8,0#6514237545610002"
"3862071332075487:separator][*@(0.0.255) $$9,0#83433469410354161042741608181528:ba"
"se][t4167;C+117 $$10,0#37138531426314131251341829483380:class][l288;a17;*1 $$11,1"
"1#70004532496200323422659154056402:requirement][i416;b42;a42;O9;~~~.416; $$12,12#"
"10566046415157235020018451313112:tparam][b167;C $$13,13#9243045944346046191110808"
"0531343:item1][a42;C $$14,14#77422149456609303542238260500223:item2][*@2$(0.128.1"
"28) $$15,15#34511555403152284025741354420178:NewsDate][l321;*C$7 $$16,16#03451589"
"433145915344929335295360:result][l321;b83;a83;*C$7 $$17,17#0753155046352950537122"
"8428965313:result`-line][l160;t4167;*C+117 $$18,5#8860394944220582595880005322242"
"5:package`-title][{_}%EN-US [s10;* [* template <class ][@4 K][* , class ][@4 T][*"
" , class ][@4 HashFn][*  = StdHash<][@4 K][* > > class ]VectorMap&][s5; &][s12; ["
"*C@4 K]-|Type of keys. K must have deep copy constructor, be [*/^dpp`:`/`/SourceD"
"oc`/Containers`/Moveable^ moveable] and must have [*C operator==] defined.&][s12;"
" [*C@4 T]-|Type of elements stored in Vector. T is required to be [*/^dpp`:`/`/So"
"urceDoc`/Containers`/Moveable^ moveable][*/  ]and must have either [*/^dpp`:`/`/S"
"ourceDoc`/Containers`/pick`_^ deep copy constructor], [*/^dpp`:`/`/SourceDoc`/Con"
"tainers`/pick`_^ pick constructor] or [*/^dpp`:`/`/SourceDoc`/Containers`/pick`_^"
" default constructor].&][s12; [*C@4 HashFn]-|Hashing class. Must have defined [*C"
" unsigned operator()(const K`& x)] method returning hash value for elements.&][s3"
"; Base classes&][s9; AMap< K, T, Vector<T>, HashFn >  &][")
TOPIC_TEXT(
"s0; &][s5; Vector flavor of map. Inherits most of its functionality from [^dpp`:"
"`/`/`:`:`/AMap`<class K`, class T`, class V`, class HashFn`>`/template `<class K`"
", class T`, class V`, class HashFn`> class AMap^ AMap] and adds only members spec"
"ific for its flavor.&][s5; Like any other NTL container, VectorMap is [*/^dpp`:`/"
"`/SourceDoc`/Containers`/Moveable^ moveable][*/  ]type with [*/^dpp`:`/`/SourceDo"
"c`/Containers`/pick`_^ pick and optional deep copy] transfer semantics. Calling m"
"ethods of picked VectorMap is logic error with exception of&][s0;/+75 [/C void ]["
"*/C operator=][/C (pick`_ VectorMap`& ][*/C@3 v][/C ) ](defined by composition)&]"
"[s0;/+75 [/C void ][*/C operator<<=][/C (const AMap`& ][*/C@3 v][/C )][/  ](defin"
"ed in AMap)&][s0;C+75 void [* Clear]()&][s0;C+75 bool [* IsPicked]() const&][s5; "
"Optional deep copy operator is inherited from AMap class. Pick operator is implic"
"itly defined by composition.&][s3; Members&][s4;:T Pop`(`): T [* Pop]()&][s6; Dro"
"ps last element of VectorMap and returns its value.&][s1; [*/ Return value]-|Valu"
"e of dropped element.&][s4;:VectorMap`(const VectorMap`& s`, int`): [* VectorMap]"
"(const VectorMap`& [*@3 s], int)&][s6; Optional deep copy constructor.&][s6;*1 Re"
"quires T to have deep copy constructor or optional deep copy constructor.&][s1; ["
"*C@3 s]-|Source VectorMap.&][s4;:VectorMap`(pick`_ Index`<K`>`& ndx`, pick`_ Vect"
"or`<T`>`& val`): [* VectorMap](pick`_ Index<K>`& [*@3 ndx], pick`_ Vector<T>`& [*"
"@3 val])&][s6; Pick-constructs VectorMap from Index of keys and Vector of values."
" Both source containers should have the same number of elements.&][s1; [*C@3 ndx]"
"-|Source Index.&][s1; [*C@3 val]-|Source Vector.&][s4;:VectorMap`(pick`_ Vector`<"
"K`>`& ndx`, pick`_ Vector`<T`>`& val`): [* VectorMap](pick`_ Vector<K>`& [*@3 ndx"
"], pick`_ Vector<T>`& [*@3 val])&][s6; Pick-constructs VectorMap from Vector of k"
"eys and Vector of values. Both source containers should have the same number of e"
"lements.&][s1; [*C@3 ndx]-|Source Vector of keys.&][s1; [")
TOPIC_TEXT(
"*C@3 val]-|Source Vector of values.&][s4;:VectorMap`(`): [* VectorMap]()&][s6; D"
"efault constructor. Constructs empty VectorMap.")
