#pragma once

#ifdef PLATFORM_WINDOWS

extern Engine::Application* Engine::CreateApplication();

int main(int argc, char** argv) 
{
	auto app = Engine::CreateApplication();
	app->Run();
	delete app;
}

#else
	#error The engine only supports Windows
#endif