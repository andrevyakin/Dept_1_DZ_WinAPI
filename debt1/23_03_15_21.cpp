/*2.	Щелчок правой кнопкой на клиентской области окна приводит к изменению реакции на нажатие цифр.
Каждое следующее нажатие цифры изменяет размер окна.
Нажатие любой другой клавиши возвращает исходные размеры.*/

#include<Windows.h>
#include<stdlib.h>
#include<string>
#include<tchar.h>

static TCHAR WindowsClass[] = _T("win32app");
static TCHAR Title[] = _T("MyApp");
HINSTANCE hinst;

RECT desktop;
int W, H, num, dX, dY, dCX, dCY; //это все координаты для изменения размеров окна, навереняка можно проще, но я не математик :)
bool buben = true;				//Бубен. Для дальнейших плясок.

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)

{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hinstance;
	wcex.hIcon = LoadIcon(hinstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	/*wcex.hbrBackground = CreateSolidBrush(RGB(100, 70, 150));*/
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = WindowsClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, _T("Call faild!"), _T("MyApp"), NULL);
		return 1;
	}

	hinst = hinstance;

	//Получить координаты рабочего стола
	GetWindowRect(GetDesktopWindow(), &desktop);

	W = desktop.right / 11;
	H = desktop.bottom / 11;
	dX = desktop.right / 22;
	dY = desktop.bottom / 22;
	dCX = desktop.right / 2 - dX;
	dCY = desktop.bottom / 2 - dY;


	HWND hWnd = CreateWindow(
		WindowsClass,
		Title,
		WS_OVERLAPPEDWINDOW,
		dCX - dX,
		dCY - dY,
		W + dX * 2,
		H + dY * 2,
		NULL,
		NULL,
		hinst,
		NULL);

	if (!hWnd)
	{
		MessageBox(NULL, _T("Create window faild!"), _T("MyApp"), NULL);
		return 1;
	}

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return(int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rect;
	RECT desktop;

	switch (message)
	{
	case WM_RBUTTONDOWN:
		buben = false;
		// не знаю как корректно передать управление клавиатуре после мышиного клика, поэтому такие пляски с бубном...
	case WM_KEYDOWN:
		if (!buben)
		{
			num = LOWORD(wParam);
			if (num >= 48 && num <= 57)
			{
				num -= 47;
				SetWindowPos(hWnd, HWND_TOP, dCX - num*dX, dCY - num*dY, W + num*dX * 2, H + num*dY * 2, SWP_SHOWWINDOW);
			}
			else
				SetWindowPos(hWnd, HWND_TOP, dCX - dX, dCY - dY, W + dX * 2, H + dY * 2, SWP_SHOWWINDOW);
		}

		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}