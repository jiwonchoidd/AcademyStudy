#include <iostream>
#ifdef KGCA
using namespace std;
//reinterpret_cast 강제 형변환에 사용.
//보통 포인터->데이터, 포인터->포인터 형변환에 사용된다.
unsigned short Hash(void *p) 
{
	// 주소를 정수로 변환.
	unsigned int val = reinterpret_cast<unsigned int>(p);
	return (unsigned short)(val ^ (val >> 16));
}
// 두 개의 동일한 값 일 경우에 주소를 사용하여 해시값을 얻는다. 
// 주소를 인덱스에 매핑하는 해시 함수에서 
// reinterpret_cast를 유용하게 사용할 수 있다.
int main() 
{
	int a[3] = { 1,2,3 };
	for (int i = 0; i < 3; i++)
	{
		cout << a[i] << " " << Hash(a + i) << endl;
	}
}
#else
#include <iostream>
const char pass[5] = "1234";
int g_iArray[10][10];
unsigned short Hash(void* p)
{
	unsigned short val = reinterpret_cast<unsigned short>(p);
	return val % 10;
}
void Set(int iRow, int iValue)
{
	for (int i = 0; i < 10; i++)
	{
		if (g_iArray[iRow][i] <= 0)
		{
			g_iArray[iRow][i] = iValue;
			break;
		}
	}
}
int Get(int iRow, int iValue)
{
	for (int i = 0; i < 10; i++)
	{
		if (g_iArray[iRow][i] == iValue)
		{
			return g_iArray[iRow][i];
		}
	}
	return -1;
}
int main()
{
	int iArray[100];
	for (int i = 0; i < 10; i++)
	{
		iArray[i] = i + 1;
		unsigned short iHash =
			Hash(&iArray[i]);
		Set(iHash, iArray[i]);
	}
	for (int i = 0; i < 10; i++)
	{
		unsigned short iHash =
			Hash(&iArray[i]);
		std::cout <<
			Get(iHash, iArray[i]) << " " << std::endl;
	}

	int iValue = 777;
	_int64 iAddress = reinterpret_cast<_int64>(&iValue);
	_int64* pAddress;
	pAddress = reinterpret_cast<_int64*>(iAddress);
	*pAddress = 111;
	return 0;
}
#endif

