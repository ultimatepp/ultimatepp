topic "GUI programming and multithreading";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[a83;*R6 $$10,0#31310162474203024125188417583966:caption]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s10; GUI programming and multithreading&]
[s0; Multithreading in GUI programming model has to follow target 
platform limitations, where most target platforms have a notion 
of `"main thread`" and limit GUI actions allowed in non`-main 
threads.&]
[s0; &]
[s0; Everything that is manipulating the status of top`-level windows 
(TopWindow or popups) and everything related to event loop must 
run on the main thread. The only non`-const operation allowed 
to non`-main thread on top`-level windows is Refresh.&]
[s0; &]
[s0; Non`-main threads are in general allowed to alter the status 
of widgets, as long as they serialize access with single global 
GUI mutex, either with EnterGuiMutex/LeaveGuiMutex or with GuiLock 
helper guard class (it calls EnterGuiMutex in constructor, LeaveGuiMutex 
in destructor). GuiLock is reentrant and is optimized for great 
reentrant performance. It is NOT required to use GuiLock in the 
even processing methods (like MouseLeft or Key) as U`+`+ locks 
it in this case itself.&]
[s0; &]
[s0; Other means of communication of non`-main threads with main 
thread is PostCallback, which posts an Event to timer queue for 
immediate processing, but does not wait for it to finish and 
Ctrl`::Call, which passes event to main thread to be processed 
and returns when the event is executed by the main thread. Both 
of these methods have quite high latency, GuiLock and direct 
access should be preferred if possible.&]
[s0; ]]