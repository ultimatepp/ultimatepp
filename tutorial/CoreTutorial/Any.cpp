#include "Tutorial.h"

void AnyTutorial()
{
	/// .`Any`

	/// `Any` is a container that can contain none or one element of %any% type. `Any::Is`
	/// method matches exact type ignoring class hierarchies (unlike `One::Is`). You can use
	/// `Get` to retrieve a reference to the instance stored:

	for(int pass = 0; pass < 2; pass++) {
		Any x;
		if(pass)
			x.Create<String>() = "Hello!";
		else
			x.Create<Color>() = Blue();
		
		if(x.Is<String>())
			LOG("Any is now String: " << x.Get<String>());
		
		if(x.Is<Color>())
			LOG("Any is now Color: " << x.Get<Color>());
	}
	
	///
}