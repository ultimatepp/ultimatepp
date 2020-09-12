#ifndef GTEST_INCLUDE_GTEST_UPP_H
#define GTEST_INCLUDE_GTEST_UPP_H

#include <Core/config.h>

#if defined(PLATFORM_WIN32) || defined(PLATFORM_WIN64)
	#define TEST_APP_MAIN \
	void TestMainFn(); \
	\
	int main(int argc, char** argv) \
	{ \
		Upp::MemoryIgnoreLeaksBlock __; \
		UPP::AppInit__(argc, (const char **)argv); \
		testing::InitGoogleTest(&argc, argv); \
		UPP::AppExecute__(TestMainFn); \
		int testsResult = RUN_ALL_TESTS(); \
		UPP::AppExit__(); \
		return testsResult; \
	} \
	void TestMainFn()
#else
	#define TEST_APP_MAIN \
	void TestMainFn(); \
	\
	int main(int argc, char **argv, const char **envptr) \
	{ \
		UPP::AppInit__(argc, (const char **)argv, envptr); \
		testing::InitGoogleTest(&argc, argv); \
		UPP::AppExecute__(TestMainFn); \
		int testsResult = RUN_ALL_TESTS(); \
		UPP::AppExit__(); \
		return testsResult; \
	} \
	void TestMainFn()
#endif

#if defined(PLATFORM_WIN32) || defined(PLATFORM_WIN64)
	
	#define TEST_GUI_APP_MAIN \
	void TestGuiMainFn(); \
	\
	int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow) \
	{ \
		Upp::MemoryIgnoreLeaksBlock __; \
		UPP::AppInitEnvironment__(); \
		UPP::Ctrl::InitWin32(hInstance); \
		testing::InitGoogleTest(&__argc, __argv); \
		UPP::AppExecute__(TestGuiMainFn); \
		int testsResult = RUN_ALL_TESTS(); \
		UPP::Ctrl::ExitWin32(); \
		UPP::AppExit__(); \
		return testsResult; \
	} \
	void TestGuiMainFn()

#elif defined(PLATFORM_X11)
	
	#define TEST_GUI_APP_MAIN \
	void TestGuiMainFn(); \
	\
	int main(int argc, char **argv, const char **envptr) \
	{ \
		UPP::AppInit__(argc, (const char **)argv, envptr); \
		UPP::InitGtkApp(argc, argv, envptr); \
		testing::InitGoogleTest(&argc, (char**)argv); \
		TestGuiMainFn(); \
		int testsResult = RUN_ALL_TESTS(); \
		UPP::ExitGtkApp(); \
		UPP::AppExit__(); \
		return testsResult; \
	} \
	void TestGuiMainFn()

#elif defined(PLATFORM_COCOA)
	
	#define TEST_GUI_APP_MAIN \
	void TestGuiMainFn(); \
	\
	int main(int argc, const char **argv, const char **envptr) \
	{ \
		UPP::AppInit__(argc, (const char **)argv, envptr); \
		UPP::CocoInit(argc, argv, envptr); \
		testing::InitGoogleTest(&argc, (char**)argv); \
		TestGuiMainFn(); \
		int testsResult = RUN_ALL_TESTS(); \
		UPP::Ctrl::CloseTopCtrls(); \
		UPP::CocoExit(); \
		return testsResult; \
	} \
	void TestGuiMainFn()

#endif

#endif 
