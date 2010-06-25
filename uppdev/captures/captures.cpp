#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    

    switch (message) 
    {
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            
            /* Put your drawing code here */
            
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

extern "C" __declspec(dllimport) int capture_screen(int x, int y, int cx, int cy, char *filename);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    capture_screen(100, 100, 400, 200, "u:/test.jpg");
    
    
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX); 

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = (WNDPROC) WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH) (COLOR_WINDOW + 1);
    wcex.lpszMenuName   = 0;
    wcex.lpszClassName  = "UPPW32APP";

    if(RegisterClassEx(&wcex) == 0)
        return FALSE;
    
    HWND hWnd = CreateWindow(wcex.lpszClassName, "Ultimate application", WS_OVERLAPPEDWINDOW, 
                             CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, 
                             NULL, NULL, hInstance, NULL);
    if(!hWnd)
        return FALSE;
    
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    
    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
