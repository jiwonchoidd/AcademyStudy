#include <iostream>

using namespace std;
class CCTest {
public:
	void setNumber(int);
	void printNumber() const;
	const int* GetConst();
private:
	int number;
};

void CCTest::setNumber(int num) { number = num; }
// 멤버함수 const 가 붙으면 클래스 자체가 상수화된다.
void CCTest::printNumber() const {
	cout << "\nBefore: " << number;
	const_cast< CCTest * >(this)->number--;
	cout << "\nAfter: " << number;
}
const int* CCTest::GetConst()
{
	return &number;
}
int main() {
	CCTest X;
	X.setNumber(8);

	//1
	const int* iValue = X.GetConst();
	//*iValue = 20; //  상수 에러
	//2
	int* iValue2 = const_cast<int*>(X.GetConst());
	*iValue2 = 20;

	X.printNumber();
}