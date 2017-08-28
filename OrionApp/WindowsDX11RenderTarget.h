#pragma once
#include "RenderTarget.h"
class WindowsDX11RenderTarget :
	public RenderTarget
{
public:
	WindowsDX11RenderTarget();
	virtual ~WindowsDX11RenderTarget();

	void Initialize(RenderDriver* driver, int Width, int Height, int Depth) override;
	void SetActive(RenderDriver* driver) override;
	void Clear(RenderDriver* driver, Color col) override;

private:
	ID3D11RenderTargetView* m_renderTarget;
	ID3D11DepthStencilView* m_depthStencilView;

	ID3D11Texture2D* m_backBuffer;
	ID3D11Texture2D* m_depthStencil;

	D3D11_TEXTURE2D_DESC* m_backbufferDesc;
	CD3D11_DEPTH_STENCIL_VIEW_DESC* m_depthStencilDesc;
	D3D11_VIEWPORT* m_viewPort;
};

