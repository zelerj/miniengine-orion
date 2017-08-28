#pragma once

class Shader;

typedef struct _ProjectionDatas
{
	Matrix4x4 Model;
	Matrix4x4 View;
	Matrix4x4 Projection;
} CBuffer_ProjectionData;

struct _VertexDataFull
{
	Vector3 Position;
	Vector3 Normal;
	Vector3 Tangent;
	Vector3 VertexColor;
} CBuffer_MeshDataFull;

struct _VertexDataScreen
{
	Vector3 Position;
} CBuffer_MeshDataScreen;

class Drawcall
{
public:
	Drawcall();
	virtual ~Drawcall();

	virtual void Setup(RenderDriver* driver) = 0;
	virtual void Pass(RenderDriver* driver) = 0;
	virtual void CreateVBO(RenderDriver* driver, void* VBData, unsigned long VBSize) = 0;

	// Accessors
	virtual void* GetVBO() = 0;
	unsigned long GetVerticesCount() { return m_verticesCount; }

protected:
	Shader* m_shader;
	unsigned long m_verticesCount;
};

