/*

12 DEREF DEREF RETIF

LOOP
RETIF

DUP
DROP
SWAP
ROT3
ROT4
*
+
-
/
<<
>>
&
|
&&
||
!
>
<
==
STORE
LOAD
CALL

NEG: 0 SWAP -
NOT: 1 200 STORE

C235: 12 25 ADD DEREF RETZ

C236: 1 10 > RETIF DUP STORE 1 + LOOP

struct Gode {
	WString gode[65535];
	int16   memory[63353];


	void   Read(const char *text);
	String Write();

	bool   Run(int op = 0);
};

bool Run(int code, int& oplimit, int& depth)
{
	wchar *s = gode[code];
	wchar *e = gode[code].End();
	while(s < e) {
		if(
	}
}
*/
