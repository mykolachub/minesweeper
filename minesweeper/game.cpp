#include "framework.h"
#include <math.h>
#include "game.h"
#include "colors.h"
#include <windowsx.h>

#include <cstdlib>
#include <ctime>

Game::Game() {}
Game::~Game() {
	clearBoard();
}

void Game::clearBoard() {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			delete board[i][j];
		}
		delete[] board[i];
	}
	delete[] board;
	board = NULL;
}

void Game::restartGame(HWND hWnd) {
	clearBoard();
	startGame(hWnd);
}
 
void Game::setBegginer(HWND hWnd) {
	clearBoard();
	rows = 9; cols = 9; bombs = 10;
	startGame(hWnd);
};

void Game::setIntermediate(HWND hWnd) {
	clearBoard();
	rows = 16; cols = 16; bombs = 40;
	startGame(hWnd);
};

void Game::setExpert(HWND hWnd) {
	clearBoard();
	rows = 16; cols = 30; bombs = 99;
	startGame(hWnd);
};

void Game::startGame(HWND hWnd) {
	isGameOver = false;

	// динамический массив
	int** boardTemplate = new int* [rows];
	for (int i = 0; i < rows; i++) {
		boardTemplate[i] = new int[cols] {0};
	}
	
	// динамический массив
	board = new Block** [rows];
	for (int i = 0; i < rows; i++) {
		board[i] = new Block* [cols];
	}

	// генерация бомб
	srand(time(NULL));
	for (size_t bomb = 0; bomb <= bombs; bomb++) {
		int i = rand() % rows;
		int j = rand() % cols;
		boardTemplate[i][j] = -1;
		board[i][j] = new Mine();
		board[i][j]->setCoords(gameCoordsX + j * blockSize, gameCoordsY + i * blockSize, i, j);
	}

	// генерация полей
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (boardTemplate[i][j] == 0) {
				board[i][j] = new Field();
				board[i][j]->setCoords(gameCoordsX + j * blockSize, gameCoordsY + i * blockSize, i, j);
			}
		}
	}

	// генерация полей-подсказок (цифр)
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (boardTemplate[i][j] == 0) {
				int counter = 0;
				/* i- j- */ if (i != 0 && j != 0 && board[i - 1][j - 1]->isMine) { counter += 1; }
				/* i- j  */ if (i != 0 && board[i - 1][j]->isMine) { counter += 1; }
				/* i- j+ */ if (i != 0 && j != cols - 1 && board[i - 1][j + 1]->isMine) { counter += 1; }

				/* i  j- */ if (j != 0 && board[i][j - 1]->isMine) { counter += 1; }
				/* i  j+ */ if (j != cols - 1 && board[i][j + 1]->isMine) { counter += 1; }

				/* i+ j- */ if (i != rows - 1 && j != 0 && board[i + 1][j - 1]->isMine) { counter += 1; }
				/* i+ j  */ if (i != rows - 1 && board[i + 1][j]->isMine) { counter += 1; }
				/* i+ j+ */ if (i != rows - 1 && j != cols - 1 && board[i + 1][j + 1]->isMine) { counter += 1; }

				boardTemplate[i][j] = counter;
				board[i][j]->mines = counter;
			}
		}
	}

	InvalidateRect(hWnd, NULL, TRUE);
	//onPaint(hWnd, nullptr);

	// деаллокация динамического массива
	/*for (int i = 0; i < rows; i++)
		delete[] boardTemplate[i];
	delete[] boardTemplate;
	boardTemplate = NULL;*/
}

Block* Game::getActiveItem(HWND hWnd) {
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(hWnd, &pt);
	bool clickedOnBoard = pt.x >= gameCoordsX && pt.x <= gameCoordsX + cols * blockSize &&
						  pt.y >= gameCoordsY && pt.y <= gameCoordsY + rows * blockSize;
	if (clickedOnBoard) {
		int j = floor((double)(pt.x - (double)gameCoordsX) / (double)blockSize);
		int i = floor((double)(pt.y - (double)gameCoordsY) / (double)blockSize);
		
		return board[i][j];
	}
	return nullptr;
}

bool Game::detectSaveSpace(Block* block, HWND hWnd) {
	if (block->mines > 0) {
		block->isClicked = true;
		onPaint(hWnd, block);
		return 1;
	}
	else if (block->mines == 0 && block->isClicked == false) {
		const int i = block->i;
		const int j = block->j;
		block->isClicked = true;
		onPaint(hWnd, block);
		/* i- j- */ if (i != 0 && j != 0) { detectSaveSpace(board[i - 1][j - 1], hWnd); }
		/* i- j  */ if (i != 0) { detectSaveSpace(board[i - 1][j], hWnd); }
		/* i- j+ */ if (i != 0 && j != cols - 1) { detectSaveSpace(board[i - 1][j + 1], hWnd); }

		/* i  j- */ if (j != 0) { detectSaveSpace(board[i][j - 1], hWnd); }
		/* i  j+ */ if (j != cols - 1) { detectSaveSpace(board[i][j + 1], hWnd); }

		/* i+ j- */ if (i != rows - 1 && j != 0) { detectSaveSpace(board[i + 1][j - 1], hWnd); }
		/* i+ j  */ if (i != rows - 1) { detectSaveSpace(board[i + 1][j], hWnd); }
		/* i+ j+ */ if (i != rows - 1 && j != cols - 1) { detectSaveSpace(board[i + 1][j + 1], hWnd); }
	}
};

// ЛКМ
void Game::onLBUp(HWND hWnd) {
	Block* clickedItem = getActiveItem(hWnd);
	// если блок есть и еще не кликали и не поставили флаг
	if (clickedItem != nullptr && clickedItem->isClicked == false && clickedItem->isFlagged == false)
	{
		// если не мина
		if (clickedItem->isMine == false) {
			detectSaveSpace(clickedItem, hWnd);
		}
		// если мина
		else {
			isGameOver = true;
			clickedItem->isClicked = true;
			onPaint(hWnd, nullptr);
		}
	}
}

// ПКМ
void Game::onRBUp(HWND hWnd) {
	Block* clickedItem = getActiveItem(hWnd);
	// если блок есть
	if (clickedItem != nullptr && clickedItem->isClicked == false) 
	{
		// если небыло флага - ставим флаг
		if (clickedItem->isFlagged == false) {
			clickedItem->isFlagged = true;
		}
		// если флаг уже есть - убираем флаг
		else {
			clickedItem->isFlagged = false;
		}
		onPaint(hWnd, clickedItem);
	}
}

void Game::onPaint(HWND hWnd, Block* block) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	if (block == nullptr) {
		if (isGameOver == true) {
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < cols; j++) {
					board[i][j]->isClicked = true;
					if (board[i][j]->isMine == true) {
						board[i][j]->render(hWnd, blockSize);
					}
				}
			}
		}
		else if (board != NULL) {
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < cols; j++) {
					board[i][j]->render(hWnd, blockSize);
				}
			}
		}
	}
	else {
		block->render(hWnd, blockSize);
	}

	EndPaint(hWnd, &ps);
}