#include <iostream> 

using std::cin;
using std::cout;
using std::endl;
#define MINHEIGHT 1 
#define MAXHEIGHT 20 
#define MAXSIZE 9 

class Figure
{
protected:
	int width;
	int height;
public:
	Figure()
	{ }
	virtual void draw(int mode) = 0;
	void setWidth(int width)
	{
		this->width = width;
	}
	void setHeight(int height)
	{
		this->height = height;
	}
};
class Triangle : public Figure
{
public:
	Triangle()
	{ }
	void draw(int mode)
	{
		int i, j;
		float k;
		switch (mode)
		{
		case 1:
			for (i = 0; i < height; i++)
			{
				for (j = 0; j <= i; j++)
					cout << "*";
				cout << endl;
			}
			break;
		case 2:
			for (i = 0; i < height; i++)
			{
				for (j = 0; j < height - i - 1; j++)
					cout << " ";
				for (j = 0; j <= i; j++)
					cout << "*";
				cout << endl;
			}
			break;
		case 3:
			for (i = 0; i < height; i++)
			{
				for (j = i + 1; j < height; j++)
					cout << " ";
				for (k = 0; k <= i; k += 0.5f)
					cout << "*";
				cout << endl;
			}
			break;
		}
	}
};
class Rectangle : public Figure
{
public:
	Rectangle()
	{}
	void draw(int mode)
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
				cout << "*";
			cout << endl;
		}
	}
};

class Rhombus : public Figure
{
public:
	Rhombus()
	{ }
	void draw(int mode)
	{
		int i, j, k, star = 1;
		int center = height / 2;
		switch (mode)
		{
		case 1:
			for (i = 0; i <= center; i++, star += 2)
			{
				for (j = center; j > i; j--)
					cout << " ";
				for (k = 0; k < star; k++)
					cout << "*";
				cout << endl;
			}
			star -= 4;
			for (i = 0; i < center; i++, star -= 2)
			{
				for (j = 0; j <= i; j++)
					cout << " ";
				for (k = 0; k < star; k++)
					cout << "*";
				cout << endl;
			}
			break;
		case 2:
			for (i = 0; i <= center; i++, star += 1)
			{
				for (j = center; j > i; j--)
					cout << " ";
				for (k = 0; k < star; k++)
					cout << "* ";
				cout << endl;
			}
			star -= 2;
			for (i = 0; i < center; i++, star -= 1)
			{
				for (j = 0; j <= i; j++)
					cout << " ";
				for (k = 0; k < star; k++)
					cout << "* ";
				cout << endl;
			}
			break;
		}
	}
};

class Snail : public Figure
{
public:
	Snail()	{}
	void draw(int mode)
	{
		int size = height;
		int** snailArr = NULL;
		int val = 0;
		int count;
		int s = 1;
		int r = 0, c = 0;
		int i = 0, j = 0;
		snailArr = (int**)malloc(sizeof(int) * size);
		for (i = 0; i < size; i++)
			snailArr[i] = (int*)malloc(sizeof(int) * size);
		while (val < (height * height))
		{
			for (j = c, count = 0; count < size; count++, j += s)
			{
				snailArr[r][j] = ++val;
				c = j;
			}
			r += s;
			size--;
			for (i = r, count = 0; count < size; count++, i += s)
			{
				snailArr[i][c] = ++val;
				r = i;
			}
			s *= -1;
			c += s;
		}
		cout << endl;
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < height; j++)
				printf("%02d ", *(*(snailArr + i) + j));
			cout << endl;
		}
		cout << endl;
		for (i = 0; i < height; i++)
			free(snailArr[i]);
		free(snailArr);
	}
};

void finish(Figure** figureList)

{

	for (int i = 0; i < 4; i++)

		if (figureList[i] != NULL)

			delete figureList[i];

}

int main(void)

