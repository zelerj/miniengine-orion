#include "stdafx.h"
#include "RenderingEngine.h"

RenderingEngine::RenderingEngine()
{
}

RenderingEngine::~RenderingEngine()
{
	delete m_renderer;
}

void RenderingEngine::InitializeRenderer(int* context, RenderDriverType Type, int Width, int Height, int Depth, int TargetRefreshRate)
{
	m_rendererType = Type;

	switch (m_rendererType)
	{
		case RenderDriverType::RENDERER_WIN_DX11: m_renderer = new WindowsDX11RenderKernel(); break;
	}

	m_renderer->InitializeDriver(context, Width, Height, TargetRefreshRate);
	auto rt = m_renderer->CreateRenderTarget(Width, Height, Depth);
}

void RenderingEngine::Update()
{

}

void RenderingEngine::Render()
{
	m_renderer->RenderFrame();
}