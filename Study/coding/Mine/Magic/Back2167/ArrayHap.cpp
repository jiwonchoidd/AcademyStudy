#include "ArrayHap.h"
#include <iostream>
#include <vector>
using namespace std;

int ArraySum(int i, int j);
int main()
{
	vector<int> array;
	//todo : �迭�� ũ�� �Է�
	int row = 0;
	int col = 0;
	int temp = 0;
	cin >> col >> row;
	//todo : ������ �迭 �Է�
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

	cout << "2���� �迭�� �ֿ���" << endl;


}
