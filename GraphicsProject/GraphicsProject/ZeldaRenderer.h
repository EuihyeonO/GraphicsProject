#pragma once
#include "Renderer.h"

class ZeldaRenderer : public Renderer
{

public:

	ZeldaRenderer();
	~ZeldaRenderer();

	ZeldaRenderer(const ZeldaRenderer& _Other) = delete;
	ZeldaRenderer(ZeldaRenderer&& _Other) noexcept = delete;
	ZeldaRenderer& operator=(const ZeldaRenderer& _Other) = delete;
	ZeldaRenderer& operator=(ZeldaRenderer&& _Other) noexcept = delete;

protected:
	virtual void Init() override;
	virtual void Update(float _DeltaTime) override;
private:
};

