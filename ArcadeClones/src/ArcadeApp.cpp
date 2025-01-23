#include <Arcane.h>
#include <Arcane/Core/EntryPoint.h>

#include "GameLayer.h"

class Arcade : public Arcane::Application
{
public:
	Arcade(const Arcane::WindowProps& props) :
		Application(props)
	{
		PushLayer(new GameLayer);
	}
};

Arcane::Application* Arcane::CreateApplication()
{
	return new Arcade({"Arcade Classics"});
}