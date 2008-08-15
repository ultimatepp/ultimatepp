#include "TestScatter.h"

#include <Report/Report.h>
#include <Draw/Draw.h>
#include <plugin/jpg/jpg.h>


double funct1(double x){return (3*x-x*x);}
double funct2(double x){return (3*x-x*x+6);}

double funct3(double x){return (1.5*x-2);}
double funct4(double x){return (10*sin(3.14*x/10)+10);}
double funct5(double x){return (10*cos(3.14*x/10)+10);}
XY para1(double t) {return XY(cos(2*3.1415*t),sin(2*3.1415*t));}
XY para2(double t) {return XY(0.5*cos(6*3.1415*t)+3*t-1, 1.5+0.5*sin(6*3.1415*t));}
XY para3(double t) {return XY(3+(0.5+t)*cos(6*3.1415*t),(0.5+t)*sin(6*3.1415*t));}
void TestScatter::ModifPoint()
{
	myChart1.SetData(0,2,XY(15,40));//(series number, point index, point)
}
void TestScatter::AddPoint()
{
	myChart1.AddPoint(0,XY(78,10));//(series number, point)
}
void TestScatter::InsertPoint()
{
	myChart1.InsertPoint(0,1,XY(7,31.5));
}
void TestScatter::RemovePoint()
{
	myChart1.RemovePoint(0,1);
}
void TestScatter::ChangeColor()
{
	myChart1.SetDataColor(1,b7.GetData());
}
void TestScatter::ChangeMarkColor()
{
	myChart1.SetMarkColor(1,b9.GetData());
}
bool join=false;
void TestScatter::Join()
{
	myChart1.SetJoin(0,join);
	join=!join;
}
void TestScatter::RemoveSeries()
{
	myChart1.RemoveSeries(0);
}
void TestScatter::RemoveFunct()
{
	myChart1.RemoveFSeries(0);
}
bool z=true;
void TestScatter::Zoom()
{
	if(z)
	{
		myChart1.SetRange(6,5);
		myChart1.SetXYMin(29,0);
	}
	else
	{
		myChart1.SetRange(80,50);
		myChart1.SetXYMin(0,0);
		myChart1.SetMajorUnits(10,10);
	}
	z=!z;
	myChart1.Refresh();
}
void TestScatter::SaveJPG()
{
	JPGEncoder encoder(90);
	encoder.SaveFile("myChart1.jpg",myChart1.getImage(1));
}
void TestScatter::SavePNG()
{
	PNGEncoder encoder;
	encoder.SaveFile("myChart1.png",myChart1.getImage(6));
}
void TestScatter::Preview()
{
	Report r;
	r.DrawRect(295,295,myChart1.getDrawing().GetSize().cx+10,myChart1.getDrawing().GetSize().cy+10,Gray);
	r.DrawDrawing(300,300,myChart1.getDrawing().GetSize().cx,myChart1.getDrawing().GetSize().cy, myChart1.getDrawing());

	Perform(r);
}

TestScatter::TestScatter()
{
	CtrlLayout(*this, "Test Scatter");
	Sizeable();


	b1 <<= THISBACK(ModifPoint);
	b2 <<= THISBACK(AddPoint);
	b3 <<= THISBACK(InsertPoint);
	b4 <<= THISBACK(RemovePoint);
	b5 <<= THISBACK(RemoveSeries);
	b6 <<= THISBACK(RemoveFunct);
	b7 <<= THISBACK(ChangeColor);
	b9 <<= THISBACK(ChangeMarkColor);
	b8 <<= THISBACK(Join);
	b10<<= THISBACK(Zoom);

	b18 <<= THISBACK(SaveJPG);
	b19 <<= THISBACK(SavePNG);
	b20 <<= THISBACK(Preview);


	myChart1.SetRange(80,50);
	myChart1.SetMajorUnits(10,10);
	//myChart1.SetLabels("Time [s]","Amplitude [m]");

	Vector<XY> d1,d2,d3,d4;

	d1<<XY(5,25)<<XY(10,25)<<XY(15,25)<<XY(20,24.3)<<XY(25,31.2);
	d1<<XY(30,21.4)<<XY(35,29.4)<<XY(40,20)<<XY(45,28.3)<<XY(50,33.2);
	d1<<XY(55,23.4)<<XY(60,26.4)<<XY(65,30)<<XY(70,30)<<XY(75,34.2)<<XY(75,20)<<XY(50,15);
	myChart1.AddSeries(d1,"Series1",true,LtBlue,42,6);
	myChart1.SetMarkStyle(0,Scatter::RHOMB);
	myChart1.SetMarkColor(0,Black);


	d2<<XY(5,34.4)<<XY(10,48.4)<<XY(15,37.6)<<XY(20,44.3)<<XY(25,41.2);
	d2<<XY(30,31.4)<<XY(35,44.4)<<XY(40,33.6)<<XY(45,46.3)<<XY(50,38.2);
	d2<<XY(55,33.4)<<XY(60,46.4)<<XY(65,48.5)<<XY(70,47.3)<<XY(75,44.2);
	myChart1.AddSeries(d2,"Series2",true,LtRed,40,12);
	myChart1.SetMarkStyle(1,1);
	myChart1.SetSmooth(0,true);
	d3<<XY(10,40)<<XY(30,35);
	d4<<XY(20,36);
	myChart1.AddSeries(d3,"Series3",false,Cyan,30,0);
	myChart1.AddSeries(d4,"Series4",true,Black,30,6);
	myChart1.plotFunction(funct4,"f4");
	myChart1.plotFunction(funct5,"f5",Magenta);

	myChart2.SetRange(7,4);
	myChart2.SetMajorUnits(0.5,0.5);
	myChart2.SetXYMin(-2,-2);

	//myChart2.plotFunction(funct1,"3X-X^2",class::Color(28,85,255),12);
	//myChart2.plotFunction(funct2,"6+3X+X^2",LtRed);
	//myChart2.plotFunction(funct3,"-2+1.5X",LtGreen,12);
	myChart2.plotParaFunction(para1,"Circle",Yellow,6);
	myChart2.plotParaFunction(para2,"Coil",White,6,100);
	myChart2.plotParaFunction(para3,"Spiral",Color::Color(56,170,255),6,100);
	b7.SetData(myChart1.GetDataColor(1));
	b7.Info("Line Color");
	b9.Info("Mark Color");
	b9.SetData(myChart1.GetMarkColor(1));
	ActiveFocus(b1);


}


GUI_APP_MAIN
{
	TestScatter test;
	test.Run();
}
