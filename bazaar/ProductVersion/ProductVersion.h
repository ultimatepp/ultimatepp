#ifndef _ProductVersion_h
#define _ProductVersion_h

#include <Core/Core.h>

namespace Upp
{

// Application version handling - holds a version
// with subversion and devel field
// brings som compare functions and String conversion
class ProductVersion : public Moveable<ProductVersion>
{
	private:
	
		int major;
		int minor;
		int devel;
	
	protected:
	
	public:
	
		// constructor
		ProductVersion() { major = minor = devel = 0; }
		ProductVersion(int major, int minor, int devel);
		ProductVersion(int major, int minor);
		ProductVersion(ProductVersion const &v);
		ProductVersion(String const &s);
		
		// clearing
		ProductVersion &Clear() { major = minor = devel = 0; return *this; }
		
		// comparing
		bool operator <(ProductVersion const &v2) const;
		bool operator <=(ProductVersion const &v2) const;
		bool operator >(ProductVersion const &v2) const;
		bool operator >=(ProductVersion const &v2) const;
		bool operator ==(ProductVersion const &v2) const;
		
		// assignement
		ProductVersion &operator=(ProductVersion const &v);
		ProductVersion &operator=(String const &s);
		
		// conversion to string
		String ToString(void) const;
		operator String(void) const;
		
		// check if version is a devel one
		bool IsDevel(void) const;
		
		// check for 'empty' version
		bool operator!(void);
		operator bool(void);
		
		// gets/sets partial numbers
		ProductVersion &SetMajor(int maj) { major = maj; return *this; }
		ProductVersion &SetMinor(int min) { minor = min; return *this; }
		ProductVersion &SetDevel(int dev) { devel = dev; return *this; }
		int GetMajor(void) { return major; }
		int GetMinor(void) { return minor; }
		int GetDevel(void) { return devel; }
		ProductVersion &IncMajor(int delta = 1) { major += delta; return *this; }
		ProductVersion &IncMinor(int delta = 1) { minor += delta; return *this; }
		ProductVersion &IncDevel(int delta = 1) { devel += delta; return *this; }
		
		// xml support
		void Xmlize(XmlIO &xml);
};

class ProductVersions : public Vector<ProductVersion>
{
	private:
	
	protected:
	
	public:
	
		// constructors
		ProductVersions();
		ProductVersions(ProductVersions rval_ vs);
		ProductVersions(ProductVersions const &vs, int);
		ProductVersions(String s);
		
		// assignement
		ProductVersions &operator=(ProductVersions rval_ vs);
		
		// load/store to/from string
		// one version per line, format MMMM.mmmm.dddddd
		String ToString(void) const;
		operator String(void) const;
		
		// find next available version -- including or not devel versions
		bool HasNew(ProductVersion const &v, bool devel) const;
		bool HasNew(ProductVersion const &v, ProductVersion const &maxv, bool devel) const;
		ProductVersion FindMax(ProductVersion const &v, bool devel) const;
		ProductVersion FindMax(ProductVersion const &v, ProductVersion const &maxv, bool devel) const;
};

}; // end upp namespace

#endif

