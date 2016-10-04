#include <CtrlLib/CtrlLib.h>

struct Foo {
	int a, b;
};

void Add(Foo& x, int q) { x.a += q; x.b += q; }

void Print(int);

GUI_APP_MAIN
{
	__BREAK__;
	Foo x;
	x.a = 10;
	x.b = 20;
	Add(x, rand());
	Print(x.a);
	Print(x.b);
}

/*
> Inlining a function doesn't necessarily imply replacing a reference type
> with a value type.
>
> The problem is, a compiler looks at a UDT from the lowest level
> possible. It has *no idea* what that type is supposed to be; not a clue.
> All it sees are the low level operations on it. Trying to infer
> identities and high level operations from that is akin to handing an AI
> engine a few identities and expecting it to construct all kinds of
> useful theorems from them. It ain't gonna happen.

I am afraid my response might be considered a little bit offending, but I have to ask:

Are we speaking about DMC++ here? :)

In reality, I see no reason why good compiler could not optimize out a reference.

Mirek
*/