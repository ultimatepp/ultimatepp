#ifndef MAP_COMMON__I_MAP_EVENT_H
#define MAP_COMMON__I_MAP_EVENT_H

#include <CtrlLib/CtrlLib.h>
using namespace Upp;

class IMapEvent
{
public:
	virtual ~IMapEvent() {}

	virtual bool ProcessLeftDouble (Point p, dword keyflags) { return false; }
	virtual bool ProcessLeftDown   (Point p, dword keyflags) { return false; }
	virtual bool ProcessLeftDrag   (Point p, dword keyflags) { return false; }
	virtual bool ProcessLeftUp     (Point p, dword keyflags) { return false; }

	virtual bool ProcessRightDouble(Point p, dword keyflags) { return false; }
	virtual bool ProcessRightDown  (Point p, dword keyflags) { return false; }
	virtual bool ProcessRightDrag  (Point p, dword keyflags) { return false; }
	virtual bool ProcessRightUp    (Point p, dword keyflags) { return false; }

	virtual bool ProcessMouseMove  (Point p, dword keyflags) { return false; }

	virtual bool ProcessMouseEnter() { return false; }
	virtual bool ProcessMouseLeave() { return false; }
};

#endif
