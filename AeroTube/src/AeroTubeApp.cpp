#include <Engine.h>

class AeroTube : public Engine::Application 
{
public:
	AeroTube() : Application(){
	}

	~AeroTube() {
	}
};

Engine::Application* Engine::CreateApplication() {
	return new AeroTube();
}