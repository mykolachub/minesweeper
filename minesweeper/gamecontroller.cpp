
#include "gamecontroller.h"
#include "game.h"

GameController::GameController() {}

void GameController::onStart() {
	/*int row, colomn, bombs;
	if (difficulty == 2) {
		// intermediate
		row = 16;
		colomn = 16;
		bombs = 40;
	}
	else if (difficulty == 3) {
		// expert
		row = 16;
		colomn = 30;
		bombs = 99;
	}
	else {
		// begginer
		row = 9;
		colomn = 9;
		bombs = 10;
	}


	game = new Game(row, colomn, bombs);
	*/
}

void GameController::onDestroy() {
	delete game;
}

void GameController::setBegginerDifficulty() {
	difficulty = 1;
}

void GameController::setIntermediateDifficulty() {
	difficulty = 2;
}

void GameController::setExpertDifficulty() {
	difficulty = 3;
}