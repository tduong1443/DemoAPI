// Demo.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Demo.h"
#include "cmath"

#define MAX_LOADSTRING 100
#define M_PI 3.14

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

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
    LoadStringW(hInstance, IDC_DEMO, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DEMO));

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

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(129));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDR_MENU1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

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

   HWND hWnd = CreateWindowW(szWindowClass, TEXT("Vẽ hình"), WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HDC hdc;
    static int xleft, xright, ytop, ybottom, Hinh, m = 60, s = 0, width, height;
    static HPEN hPen;
    static HBRUSH hBrush = NULL, hDiaBrush = NULL;
    static POINT point[10];
    static COLORREF colorNen = RGB(255, 255, 255), colorVien = RGB(0, 0, 0);
    static int styleNen = 0, styleVien = 0;
    static TCHAR timerleft[16];

    switch (message)
    {
    case WM_CREATE:
        SetTimer(hWnd, 1, 1000, NULL);
        break;
    case WM_SIZE:
        width = LOWORD(lParam);
        height = HIWORD(lParam);
        break;
    case WM_TIMER:
        if (s > 0 && s <= 59) {
            s = s - 1;
        }
        else {
            if (m > 0 && s == 0) {
                m = m - 1;
                s = 59;
            }
            if (m == 0 && s == 0) {
                KillTimer(hWnd, 1);
            }
        }
        wsprintfW(timerleft, L"Timer left: %d:%d", m, s);
        hdc = GetDC(hWnd);
        SetTextColor(hdc, RGB(255, 0, 0));
        TextOut(hdc, width - 120, height - 20, timerleft, 20);
        ReleaseDC(hWnd, hdc);
        break;
    case WM_LBUTTONDOWN:
        xleft = LOWORD(lParam);
        ytop = HIWORD(lParam);
        break;
    case WM_LBUTTONUP:
        xright = LOWORD(lParam);
        ybottom = HIWORD(lParam);
        hdc = GetDC(hWnd);
        hPen = CreatePen(styleVien, 1, colorVien);
        hBrush = CreateSolidBrush(colorNen);
        if (styleNen != 0) {
            hDiaBrush = CreateHatchBrush(styleNen, colorNen);
        }

        SelectObject(hdc, hPen);
        SelectObject(hdc, hBrush);
        SelectObject(hdc, hDiaBrush);

        if (Hinh == ID_TGC) {
            point[0].x = xleft + (xright - xleft) / 2;
            point[0].y = ytop;
            point[1].x = xleft;
            point[1].y = ybottom;
            point[2].x = xright;
            point[2].y = ybottom;
            Polygon(hdc, point, 3);
        }

        if (Hinh == ID_NGC) {
            int centerX = (xleft + xright) / 2; // Tọa độ x của tâm ngũ giác
            int centerY = (ytop + ybottom) / 2; // Tọa độ y của tâm ngũ giác (ngang với yTop)
            int radius = abs(xright - xleft) / 2; // Bán kính

            for (int i = 0; i < 5; ++i) {
                point[i].x = centerX + static_cast<int>(radius * sin(2 * M_PI * i / 5));
                point[i].y = centerY - static_cast<int>(radius * cos(2 * M_PI * i / 5));
            }

            Polygon(hdc, point, 5);
        }

        if (Hinh == ID_LGC) {
            int centerX = (xleft + xright) / 2; // Tọa độ x của tâm lục giác
            int centerY = (ytop + ybottom) / 2; // Tọa độ y của tâm lục giác
            int radius = abs(xright - xleft) / 2; // Bán kính
            for (int i = 0; i < 6; ++i) {
                point[i].x = centerX + static_cast<int>(radius * cos(2 * M_PI * i / 6));
                point[i].y = centerY - static_cast<int>(radius * sin(2 * M_PI * i / 6));
            }


            Polygon(hdc, point, 6);
        }

        if (Hinh == ID_HCN) {
            Rectangle(hdc, xleft, ytop, xright, ybottom);
        }

        if (Hinh == ID_ELLIPSE) {
            Ellipse(hdc, xleft, ytop, xright, ybottom);
        }

        if (Hinh == ID_CHORD) {
            Chord(hdc, xleft, ytop, xright, ybottom, xleft + (xright - xleft) / 2, ytop, xleft, ytop + (ybottom - ytop) * 3 / 4);
        }

        if (Hinh == ID_TGV) {
            point[0].x = xleft;
            point[0].y = ytop;
            point[1].x = xright;
            point[1].y = ybottom;
            point[2].x = xleft;
            point[2].y = ybottom;
            Polygon(hdc, point, 3);
        }
        break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
                // nút thoát
            case ID_THOAT:
                if (MessageBox(NULL, TEXT("Bạn có muốn thoát không ?"), TEXT("Thông báo"),
                    MB_YESNO | MB_ICONQUESTION)) PostQuitMessage(0);
                break;

                // vẽ hình
            case ID_NGC:
            case ID_TGC:
            case ID_LGC:
            case ID_CHORD:
            case ID_HCN:
            case ID_ELLIPSE:
            case ID_TGV:
                Hinh = wmId;
                break;

                // màu viền
            case ID_MV_DO:
                colorVien = RGB(255, 0, 0);
                break;
            case ID_MV_VANG:
                colorVien = RGB(255, 255, 0);
                break;
            case ID_MV_XANHLC:
                colorVien = RGB(0, 255, 0);
                break;

                // kiểu viền
            case ID_KV_SOLID:
                styleVien = PS_SOLID;
                break;
            case ID_KV_DASH:
                styleVien = PS_DASH;
                break;
            case ID_KV_DASHDOT:
                styleVien = PS_DASHDOT;
                break;
            case ID_KV_DOT:
                styleVien = PS_DOT;
                break;

                // màu nền
            case ID_MN_DO:
                colorNen = RGB(255, 0, 0);
                break;
            case ID_MN_VANG:
                colorNen = RGB(255, 255, 0);
                break;
            case ID_MN_XLC:
                colorNen = RGB(0, 255, 0);
                break;

                // Kiểu nền
            case ID_KN_GN:
                styleNen = HS_HORIZONTAL;
                hDiaBrush = CreateHatchBrush(styleNen, colorNen);
                break;
            case ID_KN_GD:
                styleNen = HS_VERTICAL;
                break;
            case ID_KN_DCT:
                styleNen = HS_CROSS;
                break;
            case ID_KN_DHT:
                styleNen = HS_DIAGCROSS;
                break;

            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
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
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
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
