#ifndef _KanjiFlash_Kanji_h_
#define _KanjiFlash_Kanji_h_

class Kanji: public Moveable<Kanji>
{
	String literal;
	int grade;
	int sc;
	int indRMK;
public:
	Kanji&	Literal(String _literal) 			{ literal = _literal; return *this; }
	String	Literal() const						{ return literal; }

	Kanji&	StrokeCount(int _literal) 			{ sc = _literal; return *this; }
	int		StrokeCount() const					{ return sc; }

	Kanji&	Grade(int _grade)					{ grade = _grade; return *this; }
	int		Grade() const						{ return grade; }
	String	GradeAsString() const;

	int		GetDictIndex(int _dict)	const		{ return indRMK; }
	void	SetDictIndex(int _dict, int _index)	{ indRMK = _index; }

	Kanji(): literal(0), grade(0), sc(0), indRMK(-1) {}
};

#endif
