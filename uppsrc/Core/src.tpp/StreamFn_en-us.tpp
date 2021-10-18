topic "Stream utilities";
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
[ {{10000@(113.42.0) [s0;%% [*@2;4 Stream helpers]]}}&]
[s0;%% &]
[ {{10000t/25b/25@1 [s0; [* Function List]]}}&]
[s3; &]
[s5;:LoadStream`(Stream`&`): [_^String^ String]_[* LoadStream]([_^Stream^ Stream][@(0.0.255) `&
]_[*@3 in])&]
[s2;%% Reads the stream starting with the current position till the 
end is reached and returns data in String.&]
[s7;%% [%-*C@3 in]-|Stream.&]
[s7;%% [*/ Return value]-|Content of stream.&]
[s3; &]
[s4; &]
[s5;:SaveStream`(Stream`&`,const String`&`): [@(0.0.255) bool]_[* SaveStream]([_^Stream^ St
ream][@(0.0.255) `&]_[*@3 out], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&]_[*@3 dat
a])&]
[s2;%% Writes data to stream.&]
[s7;%% [%-*C@3 out]-|Output stream.&]
[s7;%% [%-*C@3 data]-|Data to write.&]
[s7;%% [*/ Return value]-|True if all data were successfully written 
to the stream.&]
[s3; &]
[s4; &]
[s5;:CopyStream`(Stream`&`,Stream`&`,int64`): [_^int64^ int64]_[* CopyStream]([_^Stream^ St
ream][@(0.0.255) `&]_[*@3 dest], [_^Stream^ Stream][@(0.0.255) `&]_[*@3 src], 
[_^int64^ int64]_[*@3 count] `= INT64`_MAX)&]
[s2;%% Copies at most [%-*@3 count] bytes from source to destination 
stream. Returns the actual number of bytes copied. With default 
[%-*@3 count] value it copies all data from [%-*@3 src] until EOF.&]
[s3; &]
[s4; &]
[s5;:Cout`(`): [_^Stream^ Stream][@(0.0.255) `&]_[* Cout]()&]
[s2;%% Returns special output stream representing console output. 
Data written to this stream are displayed as characters in console.&]
[s3; &]
[s4; &]
[s5;:Cerr`(`): [_^Stream^ Stream][@(0.0.255) `&]_[* Cerr]()&]
[s2;%% Returns special output stream representing console error output. 
Data written to this stream are displayed as characters in console.&]
[s3; &]
[s4; &]
[s5;:ReadStdIn`(`): [_^String^ String]_[* ReadStdIn]()&]
[s2;%% Reads one line of input data from the console.&]
[s7;%% [*/ Return value]-|Console input.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:ReadSecret`(`): [_^Upp`:`:String^ String]_[* ReadSecret]()&]
[s2;%% Reads one line of input data from the console without echoing. 
This function is useful for reading passwords, secret phrases, 
etc. from the console.&]
[s7;%% [*/ Return value]-|Console input.&]
[s3; &]
[s4; &]
[s5;:NilStream`(`): [_^Stream^ Stream][@(0.0.255) `&]_[* NilStream]()&]
[s2;%% Returns special stream that is always in IsEof state and simply 
discards all data written to it.&]
[s7;%% [*/ Return value]-|`"Black hole`" stream.&]
[s3; &]
[s4; &]
[s5;:LoadFile`(const char`*`): [_^String^ String]_[* LoadFile]([@(0.0.255) const]_[@(0.0.255) c
har]_`*[*@3 filename])&]
[s2;%% Loads the content of specified file.&]
[s7;%% [%-*C@3 filename]-|File name.&]
[s7;%% [*/ Return value]-|Content of file.&]
[s3; &]
[s4; &]
[s5;:SaveFile`(const char`*`,const String`&`): [@(0.0.255) bool]_[* SaveFile]([@(0.0.255) c
onst]_[@(0.0.255) char]_`*[*@3 filename], [@(0.0.255) const]_[_^String^ String][@(0.0.255) `&
]_[*@3 data])&]
[s2;%% Saves data as the file (overwrites existing).&]
[s7;%% [%-*C@3 filename]-|File name.&]
[s7;%% [%-*C@3 data]-|Data to write.&]
[s7;%% [*/ Return value]-|true if file was successfully written.&]
[s3; &]
[s4; &]
[s5;:operator`%`(Stream`&`,T`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T][@(0.0.255) >
]_[_^Stream^ Stream][@(0.0.255) `&]_[* operator%]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 s], 
[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s2;%% Serialization operator. Simply invokes x.Serialize(s);&]
[s7;%% [*C@4 T]-|Type of variable to be serialized.&]
[s7;%% [%-*C@3 s]-|Stream.&]
[s7;%% [%-*C@3 x]-|Variable to be serialized.&]
[s7;%% [*/ Return value]-|s for chaining.&]
[s3; &]
[s4; &]
[s5;:operator`<`<`(Stream`&`,const char`*`): [_^Stream^ Stream][@(0.0.255) `&]_[* operator<
<]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 s], [@(0.0.255) const]_[@(0.0.255) char]_`*[*@3 x])
&]
[s2;%% Overload of stream insertion operator to get simple case work. 
Insertion operator uses formatted stream output (Putf).&]
[s7;%% [%-*C@3 s]-|Output stream.&]
[s7;%% [%-*C@3 x]-|Data to write `- all characters are written to the 
stream.&]
[s7;%% [*/ Return value]-|s for chaining.&]
[s3; &]
[s4; &]
[s5;:operator`<`<`(Stream`&`,char`*`): [_^Stream^ Stream][@(0.0.255) `&]_[* operator<<]([_^Stream^ S
tream][@(0.0.255) `&]_[*@3 s], [@(0.0.255) char]_`*[*@3 x])&]
[s2;%% Overload of stream insertion operator to get simple case work. 
Insertion operator uses formatted stream output (Putf).&]
[s7;%% [%-*C@3 s]-|Output stream.&]
[s7;%% [%-*C@3 x]-|Data to write `- all characters are written to the 
stream.&]
[s7;%% [*/ Return value]-|s for chaining.&]
[s3; &]
[s4; &]
[s5;:operator`<`<`(Stream`&`,const String`&`): [_^Stream^ Stream][@(0.0.255) `&]_[* operato
r<<]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 s], [@(0.0.255) const]_[_^String^ String]_`&[*@3 x
])&]
[s2;%% Overload of stream insertion operator to get simple case work. 
Insertion operator uses formatted stream output (Putf).&]
[s7;%% [%-*C@3 s]-|Output stream.&]
[s7;%% [%-*C@3 x]-|Data to write `- all characters are written to the 
stream.&]
[s7;%% [*/ Return value]-|s for chaining.&]
[s3; &]
[s4; &]
[s5;:operator`<`<`(Stream`&`,char`): [_^Stream^ Stream][@(0.0.255) `&]_[* operator<<]([_^Stream^ S
tream][@(0.0.255) `&]_[*@3 s], [@(0.0.255) char]_[*@3 x])&]
[s2;%% Overload of stream insertion operator to get simple case work. 
Insertion operator uses formatted stream output (Putf).&]
[s7;%% [%-*C@3 s]-|Output stream.&]
[s7;%% [%-*C@3 x]-|Data to write `- it is written as single character.&]
[s7;%% [*/ Return value]-|s for chaining.&]
[s3; &]
[s4; &]
[s5;:operator`<`<`(Stream`&`,const void`*`): [_^Stream^ Stream][@(0.0.255) `&]_[* operator<
<]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 s], [@(0.0.255) const]_[@(0.0.255) void]_`*[*@3 x])
&]
[s2;%% Overload of stream insertion operator to get simple case work. 
Insertion operator uses formatted stream output (Putf).&]
[s7;%% [%-*C@3 s]-|Output stream.&]
[s7;%% [%-*C@3 x]-|Data to write `- pointer is formatted as hexadecimal 
value.&]
[s7;%% [*/ Return value]-|s for chaining.&]
[s3; &]
[s4; &]
[s5;:operator`<`<`(Stream`&`,void`*`): [_^Stream^ Stream][@(0.0.255) `&]_[* operator<<]([_^Stream^ S
tream][@(0.0.255) `&]_[*@3 s], [@(0.0.255) void]_`*[*@3 x])&]
[s2;%% Overload of stream insertion operator to get simple case work. 
Insertion operator uses formatted stream output (Putf).&]
[s7;%% [%-*C@3 s]-|Output stream.&]
[s7;%% [%-*C@3 x]-|Data to write `- pointer is formatted as hexadecimal 
value.&]
[s7;%% [*/ Return value]-|s for chaining.&]
[s3; &]
[s4; &]
[s5;:operator`<`<`(Stream`&`,const T`&`): [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T][@(0.0.255) >
]_[_^Stream^ Stream][@(0.0.255) `&]_[* operator<<]([_^Stream^ Stream][@(0.0.255) `&]_[*@3 s],
 [@(0.0.255) const]_[*@4 T][@(0.0.255) `&]_[*@3 x])&]
[s2;%% Global stream insertion operator. Calls AsString for x and 
stores the result to the stream. Global AsString version in turn 
calls ToString method of x.&]
[s7;%% [*C@4 T]-|Type of data to write to the stream.&]
[s7;%% [%-*C@3 s]-|Output stream.&]
[s7;%% [%-*C@3 x]-|Data to write.&]
[s7;%% [*/ Return value]-|s for chaining.&]
[s3;%% &]
[s0;%% ]]