{

	Figure* figureList[4] = { NULL, };

	int menu;

	int index = 0;

	int width;

	int height;

	int triangle;

	int rhombus;

	while (1)

	{

		cout << "도형 그리기" << endl;

		cout << "1. 삼각형" << endl;

		cout << "2. 사각형" << endl;

		cout << "3. 마름모" << endl;

		cout << "4. 달팽이..." << endl;

		cout << "5. 종료" << endl;

		cout << "선택하세요 : ";

		cin >> menu;

		switch (menu)

		{

		case 1:

			cout << "1. 왼쪽 직각 삼각형" << endl;

			cout << "2. 오른쪽 직각 삼각형" << endl;

			cout << "3. 정삼각형" << endl;

			cout << "그릴 삼각형을 선택하세요 : ";

			cin >> triangle;

			if (triangle > 3 || triangle < 1)

			{

				cout << "잘못 입력하셨습니다. 처음부터 다시 입력해주세요" << endl;

				break;

			}

			cout << "삼각형의 높이를 입력하세요 : ";

			cin >> height;

			if (height > MAXHEIGHT || height < MINHEIGHT)

			{

				cout << "삼각형의 높이는 " << MINHEIGHT << "부터 " << MAXHEIGHT << "까지 가능합니다. 처음부터 다시 입력해주세요" << endl;

				break;

			}

			if (figureList[0] == NULL)

			{

				figureList[0] = new Triangle;

			}

			figureList[0]->setHeight(height);

			figureList[0]->draw(triangle);

			break;

		case 2:

			cout << "그릴 사각형의 가로 길이를 입력하세요 : ";

			cin >> width;

			cout << "그릴 사각형의 세로 길이를 입력하세요 : ";

			cin >> height;

			if (width > MAXHEIGHT || width < MINHEIGHT || height > MAXHEIGHT || height < MINHEIGHT)

			{

				cout << "사각형의 가로, 세로 길이는 " << MINHEIGHT << "부터 " << MAXHEIGHT << "까지 가능합니다. 처음부터 다시 입력해주세요" << endl;

				break;

			}

			if (figureList[1] == NULL)

			{

				figureList[1] = new Rectangle;

			}

			figureList[1]->setWidth(width);

			figureList[1]->setHeight(height);

			figureList[1]->draw(0);

			break;

		case 3:

			cout << "1. 일반형 마름모" << endl;

			cout << "2. 특수형 마름모" << endl;

			cout << "그릴 마름모를 선택하세요 : ";

			cin >> rhombus;

			if (rhombus > 2 || rhombus < 1)

			{

				cout << "잘못 입력하셨습니다. 처음부터 다시 입력해주세요" << endl;

				break;

			}

			cout << "그릴 마름모의 높이를 입력하세요(홀수) : ";

			cin >> height;

			if (height > MAXHEIGHT || height < MINHEIGHT)

			{

				cout << "마름모의 높이는 " << MINHEIGHT << "부터 " << MAXHEIGHT << "까지 가능합니다. 처음부터 다시 입력해주세요" << endl;

				break;

			}

			if (height % 2 == 0)

				height += 1;

			if (figureList[2] == NULL)

			{

				figureList[2] = new Rhombus;

			}

			figureList[2]->setHeight(height);

			figureList[2]->draw(rhombus);

			break;

		case 4:

			cout << "달팽이의 크기를 입력하세요 : ";

			cin >> height;

			if (height > MAXSIZE || height < MINHEIGHT)

			{

				cout << "달팽이의 크기는 " << MINHEIGHT << "부터 " << MAXSIZE << "까지 가능합니다. 처음부터 다시 입력해주세요" << endl;

				break;

			}

			if (figureList[3] == NULL)

			{

				figureList[3] = new Snail;

			}

			figureList[3]->setHeight(height);

			figureList[3]->draw(0);

			break;

		case 5:

			cout << "프로그램을 종료합니다." << endl;

			finish(figureList);

			return 0;

		default:

			cout << "잘못 입력하셨습니다. 다시 입력해주세요" << endl << endl;

			break;

		}

		system("pause");

		system("cls");

	}

	finish(figureList);

	return 0;

}