#include "ScatterCtrl_Demo.h"

void Tab11::Init()
{
	CtrlLayout(*this);	
	HSizePos().VSizePos();
	scatter.SetRange(5, 1500).SetMajorUnits(1, 250).SetXYMin(-3, 0);
	scatter.SetMouseHandling(true, true).ShowContextMenu().SetLegendWidth(160);
	
	sx << -3.067E0 << -2.981E0 << -2.921E0 << -2.912E0 << -2.840E0 << -2.797E0 << -2.702E0 << -2.699E0 << -2.633E0 << -2.481E0 << -2.363E0 << -2.322E0 << -1.501E0 << -1.460E0 << -1.274E0 << -1.212E0 << -1.100E0 << -1.046E0 << -0.915E0 << -0.714E0 << -0.566E0 << -0.545E0 << -0.400E0 << -0.309E0 << -0.109E0 << -0.103E0 << 0.010E0 << 0.119E0 << 0.377E0 << 0.790E0 << 0.963E0 << 1.006E0 << 1.115E0 << 1.572E0 << 1.841E0 << 2.047E0 << 2.200E0;
	sy << 80.574E0 << 84.248E0 << 87.264E0 << 87.195E0 << 89.076E0 << 89.608E0 << 89.868E0 << 90.101E0 << 92.405E0 << 95.854E0 << 100.696E0 << 101.060E0 << 401.672E0 << 390.724E0 << 567.534E0 << 635.316E0 << 733.054E0 << 759.087E0 << 894.206E0 << 990.785E0 << 1090.109E0 << 1080.914E0 << 1122.643E0 << 1178.351E0 << 1260.531E0 << 1273.514E0 << 1288.339E0 << 1327.543E0 << 1353.863E0 << 1414.509E0 << 1425.208E0 << 1421.384E0 << 1442.962E0 << 1464.350E0 << 1468.705E0 << 1447.894E0 << 1457.628E0;		
		
	VectorDouble vs1(sy, sx);
	double l_r2, p2_r2, p4_r2, f_r2;
	
	linear.Fit(vs1, l_r2);
	poly2.SetDegree(2);
	poly2.Fit(vs1, p2_r2);
	poly4.SetDegree(4);
	poly4.Fit(vs1, p4_r2);
	fourier.SetDegree(3);
	fourier.Fit(vs1, f_r2);
	
	scatter.AddSeries(sx, sy).Legend("Series").MarkStyle<RhombMarkPlot>().MarkWidth(10).NoPlot();
	scatter.AddSeries(linear).Legend(linear.GetFullName()).NoMark().Stroke(2);
	scatter.AddSeries(poly2).Legend(poly2.GetFullName()).NoMark().Stroke(2);
	scatter.AddSeries(poly4).Legend(poly4.GetFullName()).NoMark().Stroke(2);
	scatter.AddSeries(fourier).Legend(fourier.GetFullName()).NoMark().Stroke(2);
	
	grid.AddColumn("Type", 10);
	grid.AddColumn("Equation", 40);
	grid.AddColumn("R2", 5);
	
	grid.Add(linear.GetFullName(), 	linear.GetEquation(),  FormatDoubleFix(l_r2, 5));
	grid.Add(poly2.GetFullName(), 	poly2.GetEquation(),   FormatDoubleFix(p2_r2, 5));
	grid.Add(poly4.GetFullName(), 	poly4.GetEquation(),   FormatDoubleFix(p4_r2, 5));
	grid.Add(fourier.GetFullName(), fourier.GetEquation(), FormatDoubleFix(f_r2, 5));
}

ScatterDemo *Construct11()
{
	static Tab11 tab;
	return &tab;
}

INITBLOCK {
	RegisterExample(t_("Trend Lines"), Construct11, __FILE__);
}