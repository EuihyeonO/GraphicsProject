#include "EngineBase.h"

EngineBase::EngineBase()
{
}

EngineBase::~EngineBase()
{
}

ATOM EngineBase::MyRegisterClass(HINSTANCE _hInstance)
{
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = &EngineBase::WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = _hInstance;
    wcex.hIcon = nullptr; //LoadIcon(_hInstance, MAKEINTRESOURCE(IDI_GRAPHICSPROJECT));
    wcex.hCursor = nullptr; //LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr; //MAKEINTRESOURCEW(IDC_GRAPHICSPROJECT);
    wcex.lpszClassName = L"WindowDefault";
    wcex.hIconSm = nullptr; //LoadIcon(wcex._hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

LRESULT CALLBACK EngineBase::WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
    switch (_message) {
    case WM_SIZE:
        break;
    case WM_SYSCOMMAND:
        if ((_wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_MOUSEMOVE:
        break;
    case WM_LBUTTONUP:
        break;
    case WM_RBUTTONUP:
        break;
    case WM_KEYDOWN:
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }

    return ::DefWindowProc(_hWnd, _message, _wParam, _lParam);
}

BOOL EngineBase::InitInstance(HINSTANCE _hInstance, int nCmdShow)
{
    hInstance = _hInstance;

    HWND hWnd = CreateWindowW(L"WindowDefault", L"GraphicsProject", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

void EngineBase::Loop()
{
    while(WM_QUIT != msg.message)
    {
        if ((PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

WPARAM EngineBase::End()
{
    return msg.wParam;
}

BOOL EngineBase::Init(HINSTANCE _hInstance)
{
	hInstance = _hInstance;

	MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, SW_SHOW))
    {
        return FALSE;
    }

    return TRUE;
}
