#include <windows.h>
#include <windowsx.h>
#include "framework.h"
#include "blocks.h"
#include "colors.h"

#define field_default 2 * 24
#define field_flag 3 * 24
#define field_bomb 4 * 24
#define field_empty 5 * 24
#define field_one 6 * 24
#define field_two 7 * 24
#define field_tree 8 * 24
#define field_four 9 * 24
#define field_five 10 * 24
#define field_six 11 * 24
#define field_seven 12 * 24
#define field_eight 13 * 24


void drawTriangle(HWND hWnd, int x1, int y1, int x2, int y2, int x3, int y3, COLORREF color);
void drawRectangle(HWND hWnd, int x1, int y1, int x2, int y2, COLORREF color);
bool LoadAndBlitBitmap(LPCWSTR szFileName, HDC hdc, int destX, int destY, int width, int height, int srcX, int srcY);

// BLOCK

void Block::setCoords(int newX, int newY, int mapI, int mapJ) {
	x = newX;
	y = newY;
	i = mapI;
	j = mapJ;
}

// FIELD

Field::Field() {
	mines = 0;
	isClicked = false;
	isMine = false;
	isFlagged = false;
}

void Field::render(HWND hWnd, int blockSize) {
	HDC hdc = GetDC(hWnd);
	if (isFlagged) {
		LoadAndBlitBitmap(__T("bitmap1.bmp"), hdc, x, y, 24, 24, field_flag, 0);
	}
	else if (isClicked) {
		LoadAndBlitBitmap(__T("bitmap1.bmp"), hdc, x, y, 24, 24, (5 + mines) * 24, 0);
	}
	else {
		LoadAndBlitBitmap(__T("bitmap1.bmp"), hdc, x, y, 24, 24, field_default, 0);
	}
}

// MINE

Mine::Mine() {
	mines = 0;
	isMine = true;
	isClicked = false;
	isFlagged = false;
}

void Mine::render(HWND hWnd, int blockSize) {
	HDC hdc = GetDC(hWnd);
	if (isFlagged) {
		LoadAndBlitBitmap(__T("bitmap1.bmp"), hdc, x, y, 24, 24, field_flag, 0);
	}
	else if (isClicked) {
		LoadAndBlitBitmap(__T("bitmap1.bmp"), hdc, x, y, 24, 24, field_bomb, 0);
	}
	else {
		LoadAndBlitBitmap(__T("bitmap1.bmp"), hdc, x, y, 24, 24, field_default, 0);
	}
}

bool LoadAndBlitBitmap(LPCWSTR szFileName, HDC hdc, int destX, int destY, int width, int height, int srcX, int srcY) {
	HBITMAP hBitmap;
	hBitmap = (HBITMAP)::LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (hBitmap == NULL) {
		::MessageBox(NULL, __T("LoadImage Failed"), __T("Error"), MB_OK);
		return 0;
	}

	HDC hLocalDC;
	hLocalDC = ::CreateCompatibleDC(hdc);
	if (hLocalDC == NULL) {
		::MessageBox(NULL, __T("CreateCompatibleDC Failed"), __T("Error"), MB_OK);
		return 0;
	}

	BITMAP qBitmap;
	int iReturn = GetObject(reinterpret_cast<HGDIOBJ>(hBitmap), sizeof(BITMAP), reinterpret_cast<LPVOID>(&qBitmap));
	if (iReturn == NULL) {
		::MessageBox(NULL, __T("GetObject Failed"), __T("Error"), MB_OK);
		return 0;
	}

	HBITMAP hOldBmp = (HBITMAP)::SelectObject(hLocalDC, hBitmap);
	if (hOldBmp == NULL) {
		::MessageBox(NULL, __T("SelectObject Failed"), __T("Error"), MB_OK);
		return 0;
	}

	BOOL qRetBlit = ::BitBlt(hdc, destX, destY, width, height, hLocalDC, srcX, srcY, SRCCOPY);
	if (qRetBlit == NULL) {
		::MessageBox(NULL, __T("BitBlt Failed"), __T("Error"), MB_OK);
		return 0;
	}

	::SelectObject(hLocalDC, hOldBmp);
	::DeleteDC(hLocalDC);
	::DeleteObject(hBitmap);
	return 1;
}

void drawTriangle(HWND hWnd, int x1, int y1, int x2, int y2, int x3, int y3, COLORREF color) {
	HDC hdc = GetDC(hWnd);
	HPEN hPen, hOldPen;
	HBRUSH hBrush, hOldBrush;
	
	hPen = CreatePen(PS_SOLID, 2, color);
	hBrush = CreateSolidBrush(color);

	hOldPen = SelectPen(hdc, hPen);
	hOldBrush = SelectBrush(hdc, hBrush);

	POINT vertices[] = { {x1, y1}, {x2, y2}, {x3, y3} };
	Polygon(hdc, vertices, sizeof(vertices) / sizeof(vertices[0]));

	SelectBrush(hdc, hOldBrush);
	DeleteObject(hBrush);

	SelectPen(hdc, hOldPen);
	DeleteObject(hPen);
}

void drawRectangle(HWND hWnd, int x1, int y1, int x2, int y2, COLORREF color) {
	HDC hdc = GetDC(hWnd);
	RECT rect = { x1, y1, x2, y2 };
	HBRUSH brush = CreateSolidBrush(color);

	FillRect(hdc, &rect, brush);

	DeleteObject(brush);
}