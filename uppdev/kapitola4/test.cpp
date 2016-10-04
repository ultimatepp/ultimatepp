#include <iostream>
#include <string>
#include <vector>

using std::istream;
using std::string;
using std::vector;
using std::cin;

istream& read_words(istream& input_stream, vector<string>& words)
{
	words.clear();

	string word;

	while (input_stream >> word)
	{
		words.push_back(word);
	}

	return input_stream;
}

int main()
{
	vector<string> words;

	read_words(cin, words);

	return 0;
}
