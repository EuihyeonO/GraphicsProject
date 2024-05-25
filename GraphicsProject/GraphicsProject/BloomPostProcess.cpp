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

	PostProcessRenderer->CreateConstantBuffer(EShaderType::PSShader, L"BloomPixelShader.hlsl", BloomData);

	PostProcessRenderer->SetVSShader(L"BloomVertexShader.hlsl");
	PostProcessRenderer->SetPSShader(L"BloomPixelShader.hlsl");

	PostProcessRenderer->SetSampler("LINEARWRAP");
}

void BloomPostProcess::Render(float _DeltaTime)
{
	PostProcessRenderer->Render(_DeltaTime);

	ID3D11ShaderResourceView* SRV = NULL;
	EngineBase::GetInstance().GetContext()->PSSetShaderResources(0, 1, &SRV);
}
