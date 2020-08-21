#include <plugin/gmock/gmock.h>
#include <plugin/gtest/gtest.h>

#include <Core/Core.h>

using namespace Upp;

const String CAR_REGISTRATION_NUMBER = "UPP IS THE BEST";

class Car {
public:
	virtual ~Car() {}
	
	virtual void OpenHood() = 0;
	virtual String ReadRegistrationNumbers() = 0;
};

class MockCar final : public Car {
public:
	MOCK_METHOD(void, OpenHood, (), (override));
	MOCK_METHOD(String, ReadRegistrationNumbers, (), (override));
};

class CarRepairShop final {
public:
	CarRepairShop()
		: carsWaitingForService({ CAR_REGISTRATION_NUMBER })
	{}

	bool Service(Car& car) {
		if (carsWaitingForService.Find(car.ReadRegistrationNumbers()) < 0) {
			return false;
		}
		
		car.OpenHood();
		
		return true;
	}
	
private:
	Index<String> carsWaitingForService;
};

TEST(CarRepairShopTest, ServiceChecksAllMainCarElements) {
	MockCar car;
	CarRepairShop repairShop;
	
	EXPECT_CALL(car, ReadRegistrationNumbers()).WillOnce(::testing::Return(String(CAR_REGISTRATION_NUMBER)));
	EXPECT_CALL(car, OpenHood()).Times(::testing::AtLeast(1));
	
	EXPECT_TRUE(repairShop.Service(car));
}

TEST_APP_MAIN {}
