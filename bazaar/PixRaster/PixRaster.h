#ifndef _PixRaster_h_
#define _PixRaster_h_

#include <Draw/Draw.h>

#define Pix _Pix
#include "lib/allheaders.h"
#undef Pix

#include "Marker.h"

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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// base class for Pix and PixRaster -- used mainly to have a common base class for PixRasterCtrl
class PixBase : public Raster
{
	public:
		typedef PixBase CLASSNAME;

		virtual void SeekPage(int page) {};
		virtual int GetPageCount() { return 1; }
		virtual int GetActivePage() { return 0; }

		// gets underlying PIX object
		// WARNING -- Pix has ownership of it, so DON'T free nor destroy it
		virtual PIX *GetPIX(int page = PIXRASTER_CURPAGE) = 0;

		// standard Raster functions -- they operate on active page
		virtual Size GetSize()					{ return GetSizeEx(PIXRASTER_CURPAGE); }
		virtual Raster::Info GetInfo()			{ return GetInfoEx(PIXRASTER_CURPAGE); }
		virtual Raster::Line GetLine(int line)	{ return GetLineEx(line, PIXRASTER_CURPAGE); }
		virtual int GetPaletteCount()			{ return GetPaletteCountEx(PIXRASTER_CURPAGE); }
		virtual const RGBA *GetPalette()		{ return GetPaletteEx(PIXRASTER_CURPAGE); }
		virtual const RasterFormat *GetFormat() { return GetFormatEx(PIXRASTER_CURPAGE); }
		virtual int GetWidth() { return GetSize().cx; }
		virtual int GetHeight() { return GetSize().cy; }
		virtual Markers *GetMarkers() { return GetMarkersEx(PIXRASTER_CURPAGE); }
		

		// extended Raster functions -- they allow to query
		// a given page inside raster
		virtual Size GetSizeEx(int page) = 0;
		virtual Info GetInfoEx(int page) = 0;
		virtual Line GetLineEx(int line, int page) = 0;
		virtual int GetPaletteCountEx(int page) = 0;
		virtual const RGBA *GetPaletteEx(int page) = 0;
		virtual const RasterFormat *GetFormatEx(int page) = 0;
		virtual int GetWidthEx(int page)		{ return GetSizeEx(page).cx; }
		virtual int GetHeightEx(int page)		{ return GetSizeEx(page).cy; }
		virtual Markers *GetMarkersEx(int page) = 0;
		
		virtual bool IsEmpty() = 0;
		operator bool() { return !IsEmpty(); }
		
		virtual void Clear() = 0;

}; // END Class PixBase

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class wrapping Leptonica PIX image -- it's a single page raster format
class PixRaster;
class Pix : public PixBase
{
	// give PixRaster access to private members
	friend class PixRaster;
	
	public:
		typedef Pix CLASSNAME;
		
	private:
	
		// the Leptonica PIX
		PIX *pix;

		// raster format
		RasterFormat *rasterFormat;
		
		// local palette
		RGBA *localPalette;
		
		// polygon markers
		Markers markers;
	
	protected:
	
		// internal destructor
		void Destroy();
		
		// sets raster format depending on pix format
		void SetRasterFormat();
		
		// synthesyze grayscale palettes
		void CreateGrayPalette2();
		void CreateGrayPalette4();
		void CreateGrayPalette16();
		void CreateGrayPalette256();
		
		// convert PIX palette
		void ConvertPIXPalette(PIXCMAP *colormap);
		
		// raster loading/conversion routines
		bool LoadRASTER_RGBA(Raster &raster);

		bool LoadRASTER_1(Raster &raster);
		bool LoadRASTER_2(Raster &raster);
		bool LoadRASTER_4(Raster &raster);
		bool LoadRASTER_8(Raster &raster);
		bool LoadRASTER_8ALPHA(Raster &raster);
		bool LoadRASTER_16(Raster &raster);
		bool LoadRASTER_24(Raster &raster);
		bool LoadRASTER_32(Raster &raster);
		
