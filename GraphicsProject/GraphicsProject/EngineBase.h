#pragma once
#include "Windows.h"

class EngineBase
{

public:

	EngineBase();
	~EngineBase();

	EngineBase(const EngineBase& _Other) = delete;
	EngineBase(EngineBase&& _Other) noexcept = delete;
	EngineBase& operator=(const EngineBase& _Other) = delete;
	EngineBase& operator=(EngineBase&& _Other) noexcept = delete;

	ATOM MyRegisterClass(HINSTANCE hInstance);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
	
	BOOL Init(HINSTANCE _hInstance);
	void Loop();
	WPARAM End();

protected:

private:
	HINSTANCE hInstance;
	WNDCLASSEXW wcex;
	MSG msg;
};

