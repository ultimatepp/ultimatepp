#ifndef FORM_EDITOR_COMMON__I_PART_BASE_H
#define FORM_EDITOR_COMMON__I_PART_BASE_H

#include "IHistory.h"

class IPartBase
{
public:
	virtual ~IPartBase() {}

	virtual String GetObjectClass() const { return String(); }
	virtual String GetObjectWidgetClass() const { return String(); }

	virtual void AddToHistory(IHistoryItem* item) {}
};

#endif
