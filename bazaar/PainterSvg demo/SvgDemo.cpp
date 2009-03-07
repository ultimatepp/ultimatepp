#include "Examples.h"

void ParseSVG(Painter& p, const char *svg);

void SvgDemo(Painter& sw)
{
	String file = "C:\\demo.svg";
	
	try {
		if (FileExists(file))	
			ParseSVG(sw, LoadFileBOM(file));
		else {
			sw.Scale(1./20);
			sw.Text(200, 200, "Please copy the svg file in '" + file + "'", Font(Font::ARIAL, 400));
			sw.Text(200, 700, "or change String file in SvgDemo.cpp", Font(Font::ARIAL, 400));
			sw.Opacity(1);
			sw.Stroke(1, Black());
			sw.Fill(Black());
		}
	}
	catch(XmlError e) {
		LOG("SVG XML error: " + e);
	}
}

INITBLOCK {
	RegisterExample("Svg demo", SvgDemo);
}
