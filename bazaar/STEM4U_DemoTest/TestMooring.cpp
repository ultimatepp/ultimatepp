#include <Core/Core.h>
#include <ScatterDraw/ScatterDraw.h>
#include <plugin/Eigen/Eigen.h>
#include <Functions4U/Functions4U.h>
#include <STEM4U/Mooring.h>
#include <plugin/png/png.h>


using namespace Upp;


void TestMooring(bool test) {
	UppLog() << "\n\nCatenary mooring demo";
	
	double rho_m = 346.7;		// Kg/m   
	double rho_m3 = 7850;
	double rho_water = 1025;
	
	double BL = 1.49E9;			// Mooring line break load
	double moorlen = 490;
	
	double xanchorvessel = 480;	
	double zanchor = 0;
	double zvessel = 54;

	double Fhanchorvessel, Fvanchor, Fvvessel, xonfloor;
	Vector<double> x, z;
	MooringStatus status;
	
	Catenary(moorlen, xanchorvessel, zanchor, zvessel, xonfloor);

	double moorlen2;
	CatenaryGetLen(xonfloor, xanchorvessel, zanchor, zvessel, moorlen2);
	VERIFY(EqualDecimals(moorlen, moorlen2, 4));

	CatenaryGetLen(0.1*xanchorvessel, xanchorvessel, 0, zvessel, moorlen2);
	VERIFY(EqualDecimals(484.4677, moorlen2, 4));
	CatenaryGetLen(0.15*xanchorvessel, xanchorvessel, 0, zvessel, moorlen2);
	VERIFY(EqualDecimals(484.7264, moorlen2, 4));
	CatenaryGetLen(0.2*xanchorvessel, xanchorvessel, 0, zvessel, moorlen2);
	VERIFY(EqualDecimals(485.0166, moorlen2, 4));
	CatenaryGetLen(0.25*xanchorvessel, xanchorvessel, 0, zvessel, moorlen2);
	VERIFY(EqualDecimals(485.3445, moorlen2, 4));
	CatenaryGetLen(0.3*xanchorvessel, xanchorvessel, 0, zvessel, moorlen2);
	VERIFY(EqualDecimals(485.7176, moorlen2, 4));
	
	status = Catenary(rho_m, rho_m3, rho_water, moorlen, BL, xanchorvessel, zanchor, zvessel, Fhanchorvessel, Fvanchor, Fvvessel, xonfloor, x, z, 10);
	UppLog() << "\nStatus is: " << MooringStatusStr(status);
	VERIFY(status == CATENARY_ON_FLOOR);
	VERIFY(EqualDecimals(xonfloor, 292.5933, 4));
	VERIFY(EqualDecimals(Fhanchorvessel, 987138.9765, 4));
	VERIFY(EqualDecimals(Fvvessel, 583737.6409, 4));
	
	double Fanchor = sqrt(sqr(Fhanchorvessel) + sqr(Fvanchor));
	double anganchor = atan2(Fvanchor, Fhanchorvessel)*180./M_PI;
	double Fvessel = sqrt(sqr(Fhanchorvessel) + sqr(Fvvessel));
	double angvessel = atan2(Fvvessel, Fhanchorvessel)*180./M_PI;
	UppLog() << Format("\nFhanchorvessel=%.2f, Fvanchor=%.2f, Fanchor=%.2f, ang_anchor=%.1f grad\nFvvessel=%.2f, Fvessel=%.2f, ang_vessel=%.1f, xonfloor=%.1f", 
		Fhanchorvessel, -Fvanchor, Fanchor, anganchor, -Fvvessel, Fvessel, angvessel, xonfloor);
	VERIFY(FormatDouble(angvessel, 4) == "30.5976");
		
	UppLog() << "\nX\tY";
	for (int i = 0; i < x.size(); ++i) 
		UppLog() << Format("\n%.2f\t%.4f", x[i], z[i]);
	VERIFY(FormatDouble(z[7], 4) == "9.8116");
	
	if (!test) {
		int num = 100;
		Vector<Vector<double>> xx(5), zz(5);
		String dir = AppendFileName(GetDesktopFolder(), "STEM4U_Demo");
		RealizeDirectory(dir);
		ScatterDraw scatter;
		scatter.SetLabelX("Distance from anchor to vessel [m]").
				SetLabelY("Height from floor to vessel [m]").
				SetLeftMargin(70).SetBottomMargin(50).SetSize(Size(1000, 400)).
				SetLegendAnchor(ScatterDraw::LEFT_TOP);
		{
			scatter.RemoveAllSeries().SetTitle("Shifting the vessel to the anchor");
			for (int iz = 0; iz < 5; ++iz) {
				Catenary(rho_m, rho_m3, rho_water, moorlen, BL, 
					xanchorvessel-iz*15, zanchor, zvessel, Fhanchorvessel, Fvanchor, Fvvessel, 
					xonfloor, xx[iz], zz[iz], num);
				String legend = Format("Xanchorvessel:%.0f, Fx:%.0f, Fzanchor:%.0f, Fzvessel:%.0f, xonfloor:%.1f", 
						xanchorvessel-iz*15, Fhanchorvessel/1000., Fvanchor/1000., Fvvessel/1000., xonfloor);
				scatter.AddSeries(xx[iz], zz[iz]).Legend(legend).NoMark();
				scatter.AddSeries(&zz[iz][0], 1, xx[iz][0], 0).NoSeriesLegend().MarkStyle<CircleMarkPlot>().MarkWidth(15);
				scatter.AddSeries(&zz[iz].Top(), 1, xx[iz].Top(), 0).NoSeriesLegend().MarkStyle<CircleMarkPlot>().MarkWidth(15);
			}
			scatter.ZoomToFit(true, true);
			PNGEncoder().SaveFile(AppendFileName(dir, "Moor xanchorvessel.png"), scatter.GetImage());
		}
		{
			scatter.RemoveAllSeries().SetTitle("Raising the vessel");
			for (int iz = 0; iz < 5; iz++) {
				Catenary(rho_m, rho_m3, rho_water, moorlen, BL, 
					xanchorvessel, zanchor, zvessel+iz*10, Fhanchorvessel, Fvanchor, Fvvessel, 
					xonfloor, xx[iz], zz[iz], num);
				String legend = Format("Zvessel:%.0f, Fx:%.0f, Fzanchor:%.0f, Fzvessel:%.0f, xonfloor:%.1f", 
						zvessel+iz*10, Fhanchorvessel/1000., Fvanchor/1000., Fvvessel/1000., xonfloor);
				scatter.AddSeries(xx[iz], zz[iz]).Legend(legend).NoMark();
				scatter.AddSeries(&zz[iz][0], 1, xx[iz][0], 0).NoSeriesLegend().MarkStyle<CircleMarkPlot>().MarkWidth(15);
				scatter.AddSeries(&zz[iz].Top(), 1, xx[iz].Top(), 0).NoSeriesLegend().MarkStyle<CircleMarkPlot>().MarkWidth(15);
			}
			scatter.ZoomToFit(true, true);
			PNGEncoder().SaveFile(AppendFileName(dir, "Moor zvessel.png"), scatter.GetImage());
		}
		{
			scatter.RemoveAllSeries().SetTitle("Raising the anchor");
			for (int iz = 0; iz < 5; iz++) {
				Catenary(rho_m, rho_m3, rho_water, moorlen, BL, 
					xanchorvessel, zanchor+iz*30, zvessel, Fhanchorvessel, Fvanchor, Fvvessel, 
					xonfloor, xx[iz], zz[iz], num);
				String legend = Format("Zanchor:%.0f, Fx:%.0f, Fzanchor:%.0f, Fzvessel:%.0f, xonfloor:%.1f", 
						zanchor+iz*30, Fhanchorvessel/1000., Fvanchor/1000., Fvvessel/1000., xonfloor);
				scatter.AddSeries(xx[iz], zz[iz]).Legend(legend).NoMark();
				scatter.AddSeries(&zz[iz][0], 1, xx[iz][0], 0).NoSeriesLegend().MarkStyle<CircleMarkPlot>().MarkWidth(15);
				scatter.AddSeries(&zz[iz].Top(), 1, xx[iz].Top(), 0).NoSeriesLegend().MarkStyle<CircleMarkPlot>().MarkWidth(15);
			}
			scatter.ZoomToFit(true, true);
			PNGEncoder().SaveFile(AppendFileName(dir, "Moor zanchor.png"), scatter.GetImage());
		}
	}
}