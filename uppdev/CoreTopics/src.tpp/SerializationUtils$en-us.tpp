topic "Serialization utilities";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a17;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[{_}%EN-US 
[s0; Serialization utilities&]
[s0; &]
[s5;K%- [^`:`:Stream^ Stream]`&_Pack16[@(64) (][^`:`:Stream^@(64) Stream][@(64) `&_][@3 s][@(64) ,
 ][@(0.0.255) int][@(64) `&_][@3 i][@(64) )]&]
[s2; Serializes 32`-bit data, optimizing it for 16`-bit values. If 
value is in `-32767`-32767 range, it is serialized as 2 bytes, 
otherwise 6 bytes are used.&]
[s7; [%-*C@3 s]-|Stream.&]
[s7; [%-*C@3 i]-|Data to serialize.&]
[s7; [*/ Return value]-|s for chaining.&]
[s0; &]
[s5;K%- [^`:`:Stream^ Stream]`&_Pack16[@(64) (][^`:`:Stream^@(64) Stream][@(64) `&_][@3 s][@(64) ,
 ][@(0.0.255) int][@(64) `&_][@3 i1][@(64) , ][@(0.0.255) int][@(64) `&_][@3 i2][@(64) )]&]
[s5;K%- [^`:`:Stream^ Stream]`&_Pack16[@(64) (][^`:`:Stream^@(64) Stream][@(64) `&_][@3 s][@(64) ,
 ][@(0.0.255) int][@(64) `&_][@3 i1][@(64) , ][@(0.0.255) int][@(64) `&_][@3 i2][@(64) , 
][@(0.0.255) int][@(64) `&_][@3 i3][@(64) )]&]
[s5;K%- [^`:`:Stream^ Stream]`&_Pack16[@(64) (][^`:`:Stream^@(64) Stream][@(64) `&_][@3 s][@(64) ,
 ][@(0.0.255) int][@(64) `&_][@3 i1][@(64) , ][@(0.0.255) int][@(64) `&_][@3 i2][@(64) , 
][@(0.0.255) int][@(64) `&_][@3 i3][@(64) , ][@(0.0.255) int][@(64) `&_][@3 i4][@(64) )]&]
[s5;K%- [^`:`:Stream^ Stream]`&_Pack16[@(64) (][^`:`:Stream^@(64) Stream][@(64) `&_][@3 s][@(64) ,
 ][@(0.0.255) int][@(64) `&_][@3 i1][@(64) , ][@(0.0.255) int][@(64) `&_][@3 i2][@(64) , 
][@(0.0.255) int][@(64) `&_][@3 i3][@(64) , ][@(0.0.255) int][@(64) `&_][@3 i4][@(64) , 
][@(0.0.255) int][@(64) `&_][@3 i5][@(64) )]&]
[s2; Calls Pack16 for each of individual 32`-bit integer parameters.&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]Load[@(64) (][^`:`:Callback1^@(64) Callback1][@(64) <][^`:`:Stream^@(64) S
tream][@(64) `&>_][@3 serialize][@(64) , ][^`:`:Stream^@(64) Stream][@(64) `&_][@3 stream][@(64) )
]&]
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
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]Store[@(64) (][^`:`:Callback1^@(64) Callback1][@(64) <][^`:`:Stream^@(64) S
tream][@(64) `&>_][@3 serialize][@(64) , ][^`:`:Stream^@(64) Stream][@(64) `&_][@3 stream][@(64) )
]&]
[s2; Serialization save. Data to serialize are represented by Callback.&]
[s7; [%-*C@3 serialize]-|Callback to the serialization function.&]
[s7; [%-*C@3 stream]-|Stream.&]
[s7; [*/ Return value]-|true if data were properly stored to the stream.&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]LoadFromFile[@(64) (][^`:`:Callback1^@(64) Callback1][@(64) <
Stream`&>_][@3 serialize][@(64) , ][@(0.0.255) const][@(64) _][@(0.0.255) char][@(64) _`*][@3 f
ile][@(64) _`=_NULL)]&]
[s2; Using [^topic`:`/`/Core`/src`/SerializationUtils`$en`-us`#`:`:Load`(Callback1`<Stream`&`>`,Stream`&`)^ L
oad], data are restored from the file.&]
[s7; [%-*C@3 serialize]-|Callback to the serialization function.&]
[s7; [%-*C@3 file]-|Filename.&]
[s7; [*/ Return value]-|true indicates success.&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]StoreToFile[@(64) (][^`:`:Callback1^@(64) Callback1][@(64) <S
tream`&>_][@3 serialize][@(64) , ][@(0.0.255) const][@(64) _][@(0.0.255) char][@(64) _`*][@3 fi
le][@(64) _`=_NULL)]&]
[s2; Using [^topic`:`/`/Core`/src`/SerializationUtils`$en`-us`#`:`:Store`(Callback1`<Stream`&`>`,Stream`&`)^ S
tore] data are saved to the file.&]
[s7; [%-*C@3 serialize]-|Callback to the serialization function.&]
[s7; [%-*C@3 file]-|Filename.&]
[s7; [*/ Return value]-|true indicates success.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 T][@(64) >__][@(0.0.255) bool][@(64) _]Load[@(64) (
][^T^@(64) T][@(64) `&_][@3 x][@(64) , ][^`:`:Stream^@(64) Stream][@(64) `&_][@3 s][@(64) )]&]
[s2; Calls [^topic`:`/`/Core`/src`/SerializationUtils`$en`-us`#`:`:Load`(Callback1`<Stream`&`>`,Stream`&`)^ L
oad] with callback(`&[@3 x], `&T`::Serialize) as serialization callback. 
In other words, [@3 x] is restored using its Serialize method.&]
[s7; [*C@4 T]-|Type of object to restore.&]
[s7; [%-*C@3 x]-|Reference to the object.&]
[s7; [%-*C@3 s]-|Stream.&]
[s7; [*/ Return value]-|true indicates success.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 T][@(64) >__][@(0.0.255) bool][@(64) _]Store[@(64) (
][^T^@(64) T][@(64) `&_][@3 x][@(64) , ][^`:`:Stream^@(64) Stream][@(64) `&_][@3 s][@(64) )]&]
[s2; Calls [^topic`:`/`/Core`/src`/SerializationUtils`$en`-us`#`:`:Store`(T`&`,Stream`&`)^ S
tore] with callback(`&[@3 x], `&T`::Serialize) as serialization 
callback. In other words, [@3 x] is stored using its Serialize 
method.&]
[s7; [*C@4 T]-|Type of object to restore.&]
[s7; [%-*C@3 x]-|Reference to the object.&]
[s7; [%-*C@3 s]-|Stream.&]
[s7; [*/ Return value]-|true indicates success.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 T][@(64) >__][@(0.0.255) bool][@(64) _]LoadFromF
ile[@(64) (][^T^@(64) T][@(64) `&_][@3 x][@(64) , ][@(0.0.255) const][@(64) _][@(0.0.255) char][@(64) _
`*][@3 name][@(64) _`=_NULL)]&]
[s2; Calls [^topic`:`/`/Core`/src`/SerializationUtils`$en`-us`#`:`:LoadFromFile`(Callback1`<Stream`&`>`,const char`*`)^ L
oadFromFile] with callback(`&[@3 x], `&T`::Serialize) as serialization 
callback. In other words, [@3 x] is restored from the file using 
its Serialize method.&]
[s7; [*C@4 T]-|Type of object to restore.&]
[s7; [%-*C@3 x]-|Reference to the object.&]
[s7; [%-*C@3 name]-|Filename.&]
[s7; [*/ Return value]-|true indicates success.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 T][@(64) >__][@(0.0.255) bool][@(64) _]StoreToFi
le[@(64) (][^T^@(64) T][@(64) `&_][@3 x][@(64) , ][@(0.0.255) const][@(64) _][@(0.0.255) char][@(64) _
`*][@3 name][@(64) _`=_NULL)]&]
[s2; Calls [^topic`:`/`/Core`/src`/SerializationUtils`$en`-us`#`:`:StoreToFile`(Callback1`<Stream`&`>`,const char`*`)^ S
toreToFile] with callback(`&[@3 x], `&T`::Serialize) as serialization 
callback. In other words, [@3 x] is stored to the file using its 
Serialize method.&]
[s7; [*C@4 T]-|Type of object to store.&]
[s7; [%-*C@3 x]-|Reference to the object.&]
[s7; [%-*C@3 name]-|Filename.&]
[s7; [*/ Return value]-|true indicates success.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 T][@(64) >__][^`:`:String^@(64) String][@(64) _]S
toreAsString[@(64) (][^T^@(64) T][@(64) `&_][@3 x][@(64) )]&]
[s2; Stores [@3 x] using its Serialize method and StringStream.&]
[s7; [*C@4 T]-|Type of object to store.&]
[s7; [%-*C@3 x]-|Reference to the object.&]
[s7; [*/ Return value]-|String containing serialized [@3 x].&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 T][@(64) >__][@(0.0.255) bool][@(64) _]LoadFromS
tring[@(64) (][^T^@(64) T][@(64) `&_][@3 x][@(64) , ][@(0.0.255) const][@(64) _][^`:`:String^@(64) S
tring][@(64) `&_][@3 s][@(64) )]&]
[s2; Restores serialized data from the String (e.g. previously stored 
by StoreAsString).&]
[s7; [*C@4 T]-|Type of object to restore.&]
[s7; [%-*C@3 x]-|Reference to object.&]
[s7; [%-*C@3 s]-|String containing serialized [@3 x].&]
[s7; [*/ Return value]-|true indicates success.&]
[s0; &]
[s0; &]
[s0; Global modular serialization support&]
[s0; Modular serialization is a viable option for storing configuration 
of applications consisting of many modules. It allows individual 
storing/loading data for given global configuration key and also 
serialization of all such data with single stream.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]RegisterGlobalConfig[@(64) (][@(0.0.255) const][@(64) _][@(0.0.255) c
har][@(64) _`*][@3 name][@(64) )]&]
[s2; Registers name as global configuration key.&]
[s7; [%-*C@3 name]-|Key.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]RegisterGlobalConfig[@(64) (][@(0.0.255) const][@(64) _][@(0.0.255) c
har][@(64) _`*][@3 name][@(64) , ][^`:`:Callback^@(64) Callback][@(64) _][@3 WhenFlush][@(64) )
]&]
[s2; Registers name as global configuration key, with flush callback.&]
[s7; [%-*C@3 name]-|Key.&]
[s7; [%-*C@3 WhenFlush]-|This callback is called before storing of all 
configuration keys is performed by SerializeGlobalConfigs `- 
this is useful when StoreToGlobal has to be explicitly triggered 
before storing configuration.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 T][@(64) >__][@(0.0.255) bool][@(64) _]LoadFromG
lobal[@(64) (][^T^@(64) T][@(64) `&_][@3 x][@(64) , ][@(0.0.255) const][@(64) _][@(0.0.255) char][@(64) _
`*][@3 name][@(64) )]&]
[s2; Loads [@3 x] from global configuration key, using smart backup 
methods provided by [^topic`:`/`/Core`/src`/SerializationUtils`$en`-us`#`:`:Load`(Callback1`<Stream`&`>`,Stream`&`)^ L
oad].&]
[s7; [*C@4 T]-|Type of serialized object.&]
[s7; [%-*C@3 x]-|Reference to serialized object.&]
[s7; [%-*C@3 name]-|Configuration key.&]
[s7; [*/ Return value]-|true indicates success.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 T][@(64) >__][@(0.0.255) void][@(64) _]StoreToGl
obal[@(64) (][^T^@(64) T][@(64) `&_][@3 x][@(64) , ][@(0.0.255) const][@(64) _][@(0.0.255) char][@(64) _
`*][@3 name][@(64) )]&]
[s2; Stores [@3 x] to global configuration key.&]
[s7; [*C@4 T]-|Type of serialized object.&]
[s7; [%-*C@3 x]-|Reference to serialized object.&]
[s7; [%-*C@3 name]-|Configuration key.&]
[s0; &]
[s5;K%- [@(0.0.255) void][@(64) _]SerializeGlobalConfigs[@(64) (][^`:`:Stream^@(64) Stream][@(64) `&
_][@3 s][@(64) )]&]
[s2; Serializes all global configuration data from/to single stream.&]
[s7; [%-*C@3 s]-|Stream.&]
[s0; ]