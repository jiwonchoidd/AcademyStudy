#include <functional>
#include <string>
#include <iostream>
//placeholders : (���־�)���� �ִ� �ٸ� ���� ����ϴ� ��ȣ�� �ؽ�Ʈ�� �Ϻ�
void goodbye(const std::string& s)
{
	std::cout << "kgca " << s << '\n';
}

class Object {
public:
	void hello(const std::string& s)
	{
		std::cout << "KGCA " << s << '\n';
	}
};
int f(int a, int b)
{
	int c = a + b;
	std::cout << c << std::endl;
	return c;
}

int main()
{
	int a1 = 10, a2 = 20;
	// �Ʒ��� ���ε������ ��� 30�̴�.
	std::bind(f, std::placeholders::_1, std::placeholders::_2)(a1, a2);	// f(a1, a2)
	std::bind(f, 10, std::placeholders::_2)(a1, a2);					// f(10, a2)
	std::bind(f, std::placeholders::_1, 20)(a1, a2);					// f(a1, 20)
	std::bind(f, std::placeholders::_2, std::placeholders::_1)(a1, a2);	// f(a2, a1)


	typedef std::function<void(const std::string&)> ExampleFunction;
	Object instance;
	std::string str("Game Academy");
	// Ŭ���� ��� ȣ��
	ExampleFunction f = std::bind(&Object::hello, &instance,std::placeholders::_1);

	// instance.hello(str)
	f(str);

	// �����Լ� ȣ��
	f = std::bind(&goodbye, std::placeholders::_1);
	// goodbye(str)
	f(str);
	return 0;
}
//Hello World
//Goodbye World