topic "EditField derived classes";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s0; [*@3+150 EditField derived classes]&]
[s9; Widgets editing values in text representation are based on EditField 
and appropriate [^`:`:Convert^ Convert] class. Majority of them 
are defined using simple EditValue, EditMinMax and EditMinMaxNotNull 
template classes:&]
[s3; &]
[s0;%- &]
[ {{10000@3 [s0;%- [*@(229)4 EditValue]]}}&]
[s3; &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 DataType][3 , 
][@(0.0.255)3 class][3 _][*@3;3 Cv][@(0.0.255)3 >]&]
[s1;:EditValue`:`:class:%- [@(0.0.255) class]_[* EditValue]_:_[@(0.0.255) public]_[*@3 EditFie
ld], [@(0.0.255) public]_[*@3 Cv]&]
[s9; This class provides composition of EditField and specific [^topic`:`/`/Core`/src`/Convert`$en`-us`#Convert`:`:class^ C
onvert] class. The advantage of deriving from Convert is that 
all Convert setup methods (like setting min/max value) are directly 
available without using further glue code.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3;%- &]
[s5;:EditValue`:`:EditValue`(`):%- [* EditValue]()&]
[s2; Constructor assigns [^`:`:Convert^ Convert] class (in fact, `*this) 
as the EditField converter.&]
[s3; &]
[s3; &]
[s0;%- &]
[ {{10000@3 [s0;%- [*@(229)4 EditMinMax]]}}&]
[s3; &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 DataType][3 , 
][@(0.0.255)3 class][3 _][*@4;3 Cv][@(0.0.255)3 >]&]
[s1;:EditMinMax`:`:class:%- [@(0.0.255) class]_[* EditMinMax]_:_[@(0.0.255) public]_[*@3 EditV
alue][@(0.0.255) <][*@4 DataType], [*@4 Cv][@(0.0.255) >]_&]
[s9; Wrapper class for converters providing Min, Max and NotNull 
properties.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3;%- &]
[s5;:EditMinMax`:`:EditMinMax`(`):%- [* EditMinMax]()&]
[s2; Default constructor.&]
[s3; &]
[s4;%- &]
[s5;:EditMinMax`:`:EditMinMax`(DataType`,DataType`):%- [* EditMinMax]([*@4 DataType]_[*@3 m
in], [*@4 DataType]_[*@3 max])&]
[s2; This constructor sets [* Min] and [* Max] properties.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:EditMinMax`:`:Min`(DataType`):%- [_^EditMinMax^ EditMinMax][@(0.0.255) `&]_[* Min]([*@4 D
ataType]_[*@3 min])&]
[s5;:EditMinMax`:`:Max`(DataType`):%- [_^EditMinMax^ EditMinMax][@(0.0.255) `&]_[* Max]([*@4 D
ataType]_[*@3 max])&]
[s5;:EditMinMax`:`:NotNull`(bool`):%- [_^EditMinMax^ EditMinMax][@(0.0.255) `&]_[* NotNull](
[@(0.0.255) bool]_[*@3 nn]_`=_[@(0.0.255) true])&]
[s2; Sets [* Min], [* Max] and [* NotNull] properties. In fact, the only 
purpose of these methods is to change the returns value to the 
correctly typed `*this.&]
[s3; &]
[s0; &]
[s0; &]
[s0;%- &]
[ {{10000@3 [s0;%- [*@(229)4 EditMinMaxNotNull]]}}&]
[s3; &]
[s1;:noref:%- [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 DataType][3 , 
][@(0.0.255)3 class][3 _][*@4;3 Cv][@(0.0.255)3 >]&]
[s1;:EditMinMaxNotNull`:`:class:%- [@(0.0.255) class]_[* EditMinMaxNotNull]_:_[@(0.0.255) pu
blic]_[*@3 EditValue][@(0.0.255) <][*@4 DataType], [*@4 Cv][@(0.0.255) >]_&]
[s9; Similar to EditMinMax template class, but constructs activate 
[* NotNull] property.&]
[s3; &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3;%- &]
[s5;:EditMinMaxNotNull`:`:EditMinMaxNotNull`(`):%- [* EditMinMaxNotNull]()&]
[s2; Default constructor.&]
[s3; &]
[s4;%- &]
[s5;:EditMinMaxNotNull`:`:EditMinMaxNotNull`(DataType`,DataType`):%- [* EditMinMaxNotNu
ll]([*@4 DataType]_[*@3 min], [*@4 DataType]_[*@3 max])&]
[s2; This constructor sets [* Min] and [* Max] properties.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:EditMinMaxNotNull`:`:Min`(DataType`):%- [_^EditMinMaxNotNull^ EditMinMaxNotNull][@(0.0.255) `&
]_[* Min]([*@4 DataType]_[*@3 min])&]
[s5;:EditMinMaxNotNull`:`:Max`(DataType`):%- [_^EditMinMaxNotNull^ EditMinMaxNotNull][@(0.0.255) `&
]_[* Max]([*@4 DataType]_[*@3 max])&]
[s5;:EditMinMaxNotNull`:`:NotNull`(bool`):%- [_^EditMinMaxNotNull^ EditMinMaxNotNull][@(0.0.255) `&
]_[* NotNull]([@(0.0.255) bool]_[*@3 nn]_`=_[@(0.0.255) true])&]
[s2; Sets [* Min], [* Max] and [* NotNull] properties. In fact, the only 
purpose of these methods is to change the returns value to the 
correctly typed `*this.&]
[s3; &]
[s0; &]
[s0; &]
[s0; &]
[s0; Based on EditMinMax and EditMinMaxNotNull, basic value editors 
are defined as typedefs:&]
[s3; &]
[s5;:EditDate`:`:typedef:%- [@(0.0.255) typedef]_[_^EditMinMax^ EditMinMax][@(0.0.255) <][_^Date^ D
ate], [_^ConvertDate^ ConvertDate][@(0.0.255) >]_[* EditDate]&]
[s2; Edit field for date values.&]
[s3;%- &]
[s4;%- &]
[s5;:EditDateNotNull`:`:typedef:%- [@(0.0.255) typedef]_[_^EditMinMaxNotNull^ EditMinMaxN
otNull][@(0.0.255) <][_^Date^ Date], [_^ConvertDate^ ConvertDate][@(0.0.255) >]_[* EditDate
NotNull]&]
[s2; Edit field for not null date values.&]
[s3;%- &]
[s4;%- &]
[s5;:EditDouble`:`:typedef:%- [@(0.0.255) typedef]_[_^EditMinMax^ EditMinMax][@(0.0.255) <d
ouble], [_^ConvertDouble^ ConvertDouble][@(0.0.255) >]_[* EditDouble]&]
[s2; Edit field for double values.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:EditFloat`:`:typedef:%- [@(0.0.255) typedef]_[_^Upp`:`:EditMinMax^ EditMinMax
]<[@(0.0.255) double], ConvertFloat>_[* EditFloat]&]
[s2; Edit field for float values. The difference from EditDouble 
is that when decimals are rounded to be presented to user, EditFloat 
rounds to just 7 valid digits to reflect the precision of float 
type.&]
[s3;%- &]
[s4;%- &]
[s5;:EditDoubleNotNull`:`:typedef:%- [@(0.0.255) typedef]_[_^EditMinMaxNotNull^ EditMinMa
xNotNull][@(0.0.255) <double], [_^ConvertDouble^ ConvertDouble][@(0.0.255) >]_[* EditDoub
leNotNull]&]
[s2; Edit field for not null double values.&]
[s3;%- &]
[s4;%- &]
[s5;:EditInt`:`:typedef:%- [@(0.0.255) typedef]_[_^EditMinMax^ EditMinMax][@(0.0.255) <int],
 [_^ConvertInt^ ConvertInt][@(0.0.255) >]_[* EditInt]&]
[s2; Edit field for integer values.&]
[s3;%- &]
[s4;%- &]
[s5;:EditIntNotNull`:`:typedef:%- [@(0.0.255) typedef]_[_^EditMinMaxNotNull^ EditMinMaxNo
tNull][@(0.0.255) <int], [_^ConvertInt^ ConvertInt][@(0.0.255) >]_[* EditIntNotNull]&]
[s2; Edit field for not null integer values.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:EditInt64NotNull`:`:typedef:%- [@(0.0.255) typedef]_[_^Upp`:`:EditMinMaxNotNull^ E
ditMinMaxNotNull]<[_^Upp`:`:int64^ int64], [_^Upp`:`:EditInt64^ EditInt64]>_[* EditInt6
4NotNull]&]
[s2; Edit field for not null int64 values.&]
[s3;%- &]
[s4;%- &]
[s5;:Upp`:`:EditFloatNotNull`:`:typedef:%- [@(0.0.255) typedef]_[_^Upp`:`:EditMinMaxNotNull^ E
ditMinMaxNotNull]<[@(0.0.255) double], [_^Upp`:`:EditFloat^ EditFloat]>_[* EditFloatNot
Null]&]
[s2; Edit field for not null float values.&]
[s3;%- &]
[s4;%- &]
[s5;:EditInt64`:`:typedef:%- [@(0.0.255) typedef]_[_^EditMinMax^ EditMinMax][@(0.0.255) <][_^int64^ i
nt64], [_^ConvertInt64^ ConvertInt64][@(0.0.255) >]_[* EditInt64]&]
[s2; Edit field for 64 bit integer values.&]
[s3;%- &]
[s4;%- &]
[s5;:EditTime`:`:typedef:%- [@(0.0.255) typedef]_[_^EditMinMax^ EditMinMax][@(0.0.255) <][_^Time^ T
ime], [_^ConvertTime^ ConvertTime][@(0.0.255) >]_[* EditTime]&]
[s2; Edit field for time values.&]
[s3;%- &]
[s4;%- &]
[s5;:EditTimeNotNull`:`:typedef:%- [@(0.0.255) typedef]_[_^EditMinMaxNotNull^ EditMinMaxN
otNull][@(0.0.255) <][_^Time^ Time], [_^ConvertTime^ ConvertTime][@(0.0.255) >]_[* EditTime
NotNull]&]
[s2; Edit field for not null time values.&]
[s3;%- &]
[s0;%- &]
[s0; &]
[s0; &]
[s0; Some value editors are not based on EditMinMax and EditMinMaxNotNull:&]
[s3; &]
[s0;%- &]
[ {{10000@3 [s0;%- [*@(229)4 EditString]]}}&]
[s3; &]
[s1;:EditString`:`:class:%- [@(0.0.255)3 class][3 _][*3 EditString][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 EditValue][@(0.0.255)3 <][_^WString^3 WString][3 , ][_^ConvertString^3 ConvertStrin
g][@(0.0.255)3 >][3 _]&]
[s9; EditString is not based on EditMinMax because instead of minimal 
and maximal values, it implements `"maximum number of characters`" 
constraint.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3;%- &]
[s5;:EditString`:`:EditString`(`):%- [* EditString]()&]
[s2; Default constructor.&]
[s3; &]
[s4;%- &]
[s5;:EditString`:`:EditString`(int`):%- [* EditString]([@(0.0.255) int]_[*@3 maxlen])&]
[s2; This constructor sets [%-*@3 maxlen] as maximum length of contained 
text.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:EditString`:`:MaxLen`(int`):%- [_^EditString^ EditString][@(0.0.255) `&]_[* MaxLen]([@(0.0.255) i
nt]_[*@3 maxlen])&]
[s2; Sets [%-*@3 maxlen] as maximum length of contained text.[%-*@3 maxlen] 
Returns `*this.&]
[s3; &]
[s4;%- &]
[s5;:EditString`:`:NotNull`(bool`):%- [_^EditString^ EditString][@(0.0.255) `&]_[* NotNull](
[@(0.0.255) bool]_[*@3 nn]_`=_[@(0.0.255) true])&]
[s2;%- [%% Sets ][* NotNull] property.&]
[s3; &]
[s4;%- &]
[s5;:Upp`:`:EditString`:`:TrimLeft`(bool`):%- [_^Upp`:`:EditString^ EditString][@(0.0.255) `&
]_[* TrimLeft]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s5;:Upp`:`:EditString`:`:TrimRight`(bool`):%- [_^Upp`:`:EditString^ EditString][@(0.0.255) `&
]_[* TrimRight]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s5;:Upp`:`:EditString`:`:TrimBoth`(bool`):%- [_^Upp`:`:EditString^ EditString][@(0.0.255) `&
]_[* TrimBoth]([@(0.0.255) bool]_[*@3 b]_`=_[@(0.0.255) true])&]
[s2; Whitechars on the left/right/both side(s) are removed.&]
[s0; &]
[s0; &]
[s0;%- &]
[ {{10000@3 [s0;%- [*@(229)4 EditStringNotNull]]}}&]
[s3; &]
[s1;:EditStringNotNull`:`:class:%- [@(0.0.255)3 class][3 _][*3 EditStringNotNull][3 _:_][@(0.0.255)3 p
ublic][3 _][*@3;3 EditString]&]
[s9; This class sets the NotNull property in the constructors.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3;%- &]
[s5;:EditStringNotNull`:`:EditStringNotNull`(`):%- [* EditStringNotNull]()&]
[s2; Default constructor.&]
[s3; &]
[s4;%- &]
[s5;:EditStringNotNull`:`:EditStringNotNull`(int`):%- [* EditStringNotNull]([@(0.0.255) i
nt]_[*@3 maxlen])&]
[s2; This constructor sets [%-*@3 maxlen] as maximum length of contained 
text.&]
[s3; &]
[s0; &]
[s0; &]
[s0;%- &]
[ {{10000@3 [s0;%- [*@(229)4 EditIntSpin]]}}&]
[s3; &]
[s1;:EditIntSpin`:`:class:%- [@(0.0.255)3 class][3 _][*3 EditIntSpin][3 _:_][@(0.0.255)3 public][3 _
][*@3;3 EditInt]&]
[s9; This editor adds spin buttons to the EditInt class.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3;%- &]
[s5;:EditIntSpin`:`:EditIntSpin`(`):%- [* EditIntSpin]()&]
[s2; Default constructor. Spin buttons are on.&]
[s3; &]
[s4;%- &]
[s5;:EditIntSpin`:`:EditIntSpin`(int`,int`):%- [* EditIntSpin]([@(0.0.255) int]_[*@3 min], 
[@(0.0.255) int]_[*@3 max])&]
[s2; This constructor sets the [%-*@3 min] and [%-*@3 max] values. Spin 
buttons are on.&]
[s3; &]
[s4;%- &]
[s5;:EditIntSpin`:`:`~EditIntSpin`(`):%- [@(0.0.255) `~][* EditIntSpin]()&]
[s2; Default destructor.&]
[s3;%- &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:EditIntSpin`:`:ShowSpin`(bool`):%- [_^EditIntSpin^ EditIntSpin][@(0.0.255) `&]_[* Show
Spin]([@(0.0.255) bool]_[*@3 s]_`=_[@(0.0.255) true])&]
[s2; Shows/hides spin buttons.&]
[s3; &]
[s4;%- &]
[s5;:EditIntSpin`:`:SetInc`(int`):%- [_^EditIntSpin^ EditIntSpin][@(0.0.255) `&]_[* SetInc](
[@(0.0.255) int]_[*@3 `_inc])&]
[s2; Set the increment factor to [%-*@3 `_inc].&]
[s3; &]
[s4;%- &]
[s5;:EditIntSpin`:`:GetInc`(`)const:%- [@(0.0.255) int]_[* GetInc]()_[@(0.0.255) const]&]
[s2; Returns the increment factor.&]
[s3;%- &]
[s0; &]
[s0; &]
[s0;%- &]
[ {{10000@3 [s0;%- [*@(229)4 EditDoubleSpin]]}}&]
[s3; &]
[s1;:EditDoubleSpin`:`:class:%- [@(0.0.255)3 class][3 _][*3 EditDoubleSpin][3 _:_][@(0.0.255)3 p
ublic][3 _][*@3;3 EditDouble]&]
[s9; This editor adds spin buttons to the EditDouble class.&]
[s3; &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3;%- &]
[s5;:EditDoubleSpin`:`:EditDoubleSpin`(double`):%- [* EditDoubleSpin]([@(0.0.255) double]_
[*@3 inc]_`=_[@3 0][@(0.0.255) .][@3 1])&]
[s2; Sets the spin increment to [%-*@3 inc]. Spin buttons are on.&]
[s3; &]
[s4;%- &]
[s5;:EditDoubleSpin`:`:EditDoubleSpin`(double`,double`,double`):%- [* EditDoubleSpin]([@(0.0.255) d
ouble]_[*@3 min], [@(0.0.255) double]_[*@3 max], [@(0.0.255) double]_[*@3 inc]_`=_[@3 0][@(0.0.255) .
][@3 1])&]
[s2; This constructor sets [%-*@3 min], [%-*@3 max] and [%-*@3 inc] values. 
Spin buttons are on.&]
[s3; &]
[s4;%- &]
[s5;:EditDoubleSpin`:`:`~EditDoubleSpin`(`):%- [@(0.0.255) `~][* EditDoubleSpin]()&]
[s2; Default destructor.&]
[s3;%- &]
[s0; &]
[ {{10000F(128)G(128)@1 [s0; [* Public Method List]]}}&]
[s3;%- &]
[s5;:EditDoubleSpin`:`:SetInc`(double`):%- [_^EditDoubleSpin^ EditDoubleSpin][@(0.0.255) `&
]_[* SetInc]([@(0.0.255) double]_[*@3 `_inc]_`=_[@3 0][@(0.0.255) .][@3 1])&]
[s2; Set the spin increment to [%-*@3 `_inc].&]
[s3; &]
[s4;%- &]
[s5;:EditDoubleSpin`:`:GetInc`(`)const:%- [@(0.0.255) double]_[* GetInc]()_[@(0.0.255) cons
t]&]
[s2; Returns current spin increment.&]
[s3; &]
[s4;%- &]
[s5;:EditDoubleSpin`:`:ShowSpin`(bool`):%- [_^EditDoubleSpin^ EditDoubleSpin][@(0.0.255) `&
]_[* ShowSpin]([@(0.0.255) bool]_[*@3 s]_`=_[@(0.0.255) true])&]
[s2; Shows/hides spin buttons.&]
[s3; &]
[s0; &]
[s0; &]
[s0;%- &]
[ {{10000@3 [s0;%- [*@(229)4 EditDoubleNotNullSpin]]}}&]
[s3; &]
[s1;:EditDoubleNotNullSpin`:`:class:%- [@(0.0.255)3 class][3 _][*3 EditDoubleNotNullSpin][3 _
:_][@(0.0.255)3 public][3 _][*@3;3 EditDoubleSpin]&]
[s9; This editor adds spin buttons to the EditDoubleNotNull class.&]
[s3;%- &]
[s0;%- &]
[ {{10000F(128)G(128)@1 [s0; [* Constructor Detail]]}}&]
[s3; &]
[s5;:EditDoubleNotNullSpin`:`:EditDoubleNotNullSpin`(double`):%- [* EditDoubleNotNullSp
in]([@(0.0.255) double]_[*@3 inc]_`=_[@3 0][@(0.0.255) .][@3 1])&]
[s2; Sets the spin increment to [%-*@3 inc]. Spin buttons are on.&]
[s3; &]
[s4;%- &]
[s5;:EditDoubleNotNullSpin`:`:EditDoubleNotNullSpin`(double`,double`,double`):%- [* Edi
tDoubleNotNullSpin]([@(0.0.255) double]_[*@3 min], [@(0.0.255) double]_[*@3 max], 
[@(0.0.255) double]_[*@3 inc]_`=_[@3 0][@(0.0.255) .][@3 1])&]
[s2; This constructor sets [%-*@3 min], [%-*@3 max] and [%-*@3 inc] values. 
Spin buttons are on.&]
[s3; &]
[s0; ]]