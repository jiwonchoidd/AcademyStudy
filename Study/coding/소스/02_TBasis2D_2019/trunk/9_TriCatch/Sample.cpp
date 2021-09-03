// 13장 에러처리
#include <iostream>

using std::endl;
using std::cout;
using std::cin;
//exception
char* account = "1234";  //계좌번호
int sid = 1122;    //비밀번호
int balance = 1000;  //잔액.

class AccountExpt
{
	char acc[10];
	int sid;
public:
	AccountExpt(char* str, int id) {
		strcpy_s(acc, str);
		sid = id;
	}
	void What() {
		cout << "계좌번호: " << acc << endl;
		cout << "비밀번호: " << sid << endl;
	}
};

int main(void)
{
	char acc[10];
	int id;
	int money;
	while (1)
	{
		try {
			cout << "계좌번호 입력: ";
			cin >> acc;
			cout << "비밀번호 4자리 입력: ";
			cin >> id;
			if (strcmp(account, acc) )
				throw  std::exception("strcmp(account, acc)");
			if ( sid != id)
				throw  AccountExpt(acc, id);			

			cout << "출금액 입력: ";
			cin >> money;
			if (balance < money)
				throw money;

			balance -= money;
			cout << "잔액: " << balance << endl;
			throw 3.14f;
		}
		
		catch (int money) {
			cout << "부족 금액: " << money - balance << endl;
		}
		catch (std::exception& expt) {
			cout << "다음 입력을 다시 한번 확인 하세요" << expt.what() <<endl;
		}
		catch (AccountExpt& expt) {
			cout << "다음 입력을 다시 한번 확인 하세요" << endl;
			expt.What();
		}
		catch (...)
		{
			cout << "부족 금액: " << endl;
		}
	}

	return 0;
}
