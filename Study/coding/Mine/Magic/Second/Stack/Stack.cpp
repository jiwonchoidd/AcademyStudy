#include <iostream>
#define MAX_SIZE 3
//스택 구현
int g_iSize = MAX_SIZE;
int g_iArray[MAX_SIZE];
int g_Top;
void Create();
void Destroy();
void Push(int iData);
int Pop();
void Top();
bool isEmpty();
bool isFull();
int main()
{
    int iSelect = 0;
    std::cout << "0";
    std::cout << "select : ";
    std::cin >> iSelect;
    switch (iSelect)
    {
    case 0:{

    }break;
    case 1: {

    }break;
    case 2: {

    }break;
    default:
        break;
    }
}

void Create()
{
    /*for (int i = 0; i < 10; i++)
    {
        g_iArray[i] = -1;
    }*/
    //정해진만큼 첨가 0으로 초기화하는데 쓰임
    memset(g_iArray, 0, sizeof(int));
    g_Top = 0;
}

void Destroy()
{
}

void Push()
{
}

int Pop()
{
}

void Top()
{
}

bool isEmpty()
{
    return false;
}
bool isFull()
{
    return false;
}
