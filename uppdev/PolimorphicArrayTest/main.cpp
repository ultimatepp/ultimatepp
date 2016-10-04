#include <CtrlLib/CtrlLib.h>
using namespace Upp;

struct Shape
{
	virtual void Description() {}
	virtual ~Shape() {}
};

struct Line : public Shape
{
	void Description() 
	{
		Cout()<<"Line\n";
	}
};

struct Pict : public Shape
{
	Image img;
	Pict(Image image): img(image){}
	void Description() 
	{
		Cout()<<"Picture\n";
	}
};
	
CONSOLE_APP_MAIN
{
	Array <Shape> array;
	array.Add(new Line());
	array.Add(new Pict(Image::Arrow()));
	array[0].Description();
	array[1].Description();
}
