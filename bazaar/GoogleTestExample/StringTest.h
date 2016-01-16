#ifndef _GoogleTestExample_VectorTest_h_
#define _GoogleTestExample_VectorTest_h_

#include <Core/Core.h>
#include <plugin/gtest/gtest.h>

NAMESPACE_UPP

class StringTest : public testing::Test {
protected:
	virtual void SetUp();
	
protected:
	String sEmpty;
	String sCat;
	String sDog;
};

END_UPP_NAMESPACE

#endif
