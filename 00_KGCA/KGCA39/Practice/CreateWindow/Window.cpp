#include "Window.h"
HWND g_hWnd;
RECT g_rtClient;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
        {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    return 0;
}
bool Window::WindowRegister(HINSTANCE hInstance, int nCmdShow, 
    const WCHAR* strTitle, int iWidth, int iHeight, DWORD dStyle)
{
        m_hInstance = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
        WNDCLASSEXW wcex;
        //제로메모리로 초기화
        ZeroMemory(&wcex, sizeof(WNDCLASSEXW));

        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = m_hInstance;
        //wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSAMPLE));
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
        //wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINDOWSAMPLE);
        wcex.lpszClassName = L"Jiwon";
        //wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

        if (!RegisterClassExW(&wcex))
        {
            return false;
        }
        //클라이언트 크기로 변환
        RECT rc = { 0, 0, iWidth, iHeight};
        AdjustWindowRect(&rc, dStyle, false);

        m_hWnd = CreateWindowEx(0,L"Jiwon", strTitle, dStyle,
            0, 0, rc.right-rc.left, rc.bottom-rc.top,
            nullptr, nullptr, m_hInstance, nullptr);
        if (m_hWnd==NULL)
        {
            return false;
        }

        GetWindowRect(m_hWnd, &m_rtWindow);
        GetClientRect(m_hWnd, &m_rtClient);

        g_hWnd = m_hWnd;
        g_rtClient = m_rtClient;

        ShowWindow(m_hWnd, nCmdShow);
        return true;
}

bool Window::WindowRun()
{
    //GameInit();
    MSG msg;
    while (1)
    {
        if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {

        }
    }
    return false;
}

Window::Window()
{
}
