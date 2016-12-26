#include <Core/Core.h>

using namespace Upp;

void GetRepoInfo(String repo, Date& d, int& rev)
{
	String s = Sys("svn info " + repo);
	LOG("SVN info:");
	LOG(s);

	int q = s.FindAfter("Last Changed Rev: ");
	ASSERT(q >= 0);
	rev = atoi(~s + q);
	ASSERT(rev > 0);

	q = s.FindAfter("Last Changed Date: ");
	ASSERT(q >= 0);
	s = s.Mid(q);
	ASSERT(s.GetCount() > 18);

	// 2014-10-30 01:01:56
	// 0123456789012345678
	d.year = atoi(s);
	d.month = atoi(~s + 5);
	d.day = atoi(~s + 8);
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT|LOG_FILE);
	
	Date d, d1;
	int  rev, rev1;
	
	GetRepoInfo("svn://localhost/upp", d, rev);

	GetRepoInfo("svn://www.ultimatepp.org/upp", d1, rev1);

	LOG("Main repository last date: " << d1);
	LOG("Main repository revision: " << rev1);

	LOG("Mirror repository last date: " << d);
	LOG("Mirror repository revision: " << rev);

	ASSERT(d == d1 && rev == rev1);

	GetRepoInfo("svn://www.ultimatepp.org/upp/trunk/uppsrc", d, rev);
	LOG("upp.src revision: " << rev);
	
	String h = HttpRequest("https://github.com/ultimatepp/mirror/commits/master").Execute();
	LOG("---- GIT");
	LOG(h);
	int q = h.FindAfter("git-svn-id: svn://ultimatepp.org/upp/trunk@");
	
	ASSERT(q >= 0);
	rev1 = atoi(~h + q);
	LOG("GIT mirror revision " << rev1);
	ASSERT(rev == rev1);
	
	LOG("------------------- OK");
}
