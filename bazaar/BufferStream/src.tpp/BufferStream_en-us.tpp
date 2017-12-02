topic "class BufferStream : public MemStream";
[ $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9; $$1,0#37138531426314131252341829483380:structitem]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:codeitem]
[{_} 
[s1;:BufferStream`:`:class: [@(0.0.255) class]_[* BufferStream]_:_[@(0.0.255) public]_[*@3 Mem
Stream]&]
[s2;%% A Stream based on a Vector<byte> buffer, beeing able to open 
such a Vector<byte> to Loading or directly serializing to such 
a Vector.It automatically increases the underlying Vector. It 
uses `_pick semantics to open a Vector<byte> and returns the 
underlying Vector<byte> as `_pick (leaving a new instance under 
hood, so new buffering is possible as well)&]
[s3; &]
[s4; &]
[s5;:BufferStream`:`:Open`(Vector`<byte`>`&`): [@(0.0.255) void]_[* Open]([_^Vector^ Vector
]<[_^byte^ byte]>_`&_[*@3 d])&]
[s2;%% Picks the Vector [%-*@3 d].as underlying buffer in Loading mode.&]
[s3;%% &]
[s4; &]
[s5;:BufferStream`:`:Create`(`): [@(0.0.255) void]_[* Create]()&]
[s2;%% resets underlying buffer and sets Stream to Storing mode. 
rewinds current ptr to beginning.&]
[s3; &]
[s4; &]
[s5;:BufferStream`:`:Reserve`(int`): [@(0.0.255) void]_[* Reserve]([@(0.0.255) int]_[*@3 n])&]
[s2;%% reserves additional [%-*@3 n].bytes in the Stream. sets Storing 
mode.can speed up things if you know how much is to come.&]
[s3;%% &]
[s4; &]
[s5;:BufferStream`:`:Crop`(`): [@(0.0.255) void]_[* Crop]()&]
[s2;%% crops the internal buffer to currently used size, use when 
storing.&]
[s3; &]
[s4; &]
[s5;:BufferStream`:`:GetResult`(`): [_^Vector^ Vector]<[_^byte^ byte]>_[* GetResult]()&]
[s2;%% picks internal Vector, leaving an initialized Vector for new 
operations.&]
[s3; &]
[s4; &]
[s5;:BufferStream`:`:BufferStream`(`): [* BufferStream]()&]
[s2;%% creates a BufferStream in Storing Mode&]
[s3; &]
[s4; &]
[s5;:BufferStream`:`:BufferStream`(Vector`<byte`>`&`): [* BufferStream]([_^Vector^ Vector
]<[_^byte^ byte]>`&_[*@3 d])&]
[s2;%% Creates a BufferStream in Loading mode. Loads Vector [%-*@3 d].picking 
it.&]
[s3;%% &]
[s0; ]