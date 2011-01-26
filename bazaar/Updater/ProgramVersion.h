#ifndef _Updater_ProgramVersion_h_
#define _Updater_ProgramVersion_h_

#include <CtrlLib/CtrlLib.h>

NAMESPACE_UPP

// Application version handling - holds a version
// with subversion and devel field
// brings som compare functions and String conversion
class ProgramVersion : public Moveable<ProgramVersion>
{
	private:
	
		int major;
		int minor;
		int devel;
	
	protected:
	
	public:
	
		// constructor
		ProgramVersion() { major = minor = devel = 0; }
		ProgramVersion(int major, int minor, int devel);
		ProgramVersion(int major, int minor);
		ProgramVersion(ProgramVersion const &v);
		ProgramVersion(String const &s);
		
		// clearing
		ProgramVersion &Clear() { major = minor = devel = 0; return *this; }
		
		// comparing
		bool operator <(ProgramVersion const &v2) const;
		bool operator <=(ProgramVersion const &v2) const;
		bool operator >(ProgramVersion const &v2) const;
		bool operator >=(ProgramVersion const &v2) const;
		bool operator ==(ProgramVersion const &v2) const;
		
		// assignement
		ProgramVersion &operator=(ProgramVersion const &v);
		ProgramVersion &operator=(String const &s);
		
		// conversion to string
		String ToString(void) const;
		operator String(void) const;
		
		// check if version is a devel one
		bool IsDevel(void) const;
		
		// check for 'empty' version
		bool operator!(void);
		operator bool(void);
};

class ProgramVersions : public Vector<ProgramVersion>
{
	private:
	
	protected:
	
	public:
	
		// constructors
		ProgramVersions();
		ProgramVersions(ProgramVersions const &vs);
		ProgramVersions(ProgramVersions const &vs, int);
		ProgramVersions(String s);
		
		// assignement
		ProgramVersions &operator=(pick_ ProgramVersions &vs);
		
		// load/store to/from string
		// one version per line, format MMMM.mmmm.dddddd
		String ToString(void) const;
		operator String(void) const;
		
		// find next available version -- including or not devel versions
		bool HasNew(ProgramVersion const &v, bool devel) const;
		bool HasNew(ProgramVersion const &v, ProgramVersion const &maxv, bool devel) const;
		ProgramVersion FindMax(ProgramVersion const &v, bool devel) const;
		ProgramVersion FindMax(ProgramVersion const &v, ProgramVersion const &maxv, bool devel) const;
};

END_UPP_NAMESPACE

#endif
