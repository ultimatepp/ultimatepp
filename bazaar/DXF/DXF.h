#ifndef _DXF_DXF_h_
#define _DXF_DXF_h_

#define DXF_EPSILON 0.0000001

#include "Block.h"
#include "Header.h"
#include "Tables.h"
#include "Objects.h"
#include "Blocks.h"

#include "Entities.h"

#include "DXFColorChooser.h"

extern "C" byte *dxf_hdr;
extern "C" byte *dxf_tbl;
extern "C" byte *dxf_blkrec;
extern "C" byte *dxf_blk;
extern "C" byte *dxf_obj;

class DXF : public DXFBlock
{
	friend class DXFTables;
	friend class DXFEntities;
	friend class DXFBlock;
	
	public:
		enum Colors { ByBlock = 0, Red = 1, Yellow, Green, Cyan, Blue, Magenta, White, ByLayer = 256 };
	
	private:
	
		// next free handle
		uint32 nextHandle;
	
		// the header
		DXFHeader header;
		
		// gets next free handle
		uint32 GetNextHandle(void) { return nextHandle++; }
		
		// blocks
		DXFBlocks blocks;
		
		// objects
		DXFObjects objects;
		
		// gets main dxf object
		virtual DXF &GetDXF(void) { return *this; }
		
		// flag for scaling insertions with transform matrix
		bool scaleInsertions;
		
		// view center point and height
		// calculated from GetBoundingBox functions
		Pointf viewCenter;
		double viewHeight;

	protected:
	
	public:
	
		// constructor
		DXF();
		
		// the tables section
		DXFTables tables;
		
		// write drawing to file
		bool Write(Stream &s);
		
		// some utility functions
		
		// normalizes an angle to -M_PI .. +M_PI
		static double NormalizeAngle(double a);
		
		// Add a block definition
		DXFBlock &AddBlock(String const &name);
		
		// checks if a block is present
		bool HasBlock(String const &name) const;
		
		// gets a block by name
		DXFBlock &GetBlock(String const &name);
		DXFBlock const &GetBlock(String const &name) const;
		
		// sets insertion scale option
		DXF &SetScaleInsertions(bool i = true) { scaleInsertions = i; return *this; }
		DXF &SetNoScaleInsertions() { return SetScaleInsertions(false); }

		// gets insertion scale option
		bool GetScaleInsertions() const { return scaleInsertions; }
};

#endif
