#include "pch.h"
//#include "Tools/Window.h"
//#include "Platform.h"
//#include <iostream>
#pragma comment(lib, "user32.lib")	// LoadIcon
#include <WinUser.h>
#include <tchar.h>
#include "System/Renderer.h"
#include "../../Resources/resource.h"

#define STYLE_FULL_SCREEN WS_POPUP
#define STYLE_NOT_FULL WS_OVERLAPPEDWINDOW//^(WS_THICKFRAME | WS_MAXIMIZEBOX)

HICON icon;

LRESULT CALLBACK Win32WindowEventHandler(HWND wnd_handle, UINT msg_type, WPARAM w_param, LPARAM l_param)
{
	C_Window* this_window = reinterpret_cast<C_Window*>(GetWindowLongPtr(wnd_handle, GWLP_USERDATA));
	if (this_window == nullptr)
	{
		return DefWindowProc(wnd_handle, msg_type, w_param, l_param);
	}
	switch (msg_type)
	{
	case WM_CLOSE:
		this_window->windowShouldRun = false;
		return 0;
	case WM_SIZE:
		this_window->Resize(LOWORD(l_param), HIWORD(l_param));
		return 0;
	case WM_ACTIVATE:
		switch (LOWORD(w_param))
		{
		case WA_ACTIVE:
			//std::cout << "ACTIVATE" << std::endl;
			this_window->Activated = true;
			ShowCursor(FALSE);
			break;
		case WA_CLICKACTIVE:
			this_window->Activated = true;
			ShowCursor(FALSE);
			//std::cout << "ACTIVATE CLICK" << std::endl;
			break;
		case WA_INACTIVE:
			this_window->Activated = false;
			ShowCursor(TRUE);
			//std::cout << "INACTIVATE" << std::endl;
			break;
		default:
			break;
		}
	default:
		break;
	}
	return DefWindowProc(wnd_handle, msg_type, w_param, l_param);
}

C_Window::C_Window(std::string window_name, bool full_screen, uint16_t _width, uint16_t _height) : Activated(false)
{
	width = _width;
	height = _height;
	if (full_screen)
	{
		width = GetSystemMetrics(SM_CXSCREEN);
		height = GetSystemMetrics(SM_CYSCREEN);
	}
	windowShouldRun = true;

	win32WindowClassInstance = GetModuleHandle(nullptr);

	//Prepare for window class registeration
	WNDCLASSEX window_class{};
	window_class.cbSize = sizeof(WNDCLASSEX);
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = Win32WindowEventHandler;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0;
	window_class.hInstance = win32WindowClassInstance;
	window_class.hIcon = LoadIcon(win32WindowClassInstance, MAKEINTRESOURCE(IDI_ICON1));
	window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
	window_class.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	window_class.lpszMenuName = NULL;
	window_class.lpszClassName = window_name.c_str();
	//window_class.hIconSm = LoadIcon(NULL, IDI_WINLOGO);
	window_class.hIconSm = LoadIcon(win32WindowClassInstance, MAKEINTRESOURCE(IDI_ICON1)); //change console icon
	//Reguster window class
	if (!RegisterClassEx(&window_class))
	{
		//report failed;
	}

	//Create window instance using registered window class
	//DWORD ex_style = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	DWORD style = STYLE_NOT_FULL;// ^ (WS_THICKFRAME | WS_MAXIMIZEBOX);
	if (full_screen)
	{
		style = STYLE_FULL_SCREEN;
	}
	RECT window_rect = { 0,0,LONG(width),LONG(height) };
	//AdjustWindowRectEx(&window_rect, style, FALSE, ex_style);
	win32Window = CreateWindowEx(
		0,
		window_name.c_str(),					//class name
		window_name.c_str(),					//app name
		style,									//window style
		CW_USEDEFAULT, CW_USEDEFAULT,			//x/y coords
		window_rect.right - window_rect.left,	//width
		window_rect.bottom - window_rect.top,	//height
		NULL,									//handle to parent
		NULL,									//handle to menu
		win32WindowClassInstance,				//hInstance
		NULL									//no extra parameters
	);
	if (!win32Window)
	{
		//do some report
		debug_func::ReportErr("Window Creation ERROR!");
	}

	//GetWindowRect(win32Window, windowRect);

	//Set this CWindow instance as the parameter of Win32 window
	SetWindowLongPtr(win32Window, GWLP_USERDATA, (LONG_PTR)this);

	//Show window
	ShowWindow(win32Window, SW_SHOW);
	SetForegroundWindow(win32Window);
	SetFocus(win32Window);

	//ResetWindowSize(E_RESOLUTION_FULL_SCREEN);
}

