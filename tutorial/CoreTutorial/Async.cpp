#include "Tutorial.h"

void AsyncTutorial()
{
	/// .`AsyncWork`
	
	/// `AsyncWork` is `CoWork` based tool that resembles std::future. `AsyncWork` instances
	/// are created using `Async` function and represent a work that can be done in parallel
	/// with current thread. `AsyncWork` supports returning values. A call to `AsyncWork::Get`
	/// makes sure that a work routine was finished and returns the return value (if any):

	auto a = Async([](int n) -> double {
		double f = 1;
		for(int i = 2; i <= n; i++)
			f *= i;
		return f;
	}, 100);
	
	DUMP(a.Get());
	
	/// Exceptions thrown in Async work are propagated upon call to `Get`:
	
	auto b = Async([] { throw "error"; });
	
	try {
		b.Get();
	}
	catch(...) {
		LOG("Exception has been caught");
	}
	
	/// `AsyncWork` instances can be canceled (and are canceled in destructor if Get is not
	/// called on them):
	
	{
		auto c = Async([] {
			for(;;)
				if(CoWork::IsCanceled()) {
					LOG("Work was canceled");
					break;
				}
		});
		Sleep(100); // give it chance to start
		// c destructor cancels the work (can be explicitly canceled by Cancel method too)
	}
	
	///
}
