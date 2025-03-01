topic "Serialization utilities";
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
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@2;4 Serialization utilities]]}}&]
[s0;*@3;4%% &]
[ {{10000t/25b/25@1 [s0; [* Function List]]}}&]
[s0;0 &]
[s5;:Pack16`(Stream`&`,int`&`): [_^Stream^ Stream][@(0.0.255) `&]_[* Pack16]([_^Stream^ Strea
m][@(0.0.255) `&]_[*@3 s], [@(0.0.255) int`&]_[*@3 i])&]
[s2;%% Serializes 32`-bit data, optimizing it for 16`-bit values. 
If value is in `-32767`-32767 range, it is serialized as 2 bytes, 
otherwise 6 bytes are used.&]
[s7;%% [%-*C@3 s]-|Stream.&]
[s7;%% [%-*C@3 i]-|Data to serialize.&]
[s7;%% [*/ Return value]-|s for chaining.&]
[s3;%% &]
[s4; &]
[s5;:Pack16`(Stream`&`,int`&`,int`&`): [_^Stream^ Stream][@(0.0.255) `&]_[* Pack16]([_^Stream^ S
tream][@(0.0.255) `&]_[*@3 s], [@(0.0.255) int`&]_[*@3 i1], [@(0.0.255) int`&]_[*@3 i2])&]
[s5;:Pack16`(Stream`&`,int`&`,int`&`,int`&`): [_^Stream^ Stream][@(0.0.255) `&]_[* Pack16](
[_^Stream^ Stream][@(0.0.255) `&]_[*@3 s], [@(0.0.255) int`&]_[*@3 i1], 
[@(0.0.255) int`&]_[*@3 i2], [@(0.0.255) int`&]_[*@3 i3])&]
[s5;:Pack16`(Stream`&`,int`&`,int`&`,int`&`,int`&`): [_^Stream^ Stream][@(0.0.255) `&]_[* P
ack16]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 s], [@(0.0.255) int`&]_[*@3 i1], 
[@(0.0.255) int`&]_[*@3 i2], [@(0.0.255) int`&]_[*@3 i3], [@(0.0.255) int`&]_[*@3 i4])&]
[s5;:Pack16`(Stream`&`,int`&`,int`&`,int`&`,int`&`,int`&`): [_^Stream^ Stream][@(0.0.255) `&
]_[* Pack16]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 s], [@(0.0.255) int`&]_[*@3 i1], 
[@(0.0.255) int`&]_[*@3 i2], [@(0.0.255) int`&]_[*@3 i3], [@(0.0.255) int`&]_[*@3 i4], 
[@(0.0.255) int`&]_[*@3 i5])&]
[s2;%% Calls Pack16 for each of individual 32`-bit integer parameters.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Store`(Event`,Stream`&`,int`): [@(0.0.255) bool] [* Store](Event<Stream[@(0.0.255) `&
]> [*@3 serialize], Stream[@(0.0.255) `&] [*@3 stream], [@(0.0.255) int] 
[*@3 version ][@(0.0.255) `=] [* Null])&]
[s2;%% Serialization save with some additional data to ensure data 
integrity. Data to serialize are represented by Event. If [%-*@3 version] 
is not Null, it is stored with data and must be specified with 
Load `- only the same version number will successfully be Loaded.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:Load`(Event`,Stream`&`,int`): [@(0.0.255) bool] [* Load](Event<Stream[@(0.0.255) `&
]> [*@3 serialize], Stream[@(0.0.255) `&] [*@3 stream], [@(0.0.255) int] 
[*@3 version ][@(0.0.255) `=] [* Null])&]
[s2;%% Smart serialization restore. Data to serialize is represented 
by Event. First, backup of current state of data is performed 
and stored in the memory. Then data is loaded from the specified 
stream. If restoring of data is successful (no LoadingError exception 
is thrown), Load returns true. If LoadingError is thrown, it 
is caught by Load, data are restored from backup copy and Load 
returns false. If [%-*@3 version ]is not Null, Load loads it from 
the stream (at appropriate unspecified point) and checks that 
the version is the same as used with Store.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:LoadFromFile`(Event`,const char`*`,int`): [@(0.0.255) bool] 
[* LoadFromFile](Event<Stream[@(0.0.255) `&]> [*@3 serialize], [@(0.0.255) const] 
[@(0.0.255) char] [@(0.0.255) `*][*@3 file] [@(0.0.255) `=] [@3 0], [@(0.0.255) int] 
[*@3 version ][@(0.0.255) `=] [* Null])&]
[s2;%% Using [%-*^topic`:`/`/Core`/src`/SerializationUtils`_en`-us`#Upp`:`:Load`(Event`,Stream`&`,int`)^ L
oad], restores data from the [%-*@3 file].&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StoreToFile`(Event`,const char`*`,int`): [@(0.0.255) bool] 
[* StoreToFile](Event<Stream[@(0.0.255) `&]> [*@3 serialize], [@(0.0.255) const] 
[@(0.0.255) char] [@(0.0.255) `*][*@3 file] [@(0.0.255) `=] [@3 0], [@(0.0.255) int] 
[*@3 version ][@(0.0.255) `=] [* Null])&]
[s2;%% Using [%-*^topic`:`/`/Core`/src`/SerializationUtils`_en`-us`#Upp`:`:Store`(Event`,Stream`&`,int`)^ S
tore], stores data to the [%-*@3 file].&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Load`(T`&`,Stream`&`,int`): [@(0.0.255) template] <[@(0.0.255) class] 
T> [@(0.0.255) bool] [* Load](T[@(0.0.255) `&] [*@3 x], Stream[@(0.0.255) `&] 
[*@3 s], [@(0.0.255) int] [*@3 version ][@(0.0.255) `=] [* Null])&]
[s2;%% Using [*^topic`:`/`/Core`/src`/SerializationUtils`_en`-us`#Upp`:`:Load`(Event`,Stream`&`,int`)^ L
oad ]and T`::Serialize, loads [%-*@3 x] from the stream[%-  ][%-*@3 s].&]
[s3; &]
[s4; &]
[s5;:Upp`:`:Store`(T`&`,Stream`&`,int`): [@(0.0.255) template] <[@(0.0.255) class] 
T> [@(0.0.255) bool] [* Store](T[@(0.0.255) `&] [*@3 x], Stream[@(0.0.255) `&] 
[*@3 s], [@(0.0.255) int] [*@3 version ][@(0.0.255) `=] [* Null])&]
[s2;%% Using [%-*^topic`:`/`/Core`/src`/SerializationUtils`_en`-us`#Upp`:`:Store`(Event`,Stream`&`,int`)^ S
tore ]and T`::Serialize, stores [%-*@3 x] to the stream.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:LoadFromFile`(T`&`,const char`*`,int`): [@(0.0.255) template] 
<[@(0.0.255) class] T> [@(0.0.255) bool] [* LoadFromFile](T[@(0.0.255) `&] 
[*@3 x], [@(0.0.255) const] [@(0.0.255) char] [@(0.0.255) `*][*@3 name] 
[@(0.0.255) `=] [@3 0], [@(0.0.255) int] [*@3 version ][@(0.0.255) `=] 
[* Null])&]
[s2;%% Using [*^topic`:`/`/Core`/src`/SerializationUtils`_en`-us`#Upp`:`:LoadFromFile`(Event`,const char`*`,int`)^ L
oadFromFile ]and T`::Serialize, loads [%-*@3 x] from the stream.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StoreToFile`(T`&`,const char`*`,int`): [@(0.0.255) template] 
<[@(0.0.255) class] T> [@(0.0.255) bool] [* StoreToFile](T[@(0.0.255) `&] 
[*@3 x], [@(0.0.255) const] [@(0.0.255) char] [@(0.0.255) `*][*@3 name] 
[@(0.0.255) `=] [@3 0], [@(0.0.255) int] [*@3 version ][@(0.0.255) `=] 
[* Null])&]
[s2;%% Using [*^topic`:`/`/Core`/src`/SerializationUtils`_en`-us`#Upp`:`:StoreToFile`(Event`,const char`*`,int`)^ L
oadFromFile ]and T`::Serialize, stores [%-*@3 x] to the stream.&]
[s3;%% &]
[s4;%% &]
[s5;:StoreAsString`(T`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T][@(0.0.255) >]_[_^String^ S
tring]_[* StoreAsString]([*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s2;%% Stores [@3 x] using its Serialize method and StringStream.&]
[s3; &]
[s4; &]
[s5;:LoadFromString`(T`&`,const String`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T
][@(0.0.255) >]_[@(0.0.255) bool]_[* LoadFromString]([*@4 T][@(0.0.255) `&]_[*@3 x], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s])&]
[s2;%% Restores serialized data from the String (e.g. previously 
stored by StoreAsString).&]
[s3;%% &]
[s0;%% &]
[s0;%% [*@3;4 Global modular serialization support]&]
[s0;#%% Modular serialization is a viable option for storing configuration 
of applications consisting of many modules. It allows individual 
storing/loading data for given global configuration key and also 
serialization of all such data with single stream.&]
[s0;3%% &]
[ {{10000t/25b/25@1 [s0; [* Function List]]}}&]
[s0;0 &]
[s5;:RegisterGlobalConfig`(const char`*`): [@(0.0.255) void]_[* RegisterGlobalConfig]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 name])&]
[s7;%% Registers name as global configuration key.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:RegisterGlobalSerialize`(const char`*`,Upp`:`:Event`<Upp`:`:Stream`&`>`): [@(0.0.255) v
oid]_[* RegisterGlobalSerialize]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 name], 
[_^Upp`:`:Event^ Event]<Stream[@(0.0.255) `&]>_[*@3 WhenSerialize])&]
[s2;%% Registers name as global configuration key. [%-*@3 WhenSerialize] 
is directly used to serialize data, unlike other variants, where 
data are stored / retrieved using LoadFromGlobal / StoreToGlobal.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:RegisterGlobalConfig`(const char`*`,Upp`:`:Event`<`>`): [@(0.0.255) void]_[* R
egisterGlobalConfig]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 name], 
[_^Upp`:`:Event^ Event]<>_[*@3 WhenFlush])&]
[s2;%% Registers name as global configuration key. [%-*C@3 WhenFlush] 
 is called before storing of all configuration keys is performed 
by SerializeGlobalConfigs `- this is useful when StoreToGlobal 
has to be explicitly triggered before storing configuration.&]
[s3;%% &]
[s4; &]
[s5;:LoadFromGlobal`(T`&`,const char`*`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T][@(0.0.255) >
]_[@(0.0.255) bool]_[* LoadFromGlobal]([*@4 T][@(0.0.255) `&]_[*@3 x], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 name])&]
[s2;%% Loads [%-*@3 x] from global configuration key [%-*@3 name], using 
Serialize method of T.&]
[s3;%% &]
[s4; &]
[s5;:StoreToGlobal`(T`&`,const char`*`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T][@(0.0.255) >
]_[@(0.0.255) void]_[* StoreToGlobal]([*@4 T][@(0.0.255) `&]_[*@3 x], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 name])&]
[s2;%% Stores [@3 x] to global configuration key [%-*@3 name], using 
Serialize method of T.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:LoadFromGlobal`(Upp`:`:Event`<Upp`:`:Stream`&`>`,const char`*`): [@(0.0.255) b
ool]_[* LoadFromGlobal]([_^Upp`:`:Event^ Event]<Stream[@(0.0.255) `&]>_[*@3 serialize], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 name])&]
[s2;%% Loads [%-*@3 x] from global configuration key [%-*@3 name], using 
[%-*@3 serialize] as serialization function.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:StoreToGlobal`(Upp`:`:Event`<Upp`:`:Stream`&`>`,const char`*`): [@(0.0.255) v
oid]_[* StoreToGlobal]([_^Upp`:`:Event^ Event]<Stream[@(0.0.255) `&]>_[*@3 x], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 name])&]
[s2;%% Stores [@3 x] to global configuration key [%-*@3 name], using 
[%-*@3 serialize] as serialization function.&]
[s3;%% &]
[s4; &]
[s5;:SerializeGlobalConfigs`(Stream`&`): [@(0.0.255) void]_[* SerializeGlobalConfigs]([_^Stream^ S
tream][@(0.0.255) `&]_[*@3 s])&]
[s2;%% Serializes all registered global configuration data from/to 
single stream.&]
[s3;%% &]
[s0;%% ]]