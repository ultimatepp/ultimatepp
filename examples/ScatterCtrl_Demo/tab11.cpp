#include "ScatterCtrl_Demo.h"

void Tab11::Init()
{
	CtrlLayout(*this);	
	SizePos();
	
	seriesList.Add("Series 1").Add("Series 2");
	seriesList = "Series 1";
	seriesList.WhenAction = THISBACK(OnSeries);
	
	scatter.SetMouseHandling(true, true).ShowContextMenu();
	
	grid.AddColumn("Type", 10);
	grid.AddColumn("Equation", 40);
	grid.AddColumn("R2", 5);
	
	OnSeries();
}

void Tab11::OnSeries() {
	double l_r2, p2_r2, p4_r2, f_r2, exp_r2, rat1_r2, s_r2;
	
	scatter.RemoveAllSeries();
	grid.Clear();
	if (seriesList == "Series 1") {
		scatter.SetRange(5, 1500).SetMajorUnits(1, 250).SetXYMin(-3, 0);
		s1x.Clear();
		s1y.Clear();
		s1x << -3.067E0 << -2.981E0 << -2.921E0 << -2.912E0 << -2.840E0 << -2.797E0 << -2.702E0 << -2.699E0 << -2.633E0 << -2.481E0 << -2.363E0  << -2.322E0  << -1.501E0  << -1.460E0  << -1.274E0  << -1.212E0  << -1.100E0  << -1.046E0 << -0.915E0 << -0.714E0 << -0.566E0 << -0.545E0 << -0.400E0 << -0.309E0 << -0.109E0 << -0.103E0 << 0.010E0 << 0.119E0 << 0.377E0 << 0.790E0 << 0.963E0 << 1.006E0 << 1.115E0 << 1.572E0 << 1.841E0 << 2.047E0 << 2.200E0;
		s1y << 80.574E0 << 84.248E0 << 87.264E0 << 87.195E0 << 89.076E0 << 89.608E0 << 89.868E0 << 90.101E0 << 92.405E0 << 95.854E0 << 100.696E0 << 101.060E0 << 401.672E0 << 390.724E0 << 567.534E0 << 635.316E0 << 733.054E0 << 759.087E0 << 894.206E0 << 990.785E0 << 1090.109E0 << 1080.914E0 << 1122.643E0 << 1178.351E0 << 1260.531E0 << 1273.514E0 << 1288.339E0 << 1327.543E0 << 1353.863E0 << 1414.509E0 << 1425.208E0 << 1421.384E0 << 1442.962E0 << 1464.350E0 << 1468.705E0 << 1447.894E0 << 1457.628E0;		
		
		VectorDouble vs1(s1y, s1x);
		
		linear.Fit(vs1, l_r2);
		poly2.SetDegree(2);
		poly2.Fit(vs1, p2_r2);
		poly4.SetDegree(4);
		poly4.Fit(vs1, p4_r2);
		fourier.SetDegree(3);
		fourier.Fit(vs1, f_r2);
		exponential.Fit(vs1, exp_r2);
		rational1.Fit(vs1, rat1_r2);
		sin.GuessCoeff(vs1);		
		sin.Fit(vs1, s_r2);	
		
		scatter.AddSeries(s1x, s1y).Legend("Series 1").MarkStyle<RhombMarkPlot>().MarkWidth(10).NoPlot();
	} else if (seriesList == "Series 2") {
		s2x.Clear();
		s2y.Clear();
		s2y << -0.9 << -1.1	<< 2.8 << -3.4 << 2.7 << -0.9 << -1.1 << 2.8 << -3.4 << 2.7 << -0.9 << -1.1 << 2.8 << -3.4 << 2.7 << -0.7 << -1.2;
		s2x << 0 << 0.6 << 1.2 << 1.8 << 2.5 << 3.1 << 3.7 << 4.3 << 5.0 << 5.6 << 6.2 << 6.9 << 7.5 << 8.1 << 8.7 << 9.4 << 10.0;
		
		VectorDouble v2(s2y, s2x);

		linear.Fit(v2, l_r2);
		poly2.SetDegree(2);
		poly2.Fit(v2, p2_r2);
		poly4.SetDegree(4);
		poly4.Fit(v2, p4_r2);
		fourier.SetDegree(3);
		fourier.Fit(v2, f_r2);
		exponential.Fit(v2, exp_r2);
		rational1.Fit(v2, rat1_r2);	
		sin.GuessCoeff(v2);	
		sin.Fit(v2, s_r2);	
				
		scatter.AddSeries(s2x, s2y).Legend("Series 2").MarkStyle<RhombMarkPlot>().MarkWidth(10).NoPlot();
	}
	if (l_r2 > 0.5)
		scatter.AddSeries(linear).Legend(linear.GetFullName()).NoMark().Stroke(2);
	if (p2_r2 > 0.5)
		scatter.AddSeries(poly2).Legend(poly2.GetFullName()).NoMark().Stroke(2);
	if (p4_r2 > 0.5)	
		scatter.AddSeries(poly4).Legend(poly4.GetFullName()).NoMark().Stroke(2);
	if (f_r2 > 0.5)	
		scatter.AddSeries(fourier).Legend(fourier.GetFullName()).NoMark().Stroke(2);
	if (exp_r2 > 0.5)
		scatter.AddSeries(exponential).Legend(exponential.GetFullName()).NoMark().Stroke(2);
	if (rat1_r2 > 0.5)
		scatter.AddSeries(rational1).Legend(rational1.GetFullName()).NoMark().Stroke(2);
	if (s_r2 > 0.5)	
		scatter.AddSeries(sin).Legend(sin.GetFullName()).NoMark().Stroke(2);
	scatter.FitToData(true);
	scatter.Refresh();
	
	grid.Add(linear.GetFullName(), 	linear.GetEquation(),  FormatDoubleFix(l_r2, 5));
	grid.Add(poly2.GetFullName(), 	poly2.GetEquation(),   FormatDoubleFix(p2_r2, 5));
	grid.Add(poly4.GetFullName(), 	poly4.GetEquation(),   FormatDoubleFix(p4_r2, 5));
	grid.Add(fourier.GetFullName(), fourier.GetEquation(), FormatDoubleFix(f_r2, 5));
	grid.Add(exponential.GetFullName(), exponential.GetEquation(), FormatDoubleFix(exp_r2, 5));
	grid.Add(rational1.GetFullName(), rational1.GetEquation(), FormatDoubleFix(rat1_r2, 5));
	grid.Add(sin.GetFullName(), sin.GetEquation(), FormatDoubleFix(s_r2, 5));
	grid.SetSortColumn(2, true);
}

ScatterDemo *Construct11()
{
	static Tab11 tab;
	return &tab;
}

INITBLOCK {
	RegisterExample(t_("Trend Lines"), Construct11, __FILE__);
}