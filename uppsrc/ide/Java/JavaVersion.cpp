#include "Java.h"

namespace Upp {

JavaVersion::JavaVersion()
	: mMajor(0)
	, mMinor(0)
{
}

JavaVersion::JavaVersion(int major, int minor)
	: mMajor(major)
	, mMinor(minor)
{
}

bool JavaVersion::IsGreate(int major) const
{
	return mMajor > major;
}

bool JavaVersion::IsGreaterOrEqual(int major) const
{
	return mMajor >= major;
}

bool JavaVersion::IsGreaterOrEqual(int major, int minor) const
{
	return IsGreate(major) || (IsGreaterOrEqual(major) && minor >= mMinor);
}

}
