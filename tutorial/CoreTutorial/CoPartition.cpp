#include "Tutorial.h"

void CoPartitionTutorial()
{
	/// .CoPartition
	
	/// There is some overhead associated with CoWork worker threads. That is why e.g.
	/// performing a simple operation on the array spawning worker thread for each element is
	/// not a good idea performance wise:
	
	Vector<int> data;
	for(int i = 0; i < 10000; i++)
		data.Add(i);
	
	int sum = 0;
	
	CoWork co;
	for(int i = 0; i < data.GetCount(); i++)
		co & [i, &sum, &data] { CoWork::FinLock(); sum += data[i]; };
	co.Finish();
	DUMP(sum);
	
	/// Above code computes the sum of all elements in the `Vector`, using CoWorker job for
	/// each element. While producing the correct result, it is likely to run much slower than
	/// single-threaded version.
	
	/// The solution to the problem is to split the array into small number of larger subranges
	/// that are processed in parallel. This is what `CoPartition` template algorithm does:
	
	sum = 0;
	CoPartition(data, [&sum](const auto& subrange) {
		int partial_sum = 0;
		for(const auto& x : subrange)
			partial_sum += x;
		CoWork::FinLock(); // available as CoPartition uses CoWork
		sum += partial_sum;
	});
	DUMP(sum);
	
	/// Note that CoWork is still internally used, so `CoWork::FinLock` is available.
	/// Instead of working on subranges, it is also possible to use iterators:
	
	sum = 0;
	CoPartition(data.begin(), data.end(), [&sum] (auto l, auto h) {
		int partial_sum = 0;
		while(l != h)
			partial_sum += *l++;
		CoWork::FinLock(); // available as CoPartition uses CoWork
		sum += partial_sum;
	});
	DUMP(sum);
	
	/// There is no requirement on the type of iterators, so it is even possible to use just
	/// indices:
	
	sum = 0;
	CoPartition(0, data.GetCount(), [&sum, &data] (int l, int h) {
		int partial_sum = 0;
		while(l != h)
			partial_sum += data[l++];
		CoWork::FinLock(); // available as CoPartition uses CoWork
		sum += partial_sum;
	});
	DUMP(sum);
	
	///
}