		bool LoadRASTER_1_MSBFIRST(Raster &raster);
		bool LoadRASTER_2_MSBFIRST(Raster &raster);
		bool LoadRASTER_4_MSBFIRST(Raster &raster);
		bool LoadRASTER_8_MSBFIRST(Raster &raster);
		bool LoadRASTER_8ALPHA_MSBFIRST(Raster &raster);
		bool LoadRASTER_16_MSBFIRST(Raster &raster);
		bool LoadRASTER_24_MSBFIRST(Raster &raster);
		bool LoadRASTER_32_MSBFIRST(Raster &raster);

	public:
	
		// gets underlying PIX object
		// WARNING -- Pix has ownership of it, so DON'T free nor destroy it
		virtual PIX *GetPIX(int page = PIXRASTER_CURPAGE) { return pix; }
		operator PIX*() { return GetPIX(PIXRASTER_CURPAGE); }
	
		// bring-in color modes
		enum BringInModes { PIXRASTER_BRING_IN_WHITE = L_BRING_IN_WHITE, PIXRASTER_BRING_IN_BLACK = L_BRING_IN_BLACK };
		
		// rotate modes
		enum RotateModes { PIXRASTER_ROTATE_AREA_MAP = L_ROTATE_AREA_MAP, PIXRASTER_ROTATE_SHEAR = L_ROTATE_SHEAR, PIXRASTER_ROTATE_SAMPLING = L_ROTATE_SAMPLING };

		// scan direction modes
		enum ScanModes { PIXRASTER_FROM_LEFT = L_FROM_LEFT, PIXRASTER_FROM_RIGHT = L_FROM_RIGHT, PIXRASTER_FROM_TOP = L_FROM_TOP, PIXRASTER_FROM_BOTTOM = L_FROM_BOTTOM };

		// loads Pix from another raster object
		void Load(Raster& raster, bool deepCopy = false, int page = PIXRASTER_CURPAGE);
		
		// empty constructor -- sets NULL PIX
		Pix();
		
		// creates a PIX in given format and size
		Pix(int width, int height, int depth, RGBA *colorTable = NULL);

		// constructor -- takes a Leptonica PIX
		// TAKES OWNERSHIP of underlying PIX object which is NULLed
		Pix(PIX **_pix);
		
		// copy constructor -- reference increments underlying PIX
		Pix(Pix const &_pix);
		
		// deep copy constructor -- deep copy of underlying PIX
		Pix(Pix const &_pix, int i);
		
		// creation of Pix from a raster object
		Pix(Raster &raster, bool deepCopy = false, int page = PIXRASTER_CURPAGE);
		
		// creation of Pix from file/stream
		Pix(FileIn &fs, int page = 0);
		Pix(String const &fileName, int page = 0);
		
		// destructor
		~Pix();
		
		// assignment -- increment reference of underlying PIX
		Pix &operator=(Pix &pix);
		Pix const &operator=(Pix const &pix) { return operator=((Pix &)pix); }
		
		// deep copy -- deep copies underlying PIX
		Pix &operator <<=(Pix &pix);
		Pix &DeepCopy(Pix &pix);

		// checks wether Pix is empty
		virtual bool IsEmpty() { return (pix == NULL); }
		
		// empties the image
		virtual void Clear() { Destroy(); }

		// extended Raster functions -- they allow to query
		// a given page inside raster
		virtual Size GetSizeEx(int page);
		virtual Info GetInfoEx(int page);
		virtual Line GetLineEx(int line, int page);
		virtual int GetPaletteCountEx(int page);
		virtual const RGBA *GetPaletteEx(int page);
		virtual const RasterFormat *GetFormatEx(int page);
		
		// gets polygon markers
		virtual Markers *GetMarkers() { return &markers; }
		virtual Markers *GetMarkersEx(int) { return &markers; }
		
		// file I/O
		bool Load(FileIn &fs, int page = 0);
		bool Load(String fileName, int page = 0);
		bool Save(String fileName, int quality = 75, bool progressive = false);

		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////// LEPTONICA OPERATIONS //////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// IMAGE THRESHOLDING //

		Pix DitherToBinary();
		Pix DitherToBinarySpec(int lowerclip, int upperclip);

		// threshold the image -- warning, operates ONLY on grayscale pixmaps
		Pix ThresholdToBinary(int threshold);

