#include "Sample.h"

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    Sample mySample;
    mySample.WindowRegister(hInstance,nCmdShow,L"게임엔진",800,600,WS_OVERLAPPEDWINDOW);   
    mySample.WindowRun();

}