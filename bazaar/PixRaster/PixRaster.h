#ifndef _PixRaster_h_
#define _PixRaster_h_

#include <Draw/Draw.h>

#include "lib/allheaders.h"

NAMESPACE_UPP

///////////////////////////////////////////////////////////////////////////////
// WARNING :
//
// ENDIANNESS MUST BE DEFINED AS L_BIG_ENDIAN or L_LITTLE_ENDIAN
// PIXEL ACCESSES DEPENDS ON IT ON LEPTONICA
///////////////////////////////////////////////////////////////////////////////
#define L_LITTLE_ENDIAN

// shortcut for current page
// set to a value which should never happen
#define PIXRASTER_INVALID_PAGE	0x8fffffff
#define PIXRASTER_CURPAGE		0x8ffffffe
#define PIXRASTER_LASTPAGE		0x8ffffffd
#define PIXRASTER_END			0x8ffffffc
#define PIXRASTER_BEGIN			0x00000000

// raster class wrapping Leptonica PIX and PIXA structures
class PixRaster : public Raster
{
	protected:
	
		// array of Leptonica PIXs
		// can contain just one PIX for single page images
		PIXA *pixa;
		
		// currently active page for multipage images
		int curPage;
		
		// raster format
		Array<RasterFormat *>formats;
		
		// sets raster format depending on page's pix format
		void SetRasterFormat(int page);
		
		// checks wether page format is valid
		bool IsRasterFormatValid(int page = PIXRASTER_CURPAGE);
		
		// destroys page format
		void DestroyRasterFormat(int page = PIXRASTER_CURPAGE); 
		
		// local copy of palettes, if any
		Array<RGBA *>localPalettes;
		
		// synthesyze grayscale palettes
		void CreateGrayPalette2(int page);
		void CreateGrayPalette4(int page);
		void CreateGrayPalette16(int page);
		void CreateGrayPalette256(int page);
		
		// convert PIX palette
		void ConvertPIXPalette(PIXCMAP *colormap, int page);
		
		// destroys local palette
		void DestroyLocalPalette(int page = PIXRASTER_CURPAGE);

		// checks wether local palette is valid
		bool IsLocalPaletteValid(int page = PIXRASTER_CURPAGE);

		// raster loading/conversion routines
		PIX *LoadRASTER_RGBA(Raster &raster);

		PIX *LoadRASTER_1(Raster &raster);
		PIX *LoadRASTER_2(Raster &raster);
		PIX *LoadRASTER_4(Raster &raster);
		PIX *LoadRASTER_8(Raster &raster);
		PIX *LoadRASTER_8ALPHA(Raster &raster);
		PIX *LoadRASTER_16(Raster &raster);
		PIX *LoadRASTER_24(Raster &raster);
		PIX *LoadRASTER_32(Raster &raster);
		
		PIX *LoadRASTER_1_MSBFIRST(Raster &raster);
		PIX *LoadRASTER_2_MSBFIRST(Raster &raster);
		PIX *LoadRASTER_4_MSBFIRST(Raster &raster);
		PIX *LoadRASTER_8_MSBFIRST(Raster &raster);
		PIX *LoadRASTER_8ALPHA_MSBFIRST(Raster &raster);
		PIX *LoadRASTER_16_MSBFIRST(Raster &raster);
		PIX *LoadRASTER_24_MSBFIRST(Raster &raster);
		PIX *LoadRASTER_32_MSBFIRST(Raster &raster);

		// gets real page
		int getTruePage(int page = PIXRASTER_CURPAGE);
		
		// internal destructor
		void Destroy(void);

	public:
	
		// pixmaps copy modes
		enum CopyModes { PIXRASTER_CLONE = L_CLONE, PIXRASTER_COPY = L_COPY, PIXRASTER_COPY_CLONE = L_COPY_CLONE, PIXRASTER_REF };
		
		// bring-in color modes
		enum BringInModes { PIXRASTER_BRING_IN_WHITE = L_BRING_IN_WHITE, PIXRASTER_BRING_IN_BLACK = L_BRING_IN_BLACK };

		// constructor -- empty raster
		PixRaster();

		// constructor -- takes a Leptonica Pix
		// PIX get cloned and its reference count increased or copied
		PixRaster(PIX *pix, CopyModes copyMode = PIXRASTER_CLONE);
		
		// constructor -- takes an array of Leptonica Pix
		// PIXA gets cloned and its reference count increased
		PixRaster(PIXA *pixa, CopyModes copyMode = PIXRASTER_COPY_CLONE);

