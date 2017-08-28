#pragma once
#include "Drawcall.h"

class WindowsDX11Drawcall :
	public Drawcall
{
public:
	WindowsDX11Drawcall();
	~WindowsDX11Drawcall();

	void Setup(RenderDriver* driver) override;
	void Pass(RenderDriver* driver) override;

	void CreateVBO(RenderDriver* driver, void* VBData, unsigned long VBSize) override;

	// Accessors
	void* GetVBO() override { return m_vbo; }

private:
	ID3D11Buffer* m_vbo;
};

