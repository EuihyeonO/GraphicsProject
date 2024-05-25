#include "PostProcess.h"
#include "ScreenRenderer.h"

PostProcess::PostProcess()
{
}

PostProcess::~PostProcess()
{
}

void PostProcess::SetTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _SRV)
{
	std::shared_ptr<RenderBase> RenderUnit = *PostProcessRenderer->GetUnits().begin();
	RenderUnit->SetTextureToSRV(_SRV);
}

void PostProcess::Init()
{
	PostProcessRenderer = std::make_shared<ScreenRenderer>();
}

void PostProcess::Render(float _DeltaTime)
{
	PostProcessRenderer->Render(_DeltaTime);

	ID3D11ShaderResourceView* SRV = NULL;
	EngineBase::GetInstance().GetContext()->PSSetShaderResources(0, 1, &SRV);
}
