//하나의 추상 클래스에 execute() 메서드를 하나 만들고 각 명령이 들어오면 
//그에 맞는 서브 클래스(copy_command)가 선택되어 실행하는 것이 효율적이다.
//이는 함수 오버로딩(overloading)과 같은 추상화 개념을 사용한 것이다.

#include <iostream>
#include <string>
#include <map>
using namespace std;

static const int NA_POS = -1;

#define CMD_NAME    "cmd"

#define LOGIN_VAL   "login"
#define BBSLIST_VAL "bbslist"
#define BBSREAD_VAL "bbsread"

class Request 
{
public:
	string GetValue(string name) 
	{
		return nvList_[name];
	}
	void SetNameValue(string name, string value) 
	{
		nvList_[name] = value;
	}
private:
	map<string, string> nvList_;
};

class RequestParser 
{
public:
	bool GetRequest(string input, Request& req) 
	{
		string key, value, str;
		int start = 0, pos = 0;
		while (pos != NA_POS) {
			pos = input.find("=", start);
			if (pos == NA_POS) continue;

			key = input.substr(start, pos - start);
			start = pos + 1;

			pos = input.find("&", start);
			if (pos == NA_POS)
				value = input.substr(start, input.length() - start);
			else
				value = input.substr(start, pos - start);
			start = pos + 1;

			if (!key.empty()) {
				req.SetNameValue(DecodeString(key), DecodeString(value));
				key = "";
			}
		}
		return true;
	}
protected:
	string DecodeString(string s) 
	{
		string output;
		int len = s.length();
		for (int i = 0; i < len; i++) 
		{
			if (s[i] == '+')
				output += ' ';
			else if (s[i] == '%') 
			{
				const char *pStr = s.data() + i + 1;
				char ch = Hex2Digit(pStr);
				output += ch;
				i += 2;
			}
			else
				output += s[i];
		}
		return output;
	}
	unsigned int Hex2Digit(const char* hex)
	{
		register char digit;
		digit = (hex[0] >= 'A' ? ((hex[0] & 0x4F) - 'A') + 10 : (hex[0] - '0'));
		digit <<= 4;
		digit += (hex[1] >= 'A' ? ((hex[1] & 0x4F) - 'A') + 10 : (hex[1] - '0'));

		return digit;
	}
private:
};

// ----------------------------------------
class UserManager 
{
public:
	bool CheckPasswd(Request& req) 
	{
		cout << "Passwd OK" << endl;
		return true;
	}
};
//Bulletin Board System
class BBS 
{
public:
	void DisplayList(Request& req) 
	{
		cout << "Display BBS List" << endl;
	}
	void DisplayItem(Request& req) 
	{
		cout << "Display BBS Item" << endl;
	}
};

// ----------------------------------------
class Command 
{
public:
	virtual void Execute(Request& req) = 0;
};

class LoginCommand : public Command 
{
public:
	LoginCommand(UserManager *pUserMan) 
	{
		pUserMan_ = pUserMan;
	}
	void Execute(Request& req) 
	{
		pUserMan_->CheckPasswd(req);
	}
private:
	UserManager * pUserMan_;
};

class ListCommand : public Command 
{
public:
	ListCommand(BBS *pBbs) 
	{
		pBbs_ = pBbs;
	}
	void Execute(Request& req) 
	{
		pBbs_->DisplayList(req);
	}
private:
	BBS * pBbs_;
};

class ReadCommand : public Command 
{
public:
	ReadCommand(BBS *pBbs) 
	{
		pBbs_ = pBbs;
	}
	void Execute(Request& req) 
	{
		pBbs_->DisplayItem(req);
	}
private:
	BBS * pBbs_;
};

// ----------------------------------------
// Global Instance
// ----------------------------------------
BBS _bbs;
UserManager _userMan;
map<string, Command *> _req2cmd;

void RegisterCommand() 
{
	_req2cmd[LOGIN_VAL] = new LoginCommand(&_userMan);
	_req2cmd[BBSLIST_VAL] = new ListCommand(&_bbs);
	_req2cmd[BBSREAD_VAL] = new ReadCommand(&_bbs);
}

void main()
{
	string input;
	RequestParser parser;

	RegisterCommand();

	while (1) {
		cin >> input;

		Request req;
		parser.GetRequest(input, req);

		string cmd = req.GetValue(CMD_NAME);
		Command *pCmd = _req2cmd[cmd];
		if (pCmd != NULL)
			pCmd->Execute(req);
		else
			cout << "Not Available Command : " << cmd << endl;
	}
}
