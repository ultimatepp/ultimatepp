#include "ProductVersion.h"

NAMESPACE_UPP

//////////////////////////////////////////////////////////////////////////////////////////////
// CLASS ProductVersion
// Small handy class to hold product versions, composed my Major, Minor and devel numbers
// hancles comparaison ant to/from string conversions
//////////////////////////////////////////////////////////////////////////////////////////////

// constructor
ProductVersion::ProductVersion(int _major, int _minor, int _devel)
{
	major = _major;
	minor = _minor;
	devel = _devel;
}

ProductVersion::ProductVersion(int _major, int _minor)
{
	major = _major;
	minor = _minor;
	devel = 0;
}

ProductVersion::ProductVersion(ProductVersion const &v)
{
	major = v.major;
	minor = v.minor;
	devel = v.devel;
}

ProductVersion::ProductVersion(String const &s)
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
bool ProductVersion::operator <(ProductVersion const &v2) const
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

bool ProductVersion::operator <=(ProductVersion const &v2) const
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

bool ProductVersion::operator >(ProductVersion const &v2) const
{
	return !operator<=(v2);
}

bool ProductVersion::operator >=(ProductVersion const &v2) const
{
	return !operator<(v2);
}

bool ProductVersion::operator ==(ProductVersion const &v2) const
{
	return major == v2.major && minor == v2.minor && devel == v2.devel;
}

// assignement
ProductVersion &ProductVersion::operator=(ProductVersion const &v)
{
	major = v.major;
	minor = v.minor;
	devel = v.devel;
	return *this;
}

ProductVersion &ProductVersion::operator=(String const &s)
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
String ProductVersion::ToString(void) const
{
	String s = Format("%d.%d", major, minor);
	if(devel != 0)
		s += Format(".%d", devel);
	return s;
}

ProductVersion::operator String(void) const
{
	return ToString();
}

// check if version is a devel one
bool ProductVersion::IsDevel(void) const
{
	return devel != 0;
}

// check for 'empty' version
bool ProductVersion::operator!(void)
{
	return !operator bool();
}

ProductVersion::operator bool(void)
{
	return major != 0 || minor != 0 || devel != 0;
}

// xml support
void ProductVersion::Xmlize(XmlIO &xml)
{
	String s;
	if(xml.IsStoring())
		s = ToString();
	Upp::Xmlize(xml, s);
	if(xml.IsLoading())
		operator=(s);
}

//////////////////////////////////////////////////////////////////////////////////////////////
// CLASS ProductVersions
//////////////////////////////////////////////////////////////////////////////////////////////

// constructors
ProductVersions::ProductVersions()
{
}

ProductVersions::ProductVersions(ProductVersions rval_ vs) : Vector<ProductVersion>(pick(vs))
{
}

ProductVersions::ProductVersions(ProductVersions const &vs, int) : Vector<ProductVersion>(vs, 1)
{
}

ProductVersions::ProductVersions(String s)
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
ProductVersions &ProductVersions::operator=(ProductVersions rval_ vs)
{
	Vector<ProductVersion>::operator=(pick(vs));
	return *this;
}

// load/store to/from string
// one version per line, format MMMM.mmmm.dddddd
String ProductVersions::ToString(void) const
{
	String s;
	for(int i = 0; i < GetCount(); i++)
		s += operator[](i).ToString() + "\n";
	return s;
}

ProductVersions::operator String(void) const
{
	return ToString();
}

// find next available version -- including or not devel versions
bool ProductVersions::HasNew(ProductVersion const &v, bool devel) const
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

bool ProductVersions::HasNew(ProductVersion const &v, ProductVersion const &maxv, bool devel) const
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

ProductVersion ProductVersions::FindMax(ProductVersion const &v, bool devel) const
{
	ProductVersion maxV(v);
	for(int i = 0; i < GetCount(); i++)
	{
		if(!devel && operator[](i).IsDevel())
			continue;
		if(operator[](i) > maxV)
			maxV = operator[](i);
	}
	return maxV;
}

ProductVersion ProductVersions::FindMax(ProductVersion const &v, ProductVersion const &maxv, bool devel) const
{
	ProductVersion maxV(v);
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
