#include "BloomPostProcess.h"
#include "ScreenRenderer.h"

BloomPostProcess::BloomPostProcess()
{
}

BloomPostProcess::~BloomPostProcess()
{
}

void BloomPostProcess::Init()
{
	PostProcessRenderer = std::make_shared<ScreenRenderer>();
	PostProcessRenderer->Init();

	PostProcessRenderer->CreateConstantBuffer(EShaderType::PSShader, L"BlurPixelShader.hlsl", BlurData);
	PostProcessRenderer->SetVSShader(L"MeshVertexShader.hlsl");

	PostProcessRenderer->SetSampler("LINEARWRAP");

	RenderTarget DoubleBuffer = EngineBase::GetInstance().GetDoubleBuffer();
	SetDoubleBuffer(DoubleBuffer);
	SetDepthStencil(EngineBase::GetInstance().GetDepthStencilView());

	std::pair<int, int> WindowSize = EngineBase::GetInstance().GetWindowSize();

	BlurTarget = EngineBase::GetInstance().CreateRenderTarget(BlurData.Width, BlurData.Height);
	DetectTarget = EngineBase::GetInstance().CreateRenderTarget(WindowSize.first, WindowSize.second);

	ZeroMemory(&DownViewPort, sizeof(D3D11_VIEWPORT));
	DownViewPort.TopLeftX = 0;
	DownViewPort.TopLeftY = 0;
	DownViewPort.Width = float(BlurData.Width);
	DownViewPort.Height = float(BlurData.Height);
	DownViewPort.MinDepth = 0.0f;
	DownViewPort.MaxDepth = 1.0f;

	ZeroMemory(&UpViewPort, sizeof(D3D11_VIEWPORT));
	UpViewPort.TopLeftX = 0;
	UpViewPort.TopLeftY = 0;
	UpViewPort.Width = float(1600);
	UpViewPort.Height = float(900);
	UpViewPort.MinDepth = 0.0f;
	UpViewPort.MaxDepth = 1.0f;

	CreateBlendState();
}

void BloomPostProcess::Render(float _DeltaTime)
{
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	//추출
	SetTexture(DoubleBufferSRV);

	EngineBase::GetInstance().GetContext()->ClearRenderTargetView(DetectTarget.RTV.Get(), ClearColor);
	EngineBase::GetInstance().GetContext()->OMSetRenderTargets(1, DetectTarget.RTV.GetAddressOf(), DepthStencilView.Get());
	
	PostProcessRenderer->SetPSShader(L"BrightDetectPixelShader.hlsl");
	PostProcessRenderer->Render(_DeltaTime);

	//블러
	SetTexture(DetectTarget.SRV);
	EngineBase::GetInstance().GetContext()->RSSetViewports(1, &DownViewPort);

	EngineBase::GetInstance().GetContext()->ClearRenderTargetView(BlurTarget.RTV.Get(), ClearColor);
	EngineBase::GetInstance().GetContext()->OMSetRenderTargets(1, BlurTarget.RTV.GetAddressOf(), DepthStencilView.Get());

	PostProcessRenderer->SetPSShader(L"BlurPixelShader.hlsl");
	PostProcessRenderer->Render(_DeltaTime);

	EngineBase::GetInstance().GetContext()->RSSetViewports(1, &UpViewPort);

	////병합
	SetTexture(BlurTarget.SRV, 0);

	EngineBase::GetInstance().GetContext()->OMSetRenderTargets(1, DoubleBufferRTV.GetAddressOf(), DepthStencilView.Get());
	EngineBase::GetInstance().GetContext()->OMSetBlendState(BlendState.Get(), NULL, 0xFFFFFFFF);

	PostProcessRenderer->SetPSShader(L"BloomPixelShader.hlsl");
	PostProcessRenderer->Render(_DeltaTime);
}

void BloomPostProcess::CreateBlendState()
{
	D3D11_BLEND_DESC Desc = { 0, };

	Desc.AlphaToCoverageEnable = false;
	Desc.IndependentBlendEnable = false;
	Desc.RenderTarget[0].BlendEnable = true;
	Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
	Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

	EngineBase::GetInstance().GetDevice()->CreateBlendState(&Desc, BlendState.GetAddressOf());
}
