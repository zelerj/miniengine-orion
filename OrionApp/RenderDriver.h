#pragma once

#include "Utilities.h"

class RenderTarget;
class Drawcall;

class RenderDriver
{
public:
	RenderDriver();
	~RenderDriver();

	virtual void InitializeDriver(int* Context, int Width, int Height, int TargetRefreshRate) = 0;
	virtual RenderTarget* CreateRenderTarget(int Width, int Height, int Depth) = 0;

	virtual void RenderFrame() = 0;
	virtual int* GetDriver() = 0;

	RenderTarget* GetRenderTarget(int index) { return m_renderTargets[index]; }
	
protected:
	std::vector<RenderTarget*> m_renderTargets;
	std::list<Drawcall*> m_drawcalls;
};

