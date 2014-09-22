topic "Serialization utilities";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[{_}%EN-US 
[s0; [*@3;4 Serialization utilities]&]
[s0;3%- &]
[ {{10000t/25b/25@1 [s0;%- [* Function List]]}}&]
[s0;0%- &]
[s5;:Pack16`(Stream`&`,int`&`):%- [_^Stream^ Stream][@(0.0.255) `&]_[* Pack16]([_^Stream^ Str
eam][@(0.0.255) `&]_[*@3 s], [@(0.0.255) int`&]_[*@3 i])&]
[s2; Serializes 32`-bit data, optimizing it for 16`-bit values. If 
value is in `-32767`-32767 range, it is serialized as 2 bytes, 
otherwise 6 bytes are used.&]
[s7; [%-*C@3 s]-|Stream.&]
[s7; [%-*C@3 i]-|Data to serialize.&]
[s7; [*/ Return value]-|s for chaining.&]
[s3; &]
[s4;%- &]
[s5;:Pack16`(Stream`&`,int`&`,int`&`):%- [_^Stream^ Stream][@(0.0.255) `&]_[* Pack16]([_^Stream^ S
tream][@(0.0.255) `&]_[*@3 s], [@(0.0.255) int`&]_[*@3 i1], [@(0.0.255) int`&]_[*@3 i2])&]
[s5;:Pack16`(Stream`&`,int`&`,int`&`,int`&`):%- [_^Stream^ Stream][@(0.0.255) `&]_[* Pack16
]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 s], [@(0.0.255) int`&]_[*@3 i1], 
[@(0.0.255) int`&]_[*@3 i2], [@(0.0.255) int`&]_[*@3 i3])&]
[s5;:Pack16`(Stream`&`,int`&`,int`&`,int`&`,int`&`):%- [_^Stream^ Stream][@(0.0.255) `&]_
[* Pack16]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 s], [@(0.0.255) int`&]_[*@3 i1], 
[@(0.0.255) int`&]_[*@3 i2], [@(0.0.255) int`&]_[*@3 i3], [@(0.0.255) int`&]_[*@3 i4])&]
[s5;:Pack16`(Stream`&`,int`&`,int`&`,int`&`,int`&`,int`&`):%- [_^Stream^ Stream][@(0.0.255) `&
]_[* Pack16]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 s], [@(0.0.255) int`&]_[*@3 i1], 
[@(0.0.255) int`&]_[*@3 i2], [@(0.0.255) int`&]_[*@3 i3], [@(0.0.255) int`&]_[*@3 i4], 
[@(0.0.255) int`&]_[*@3 i5])&]
[s2; Calls Pack16 for each of individual 32`-bit integer parameters.&]
[s3; &]
[s4;%- &]
[s5;:Load`(Callback1`<Stream`&`>`,Stream`&`):%- [@(0.0.255) bool]_[* Load]([_^Callback1^ Ca
llback1][@(0.0.255) <][_^Stream^ Stream][@(0.0.255) `&>]_[*@3 serialize], 
[_^Stream^ Stream][@(0.0.255) `&]_[*@3 stream])&]
[s2; Smart serialization restore. Data to serialize are represented 
by Callback. First, backup of current state of data is performed 
and stored in memory. Then data are loaded from the specified 
stream. If restoring of data is successful (no LoadingError exception 
is thrown), Load returns true. If LoadingError is thrown, it 
is caught by Load, data are restored from backup copy and Load 
returns false.&]
[s7; [%-*C@3 serialize]-|Callback to the serialization function.&]
[s7; [%-*C@3 stream]-|Stream.&]
[s7; [*/ Return value]-|true indicates success.&]
[s3; &]
[s4;%- &]
[s5;:Store`(Callback1`<Stream`&`>`,Stream`&`):%- [@(0.0.255) bool]_[* Store]([_^Callback1^ C
allback1][@(0.0.255) <][_^Stream^ Stream][@(0.0.255) `&>]_[*@3 serialize], 
[_^Stream^ Stream][@(0.0.255) `&]_[*@3 stream])&]
[s2; Serialization save. Data to serialize are represented by Callback.&]
[s7; [%-*C@3 serialize]-|Callback to the serialization function.&]
[s7; [%-*C@3 stream]-|Stream.&]
[s7; [*/ Return value]-|true if data were properly stored to the stream.&]
[s3; &]
[s4;%- &]
[s5;:LoadFromFile`(Callback1`<Stream`&`>`,const char`*`):%- [@(0.0.255) bool]_[* LoadFrom
File]([_^Callback1^ Callback1][@(0.0.255) <]Stream[@(0.0.255) `&>]_[*@3 serialize], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 file]_`=_NULL)&]
[s2; Using [^topic`:`/`/Core`/src`/SerializationUtils`$en`-us`#`:`:Load`(Callback1`<Stream`&`>`,Stream`&`)^ L
oad], data are restored from the file.&]
[s7; [%-*C@3 serialize]-|Callback to the serialization function.&]
[s7; [%-*C@3 file]-|Filename.&]
[s7; [*/ Return value]-|true indicates success.&]
[s3; &]
[s4;%- &]
[s5;:StoreToFile`(Callback1`<Stream`&`>`,const char`*`):%- [@(0.0.255) bool]_[* StoreToFi
le]([_^Callback1^ Callback1][@(0.0.255) <]Stream[@(0.0.255) `&>]_[*@3 serialize], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 file]_`=_NULL)&]
[s2; Using [^topic`:`/`/Core`/src`/SerializationUtils`$en`-us`#`:`:Store`(Callback1`<Stream`&`>`,Stream`&`)^ S
tore] data are saved to the file.&]
[s7; [%-*C@3 serialize]-|Callback to the serialization function.&]
[s7; [%-*C@3 file]-|Filename.&]
[s7; [*/ Return value]-|true indicates success.&]
[s3; &]
[s4;%- &]
[s5;:Load`(T`&`,Stream`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T][@(0.0.255) >]_
[@(0.0.255) bool]_[* Load]([*@4 T][@(0.0.255) `&]_[*@3 x], [_^Stream^ Stream][@(0.0.255) `&]_[*@3 s
])&]
[s2; Calls [^topic`:`/`/Core`/src`/SerializationUtils`$en`-us`#`:`:Load`(Callback1`<Stream`&`>`,Stream`&`)^ L
oad] with callback(`&[@3 x], `&T`::Serialize) as serialization callback. 
In other words, [@3 x] is restored using its Serialize method.&]
[s7; [*C@4 T]-|Type of object to restore.&]
[s7; [%-*C@3 x]-|Reference to the object.&]
[s7; [%-*C@3 s]-|Stream.&]
[s7; [*/ Return value]-|true indicates success.&]
[s3; &]
[s4;%- &]
[s5;:Store`(T`&`,Stream`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T][@(0.0.255) >
]_[@(0.0.255) bool]_[* Store]([*@4 T][@(0.0.255) `&]_[*@3 x], [_^Stream^ Stream][@(0.0.255) `&]_
[*@3 s])&]
[s2; Calls [^topic`:`/`/Core`/src`/SerializationUtils`$en`-us`#`:`:Store`(T`&`,Stream`&`)^ S
tore] with callback(`&[@3 x], `&T`::Serialize) as serialization 
callback. In other words, [@3 x] is stored using its Serialize 
method.&]
[s7; [*C@4 T]-|Type of object to restore.&]
[s7; [%-*C@3 x]-|Reference to the object.&]
[s7; [%-*C@3 s]-|Stream.&]
[s7; [*/ Return value]-|true indicates success.&]
[s3; &]
[s4;%- &]
[s5;:LoadFromFile`(T`&`,const char`*`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T][@(0.0.255) >
]_[@(0.0.255) bool]_[* LoadFromFile]([*@4 T][@(0.0.255) `&]_[*@3 x], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 name]_`=_NULL)&]
[s2; Calls [^topic`:`/`/Core`/src`/SerializationUtils`$en`-us`#`:`:LoadFromFile`(Callback1`<Stream`&`>`,const char`*`)^ L
oadFromFile] with callback(`&[@3 x], `&T`::Serialize) as serialization 
callback. In other words, [@3 x] is restored from the file using 
its Serialize method.&]
[s7; [*C@4 T]-|Type of object to restore.&]
[s7; [%-*C@3 x]-|Reference to the object.&]
[s7; [%-*C@3 name]-|Filename.&]
[s7; [*/ Return value]-|true indicates success.&]
[s3; &]
[s4;%- &]
[s5;:StoreToFile`(T`&`,const char`*`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T][@(0.0.255) >
]_[@(0.0.255) bool]_[* StoreToFile]([*@4 T][@(0.0.255) `&]_[*@3 x], [@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 name]_`=_NULL)&]
[s2; Calls [^topic`:`/`/Core`/src`/SerializationUtils`$en`-us`#`:`:StoreToFile`(Callback1`<Stream`&`>`,const char`*`)^ S
toreToFile] with callback(`&[@3 x], `&T`::Serialize) as serialization 
callback. In other words, [@3 x] is stored to the file using its 
Serialize method.&]
[s7; [*C@4 T]-|Type of object to store.&]
[s7; [%-*C@3 x]-|Reference to the object.&]
[s7; [%-*C@3 name]-|Filename.&]
[s7; [*/ Return value]-|true indicates success.&]
[s3; &]
[s4;%- &]
[s5;:StoreAsString`(T`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T][@(0.0.255) >]_
[_^String^ String]_[* StoreAsString]([*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s2; Stores [@3 x] using its Serialize method and StringStream.&]
[s7; [*C@4 T]-|Type of object to store.&]
[s7; [%-*C@3 x]-|Reference to the object.&]
[s7; [*/ Return value]-|String containing serialized [@3 x].&]
[s3; &]
[s4;%- &]
[s5;:LoadFromString`(T`&`,const String`&`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T
][@(0.0.255) >]_[@(0.0.255) bool]_[* LoadFromString]([*@4 T][@(0.0.255) `&]_[*@3 x], 
[@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 s])&]
[s2; Restores serialized data from the String (e.g. previously stored 
by StoreAsString).&]
[s7; [*C@4 T]-|Type of object to restore.&]
[s7; [%-*C@3 x]-|Reference to object.&]
[s7; [%-*C@3 s]-|String containing serialized [@3 x].&]
[s7; [*/ Return value]-|true indicates success.&]
[s3; &]
[s0; &]
[s0; &]
[s0; &]
[s0; [*@3;4 Global modular serialization support]&]
[s0;# Modular serialization is a viable option for storing configuration 
of applications consisting of many modules. It allows individual 
storing/loading data for given global configuration key and also 
serialization of all such data with single stream.&]
[s0;3 &]
[ {{10000t/25b/25@1 [s0;%- [* Function List]]}}&]
[s0;0%- &]
[s5;:RegisterGlobalConfig`(const char`*`):%- [@(0.0.255) void]_[* RegisterGlobalConfig]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 name])&]
[s2; Registers name as global configuration key.&]
[s7; [%-*C@3 name]-|Key.&]
[s3; &]
[s4;%- &]
[s5;:RegisterGlobalConfig`(const char`*`,Callback`):%- [@(0.0.255) void]_[* RegisterGloba
lConfig]([@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 name], [_^Callback^ Callback]_[*@3 Whe
nFlush])&]
[s2; Registers name as global configuration key, with flush callback.&]
[s7; [%-*C@3 name]-|Key.&]
[s7; [%-*C@3 WhenFlush]-|This callback is called before storing of all 
configuration keys is performed by SerializeGlobalConfigs `- 
this is useful when StoreToGlobal has to be explicitly triggered 
before storing configuration.&]
[s3; &]
[s4;%- &]
[s5;:LoadFromGlobal`(T`&`,const char`*`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T
][@(0.0.255) >]_[@(0.0.255) bool]_[* LoadFromGlobal]([*@4 T][@(0.0.255) `&]_[*@3 x], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 name])&]
[s2; Loads [%-*@3 x] from global configuration key, using smart backup 
methods provided by [^topic`:`/`/Core`/src`/SerializationUtils`$en`-us`#`:`:Load`(Callback1`<Stream`&`>`,Stream`&`)^ L
oad].&]
[s7; [*C@4 T]-|Type of serialized object.&]
[s7; [%-*C@3 x]-|Reference to serialized object.&]
[s7; [%-*C@3 name]-|Configuration key.&]
[s7; [*/ Return value]-|true indicates success.&]
[s3; &]
[s4;%- &]
[s5;:StoreToGlobal`(T`&`,const char`*`):%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T
][@(0.0.255) >]_[@(0.0.255) void]_[* StoreToGlobal]([*@4 T][@(0.0.255) `&]_[*@3 x], 
[@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 name])&]
[s2; Stores [@3 x] to global configuration key.&]
[s7; [*C@4 T]-|Type of serialized object.&]
[s7; [%-*C@3 x]-|Reference to serialized object.&]
[s7; [%-*C@3 name]-|Configuration key.&]
[s3; &]
[s4;%- &]
[s5;:SerializeGlobalConfigs`(Stream`&`):%- [@(0.0.255) void]_[* SerializeGlobalConfigs]([_^Stream^ S
tream][@(0.0.255) `&]_[*@3 s])&]
[s2; Serializes all global configuration data from/to single stream.&]
[s7; [%-*C@3 s]-|Stream.&]
[s3; &]
[s0; ]]