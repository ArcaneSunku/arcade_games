#include <Arcane.h>
#include <Arcane/Core/EntryPoint.h>

#include "GameLayer.h"

class Playground : public Arcane::Application
{
public:
	Playground(const Arcane::WindowProps& props) :
		Application(props)
	{
		PushLayer(new GameLayer);
	}
};

Arcane::Application* Arcane::CreateApplication()
{
	return new Playground({"Playground App"});
}