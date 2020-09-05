#ifdef flagTESTING_GOOGLE_TEST_UI_EXAMPLE

#include "AppWindow.h"

#include <plugin/gtest/gtest.h>
#include <memory>

using namespace Upp;

class AppWindowTest : public testing::Test
{
public:
	AppWindowTest()
		: windowRect(0, 0, 200, 200)
	{}
	
	virtual void SetUp() override
	{
		window = MakeOne<AppWindow>();
	}
	
public:
	One<AppWindow> window;
	const Rect windowRect;
};

TEST_F(AppWindowTest, Initialization)
{
	EXPECT_STREQ(L"App Window", window->GetTitle().ToStd().c_str());
	EXPECT_FALSE(window->IsMaximized());
	EXPECT_FALSE(window->IsMinimized());
	
	EXPECT_EQ(windowRect, window->GetRect());
}

TEST_F(AppWindowTest, ApperanceTest)
{
	const String fileName = "MyAppWindow.png";
	
	ImageDraw id(window->GetRect().Size());
	window->DrawCtrl(id);
	
	PNGEncoder encoder;
	if (!FileExists(fileName)) {
		encoder.SaveFile(fileName, id);
		Cout() << "Non referal window image. Creating and failing test..\n";
		ASSERT_TRUE(false);
		return;
	}
	
	auto img = StreamRaster::LoadFileAny(fileName);
	EXPECT_EQ(img, id);
}

#endif
