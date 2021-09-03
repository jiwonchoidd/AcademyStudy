#include <iostream>
#define MAX 3
int array[MAX];
int top;
void Create();
void Print();
void Push(int i);
int Pop();
int TopPrint();
bool is_Empty();
bool is_Full();
int main()
{
	Create();

	while (true)
	{
		std::cout << "*****0-push, 1-pop, 2-top, 3-print 4-out" << std::endl;
		int act = 0;
		std::cin >> act;
		switch (act)
		{
		case 0:
		{
			//push
			int input = 0;
			std::cout << "input number" << std::endl;
			std::cin >> input;
			Push(input);
			Print();
		}break;
		case 1:
		{
			//pop
			if (Pop() >= 0)
			{
				std::cout << Pop();
			}
			else
			std::cout <<"is Empty" << std::endl;
		}break;
		case 2:
		{
			//top
			std::cout << TopPrint() << std::endl;
		}break;
		case 3:
		{
			//print
			Print();
		}break;
		default:
			break;
		}
	}
}

void Create()
{
	memset(array, 0, sizeof(int) * 10);
	top = 0;
}
void Push(int i)
{
	if (is_Full() == false)
	{
		array[top] = i;
			top++;
	}
}
void Print()
{
	for (int i = 0; i < top; i++)
	{
		std::cout <<"\t" << array[i] << " ";
	}
	std::cout << "\n";
}
int Pop()
{
	if (is_Empty() == false)
	{
		top--;
		return array[top];
	}
	//비어있다면
	return -1;
}
int TopPrint()
{
	return array[top-1];
}
bool is_Empty()
{
	if (top <= 0)
	{
		std::cout << "Is Empty!\n";
		return true;
	}
	return false;
}
bool is_Full()
{
	if (top== MAX)
	{
		std::cout << "Is full!\n";
		return true;
	}
	return false;
}
