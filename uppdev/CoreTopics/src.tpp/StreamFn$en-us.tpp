topic "Stream utilities";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a17;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[{_}%EN-US 
[s0; Stream utilities&]
[s0; &]
[s5;K%- [^`:`:String^ String]_LoadStream[@(64) (][^`:`:Stream^@(64) Stream][@(64) `&_][@3 in][@(64) )
]&]
[s2; Reads the stream starting with the current position till the 
end is reached and returns data in String.&]
[s7; [%-*C@3 in]-|Stream.&]
[s7; [*/ Return value]-|Content of stream.&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]SaveStream[@(64) (][^`:`:Stream^@(64) Stream][@(64) `&_][@3 out
][@(64) , ][@(0.0.255) const][@(64) _][^`:`:String^@(64) String][@(64) `&_][@3 data][@(64) )]&]
[s2; Writes data to stream.&]
[s7; [%-*C@3 out]-|Output stream.&]
[s7; [%-*C@3 data]-|Data to write.&]
[s7; [*/ Return value]-|True if all data were successfully written to 
the stream.&]
[s0; &]
[s5;K%- [^`:`:int64^ int64]_CopyStream[@(64) (][^`:`:Stream^@(64) Stream][@(64) `&_][@3 dest][@(64) ,
 ][^`:`:Stream^@(64) Stream][@(64) `&_][@3 src][@(64) , ][^`:`:int64^@(64) int64][@(64) _][@3 c
ount][@(64) )]&]
[s2; Copies data from source to destination stream.&]
[s7; [%-*C@3 dest]-|Destination stream.&]
[s7; [%-*C@3 src]-|Source stream.&]
[s7; [%-*C@3 count]-|Number of bytes to copy.&]
[s7; [*/ Return value]-|Number of bytes actually copies.&]
[s0; &]
[s5;K%- [^`:`:int64^ int64]_CopyStream[@(64) (][^`:`:Stream^@(64) Stream][@(64) `&_][@3 dest][@(64) ,
 ][^`:`:Stream^@(64) Stream][@(64) `&_][@3 src][@(64) )]&]
[s2; Copies all data from source stream starting with current position 
to the destination stream.&]
[s7; [%-*C@3 dest]-|Destination stream.&]
[s7; [%-*C@3 src]-|Source stream.&]
[s7; [*/ Return value]-|Number of bytes copied.&]
[s0; &]
[s5;K%- [^`:`:Stream^ Stream]`&_Cout[@(64) ()]&]
[s2; Returns special output stream representing console output. Data 
written to this stream are displayed as characters in console.&]
[s7; [*/ Return value]-|Console stream.&]
[s0; &]
[s5;K%- [^`:`:String^ String]_ReadStdIn[@(64) ()]&]
[s2; Reads one line of input data from the console.&]
[s7; [*/ Return value]-|Console input.&]
[s0; &]
[s5;K%- [^`:`:Stream^ Stream]`&_NilStream[@(64) ()]&]
[s2; Returns special stream that is always in IsEof state and simply 
discards all data written to it.&]
[s7; [*/ Return value]-|`"Black hole`" stream.&]
[s0; &]
[s5;K%- [^`:`:String^ String]_LoadFile[@(64) (][@(0.0.255) const][@(64) _][@(0.0.255) char][@(64) _
`*][@3 filename][@(64) )]&]
[s2; Loads the content of specified file.&]
[s7; [%-*C@3 filename]-|File name.&]
[s7; [*/ Return value]-|Content of file.&]
[s0; &]
[s5;K%- [@(0.0.255) bool][@(64) _]SaveFile[@(64) (][@(0.0.255) const][@(64) _][@(0.0.255) char][@(64) _
`*][@3 filename][@(64) , ][@(0.0.255) const][@(64) _][^`:`:String^@(64) String][@(64) `&_][@3 d
ata][@(64) )]&]
[s2; Saves data as the file (overwrites existing).&]
[s7; [%-*C@3 filename]-|File name.&]
[s7; [%-*C@3 data]-|Data to write.&]
[s7; [*/ Return value]-|true if file was successfully written.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 T][@(64) >__][^`:`:Stream^@(64) Stream][@(64) `&
_]operator%[@(64) (][^`:`:Stream^@(64) Stream][@(64) `&_][@3 s][@(64) , 
][^T^@(64) T][@(64) `&_][@3 x][@(64) )]&]
[s2; Serialization operator. Simply invokes x.Serialize(s);&]
[s7; [*C@4 T]-|Type of variable to be serialized.&]
[s7; [%-*C@3 s]-|Stream.&]
[s7; [%-*C@3 x]-|Variable to be serialized.&]
[s7; [*/ Return value]-|s for chaining.&]
[s0; &]
[s5;K%- [^`:`:Stream^ Stream]`&_operator<<[@(64) (][^`:`:Stream^@(64) Stream][@(64) `&_][@3 s][@(64) ,
 ][@(0.0.255) const][@(64) _][@(0.0.255) char][@(64) _`*][@3 x][@(64) )]&]
[s2; Overload of stream insertion operator to get simple case work. 
Insertion operator uses formatted stream output (Putf).&]
[s7; [%-*C@3 s]-|Output stream.&]
[s7; [%-*C@3 x]-|Data to write `- all characters are written to the 
stream.&]
[s7; [*/ Return value]-|s for chaining.&]
[s0; &]
[s5;K%- [^`:`:Stream^ Stream]`&_operator<<[@(64) (][^`:`:Stream^@(64) Stream][@(64) `&_][@3 s][@(64) ,
 ][@(0.0.255) char][@(64) _`*][@3 x][@(64) )]&]
[s2; Overload of stream insertion operator to get simple case work. 
Insertion operator uses formatted stream output (Putf).&]
[s7; [%-*C@3 s]-|Output stream.&]
[s7; [%-*C@3 x]-|Data to write `- all characters are written to the 
stream.&]
[s7; [*/ Return value]-|s for chaining.&]
[s0; &]
[s5;K%- [^`:`:Stream^ Stream]`&_operator<<[@(64) (][^`:`:Stream^@(64) Stream][@(64) `&_][@3 s][@(64) ,
 ][@(0.0.255) const][@(64) _][^`:`:String^@(64) String][@(64) _`&][@3 x][@(64) )]&]
[s2; Overload of stream insertion operator to get simple case work. 
Insertion operator uses formatted stream output (Putf).&]
[s7; [%-*C@3 s]-|Output stream.&]
[s7; [%-*C@3 x]-|Data to write `- all characters are written to the 
stream.&]
[s7; [*/ Return value]-|s for chaining.&]
[s0; &]
[s5;K%- [^`:`:Stream^ Stream]`&_operator<<[@(64) (][^`:`:Stream^@(64) Stream][@(64) `&_][@3 s][@(64) ,
 ][@(0.0.255) char][@(64) _][@3 x][@(64) )]&]
[s2; Overload of stream insertion operator to get simple case work. 
Insertion operator uses formatted stream output (Putf).&]
[s7; [%-*C@3 s]-|Output stream.&]
[s7; [%-*C@3 x]-|Data to write `- it is written as single character.&]
[s7; [*/ Return value]-|s for chaining.&]
[s0; &]
[s5;K%- [^`:`:Stream^ Stream]`&_operator<<[@(64) (][^`:`:Stream^@(64) Stream][@(64) `&_][@3 s][@(64) ,
 ][@(0.0.255) const][@(64) _][@(0.0.255) void][@(64) _`*][@3 x][@(64) )]&]
[s2; Overload of stream insertion operator to get simple case work. 
Insertion operator uses formatted stream output (Putf).&]
[s7; [%-*C@3 s]-|Output stream.&]
[s7; [%-*C@3 x]-|Data to write `- pointer is formatted as hexadecimal 
value.&]
[s7; [*/ Return value]-|s for chaining.&]
[s0; &]
[s5;K%- [^`:`:Stream^ Stream]`&_operator<<[@(64) (][^`:`:Stream^@(64) Stream][@(64) `&_][@3 s][@(64) ,
 ][@(0.0.255) void][@(64) _`*][@3 x][@(64) )]&]
[s2; Overload of stream insertion operator to get simple case work. 
Insertion operator uses formatted stream output (Putf).&]
[s7; [%-*C@3 s]-|Output stream.&]
[s7; [%-*C@3 x]-|Data to write `- pointer is formatted as hexadecimal 
value.&]
[s7; [*/ Return value]-|s for chaining.&]
[s0; &]
[s5;K%- template_<[@(0.0.255) class][@(64) _][@4 T][@(64) >__][^`:`:Stream^@(64) Stream][@(64) `&
_]operator<<[@(64) (][^`:`:Stream^@(64) Stream][@(64) `&_][@3 s][@(64) , 
][@(0.0.255) const][@(64) _][^T^@(64) T][@(64) `&_][@3 x][@(64) )]&]
[s2; Global stream insertion operator. Calls AsString for x and stores 
the result to the stream. Global AsString version in turn calls 
ToString method of x.&]
[s7; [*C@4 T]-|Type of data to write to the stream.&]
[s7; [%-*C@3 s]-|Output stream.&]
[s7; [%-*C@3 x]-|Data to write.&]
[s0; [*/ Return value]-|s for chaining.]