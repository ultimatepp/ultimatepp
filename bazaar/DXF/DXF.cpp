#include "DXF.h"

// brc file containing dxf prebuilt code
#include "dxf.brc"

// translation file
#define TFILE <DXF/DXF.t>
#include <Core/t.h>

// constructor
// seeds handles with a number greater than the one in template
DXF::DXF() : nextHandle(0x1000), header(this), blocks(this), objects(this), tables(this)
{
	tables.AddLineType("CONTINUOUS");
	tables.AddLayer("0");

	// override block default values
	color = DXF::ByLayer;
	lineType = "ByLayer";
	
	// do NOT scale insertions by default
	scaleInsertions = false;
	
	// initialize view center and height
	viewCenter = Pointf(100, 100);
	viewHeight = 50;
}

// write drawing to file
bool DXF::Write(Stream &s)
{
	// if we've got some entities, calculate drawing bounding box
	// and zoom to fit it before saving
	if(entities.entities.GetCount())
	{
		Rectf bb = entities.GetBoundingBox();
		viewCenter = Pointf( (bb.left + bb.right) / 2, (bb.top + bb.bottom) / 2);
		viewHeight = bb.top - bb.bottom;
		double viewWidth = bb.right - bb.left;
		if(viewHeight * 2.128571428571428 < viewWidth)
			viewHeight = viewWidth / 2.128571428571428;
	}
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
DXFBlock &DXF::AddBlock(String const &name)
{
	int idx = blocks.blocks.Find(name);
	if(idx >= 0)
		return blocks.blocks[idx];
	blocks.blocks.Add(name, new DXFBlock(this));
	return blocks.blocks.Top();
}

// checks if a block is present
bool DXF::HasBlock(String const &name) const
{
	return blocks.blocks.Find(name) >= 0;
}
		
// gets a block by name
DXFBlock &DXF::GetBlock(String const &name)
{
	int idx = blocks.blocks.Find(name);
	if(idx >= 0)
		return blocks.blocks[idx];
	return *(DXFBlock *)NULL;
}

DXFBlock const &DXF::GetBlock(String const &name) const
{
	int idx = blocks.blocks.Find(name);
	if(idx >= 0)
		return blocks.blocks[idx];
	return *(DXFBlock *)NULL;
}
