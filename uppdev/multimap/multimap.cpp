#include <Core/Core.h>
#include <iostream>
#include <map>
#include <string>
using namespace std;

CONSOLE_APP_MAIN
{
  string filename;
  multimap<string,string> wpairs;
  typedef multimap<string,string>::iterator mit;
  wpairs.clear();
  wpairs.insert(pair<string,string>("butor","asztal"));
  wpairs.insert(pair<string,string>("butor","szek"));
  wpairs.insert(pair<string,string>("butor","szekreny"));
  wpairs.insert(pair<string,string>("gyumolcs","alma"));
  wpairs.insert(pair<string,string>("gyumolcs","barack"));
  wpairs.insert(pair<string,string>("gyumolcs","cseresznye"));
  wpairs.insert(pair<string,string>("gyumolcs","korte"));
  wpairs.insert(pair<string,string>("gyumolcs","meggy"));
  wpairs.insert(pair<string,string>("gyumolcs","szilva"));
  wpairs.insert(pair<string,string>("fa","fenyo"));
  wpairs.insert(pair<string,string>("fa","hars"));
  wpairs.insert(pair<string,string>("fa","nyar"));
  wpairs.insert(pair<string,string>("fa","nyir"));
  wpairs.insert(pair<string,string>("fa","ostorfa"));
  wpairs.insert(pair<string,string>("fa","platan"));
  wpairs.insert(pair<string,string>("fa","tolgy"));
  wpairs.insert(pair<string,string>("aaa","ak"));
  wpairs.insert(pair<string,string>("bbb","bek"));
  wpairs.insert(pair<string,string>("ccc","cek"));
  wpairs.insert(pair<string,string>("ddd","dek"));
  wpairs.insert(pair<string,string>("ooo","ok"));
  wpairs.insert(pair<string,string>("zzz","zek"));
  string b = "fa";
  mit lower = wpairs.lower_bound(b);
  mit upper = wpairs.upper_bound(b);
  cout << lower->second << endl;
  cout << upper->second << endl;
  cout << wpairs.count(b);
}
