#include "stdafx.h"
#include "WindowsDX11RenderKernel.h"
#include "Drawcall.h"
#include "MathInclude.h"

WindowsDX11RenderKernel::WindowsDX11RenderKernel()
{
	m_device = NULL;
	m_mainThreadCtx = NULL;
	m_dxgiSwapChain = NULL;
}

WindowsDX11RenderKernel::~WindowsDX11RenderKernel()
{
}

void WindowsDX11RenderKernel::InitializeDriver(int* Context, int Width, int Height, int TargetRefreshRate)
{
	HWND wnd = (HWND)Context;

	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,		
	};

	// This flag adds support for surfaces with a color-channel ordering different
	// from the API default. It is required for compatibility with Direct2D.
	UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(DEBUG) || defined(_DEBUG)
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
	desc.Windowed = TRUE; // Sets the initial state of full-screen mode.
	desc.BufferCount = 2;
	desc.BufferDesc.Width = Width;
	desc.BufferDesc.Height = Height;
	desc.BufferDesc.RefreshRate.Numerator = TargetRefreshRate;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // Non HDR Buffer, try switching to HDR
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SampleDesc.Count = 1;      //multisampling setting
	desc.SampleDesc.Quality = 0;    //vendor-specific flag
	desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	desc.Flags = 0;
	desc.OutputWindow = wnd;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,                    // Specify nullptr to use the default adapter.
		D3D_DRIVER_TYPE_HARDWARE,   // Create a device using the hardware graphics driver.
		0,                          // Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
		deviceFlags,                // Set debug and Direct2D compatibility flags.
		levels,                     // List of feature levels this app can support.
		ARRAYSIZE(levels),          // Size of the list above.
		D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Windows Store apps.
		&desc,
		&m_dxgiSwapChain,
		&m_device,                  // Returns the Direct3D device created.
		&m_dxFeatureLevel,          // Returns feature level of device created.
		&m_mainThreadCtx            // Returns the device immediate context.
	);

	if (FAILED(hr))
	{
		// Handle device interface creation failure if it occurs.
		// For example, reduce the feature level requirement, or fail over 
		// to WARP rendering.
		MessageBoxW(NULL, (LPCWSTR)L"Cannot initialize DirectX", (LPCWSTR)L"Error", 0x00000000L);
		assert(false);
	}
}

RenderTarget* WindowsDX11RenderKernel::CreateRenderTarget(int Width, int Height, int Depth)
{
	WindowsDX11RenderTarget* targ = new WindowsDX11RenderTarget();
	targ->Initialize(this, Width, Height, Depth);

	m_renderTargets.push_back(targ);

	// TEST DRAW CALL
	WindowsDX11Drawcall* dc = new WindowsDX11Drawcall();
	dc->Setup(this);

	_VertexDataScreen screenVertices[] =
	{
		{ Vector3(0.0f, 0.5f, 0.0f) },
		{ Vector3(0.45f, -0.5f, 0.0f) },
		{ Vector3(-0.45f, -0.5f, 0.0f) }
	};

	dc->CreateVBO(this, &screenVertices, ARRAYSIZE(screenVertices));
	m_drawcalls.push_back(dc);

	return targ;
}

void WindowsDX11RenderKernel::RenderFrame()
{
	m_renderTargets[0]->SetActive(this);
	m_renderTargets[0]->Clear(this, Color(0.0f, 0.1f, 0.2f, 0.0f));

	for (auto dc : m_drawcalls)
		dc->Pass(this);

	// First arg is VSync, second is options
	HRESULT hr = m_dxgiSwapChain->Present(0, 0);

#if defined(DEBUG) || defined(_DEBUG)
	DXGI_FRAME_STATISTICS m_counter;
	m_dxgiSwapChain->GetFrameStatistics(&m_counter);
#endif

	if (FAILED(hr))
	{
		MessageBoxW(NULL, (LPCWSTR)L"Cannot present device", (LPCWSTR)L"Error", 0x00000000L);
		assert(false);
	}
}