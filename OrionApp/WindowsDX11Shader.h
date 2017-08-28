#pragma once
#include "Shader.h"

class Drawcall;

class WindowsDX11Shader : Shader
{
public:
	WindowsDX11Shader();
	~WindowsDX11Shader();

	void Load(RenderDriver* dev, ShaderType type, std::string Vertex, std::string Fragment) override;
	void PreparePass(RenderDriver* dev, Drawcall* call) override;
	void Pass(RenderDriver* dev, Drawcall* call) override;

	unsigned int GetVerticesStride() { return m_vertexLayoutStride; }

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_fragmentShader;

	std::vector<byte> m_vertexBytecode;
	std::vector<byte> m_fragmenetBytecode;

	ID3D11InputLayout* m_vertexLayout;
	unsigned int m_vertexLayoutStride;
};

