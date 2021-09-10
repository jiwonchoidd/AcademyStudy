#include <iostream>
using namespace std;

namespace KGCA
{
	class TStudent
	{
	public:
		int ikor;
	};
	int iValue = 10;
}
namespace GAME
{
	class TStudent
	{
	public:
		int ikor;
	};
	int iValue = 10;
}
using namespace KGCA;
void main()
{	
	TStudent data1;
	GAME::TStudent data2;
	cout << data1.ikor << std::endl;
}