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
	void LoadAllTexture();

	void Update(float _DeltaTime);
	void Render(float _DeltaTime);

	void SetLight();
	//IMGUI
public:
	BOOL ImguiInit();
	void ImguiUpdate();

	void AddGUIFunction(const std::function<void()> _Func)
	{
		GUIFunctions.push_back(_Func);
	}

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
	BOOL CreateDoubleBuffer();

	BOOL CreateVertexShader(const std::wstring& _ShaderFileName, std::vector<D3D11_INPUT_ELEMENT_DESC> _InputElement);
	BOOL CreateInputLayOut(std::vector<D3D11_INPUT_ELEMENT_DESC> _InputElement, Microsoft::WRL::ComPtr<ID3D11InputLayout>& _InputLayOut, Microsoft::WRL::ComPtr<ID3DBlob> _ShaderBlob);
	BOOL CreatePixelShader(const std::wstring& _ShaderFileName);

	BOOL LoadTexture(const std::string& _TextureName);
	BOOL CreateSampler();
	RenderTarget CreateRenderTarget(UINT _Width, UINT _Height);
	
	RenderTarget GetDoubleBuffer()
	{
		return { DoubleBufferRTV, DoubleBufferSRV };
	}

	void SetViewport();

	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice()
	{
		return Device;
	}

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext()
	{
		return Context;
	}

	VertexShaderData& GetVertexShaderData(const std::wstring& _ShaderName)
	{
		return VertexShaders[_ShaderName];
	}

	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetPixelShaderData(const std::wstring& _ShaderName)
	{
		return PixelShaders[_ShaderName];
	}

	const Microsoft::WRL::ComPtr<ID3D11SamplerState> GetSampler(const std::string& _SamplerName)
	{
		if (Samplers.find(_SamplerName) == Samplers.end())
		{
			std::cout << "No Exist Sampler" << std::endl;
		}

		return Samplers[_SamplerName];
	}

	EWorldLight& GetWorldLight()
	{
		return WorldLight;
	}

	std::pair<int, int> GetWindowSize()
	{
		return { WindowWidth, WindowHeight };
	}

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> GetDepthStencilView()
	{
		return DepthStencilView;
	}

public:
	DirectX::SimpleMath::Matrix ViewMat;
	
	DirectX::SimpleMath::Vector3 CameraRotation;
	DirectX::SimpleMath::Vector3 CameraTranslation = { 0.0f, 0.0f, -2.0f };

	DirectX::SimpleMath::Vector3 EyePos = { 0.0f, 0.0f, -1.0f };
	DirectX::SimpleMath::Vector3 EyeDir = { 0.0f, 0.0f, 1.0f };
	DirectX::SimpleMath::Vector3 UpDir = { 0.0f, 1.0f, 0.0f };

public:
	void AddRenderer(std::shared_ptr<class Renderer> _NewRenderer);
	void CreatePostProcess();

protected:


private:
	EWorldLight WorldLight;

	std::list<std::shared_ptr<class Renderer>> Renderers;

	std::unordered_map<std::wstring, VertexShaderData> VertexShaders;
	std::unordered_map<std::wstring, Microsoft::WRL::ComPtr<ID3D11PixelShader>> PixelShaders;
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11SamplerState>> Samplers;

	std::list<std::function<void()>> GUIFunctions;
private:
	int WindowWidth = 0;
	int WindowHeight = 0;

	HINSTANCE hInstance;
	WNDCLASSEXW wcex;
	MSG msg;
	HWND hWnd;
private:
	bool isWireFrame = false;

	Microsoft::WRL::ComPtr<ID3D11Device> Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> Context;
	Microsoft::WRL::ComPtr<IDXGISwapChain> SwapChain;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> BackBufferRTV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> BackBufferSRV;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> DoubleBufferRTV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> DoubleBufferSRV;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> SolidRasterizerState;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> WireRasterizerState;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> DepthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> DepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> DepthStencilState;
	
	D3D11_VIEWPORT ScreenViewPort;

	UINT NumQualityLevels = 0;

	std::vector<std::shared_ptr<class PostProcess>> PostProcesses;

	std::shared_ptr<Renderer> BackBufferRenderer;
};

