TITLE("Index class template")
REF("T Pop()")
REF("Index()")
REF("Index(pick_ Index& s)")
REF("Index(const Index& s, int)")
REF("Index(pick_ Vector<T>& s)")
REF("Index(const Vector<T>& s, int)")
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
"5:package`-title][{_}%EN-US [s10;~~~64; template <class [*@4 T], class [*@4 HashF"
"n] = StdHash<[*@4 T]> >&][s10;~~~64;* [* class ]Index&][s0; &][s12; [*C@4 T]-|Typ"
"e of elements stored in Index. T is required to be [*/ moveable ]and must have ei"
"ther [*/ deep copy constructor], [*/ pick constructor] or [*/ default constructor"
"].&][s12; [*C@4 HashFn]-|Hashing class. Must have defined [*C unsigned operator()"
"(const T`& x)] method returning hash value for elements. Defaults to[*  ][*C StdH"
"ash<T>] which requires [*C unsigned GetHashValue(const T`&)][C  ]function returni"
"ng hash value of elements to be defined.&][s3; Base class&][s9; [^dpp`:`/`/`:`:`/"
"AIndex`<class T`, class V`, class HashFn`>`/template `<class T`, class V`, class "
"HashFn`> class AIndex^ AIndex<T, Vector<T>, HashFn> ] &][s5; &][s5; Vector flavor"
" of index. Inherits most of its functionality from [^dpp`")
TOPIC_TEXT(
":`/`/`:`:`/AIndex`<class T`, class V`, class HashFn`>`/template `<class T`, clas"
"s V`, class HashFn`> class AIndex^ AIndex] and adds only members specific for its"
" flavor.&][s5; Like any other NTL container, Index is [*/^dpp`:`/`/SourceDoc`/Con"
"tainers`/Moveable^ moveable][*/  ]type with [*/^dpp`:`/`/SourceDoc`/Containers`/p"
"ick`_^ pick and optional deep copy] transfer semantics. Calling methods of picked"
" Index is logic error with exception of&][s0;/+75 [/C void ][*/C operator=][/C (p"
"ick`_ Index`& ][*/C@3 v][/C ) ](defined by composition)&][s0;/+75 [/C void ][*/C "
"operator<<=][/C (const AIndex`& ][*/C@3 v][/C )][/  ](defined in AIndex)&][s0;C+7"
"5 void [* Clear]()&][s0;C+75 bool [* IsPicked]() const&][s5; Optional deep copy o"
"perator is inherited from AIndex class. Pick operator is implicitly defined by co"
"mposition.&][s3; Members&][s4;:T Pop`(`): T [* Pop]()&][s6; Drops last element of"
" Index and returns its value.&][s11; Requires T to have deep copy constructor.&]["
"s1; [*/ Return value]-|Value of dropped element.&][s4;:Index`(`): [* Index]()&][s"
"6; Constructs empty Index.&][s4;:Index`(pick`_ Index`& s`): [* Index](pick`_ Inde"
"x`& [*@3 s])&][s6; Pick constructor. Transfers source Index in low constant time,"
" but destroys it by picking.&][s1; [*C@3 s]-|Source Index.&][s4; Index`& [* opera"
"tor]=(pick`_ Vector<T>`& [*@3 x])&][s6; Pick operator. Transfers source Vector to"
" Index in low constant time, but destroys it by picking.&][s1; [*C@3 x]-|Source V"
"ector.&][s4;:Index`(const Index`& s`, int`): [* Index](const Index`& [*@3 s], int"
")&][s6; Optional deep copy constructor.&][s11; Requires T to have deep copy const"
"ructor or optional deep copy constructor.&][s1; [*C@3 s]-|Source Index.&][s4;:Ind"
"ex`(pick`_ Vector`<T`>`& s`): [* Index](pick`_ Vector<T>`& [*@3 s])&][s6; Pick-co"
"nstructs Index from Vector. Transfers source container in low constant time, but "
"destroys it by picking.&][s1; [*C@3 s]-|Source Vector.&][s4;:Index`(const Vector`"
"<T`>`& s`, int`): [* Index](const Vector<T>`& [*@3 s], in")
TOPIC_TEXT(
"t)&][s6; Deep-copy constructs Index from Vector.&][s11; Requires T to have deep "
"copy constructor or optional deep copy constructor.&][s1; [*C@3 s]-|Source Vector"
".&][s0; ")
