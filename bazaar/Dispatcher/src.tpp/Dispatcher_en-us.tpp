topic "template <class T>";
[ $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9; $$1,0#37138531426314131252341829483380:structitem]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:codeitem]
[{_} 
[s1; [@(0.0.255) Dispatcher:]&]
[s0; sometimes it is quite handy to bundle a set of interfaces and 
to forward objects to all of them with a single call. the Dispatcher 
makes this easy providing means of registering and unregistering 
the receivers`' interface.&]
[s0; &]
[s0; There are 2 types of Dispatchers. the Dispatchable<T> interface 
based Dispatcher<T> and the Callback1<const T`&> based DispatcherCB<T>. 
while Dispatcher<T> is probably a bit faster, DispatcherCB<T> 
is more flexible, because you can use whatever member function 
of your class to register (as long as it fits the parameter list).&]
[s0; &]
[s0; Take a look at the diverse Test applications to find out what 
case best fits for you.&]
[s0; &]
[s1;:noref: [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>&]
[s1;:Dispatcher`:`:class: [@(0.0.255) class]_[* Dispatcher]_:_[@(0.0.255) public]_[*@3 EnableO
ption]<>_&]
[s2;%% a class that can forward T to registered receivers (Dispatchable<T>)&]
[s3; &]
[s4; &]
[s5;:Dispatcher`:`:DoDispatch`(const T`&`)const: [@(0.0.255) void]_[* DoDispatch]([@(0.0.255) c
onst]_[*@4 T]_`&_[*@3 o])_[@(0.0.255) const]&]
[s2;%% Entrance point for the Dispatcher, will forward object [%-*@3 o].to 
all registered Dispatchable<T> interfaces&]
[s3;%% &]
[s4; &]
[s5;:Dispatcher`:`:Register`(Dispatchable`<T`>`&`,unsigned`): [@(0.0.255) void]_[* Regist
er]([_^Dispatchable^ Dispatchable]<[*@4 T]>_`&_[*@3 d], [@(0.0.255) unsigned]_[*@3 key]_`=_
[@3 0])&]
[s2;%% register Dispatchable<T> interface [%-*@3 d ][%- using] [%-*@3 key].(if 
not specified, GetPtrHashValue will be used from d)&]
[s0;%% &]
[s3;%% &]
[s4; &]
[s5;:Dispatcher`:`:Unregister`(Dispatchable`<T`>`&`,unsigned`): [@(0.0.255) void]_[* Unre
gister]([_^Dispatchable^ Dispatchable]<[*@4 T]>_`&_[*@3 d], [@(0.0.255) unsigned]_[*@3 key]_
`=_[@3 0])&]
[s2;%% unregisters [%-*@3 d] using [%-*@3 key].(if not specified, GetPtrHashValue 
will be used from d)&]
[s3;%% &]
[s4; &]
[s5;:Dispatcher`:`:GetDispatchable`(unsigned`)const: [_^Dispatchable^ Dispatchable]<[*@4 T
]>_`*_[* GetDispatchable]([@(0.0.255) unsigned]_[*@3 key])_[@(0.0.255) const]&]
[s2;%% find the [%-*@3 key].respective Dispatchable<T> interface&]
[s3;%% &]
[s4; &]
[s5;:Dispatcher`:`:GetDests`(`)const: [@(0.0.255) const]_[_^VectorMap^ VectorMap]<[@(0.0.255) u
nsigned], [_^Dispatchable^ Dispatchable]<[*@4 T]>_`*_>_`&_[* GetDests]()_[@(0.0.255) cons
t]&]
[s2;%% Get all destinations&]
[s3; &]
[s4; &]
[s5;:Dispatcher`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%% Clears the Dispatcher, no destinations present&]
[s3; &]
[s0; ]