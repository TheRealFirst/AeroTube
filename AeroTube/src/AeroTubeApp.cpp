#include <Engine.h>

class AeroTube : public Engine::Application 
{
public:
	AeroTube() {
		LOG_INFO("Started up the Engine.")
	}

	~AeroTube() {
		LOG_INFO("Shutting the Engine down.")
	}
};

Engine::Application* Engine::CreateApplication() {
	return new AeroTube();
}