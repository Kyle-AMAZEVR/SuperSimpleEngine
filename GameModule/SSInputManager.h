#pragma once

class GAMEMODULE_API SSInputManager
{
public:
	void OnKeyDown(ULONGLONG keyCode);
	void OnKeyUp(ULONGLONG keyCode);

	static SSInputManager& Get();
	static SSInputManager* GetPtr();

	void OnMouseMove(int x, int y);
	
	void OnMouseLBtnDown(int x, int y);
	void OnMouseLBtnUp();

	void OnMouseRBtnDown(int x, int y);

private:

	int mMouseLBtnDownPosX = 0;
	int mMouseLBtnDownPosY = 0;
	int mLastMousePosX = 0;
	int mLastMousePosY = 0;

	bool mMouseLBtnDown = false;

	static SSInputManager* mInstance;
};