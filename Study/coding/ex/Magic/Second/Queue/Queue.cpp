#include <iostream>
#define MAX_SIZE 3
//스택 구현
int g_iSize = MAX_SIZE;
int g_iArray[MAX_SIZE];
int g_iPush;
int g_iFront;
int g_iBack;
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
    case 0: {

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
    memset(g_iArray, 0, sizeof(int)*10);
    g_iBack = 0;
    g_iFront = 0;
    g_iPush = 0;
}

void Destroy()
{
}

void Push(int iData)
{
    if (isFull() == false)
    {
        g_iArray[g_iBack] = iData;
        g_iBack = (g_iBack + 1) % MAX_SIZE;
    }
}

int Pop()
{
    if (isEmpty() == false)
    {
        int iRet=g_iArray[g_iFront];
        g_iFront = (g_iBack + 1) % MAX_SIZE;
        return iRet;
    }
    return -1;
}

void Top()
{
}

bool isEmpty()
{
    
    return false;
}
//bool isFull()
//{
//    bool becheck=g_iFront
//    return false;
//}
