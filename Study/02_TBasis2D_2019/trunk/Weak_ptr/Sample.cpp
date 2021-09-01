#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Controller
{
public:
	int Num;
	wstring Status;
	vector<weak_ptr<Controller>> others;
public:
	explicit Controller(int i) : Num(i), Status(L"On")
	{
		wcout << L"Creating Controller" << Num << endl;
	}

	~Controller()
	{
		wcout << L"Destroying Controller" << Num << endl;
	}
public:
	void CheckStatuses() const
	{
		
		for_each(others.begin(), others.end(), [](weak_ptr<Controller> wp)
		{
			if (!wp.expired()) // ��������
			{
				try
				{
					auto p = wp.lock();
					wcout << L"Status of " << p->Num << " = " << p->Status << endl;
				}

				catch (bad_weak_ptr b)
				{
					wcout << L"Null object" << endl;
				}
			}
		});
	}
};

void RunTest()
{
	vector<shared_ptr<Controller>> v;

	v.push_back(shared_ptr<Controller>(new Controller(0)));
	v.push_back(shared_ptr<Controller>(new Controller(1)));
	v.push_back(shared_ptr<Controller>(new Controller(2)));
	v.push_back(shared_ptr<Controller>(new Controller(3)));
	v.push_back(shared_ptr<Controller>(new Controller(4)));

	for (int i = 0; i < v.size(); ++i)
	{
		for_each(v.begin(), v.end(), [v, i](shared_ptr<Controller> p)
		{
			if (p->Num != i)
			{
				v[i]->others.push_back(weak_ptr<Controller>(p));
				wcout << L"push_back to v[" << i << "]: " << p->Num << endl;
			}
		});
	}

	for_each(v.begin(), v.end(), [](shared_ptr<Controller>& p)
	{
		wcout << L"use_count = " << p.use_count() << endl;
		p->CheckStatuses();
	});
}

int main()
{
	/////////////////////////////////////////////////////////////////
	//weak_ptr�� �����Ϳ��� ���� ���� �Ұ����ϱ� ������ ��� ������ �Լ��� ����� �Ұ��ϴ�.
	//������ ����� lock()�Լ��� ����Ͽ� std::shared_ptr  ��ȯ �޾Ƽ� ó���ϴ� ���̴�.
	//weak_ptr �� ����ī������ ���� �ʴ´�.
	std::shared_ptr<int> w1(new int(3));	 // reference count = 1
	std::weak_ptr<int> weak1 = w1;			 // reference count = 1
	{
		//lock() �Լ��� std::shared_ptr ��ü�� ���� ��ȯ�Ѵ�.
		auto sharedObject = weak1.lock();				 // reference count = 2
		cout << sharedObject;
	} // ����� ����鼭 baz�� �����ȴ�.	 //reference count = 1
	w1.reset(); // reference count = 0


	/////////////////////////////////////////////////////////////////
	RunTest();
	wcout << L"Press any key" << endl;
	char ch;
	cin.getline(&ch, 1);
}