// minesweeper.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "minesweeper.h"
#include "game.h"

#include "toolbar.h"
#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")
#include <windows.h>

#define MAX_LOADSTRING 100

Toolbar toolbar;
Game game;

#define ID_TOOL_START 1
#define ID_TOOL_KILL 2


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

    //InitCommonControls(); // Toolbar init
    toolbar.initToolbar();

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MINESWEEPER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MINESWEEPER));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MINESWEEPER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MINESWEEPER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN,
      CW_USEDEFAULT, CW_USEDEFAULT, 10*24 - 9, 59 + 37 + 24*9, nullptr, nullptr, hInstance, nullptr);

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE:
            toolbar.createToolbar(hWnd, hInst);
            game.startGame(hWnd);
            break;

        case WM_SIZE:
            toolbar.resizeToolbar(hWnd);
            break;

        case WM_NOTIFY:
            toolbar.notifyToolbar(hWnd, wParam, lParam);
            break;

        // ЛКМ нажата
        case WM_LBUTTONDOWN:
            break;

        // ЛКМ поднята
        case WM_LBUTTONUP:
            game.onLBUp(hWnd);
            break;

        // ПКМ поднята
        case WM_RBUTTONUP:
            game.onRBUp(hWnd);
            break;

        case WM_PAINT: {
            /*
            RECT rect = { clickedItem->x, clickedItem->y, clickedItem->x + blockSize, clickedItem->y + blockSize };
		    activeElement = clickedItem;
		    InvalidateRect(hWnd, &rect, TRUE);
            */
            game.onPaint(hWnd, nullptr);
        }
        break;

        case WM_COMMAND: {
                int wmId = LOWORD(wParam);
                switch (wmId) {
                    // START
                    case ID_TOOL_START:
                        // toolbar.pressToolbar(hWnd, ID_TOOL_START);
                        game.restartGame(hWnd);
                        break;
                    // KILL
                    case ID_TOOL_KILL:
                        toolbar.pressToolbar(hWnd, ID_TOOL_KILL);
                        PostQuitMessage(0);
                        break;

                    // BEGINNER LEVEL
                    case ID_LEVEL_BEGINNER:
                        game.setBegginer(hWnd);
                        SetWindowPos(hWnd, NULL, 0, 0, 10 * 24 - 9, 59 + 37 + 24 * 9, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE);
                        break;
                    // BEGINNER INTERMEDIATE
                    case ID_LEVEL_INTERMEDIATE:
                        game.setIntermediate(hWnd);
                        SetWindowPos(hWnd, NULL, 0, 0, 16 * 25, 59 + 37 + 24 * 16, SWP_NOZORDER | SWP_NOACTIVATE| SWP_NOMOVE);
                        break; 
                    // BEGINNER EXPERT
                    case ID_LEVEL_EXPERT:
                        game.setExpert(hWnd);
                        SetWindowPos(hWnd, NULL, 0, 0, 30 * 25 - 14, 59 + 37 + 24 * 16, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE);
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
