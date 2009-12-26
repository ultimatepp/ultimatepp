#include <Core/Core.h>

using namespace Upp;

int Count(const Vector<String>& v, const String& x)
{
	int count = 0;
	for(int i = 0; i < v.GetCount(); i++)
		if(v[i] == x)
			count++;
	return count;
}

004025A0  push r14 
004025A2  xor r14d,r14d 
004025A5  push r13 
004025A7  push r12 
004025A9  push rbp 
004025AA  mov rbp,rdi 
004025AD  push rbx 
004025AE  mov eax,dword ptr [rdi+0x8] 
004025B1  mov rbx,rsi 
004025B4  test eax,eax 
004025B6  jle 0x402611 
004025B8  xor r13d,r13d 
004025BB  xor r12d,r12d 
004025BE  jmp 0x4025EC 

004025C0  mov rax,Qword ptr [rbx] 
004025C3  mov rdx,Qword ptr [rbx+0x8] 
004025C7  xor rax,Qword ptr [rdi] 
004025CA  xor rdx,Qword ptr [rdi+0x8] 
004025CE  or rax,rdx 
004025D1  sete al 
004025D4  movzx eax,al 
004025D7  cmp eax,0x1 
004025DA  sbb r14d,0xFFFFFFFF 
004025DE  add r13d,0x1 
004025E2  add r12,0x10 
004025E6  cmp dword ptr [rbp+0x8],r13d 
004025EA  jle 0x402611 
004025EC  mov rdi,r12 
004025EF  add rdi,Qword ptr [rbp+0x0] 
004025F3  movsx eax,byte ptr [rbx+0xE] 
004025F7  movsx edx,byte ptr [rdi+0xE] 
004025FB  or edx,eax 
004025FD  je 0x4025C0 

004025FF  mov rsi,rbx 
00402602  call Upp::String0::LEqual(Upp::String0 const&) const 
00402607  test eax,eax 
00402609  sete al 
0040260C  movzx eax,al 
0040260F  jmp 0x4025D7 

0040261C  mov eax,r14d 
0040261F  pop rbx 
00402620  pop rbp 
00402621  pop r12 
00402623  pop r13 
00402625  pop r14 
00402627  ret  

*/

For your pleasure, I have tried a little exercise to demonstrate my point. Consider this routine:<p>
<code>
int Count(const Vector<String>& v, const String& x) {<p>
     int count = 0;<p>
     for(int i = 0; i &lt; v.GetCount(); i++) <p>
        if(v[i] == x)<p>
            count++;<p>
     return count;<p>
}<p>
</code>

This involves high level objects, implemented at library level, Vector and String. There are 3 semantic calls .GetCount, Vector::operator[] and String::operator==.

It gets compiled into this piece of code:

<code>
004025A0  push r14 <br>
004025A2  xor r14d,r14d <br>
004025A5  push r13 <br>
004025A7  push r12 <br>
004025A9  push rbp <br>
004025AA  mov rbp,rdi <br>
004025AD  push rbx <br>
004025AE  mov eax,dword ptr [rdi+0x8] <br>
004025B1  mov rbx,rsi <br>
004025B4  test eax,eax <br>
004025B6  jle 0x402611 <br>
004025B8  xor r13d,r13d <br>
004025BB  xor r12d,r12d <br>
004025BE  jmp 0x4025EC <br>

004025C0  mov rax,Qword ptr [rbx] <br>
004025C3  mov rdx,Qword ptr [rbx+0x8] <br> 
004025C7  xor rax,Qword ptr [rdi] <br>
004025CA  xor rdx,Qword ptr [rdi+0x8] <br> 
004025CE  or rax,rdx <br>
004025D1  sete al <br>
004025D4  movzx eax,al <br>
004025D7  cmp eax,0x1 <br>
004025DA  sbb r14d,0xFFFFFFFF <br>
004025DE  add r13d,0x1 <br>
004025E2  add r12,0x10 <br>
004025E6  cmp dword ptr [rbp+0x8],r13d <br>
004025EA  jle 0x402611 <br>
004025EC  mov rdi,r12 <br>
004025EF  add rdi,Qword ptr [rbp+0x0] <br>
004025F3  movsx eax,byte ptr [rbx+0xE] <br>
004025F7  movsx edx,byte ptr [rdi+0xE] <br>
004025FB  or edx,eax <br>
004025FD  je 0x4025C0 <br>

004025FF  mov rsi,rbx <br>
00402602  call Upp::String0::LEqual(Upp::String0 const&) const <br>
00402607  test eax,eax <br>
00402609  sete al <br>
0040260C  movzx eax,al <br>
0040260F  jmp 0x4025D7 <br>

0040261C  mov eax,r14d <br>
0040261F  pop rbx <br>
00402620  pop rbp <br>
00402621  pop r12 <br>
00402623  pop r13 <br>
00402625  pop r14 <br>
00402627  ret  <br>
</code>

Now the fast path of loop gets compressed into opcodes at the range 004025C0 to 004025FD - 64 bytes total. GetCount gets completely out of loop, operator[] is reordered to direct pointer increments (at 4025E2) and unless slow-path is necessarry (004025FF - 0040260F), there are no calls at all. (Note: slow-path is invoked is Strings do not satisfy SSO).
