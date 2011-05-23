topic "template <class T>";
[ $$0,0#00000000000000000000000000000000:Default]
[i448;a25;kKO9; $$1,0#37138531426314131252341829483380:structitem]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[{_}%EN-US 
[s1;:noref:%- [@(0.0.255) template]_<[@(0.0.255) class]_[*@4 T]>&]
[s1;:Shared`:`:class:%- [@(0.0.255) class]_[* Shared]_:_[@(0.0.255) private]_[*@3 Moveable]<_[* S
hared]<[*@4 T]>_>_&]
[s2; Shared pointer/Container&]
[s2; Idea borrowed from boost`::shared`_ptr, an additional chunk of 
memory is managed which centrally holds the refcount of that 
object pointed to.&]
[s0; &]
[s2; if Shared is created freshly, its ref count is set to 1;&]
[s2; if a Shared is destroyed it AtomicDec`'s the refcount, and if 
its 0, it will delete both, the object and the refcount chunk.&]
[s2; if another instance of Shared is created as copy, the refcount 
is taken from the source and incremented. if it is assigned, 
it decrements own existing counter, possibly releasing mem, and 
retains new object. &]
[s2; [* pick semantic is not needed here anymore, it is not even possible 
]since an `'operator`=(const Shared<>`&) is needed to aquire 
the source. pick is const in some cases as well).&]
[s2; Thus Shared is only Moveable, without DeepCopyOption, which 
in fact would speak against the idea of Shared anyway. Attach 
/ Detach remains&]
[s2; &]
[s0; IMPORTANT NOTE:&]
[s3;%- &]
[s0;%- There is no usecase for Shared<>, as long as you design the 
code U`+`+ way. The author of U`+`+ has produced quite a lot 
of software in the past 10 years (say about half million lines 
of C`+`+). and was addressing pretty wide area of problems, from 
theide to website backends. There are no shared ownership pointers 
in his code anywhere.&]
[s0;%- &]
[s0;%- Nevertheless, if using Shared<> for a specific type, stick 
to the rule: [* All ar nothing]. either use Shared<> for a type 
throughout the project [* without exception] or don`'t use it at 
all. you will surely have a lot of trouble otherwise.&]
[s0; ]