		// destructor -- must free PIXA array
		~PixRaster() { Destroy() ; }
		
		virtual void SeekPage(int page);
		virtual int GetPageCount();
		virtual int GetActivePage();
		
		// standard Raster functions -- they operate on active page
		virtual Size GetSize(void) { return GetSize(PIXRASTER_CURPAGE); }
		virtual Info GetInfo(void) { return GetInfo(PIXRASTER_CURPAGE); }
		virtual Line GetLine(int line) { return GetLine(line, PIXRASTER_CURPAGE); }
		virtual int GetPaletteCount() { return GetPaletteCount(PIXRASTER_CURPAGE); }
		virtual const RGBA *GetPalette() { return GetPalette(PIXRASTER_CURPAGE); }
		virtual const RasterFormat *GetFormat() { return GetFormat(PIXRASTER_CURPAGE); }
		virtual int GetWidth() { return GetSize().cx; }
		virtual int GetHeight() { return GetSize().cy; }
	
		// extended Raster functions -- they allow to query
		// a given page inside raster
		virtual Size GetSize(int page);
		virtual Info GetInfo(int page);
		virtual Line GetLine(int line, int page);
		virtual int GetPaletteCount(int page);
		virtual const RGBA *GetPalette(int page);
		virtual const RasterFormat *GetFormat(int page);
		virtual int GetWidth(int page) { return GetSize(page).cx; }
		virtual int GetHeight(int page) { return GetSize(page).cy; }
	
		// check wether pixraster has images
		bool IsEmpty(void);
		
		// gets PIX and PIX array from raster
		// by default objects get it's reference count increased, so they
		// must be freed by pixDestroy() - can be changed by copyMode parameter
		PIX *GetPIX(int page = PIXRASTER_CURPAGE, CopyModes copyMode = PIXRASTER_REF);
		PIXA *GetPIXA(CopyModes copyMode = PIXRASTER_REF);
		operator PIX*(void) { return GetPIX(PIXRASTER_CURPAGE, PIXRASTER_REF); }
		
		// sets PIX and PIX array
		void SetPIX(PIX *pix, int page = PIXRASTER_CURPAGE, CopyModes copyMode = PIXRASTER_CLONE);
		void SetPIXA(PIXA *pixa, CopyModes copyMode = PIXRASTER_COPY_CLONE);

		// adds a PIX or a PIX array
		void AddPIX(PIX *pix, CopyModes copyMode = PIXRASTER_CLONE);
		void AddPIXA(PIXA *_pixa, CopyModes copyMode = PIXRASTER_COPY_CLONE);
		
		// inserts a PIX or a PIX array
		void InsertPIX(PIX *pix, int where = PIXRASTER_CURPAGE, CopyModes copyMode = PIXRASTER_COPY_CLONE);
		void InsertPIXA(PIXA *pixa, int where = PIXRASTER_CURPAGE, CopyModes copyMode = PIXRASTER_COPY_CLONE);
		
		// removes a PIX or a series of PIX
		void RemovePIX(int startPage = PIXRASTER_CURPAGE, int count = 1);
		
		// duplicates a page at the end of array (useful for stack-like operations)
		// NOTE: it seeks added page too
		void Dup(int page = PIXRASTER_CURPAGE);
		
		// Drops last page(s) of array (ditto)
		// NOTE: it seeks last page too
		void Drop(int count = 1);
		
		// loads Pix from another raster object
		void Load(Raster& raster, bool Append = false, CopyModes copyMode = PIXRASTER_CLONE);
		
		// sets/appends a source raster
		void operator=(Raster &raster) { Load(raster, false, PIXRASTER_COPY); }
		void operator+=(Raster &raster) { Load(raster, true, PIXRASTER_COPY); }
		
		// file I/O
		bool Load(FileIn &fs, bool Append = false);
		bool Load(String fileName, bool Append = false);
		bool Save(String fileName, int page = PIXRASTER_CURPAGE); // @@ to do - add compression and type handling
		bool SaveAll(String fileName);
		
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////// LEPTONICA OPERATIONS //////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// IMAGE THRESHOLDING //

		bool DitherToBinary(int page = PIXRASTER_CURPAGE);
		bool DitherToBinarySpec(int lowerclip, int upperclip, int page = PIXRASTER_CURPAGE);

		// threshold the image -- warning, operates ONLY on grayscale pixmaps
		// original image is unchanged - a new modified image at raster's end
		bool ThresholdToBinary(int threshold, int page = PIXRASTER_CURPAGE);

