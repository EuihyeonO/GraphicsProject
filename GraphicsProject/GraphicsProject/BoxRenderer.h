#pragma once
#include "RenderBase.h"

class BoxRenderer : public RenderBase
{

public:
	
	BoxRenderer();
	~BoxRenderer();

	BoxRenderer(const BoxRenderer& _Other) = delete;
	BoxRenderer(BoxRenderer&& _Other) noexcept = delete;
	BoxRenderer& operator=(const BoxRenderer& _Other) = delete;
	BoxRenderer& operator=(BoxRenderer&& _Other) noexcept = delete;

public:
	virtual void Render() override{}
	virtual void Init() override;

protected:

private:
	void CreateVertexAndIndex();
};

