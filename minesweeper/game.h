#pragma once
#include "Resource.h"
#include "blocks.h"
#include "framework.h"

class Game {
private:
	int blockSize = 24, gameCoordsX = 0, gameCoordsY = 37;
public:
	bool isGameOver = false;
	int rows = 9, cols = 9, bombs = 10;
	Block*** board = NULL;

	//Block* activeElement = nullptr;
public:
	Game();
	~Game();
	
	void startGame(HWND hWnd);
	void clearBoard();
	void restartGame(HWND hWnd);
	void setBegginer(HWND hWnd);
	void setIntermediate(HWND hWnd);
	void setExpert(HWND hWnd);
	bool detectSaveSpace(Block* block, HWND hWnd);
	Block* getActiveItem(HWND hWnd);

	void onPaint(HWND hWnd, Block* block);
	void onLBUp(HWND hWnd);
	void onRBUp(HWND hWnd);
};