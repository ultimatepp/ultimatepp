#include "KanjiFlash.h"

String Kanji::GradeAsString() const
{
	if (grade >= 1 &&  grade <= 6)
		return IntStr(grade);
	else if (grade == 8)
		return "general jouyou";
	else if (grade == 9)
		return "jinmeiyou";
	else
		return "non-jouyou";
}
