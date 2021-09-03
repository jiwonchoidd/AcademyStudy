#include <vector>
#include <functional>
#include <algorithm>
#include <iostream>

using namespace std;
class greaterthan15 : unary_function<int, bool>
{
public:
	result_type operator()(argument_type i)
	{
		return (result_type)(i > 15);
	}
};
// bind2nd
// ���� �Լ��� �� ��° �μ��� ������ ���� �����Ͽ� 
// ���� �Լ� ��ü�� ���� �Լ� ��ü�� ��ȯ�ϴ� ����͸� ����� ���� ���ø� �Լ�
// bind1st
// ���� �Լ��� ù ��° �μ��� ������ ���� �����Ͽ� 
// ���� �Լ� ��ü�� ���� �Լ� ��ü�� ��ȯ�ϴ� ����͸� ����� ���� ���ø� �Լ�
int main()
{
	vector<int> v1;
	vector<int>::iterator Iter;

	int i;
	for (i = 0; i <= 5; i++)
	{
		v1.push_back(5 * i);
	}

	cout << "The vector v1 = ( ";
	for (Iter = v1.begin(); Iter != v1.end(); Iter++)
		cout << *Iter << " ";
	cout << ")" << endl;

	// Count the number of integers > 10 in the vector
	vector<int>::iterator::difference_type result1a;
	result1a = count_if(v1.begin(), v1.end(), bind2nd(greater<int>(), 10));
	cout << "The number of elements in v1 greater than 10 is: "
		<< result1a << "." << endl;

	// Compare counting the number of integers > 15 in the vector
	// with a user-defined function object
	vector<int>::iterator::difference_type result1b;
	result1b = count_if(v1.begin(), v1.end(), greaterthan15());
	cout << "The number of elements in v1 greater than 15 is: "
		<< result1b << "." << endl;

	// Count the number of integers < 10 in the vector
	vector<int>::iterator::difference_type result2;
	result2 = count_if(v1.begin(), v1.end(), bind1st(greater<int>(), 10));
	cout << "The number of elements in v1 less than 10 is: "
		<< result2 << "." << endl;

	// Count the number of integers > 10 in the vector	
	result1a = count_if(v1.begin(), v1.end(), bind1st(less<int>(), 10));
	cout << "The number of elements in v1 greater than 10 is: "
		<< result1a << "." << endl;

	// Count the number of integers < 10 in the vector	
	result2 = count_if(v1.begin(), v1.end(), bind2nd(less<int>(), 10));
	cout << "The number of elements in v1 less than 10 is: "
		<< result2 << "." << endl;
}