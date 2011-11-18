#ifndef FORM_EDITOR_COMMON__I_HISTORY_H
#define FORM_EDITOR_COMMON__I_HISTORY_H

#include <Core/Core.h>
using namespace Upp;

class IHistoryItem
{
public:
	virtual ~IHistoryItem() {}

	virtual String SetDesc(const char* desc) { return String(); }
	virtual String GetDesc() const = 0;
	virtual void   Undo() = 0;
	virtual void   Do() = 0;
};

// Group action
class HBatchAction : public IHistoryItem
{
	Array<IHistoryItem*> _Actions;
	String _Desc;

public:
	virtual  HBatchAction& Add(IHistoryItem* item) { _Actions.Add(item); return *this; }
	virtual ~HBatchAction()
	{
		for (int i = 0; i < _Actions.GetCount(); ++i)
			if (_Actions[i])
				delete _Actions[i];
	}

	virtual String SetDesc(const char* desc) { return _Desc = String(desc); }
	virtual String GetDesc() const { return _Desc; }

	virtual void Undo()
	{
		for (int i = _Actions.GetCount() - 1; i >= 0; --i)
			if (_Actions[i])
				_Actions[i]->Undo();
	}

	virtual void Do()
	{
		for (int i = 0; i < _Actions.GetCount(); ++i)
			if (_Actions[i])
				_Actions[i]->Do();
	}
};

#endif
