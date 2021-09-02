
#include <iostream>
int array[20][20];
int main()
{
	std::cout << "홀수의 값을 입력하세요" << std::endl;
	//입력
	int n;
	std::cin >> n;
	int x=(int)n/2;
	int y=0;
	//홀수 일떄만!!
	for (int i = 1; i <= n * n; i++)  
	{
		array[y][x]=i;
		x++;
		y--;
		if (y < 0 && x > n - 1)
		{
			y += 2;
			x--;
		}
		else
		{
			if (y < 0)
			{
				y = n - 1;
			}
			if (x > n - 1)
			{
				x = 0;
			}
		}
		if (array[y][x] != 0)
		{
			y += 2;
			x --;
		}
		
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			std::cout << array[i][j] << "\t";
		std::cout << "\n";
	}
}
