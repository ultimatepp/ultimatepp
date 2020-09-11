#ifdef flagTESTING_GTEST_UI_EXAMPLE

#include "AppWindow.h"

#include <plugin/gmock/gmock.h>

using namespace Upp;

class AppWindowMock final : public AppWindow {
public:
	MOCK_METHOD(void, OnButtonClick, (), (override));
};

class AppWindowTest : public testing::Test
{
protected:
	AppWindowTest()
		: windowRect(0, 0, 200, 200)
	{}
	
	void SetUp() override
	{
		window = MakeOne<AppWindowMock>();
	}
	
	void clickButton()
	{
		EXPECT_CALL(*window.Get(), OnButtonClick());
		
		window->button.LeftDown(Point(), 0);
		window->button.LeftUp(Point(), 0);
	}
	
protected:
	One<AppWindowMock> window;
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
		ASSERT_TRUE(false) << "Non referal window image found. Creating new one and failing test..\n";
	}
	
	auto img = StreamRaster::LoadFileAny(fileName);
	EXPECT_EQ(img, id);
}

TEST_F(AppWindowTest, ButtonCanBeClick)
{
	clickButton();
}

#endif
