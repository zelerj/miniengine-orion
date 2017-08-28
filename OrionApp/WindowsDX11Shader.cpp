#include "stdafx.h"
#include "WindowsDX11Shader.h"

D3D11_INPUT_ELEMENT_DESC CBuffer_VertexDesc_Full[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

D3D11_INPUT_ELEMENT_DESC CBuffer_VertexDesc_Screen[] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

WindowsDX11Shader::WindowsDX11Shader()
{
	m_vertexShader = NULL;
	m_fragmentShader = NULL;
}

WindowsDX11Shader::~WindowsDX11Shader()
{
	if (m_vertexShader)
	{
		m_vertexShader->Release();
		delete m_vertexShader;
		m_vertexShader = NULL;
	}

	if (m_fragmentShader)
	{
		m_fragmentShader->Release();
		delete m_fragmentShader;
		m_fragmentShader = NULL;
	}
}

void WindowsDX11Shader::Load(RenderDriver* dev, ShaderType type, std::string Vertex, std::string Fragment)
{
	ID3D11Device* localDev = (ID3D11Device*)dev->GetDriver();

	// Fetch the vertex shader
	std::stringstream pathBuild;
	pathBuild << "Shaders\\";
	pathBuild << Vertex;
	pathBuild << ".cso";

	auto VS = Utilities::ReadData(pathBuild.str().c_str());

	// Clear path buffer
	pathBuild.str("");
	pathBuild.clear();

	// Fetch the fragment shader
	pathBuild << "Shaders\\";
	pathBuild << Fragment;
	pathBuild << ".cso";

	auto PS = Utilities::ReadData(pathBuild.str().c_str());

	m_vertexBytecode = VS;
	m_fragmenetBytecode = PS;

	// Build the shaders from loaded bytecode
	HRESULT res = localDev->CreateVertexShader(VS.data(), VS.size(), NULL, &m_vertexShader);
	res = localDev->CreatePixelShader(PS.data(), PS.size(), NULL, &m_fragmentShader);

	WindowsDX11RenderKernel* localDriver = (WindowsDX11RenderKernel*)dev;
	
	// Select input layout depending on the needs
	switch (type)
	{
	case ShaderType::Screen:
		res = ((ID3D11Device*)(localDriver->GetDriver()))->CreateInputLayout(CBuffer_VertexDesc_Screen, ARRAYSIZE(CBuffer_VertexDesc_Screen), VS.data(), VS.size(), &m_vertexLayout);
		m_vertexLayoutStride = 12;//sizeof(CBuffer_VertexDesc_Screen);
		break;

	case ShaderType::MeshFull:
		res = ((ID3D11Device*)(localDriver->GetDriver()))->CreateInputLayout(CBuffer_VertexDesc_Full, ARRAYSIZE(CBuffer_VertexDesc_Full), VS.data(), VS.size(), &m_vertexLayout);
		m_vertexLayoutStride = 52;//sizeof(CBuffer_VertexDesc_Full);
		break;
	}
}

void WindowsDX11Shader::PreparePass(RenderDriver* dev, Drawcall* call)
{
	WindowsDX11RenderKernel* driver = (WindowsDX11RenderKernel*)dev;
	auto ctx = driver->GetImmediateContext();

	ctx->IASetInputLayout(m_vertexLayout);

	unsigned int offsets = 0;

	ID3D11Buffer* vbo = (ID3D11Buffer*)call->GetVBO();
	ctx->IASetVertexBuffers(0, 1, &vbo, &m_vertexLayoutStride, &offsets);

	ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ctx->VSSetShader(m_vertexShader, 0, 0);
	ctx->PSSetShader(m_fragmentShader, 0, 0);
}

void WindowsDX11Shader::Pass(RenderDriver* dev, Drawcall* call)
{
	WindowsDX11RenderKernel* driver = (WindowsDX11RenderKernel*)dev;
	auto ctx = driver->GetImmediateContext();

	unsigned long vertices = call->GetVerticesCount();
	ctx->Draw(vertices, 0);
}