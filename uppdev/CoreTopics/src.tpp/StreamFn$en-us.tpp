topic "Stream utilities";[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;*@(64)2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[a83;*R6 $$3,0#31310162474203024125188417583966:caption]
[l288;i1121;b17;O9;~~~.1408;2 $$4,0#10431211400427159095818037425705:param]
[i448;a25;kKO9;*@(64)2 $$5,0#37138531426314131252341829483370:item]
[*+117 $$6,6#14700283458701402223321329925657:header]
[l288;a17;*1 $$7,7#70004532496200323422659154056402:requirement]
[{_}%EN-US 
[s3; Stream utilities&]
[s0;3 &]
[s5;K [%00-00^`:`:String^ String][%00-00 _][%00-00@0 LoadStream][%00-00 (][%00-00^`:`:Stream^ S
tream][%00-00 `&_][%00-00@3 in][%00-00 )]&]
[s2; Reads the stream starting with the current position till the 
end is reached and returns data in String.&]
[s4; [%00-00*C@3 in]-|Stream.&]
[s4; [*/ Return value]-|Content of stream.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) bool][%00-00 _][%00-00@0 SaveStream][%00-00 (][%00-00^`:`:Stream^ Strea
m][%00-00 `&_][%00-00@3 out][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00^`:`:String^ S
tring][%00-00 `&_][%00-00@3 data][%00-00 )]&]
[s2; Writes data to stream.&]
[s4; [%00-00*C@3 out]-|Output stream.&]
[s4; [%00-00*C@3 data]-|Data to write.&]
[s4; [*/ Return value]-|True if all data were successfully written to 
the stream.&]
[s0;3 &]
[s5;K [%00-00^`:`:int64^ int64][%00-00 _][%00-00@0 CopyStream][%00-00 (][%00-00^`:`:Stream^ Str
eam][%00-00 `&_][%00-00@3 dest][%00-00 , ][%00-00^`:`:Stream^ Stream][%00-00 `&_][%00-00@3 sr
c][%00-00 , ][%00-00^`:`:int64^ int64][%00-00 _][%00-00@3 count][%00-00 )]&]
[s2; Copies data from source to destination stream.&]
[s4; [%00-00*C@3 dest]-|Destination stream.&]
[s4; [%00-00*C@3 src]-|Source stream.&]
[s4; [%00-00*C@3 count]-|Number of bytes to copy.&]
[s4; [*/ Return value]-|Number of bytes actually copies.&]
[s0;3 &]
[s5;K [%00-00^`:`:int64^ int64][%00-00 _][%00-00@0 CopyStream][%00-00 (][%00-00^`:`:Stream^ Str
eam][%00-00 `&_][%00-00@3 dest][%00-00 , ][%00-00^`:`:Stream^ Stream][%00-00 `&_][%00-00@3 sr
c][%00-00 )]&]
[s2; Copies all data from source stream starting with current position 
to the destination stream.&]
[s4; [%00-00*C@3 dest]-|Destination stream.&]
[s4; [%00-00*C@3 src]-|Source stream.&]
[s4; [*/ Return value]-|Number of bytes copied.&]
[s0;3 &]
[s5;K [%00-00^`:`:Stream^ Stream][%00-00 `&_][%00-00@0 Cout][%00-00 ()]&]
[s2; Returns special output stream representing console output. Data 
written to this stream are displayed as characters in console.&]
[s4; [*/ Return value]-|Console stream.&]
[s0;3 &]
[s5;K [%00-00^`:`:String^ String][%00-00 _][%00-00@0 ReadStdIn][%00-00 ()]&]
[s2; Reads one line of input data from the console.&]
[s4; [*/ Return value]-|Console input.&]
[s0;3 &]
[s5;K [%00-00^`:`:Stream^ Stream][%00-00 `&_][%00-00@0 NilStream][%00-00 ()]&]
[s2; Returns special stream that is always in IsEof state and simply 
discards all data written to it.&]
[s4; [*/ Return value]-|`"Black hole`" stream.&]
[s0;3 &]
[s5;K [%00-00^`:`:String^ String][%00-00 _][%00-00@0 LoadFile][%00-00 (][%00-00@(0.0.255) const
][%00-00 _][%00-00@(0.0.255) char][%00-00 _`*][%00-00@3 filename][%00-00 )]&]
[s2; Loads the content of specified file.&]
[s4; [%00-00*C@3 filename]-|File name.&]
[s4; [*/ Return value]-|Content of file.&]
[s0;3 &]
[s5;K [%00-00@(0.0.255) bool][%00-00 _][%00-00@0 SaveFile][%00-00 (][%00-00@(0.0.255) const][%00-00 _
][%00-00@(0.0.255) char][%00-00 _`*][%00-00@3 filename][%00-00 , ][%00-00@(0.0.255) const][%00-00 _
][%00-00^`:`:String^ String][%00-00 `&_][%00-00@3 data][%00-00 )]&]
[s2; Saves data as the file (overwrites existing).&]
[s4; [%00-00*C@3 filename]-|File name.&]
[s4; [%00-00*C@3 data]-|Data to write.&]
[s4; [*/ Return value]-|true if file was successfully written.&]
[s0;3 &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 T][%00-00 >__][%00-00^`:`:Stream^ S
tream][%00-00 `&_][%00-00@0 operator%][%00-00 (][%00-00^`:`:Stream^ Stream][%00-00 `&_][%00-00@3 s
][%00-00 , ][%00-00^T^ T][%00-00 `&_][%00-00@3 x][%00-00 )]&]
[s2; Serialization operator. Simply invokes x.Serialize(s);&]
[s4; [*C@4 T]-|Type of variable to be serialized.&]
[s4; [%00-00*C@3 s]-|Stream.&]
[s4; [%00-00*C@3 x]-|Variable to be serialized.&]
[s4; [*/ Return value]-|s for chaining.&]
[s0;3 &]
[s5;K [%00-00^`:`:Stream^ Stream][%00-00 `&_][%00-00@0 operator<<][%00-00 (][%00-00^`:`:Stream^ S
tream][%00-00 `&_][%00-00@3 s][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00@(0.0.255) c
har][%00-00 _`*][%00-00@3 x][%00-00 )]&]
[s2; Overload of stream insertion operator to get simple case work. 
Insertion operator uses formatted stream output (Putf).&]
[s4; [%00-00*C@3 s]-|Output stream.&]
[s4; [%00-00*C@3 x]-|Data to write `- all characters are written to 
the stream.&]
[s4; [*/ Return value]-|s for chaining.&]
[s0;3 &]
[s5;K [%00-00^`:`:Stream^ Stream][%00-00 `&_][%00-00@0 operator<<][%00-00 (][%00-00^`:`:Stream^ S
tream][%00-00 `&_][%00-00@3 s][%00-00 , ][%00-00@(0.0.255) char][%00-00 _`*][%00-00@3 x][%00-00 )
]&]
[s2; Overload of stream insertion operator to get simple case work. 
Insertion operator uses formatted stream output (Putf).&]
[s4; [%00-00*C@3 s]-|Output stream.&]
[s4; [%00-00*C@3 x]-|Data to write `- all characters are written to 
the stream.&]
[s4; [*/ Return value]-|s for chaining.&]
[s0;3 &]
[s5;K [%00-00^`:`:Stream^ Stream][%00-00 `&_][%00-00@0 operator<<][%00-00 (][%00-00^`:`:Stream^ S
tream][%00-00 `&_][%00-00@3 s][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00^`:`:String^ S
tring][%00-00 _`&][%00-00@3 x][%00-00 )]&]
[s2; Overload of stream insertion operator to get simple case work. 
Insertion operator uses formatted stream output (Putf).&]
[s4; [%00-00*C@3 s]-|Output stream.&]
[s4; [%00-00*C@3 x]-|Data to write `- all characters are written to 
the stream.&]
[s4; [*/ Return value]-|s for chaining.&]
[s0;3 &]
[s5;K [%00-00^`:`:Stream^ Stream][%00-00 `&_][%00-00@0 operator<<][%00-00 (][%00-00^`:`:Stream^ S
tream][%00-00 `&_][%00-00@3 s][%00-00 , ][%00-00@(0.0.255) char][%00-00 _][%00-00@3 x][%00-00 )
]&]
[s2; Overload of stream insertion operator to get simple case work. 
Insertion operator uses formatted stream output (Putf).&]
[s4; [%00-00*C@3 s]-|Output stream.&]
[s4; [%00-00*C@3 x]-|Data to write `- it is written as single character.&]
[s4; [*/ Return value]-|s for chaining.&]
[s0;3 &]
[s5;K [%00-00^`:`:Stream^ Stream][%00-00 `&_][%00-00@0 operator<<][%00-00 (][%00-00^`:`:Stream^ S
tream][%00-00 `&_][%00-00@3 s][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00@(0.0.255) v
oid][%00-00 _`*][%00-00@3 x][%00-00 )]&]
[s2; Overload of stream insertion operator to get simple case work. 
Insertion operator uses formatted stream output (Putf).&]
[s4; [%00-00*C@3 s]-|Output stream.&]
[s4; [%00-00*C@3 x]-|Data to write `- pointer is formatted as hexadecimal 
value.&]
[s4; [*/ Return value]-|s for chaining.&]
[s0;3 &]
[s5;K [%00-00^`:`:Stream^ Stream][%00-00 `&_][%00-00@0 operator<<][%00-00 (][%00-00^`:`:Stream^ S
tream][%00-00 `&_][%00-00@3 s][%00-00 , ][%00-00@(0.0.255) void][%00-00 _`*][%00-00@3 x][%00-00 )
]&]
[s2; Overload of stream insertion operator to get simple case work. 
Insertion operator uses formatted stream output (Putf).&]
[s4; [%00-00*C@3 s]-|Output stream.&]
[s4; [%00-00*C@3 x]-|Data to write `- pointer is formatted as hexadecimal 
value.&]
[s4; [*/ Return value]-|s for chaining.&]
[s0;3 &]
[s5;K [%00-00 template_<][%00-00@(0.0.255) class][%00-00 _][%00-00@4 T][%00-00 >__][%00-00^`:`:Stream^ S
tream][%00-00 `&_][%00-00@0 operator<<][%00-00 (][%00-00^`:`:Stream^ Stream][%00-00 `&_][%00-00@3 s
][%00-00 , ][%00-00@(0.0.255) const][%00-00 _][%00-00^T^ T][%00-00 `&_][%00-00@3 x][%00-00 )]&]
[s2; Global stream insertion operator. Calls AsString for x and stores 
the result to the stream. Global AsString version in turn calls 
ToString method of x.&]
[s4; [*C@4 T]-|Type of data to write to the stream.&]
[s4; [%00-00*C@3 s]-|Output stream.&]
[s4; [%00-00*C@3 x]-|Data to write.&]
[s4; [*/ Return value]-|s for chaining.]