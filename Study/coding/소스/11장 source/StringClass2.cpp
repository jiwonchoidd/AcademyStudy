/*
   StringClass2.cpp   
*/
#include <iostream>

using std::endl;
using std::cout;
using std::cin;

using std::ostream;
using std::istream;

class string{
	int len;
	char* str;
public:
	string(const char* s=NULL);
	string(const string& s);
	~string();
	string& operator=(const string& s);
	string& operator+=(const string& s);
	bool operator==(const string& s);
	string operator+(const string& s);

	friend ostream& operator<<(ostream& os, const string& s);
	friend istream& operator>>(istream& is, string& s);
};

string::string(const char* s){
	len=(s!=NULL ? strlen(s)+1 : 1);
	str=new char[len];

	if(s!=NULL)
		strcpy(str, s);
}

string::string(const string& s){
	len=s.len;
	str=new char[len];
	strcpy(str, s.str);
}

string::~string(){
	delete []str;
}

string& string::operator=(const string& s){
	delete []str;
	len=s.len;
	str=new char[len];
	strcpy(str, s.str);
	return *this;
}

string& string::operator+=(const string& s){
	len=len+s.len-1;
	char* tStr=new char[len];
	strcpy(tStr, str);
	delete []str;

	strcat(tStr, s.str);
	str=tStr;
	return *this;
}

bool string::operator==(const string& s){
	return strcmp(str, s.str)? false:true;
}

string string::operator+(const string& s){
	char* tStr=new char[len+s.len-1];
	strcpy(tStr, str);
	strcat(tStr, s.str);
	
	string temp(tStr);
	delete []tStr;
	return temp;
}

ostream& operator<<(ostream& os, const string& s){
	os<<s.str;
	return os;
}

istream& operator>>(istream& is, string& s){
	char str[100];
	is>>str;

	s=string(str);
	return is;
}

// main 함수는 StringClass1.cpp와 같다.
int main()
{
	string str1="Good ";
	string str2="morning";
	string str3=str1+str2;

	cout<<str1<<endl;
	cout<<str2<<endl;
	cout<<str3<<endl;

	str1+=str2;
	
	if(str1==str3) {   //str1과 str3의 내용 비교.	
		cout<<"equal!"<<endl;
	}

	string str4;
	
	cout<<"문자열 입력: ";
	cin>>str4;
	cout<<"입력한 문자열: "<<str4<<endl;
	
	return 0;
}
