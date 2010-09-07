topic "template <class T, class B = EmptyClass>";
[ $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9; $$1,0#37138531426314131252341829483380:structitem]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:codeitem]
[{_} 
[s1;:noref: [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T], [@(0.0.255) class]_[*@4 B]_`=_Em
ptyClass>&]
[s1;:Dispatchable`:`:class: [@(0.0.255) class]_[* Dispatchable]&]
[s2;%% an interface for a class that wants to participate in dispatching 
from Dispatcher<T>&]
[s3; &]
[s4; &]
[s5;:Dispatchable`:`:Dispatch`(const T`&`): [@(0.0.255) virtual] [@(0.0.255) void]_[* Dispa
tch]([@(0.0.255) const]_[*@4 T]_`&_[*@3 o])_`=_[@3 0]&]
[s2;%% this one need to be implemented, it receives an [%-*@3 o].object 
ref, which has been posted to the Dispatcher<T>. do something 
with this object.&]
[s3;%% &]
[s4; &]
[s5;:Dispatchable`:`:Unregister`(const Any`&`): [@(0.0.255) void]_[* Unregister]([@(0.0.255) c
onst]_[_^Any^ Any]_`&_[*@3 `_src])&]
[s2;%% unregisters the interface from [%-*@3 `_src].Dispatcher<T> or 
DispatcherGen&]
[s3;%% &]
[s4; &]
[s5;:Dispatchable`:`:UnregisterAll`(`): [@(0.0.255) void]_[* UnregisterAll]()&]
[s2;%% unregisters the interface from all Dispatchers it is registered 
in&]
[s3; &]
[s4; &]
[s5;:Dispatchable`:`:Unregister`(Dispatcher`<T`>`&`): [@(0.0.255) void]_[* Unregister]([_^Dispatcher^ D
ispatcher]<[*@4 T]>_`&_[*@3 from])&]
[s2;%% unregisters the interface [%-*@3 from].Dispatcher&]
[s3;%% &]
[s4; &]
[s5;:Dispatchable`:`:Unregister`(DispatcherGen`&`): [@(0.0.255) void]_[* Unregister]([_^DispatcherGen^ D
ispatcherGen]_`&_[*@3 from])&]
[s2;%% unregisters the interface [%-*@3 from].DispatcherGen&]
[s3;%% &]
[s4; &]
[s5;:Dispatchable`:`:GetSrc`(`)const: [@(0.0.255) const]_[_^VectorMap^ VectorMap]<[@(0.0.255) u
nsigned], [_^Any^ Any]>_`&_[* GetSrc]()_[@(0.0.255) const]&]
[s2;%% Get all source dispatchers (either Dispatcher<T> or DispatcherGen)&]
[s3; &]
[s0; ]