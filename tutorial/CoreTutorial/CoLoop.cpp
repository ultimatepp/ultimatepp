#include "Tutorial.h"

void CoLoopTutorial()
{
	/// .CoDo
	
	/// An alternative to `CoPartition` is `CoDo`. In this pattern, the job is simply started
	/// in all threads and the code is responsible for scheduling the work. `CoDo` waits for
	/// all started threads to finish. Scheduling is the responsibility of client code, but can
	/// be easily managed using the std::atomic counter. This way, the overhead associated with
	/// creating lambdas and scheduling them is kept to the minimum (basically the cost of
	/// atomic increment). Once again, CoDo is based on CoWork, so `CoWork::FinLock` is available.
	
	Vector<String> data;
	for(int i = 0; i < 100; i++)
		data.Add(AsString(1.0 / i));
	
	double sum = 0;

	std::atomic<int> ii(0);

	CoDo([&] {
		double m = 0;
		for(int i = ii++; i < data.GetCount(); i = ii++)
			m += atof(data[i]);
		CoWork::FinLock();
		sum += m;
	});

	DUMP(sum);
	
	///
}
