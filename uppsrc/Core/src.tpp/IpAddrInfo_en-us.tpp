topic "IpAddrInfo";
[2 $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 IpAddrInfo]]}}&]
[s3; &]
[s1;:IpAddrInfo`:`:class: [@(0.0.255)3 class][3 _][*3 IpAddrInfo]&]
[s2;%% This class encapsulates [^http`:`/`/en`.wikipedia`.org`/wiki`/Getaddrinfo^ getad
drinfo] API, also providing optional non`-blocking behavior (using 
auxiliary threads). Note that there is currently no synchronization 
available for this non`-blocking behavior (it is not possible 
to e.g. select on IpAddrInfo). Also note that IpAddrInfo has 
some maximum number (currently 256) of slots used to resolve 
hosts simultaneously, however when this number is exceeded, it 
only means that InProgress tries to put the request to the slot 
(and returns true if non available or if request is not resolved 
yet), so from the client view, this limit has little impact. 
It also means that client should release the slot as soon as 
possible, using Clear method or by destructing IpAddrInfo.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:IpAddrInfo`:`:Start`(const String`&`,int`,int`): [@(0.0.255) void]_[* Start]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 host], [@(0.0.255) int]_[*@3 port], 
[@(0.0.255) int]_[*@3 family]_`=_FAMILY`_ANY)&]
[s2;%% Starts resolving address of [%-*@3 host] (with [%-*@3 port]). 
[%-*@3 family] can be used to narrow the scan to particular IP 
protocol (FAMILY`_IPV4, FAMILY`_IPV6).&]
[s3;%% &]
[s4; &]
[s5;:IpAddrInfo`:`:InProgress`(`): [@(0.0.255) bool]_[* InProgress]()&]
[s2;%% Returns true if resolving of address is still in progress.&]
[s3; &]
[s4; &]
[s5;:IpAddrInfo`:`:Execute`(const String`&`,int`,int`): [@(0.0.255) bool]_[* Execute]([@(0.0.255) c
onst]_[_^String^ String][@(0.0.255) `&]_[*@3 host], [@(0.0.255) int]_[*@3 port], 
[@(0.0.255) int]_[*@3 family]_`=_FAMILY`_ANY)&]
[s2;%% Resolves address [%-*@3 host] (with [%-*@3 port]). This is blocking 
variant, returns after address is resolved. Returns true if address 
was found. [%-*@3 family] can be used to narrow the scan to particular 
IP protocol (FAMILY`_IPV4, FAMILY`_IPV6).&]
[s3;%% &]
[s4; &]
[s5;:IpAddrInfo`:`:GetResult`(`)const: addrinfo_`*[* GetResult]()_[@(0.0.255) const]&]
[s2;%% Returns resulting [^http`:`/`/en`.wikipedia`.org`/wiki`/Getaddrinfo^ addrinfo] 
if address resolving was finished and successfull, NULL otherwise.&]
[s3; &]
[s4; &]
[s5;:IpAddrInfo`:`:Clear`(`): [@(0.0.255) void]_[* Clear]()&]
[s2;%% Resets IpAddrInfo to initial state. Not that this also releases 
the slot used for resolving the request.&]
[s3; &]
[s4; &]
[s5;:IpAddrInfo`:`:IpAddrInfo`(`): [* IpAddrInfo]()&]
[s5;:IpAddrInfo`:`:`~IpAddrInfo`(`): [@(0.0.255) `~][* IpAddrInfo]()&]
[s2;%% Constructor, destructor.&]
[s3; ]]