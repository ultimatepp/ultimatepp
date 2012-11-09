#ifndef _DXF_Block_h_
#define _DXF_Block_h_

#include <Core/Core.h>
using namespace Upp;

#include "TransMatrix.h"
#include "Entities.h"

class DXF;
class DXFBlock : public Pte<DXFBlock>
{
	friend class DXFEntities;
	friend class DXF;
	friend class DXFBlocks;
	friend class DXFHeader;
	private:
	
		// dxf container object, if not this one
		DXF *dxf;
		
		// start and end handles for block
		// don't know why we need 2....
		uint32 startHandle, endHandle;

	protected:
	
		// transformation matrix
		TransMatrix transMatrix;
		
		// transformation matrix stack
		Vector<TransMatrix> transStack;
		
		// current layer and lineType
		String layer;
		Vector<String> layerStack;
		
		String lineType;
		Vector<String>lineTypeStack;
		
		// current color
		int color;
		Vector<int>colorStack;
		
		// gets main dxf object
		virtual DXF &GetDXF(void) { return *dxf; }
		
		// blocks can only be constructed by DXF class
		// the main DXF block has a dxf member NULL
		DXFBlock(DXF *d = NULL);
		
		// write block to file
		bool Write(String const &name, Stream &s);
		
	public:

		// entities section
		DXFEntities entities;
	
		// push/pop Ucs
		DXFBlock &PushUcs(void);
		DXFBlock &PopUcs(void);
		
		// scales transformation matrix
		DXFBlock &ScaleUcs(double s);
		
		// rotates transformation matrix
		DXFBlock &RotateUcs(double a);
		
		DXFBlock &TranslateUcs(Pointf const &p);
		DXFBlock &TranslateUcs(double dx, double dy);
		
		// sets/gets current layer
		DXFBlock &SetCurrentLayer(String const &l);
		String const &GetCurrentLayer(void) const { return layer; }
		DXFBlock &PushLayer(String const &newLayer = "");
		DXFBlock &PopLayer(void);
		
		// sets/gets current linetype
		DXFBlock &SetCurrentLineType(String const &l);
		String const &GetCurrentLineType(void) const { return lineType; }
		DXFBlock &PushLineType(String const &newLineType = "");
		DXFBlock &PopLineType(void);
		
		// sets/gets current color
		DXFBlock &SetCurrentColor(int c);
		int GetCurrentColor(void) const { return color; }
		DXFBlock &PushColor(int newColor = Null);
		DXFBlock &PopColor(void);

		// gets the transformation matrix
		TransMatrix const &GetTransformationMatrix(void) const { return transMatrix; }
		
		// virtual destructor for polymorphic correctness
		virtual ~DXFBlock() {};
};

#endif
