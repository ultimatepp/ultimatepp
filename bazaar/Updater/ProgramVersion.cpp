#include "ProgramVersion.h"

NAMESPACE_UPP

//////////////////////////////////////////////////////////////////////////////////////////////
// CLASS ProgramVersion
//////////////////////////////////////////////////////////////////////////////////////////////

// constructor
ProgramVersion::ProgramVersion(int _major, int _minor, int _devel)
{
	major = _major;
	minor = _minor;
	devel = _devel;
}

ProgramVersion::ProgramVersion(int _major, int _minor)
{
	major = _major;
	minor = _minor;
	devel = 0;
}

ProgramVersion::ProgramVersion(ProgramVersion const &v)
{
	major = v.major;
	minor = v.minor;
	devel = v.devel;
}

ProgramVersion::ProgramVersion(String const &s)
{
	Vector<String>sv = Split(s, '.', false);
	if(sv.GetCount() > 0)
		major = ScanInt(sv[0]);
	else
		major = 0;
	if(sv.GetCount() > 1)
		minor = ScanInt(sv[1]);
	else
		minor = 0;
	if(sv.GetCount() > 2)
		devel = ScanInt(sv[2]);
	else
		devel = 0;
}

// comparing
bool ProgramVersion::operator <(ProgramVersion const &v2) const
{
	if(major < v2.major)
		return true;
	else if(major > v2.major)
		return false;
	else if(minor < v2.minor)
		return true;
	else if(minor > v2.minor)
		return false;
	else if(devel < v2.devel)
		return true;
	else if(devel > v2.devel)
		return false;
	else
		return false;
}

bool ProgramVersion::operator <=(ProgramVersion const &v2) const
{
	if(major < v2.major)
		return true;
	else if(major > v2.major)
		return false;
	else if(minor < v2.minor)
		return true;
	else if(minor > v2.minor)
		return false;
	else if(devel < v2.devel)
		return true;
	else if(devel > v2.devel)
		return false;
	else
		return true;
}

bool ProgramVersion::operator >(ProgramVersion const &v2) const
{
	return !operator<=(v2);
}

bool ProgramVersion::operator >=(ProgramVersion const &v2) const
{
	return !operator<(v2);
}

bool ProgramVersion::operator ==(ProgramVersion const &v2) const
{
	return major == v2.major && minor == v2.minor && devel == v2.devel;
}

// assignement
ProgramVersion &ProgramVersion::operator=(ProgramVersion const &v)
{
	major = v.major;
	minor = v.minor;
	devel = v.devel;
	return *this;
}

ProgramVersion &ProgramVersion::operator=(String const &s)
{
	Vector<String>sv = Split(s, '.', false);
	if(sv.GetCount() > 0)
		major = ScanInt(sv[0]);
	else
		major = 0;
	if(sv.GetCount() > 1)
		minor = ScanInt(sv[1]);
	else
		minor = 0;
	if(sv.GetCount() > 2)
		devel = ScanInt(sv[2]);
	else
		devel = 0;
	return *this;
}

// conversion to string
String ProgramVersion::ToString(void) const
{
	String s = Format("%d.%d", major, minor);
	if(devel != 0)
		s += Format(".%d", devel);
	return s;
}

ProgramVersion::operator String(void) const
{
	return ToString();
}

// check if version is a devel one
bool ProgramVersion::IsDevel(void) const
{
	return devel != 0;
}

// check for 'empty' version
bool ProgramVersion::operator!(void)
{
	return !operator bool();
}

ProgramVersion::operator bool(void)
{
	return major != 0 || minor != 0 || devel != 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// CLASS ProgramVersions
//////////////////////////////////////////////////////////////////////////////////////////////

// constructors
ProgramVersions::ProgramVersions()
{
}

ProgramVersions::ProgramVersions(ProgramVersions const &vs) : Vector<ProgramVersion>(vs)
{
}

ProgramVersions::ProgramVersions(ProgramVersions const &vs, int) : Vector<ProgramVersion>(vs, 1)
{
}

ProgramVersions::ProgramVersions(String s)
{
	// strip eventually contained \r characters
	int i;
	while( (i = s.Find('\r')) >= 0)
		s.Remove(i);
	
	// split string on newlines
	Vector<String>sv = Split(s, '\n');
	
	// read all versions from vector
	for(i = 0; i < sv.GetCount(); i++)
		Add(sv[i]);
}

// assignement
ProgramVersions &ProgramVersions::operator=(pick_ ProgramVersions &vs)
{
	Vector<ProgramVersion>::operator=(vs);
	return *this;
}

// load/store to/from string
// one version per line, format MMMM.mmmm.dddddd
String ProgramVersions::ToString(void) const
{
	String s;
	for(int i = 0; i < GetCount(); i++)
		s += operator[](i).ToString() + "\n";
	return s;
}

ProgramVersions::operator String(void) const
{
	return ToString();
}

// find next available version -- including or not devel versions
bool ProgramVersions::HasNew(ProgramVersion const &v, bool devel) const
{
	for(int i = 0; i < GetCount(); i++)
	{
		if(!devel && operator[](i).IsDevel())
			continue;
		if(operator[](i) > v)
			return true;
	}
	return false;
}

bool ProgramVersions::HasNew(ProgramVersion const &v, ProgramVersion const &maxv, bool devel) const
{
	for(int i = 0; i < GetCount(); i++)
	{
		if(!devel && operator[](i).IsDevel())
			continue;
		if(operator[](i) > maxv)
			continue;
		if(operator[](i) > v)
			return true;
	}
	return false;
}

ProgramVersion ProgramVersions::FindMax(ProgramVersion const &v, bool devel) const
{
	ProgramVersion maxV(v);
	for(int i = 0; i < GetCount(); i++)
	{
		if(!devel && operator[](i).IsDevel())
			continue;
		if(operator[](i) > maxV)
			maxV = operator[](i);
	}
	return maxV;
}

ProgramVersion ProgramVersions::FindMax(ProgramVersion const &v, ProgramVersion const &maxv, bool devel) const
{
	ProgramVersion maxV(v);
	for(int i = 0; i < GetCount(); i++)
	{
		if(!devel && operator[](i).IsDevel())
			continue;
		if(operator[](i) > maxv)
			continue;
		if(operator[](i) > maxV)
			maxV = operator[](i);
	}
	return maxV;
}


END_UPP_NAMESPACE