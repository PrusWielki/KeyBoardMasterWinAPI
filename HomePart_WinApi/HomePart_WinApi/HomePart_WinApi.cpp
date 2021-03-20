// Project3.cpp : Defines the entry point for the application.
//links used:
//https://forums.codeguru.com/showthread.php?500841-Set-my-window-position-at-screen-center
//https://stackoverflow.com/questions/4631292/how-detect-current-screen-resolution
//https://zetcode.com/gui/winapi/menus/
//https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getmenustate
//https://social.msdn.microsoft.com/Forums/vstudio/en-US/fdd4edda-cc6e-42ae-b7d7-465b990f4f9b/win32-api-creating-fixedsize-window?forum=vcgeneral
//https://social.msdn.microsoft.com/Forums/vstudio/en-US/aa02e263-1f65-453b-90ae-492912e6e56c/disable-the-maximize-button-in-a-win32-program?forum=vcgeneral
//https://stackoverflow.com/questions/2312220/creating-context-menu-for-win32-api
//https://cpp0x.pl/kursy/Kurs-WinAPI-C++/Podstawy/Okna-dialogowe-cz-4/187
// https://docs.microsoft.com/en-us/windows/win32/dlgbox/using-common-dialog-boxes?redirectedfrom=MSDN
//
//
//
//
//
//
//

#include "framework.h"
#include "HomePart_WinApi.h"
#include <commdlg.h>
#include <time.h>
#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void GetTextInfoForKeyMsg(WPARAM wParam, const TCHAR* msgName,
	TCHAR* buf, int bufSize);
void AddMenus(HWND hwnd);
void DrawBitmap(HWND hWnd);
void DrawBitmap_stretched(HWND hWnd);
void PaintKid(HWND hWnd);
void ChangeBackground(HWND hWnd);
void HideKid(HWND hWnd, WPARAM wParam);
void RightClickMenu(HWND hWnd);
void Timer7(HWND hWnd);
void Timer8(HWND hWnd, int& privy, RECT& rc_c);
void SpaceToRemove();
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_HOMEPARTWINAPI, szWindowClass, MAX_LOADSTRING);

	MyRegisterClass(hInstance);
	//ChildRegisterClass(hInstance);
	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HOMEPARTWINAPI));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HOMEPARTWINAPI));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_INACTIVECAPTION); //(HBRUSH)(WM_ERASEBKGND);// (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_HOMEPARTWINAPI);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}
