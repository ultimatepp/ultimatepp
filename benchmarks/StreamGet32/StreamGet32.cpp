#include <Core/Core.h>

using namespace Upp;

dword qqq;

CONSOLE_APP_MAIN
{
	String s;
	for(int i = 0; i < 1000000; i++)
		s.Cat(i);

	for(int i = 0; i < 1000; i++) {
		StringStream ss(s);
		RTIMING("Time");
		int q = 0;
		while(!ss.IsEof())
			q += ss.Get32();
		qqq += q;
	}
}


400015E0  mov eax,DWORD PTR [rcx]
400015E2  add rcx,0x4
400015E6  mov QWORD PTR [rsp+0x48],rcx
400015EB  add r13d,eax
400015EE  mov rcx,QWORD PTR [rsp+0x48]
400015F3  mov rax,QWORD PTR [rsp+0x50]
400015F8  cmp rcx,rax
400015FB  jb 0x140001616 <ConsoleMainFn_()+310>

400015FD  mov rax,QWORD PTR [rsp+0x30]
40001602  mov rcx,rbx
40001605  call QWORD PTR [rax+0x8]
40001608  test eax,eax
4000160A  js 0x140001630 <ConsoleMainFn_()+336>

4000160C  mov rcx,QWORD PTR [rsp+0x48]
40001611  mov rax,QWORD PTR [rsp+0x50]
40001616  lea rdx,[rcx+0x3]
4000161A  cmp rdx,rax
4000161D  jb 0x1400015e0 <ConsoleMainFn_()+256>

