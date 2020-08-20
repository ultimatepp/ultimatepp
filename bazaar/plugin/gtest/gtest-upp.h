#ifndef GTEST_INCLUDE_GTEST_UPP_H
#define GTEST_INCLUDE_GTEST_UPP_H

#include <Core/config.h>

#if defined(PLATFORM_WIN32) || defined(PLATFORM_WIN64)

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
	
#else

	#define TEST_APP_MAIN \
	void TestMainFn(int argc, char** argv); \
	\
	int main(int argc, char** argv) \
	{ \
		testing::InitGoogleTest(&argc, argv); \
		TestMainFn(argc, argv); \
		return RUN_ALL_TESTS(); \
	} \
	\
	void TestMainFn(int argc, char** argv)
	
#endif

#endif 
