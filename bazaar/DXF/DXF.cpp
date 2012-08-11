#include "DXF.h"

// brc file containing dxf prebuilt code
#include "dxf.brc"

// translation file
#define TFILE <DXF/DXF.t>
#include <Core/t.h>

// constructor
// seeds handles with a number greater than the one in template
DXF::DXF() : nextHandle(0x1000), blocks(this), objects(this), tables(this)
{
	tables.AddLineType("CONTINUOUS");
	tables.AddLayer("0");

	// override block default values
	color = DXF::ByLayer;
	lineType = "CONTINUOUS";
	
	// do NOT scale insertions by default
	scaleInsertions = false;
}

// write drawing to file
bool DXF::Write(Stream &s)
{
	header.Write(s);
	tables.Write(s);
	blocks.Write(s);
	objects.Write(s);
	entities.Write(s);
	s << "0\nEOF\n";
	return true;
}

// normalizes an angle to -M_PI .. +M_PI
double DXF::NormalizeAngle(double a)
{
	a = fmod(a, 2*M_PI);
	if(a < - M_PI)
		a += 2 * M_PI;
	if(a > M_PI)
		a -= 2 * M_PI;
	return a;
}

// Add a block definition
DXFBlock *DXF::AddBlock(String const &name)
{
	int idx = blocks.blocks.Find(name);
	if(idx >= 0)
		return &blocks.blocks[idx];
	blocks.blocks.Add(name, new DXFBlock(this));
	return &blocks.blocks.Top();
}

// gets a block by name
DXFBlock *DXF::GetBlock(String const &name)
{
	int idx = blocks.blocks.Find(name);
	if(idx >= 0)
		return &blocks.blocks[idx];
	return NULL;
}
