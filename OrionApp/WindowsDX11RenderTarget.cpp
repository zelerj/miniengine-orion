#include "stdafx.h"
#include "WindowsDX11RenderTarget.h"

WindowsDX11RenderTarget::WindowsDX11RenderTarget()
{
	m_backBuffer = NULL;
	m_backbufferDesc = new D3D11_TEXTURE2D_DESC();
	m_depthStencilDesc = NULL;
	m_renderTarget = NULL;
	m_depthStencil = NULL;
	m_backBuffer = NULL;
	m_viewPort = NULL;
	m_renderTarget = NULL;
	m_depthStencilView = NULL;
}

WindowsDX11RenderTarget::~WindowsDX11RenderTarget()
{
	if (m_backbufferDesc != NULL)
		delete m_backbufferDesc;

	if (m_depthStencilDesc != NULL)
		delete m_depthStencilDesc;

	if (m_viewPort != NULL)
		delete m_viewPort;

	if (m_backBuffer != NULL)
	{
		m_backBuffer->Release();
		delete m_backBuffer;
		m_backBuffer = NULL;
	}

	if (m_renderTarget != NULL)
	{
		m_renderTarget->Release();
		delete m_renderTarget;
		m_renderTarget = NULL;
	}

	if (m_depthStencil != NULL)
	{
		m_depthStencil->Release();
		delete m_depthStencil;
		m_depthStencil = NULL;
	}

	if (m_depthStencilView != NULL)
	{
		m_depthStencilView->Release();
		delete m_depthStencilView;
		m_depthStencilView = NULL;
	}
}

void WindowsDX11RenderTarget::Initialize(RenderDriver* driver, int Width, int Height, int Depth)
{
	WindowsDX11RenderKernel* localDriver = (WindowsDX11RenderKernel*)driver;
	ID3D11Device* dev = (ID3D11Device*)driver->GetDriver();

	HRESULT hr = localDriver->GetDXGISwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_backBuffer);

	if (FAILED(hr))
	{
		MessageBoxW(NULL, (LPCWSTR)L"Cannot get Backbuffer", (LPCWSTR)L"Error", 0x00000000L);
		assert(false);
	}

	hr = dev->CreateRenderTargetView(m_backBuffer, nullptr, &m_renderTarget);

	if (FAILED(hr))
	{
		MessageBoxW(NULL, (LPCWSTR)L"Cannot create RenderTarget", (LPCWSTR)L"Error", 0x00000000L);
		assert(false);
	}

	m_backBuffer->GetDesc(m_backbufferDesc);

	CD3D11_TEXTURE2D_DESC depthStencilDesc(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		static_cast<UINT> (m_backbufferDesc->Width),
		static_cast<UINT> (m_backbufferDesc->Height),
		1, // This depth stencil view has only one texture.
		1, // Use a single mipmap level.
		D3D11_BIND_DEPTH_STENCIL
	);

	hr = dev->CreateTexture2D(&depthStencilDesc, nullptr, &m_depthStencil);

	if (FAILED(hr))
	{
		MessageBoxW(NULL, (LPCWSTR)L"Cannot create Depth Texture", (LPCWSTR)L"Error", 0x00000000L);
		assert(false);
	}

	m_depthStencilDesc = new CD3D11_DEPTH_STENCIL_VIEW_DESC(D3D11_DSV_DIMENSION_TEXTURE2D);

	hr = dev->CreateDepthStencilView(
		m_depthStencil,
		m_depthStencilDesc,
		&m_depthStencilView
	);

	if (FAILED(hr))
	{
		MessageBoxW(NULL, (LPCWSTR)L"Cannot create DepthTarget", (LPCWSTR)L"Error", 0x00000000L);
		assert(false);
	}

	m_viewPort = new D3D11_VIEWPORT();
	ZeroMemory(m_viewPort, sizeof(D3D11_VIEWPORT));

	m_viewPort->Width = (float)m_backbufferDesc->Width;
	m_viewPort->Height = (float)m_backbufferDesc->Height;
	m_viewPort->MinDepth = 0;
	m_viewPort->MaxDepth = 1;
}

void WindowsDX11RenderTarget::SetActive(RenderDriver* driver)
{
	WindowsDX11RenderKernel* dev = (WindowsDX11RenderKernel*)driver;
	dev->GetImmediateContext()->OMSetRenderTargets(1, &m_renderTarget, m_depthStencilView);
	dev->GetImmediateContext()->RSSetViewports(1, m_viewPort);
}

void WindowsDX11RenderTarget::Clear(RenderDriver* driver, Color col)
{
	ID3D11DeviceContext* ctx = ((WindowsDX11RenderKernel*)driver)->GetImmediateContext();
	ctx->ClearRenderTargetView(m_renderTarget, (FLOAT*)&col);
}