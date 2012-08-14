#include "DXF.h"

//////////////////////////////////////////////////////////////////////////

// private constructor
DXFLayer::DXFLayer(DXFTables *t) : tables(t)
{
	handle = tables->GetNextHandle();
}

// write layer to stream
bool DXFLayer::Write(Stream &s)
{
	s << "0\nLAYER\n";
	s << "5\n" << Format64Hex(handle) << "\n";
	s << "100\nAcDbSymbolTableRecord\n100\nAcDbLayerTableRecord\n";
	s << "2\n" << ToCharset(CHARSET_WIN1252, name) << "\n";
	s << "70\n64\n";
	s << "62\n" << color << "\n";
	s << "6\n" << ToCharset(CHARSET_WIN1252, lineType) << "\n";
	s << "390\n" << tables->plotStyleHandle << "\n";

	return true;
}
		
DXFLayer &DXFLayer::SetName(String const &n)
{
	name = n;
	return *this;
}

DXFLayer &DXFLayer::SetColor(int c)
{
	color = c;
	return *this;
}

DXFLayer &DXFLayer::SetLineType(String const &lt)
{
	// add the linetype if needed
	if(!tables->HasLineType(lt))
		tables->AddLineType(lt);

	lineType = lt;
	return *this;
}

//////////////////////////////////////////////////////////////////////////
// private constructor
DXFLineType::DXFLineType(DXFTables *t) : tables(t)
{
	handle = tables->GetNextHandle();
}

// write linetype to stream
bool DXFLineType::Write(Stream &s)
{
/*
	73 The number of linetype elements
	40 Total pattern length
	49 Dash, dot or space length (one entry per element)}
	74 Complex linetype element type (one per element).
	   Default is 0 (no embedded shape/text).
*/

	s << "0\nLTYPE\n";
	s << "5\n" << Format64Hex(handle) << "\n";
	s << "100\nAcDbSymbolTableRecord\n100\nAcDbLinetypeTableRecord\n";
	s << "2\n" << ToCharset(CHARSET_WIN1252, name) << "\n";
	s << "70\n64\n";
	s << "3\n" << ToCharset(CHARSET_WIN1252, desc) << "\n";
	s << "72\n65\n";
	s << "73\n" << elements.GetCount() << "\n";
	double patLen = 0;
	for(int i = 0; i < elements.GetCount(); i++)
		patLen += fabs(elements[i]);
	s << "40\n" << patLen << "\n";
	for(int i = 0; i < elements.GetCount(); i++)
		s << "49\n" << elements[i] << "\n" << "74\n0\n";

	return true;	
}

DXFLineType &DXFLineType::SetName(String const &n)
{
	name = n;
	return *this;
}

DXFLineType &DXFLineType::SetDescription(String const &d)
{
	desc = d;
	return *this;
}

DXFLineType &DXFLineType::SetElements(Vector<double> const &e)
{
	elements <<= e;
	return *this;
}

DXFLineType &DXFLineType::SetElements(double e1, double e2, double e3, double e4, double e5, double e6, double e7, double e8)
{
	Vector<double> v;
	v << e1 << e2;
	if(!IsNull(e3)) v << e3;
	if(!IsNull(e4)) v << e4;
	if(!IsNull(e5)) v << e5;
	if(!IsNull(e6)) v << e6;
	if(!IsNull(e7)) v << e7;
	if(!IsNull(e8)) v << e8;
	return SetElements(v);
}

//////////////////////////////////////////////////////////////////////////
// private constructor -- may be created only by DXF class
DXFTables::DXFTables(DXF *d)
{
	dxf = d;
	
	// hard coded from dxf template
	plotStyleHandle = 0x0f;
	layersHandle = 0x02;
	lineTypesHandle = 0x05;

	// create default ByBlock, ByLayer and CONTINUOUS linetypes
	DXFLineType *lt;
	lt = new DXFLineType(this);
	lt->SetName("ByBlock");
	lt->handle = 0x14; // hardwired
	lineTypes.Add("ByBlock", lt);
	lt = new DXFLineType(this);
	lt->SetName("ByLayer");
	lt->handle = 0x15; // hardwired
	lineTypes.Add("ByLayer", lt);
	lt = new DXFLineType(this);
	lt->SetName("Continuous");
	lt->handle = 0x16; // hardwired
	lineTypes.Add("Continuous", lt);
	
	// create default layer 0
	DXFLayer *la;
	la = new DXFLayer(this);
	la->SetName("0");
	la->handle = 0x10; // hardwired
	la->SetLineType("Continuous");
	la->SetColor(DXF::White);
	layers.Add("0", la);
}

