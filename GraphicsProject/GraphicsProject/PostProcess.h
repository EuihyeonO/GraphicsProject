#pragma once
#include "BaseHeader.h"

class PostProcess
{

public:

	PostProcess();
	~PostProcess();

	PostProcess(const PostProcess& _Other) = delete;
	PostProcess(PostProcess&& _Other) noexcept = delete;
	PostProcess& operator=(const PostProcess& _Other) = delete;
	PostProcess& operator=(PostProcess&& _Other) noexcept = delete;

	void SetTexture(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _SRV);

	virtual void Init();
	virtual void Render(float _DeltaTime);

protected:
	std::shared_ptr<class ScreenRenderer> PostProcessRenderer;

private:

};

