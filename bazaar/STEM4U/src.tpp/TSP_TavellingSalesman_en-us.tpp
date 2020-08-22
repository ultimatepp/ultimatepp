topic "Traveling salesman problem (TSP) ";
[H6;0 $$1,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$2,0#37138531426314131252341829483370:codeitem]
[l288;2 $$3,0#27521748481378242620020725143825:desc]
[0 $$4,0#96390100711032703541132217272105:end]
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[ {{10000@3 [s0; [*@7;4 Traveling salesman]]}}&]
[s1;%- &]
[s0;%- &]
[s0; [2 The symmetric ][^https`:`/`/en`.wikipedia`.org`/wiki`/Travelling`_salesman`_problem^2 T
raveling salesman problem][2  (TSP) of a list of points calculates 
the shortest possible route that visits each point and returns 
to the origin, considering that it does not matter the direction 
(sense) of the movement.]&]
[s0;2 &]
[s0; [2 This is a ][^https`:`/`/en`.wikipedia`.org`/wiki`/NP`-hardness^2 NP`-hard][2  
problem that for medium sets could be impractical to be solved 
by an exact algorithm as brute force or ][^https`:`/`/en`.wikipedia`.org`/wiki`/Linear`_programming^2 l
inear programming][2 . Because of that, for practical reasons, 
its resolution using heuristics and approximations is advised.]&]
[s0;2 &]
[s0; [2 This implementation can use a consecutive, random, or a ][^https`:`/`/en`.wikipedia`.org`/wiki`/Nearest`_neighbour`_algorithm^2 N
earest neighbor algorithm][2  to get an initial guess, and follows 
with a ][^https`:`/`/en`.wikipedia`.org`/wiki`/2`-opt^2 2`-opt 
algorithm][2 . Since it uses templates, the input data can be either 
integer or floating point.]&]
[s0;2 &]
[s1; &]
[s2;:Upp`:`:TSP`(const Upp`:`:Vector`<Upp`:`:Point`_`<T`>`>`&`,Upp`:`:Vector`<int`>`&`,TSP`_Init`):%- [@(0.0.255) t
emplate]_<[@(0.0.255) typename]_[*@4 T]>_[*@4 T]_[* TSP]([@(0.0.255) const]_[_^Upp`:`:Vector^ V
ector]<[_^Upp`:`:Point`_^ Point`_]<[*@4 T]>>`&_[*@3 points], [_^Upp`:`:Vector^ Vector]<[@(0.0.255) i
nt]>_`&[*@3 order], TSP`_Init_[*@3 init])&]
[s3; Returns the total distance of a traveling salesman problem defined 
by a list of euclidean coordinates of the [%-*@3 points]. It also 
returns the most optimal [%-*@3 order] between the nodes.&]
[s3; [%-*@3 init ]defines the initial conditions, as:&]
[s3;i150;O0; TSP`_CONSECUTIVE: The actual order of supplied points. 
[%-*@3 order] is neglected.&]
[s3;i150;O0; TSP`_RANDOM: A random order of  supplied points. [%-*@3 order] 
is neglected.&]
[s3;i150;O0; TSP`_USER: [%-*@3 order] has to include an initial guess 
of the right ordering.&]
[s4; &]
[s1;%- &]
[s2;:Upp`:`:TSP`(const Upp`:`:Vector`<Upp`:`:Vector`<T`>`>`&`,Upp`:`:Vector`<int`>`&`,TSP`_Init`):%- [@(0.0.255) t
emplate]_<[@(0.0.255) typename]_[*@4 T]>_[*@4 T]_[* TSP]([@(0.0.255) const]_[_^Upp`:`:Vector^ V
ector]<[_^Upp`:`:Vector^ Vector]<[*@4 T]>>`&_[*@3 matrix], [_^Upp`:`:Vector^ Vector]<[@(0.0.255) i
nt]>_`&[*@3 order], TSP`_Init_[*@3 init])&]
[s3; Returns the total distance of a traveling salesman problem defined 
by a [%-*@3 matrix] of distances between nodes. All the elements 
of the diagonal of this matrix have to be zero and, as the problem 
is symmetric, only the elements above the diagonal are considered. 
It also returns the most optimal [%-*@3 order] between the nodes. 
&]
[s3; [%-*@3 init ]defines the initial conditions, as:&]
[s3;i150;O0; TSP`_CONSECUTIVE: The actual order of supplied points. 
[%-*@3 order] is neglected.&]
[s3;i150;O0; TSP`_RANDOM: A random order of  supplied points. [%-*@3 order] 
is neglected.&]
[s3;i150;O0; TSP`_USER: [%-*@3 order] has to include an initial guess 
of the right ordering.&]
[s4; &]
[s0; ]]