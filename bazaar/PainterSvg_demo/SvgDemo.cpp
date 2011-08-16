#include "Examples.h"

String file = "C:\\Demo.svg";

void SvgDemo(Painter& sw)
{	
	try {
		if (FileExists(file)) 
			ParseSVG(sw, LoadFileBOM(file), GetFileDirectory(file));
		else {
			sw.Scale(1./20);
			sw.Text(200, 200, "File '" + file + "' not found", Font(Font::ARIAL, 400));
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
