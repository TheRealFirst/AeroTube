#include <Engine.h>
#include <Core/Entrypoint.h>

#include "AeroTubeLayer.h"

namespace Engine {
	class AeroTube : public Application
	{
	public:
		AeroTube() : Application() {
			PushLayer(new AeroTubeLayer());
		}

		~AeroTube() = default;
	};

	Application* CreateApplication() {
		return new AeroTube();
	}
}