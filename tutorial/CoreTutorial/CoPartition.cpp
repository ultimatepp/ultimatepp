#include "Tutorial.h"

void CoPartitionTutorial()
{
	/// .CoPartition
	
	Vector<int> data;
	for(int i = 0; i < 10000; i++)
		data.Add(i);
	
	int sum = 0;
	
	CoWork co;
	for(int i = 0; i < data.GetCount(); i++)
		co & [i, &sum, &data] { CoWork::FinLock(); sum += data[i]; };
	co.Finish();
	DUMP(sum);
	
	///
	
	sum = 0;
	CoPartition(data, [&sum](const auto& subrange) {
		int partial_sum = 0;
		for(const auto& x : subrange)
			partial_sum += x;
		CoWork::FinLock(); // available as CoPartition uses CoWork
		sum += partial_sum;
	});
	DUMP(sum);
	
	///
	
	sum = 0;
	CoPartition(data.begin(), data.end(), [&sum] (auto l, auto h) {
		int partial_sum = 0;
		while(l != h)
			partial_sum += *l++;
		CoWork::FinLock(); // available as CoPartition uses CoWork
		sum += partial_sum;
	});
	DUMP(sum);
	
	///
	
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