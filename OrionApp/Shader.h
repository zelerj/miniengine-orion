#pragma once
#include "RenderDriver.h"
#include "Drawcall.h"

enum ShaderType
{
	Screen,
	MeshFull
};

class Shader
{
public:
	Shader();
	virtual ~Shader();

	virtual void Load(RenderDriver* dev, ShaderType type, std::string VertexShader, std::string FragmentShader) = 0;
	virtual void PreparePass(RenderDriver* dev, Drawcall* call) = 0;
	virtual void Pass(RenderDriver* dev, Drawcall* call) = 0;
};

