#include "Tutorial.h"

void CoLoopTutorial()
{
	/// .CoWork loop method
	
	/// An alternative to `CoPartition` is 'loop' method of `CoWork` (`Loop` method with synonym
	/// `operator*`). In this pattern, the job is simply started in all threads and the code is
	/// responsible for scheduling the work. `Loop` / `operator*` waits for all started threads
	/// to finish. For scheduling, it is possible to use internal `CoWork` index counter,
	/// which is set to 0 at the start of `Loop` / `operator*`. This way, the overhead associated
	/// with creating lambdas and scheduling them is kept to the minimum.
	
	Vector<String> data;
	for(int i = 0; i < 100; i++)
		data.Add(AsString(1.0 / i));
	
	double sum = 0;
	
	CoWork co;
	co * [&] {
		int i;
		double m = 0;
		while((i = co.Next()) < data.GetCount())
			m += atof(data[i]);
		CoWork::FinLock();
		sum += m;
	};

	DUMP(sum);
	
	///
}
