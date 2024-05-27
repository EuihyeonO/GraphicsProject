#include "PostProcess.h"
#include "ScreenRenderer.h"

PostProcess::PostProcess()
{
}

PostProcess::~PostProcess()
{
}

void PostProcess::SetTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _SRV, int _Index)
{
	std::shared_ptr<RenderBase> RenderUnit = *PostProcessRenderer->GetUnits().begin();
	RenderUnit->SetTextureToSRV(_SRV, _Index);
}

void PostProcess::Init()
{
	PostProcessRenderer = std::make_shared<ScreenRenderer>();

	RenderTarget DoubleBuffer = EngineBase::GetInstance().GetDoubleBuffer();
	SetDoubleBuffer(DoubleBuffer);
}

void PostProcess::Render(float _DeltaTime)
{
	PostProcessRenderer->Render(_DeltaTime);

	ID3D11ShaderResourceView* SRV[2] = { NULL, };
	EngineBase::GetInstance().GetContext()->PSSetShaderResources(0, 2, SRV);
}
