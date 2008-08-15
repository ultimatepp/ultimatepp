#include <Core/Core.h>

using namespace Upp;

struct Tree {
	Tree *left;
	Tree *right;
};

Tree *CreateTree(int n)
{
	if(n <= 0)
		return NULL;
	Tree *t = new Tree;
	t->left = CreateTree(n - 1);
	t->right = CreateTree(n - 1);
	return t;
}

void DeleteTree(Tree *t)
{
	if(t) {
		DeleteTree(t->left);
		DeleteTree(t->right);
		delete t;
	}
}

CONSOLE_APP_MAIN
{
	RTIMING("Tree new/delete");
	for(int i = 0; i < 100; i++)
		DeleteTree(CreateTree(20));
}
