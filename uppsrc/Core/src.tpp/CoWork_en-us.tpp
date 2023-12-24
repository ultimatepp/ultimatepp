topic "CoWork";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,0#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_} 
[s0; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 CoWork]]}}&]
[s3; &]
[s1;:CoWork`:`:class: [@(0.0.255)3 class][3 _][*3 CoWork]&]
[s9;%% This class is indented as general parallelization tool. Whenever 
jobs (e.g. loop iterations) are independent (they do not share 
any data between iterations), CoWork can be used to relatively 
easily spawn loop iterations over threads and thus over CPU cores. 
Note that previous statement does [* not] preclude CoWork iterations 
to share data at all `- sharing data using Mutex or similar serialization 
mechanisms still works. CoWork works with fixed`-size thread 
pool, which is created during initialization phase (which first 
CoWork constructor is called). No more thread are created or 
destroyed during normal work. Nesting of CoWork instances is 
also possible. Thread pool is normally terminated when the main 
thread finishes.&]
[s9;%% No synchronization is required to access CoWork instances 
from various threads (CoWork is internally synchronized).&]
[s9;%% If an exception is thrown in worker thread, which is not handled 
by worker thread, it is caught and rethrown in CoWork thread 
in Finish routine. Any such exception also causes the Cancel 
of the CoWork. For this reason, CoWork destructor [*/ CAN] throw 
exceptions and CoWork should be usually used as automatick (stack) 
variable. If you need CoWork that does not throw exceptions in 
destructor, use CoWorkNX.&]
[s9;%% [*/ Implementation notes: ]Current implementation has single 
global FIFO stack for 2048 scheduled jobs. When there is no slot 
available when scheduling the job, it is performed immediately 
by Do. Finish method has to wait until all jobs scheduled by 
CoWork instance are finished, while waiting it attempts to perform 
scheduled jobs from the same instance. That way work always progresses 
even if there is shortage of worker threads.&]
[s0;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:CoWork`:`:TrySchedule`(Function`&`&`): [@(0.0.255) static 
bool]_[* TrySchedule]([_^Upp`:`:Function^ Function]<[@(0.0.255) void]_()>`&`&_[*@3 fn])&]
[s5;:Upp`:`:CoWork`:`:TrySchedule`(const Function`&`): [@(0.0.255) static] 
[@(0.0.255) bool]_[* TrySchedule]([@(0.0.255) const]_[_^Upp`:`:Function^ Function]<[@(0.0.255) v
oid]_()>`&_[*@3 fn])&]
[s2;%% This is a low`-level function that attempts to schedule [%-*@3 fn] 
to be executed by worker thread. Returns true if [%-*@3 fn] was 
scheduled, false if not (in case there is no slot left in scheduling 
stacks). Note that this function only schedules the function, 
the exact time of execution is unknown.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoWork`:`:Schedule`(Function`&`&`): [@(0.0.255) static 
void]_[* Schedule]([_^Upp`:`:Function^ Function]<[@(0.0.255) void]_()>`&`&_[*@3 fn])&]
[s5;:Upp`:`:CoWork`:`:Schedule`(const Function`&`): [@(0.0.255) static] 
[@(0.0.255) void]_[* Schedule]([@(0.0.255) const]_[_^Upp`:`:Function^ Function]<[@(0.0.255) v
oid]_()>`&_[*@3 fn])&]
[s2;%% Similar to TrySchedule, but always schedules [%-*@3 fn] `- even 
if it has to wait for slot to become available.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoWork`:`:Do`(Function`&`&`): [@(0.0.255) void]_[* Do]([_^Upp`:`:Function^ Func
tion]<[@(0.0.255) void]_()>`&`&_[*@3 fn])&]
[s5;:Upp`:`:CoWork`:`:Do`(const Function`&`): [@(0.0.255) void]_[* Do]([@(0.0.255) const]_[_^Upp`:`:Function^ F
unction]<[@(0.0.255) void]_()>`&_[*@3 fn])&]
[s5;:Upp`:`:CoWork`:`:operator`&`(const Function`&`): [_^Upp`:`:CoWork^ CoWork][@(0.0.255) `&
]_[* operator`&]([@(0.0.255) const]_[_^Upp`:`:Function^ Function]<[@(0.0.255) void]_()>`&
_[*@3 fn])&]
[s5;:Upp`:`:CoWork`:`:operator`&`(Function`&`&`): [_^Upp`:`:CoWork^ CoWork][@(0.0.255) `&
]_[* operator`&]([_^Upp`:`:Function^ Function]<[@(0.0.255) void]_()>`&`&_[*@3 fn])&]
[s2;%% Schedules [%-*@3 fn] to be executed. All changes to data done 
before Do are visible in the scheduled code. The order of execution 
or whether the code is execute in another or calling thread is 
not specified. In certain situations (no scheduling slot available), 
Do can perform scheduled job immediately in calling thread.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:CoWork`:`:GetScheduledCount`(`)const: [@(0.0.255) int] 
[* GetScheduledCount]() [@(0.0.255) const]&]
[s2;%% Returns a number of remaining unfinished jobs scheduled by 
Do in this CoWork.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:CoWork`:`:FinLock`(`): [@(0.0.255) static] [@(0.0.255) void]_[* FinLock]()&]
[s2;%% This functions is to be called in scheduled routine. Its purpose 
is to serialize access to shared data at the end of the routine. 
The rationale is that CoWork has to lock some mutex anyway after 
scheduled code finishes, so FinLock can lock this mutex a bit 
earlier, joining two mutex locks into single one. Of course, 
as with all locks, execution of locked code should be short as 
not to cause congestion of CoWork scheduling.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoWork`:`:Cancel`(`): [@(0.0.255) void]_[* Cancel]()&]
[s2;%% Removes all jobs scheduled by this thread that has not started 
yet from the queue and then waits for any jobs already started 
to finish. Rethrows the exception thrown in worker threads. If 
more than single worker thread throws the exception, the first 
exception thrown in absolute time is rethrown.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:CoWork`:`:IsCanceled`(`): [@(0.0.255) static] [@(0.0.255) bool]_[* IsCanceled](
)&]
[s2;%% This methods returns true in worker thread when the worker 
thread is a part of some CoWork instance and that instance was 
canceled.&]
[s3; &]
[s4; &]
[s5;:CoWork`:`:Finish`(`): [@(0.0.255) void]_[* Finish]()&]
[s2;%% Waits until all jobs scheduled using Do (or operator`&) are 
finished. All changes to data performed by scheduled threads 
are visible after Finish. While waiting, Finish can perform scheduled 
jobs. Can &]
[s3; &]
[s4; &]
[s5;:Upp`:`:CoWork`:`:IsFinished`(`): [@(0.0.255) bool]_[* IsFinished]()&]
[s2;%% Checkes whether all jobs scheduled using Do (or operator`&) 
are finished. All changes to data performed by scheduled threads 
are visible after IsFinished returns true (so this is basically 
non`-blocking variant of Finish).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoWork`:`:Reset`(`): [@(0.0.255) void]_[* Reset]()&]
[s2;%% Calls Cancel, catches and ignores all exceptions eventually 
thrown by worker threads. Then resets CoWork to the initial state 
as if it was just constructed. Useful when using CoWork as nonlocal 
variable.&]
[s3; &]
[s4; &]
[s5;:CoWork`:`:`~CoWork`(`): [@(0.0.255) `~][* CoWork]()&]
[s2;%% Calls Finish(). Can eventually rethrow worker thread exception. 
If there is a chance of destructor being involved in stack unwinding, 
Finish should be called separately before destructor.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:CoWork`:`:IsWorker`(`): [@(0.0.255) static] [@(0.0.255) bool]_[* IsWorker]()&]
[s2;%% Returns true if current thread is CoWork worker thread.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:CoWork`:`:GetWorkerIndex`(`): [@(0.0.255) static] [@(0.0.255) int]_[* GetWorker
Index]()&]
[s2;%% Returns the index of current worker thread `- index is >`= 
0 and < GetPoolSize(). This is useful if there is a need for 
per`-thread resources. `-1 means that thread is not worker.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:CoWork`:`:GetPoolSize`(`): [@(0.0.255) static] [@(0.0.255) int]_[* GetPoolSize](
)&]
[s2;%% Returns the current count of worker threads.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:CoWork`:`:SetPoolSize`(int`): [@(0.0.255) static void]_[* SetPoolSize]([@(0.0.255) i
nt]_[*@3 n])&]
[s2;%% Adjusts the thread pool size (default pool size is CPU`_Cores() 
`+ 2).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoWork`:`:Loop`(Function`&`&`): [@(0.0.255) void]_[* Loop]([_^Upp`:`:Function^ F
unction]<[@(0.0.255) void]_()>`&`&_[*@3 fn])&]
[s5;:Upp`:`:CoWork`:`:Loop`(const Function`&`): [@(0.0.255) void]_[* Loop]([@(0.0.255) cons
t]_[_^Upp`:`:Function^ Function]<[@(0.0.255) void]_()>`&_[*@3 fn])&]
[s5;:Upp`:`:CoWork`:`:operator`*`(const Function`&`): [_^Upp`:`:CoWork^ CoWork][@(0.0.255) `&
]_[* operator`*]([@(0.0.255) const]_[_^Upp`:`:Function^ Function]<[@(0.0.255) void]_()>`&
_[*@3 fn])&]
[s5;:Upp`:`:CoWork`:`:operator`*`(Function`&`&`): [_^Upp`:`:CoWork^ CoWork][@(0.0.255) `&
]_[* operator`*]([_^Upp`:`:Function^ Function]<[@(0.0.255) void]_()>`&`&_[*@3 fn])&]
[s2;%% Schedules [%-*@3 fn] to be run on all worker threads and on 
calling thread. After the first thread returns from [%-*@3 fn], 
all other scheduled [%-*@3 fn] jobs that has not started yet are 
unscheduled. Waits for all started jobs to finish. The function 
also sets internal index counter to zero in CoWork before starting 
any worker thread. Worker thread should acquire job quantum in 
internal loop `- internal CoWork index can be used for this purpose. 
[/ Deprecated `- use CoDo instead.]&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoWork`:`:Next`(`): [@(0.0.255) int]_[* Next]()&]
[s2;%% Atomically increments internal index counter and returns its 
previous value (thus the first value returned is 0). Supposed 
to be used with Loop. [/ Deprecated `- use std`::atomic<int> variable 
with CoDo instead.]&]
[s0; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 CoWorkNX]]}}&]
[s0; &]
[s1;:Upp`:`:CoWorkNX`:`:struct: [@(0.0.255)3 struct][3 _][*3 CoWorkNX][3 _:_][@(0.0.255)3 public
][3 _][*@3;3 CoWork]&]
[s2;%% This simple helper class just removes `"noexcept(false)`" 
from CoWork destructor, removing exception throws from the destructor 
and allowing it to be used as member variable. Obviously, if 
exception handling is required, you need to use Finish (not to 
depend on destructor to call it).&]
[s3; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 Loop parallelisation functions]]}}&]
[s3; &]
[s5;:Upp`:`:CoDo`(Function`&`&`): [@(0.0.255) void]_[* CoDo]([_^Upp`:`:Function^ Function]<
[@(0.0.255) void]_()>`&`&_[*@3 fn])&]
[s2;%% Schedules [%-*@3 fn] to be run on all worker threads and on 
calling thread. After the first thread returns from [%-*@3 fn], 
all other scheduled [%-*@3 fn] jobs that has not started yet are 
unscheduled. Waits for all started jobs to finish. Worker thread 
should acquire job quantum in internal loop `- usually std`::atomic 
is used for this purpose. This is the most effective and flexible 
way to parallelise iteration.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoDo`_ST`(Function`&`&`): [@(0.0.255) void]_[* CoDo`_ST]([_^Upp`:`:Function^ Fu
nction]<[@(0.0.255) void]_()>`&`&_[*@3 fn])&]
[s2;%% This function simply calls [%-*@3 fn]. It is diagnostics tool 
`- it allows to change CoDo parallel iteration into serial one 
by adding `"`_ST`" text.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoDo`(bool`,Function`&`&`): [@(0.0.255) void]_[* CoDo]([@(0.0.255) bool]_[*@3 co],
 [_^Upp`:`:Function^ Function]<[@(0.0.255) void]_()>`&`&_[*@3 fn])&]
[s2;%% If [%-*@3 co] is true, calls CoDo([%-*@3 fn]), otherwise CoDo`_ST([%-*@3 fn]). 
This allows to parametrize algorithms with respect to parallelization.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoFor`(int`,Fn`): [@(0.0.255) template]_<[@(0.0.255) typename]_[*@4 Fn]>_[@(0.0.255) v
oid]_[* CoFor]([@(0.0.255) int]_[*@3 n], [*@4 Fn]_[*@3 iterator])&]
[s2;%% Based on CoDo, runs in parallel [%-*@3 iterator] for values 
0..[%-*@3 n] passing the value as argument.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:CoFor`_ST`(int`,Fn`): [@(0.0.255) template]_<[@(0.0.255) typename]_[*@4 Fn]>_[@(0.0.255) v
oid]_[* CoFor`_ST]([@(0.0.255) int]_[*@3 n], [*@4 Fn]_[*@3 iterator])&]
[s2;%% Single threaded variant of CoFor, for diagnostics purposes.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoFor`(bool`,int`,Fn`): [@(0.0.255) template]_<[@(0.0.255) typename]_[*@4 Fn]>_
[@(0.0.255) void]_[* CoFor]([@(0.0.255) bool]_[*@3 co], [@(0.0.255) int]_[*@3 n], 
[*@4 Fn]_[*@3 iterator])&]
[s2;%% Calls CoFor([%-*@3 n], [%-*@3 iterator]) if [%-*@3 co] is true, 
CoFor`_ST([%-*@3 n], [%-*@3 iterator]) otherwise. This allows to 
parametrize algorithms with respect to parallelization.&]
[s3; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 CoWorkerResources]]}}&]
[s3; &]
[s1;:noref: [@(0.0.255)3 template][3 _<][@(0.0.255)3 class][3 _][*@4;3 T][3 >]&]
[s1;:Upp`:`:CoWorkerResources`:`:class: [@(0.0.255) class]_[* CoWorkerResources]&]
[s2;%% This is a simple helper class that provides per`-worker resources. 
For example, certain calculation requires the instance of computation 
model that is not immutable, but can be reused over iterations. 
In single`-threaded code, a single instance of such model would 
be used over the whole loop, however in multi`-threaded code, 
each worker thread, plus thread that created CoWork need its 
own instance. CoWorkerResources helps to manage such situation.&]
[s0;%% &]
[s2;%% NOTE: The problem that this helper class solves is in majority 
cases better solved by using CoDo function.&]
[s0;%% &]
[s3;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:CoWorkerResources`:`:CoWorkerResources`(`): [* CoWorkerResources]()&]
[s2;%% Creates a required number of instances so that each sub`-job 
of CoWork has its unique instance.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoWorkerResources`:`:CoWorkerResources`(Upp`:`:Event`<T`&`>`): [* CoWorkerR
esources]([_^Upp`:`:Event^ Event]<[*@4 T][@(0.0.255) `&]>_[*@3 initializer])&]
[s2;%% Creates a required number of instances so that each sub`-job 
of CoWork has its unique instance.and initializes them using 
[%-*@3 initializer].&]
[s3; &]
[s4; &]
[s5;:Upp`:`:CoWorkerResources`:`:GetCount`(`)const: [@(0.0.255) int]_[* GetCount]()_[@(0.0.255) c
onst]&]
[s2;%% Returns the number of instances. Note that this is equal to 
CoWork`::GetPoolSize() if thread that created CoWorkerResources 
is itself CoWork worker, or CoWork`::GetPoolSize() `+ 1 if it 
is any other thread. The reason for this is that CoWork owner 
can execute CoWork jobs too (while waiting in Finish).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoWorkerResources`:`:operator`[`]`(int`): [*@4 T][@(0.0.255) `&]_[* operator`[`]
]([@(0.0.255) int]_[*@3 i])&]
[s2;%% Returns instance [%-*@3 i]. Together with GetCount can be used 
to initialize resources (alternative method to using constructor 
with [%-*@3 initializer].&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:CoWorkerResources`:`:Get`(`): [*@4 T][@(0.0.255) `&]_[* Get]()&]
[s5;:Upp`:`:CoWorkerResources`:`:operator`~`(`): [*@4 T][@(0.0.255) `&]_[* operator`~]()&]
[s2;%% Supposed to be called in CoWork job, returns a reference to 
resources unique for the thread.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:CoWorkerResources`:`:begin`(`): [*@4 T]_`*[* begin]()&]
[s5;:Upp`:`:CoWorkerResources`:`:end`(`): [*@4 T]_`*[* end]()&]
[s2;%% Standard iterator access.&]
[s3; &]
[s0; ]]