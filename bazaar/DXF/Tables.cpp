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
	s << "2\n" << name << "\n";
	s << "70\n64\n";
	s << "62\n" << color << "\n";
	s << "6\n" << lineType << "\n";
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
	s << "2\n" << name << "\n";
	s << "70\n64\n";
	s << "3\n" << desc << "\n";
	s << "72\n65\n";
	s << "73\n" << elements.GetCount() << "\n";
	double patLen = 0;
	for(int i = 0; i < elements.GetCount(); i++)
		patLen += elements[i];
	s << "40\n" << patLen << "\n";
	for(int i = 0; i < elements.GetCount(); i++)
		s << "49\n" << elements[i] << "\n";

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
	lineTypes.Add(lt);
	lt = new DXFLineType(this);
	lt->SetName("ByLayer");
	lt->handle = 0x15; // hardwired
	lineTypes.Add(lt);
	lt = new DXFLineType(this);
	lt->SetName("Continuous");
	lt->handle = 0x16; // hardwired
	lineTypes.Add(lt);
	
	// create default layer 0
	DXFLayer *la;
	la = new DXFLayer(this);
	la->SetName("0");
	la->handle = 0x10; // hardwired
	la->SetLineType("Continuous");
	la->SetColor(DXF::White);
	layers.Add(la);
}

// write tables section to stream
bool DXFTables::Write(Stream &s)
{
	s << "0\nSECTION\n2\nTABLES\n";
	
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
		s << "2\n" << blocks.GetKey(iBlock) << "\n";
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
	for(int i = 0; i < layers.GetCount(); i++)
	{
		DXFLayer &lay = layers[i];
		if(ToUpper(lay.name == name))
		{
			lay.color = color;
			lay.lineType = lType;
			return lay;
		}
	}		
	DXFLayer &lay = layers.Add(new DXFLayer(this));
	lay.name = name;
	lay.color = color;
	lay.lineType = lType;
	return lay;
}

// adds a linetype
DXFLineType &DXFTables::AddLineType(String const &name, Vector<double> const &elements)
{
	for(int i = 0; i < lineTypes.GetCount(); i++)
	{
		DXFLineType &lt = lineTypes[i];
		if(ToUpper(lt.name == name))
		{
			lt.elements <<= elements;
			return lt;
		}
	}		
	DXFLineType &lt = lineTypes.Add(new DXFLineType(this));
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
