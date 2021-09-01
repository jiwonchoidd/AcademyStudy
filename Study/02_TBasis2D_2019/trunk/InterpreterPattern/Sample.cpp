//interpreter는 '통역자'란 뜻이다. 
//대부분의 프로그래밍 언어가 컴파일러(compiler)를 사용해 기계어로 번역하는 
//반면 일부는 인터프리터(interpreter) 방식으로 번역한다. 
//인터프리터 방식은 코드를 한 줄씩 해석하여 해당 작업을 바로 수행하는 방식으로 전체 코드를 해석하고 수행해간다.

#include <iostream>
#include <fstream>
#include <string>
#include <strstream>
using namespace std;

static const int NA_POS = -1;

class RegularExp 
{
public:
	virtual bool Match(string context) = 0;
};

class LiteralExp : public RegularExp 
{
public:
	LiteralExp(const char *pStr) : literal_(pStr) { }
	LiteralExp(const string str) : literal_(str) { }

	bool Match(string context) 
	{
		string str;
		ifstream ifs(context.data());
		while (!ifs.eof()) 
		{
			ifs >> str;
			if (literal_ == str)
				return true;
		}
		return false;
	}
private:
	string literal_;
};

class OrExp : public RegularExp 
{
public:
	OrExp(RegularExp *pExp1, RegularExp *pExp2)
		: pOrExp1_(pExp1), pOrExp2_(pExp2) { }

	bool Match(string context) 
	{
		if (pOrExp1_->Match(context))
			return true;
		else {
			return pOrExp2_->Match(context);
		}
	}
private:
	RegularExp * pOrExp1_;
	RegularExp *pOrExp2_;
};

class AndExp : public RegularExp 
{
public:
	AndExp(RegularExp *pExp1, RegularExp *pExp2)
		: pAndExp1_(pExp1), pAndExp2_(pExp2) { }

	bool Match(string context) 
	{
		return pAndExp1_->Match(context) && pAndExp2_->Match(context);
	}
private:
	RegularExp * pAndExp1_;
	RegularExp *pAndExp2_;
};

RegularExp * CreateRegularExp(string searchStr)
{
	int len = searchStr.length();
	if (len == 0) return NULL;
	else
		cout << "===>" << searchStr << endl;

	int pos = searchStr.find_first_of("(&|");
	if (searchStr[pos] == '(') {
		int endParenPos = 0;
		int parenCnt = 1;
		for (int i = pos + 1; i < len; i++) 
		{
			if (searchStr[i] == '(') parenCnt++;
			else if (searchStr[i] == ')') parenCnt--;
			else {}

			if (parenCnt == 0) 
			{
				int nextOpPos = searchStr.find_first_of("&|", i + 1);
				if (nextOpPos != NA_POS) 
				{
					if (searchStr[nextOpPos] == '&')
						return new AndExp(CreateRegularExp(searchStr.substr(pos + 1, i - pos - 1)),
							CreateRegularExp(searchStr.substr(nextOpPos + 1,
								len - nextOpPos - 1)));
					else
						return new OrExp(CreateRegularExp(searchStr.substr(pos + 1, i - pos - 1)),
							CreateRegularExp(searchStr.substr(nextOpPos + 1,
								len - nextOpPos - 1)));
				}
				else
					return CreateRegularExp(searchStr.substr(pos + 1, i - pos - 1));
			}
		}
		// -- searchStr 수식이 잘못된 것임
		return NULL;
	}
	else if (searchStr[pos] == '&') 
	{
		if (pos >= len - 1) return NULL;
		return new AndExp(CreateRegularExp(searchStr.substr(0, pos)),
			CreateRegularExp(searchStr.substr(pos + 1, len - pos - 1)));
	}
	else if (searchStr[pos] == '|') 
	{
		if (pos >= len - 1) return NULL;
		return new OrExp(CreateRegularExp(searchStr.substr(0, pos)),
			CreateRegularExp(searchStr.substr(pos + 1, len - pos - 1)));
	}
	else 
	{
		// -- 앞뒤 White-space 제거
		string literal;
		strstream strm;
		strm << searchStr;
		strm >> literal;
		if (literal.empty())
			return NULL;

		return new LiteralExp(literal);
	}
}

void main()
{
	string str;
	getline(cin, str);

	RegularExp *pRegExp = CreateRegularExp(str);
	if (pRegExp == NULL) 
	{
		cout << "Search Pattern Error" << endl;
		exit(0);
	}

	if (pRegExp->Match("data.txt"))
		cout << "Found the search string" << endl;
	else
		cout << "Not Exist the search string" << endl;
}
