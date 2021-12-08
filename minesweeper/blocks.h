#pragma once
#include "framework.h"

class Block {
public: 
	long x, y, i, j;
	bool isMine, isClicked, isFlagged;
	int mines;
	void setCoords(int x, int y, int mapI, int mapJ);
	virtual void render(HWND hWnd, int blockSize) = 0;
};

class Field : public Block {
public:
	Field();
	void render(HWND hWnd, int blockSize);
};

class Mine : public Block {
public:
	Mine();
	void render(HWND hWnd, int blockSize);
};