		Pix VarThresholdToBinary(Pix &thresholdPix);
		Pix DitherToBinaryLUT(int lowerclip, int upperclip);
		Pix GenerateMaskByValue(int val, bool useCMap = false);
		Pix GenerateMaskByBand(int lower, int upper, int inband, bool useCMap = false);
		Pix DitherTo2bpp(int cmapflag);
		Pix DitherTo2bppSpec(int lowerclip, int upperclip, int cmapflag);
		Pix ThresholdTo2bpp(int nlevels, int cmapflag);
		Pix ThresholdTo4bpp(int nlevels, int cmapflag);
		Pix ThresholdOn8bpp(int nlevels, int cmapflag);
		Pix ThresholdGrayArb(const char  *edgevals, int outdepth, int use_average, int setblack, int setwhite);
		Buffer<int> MakeGrayQuantIndexTable(int nlevels);
		Buffer<int> MakeGrayQuantTargetTable(int nlevels, int depth);
		Pix GenerateMaskByBand32(unsigned refval, int delm, int delp);
		Pix GenerateMaskByDiscr32(unsigned refval1, unsigned refval2, int distflag);
		Pix GrayQuantFromHisto(Pix &mPix, double minfract, int maxsize);

		Pix ThresholdToValue(int threshval, int setval);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// COLOR CONVERSION //

		// converts pix to monochrome 1bpp
		Pix ConvertTo1(int threshold);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// SKEW REMOVAL //

		// Simple top-level deskew interfaces
		Pix Deskew(int ReductionFactor = 1);
		Pix FindSkewAndDeskew(int ReductionFactor, double *skewAngle = NULL, double *confidenceFactor = NULL);

		// Simple top-level skew angle finding interface
		bool FindSkew(double *pangle, double *pconf = NULL);
		
		// Basic angle-finding functions with all parameters
		bool FindSkewSweep(double *pangle, int reduction, double sweeprange, double sweepdelta);

		// Angle-finding functions with all parameters
		bool FindSkewSweepAndSearch(
			double *pangle, double *pconf,
			int redsweep, int redsearch,
			double sweeprange, double sweepdelta,
			double minbsdelta);

		Pix DeskewLocal();

		// NOTE : Leptonica has many more skew functions....
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// ROTATION FUNCTIONS 

 		// Rotates image (2-4-8 or 32 bpp) about center.
		Pix RotateAM(double angle, BringInModes incolor = PIXRASTER_BRING_IN_WHITE);
		Pix RotateAMColor(double angle, int colorval);
		Pix RotateAMGray(double angle, int grayval);
		Pix Rotate(double angle, int type = PIXRASTER_ROTATE_SHEAR, int incolor = PIXRASTER_BRING_IN_WHITE, int width = 0, int height = 0);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// MORPHING FUNCTIONS 

		Pix ErodeGray(int hsize, int vsize);
		Pix DilateGray(int hsize, int vsize);
		Pix OpenGray(int hsize, int vsize);
		Pix CloseGray(int hsize, int vsize);
		
		Pix ErodeBrick(int hsize, int vsize);
		Pix DilateBrick(int hsize, int vsize);
		Pix OpenBrick(int hsize, int vsize);
		Pix CloseBrick(int hsize, int vsize);
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// ARITHMETIC AND ALIKE FUNCTIONS
		
		Pix Invert(); 
		Pix AddConstantGray(int val);
		Pix MultConstantGray(int val);
		Pix AddGray(Pix &pix2);
		Pix SubtractGray(Pix &pix2);
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// BLENDING/COMBINING FUNCTIONS

		Pix CombineMasked(Pix &aPix, Pix &maskPix);

		// extracts a rectangular area from PIX
		Pix ClipRectangle(Rect r);
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// SCALE/RESIZE FUNCTIONS

		// toplevel scale function
		Pix Scale(double scalex, double scaley);
		Pix Fit(int width, int height, bool keepRatio = true);

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// DOCUMENT PAGE SEGMENTING FUNCTIONS
		
		// top level page segmenting
		PixRaster GetRegionsBinary() ;

		// text baseline finding routine
		Array<int> FindBaselines();

