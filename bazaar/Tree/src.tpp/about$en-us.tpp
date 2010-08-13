topic "Tree<T>";
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s0; [+150 Tree<T>]&]
[s0; &]
[s0; A Tree is a very effective way of bringing in some level of 
logical organisation in a bunch of data which the NTL containers 
can`'t provide inherently. They make access to a variety of `*[* equal 
/ similar]`* data easy but it is all lineary organised. Every 
item is equaly important. It is hard and errorprone to say `'The 
first ten elements are special, the others are reserved`'. In 
some cases one can use linked lists (U`+`+ Link<T>) to reflect 
such things, defining maybe sth like `'everything after LinkOwner<T> 
is free, everything before is used. But what if grouped hierarchy 
comes into play? Handling with sorts of paths etc. Here, a Tree 
can help provide means not merly to store elements, but bring 
in a model based hierarchy.&]
[s0; &]
[s0; One could use constructs like this:&]
[s0; &]
[s0;i160; [C2 struct MyTree]&]
[s0;i160; [C2 `{]&]
[s0;i160; [C2 -|Array<MyTree> tree;]&]
[s0;i160; [C2 `};]&]
[s0;i160; &]
[s0; but one might end up needing Parent references, or even Root 
references. and who is going to take care of them when moving 
the elements around?&]
[s0; &]
[s0; Here comes a solution, which is not the only one in this world, 
but serves 95 % of cases.&]
[s0; &]
[s0; The Tree<T> is actually a partially hidden Array of the same 
type elements, This making usage of well known and tested NTL 
containers. It has a parent pointer and a root pointer. some 
methods from Array are free to access, some are overblended. 
the protected inheritance ensures that the overblended base methods 
are inaccessible in any case. some methods are not critical though 
and can be exposed (using using), in some, the correct parent 
ref is to be ensured. this thing can be thought in Vector and 
the Map flavours as well, making handling more difficult though. 
Array was chosen because of its Attach/Detach ability, which 
comes in really handy when dealing with Trees (attaching detaching 
or moving trees somewhere)&]
[s0; &]
[s0; Maybe in near future, one might provide an extended templated 
version where to specify which container to use...&]
[s0; &]
[s0; Example of usage:&]
[s0; &]
[s0; HINT: using One<T> as T instead of T directly gives you the ability 
of handling true leafs, which are attachable / detachable independantly 
from the tree structure. This a tree can remain intact, while 
you detach the leafs..&]
[s0; &]
[s0;i160; [C2 class Element]&]
[s0;i160; [C2 `{]&]
[s0;i160; [C2 public:]&]
[s0;i160; [C2 -|String name;]&]
[s0;i160; [C2 -|Value value;]&]
[s0;i160; [C2 `};]&]
[s0;i160;C2 &]
[s0;i160; [C2 Node<One<Element> > root;]&]
[s0;i160; [C2 Node<Element> roota;]&]
[s0;i160;C2 &]
[s0;i160; [C2 //Tree<One<Element> >]&]
[s0;i128;C2 &]
[s0;i128; [C2 root.SetAsRoot();]&]
[s0;i128; [C2 root.leaf.Create();]&]
[s0;i128; [C2 root.leaf`->name `= `"/`";]&]
[s0;i128; [C2 root.SetCount(3);]&]
[s0;i128; [C2 Node<One<Element> > `& child `= root`[2`];]&]
[s0;i128; [C2 child.leaf.Create();]&]
[s0;i128; [C2 child.leaf`->name `= `"Hallo`";]&]
[s0;i128; [C2 String `& s `= child.GetParent()`->leaf`->name;]&]
[s0;i128; [C2 s `= `"root`";]&]
[s0;i128; [C2 String `& ss `= root.leaf`->name;]&]
[s0;i128;C2 &]
[s0;i128; [C2 //Node<Element>]&]
[s0;i128; [C2 roota.SetAsRoot();]&]
[s0;i128; [C2 roota.leaf.name `= `"/`";]&]
[s0;i128; [C2 roota.SetCount(3);]&]
[s0;i128; [C2 Node<Element> `& childa `= roota`[2`];]&]
[s0;i128; [C2 childa.leaf.name `= `"Hallo`";]&]
[s0;i128; [C2 String `& sa `= childa.GetParent()`->leaf.name;]&]
[s0;i128; [C2 sa `= `"root`";]&]
[s0;i128; [C2 String `& ssa `= roota.leaf.name;]&]
[s0;i128;C2 ]