template <class T, class U>
class X {
public:
	T alfa();
	T beta;
	U u;
	String s;
};

struct Test : X<String, Color>
{
	Test();
};

void Alfa()
{
	Test x;
	x.beta;
	x.u. <<<!!!!
}
