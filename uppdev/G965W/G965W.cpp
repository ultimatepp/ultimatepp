#include <windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message)  {
	case WM_TIMER:
		static bool b;
		b = !b;
		SetCursor(LoadCursor(NULL, b ? IDC_IBEAM : IDC_ARROW));
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
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
                     int       nCmdShow) {
	WNDCLASSEX wcex;
	
	memset(&wcex, 0, sizeof(wcex));
	wcex.cbSize = sizeof(WNDCLASSEX); 
	wcex.style          = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc    = (WNDPROC) WndProc;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 0;
	wcex.hInstance      = hInstance;
	wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground  = (HBRUSH) (COLOR_WINDOW + 1);
	wcex.lpszMenuName   = 0;
	wcex.lpszClassName  = "G965Test";
	if(RegisterClassEx(&wcex) == 0)
		return FALSE;
	HWND hWnd = CreateWindow(wcex.lpszClassName, "G965 problem", WS_OVERLAPPEDWINDOW, 
	                         CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, 
	                         NULL, NULL, hInstance, NULL);
	if(!hWnd) return FALSE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	
	SetTimer(hWnd, 0, 50, NULL);
	
	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
    }
	return msg.wParam;
}
