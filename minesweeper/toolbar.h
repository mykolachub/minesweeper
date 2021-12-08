#pragma once

class Toolbar {
private:
	int isToolbarPressed;
public:
	void initToolbar();
	void createToolbar(HWND, HINSTANCE);
	void resizeToolbar(HWND);
	void notifyToolbar(HWND, WPARAM, LPARAM);
	void pressToolbar(HWND, int);
};