#include <windows.h>
#include "resource.h"
HINSTANCE hInst;

LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_INITDIALOG:
            return TRUE;

        case WM_COMMAND:
            if(LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
            {
                EndDialog(hDlg, LOWORD(wParam));
                return TRUE;
            }
            break;
    }
    return FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    PAINTSTRUCT ps;
    HDC hdc;

    const TCHAR szHello[] = "Hello World!";

    switch (message)
    {
        case WM_COMMAND:

            wmId    = LOWORD(wParam);
            wmEvent = HIWORD(wParam);

            switch(wmId)
            {
                case IDM_ABOUT:
                   DialogBox(hInst, (LPCTSTR) IDD_ABOUT, hWnd, (DLGPROC) About);
                   break;

                case IDM_EXIT:
                   DestroyWindow(hWnd);
                   break;

                default:
                   return DefWindowProc(hWnd, message, wParam, lParam);
            }
            break;

        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);

            /* Put your drawing code here */

            RECT rt;
            GetClientRect(hWnd, &rt);
            DrawText(hdc, szHello, strlen(szHello), &rt, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            EndPaint(hWnd, &ps);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{

    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = (WNDPROC) WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    // Put your program icon into the package directory
    wcex.hIconSm        = LoadIcon(hInstance, (LPCTSTR) IDI_SMALL);
    wcex.hIcon          = LoadIcon(hInstance, (LPCTSTR) IDI_BIG);
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH) (COLOR_WINDOW + 1);
    wcex.lpszMenuName   = (LPCSTR) IDC_WIN32APP;
    wcex.lpszClassName  = "UPPW32APP";

    if(RegisterClassEx(&wcex) == 0)
        return FALSE;

    hInst = hInstance;

    HWND hWnd = CreateWindow(wcex.lpszClassName, "Ultimate application", WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
                             NULL, NULL, hInstance, NULL);
    if(!hWnd)
        return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    HACCEL hAccelTable = LoadAccelerators(hInstance, (LPCTSTR) IDC_WIN32APP);

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0))
    {
        if(!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

    }

    return msg.wParam;
}
