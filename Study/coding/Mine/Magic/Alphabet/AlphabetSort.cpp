#include <string> 
#include <vector>
using namespace std;

bool isAlphabet(char c)
{
	return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') ? true : false; 
}
string transformToJadenCase(string s)
{
	for (int i = 0; i < s.size(); i++)
	{
		if ((i == 0 || s[i - 1] == ' ') && isAlphabet(s[i]) && islower(s[i])) { s[i] = toupper(s[i]);
		}
		else if ((i != 0 && s[i - 1] != ' ') && isAlphabet(s[i]) && isupper(s[i]))
		{
			s[i] = tolower(s[i]); 
		} 
	} return s; 
} 
string solution(string s) 
{
	return transformToJadenCase(s); 
}