		// foreground bounding box locating routine
		// scans from a given border through image center to locate
		// first non-background pixel.
		// scanFlag is one of PIXRASTER_FROM_LEFT, PIXRASTER_FROM_RIGHT, PIXRASTER_FROM_TOP, PIXRASTER_FROM_BOTTOM
		// returns true on success, false otherwise
		bool ScanForForeground(int &loc, enum ScanModes scanMode);

}; // END Class Pix 

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// raster class -- array of Pix objects
class PixRaster : public PixBase
{
	protected:
	
		// array of Pixs
		Array<Pix>pages;
		
		// currently active page for multipage images
		int curPage;
		
		// gets real page
		int getTruePage(int page = PIXRASTER_CURPAGE);
		
	public:
	
		// gets PIX from PixRaster
		// WARNING -- Pix has ownership of it, so DON'T free nor destroy it
		PIX *GetPIX(int page = PIXRASTER_CURPAGE);
		operator PIX*() { return GetPIX(PIXRASTER_CURPAGE); }
		
		// constructor -- empty raster
		PixRaster();

		// constructor -- takes a Leptonica Pix
		// PixRaster TAKES OWNERSHIP of underlying PIX object which is NULLed
		PixRaster(PIX **pix);
		
		// constructor -- takes an array of Leptonica Pix
		// PIXA gets destroyed and NULLed
		PixRaster(PIXA **pixa);

		// creation of Pix from a raster object
		PixRaster(Raster &raster, bool deepCopy = false);
		
		// destructor
		~PixRaster();
		
		// loads Pix from another raster object
		void Load(Raster& raster, bool Append = false, bool deepCopy = false);
		
		// sets/appends a source raster
		PixRaster &operator=(Raster &raster) { Load(raster, false, false); return *this; }
		PixRaster &operator+=(Raster &raster) { Load(raster, true, false); return *this; }
		
		// deep copy of source raster
		PixRaster &operator <<=(Raster &raster) { Load(raster, false, true); return *this; }
		
		// page handling
		virtual void SeekPage(int page);
		virtual int GetPageCount();
		virtual int GetActivePage();

		// extended Raster functions -- they allow to query
		// a given page inside raster
		virtual Size GetSizeEx(int page);
		virtual Info GetInfoEx(int page);
		virtual Line GetLineEx(int line, int page);
		virtual int GetPaletteCountEx(int page);
		virtual const RGBA *GetPaletteEx(int page);
		virtual const RasterFormat *GetFormatEx(int page);
		
		// gets polygon markers
		virtual Markers *GetMarkers() { return GetMarkersEx(PIXRASTER_CURPAGE); }
		virtual Markers *GetMarkersEx(int page);
		
		// check whether pixraster has images
		bool IsEmpty(void) { return !pages.GetCount(); };
		
		// sets Pix
		void Set(Pix &_pix, int page = PIXRASTER_CURPAGE, bool deepCopy = false);
		
		// adds a Pix or a PixRaster
		void Add(Pix &pix, bool deepCopy = false);
		void Add(PixRaster &pix, bool deepCopy = false);
		
		// inserts a PIX or a PIX array
		void Insert(Pix &pix, int where = PIXRASTER_CURPAGE, bool deepCopy = false);
		void Insert(PixRaster &pixR, int where = PIXRASTER_CURPAGE, bool deepCopy = false);
		
		// removes a PIX or a series of PIX
		void Remove(int startPage = PIXRASTER_CURPAGE, int count = 1);
		
		// duplicates a page at the end of array (useful for stack-like operations)
		// NOTE1: deep copy of underlying PIX
		// NOTE2: it seeks added page too
		Pix &Dup(int page = PIXRASTER_CURPAGE);
		
		// Drops last page(s) of array (ditto)
		// NOTE: it seeks last page too
		void Drop(int count = 1);
		
		// Pix access operator
		Pix &operator[](int page) { return At(page); }
		
		// access current page
		operator Pix&() { return At(PIXRASTER_CURPAGE); }
		
		// array access
		Pix &At(int page);
		
		// empties the image
		virtual void Clear() { pages.Clear(); }

		// file I/O
		bool Load(FileIn &fs, bool Append = false);
		bool Load(String fileName, bool Append = false);
		bool Save(String fileName, int page = PIXRASTER_CURPAGE); // @@ to do - add compression and type handling
		bool SaveAll(String fileName);
		
		rval_default(PixRaster);
}; // END class PixRaster

END_UPP_NAMESPACE

#endif
