#pragma once
#include "BaseHeader.h"

class RenderBase;

class EngineBase
{
public:
	EngineBase(const EngineBase& _Other) = delete;
	EngineBase(EngineBase&& _Other) noexcept = delete;
	EngineBase& operator=(const EngineBase& _Other) = delete;
	EngineBase& operator=(EngineBase&& _Other) noexcept = delete;

private:
	EngineBase();

	~EngineBase()
	{
		if (Instance != nullptr)
		{
			delete (Instance);
		}
	}

	static EngineBase* Instance;

	//static
public:
	static EngineBase* GetInstance()
	{
		if (Instance == nullptr)
		{
			Instance = new EngineBase();
		}

		return Instance;
	}

	//Process
public:
	BOOL Init(HINSTANCE _hInstance, int _Width, int _Height);
	void Loop();
	WPARAM End();

	//Windows API
public:
	BOOL WindowInit(HINSTANCE _hInstance);

	ATOM MyRegisterClass(HINSTANCE hInstance);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);

	//DirectX
public:
	BOOL DirectXInit();
	BOOL CreateDevice();
	BOOL CreateSwapChain();
	BOOL CreateRasterizerState();
	BOOL CreateDepthStencil();

	void SetViewport();

	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice()
	{
		return Device;
	}

public:
	template <typename T>
	static std::shared_ptr<T> CreateRenderer()
	{
		std::shared_ptr<class RenderBase> NewRenderer = std::make_shared<T>();
		NewRenderer->Init();
		Renderers.insert(NewRenderer);

		return std::dynamic_pointer_cast<T>(NewRenderer);
	}

protected:

private:
	std::list<std::shared_ptr<class RenderBase>> Renderers;

private:
	int WindowWidth = 0;
	int WindowHeight = 0;

	HINSTANCE hInstance;
	WNDCLASSEXW wcex;
	MSG msg;
	HWND hWnd;
private:

	Microsoft::WRL::ComPtr<ID3D11Device> Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> Context;
	Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTargetView;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState>RasterizerState;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> DepthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DepthStencilState;
	
	D3D11_VIEWPORT ScreenViewPort;

	UINT NumQualityLevels = 0;
};

