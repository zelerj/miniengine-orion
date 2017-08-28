#include "stdafx.h"
#include "RenderDriver.h"

RenderDriver::RenderDriver()
{
}

RenderDriver::~RenderDriver()
{
	// Clear render targets
	for (auto it : m_renderTargets)
		delete it;
}
