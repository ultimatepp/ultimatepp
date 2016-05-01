topic "Parallel Quicksort with U++ CoWork";
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s0; Parallel QuickSort with U`+`+ CoWork&]
[s0; &]
[s0; Introduction&]
[s0; QuickSort just as many other divide`-and`-conquer algorithms 
is very suitable for parallelization. In this article, we shall 
provide such version using CoWork parallelization class of U`+`+ 
framework.&]
[s0; &]
[s0; Serial version of the algorithm&]
[s0; As the starting point, we shall use the standard Sort function 
template from U`+`+ Core library:&]
[s0; &]
[s0; [C1 // highly optimized selection sort for small number of elements]&]
[s0; [C1 template <class I, class Less>]&]
[s0; [C1 void ForwardSort(I begin, I end, const Less`& less);]&]
[s0;C1 &]
[s0; [C1 template <class I, class Less>]&]
[s0; [C1 force`_inline]&]
[s0; [C1 void OrderIter2`_`_(I a, I b, const Less`& less)]&]
[s0; [C1 `{]&]
[s0; [C1 -|if(less(`*b, `*a))]&]
[s0; [C1 -|-|IterSwap(a, b);]&]
[s0; [C1 `}]&]
[s0;C1 &]
[s0; [C1 dword  Random(dword n); // High quality PRNG]&]
[s0;C1 &]
[s0; [C1 template <class I, class Less>]&]
[s0; [C1 void Sort(I l, I h, const Less`& less)]&]
[s0; [C1 `{]&]
[s0; [C1 -|for(;;) `{]&]
[s0; [C1 -|-|int count `= int(h `- l);]&]
[s0; [C1 -|-|if(count < 2)]&]
[s0; [C1 -|-|-|return;]&]
[s0; [C1 -|-|if(count < 8) `{                         // Final optimized 
SelectSort]&]
[s0; [C1 -|-|-|ForwardSort(l, h, less);]&]
[s0; [C1 -|-|-|return;]&]
[s0; [C1 -|-|`}]&]
[s0; [C1 -|-|int pass `= 4;]&]
[s0; [C1 -|-|for(;;) `{]&]
[s0; [C1 -|-|-|I middle `= l `+ (count >> 1);        // get the middle 
element]&]
[s0; [C1 -|-|-|OrderIter2`_`_(l, middle, less);      // sort l, middle, 
h`-1 to find median of 3]&]
[s0; [C1 -|-|-|OrderIter2`_`_(middle, h `- 1, less);]&]
[s0; [C1 -|-|-|OrderIter2`_`_(l, middle, less);      // median is now 
in middle]&]
[s0; [C1 -|-|-|IterSwap(l `+ 1, middle);            // move median pivot 
to l `+ 1]&]
[s0; [C1 -|-|-|I ii `= l `+ 1;]&]
[s0; [C1 -|-|-|for(I i `= l `+ 2; i !`= h `- 1; `+`+i)   // do partitioning; 
already l <`= pivot <`= h `- 1]&]
[s0; [C1 -|-|-|-|if(less(`*i, `*(l `+ 1)))]&]
[s0; [C1 -|-|-|-|-|IterSwap(`+`+ii, i);]&]
[s0; [C1 -|-|-|IterSwap(ii, l `+ 1);                // put pivot back 
in between partitions]&]
[s0; [C1 -|-|-|I iih `= ii;]&]
[s0; [C1 -|-|-|while(iih `+ 1 !`= h `&`& !less(`*ii, `*(iih `+ 1))) // 
Find middle range of elements equal to pivot]&]
[s0; [C1 -|-|-|-|`+`+iih;]&]
[s0; [C1 -|-|-|if(pass > 5 `|`| min(ii `- l, h `- iih) > (max(ii `- l, 
h `- iih) >> pass)) `{ // partition sizes ok or we have done 
max attempts]&]
[s0; [C1 -|-|-|-|if(ii `- l < h `- iih `- 1) `{       // recurse on smaller 
partition, tail on larger]&]
[s0; [C1 -|-|-|-|-|Sort(l, ii, less);]&]
[s0; [C1 -|-|-|-|-|l `= iih `+ 1;]&]
[s0; [C1 -|-|-|-|`}]&]
[s0; [C1 -|-|-|-|else `{]&]
[s0; [C1 -|-|-|-|-|Sort(iih `+ 1, h, less);]&]
[s0; [C1 -|-|-|-|-|h `= ii;]&]
[s0; [C1 -|-|-|-|`}]&]
[s0; [C1 -|-|-|-|break;]&]
[s0; [C1 -|-|-|`}]&]
[s0; [C1 -|-|-|IterSwap(l, l `+ (int)Random(count));     // try some other 
random elements for median pivot]&]
[s0; [C1 -|-|-|IterSwap(middle, l `+ (int)Random(count));]&]
[s0; [C1 -|-|-|IterSwap(h `- 1, l `+ (int)Random(count));]&]
[s0; [C1 -|-|-|pass`+`+;]&]
[s0; [C1 -|-|`}]&]
[s0; [C1 -|`}]&]
[s0; [C1 `}]&]
[s0; &]
[s0; This is fairly standard implementation. For small final subsections 
(<8 elements), optimized selection sort is used. Algorithm uses 
`"fat`" partitioning (elements equal to pivot are excluded from 
partitions). Pivot is decided as median of first, middle, last 
elements and algorithm contains detection of degenerate cases, 
in which case it does 2 more attempts to find a better pivot, 
using random elements to get median.&]
[s0; &]
[s0; Now the feature of QuickSort that makes it particularly easy 
to parallelize is the fact that once partitioned, there is no 
data sharing between partitions. Practically all we need to do 
is to replace recursion with parallel execution.&]
[s0; &]
[s0; The only problem here is how to reasonably manage threads. Thankfully, 
U`+`+ provides a useful parallelization class called CoWork.&]
[s0; &]
[s0; Meet the CoWork&]
[s0; CoWork can be thought of as an interface to the thread pool. 
It has two fundamental operations:&]
[s0; &]
[s0; Do&]
[s0; &]
[s0; schedules some code (U`+`+ Callback or C`+`+11 lambda) to be 
executed. (Note that Do can also be invoked using operator `&).&]
[s0; &]
[s0; Finish&]
[s0; &]
[s0; waits for all the code scheduled by Do to be finished. (Finish 
is invoked from CoWork destructor, so is usually omitted from 
the code).&]
[s0; &]
[s0; With respect to memory visibility, all changes made before Do 
are visible when scheduled code is performed and all changes 
performed in scheduled code are visible after Finish.&]
[s0; &]
[s0; Now how the code is executed by CoWork is implementation issue, 
but of course CoWork is trying as hard possible to use the global 
thread pool with as small overhead as possible.&]
[s0; &]
[s0; Parallel QuickSort&]
[s0; &]
[s0; So let us apply CoWork to QuickSort:&]
[s0; &]
[s0; [R0 struct CoSorter `{]&]
[s0; [R0 -|CoWork cw;]&]
[s0; [R0 -|]&]
[s0; [R0 -|enum `{ PARALLEL`_THRESHOLD `= 50 `};]&]
[s0;R0 &]
[s0; [R0 -|template <class I, class Less>]&]
[s0; [R0 -|void CoSort(I l, I h, const Less`& less)]&]
[s0; [R0 -|`{]&]
[s0; [R0 -|-|for(;;) `{]&]
[s0; [R0 -|-|-|int count `= int(h `- l);]&]
[s0; [R0 -|-|-|if(count < 2)]&]
[s0; [R0 -|-|-|-|return;]&]
[s0; [R0 -|-|-|if(count < 8) `{                         // Final optimized 
SelectSort]&]
[s0; [R0 -|-|-|-|ForwardSort(l, h, less);]&]
[s0; [R0 -|-|-|-|return;]&]
[s0; [R0 -|-|-|`}]&]
[s0; [R0 -|-|-|int pass `= 4;]&]
[s0; [R0 -|-|-|for(;;) `{]&]
[s0; [R0 -|-|-|-|I middle `= l `+ (count >> 1);        // get the middle 
element]&]
[s0; [R0 -|-|-|-|OrderIter2`_`_(l, middle, less);      // sort l, middle, 
h`-1 to find median of 3]&]
[s0; [R0 -|-|-|-|OrderIter2`_`_(middle, h `- 1, less);]&]
[s0; [R0 -|-|-|-|OrderIter2`_`_(l, middle, less);      // median is now 
in middle]&]
[s0; [R0 -|-|-|-|IterSwap(l `+ 1, middle);            // move median pivot 
to l `+ 1]&]
[s0; [R0 -|-|-|-|I ii `= l `+ 1;]&]
[s0; [R0 -|-|-|-|for(I i `= l `+ 2; i !`= h `- 1; `+`+i)   // do partitioning; 
already l <`= pivot <`= h `- 1]&]
[s0; [R0 -|-|-|-|-|if(less(`*i, `*(l `+ 1)))]&]
[s0; [R0 -|-|-|-|-|-|IterSwap(`+`+ii, i);]&]
[s0; [R0 -|-|-|-|IterSwap(ii, l `+ 1);                // put pivot back 
in between partitions]&]
[s0; [R0 -|-|-|-|I iih `= ii;]&]
[s0; [R0 -|-|-|-|while(iih `+ 1 !`= h `&`& !less(`*ii, `*(iih `+ 1))) // 
Find middle range of elements equal to pivot]&]
[s0; [R0 -|-|-|-|-|`+`+iih;]&]
[s0; [R0 -|-|-|-|if(pass > 5 `|`| min(ii `- l, h `- iih) > (max(ii `- l, 
h `- iih) >> pass)) `{ // partition sizes ok or we have done 
max attempts]&]
[s0; [R0 -|-|-|-|-|if(ii `- l < h `- iih `- 1) `{       // recurse on smaller 
partition, tail on larger]&]
[s0; [R0 -|-|-|-|-|-|if(ii `- l < PARALLEL`_THRESHOLD)]&]
[s0; [R0 -|-|-|-|-|-|-|CoSort(l, ii, less);]&]
[s0; [R0 -|-|-|-|-|-|else]&]
[s0; [R0 -|-|-|-|-|-|-|cw `& `[`=`] `{ CoSort(l, ii, less); `};]&]
[s0; [R0 -|-|-|-|-|-|l `= iih `+ 1;]&]
[s0; [R0 -|-|-|-|-|`}]&]
[s0; [R0 -|-|-|-|-|else `{]&]
[s0; [R0 -|-|-|-|-|-|if(h `- iih `- 1 < PARALLEL`_THRESHOLD)]&]
[s0; [R0 -|-|-|-|-|-|-|CoSort(iih `+ 1, h, less);]&]
[s0; [R0 -|-|-|-|-|-|else]&]
[s0; [R0 -|-|-|-|-|-|-|cw `& `[`=`] `{ CoSort(iih `+ 1, h, less); `};]&]
[s0; [R0 -|-|-|-|-|-|h `= ii;]&]
[s0; [R0 -|-|-|-|-|`}]&]
[s0; [R0 -|-|-|-|-|break;]&]
[s0; [R0 -|-|-|-|`}]&]
[s0; [R0 -|-|-|-|IterSwap(l, l `+ (int)Random(count));     // try some 
other random elements for median pivot]&]
[s0; [R0 -|-|-|-|IterSwap(middle, l `+ (int)Random(count));]&]
[s0; [R0 -|-|-|-|IterSwap(h `- 1, l `+ (int)Random(count));]&]
[s0; [R0 -|-|-|-|pass`+`+;]&]
[s0; [R0 -|-|-|`}]&]
[s0; [R0 -|-|`}]&]
[s0; [R0 -|`}]&]
[s0; [R0 `};]&]
[s0;R0 &]
[s0; [R0 template <class I, class Less>]&]
[s0; [R0 void CoSort(I l, I h, const Less`& less)]&]
[s0; [R0 `{]&]
[s0; [R0 -|CoSorter().CoSort(l, h, less);]&]
[s0; [R0 `}]&]
[s0;R0 &]
[s0; Just as promised, about the only change in the QuickSort code 
is replacement of recursion with CoWork`::Do scheduling (in operator 
`& form). PARALLEL`_THRESHOLD constant prevents parallel execution 
for small partitions `- scheduling them in parallel has diminishing 
returns and at some point, false cacheline sharing and CoWork 
management costs are higher that benefits of using more CPU cores. 
The value 80 used here is a combination of guts feeling and some 
experimentation and is not really critical, in experiments sorting 
Strings we have found that performance generally is getting worse 
if this value is >200 or <20.&]
[s0; &]
[s0; Benchmarks&]
[s0; We have benchmarked CoSort against original serial version, 
sorting Upp`::Vector of 10000000 of randomly generated Upp`::Strings. 
For reference, we have also used standard library std`::sort with 
std`::vector<std`::string> of same numbers. Benchmarks were performed 
on two platforms:&]
[s0; &]
[s0; Intel® Pentium(R) CPU G620 `@ 2.60GHz (2 cores, 2 threads)&]
[s0; Ubuntu Linux&]
[s0; gcc (Ubuntu 5.2.1`-22ubuntu2) 5.2.1 20151010&]
[s0; &]
[s0; TIMING std`::sort      :  6.43 s  `-  6.43 s  ( 6.43 s  / 1 ), 
min:  6.43 s , max:  6.43 s , nesting: 1 `- 1&]
[s0; TIMING CoSort         :  1.24 s  `-  1.24 s  ( 1.24 s  / 1 ), 
min:  1.24 s , max:  1.24 s , nesting: 1 `- 1&]
[s0; TIMING Sort           :  2.24 s  `-  2.24 s  ( 2.24 s  / 1 ), 
min:  2.24 s , max:  2.24 s , nesting: 1 `- 1&]
[s0; &]
[s0; &]
[s0; Intel® Pentium(R) Core i7`-4771 `@ 3.5GHz (4 cores, 8 threads)&]
[s0; Windows 10&]
[s0; Visual C`+`+ 2014 64`-bit&]
[s0; &]
[s0; TIMING std`::sort      :  5.10 s  `-  5.10 s  ( 5.10 s  / 1 ), 
min:  5.10 s , max:  5.10 s , nesting: 1 `- 1&]
[s0; TIMING CoSort         : 430.00 ms `- 430.00 ms (430.00 ms / 1 
), min: 430.00 ms, max: 430.00 ms, nesting: 1 `- 1&]
[s0; TIMING Sort           :  1.57 s  `-  1.57 s  ( 1.57 s  / 1 ), 
min:  1.57 s , max:  1.57 s , nesting: 1 `- 1&]
[s0; &]
[s0; As you can see, parallel / serial ratio (or `'parallel speedup`') 
is 1.80x on dual core machine and 3.65x on Core i7. Such results 
seem satisfactory given the size of dataset, which is likely 
to stress memory and cache subsystem.&]
[s0; &]
[s0; Conclusion&]
[s0; In the article we have demonstrated how Upp`::CoWork class can 
be used for quite trivial but effective parallelization of QuickSort 
algorithm.&]
[s0; ]]