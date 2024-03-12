#include <Engine.h>

class AeroTube : public Engine::Application 
{
public:
	AeroTube() {
	}

	~AeroTube() {
	}
};

Engine::Application* Engine::CreateApplication() {
	return new AeroTube();
}