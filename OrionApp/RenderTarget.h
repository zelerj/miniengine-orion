#pragma once
#include "RenderDriver.h"

class RenderTarget
{
public:
	RenderTarget();
	virtual ~RenderTarget();

	virtual void Initialize(RenderDriver* driver, int Width, int Height, int Depth) = 0;
	virtual void SetActive(RenderDriver* driver) = 0;
	virtual void Clear(RenderDriver* driver, Color col) = 0;

private:
	RenderDriver* m_pDriver;

	int m_width;
	int m_height;
};

