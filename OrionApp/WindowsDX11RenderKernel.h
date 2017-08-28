#pragma once
#include "RenderDriver.h"
#include "RenderTarget.h"

#include <d3d11.h>
#include <DXGI1_3.h>

class WindowsDX11RenderKernel :
	public RenderDriver
{
public:
	WindowsDX11RenderKernel();
	~WindowsDX11RenderKernel();

	void InitializeDriver(int* Context, int Width, int Height, int TargetRefreshRate) override;
	RenderTarget* CreateRenderTarget(int Width, int Height, int Depth) override;

	void RenderFrame() override;
	
	/* Query Drivers Interfaces */
	int* GetDriver() override { return (int*)m_device; }

	ID3D11DeviceContext* GetImmediateContext() { return m_mainThreadCtx; }
	IDXGISwapChain* GetDXGISwapChain() { return m_dxgiSwapChain; }

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_mainThreadCtx;
	IDXGISwapChain* m_dxgiSwapChain;
	D3D_FEATURE_LEVEL m_dxFeatureLevel;
};

