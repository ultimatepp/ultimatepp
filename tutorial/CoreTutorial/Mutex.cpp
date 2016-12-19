#include "Tutorial.h"

void MutexTutorial()
{
	/// .`Mutex`
	
	/// Mutex ("mutual exclusion") is a well known concept in multithreaded programming: When
	/// multiple threads write and read the same data, the access has to be serialized using
	/// Mutex. Following invalid code demonstrates why:
	
	Thread t;
	
	int sum = 0;
	t.Run([&sum] {
		for(int i = 0; i < 1000000; i++)
			sum++;
	});
	
	for(int i = 0; i < 1000000; i++)
		sum++;
	
	t.Wait();
	DUMP(sum);
	
	/// While the expected value is 2000000, produced value is different. The problem is that
	/// both thread read / modify / write `sum` value without any locking. Using `Mutex` locks
	/// the `sum` and thus serializes access to it - read / modify / write sequence  is now
	/// exclusive for the thread that has `Mutex` locked, this fixing the issue. `Mutex` can be
	/// locked / unlocked with `Enter` / `Leave` methods. Alternatively, `Mutex::Lock` helper
	/// class locks `Mutex` in constructor and unlocks it in destructor:
	
	Mutex m;
	sum = 0;
	t.Run([&sum, &m] {
		for(int i = 0; i < 1000000; i++) {
			m.Enter();
			sum++;
			m.Leave();
		}
	});
	
	for(int i = 0; i < 1000000; i++) {
		Mutex::Lock __(m); // Lock m till the end of scope
		sum++;
	}
	
	t.Wait();
	DUMP(sum);
	
	///
}