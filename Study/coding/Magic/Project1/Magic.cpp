#include <iostream>
using namespace std;
int arr[20][20];
void Print(int);
int main() {
	//하드 코딩이다
	while (1)
	{
	int n;
	cin >> n;
	//for (int i = 1; i < n * n; i++) {
	//	for (int j = 1; i < n * n; j++)
	//		arr[i][j] = 0;
	//}
	int icount = 0;
	int x = (int)(n / 2);
	int y = 0;
	arr[y][x] = ++icount;
	for (int i = 1; i < n * n; i++) {
		x++;
		y--;
		if (y < 0 && x > n - 1)
		{
			y += 2;
			x--;
		}
		else {
			if (y < 0)
			{
				y = n - 1;
			}
			if (x > n - 1)
			{
				x = 0;
			}
		}
		if (arr[y][x] != 0)
		{
			y += 2;
			x--;
		}
		++icount;
		arr[y][x] = icount;
	}
	//출력
	Print(n);
	}
}
	void Print(int n) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++)
				cout << arr[i][j] << "\t";
			cout << "\n";
		}
	}
