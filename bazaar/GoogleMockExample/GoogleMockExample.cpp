#include <Core/Core.h>
#include <plugin/gmock/gmock.h>

using namespace Upp;

class Car {
public:
	virtual ~Car() {}
	
	virtual void OpenHood() = 0;
};

class MockCar : public Car {
public:
	MOCK_METHOD0(OpenHood, void());
};

class CarRepairShop final {
public:
	void Service(Car& car) {
		car.OpenHood();
	}
};

TEST(CarRepairShopTest, ServiceChecksAllMainCarElements) {
	MockCar car;
	
	EXPECT_CALL(car, OpenHood())
		.Times(::testing::AtLeast(1));
	
	CarRepairShop().Service(car);
}

int main(int argc, char *argv[])
{
	testing::InitGoogleTest(&argc, argv);
	
	return RUN_ALL_TESTS();
}
