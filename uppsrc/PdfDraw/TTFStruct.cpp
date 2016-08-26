#include "PdfDraw.h"

namespace Upp {

void   TTFReader::Head::Serialize(TTFStream& s)
{
	s % version % fontRevision % checkSumAdjustment % magicNumber % flags % unitsPerEm;
	s.Raw(created, 8);
	s.Raw(modified, 8);
	s % xMin % yMin % xMax % yMax % macStyle % lowestRecPPEM % fontDirectionHint
	  % indexToLocFormat % glyphDataFormat;
}

void   TTFReader::Hhea::Serialize(TTFStream& s)
{
	reserved1 = reserved2 = reserved3 = reserved4 = 0;
	s % version % ascent % descent % lineGap % advanceWidthMax % minLeftSideBearing
	  % minRightSideBearing % xMaxExtent % caretSlopeRise % caretSlopeRun % caretOffset
	  % reserved1 % reserved2 % reserved3 % reserved4 % metricDataFormat % numOfLongHorMetrics;
}

void   TTFReader::Maxp::Serialize(TTFStream& s)
{
	s % version % numGlyphs % maxPoints % maxContours % maxComponentPoints
	  % maxComponentContours % maxZones % maxTwilightPoints % maxStorage
	  % maxFunctionDefs % maxInstructionDefs % maxStackElements
	  % maxSizeOfInstructions % maxComponentElements % maxComponentDepth;
}

void   TTFReader::Post::Serialize(TTFStream& s)
{
	reserved = 0;
	s % format % italicAngle % underlinePosition % underlineThickness % isFixedPitch
	  % reserved % minMemType42 % maxMemType42 % minMemType1 % maxMemType1;
}

}
