#ifndef GTEST_INCLUDE_GTEST_UPP_H
#define GTEST_INCLUDE_GTEST_UPP_H

#define TEST_APP_MAIN \
namespace Upp { extern bool PanicMode; } \
\
void TestMainFn(int argc, char** argv); \
\
int main(int argc, char** argv) \
{ \
	Upp::PanicMode = true; \
\
	testing::InitGoogleTest(&argc, argv); \
	TestMainFn(argc, argv); \
	return RUN_ALL_TESTS(); \
} \
\
void TestMainFn(int argc, char** argv)

#endif
