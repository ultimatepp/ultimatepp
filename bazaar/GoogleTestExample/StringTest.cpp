#include "StringTest.h"

namespace Upp {

void StringTest::SetUp()
{
	sCat = "Cat";
	sDog = "Dog";
}

TEST_F(StringTest, TestDefaultConstructor)
{
	String a;
	
	ASSERT_EQ(a, "");
}

TEST_F(StringTest, TestConstructor)
{
	String a("Test");
	ASSERT_EQ(a, "Test");
}

TEST_F(StringTest, TestGetCount)
{
	ASSERT_EQ(sEmpty.GetCount(), 0);
	ASSERT_EQ(sCat.GetCount(), 3);
	ASSERT_EQ(sDog.GetCount(), 3);
}

TEST_F(StringTest, TestClear)
{
	sCat.Clear();
	
	ASSERT_EQ(sCat, "");
	ASSERT_EQ(sCat.GetCount(), 0);
}

TEST_F(StringTest, TestCompare)
{
	ASSERT_EQ(sCat.Compare(sCat), 0);
	ASSERT_EQ(sCat.Compare(sDog), -1);
}

TEST_F(StringTest, TestEqual)
{
	ASSERT_TRUE(sCat.IsEqual(sCat));
	ASSERT_FALSE(sCat.IsEqual(sDog));
}

}
