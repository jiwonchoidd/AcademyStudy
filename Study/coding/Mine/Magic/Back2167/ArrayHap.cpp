#include "ArrayHap.h"
#include <iostream>
#include <vector>
using namespace std;

int ArraySum(int i, int j);
int main()
{
	vector<int> array;
	//todo : 배열의 크기 입력
	int row = 0;
	int col = 0;
	int temp = 0;
	cin >> col >> row;
	//todo : 정수로 배열 입력
	for (int i = 0; i < col; i++)
	{
		for (int j = 0; j < row; j++)
		{
			cin >> temp;
			array.push_back(temp);
			temp = 0;
		}
		cout << "\n";
	}

	cout << "2차원 배열이 주워짐" << endl;


}
