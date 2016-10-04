#include <Core/Core.h>

using namespace Upp;

struct Item : public Moveable<Item> {
	String name;

	~Item() { LOG("~Item"); }
};

struct Collection : public Item
{
	Vector<Item> items; // Heap leak if changed to Array

	~Collection() { LOG("~Collection"); }
};

CONSOLE_APP_MAIN
{
	Collection *col = new Collection();
	col->items.Add();
	Item *item = (Item *)col;
	delete item;
}
