topic "Timer";
[ $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s0; [*R6 Timer]&]
[s0; &]
[s0; [2 Timer queue, usable without CtrlCore.]&]
[s0;2 &]
[s0; [2 Some characteristics:]&]
[s0;2 &]
[s0;i150;O0; [2 The Interface is the same as used from the Ctrl`::TimerCallback 
in CtrlCore]&]
[s0;2 &]
[s0;i150;O0; [2 To use it one does `*not`* need CtrlCore anymore, simply 
Core and Timer, where the application has a Timer instance.]&]
[s0;2 &]
[s0;i150;O0; [2 Should keep in mind, that it`'s a timer `*queue`* other 
things wont get to execution, until a current task is finished, 
and the timer queue can check whether time has come for the next 
task.]&]
[s0;2 &]
[s0;i150;O0; [2 Thus, the timing is not as accurate as maybe desired 
and can have `"timer glitches/jitter`", depending on work load..]&]
[s0;2 &]
[s0;i150;O0; [2 Favorite use is to schedule some `*non`*`-timing`-critical 
work for `"somewhere`-around`-in`-the`-future`", like i.e. observe 
some not too strict timing constraints in custom communication 
protocols (answer needs to come in within X time, keep alive 
messages..)]&]
[s0;2 &]
[s0;i150;O0; [2 For timing critical stuff, consider using OS native 
timer means, for WIN32 SetTimer (in windowing environment, using 
WM`_TIMER callback) or CreateWaitableTimer for windowing`-independent 
stuff, or even QueryPerformanceCounter. for Linux/Unix/Posix 
settimer function, but for Linux there is a lot anyway.]&]
[s0;2 &]
[s0; ]