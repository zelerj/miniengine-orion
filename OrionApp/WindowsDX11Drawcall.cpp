#include "stdafx.h"
#include "WindowsDX11Drawcall.h"

WindowsDX11Drawcall::WindowsDX11Drawcall()
{
	m_vbo = NULL;
}

WindowsDX11Drawcall::~WindowsDX11Drawcall()
{
	if (m_vbo != NULL)
	{
		m_vbo->Release();
		delete m_vbo;
		m_vbo = NULL;
	}
}

void WindowsDX11Drawcall::Setup(RenderDriver* driver)
{
	auto shader = new WindowsDX11Shader();
	m_shader = (Shader*)shader;

	shader->Load(driver, ShaderType::Screen, "ScreenVertex", "DebugTest");
}

void WindowsDX11Drawcall::Pass(RenderDriver* driver)
{
	m_shader->PreparePass(driver, this);
	m_shader->Pass(driver, this);
}

void WindowsDX11Drawcall::CreateVBO(RenderDriver* driver, void* VBData, unsigned long VBSize)
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	unsigned long dataLength = VBSize * (unsigned long)((WindowsDX11Shader*)m_shader)->GetVerticesStride();

	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = dataLength;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ID3D11Device* dev = (ID3D11Device*)driver->GetDriver();

	HRESULT hr = dev->CreateBuffer(&desc, NULL, &m_vbo);

	if (FAILED(hr))
	{
		MessageBoxW(NULL, (LPCWSTR)L"Cannot create VBO object", (LPCWSTR)L"Error", 0x00000000L);
		assert(false);
	}

	// Transfer our VBO to the newly created buffer
	D3D11_MAPPED_SUBRESOURCE sub;
	auto ctx = ((WindowsDX11RenderKernel*)driver)->GetImmediateContext();

	hr = ctx->Map(m_vbo, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &sub);
	memcpy(sub.pData, VBData, dataLength);
	ctx->Unmap(m_vbo, NULL);

	if (FAILED(hr))
	{
		MessageBoxW(NULL, (LPCWSTR)L"Cannot map VBO to CPU", (LPCWSTR)L"Error", 0x00000000L);
		assert(false);
	}

	m_verticesCount = VBSize;
}