//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable
	WCHAR newTitle[100] = { 'K','e','y','b','o','a','r','d','M','a','s','t','e','r','_','W','i','n','A','P','I','2','0','2','1','\0' };

	HWND hWnd = CreateWindowW(szWindowClass, newTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, 0, GetSystemMetrics(SM_CXSCREEN) / 2, GetSystemMetrics(SM_CYSCREEN) / 2, nullptr, nullptr, hInstance, nullptr);
	SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	RECT rc;
	srand(time(NULL));
	GetWindowRect(hWnd, &rc);

	int xPos = (GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
	int yPos = (GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;


	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	//ChildRegisterClass(hInstance);
	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
#define KID_AMOUNT 100
HMENU ghMenu[2];
HMENU hPopupMenu;
int x = 0;
int x_set = 1;
int y = 0;
int z[KID_AMOUNT] = { 0 };
int i = 0;
bool stretched = false;
HBITMAP hBitmap01 = NULL;

char sNazwaPliku[MAX_PATH] = "";
struct child {
	HWND hWndc;
	int howlow;
	bool visible;
	char letter;
	int incerement;
	char c[2];

};
int bitmap_chosen = 0;
static int missed = 0;
int wrongkeys = 0;
child kid[KID_AMOUNT];
int py[KID_AMOUNT];
static OPENFILENAME ofn{};

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	const int bufSize = 256;
	static int privy = 0;
	TCHAR buf[bufSize];
	RECT rc_c;
	GetClientRect(hWnd, &rc_c);
	static HDC offDC = NULL;
	static HBITMAP offOldBitmap = NULL;
	static HBITMAP offBitmap = NULL;
	UINT state;

	switch (message)
	{
	case WM_RBUTTONDOWN:
	{
		
		
		RightClickMenu(hWnd);
	}
	case WM_SETFOCUS: {
		SetTimer(hWnd, 8, 1000, NULL);
		for (int j = 0; j < KID_AMOUNT; j++) {
			SetTimer(kid[j].hWndc, 7, 30, NULL);
		}
		CheckMenuItem(ghMenu[0], 32772, MF_UNCHECKED);

		break;
	}
	case WM_KILLFOCUS:
	{
		CheckMenuItem(ghMenu[0], 32772, MF_CHECKED);
		KillTimer(hWnd, 8);
		for (int j = 0; j < KID_AMOUNT; j++) {
			KillTimer(kid[j].hWndc, 7);
		}

		break;
	}

	case WM_KEYDOWN:
	{
		if (hWnd == GetAncestor(hWnd, GA_ROOT)) {
			GetTextInfoForKeyMsg(wParam, _T(" KEYDOWN "), buf, bufSize);
			//if (wParam == 32) {
			//	SpaceToRemove();
			//}
			if (wParam <= 90 && wParam >= 65) {

				HideKid(hWnd, wParam);
			}


		}

		break; }

	case WM_CREATE: {
		if (hWnd != GetAncestor(hWnd, GA_ROOT)) {
			SetTimer(hWnd, 7, 30, NULL);
		}
		else {
			AddMenus(hWnd);
			SetTimer(hWnd, 8, 1000, NULL);
			SetTimer(hWnd, 9, 30, NULL);
		}
	}
				  break;

	case WM_TIMER:
	{


		if (wParam == 7) // check timer id
		{
			Timer7(hWnd);
		}
		else if (wParam == 8 && hWnd == GetAncestor(hWnd, GA_ROOT)) {

			Timer8(hWnd, privy, rc_c);

		}
		else if (wParam == 9 && hWnd == GetAncestor(hWnd, GA_ROOT)) {
			LONG lStyles = GetWindowLong(hWnd, GWL_STYLE);

			if (lStyles & WS_MINIMIZE) {
				for (int j = 0; j < KID_AMOUNT; j++) {
					ShowWindow(kid[j].hWndc, SW_HIDE);
					kid[j].visible = false;
				}
			}
			wchar_t title[100];
			_stprintf_s(title, 100, _T("KeyboardMaster: WinAPI_2021, Missed:  %d, Wrong keys: %d"), missed, wrongkeys);
			SetWindowText(hWnd, title);

		}

	}
	break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);

		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;

		case ID_BITMAP:
		{
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.lpstrFilter = L"(*.bmp)\0";
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrFile = (LPWSTR)sNazwaPliku;
			ofn.lpstrDefExt = L"txt";
			ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			ofn.hwndOwner = hWnd;

			if (!GetOpenFileName(&ofn))
			{
				return 1;
			}
			DrawBitmap(hWnd);
			RECT rc;
			GetClientRect(hWnd, &rc);
			InvalidateRect(hWnd, &rc, false);
			bitmap_chosen = 1;
			break;


		}
		case ID_COLOR: {

			ChangeBackground(hWnd);

			break;
		}
		case ID_TILED: {
			DrawBitmap(hWnd);
			RECT rc;
			GetClientRect(hWnd, &rc);
			InvalidateRect(hWnd, &rc, 0);
			stretched = false;
			break;
		}
		case ID_STRETCHED: {
			DrawBitmap_stretched(hWnd);
			RECT rc;
			GetClientRect(hWnd, &rc);
			InvalidateRect(hWnd, &rc, 0);
			stretched = true;
			break;
		}
		case ID_FILE_NEWGAME:
		{
			for (int j = 0; j < KID_AMOUNT; j++) {
				ShowWindow(kid[j].hWndc, SW_HIDE);
				kid[j].visible = false;
			}
			missed = 0;
			wrongkeys = 0;

			break;

		}
		case ID_FILE_PAUSE:
		{
			state = GetMenuState(ghMenu[0], ID_FILE_PAUSE, MF_BYCOMMAND);
			if (state != MF_CHECKED) {
				CheckMenuItem(ghMenu[0], ID_FILE_PAUSE, MF_CHECKED);
				KillTimer(hWnd, 8);
				for (int j = 0; j < KID_AMOUNT; j++) {
					KillTimer(kid[j].hWndc, 7);
				}
			}
			else {
				CheckMenuItem(ghMenu[0], ID_FILE_PAUSE, MF_UNCHECKED);
				if (hWnd == GetAncestor(hWnd, GA_ROOT))
					SetTimer(hWnd, 8, 1000, NULL);
				for (int j = 0; j < KID_AMOUNT; j++) {
					SetTimer(kid[j].hWndc, 7, 30, NULL);
				}
			}
			break;
		}
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		if (hWnd != GetAncestor(hWnd, GA_ROOT) && hWnd != NULL) {

			PaintKid(hWnd);

		}
		else if (hWnd != NULL) {
			if (bitmap_chosen) {
				if (stretched)
					DrawBitmap_stretched(hWnd);
				else
					DrawBitmap(hWnd);
			}
			else {

				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				EndPaint(hWnd, &ps);

			}

		}
	}

	break;
	/*  case WM_ERASEBKGND:


		  return 1; */
	case WM_DESTROY:
		if (offOldBitmap != NULL) {
			SelectObject(offDC, offOldBitmap);

		}
		if (offDC != NULL) {
			DeleteDC(offDC);

		}
		if (offBitmap != NULL) {
			DeleteObject(offBitmap);

		}
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
void GetTextInfoForKeyMsg(WPARAM wParam, const TCHAR* msgName,
	TCHAR* buf, int bufSize)
{
	static int counter = 0;
	if (wParam == 32)
		counter++;
	_stprintf_s(buf, bufSize, _T("%s key: %d ( counter : %d)"), msgName,
		wParam, counter);
}
void AddMenus(HWND hwnd) {

	HMENU hMenubar;

	hMenubar = CreateMenu();
	ghMenu[0] = CreateMenu();

	ghMenu[1] = CreateMenu();

	AppendMenuW(ghMenu[0], MF_STRING, ID_FILE_NEWGAME, L"&New Game\tF2");
	AppendMenuW(ghMenu[0], MF_STRING, 32772, L"&Pause\tEsc");
	AppendMenuW(ghMenu[0], MF_STRING, IDM_EXIT, L"&Exit\tAlt+F4");
	AppendMenuW(ghMenu[1], MF_STRING, IDM_ABOUT, L"&About\tF12");


	AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)ghMenu[0], L"&File");

	AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)ghMenu[1], L"&Help");
	SetMenu(hwnd, hMenubar);
}
void DrawBitmap(HWND hWnd) {
	EnableMenuItem(hPopupMenu, ID_TILED, MF_DISABLED | MF_GRAYED);
	EnableMenuItem(hPopupMenu, ID_STRETCHED, MF_ENABLED);
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	HBITMAP bitmap = (HBITMAP)LoadImage(NULL, ofn.lpstrFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	BITMAP bitmap01;
	HDC memDC = CreateCompatibleDC(hdc);
	HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, bitmap);

	GetObject(bitmap, sizeof(bitmap01), &bitmap01);

	int x = (GetSystemMetrics(SM_CXSCREEN) / 2) / bitmap01.bmWidth;
	if (x * bitmap01.bmWidth != (GetSystemMetrics(SM_CXSCREEN) / 2))
		x++;
	int y = (GetSystemMetrics(SM_CYSCREEN) / 2) / bitmap01.bmHeight;
	if (y * bitmap01.bmHeight != (GetSystemMetrics(SM_CYSCREEN) / 2))
		y++;

	for (int j = 0; j < x; j++) {
		for (int k = 0; k < y; k++) {
			BitBlt(hdc, j * bitmap01.bmWidth, k * bitmap01.bmHeight, bitmap01.bmWidth, bitmap01.bmHeight, memDC, 0, 0, SRCCOPY);
		}


	}

	SelectObject(memDC, oldBitmap);
	DeleteObject(bitmap);
	DeleteDC(memDC);
	EndPaint(hWnd, &ps);

}
void DrawBitmap_stretched(HWND hWnd) {
	EnableMenuItem(hPopupMenu, ID_TILED, MF_ENABLED);
	EnableMenuItem(hPopupMenu, ID_STRETCHED, MF_DISABLED | MF_GRAYED);
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	HBITMAP bitmap = (HBITMAP)LoadImage(NULL, ofn.lpstrFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);// LoadBitmap(hInst,
	BITMAP bitmap01;
	HDC memDC = CreateCompatibleDC(hdc);
	HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, bitmap);
	GetObject(bitmap, sizeof(bitmap01), &bitmap01);
	BitBlt(hdc, 0, 0, bitmap01.bmWidth, bitmap01.bmHeight, memDC, 0, 0, SRCCOPY);
	StretchBlt(hdc, 0, 0, (GetSystemMetrics(SM_CXSCREEN) / 2), (GetSystemMetrics(SM_CYSCREEN) / 2), memDC,
		0, 0, bitmap01.bmWidth, bitmap01.bmHeight, SRCCOPY);
	SelectObject(memDC, oldBitmap);
	DeleteObject(bitmap);
	DeleteDC(memDC);
	EndPaint(hWnd, &ps);
}
void PaintKid(HWND hWnd) {

	PAINTSTRUCT ps;
	RECT rc;
	HDC hdc = BeginPaint(hWnd, &ps);
	GetClientRect(hWnd, &rc);

	SetBkColor(hdc, RGB(0, 0, 0));
	ExtTextOut(hdc, 0, 0, ETO_OPAQUE, &rc, 0, 0, 0);
	SetBkColor(hdc, RGB(0, 0, 0));
	SetTextColor(hdc, RGB(255, 255, 255));
	char c[2];
	for (int j = 0; j < KID_AMOUNT; j++) {
		if (hWnd == kid[j].hWndc) {
			c[0] = kid[j].c[0];
			c[1] = kid[j].c[1];
		}
	}

	DrawText(hdc, (LPCWSTR)c, 1, &rc, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
	// UpdateWindow(hWnd);
	 // TODO: Add any drawing code that uses hdc here...
	EndPaint(hWnd, &ps);

}
void ChangeBackground(HWND hWnd) {
	bitmap_chosen = 0;
	CHOOSECOLOR cc;                 // common dialog box structure 
	static COLORREF acrCustClr[16]; // array of custom colors 
  //  HWND hwnd;                      // owner window
	HBRUSH hbrush;                  // brush handle
	static DWORD rgbCurrent;        // initial color selection

	// Initialize CHOOSECOLOR 
	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = hWnd;
	cc.lpCustColors = (LPDWORD)acrCustClr;
	cc.rgbResult = rgbCurrent;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	if (ChooseColor(&cc) == TRUE)
	{
		hbrush = CreateSolidBrush(cc.rgbResult);
		rgbCurrent = cc.rgbResult;
	}
	RECT rc;
	GetClientRect(hWnd, &rc);
	HBRUSH brush = CreateSolidBrush(rgbCurrent);
	SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG_PTR)brush);
	InvalidateRect(hWnd, &rc, 1);



}
void HideKid(HWND hWnd, WPARAM wParam) {

	int found = 0;

	RECT rc;
	RECT rc2;
	int lowest = 0;
	int lowestj = 0;
	for (int j = 0; j < KID_AMOUNT; j++) {

		GetWindowRect(kid[j].hWndc, &rc);
		GetClientRect(kid[j].hWndc, &rc2);
		kid[j].howlow = (rc.bottom - rc.top + z[j] - rc2.bottom - rc2.top);
		if ((kid[j].howlow > lowest && kid[j].visible) && kid[j].c[0] == wParam + 32) {
			lowest = kid[j].howlow;
			lowestj = j;
			found = 1;
		}


	}if (found) {
		ShowWindow(kid[lowestj].hWndc, SW_HIDE);
		kid[lowestj].visible = false;
	}
	else
		wrongkeys++;




}
void RightClickMenu(HWND hWnd) {

	hPopupMenu = CreatePopupMenu();

	InsertMenuW(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_COLOR, L"Color...\tCtrl+C");
	InsertMenuW(hPopupMenu,0, MF_BYPOSITION | MF_STRING, MF_SEPARATOR, NULL);
	InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_STRETCHED, L"Streach\tCtrl+S");

	InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_TILED, L"Tile\tCtrl+T");
	InsertMenu(hPopupMenu, 0, MF_BYPOSITION | MF_STRING, ID_BITMAP, L"Bitmap...\tCtrl+B");

	EnableMenuItem(hPopupMenu, ID_TILED, MF_DISABLED | MF_GRAYED);
	EnableMenuItem(hPopupMenu, ID_STRETCHED, MF_DISABLED | MF_GRAYED);
	SetForegroundWindow(hWnd);
	POINT cursor;
	GetCursorPos(&cursor);
	TrackPopupMenu(hPopupMenu, TPM_TOPALIGN | TPM_LEFTALIGN, cursor.x, cursor.y, 0, hWnd, NULL);



}
void Timer7(HWND hWnd) {
	RECT rc;
	RECT rc2;
	for (int j = 0; j < KID_AMOUNT; j++) {
		if (hWnd == kid[j].hWndc && kid[j].hWndc != NULL) {
			z[j] += kid[j].incerement;
			GetWindowRect(kid[j].hWndc, &rc);
			GetClientRect(kid[j].hWndc, &rc2);
			if (((rc.bottom - rc.top + z[j] - rc2.bottom - rc2.top) >= (GetSystemMetrics(SM_CYSCREEN) / 2) - 80) && kid[j].visible == true) {
				ShowWindow(kid[j].hWndc, SW_HIDE);
				kid[j].visible = false;
				missed++;
			}
			else {
				MoveWindow(kid[j].hWndc, rc2.left - rc2.right + rc.right - rc.left + py[j], rc.bottom - rc.top + z[j] - rc2.bottom - rc2.top,
					25, 25, TRUE);
				
				InvalidateRect(kid[j].hWndc, &rc, 0);

			}
		}
	}

}
void Timer8(HWND hWnd, int& privy, RECT& rc_c) {
	
	y += 50;
	privy = y;
		py[i] = rand() % (rc_c.left - rc_c.right - 25);
		kid[i].hWndc = CreateWindow(szWindowClass, szTitle, WS_CHILDWINDOW,//| WS_CLIPCHILDREN | WS_VISIBLE | WS_BORDER,
			py[i], 0, 25, 25, hWnd, nullptr, hInst, nullptr);

		kid[i].visible = true;
		if (!kid[i].hWndc) {
			return;
		}
		z[i] = 0;
		kid[i].incerement = rand() % 2 + 1;

		kid[i].c[0] = 'a' + rand() % 26;
		kid[i].c[1] = '\0';
		UpdateWindow(kid[i].hWndc);
		ShowWindow(kid[i].hWndc, SW_SHOW);
		SetFocus(hWnd);
		i++;
		if (i == KID_AMOUNT)
			i = 0;

}
void SpaceToRemove() {


	RECT rc;
	RECT rc2;
	int lowest = 0;
	int lowestj = 0;
	for (int j = 0; j < KID_AMOUNT; j++) {

		GetWindowRect(kid[j].hWndc, &rc);
		GetClientRect(kid[j].hWndc, &rc2);
		kid[j].howlow = (rc.bottom - rc.top + z[j] - rc2.bottom - rc2.top);
		if (kid[j].howlow > lowest && kid[j].visible) {
			lowest = kid[j].howlow;
			lowestj = j;
		}


	}
	ShowWindow(kid[lowestj].hWndc, SW_HIDE);
	kid[lowestj].visible = false;


}