4000161F  mov rcx,rbx
40001622  call 0x140008c20 <Upp::Stream::_Get32()>
40001627  jmp 0x1400015eb <ConsoleMainFn_()+267>
40001629  nop DWORD PTR [rax+0x0]
40001630  add DWORD PTR [rip+0x152e71],r13d        # 0x1401544a8 <qqq>
40001637  mov eax,DWORD PTR [rip+0x153bdf]        # 0x14015521c
4000163D  mov rcx,QWORD PTR gs:0x58
40001646  mov rax,QWORD PTR [rcx+rax*8]
4000164A  mov r8d,DWORD PTR [rax+0x8]
40001651  add r8d,0xffffffff
40001655  mov DWORD PTR [rax+0x8],r8d
4000165C  mov rcx,rdi
4000165F  mov edx,r15d
40001662  call 0x14000f900 <Upp::TimingInspector::Add(unsigned long, int)>
40001667  mov QWORD PTR [rsp+0x30],rbp
4000166C  lea rax,[rsp+0x90]
40001674  cmp QWORD PTR [rsp+0x78],rax
40001679  je 0x140001685 <ConsoleMainFn_()+421>
4000167B  lea rcx,[rsp+0x78]
40001680  call 0x140002d90 <Upp::StringBuffer::Free()>
40001685  cmp BYTE PTR [rsp+0x76],0x0
4000168A  je 0x140001580 <ConsoleMainFn_()+160>
40001690  lea rcx,[rsp+0x68]
40001695  call 0x140001960 <Upp::String0::LFree()>
4000169A  jmp 0x140001580 <ConsoleMainFn_()+160>
4000169F  lea rcx,[rip+0x152e6a]        # 0x140154510 <guard variable for ConsoleMainFn_()::sTmStat15>
400016A6  call 0x1400111d0 <__cxa_guard_acquire>
400016AB  test eax,eax
400016AD  je 0x1400015b2 <ConsoleMainFn_()+210>
400016B3  mov rcx,rdi
400016B6  lea rdx,[rip+0x1029db]        # 0x140104098 <.refptr.mingw_initltssuo_force+8>
400016BD  call 0x14000eb30 <Upp::TimingInspector::TimingInspector(char const*)>
400016C2  lea rcx,[rip+0x137]        # 0x140001800 <__dtor__ZZ14ConsoleMainFn_vE9sTmStat15()>
400016C9  call 0x140001470 <atexit>
400016CE  lea rcx,[rip+0x152e3b]        # 0x140154510 <guard variable for ConsoleMainFn_()::sTmStat15>
400016D5  call 0x1400112b0 <__cxa_guard_release>
400016DA  jmp 0x1400015b2 <ConsoleMainFn_()+210>
400016DF  cmp BYTE PTR [rsp+0x2e],0x0
400016E4  je 0x1400016f0 <ConsoleMainFn_()+528>
400016E6  lea rcx,[rsp+0x20]
400016EB  call 0x140001960 <Upp::String0::LFree()>
400016F0  nop 
400016F1  add rsp,0x198
400016F8  pop rbx
400016F9  pop rbp
400016FA  pop rdi
400016FB  pop rsi
400016FC  pop r12
400016FE  pop r13
40001700  pop r14
40001702  pop r15
40001704  ret 
40001705  mov rsi,rax
40001708  lea rcx,[rip+0x152e01]        # 0x140154510 <guard variable for ConsoleMainFn_()::sTmStat15>
4000170F  call 0x140011350 <__cxa_guard_abort>
40001714  jmp 0x14000177f <ConsoleMainFn_()+671>
40001716  mov rcx,rax
40001719  call 0x1400018d0 <__clang_call_terminate>
4000171E  mov rsi,rax
40001721  jmp 0x1400017b8 <ConsoleMainFn_()+728>
40001726  mov rcx,rax
40001729  call 0x1400018d0 <__clang_call_terminate>
4000172E  mov rcx,rax
40001731  call 0x1400018d0 <__clang_call_terminate>
40001736  mov rcx,rax
40001739  call 0x1400018d0 <__clang_call_terminate>
4000173E  mov rsi,rax
40001741  jmp 0x14000177f <ConsoleMainFn_()+671>
40001743  mov rsi,rax
40001746  jmp 0x1400017b8 <ConsoleMainFn_()+728>
40001748  mov rsi,rax
4000174B  mov eax,DWORD PTR [rip+0x153acb]        # 0x14015521c
40001751  mov rcx,QWORD PTR gs:0x58
4000175A  mov rax,QWORD PTR [rcx+rax*8]
4000175E  mov r8d,DWORD PTR [rax+0x8]
40001765  add r8d,0xffffffff
40001769  mov DWORD PTR [rax+0x8],r8d
40001770  lea rcx,[rip+0x152d39]        # 0x1401544b0 <ConsoleMainFn_()::sTmStat15>
40001777  mov edx,r15d
4000177A  call 0x14000f900 <Upp::TimingInspector::Add(unsigned long, int)>
4000177F  mov QWORD PTR [rsp+0x30],rbp
40001784  lea rax,[rsp+0x90]
4000178C  cmp QWORD PTR [rsp+0x78],rax
40001791  je 0x14000179d <ConsoleMainFn_()+701>
40001793  lea rcx,[rsp+0x78]
40001798  call 0x140002d90 <Upp::StringBuffer::Free()>
4000179D  cmp BYTE PTR [rsp+0x76],0x0
400017A2  je 0x1400017ae <ConsoleMainFn_()+718>
400017A4  lea rcx,[rsp+0x68]
400017A9  call 0x140001960 <Upp::String0::LFree()>
400017AE  lea rcx,[rsp+0x30]
400017B3  call 0x14000e2d0 <Upp::Stream::~Stream()>
400017B8  cmp BYTE PTR [rsp+0x2e],0x0
400017BD  je 0x1400017c9 <ConsoleMainFn_()+745>
400017BF  lea rcx,[rsp+0x20]
400017C4  call 0x140001960 <Upp::String0::LFree()>
400017C9  mov rcx,rsi
400017CC  call 0x140013590 <_Unwind_Resume>
400017D1  mov rcx,rax
400017D4  call 0x1400018d0 <__clang_call_terminate>
400017D9  mov rcx,rax
400017DC  call 0x1400018d0 <__clang_call_terminate>
400017E1  mov rcx,rax
400017E4  call 0x1400018d0 <__clang_call_terminate>
400017E9  mov rcx,rax
400017EC  call 0x1400018d0 <__clang_call_terminate>
400017F1  int3 
