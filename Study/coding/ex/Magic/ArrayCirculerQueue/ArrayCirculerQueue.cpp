#include <iostream>
#define MAX 3
int g_iSize = MAX+1;
int g_iArray[MAX];
int g_iBack;
int g_iFront;
int DeQueue();
void QueueInit();
void Print();
void EnQueue(int input);
void Top();
bool is_Full();
bool is_Empty();
int main()
{
    QueueInit();
    bool is_Stop = false;
    while (!is_Stop)
    {
        //어떤 활동을 할껀지? 
        //0-추가,1-삭제,2-조회,3-마지막 숫자 조회, 4-그외 숫자 Exit
        std::cout << "0-추가,1-삭제,2-조회,3-마지막 숫자 조회, 4-그외 숫자 Exit : ";
        int action = 0;
        std::cin >> action;
        // 시작하기전에 초기화
        switch (action)
        {
        case 0:
        {
            int input = 0;
            std::cin >> input;
            EnQueue(input);
        }break;
        case 1:
        {
            int iData = DeQueue();
            //비어있는 -1를 리턴하기때문에 조건문
            if (iData >= 0)
            {
                //데큐
                std::cout << iData << " is deleted.\n";
            }
            else
                std::cout << " is empty"<<std::endl;
        }break;
        case 2:
        {
            Print();
        }break;
        case 3:
        {
            Top();
        }break;
        default:is_Stop = true;
            std::cout << "종료합니다." << std::endl;
            break;
        }
    }
}

void QueueInit()
{
    //배열, 앞 뒤 초기화
    memset(g_iArray, 0, sizeof(int) * 10);
    g_iBack = 0;
    g_iFront = 0;
}

void Print()
{
        for (int i = 0; i < g_iBack; i++)
        {
            std::cout << g_iArray[i] << " ";
        }
        std::cout << "\n";
}

void EnQueue(int input)
{
    if (is_Full()==false)
    {
        g_iArray[g_iBack] = input;
        //자리 바꾸기
        g_iBack = (g_iBack + 1) % g_iSize;
        std::cout<< "추가 완료\n";
    }
    else
        std::cout << "추가 실패\n";
}

int DeQueue()
{
    if (is_Empty()==false)
    {
        int temp = g_iArray[g_iFront];
        g_iFront = (g_iFront + 1) % g_iSize;
        return temp;
    }
    //비어 있다면 -1를 리턴함..
    return -1;
}

void Top()
{
    std::cout << g_iArray[g_iBack - 1] << "\n" << std::endl;
}

bool is_Full()
{
    return g_iFront ==
        (g_iBack + 1) % g_iSize;
}

bool is_Empty()
{
    return g_iFront == g_iBack;
}