		bool VarThresholdToBinary(int thresholdPage, int page = PIXRASTER_CURPAGE);
		bool DitherToBinaryLUT(int lowerclip, int upperclip, int page = PIXRASTER_CURPAGE);
		bool GenerateMaskByValue(int val, int page = PIXRASTER_CURPAGE);
		bool GenerateMaskByBand(int lower, int upper, int inband, int page = PIXRASTER_CURPAGE);
		bool DitherTo2bpp(int cmapflag, int page = PIXRASTER_CURPAGE);
		bool DitherTo2bppSpec(int lowerclip, int upperclip, int cmapflag, int page = PIXRASTER_CURPAGE);
		bool ThresholdTo2bpp(int nlevels, int cmapflag, int page = PIXRASTER_CURPAGE);
		bool ThresholdTo4bpp(int nlevels, int cmapflag, int page = PIXRASTER_CURPAGE);
		bool ThresholdOn8bpp(int nlevels, int cmapflag, int page = PIXRASTER_CURPAGE);
		bool ThresholdGrayArb(const char  *edgevals, int outdepth, int use_average, int setblack, int setwhite, int page = PIXRASTER_CURPAGE);
		Buffer<int> MakeGrayQuantIndexTable(int nlevels);
		Buffer<int> MakeGrayQuantTargetTable(int nlevels, int depth);
		bool GenerateMaskByBand32(unsigned refval, int delm, int delp, int page = PIXRASTER_CURPAGE);
		bool GenerateMaskByDiscr32(unsigned refval1, unsigned refval2, int distflag, int page = PIXRASTER_CURPAGE);
		bool GrayQuantFromHisto(int mPage, double minfract, int maxsize, int page = PIXRASTER_CURPAGE);

		bool ThresholdToValue(int threshval, int setval, int page = PIXRASTER_CURPAGE);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// SKEW REMOVAL //

		// Simple top-level deskew interfaces
		bool Deskew(int ReductionFactor = 1, int page = PIXRASTER_CURPAGE);
		bool FindSkewAndDeskew(int ReductionFactor, double *skewAngle = NULL, double *confidenceFactor = NULL, int page = PIXRASTER_CURPAGE);

		// Simple top-level skew angle finding interface
		bool FindSkew(double *pangle, double *pconf = NULL, int page = PIXRASTER_CURPAGE);
		
		// Basic angle-finding functions with all parameters
		bool FindSkewSweep(double *pangle, int reduction, double sweeprange, double sweepdelta, int page = PIXRASTER_CURPAGE);

		// Angle-finding functions with all parameters
		bool FindSkewSweepAndSearch(
			double *pangle, double *pconf,
			int redsweep, int redsearch,
			double sweeprange, double sweepdelta,
			double minbsdelta,
			int page = PIXRASTER_CURPAGE);

		// NOTE : Leptonica has many more skew functions....
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// ROTATION FUNCTIONS 

 		// Rotates image (2-4-8 or 32 bpp) about center.
		bool RotateAM(double angle, BringInModes incolo = PIXRASTER_BRING_IN_WHITE, int page = PIXRASTER_CURPAGE);
		bool RotateAMColor(double angle, int colorval, int page = PIXRASTER_CURPAGE);
		bool RotateAMGray(double angle, int grayval, int page = PIXRASTER_CURPAGE);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// MORPHING FUNCTIONS 

		bool ErodeGray(int hsize, int vsize, int page = PIXRASTER_CURPAGE);
		bool DilateGray(int hsize, int vsize, int page = PIXRASTER_CURPAGE);
		bool OpenGray(int hsize, int vsize, int page = PIXRASTER_CURPAGE);
		bool CloseGray(int hsize, int vsize, int page = PIXRASTER_CURPAGE);
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// ARITHMETIC AND ALIKE FUNCTIONS
		
		bool Invert(int page = PIXRASTER_CURPAGE); 
		bool AddConstantGray(int val, int page = PIXRASTER_CURPAGE);
		bool MultConstantGray(int val, int page = PIXRASTER_CURPAGE);
		bool AddGray(int page1, int page2);
		bool SubtractGray(int page1, int page2);
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// BLENDING/COMBINING FUNCTIONS

		bool CombineMasked(int destPage, int sourcePage, int maskPage);
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// DOCUMENT PAGE SEGMENTING FUNCTIONS
		
		// top level page segmenting
		bool GetRegionsBinary(int page = PIXRASTER_CURPAGE) ;

		
}; // END class PixRaster

END_UPP_NAMESPACE

#endif
