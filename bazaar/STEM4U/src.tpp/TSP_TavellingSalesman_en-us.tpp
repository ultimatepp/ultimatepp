topic "";
[H6;0 $$1,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$2,0#37138531426314131252341829483370:codeitem]
[l288;2 $$3,0#27521748481378242620020725143825:desc]
[0 $$4,0#96390100711032703541132217272105:end]
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[ {{10000@3 [s0; [*@7;4 Traveling salesman]]}}&]
[s1;%- &]
[s0;%- &]
[s0; [2 The ][^https`:`/`/en`.wikipedia`.org`/wiki`/Travelling`_salesman`_problem^2 Trave
ling salesman problem][2  (TSP) of a list of points calculates 
the shortest possible route that visits each point and returns 
to the origin.]&]
[s0;2 &]
[s0; [2 This is a ][^https`:`/`/en`.wikipedia`.org`/wiki`/NP`-hardness^2 NP`-hard][2  
problem that for medium sets could be impractical to be solved 
by an exact algorithm as brute force or ][^https`:`/`/en`.wikipedia`.org`/wiki`/Linear`_programming^2 l
inear programming][2 . Because of that, for practical reasons, 
its resolution using heuristics and approximations is advised.]&]
[s0;2 &]
[s0; [2 This implementation uses a ][^https`:`/`/en`.wikipedia`.org`/wiki`/Nearest`_neighbour`_algorithm^2 N
earest neighbor algorithm][2  to get an initial guess, and follows 
with a ][^https`:`/`/en`.wikipedia`.org`/wiki`/2`-opt^2 2`-opt 
algorithm][2 .]&]
[s0;2 &]
[s1;%- &]
[s2;:Upp`:`:TSP`(const Upp`:`:Vector`<Upp`:`:Vector`<T`>`>`&`,Upp`:`:Vector`<int`>`&`):%- [@(0.0.255) t
emplate]_<[@(0.0.255) typename]_[*@4 T]>_[*@4 T]_[* TSP]([@(0.0.255) const]_[_^Upp`:`:Vector^ V
ector]<[_^Upp`:`:Vector^ Vector]<[*@4 T]>>`&_[*@3 matrix], [_^Upp`:`:Vector^ Vector]<[@(0.0.255) i
nt]>_`&[*@3 order])&]
[s3; Returns the total distance of a traveling salesman problem defined 
by a [%-*@3 matrix] of distances between nodes. It also returns 
the most optimal [%-*@3 order] between the nodes.&]
[s3; [%-*@3 order] may include an initial guess of the right ordering. 
If not, order should be empty.&]
[s4; &]
[s1;%- &]
[s2;:Upp`:`:TSP`(const Upp`:`:Vector`<Upp`:`:Point`_`<T`>`>`&`,Upp`:`:Vector`<int`>`&`):%- [@(0.0.255) t
emplate]_<[@(0.0.255) typename]_[*@4 T]>_[*@4 T]_[* TSP]([@(0.0.255) const]_[_^Upp`:`:Vector^ V
ector]<[_^Upp`:`:Point`_^ Point`_]<[*@4 T]>>`&_[*@3 points], [_^Upp`:`:Vector^ Vector]<[@(0.0.255) i
nt]>_`&[*@3 order])&]
[s3; Returns the total distance of a traveling salesman problem defined 
by a list of euclidean coordinates of the [%-*@3 points]. It also 
returns the most optimal [%-*@3 order] between the nodes.&]
[s3; [%-*@3 order] may include an initial guess of the right ordering. 
If not, order should be empty.&]
[s4; ]]