#include "Tutorial.h"

void Logging()
{
	///. Logging
	
	/// Logging is a useful technique to trace the flow of the code and examine results. In
	/// this tutorial we will be using logging extensively, so let us start tutorial with the
	/// explanation of logging.

	/// In debug mode and with default settings, macro `LOG` puts string into output log file.
	/// Log file is placed into 'config-directory', which by default is .exe directory in Win32
	/// and ~/.upp/appname in POSIX.

	/// In TheIDE, you can access the log using 'Debug'/'View the log file Alt+L'.
	
	LOG("Hello world");
	
	/// You can log values of various types, as long as they have `AsString` function defined
	/// You can chain values in single `LOG` using `operator<<`:
	
	int x = 123;
	LOG("Value of x is " << x);

	/// As it is very common to log a value of single variable, `DUMP` macro provides a useful
	/// shortcut, creating a log line with the variable name and value:

	DUMP(x);
	
	/// To get the value in hexadecimal code, you can use `LOGHEX` / `DUMPHEX`
	
	DUMPHEX(x);
	String h = "foo";
	DUMPHEX(h);
	
	/// To log the value of a container (or generic Range), you can either use normal
	/// `LOG` / `DUMP`:
	
	Vector<int> v = { 1, 2, 3 };
	
	DUMP(v);
	
	/// or you can use DUMPC for multi-line output:
	
	DUMPC(v);
	
	/// For maps, use DUMPM:
	
	VectorMap<int, String> map = { { 1, "one" }, { 2, "two" } };
	
	DUMP(map);
	
	///
	
	DUMPM(map);
	
	/// All normal `LOG`s are removed in release mode. If you need to log things in release mode,
	/// you need to use `LOG`/`DUMP` variant with '`R`' prefix (`RLOG`, `RDUMP`, `RDUMPHEX`...):
	
	RLOG("This will be logged in release mode too!");
	
	/// Sort of opposite situation is when adding temporary `LOG`s to the code for debugging.
	/// In that case, '`D`' prefixed variants (`DLOG`, `DDUMP`, `DDUMPHEX`...) are handy -
	/// these cause compile error in release mode (unless you define the flag DEBUGCODE in the
	/// main configuration), so will not get forgotten in the code past the release:
	
	DLOG("This would not compile in release mode.");
	
	/// The last flavor of `LOG` you can encounter while reading U++ sources is the one prefixed
	/// with '`L`'. This one is not actually defined in U++ library and is just a convention. On
	/// the start of file, there is usually something like:
	
	#define LLOG(x) // DLOG(x)
	
	/// and by uncommenting the body part, you can activate the logging in that particular file.
	
	/// While logging to .log file is default, there are various ways how to affect logging,
	/// for example following line adjusts logging to output the log both to the console and
	/// .log file:

#if 0
	StdLogSetup(LOG_COUT|LOG_FILE);
#endif

	///
}
