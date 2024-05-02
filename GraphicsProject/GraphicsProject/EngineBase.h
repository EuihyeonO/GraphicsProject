#pragma once

#include "BaseHeader.h"

struct VertexShaderData
{
	Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> InputLayout;
};

class EngineBase
{
public:
	EngineBase(const EngineBase& _Other) = delete;
	EngineBase(EngineBase&& _Other) noexcept = delete;
	EngineBase& operator=(const EngineBase& _Other) = delete;
	EngineBase& operator=(EngineBase&& _Other) noexcept = delete;

private:
	EngineBase();
	~EngineBase();

	//static
public:
	static EngineBase& GetInstance()
	{
		static EngineBase Instance;
		return Instance;
	}

	//Process
public:
	BOOL Init(HINSTANCE _hInstance, int _Width, int _Height);
	void Loop();
	WPARAM End();

	void CreateAllShader();

	void Update();
	void Render();

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

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext()
	{
		return Context;
	}

	BOOL CreateVertexShader(const std::wstring& _ShaderFileName, std::vector<D3D11_INPUT_ELEMENT_DESC> _InputElement);
	BOOL CreateInputLayOut(std::vector<D3D11_INPUT_ELEMENT_DESC> _InputElement, Microsoft::WRL::ComPtr<ID3D11InputLayout>& _InputLayOut, Microsoft::WRL::ComPtr<ID3DBlob> _ShaderBlob);
	
	VertexShaderData& GetVertexShaderData(const std::wstring& _ShaderName)
	{
		return VertexShaders[_ShaderName];
	}

	BOOL CreatePixelShader(const std::wstring& _ShaderFileName);

	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetPixelShaderData(const std::wstring& _ShaderName)
	{
		return PixelShaders[_ShaderName];
	}

public:
	void AddRenderer(std::shared_ptr<class RenderBase> _NewRenderer);

protected:

private:
	std::list<std::shared_ptr<class RenderBase>> Renderers;

	std::unordered_map<std::wstring, VertexShaderData> VertexShaders;
	std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<ID3D11PixelShader>> PixelShaders;

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