// write tables section to stream
bool DXFTables::Write(Stream &s)
{
	s << "0\nSECTION\n2\nTABLES\n";
	
	// active viweport -- partially hardwired, we just need to set
	// zoom factor and view center point to fit created drawing
	s << "0\nTABLE\n2\nVPORT\n";
	s << "5\n8\n330\n0\n100\nAcDbSymbolTable\n70\n2\n";
	s << "0\nVPORT\n5\n676\n330\n8\n100\nAcDbSymbolTableRecord\n100\nAcDbViewportTableRecord\n";
	s << "2\n*Active\n70\n0\n";
	s << "10\n0.0\n20\n0.0\n11\n1.0\n21\n1.0\n";
	s << "12\n" << dxf->viewCenter.x << "\n22\n" << dxf->viewCenter.y << "\n";
	s << "13\n0.0\n23\n0.0\n14\n10.0\n24\n10.0\n15\n10.0\n25\n10.0\n16\n0.0\n26\n0.0\n36\n1.0\n";
	s << "17\n0.0\n27\n0.0\n37\n0.0\n";
	s << "40\n" << dxf->viewHeight << "\n41\n2.128571428571428\n";
	s << "42\n50.0\n43\n0.0\n44\n0.0\n50\n0.0\n51\n0.0\n";
	s << "71\n0\n72\n1000\n73\n1\n74\n1\n75\n0\n76\n0\n77\n0\n78\n0\n";
	s << "281\n0\n65\n1\n110\n0.0\n120\n0.0\n130\n0.0\n111\n1.0\n121\n0.0\n131\n0.0\n";
	s << "112\n0.0\n122\n1.0\n132\n0.0\n79\n0\n146\n0.0\n";
	s << "0\nENDTAB\n";

	// hardwired tables from template
	s << (const char *)dxf_tbl;
	
	// block records, partially hardwired
	s << "0\nTABLE\n2\nBLOCK_RECORD\n";
	s << (const char *)dxf_blkrec;
	ArrayMap<String, DXFBlock> const &blocks = dxf->blocks.blocks;
	for(int iBlock = 0; iBlock < blocks.GetCount(); iBlock++)
	{
		s << "0\nBLOCK_RECORD\n";
		s << "5\n" << Format64Hex(dxf->GetNextHandle()) << "\n";
		s << "100\nAcDbSymbolTableRecord\n";
		s << "100\nAcDbBlockTableRecord\n";
		s << "2\n" << ToCharset(CHARSET_WIN1252, blocks.GetKey(iBlock)) << "\n";
	}
	s << "0\nENDTAB\n";

	// linetypes
	s << "0\nTABLE\n2\nLTYPE\n";
	s << "5\n" << Format64Hex(lineTypesHandle) << "\n";
	s << "100\nAcDbSymbolTable\n";
	s << "70\n" << lineTypes.GetCount() << "\n";
	for(int i = 0; i < lineTypes.GetCount(); i++)
		lineTypes[i].Write(s);
	s << "0\nENDTAB\n";
	
	// layers
	s << "0\nTABLE\n2\nLAYER\n";
	s << "5\n" << Format64Hex(layersHandle) << "\n";
	s << "100\nAcDbSymbolTable\n";
	s << "70\n" << layers.GetCount() << "\n";
	for(int i = 0; i < layers.GetCount(); i++)
		layers[i].Write(s);
	s << "0\nENDTAB\n";

	s << "0\nENDSEC\n";

	return true;
}
	
// gets next available handle
uint64 DXFTables::GetNextHandle(void)
{
	return dxf->GetNextHandle();
}
		

// adds a layer
DXFLayer &DXFTables::AddLayer(String const &name, int color, String const &lType)
{
	// add the linetype if needed
	if(!HasLineType(lType))
		AddLineType(lType);
	
	int layIdx = layers.Find(ToUpper(name));
	if(layIdx >= 0)
	{
		layers[layIdx].color = color;
		layers[layIdx].lineType = lType;
		return layers[layIdx];
	}
	DXFLayer &lay = layers.Add(ToUpper(name), new DXFLayer(this));
	lay.name = name;
	lay.color = color;
	lay.lineType = lType;
	return lay;
}

// checks if layer is there
bool DXFTables::HasLayer(String const &name) const
{
	return layers.Find(ToUpper(name)) >= 0;
}

// gets the layer descriptor
DXFLayer &DXFTables::GetLayer(String const &name)
{
	int idx = layers.Find(ToUpper(name));
	if(idx < 0)
		return *(DXFLayer *)NULL;
	return layers[idx];
}

// adds a linetype
DXFLineType &DXFTables::AddLineType(String const &name, Vector<double> const &elements)
{
	int ltIdx = lineTypes.Find(ToUpper(name));
	if(ltIdx >= 0)
	{
		lineTypes[ltIdx].elements <<= elements;
		return lineTypes[ltIdx];
	}
	DXFLineType &lt = lineTypes.Add(ToUpper(name), new DXFLineType(this));
	lt.name = name;
	lt.elements <<= elements;
	return lt;
}

DXFLineType &DXFTables::AddLineType(String const &name, double e1, double e2, double e3, double e4, double e5, double e6, double e7, double e8)
{
	Vector<double> elements;
	elements << e1 << e2;
	if(!IsNull(e3)) elements << e3;
	if(!IsNull(e4)) elements << e4;
	if(!IsNull(e5)) elements << e5;
	if(!IsNull(e6)) elements << e6;
	if(!IsNull(e7)) elements << e7;
	if(!IsNull(e8)) elements << e8;
	return AddLineType(name, elements);
}

// check if linetype is there
bool DXFTables::HasLineType(String const &name) const
{
	return lineTypes.Find(ToUpper(name)) >= 0;
}

// gets linetype descriptor
DXFLineType &DXFTables::GetLineType(String const &name)
{
	int idx = lineTypes.Find(ToUpper(name));
	if(idx < 0)
		return *(DXFLineType *)NULL;
	return lineTypes[idx];
}
