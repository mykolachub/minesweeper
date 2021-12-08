#pragma once
#include "game.h"

class GameController {
public:
	Game* game;
	GameController();
	int difficulty = 1; // default
	void onStart(); // new window
	void onDestroy(); // destroy window
	void onLBUp(); // when left mouse button uped
	void onRBUp(); // when right mouse button uped

	void setBegginerDifficulty();
	void setIntermediateDifficulty();
	void setExpertDifficulty();
};