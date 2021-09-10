// string_1.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
using namespace std;
//class bothWhiteSpaces {
//private:
//	const locale& loc;    // locale
//public:
//	/* constructor
//	* - save the locale object
//	*/
//	bothWhiteSpaces(const locale& l) : loc(l) {
//	}
//	/* function call
//	* - returns whether both characters are whitespaces
//	*/
//	bool operator() (char elem1, char elem2) {
//		return isspace(elem1, loc) && isspace(elem2, loc);
//	}
//};

int main(int argc, char* argv[])
{
	string filename, basename, extname, tmpname;
	const string suffix("tmp");

	/* for each command-line argument
	* (which is an ordinary C-string)
	*/
	for (int i = 1; i<argc; ++i) {
		// process argument as file name
		filename = argv[i];

		// search period in file name
		string::size_type idx = filename.find('.');
		if (idx == string::npos) {
			// file name does not contain any period
			tmpname = filename + '.' + suffix;
		}
		else {
			/* split file name into base name and extension
			* - base name contains all characters before the period
			* - extension contains all characters after the period
			*/
			basename = filename.substr(0, idx);
			extname = filename.substr(idx + 1);
			if (extname.empty()) {
				// contains period but no extension: append tmp
				tmpname = filename;
				tmpname += suffix;
			}
			else if (extname == suffix) {
				// replace extension tmp with xxx
				tmpname = filename;
				tmpname.replace(idx + 1, extname.size(), "xxx");
			}
			else {
				// replace any extension with tmp
				tmpname = filename;
				tmpname.replace(idx + 1, string::npos, suffix);
			}
		}

		// print file name and temporary name
		cout << filename << " => " << tmpname << endl;
	}

	////////////////////////////////////////////////////////////
	const string delims(" \t,.;");
	string line;

	// for every line read successfully
	while (getline(cin, line)) {
		string::size_type begIdx, endIdx;

		// search beginning of the first word
		begIdx = line.find_first_not_of(delims);

		// while beginning of a word found
		while (begIdx != string::npos) {
			// search end of the actual word
			endIdx = line.find_first_of(delims, begIdx);
			if (endIdx == string::npos) {
				// end of word is end of line
				endIdx = line.length();
			}

			// print characters in reverse order
			for (int i = endIdx - 1; i >= static_cast<int>(begIdx); --i) {
				cout << line[i];
			}
			cout << ' ';

			// search beginning of the next word
			begIdx = line.find_first_not_of(delims, endIdx);
		}
		cout << endl;
	}

	//////////////////////////////////////////////////
	//string contents;

	//// don't skip leading whitespaces
	//cin.unsetf(ios::skipws);

	//// read all characters while compressing whitespaces
	//unique_copy(istream_iterator<char>(cin),    // beginning of source
	//	istream_iterator<char>(),       // end of source
	//	back_inserter(contents),        // destination
	//	bothWhiteSpaces(cin.getloc())); // criterion for removing

	//									// process contents
	//									// - here: write it to the standard output
	//cout << contents;
}

