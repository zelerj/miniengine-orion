#pragma once

#include "RenderDriver.h"
#include "RenderTarget.h"

enum RenderDriverType
{
	RENDERER_WIN_DX11,
	RENDERER_WIN_DX12,
	RENDERER_WIN_GL,
	RENDERER_WIN_VK
};

class RenderingEngine
{
public:
	RenderingEngine();
	~RenderingEngine();

	void Update();
	void Render();

	void InitializeRenderer(int* context, RenderDriverType Type, int Width, int Height, int Depth, int TargetRefreshRate);

private:
	RenderDriver* m_renderer;
	RenderDriverType m_rendererType;
};

