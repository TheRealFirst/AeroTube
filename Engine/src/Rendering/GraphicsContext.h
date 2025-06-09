#pragma once

class GraphicsContext
{
public:
	virtual ~GraphicsContext() = default;

	virtual void Init() = 0;
	virtual void SwapBuffers() = 0;

	static Engine::Scope<GraphicsContext> Create(void* window);
};