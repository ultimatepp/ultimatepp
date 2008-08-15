#ifndef _src_dbase_utility_h_
#define _src_dbase_utility_h_

bool _isBitSet(int _byte, short _bitno);
void _bitSet(int *ptr, short _bitno);
void _bitReset(int *ptr, short _bitno);
Vector<String> StringToToken(const String &str);

bool lmlEval(String eqt);
bool lplEval(String eqt);

bool EvalWhere(dbRecord &rec1, const String &whr);
bool IsSQLWord(String &_w);

VectorMap<int, Vector<Value> > StringToVector(const String &s);
String VectorToString(VectorMap<int, Vector<Value> > &v);

#endif