C_Window::~C_Window(){}

void C_Window::releaseWindow()
{

}

bool C_Window::ShouldRun()
{
	return windowShouldRun;
}

void C_Window::UpdateWindow()
{
	MSG message;
	if (PeekMessage(&message, win32Window, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}
void C_Window::ResetWindowSize(int _width, int _height, bool full_screen)
{
	if (full_screen)
	{
		float tmpwidth = GetSystemMetrics(SM_CXSCREEN);
		float tmpheight = GetSystemMetrics(SM_CYSCREEN);
		SetWindowLongPtr(win32Window, GWL_STYLE, STYLE_FULL_SCREEN);
		SetWindowPos(win32Window, HWND_TOP, 0, 0, tmpwidth, tmpheight, SWP_SHOWWINDOW);
		//MoveWindow(win32Window, 0, 0, width, height, TRUE);
	}
	else
	{
		float tmpwidth = _width;
		float tmpheight = _height;
		SetWindowLongPtr(win32Window, GWL_STYLE, STYLE_NOT_FULL);
		SetWindowPos(win32Window, HWND_TOP, 100, 100, tmpwidth, tmpheight, SWP_SHOWWINDOW);
	}
	//switch (_resolution)
	//{
	//case E_RESOLUTION_FULL_SCREEN:
	//	width = GetSystemMetrics(SM_CXSCREEN);
	//	height = GetSystemMetrics(SM_CYSCREEN);
	//	SetWindowLongPtr(win32Window, GWL_STYLE, STYLE_FULL_SCREEN);
	//	//MoveWindow(win32Window, 0, 0, width, height, TRUE);
	//	SetWindowPos(win32Window, HWND_TOPMOST, 0, 0, width, height, SWP_SHOWWINDOW);
	//	//*windowRect = { 0,0,LONG(width),LONG(height) };
	//	//AdjustWindowRectEx(windowRect, STYLE_FULL_SCREEN, FALSE, 0);
	//	break;
	//case E_RESOLUTION_1280_720:
	//	width = 1280;
	//	height = 720;
	//	//*windowRect = { 0,0,LONG(width),LONG(height) };
	//	//AdjustWindowRectEx(windowRect, STYLE_NOT_FULL, TRUE, 0);
	//	SetWindowLongPtr(win32Window, GWL_STYLE, STYLE_NOT_FULL);
	//	//MoveWindow(win32Window, 0, 0, width, height, TRUE);
	//	SetWindowPos(win32Window, HWND_TOPMOST, 100, 100, width, height, SWP_SHOWWINDOW);
	//	break;
	//case E_RESOLUTION_800_600:
	//	width = 800;
	//	height = 600;
	//	//*windowRect = { 0,0,LONG(width),LONG(height) };
	//	//AdjustWindowRectEx(windowRect, STYLE_NOT_FULL, TRUE, 0);
	//	SetWindowLongPtr(win32Window, GWL_STYLE, STYLE_NOT_FULL);
	//	//MoveWindow(win32Window, 0, 0, width, height, TRUE);
	//	SetWindowPos(win32Window, HWND_TOPMOST, 100, 100, width, height, SWP_SHOWWINDOW);
	//	break;
	//default:
	//	break;
	//}
}
//void CWindow::addResizeCallback(RenderResizeable* resizable)
//{
//	SizeCallbacks.push_back(resizable);
//}
HWND C_Window::GetWin32Handle()
{
	return win32Window;
}
void C_Window::Resize(int w, int h)
{
	width = w;
	height = h;
	C_Renderer *_renderer = C_Renderer::globalRenderer;
	if (_renderer != nullptr)
	{
		_renderer->Resize(width, height);
	}
	//for (auto i = SizeCallbacks.begin(); i != SizeCallbacks.end(); ++i)
	//{
	//	(*i)->SizeFunc(w, h);
	//}
}