#include "Block.h"
#include "DXF.h"

DXFBlock::DXFBlock(DXF *d) : entities(this)
{
	dxf = d;
	if(dxf)
	{
		startHandle = dxf->GetNextHandle();
		endHandle = dxf->GetNextHandle();
	}
	else
		startHandle = endHandle = 0;

	color = DXF::ByBlock;
	layer = "0";
	lineType = "ByBlock";
}

// write drawing to file
bool DXFBlock::Write(String const &name, Stream &s)
{
	s << "0\nBLOCK\n";
	s << "5\n" << Format64Hex(startHandle) << "\n";
	s << "100\nAcDbEntity\n";
	s << "8\n0\n";
	s << "100\nAcDbBlockBegin\n";
	s << "2\n" << ToCharset(CHARSET_WIN1252, name) << "\n";
	s << "70\n0\n";							// attributes ???
	s << "10\n0.0\n20\n0.0\n30\n0.0\n";		// shall we handle insertion base point ?
	s << "3\n" << ToCharset(CHARSET_WIN1252, name) << "\n";
	s << "1\n\n";
	
	// write out entities
	entities.WriteNoHeader(s);

	// block end markers
	s << "0\nENDBLK\n";
	s << "5\n" << Format64Hex(endHandle) << "\n";
	s << "100\nAcDbEntity\n";
	s << "8\n0\n";
	s << "100\nAcDbBlockEnd\n";
	return true;
}
		
// push/pop Ucs
DXFBlock &DXFBlock::PushUcs(void)
{
	transStack.Add(transMatrix);
	return *this;
}

DXFBlock &DXFBlock::PopUcs(void)
{
	if(transStack.GetCount())
		transMatrix = transStack.Pop();
	return *this;
}
		
// scales transformation matrix
DXFBlock &DXFBlock::ScaleUcs(double s)
{
	transMatrix.Scale(s);
	return *this;
}

// rotates transformation matrix
DXFBlock &DXFBlock::RotateUcs(double a)
{
	transMatrix.Rotate(a);
	return *this;
}

DXFBlock &DXFBlock::TranslateUcs(Pointf const &p)
{
	transMatrix.Translate(p);
	return *this;
}

DXFBlock &DXFBlock::TranslateUcs(double dx, double dy)
{
	transMatrix.Translate(dx, dy);
	return *this;
}
		
// sets/gets current layer
DXFBlock &DXFBlock::SetCurrentLayer(String const &l)
{
	layer = l;
	return *this;
}

DXFBlock &DXFBlock::PushLayer(String const &newLayer)
{
	layerStack.Add(layer);
	if(!newLayer.IsEmpty())
		layer = newLayer;
	return *this;
}

DXFBlock &DXFBlock::PopLayer(void)
{
	if(layerStack.GetCount())
		layer = layerStack.Pop();
	return *this;
}
		
// sets/gets current linetype
DXFBlock &DXFBlock::SetCurrentLineType(String const &l)
{
	lineType = l;
	return *this;
}

DXFBlock &DXFBlock::PushLineType(String const &newLineType)
{
	lineTypeStack.Add(lineType);
	if(!newLineType.IsEmpty())
		lineType = newLineType;
	return *this;
}

DXFBlock &DXFBlock::PopLineType(void)
{
	if(lineTypeStack.GetCount())
		lineType = lineTypeStack.Pop();
	return *this;
}

// sets/gets current color
DXFBlock &DXFBlock::SetCurrentColor(int c)
{
	color = c;
	return *this;
}

DXFBlock &DXFBlock::PushColor(int newColor)
{
	colorStack.Add(color);
	if(!IsNull(newColor))
		color = newColor;
	return *this;
}

DXFBlock &DXFBlock::PopColor(void)
{
	if(colorStack.GetCount())
		color = colorStack.Pop();
	return *this;
}

