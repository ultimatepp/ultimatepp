topic "Serialization utilities";[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[l288;a17;*1 $$7,7#70004532496200323422659154056402:requirement]
[{_}%EN-US 
[s3; Serialization utilities&]
[s0;3 &]
[s5;K [%00-00^`:`:Stream^ Stream][%00-00 `&_][%00-00@0 Pack16][%00-00 (][%00-00^`:`:Stream^ Str
eam][%00-00 `&_][%00-00@3 s][%00-00 , ][%00-00@(0.0.255) int][%00-00 `&_][%00-00@3 i][%00-00 )]&]
[s2; Serializes 32`-bit data, optimizing it for 16`-bit values. If 
value is in `-32767`-32767 range, it is serialized as 2 bytes, 
otherwise 6 bytes are used.&]
[s4; [%00-00*C@3 s]-|Stream.&]
[s4; [%00-00*C@3 i]-|Data to serialize.&]
[s4; [*/ Return value]-|s for chaining.&]
[s0;3 &]
[s5;K [%00-00^`:`:Stream^ Stream][%00-00 `&_][%00-00@0 Pack16][%00-00 (][%00-00^`:`:Stream^ Str
eam][%00-00 `&_][%00-00@3 s][%00-00 , ][%00-00@(0.0.255) int][%00-00 `&_][%00-00@3 i1][%00-00 ,
 ][%00-00@(0.0.255) int][%00-00 `&_][%00-00@3 i2][%00-00 )]&]
[s5;K [%00-00^`:`:Stream^ Stream][%00-00 `&_][%00-00@0 Pack16][%00-00 (][%00-00^`:`:Stream^ Str
eam][%00-00 `&_][%00-00@3 s][%00-00 , ][%00-00@(0.0.255) int][%00-00 `&_][%00-00@3 i1][%00-00 ,
 ][%00-00@(0.0.255) int][%00-00 `&_][%00-00@3 i2][%00-00 , ][%00-00@(0.0.255) int][%00-00 `&_
][%00-00@3 i3][%00-00 )]&]
[s5;K [%00-00^`:`:Stream^ Stream][%00-00 `&_][%00-00@0 Pack16][%00-00 (][%00-00^`:`:Stream^ Str
eam][%00-00 `&_][%00-00@3 s][%00-00 , ][%00-00@(0.0.255) int][%00-00 `&_][%00-00@3 i1][%00-00 ,
 ][%00-00@(0.0.255) int][%00-00 `&_][%00-00@3 i2][%00-00 , ][%00-00@(0.0.255) int][%00-00 `&_
][%00-00@3 i3][%00-00 , ][%00-00@(0.0.255) int][%00-00 `&_][%00-00@3 i4][%00-00 )]&]
[s5;K [%00-00^`:`:Stream^ Stream][%00-00 `&_][%00-00@0 Pack16][%00-00 (][%00-00^`:`:Stream^ Str
eam][%00-00 `&_][%00-00@3 s][%00-00 , ][%00-00@(0.0.255) int][%00-00 `&_][%00-00@3 i1][%00-00 ,
 ][%00-00@(0.0.255) int][%00-00 `&_][%00-00@3 i2][%00-00 , ][%00-00@(0.0.255) int][%00-00 `&_
][%00-00@3 i3][%00-00 , ][%00-00@(0.0.255) int][%00-00 `&_][%00-00@3 i4][%00-00 , 
][%00-00@(0.0.255) int][%00-00 `&_][%00-00@3 i5][%00-00 )]&]
[s2; Calls Pack16 for each of individual 32`-bit integer parameters.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) bool][%00-00 _][%00-00@0 Load][%00-00 (][%00-00^`:`:Callback1^ Callback
1][%00-00 <][%00-00^`:`:Stream^ Stream][%00-00 `&>_][%00-00@3 serialize][%00-00 , 
][%00-00^`:`:Stream^ Stream][%00-00 `&_][%00-00@3 stream][%00-00 )]&]
[s2; Smart serialization restore. Data to serialize are represented 
by Callback. First, backup of current state of data is performed 
and stored in memory. Then data are loaded from the specified 
stream. If restoring of data is successful (no LoadingError exception 
is thrown), Load returns true. If LoadingError is thrown, it 
is caught by Load, data are restored from backup copy and Load 
returns false.&]
[s4; [%00-00*C@3 serialize]-|Callback to the serialization function.&]
[s4; [%00-00*C@3 stream]-|Stream.&]
[s4; [*/ Return value]-|true indicates success.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) bool][%00-00 _][%00-00@0 Store][%00-00 (][%00-00^`:`:Callback1^ Callbac
k1][%00-00 <][%00-00^`:`:Stream^ Stream][%00-00 `&>_][%00-00@3 serialize][%00-00 , 
][%00-00^`:`:Stream^ Stream][%00-00 `&_][%00-00@3 stream][%00-00 )]&]
[s2; Serialization save. Data to serialize are represented by Callback.&]
[s4; [%00-00*C@3 serialize]-|Callback to the serialization function.&]
[s4; [%00-00*C@3 stream]-|Stream.&]
[s4; [*/ Return value]-|true if data were properly stored to the stream.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) bool][%00-00 _][%00-00@0 LoadFromFile][%00-00 (][%00-00^`:`:Callback1^ C
allback1][%00-00 <Stream`&>_][%00-00@3 serialize][%00-00 , ][%00-00@(0.0.255) const][%00-00 _
][%00-00@(0.0.255) char][%00-00 _`*][%00-00@3 file][%00-00 _`=_NULL)]&]
[s2; Using [^topic`:`/`/Core`/src`/SerializationUtils`$en`-us`#`:`:Load`(Callback1`<Stream`&`>`,Stream`&`)^ L
oad], data are restored from the file.&]
[s4; [%00-00*C@3 serialize]-|Callback to the serialization function.&]
[s4; [%00-00*C@3 file]-|Filename.&]
[s4; [*/ Return value]-|true indicates success.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) bool][%00-00 _][%00-00@0 StoreToFile][%00-00 (][%00-00^`:`:Callback1^ C
allback1][%00-00 <Stream`&>_][%00-00@3 serialize][%00-00 , ][%00-00@(0.0.255) const][%00-00 _
][%00-00@(0.0.255) char][%00-00 _`*][%00-00@3 file][%00-00 _`=_NULL)]&]
[s2; Using [^topic`:`/`/Core`/src`/SerializationUtils`$en`-us`#`:`:Store`(Callback1`<Stream`&`>`,Stream`&`)^ S
tore] data are saved to the file.&]
[s4; [%00-00*C@3 serialize]-|Callback to the serialization function.&]
[s4; [%00-00*C@3 file]-|Filename.&]
[s4; [*/ Return value]-|true indicates success.&]
[s0;3 &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 T][%00-00 >__][%00-00@(0.0.255) b
ool][%00-00 _][%00-00@0 Load][%00-00 (][%00-00^T^ T][%00-00 `&_][%00-00@3 x][%00-00 , 
][%00-00^`:`:Stream^ Stream][%00-00 `&_][%00-00@3 s][%00-00 )]&]
[s2; Calls [^topic`:`/`/Core`/src`/SerializationUtils`$en`-us`#`:`:Load`(Callback1`<Stream`&`>`,Stream`&`)^ L
oad] with callback(`&[@3 x], `&T`::Serialize) as serialization callback. 
In other words, [@3 x] is restored using its Serialize method.&]
[s4; [*C@4 T]-|Type of object to restore.&]
[s4; [%00-00*C@3 x]-|Reference to the object.&]
[s4; [%00-00*C@3 s]-|Stream.&]
[s4; [*/ Return value]-|true indicates success.&]
[s0;3 &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 T][%00-00 >__][%00-00@(0.0.255) b
ool][%00-00 _][%00-00@0 Store][%00-00 (][%00-00^T^ T][%00-00 `&_][%00-00@3 x][%00-00 , 
][%00-00^`:`:Stream^ Stream][%00-00 `&_][%00-00@3 s][%00-00 )]&]
[s2; Calls [^topic`:`/`/Core`/src`/SerializationUtils`$en`-us`#`:`:Store`(T`&`,Stream`&`)^ S
tore] with callback(`&[@3 x], `&T`::Serialize) as serialization 
callback. In other words, [@3 x] is stored using its Serialize 
method.&]
[s4; [*C@4 T]-|Type of object to restore.&]
[s4; [%00-00*C@3 x]-|Reference to the object.&]
[s4; [%00-00*C@3 s]-|Stream.&]
[s4; [*/ Return value]-|true indicates success.&]
[s0;3 &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 T][%00-00 >__][%00-00@(0.0.255) b
ool][%00-00 _][%00-00@0 LoadFromFile][%00-00 (][%00-00^T^ T][%00-00 `&_][%00-00@3 x][%00-00 , 
][%00-00@(0.0.255) const][%00-00 _][%00-00@(0.0.255) char][%00-00 _`*][%00-00@3 name][%00-00 _
`=_NULL)]&]
[s2; Calls [^topic`:`/`/Core`/src`/SerializationUtils`$en`-us`#`:`:LoadFromFile`(Callback1`<Stream`&`>`,const char`*`)^ L
oadFromFile] with callback(`&[@3 x], `&T`::Serialize) as serialization 
callback. In other words, [@3 x] is restored from the file using 
its Serialize method.&]
[s4; [*C@4 T]-|Type of object to restore.&]
[s4; [%00-00*C@3 x]-|Reference to the object.&]
[s4; [%00-00*C@3 name]-|Filename.&]
[s4; [*/ Return value]-|true indicates success.&]
[s0;3 &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 T][%00-00 >__][%00-00@(0.0.255) b
ool][%00-00 _][%00-00@0 StoreToFile][%00-00 (][%00-00^T^ T][%00-00 `&_][%00-00@3 x][%00-00 , 
][%00-00@(0.0.255) const][%00-00 _][%00-00@(0.0.255) char][%00-00 _`*][%00-00@3 name][%00-00 _
`=_NULL)]&]
[s2; Calls [^topic`:`/`/Core`/src`/SerializationUtils`$en`-us`#`:`:StoreToFile`(Callback1`<Stream`&`>`,const char`*`)^ S
toreToFile] with callback(`&[@3 x], `&T`::Serialize) as serialization 
callback. In other words, [@3 x] is stored to the file using its 
Serialize method.&]
[s4; [*C@4 T]-|Type of object to store.&]
[s4; [%00-00*C@3 x]-|Reference to the object.&]
[s4; [%00-00*C@3 name]-|Filename.&]
[s4; [*/ Return value]-|true indicates success.&]
[s0;3 &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 T][%00-00 >__][%00-00^`:`:String^ S
tring][%00-00 _][%00-00@0 StoreAsString][%00-00 (][%00-00^T^ T][%00-00 `&_][%00-00@3 x][%00-00 )
]&]
[s2; Stores [@3 x] using its Serialize method and StringStream.&]
[s4; [*C@4 T]-|Type of object to store.&]
[s4; [%00-00*C@3 x]-|Reference to the object.&]
[s4; [*/ Return value]-|String containing serialized [@3 x].&]
[s0;3 &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 T][%00-00 >__][%00-00@(0.0.255) b
ool][%00-00 _][%00-00@0 LoadFromString][%00-00 (][%00-00^T^ T][%00-00 `&_][%00-00@3 x][%00-00 ,
 ][%00-00@(0.0.255) const][%00-00 _][%00-00^`:`:String^ String][%00-00 `&_][%00-00@3 s][%00-00 )
]&]
[s2; Restores serialized data from the String (e.g. previously stored 
by StoreAsString).&]
[s4; [*C@4 T]-|Type of object to restore.&]
[s4; [%00-00*C@3 x]-|Reference to object.&]
[s4; [%00-00*C@3 s]-|String containing serialized [@3 x].&]
[s4; [*/ Return value]-|true indicates success.&]
[s0; &]
[s0; &]
[s6; Global modular serialization support&]
[s0; Modular serialization is a viable option for storing configuration 
of applications consisting of many modules. It allows individual 
storing/loading data for given global configuration key and also 
serialization of all such data with single stream.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 RegisterGlobalConfig][%00-00 (][%00-00@(0.0.255) c
onst][%00-00 _][%00-00@(0.0.255) char][%00-00 _`*][%00-00@3 name][%00-00 )]&]
[s2; Registers name as global configuration key.&]
[s4; [%00-00*C@3 name]-|Key.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 RegisterGlobalConfig][%00-00 (][%00-00@(0.0.255) c
onst][%00-00 _][%00-00@(0.0.255) char][%00-00 _`*][%00-00@3 name][%00-00 , 
][%00-00^`:`:Callback^ Callback][%00-00 _][%00-00@3 WhenFlush][%00-00 )]&]
[s2; Registers name as global configuration key, with flush callback.&]
[s4; [%00-00*C@3 name]-|Key.&]
[s4; [%00-00*C@3 WhenFlush]-|This callback is called before storing 
of all configuration keys is performed by SerializeGlobalConfigs 
`- this is useful when StoreToGlobal has to be explicitly triggered 
before storing configuration.&]
[s0;3 &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 T][%00-00 >__][%00-00@(0.0.255) b
ool][%00-00 _][%00-00@0 LoadFromGlobal][%00-00 (][%00-00^T^ T][%00-00 `&_][%00-00@3 x][%00-00 ,
 ][%00-00@(0.0.255) const][%00-00 _][%00-00@(0.0.255) char][%00-00 _`*][%00-00@3 name][%00-00 )
]&]
[s2; Loads [@3 x] from global configuration key, using smart backup 
methods provided by [^topic`:`/`/Core`/src`/SerializationUtils`$en`-us`#`:`:Load`(Callback1`<Stream`&`>`,Stream`&`)^ L
oad].&]
[s4; [*C@4 T]-|Type of serialized object.&]
[s4; [%00-00*C@3 x]-|Reference to serialized object.&]
[s4; [%00-00*C@3 name]-|Configuration key.&]
[s4; [*/ Return value]-|true indicates success.&]
[s0;3 &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 T][%00-00 >__][%00-00@(0.0.255) v
oid][%00-00 _][%00-00@0 StoreToGlobal][%00-00 (][%00-00^T^ T][%00-00 `&_][%00-00@3 x][%00-00 , 
][%00-00@(0.0.255) const][%00-00 _][%00-00@(0.0.255) char][%00-00 _`*][%00-00@3 name][%00-00 )
]&]
[s2; Stores [@3 x] to global configuration key.&]
[s4; [*C@4 T]-|Type of serialized object.&]
[s4; [%00-00*C@3 x]-|Reference to serialized object.&]
[s4; [%00-00*C@3 name]-|Configuration key.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) void][%00-00 _][%00-00@0 SerializeGlobalConfigs][%00-00 (][%00-00^`:`:Stream^ S
tream][%00-00 `&_][%00-00@3 s][%00-00 )]&]
[s2; Serializes all global configuration data from/to single stream.&]
[s4; [%00-00*C@3 s]-|Stream.&]
[s0; ]