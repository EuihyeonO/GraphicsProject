#pragma once
#include "Renderer.h"

class ScreenRenderer : public Renderer
{

public:

	ScreenRenderer();
	~ScreenRenderer();

	ScreenRenderer(const ScreenRenderer& _Other) = delete;
	ScreenRenderer(ScreenRenderer&& _Other) noexcept = delete;
	ScreenRenderer& operator=(const ScreenRenderer& _Other) = delete;
	ScreenRenderer& operator=(ScreenRenderer&& _Other) noexcept = delete;

	virtual void Init() override;

protected:
	void Update(float _DeltaTime);

private:
	void SetTransform();
};

