// 13�� ����ó��
#include <iostream>

using std::endl;
using std::cout;
using std::cin;
//exception
char* account = "1234";  //���¹�ȣ
int sid = 1122;    //��й�ȣ
int balance = 1000;  //�ܾ�.

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
		cout << "���¹�ȣ: " << acc << endl;
		cout << "��й�ȣ: " << sid << endl;
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
			cout << "���¹�ȣ �Է�: ";
			cin >> acc;
			cout << "��й�ȣ 4�ڸ� �Է�: ";
			cin >> id;
			if (strcmp(account, acc) )
				throw  std::exception("strcmp(account, acc)");
			if ( sid != id)
				throw  AccountExpt(acc, id);			

			cout << "��ݾ� �Է�: ";
			cin >> money;
			if (balance < money)
				throw money;

			balance -= money;
			cout << "�ܾ�: " << balance << endl;
			throw 3.14f;
		}
		
		catch (int money) {
			cout << "���� �ݾ�: " << money - balance << endl;
		}
		catch (std::exception& expt) {
			cout << "���� �Է��� �ٽ� �ѹ� Ȯ�� �ϼ���" << expt.what() <<endl;
		}
		catch (AccountExpt& expt) {
			cout << "���� �Է��� �ٽ� �ѹ� Ȯ�� �ϼ���" << endl;
			expt.What();
		}
		catch (...)
		{
			cout << "���� �ݾ�: " << endl;
		}
	}

	return 0;
}
