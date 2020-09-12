#ifndef _GTestExample_StringTest_h_
#define _GTestExample_StringTest_h_

#include <Core/Core.h>
#include <plugin/gtest/gtest.h>

namespace Upp {

	class StringTest : public testing::Test {
	protected:
		virtual void SetUp() override;
		
	protected:
		String sEmpty;
		String sCat;
		String sDog;
	};

}

#endif
