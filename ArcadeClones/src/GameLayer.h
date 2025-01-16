#pragma once

#include <Arcane.h>

#include <imgui.h>

class GameLayer : public Arcane::Layer
{
public:
	GameLayer();
	virtual ~GameLayer() override;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnImGuiRender() override;

	virtual void OnUpdate(Arcane::Timestep ts) override;
	virtual void OnEvent(Arcane::Event& event) override;

private:
	void MainMenu(ImGuiWindowFlags flags = 0);
	void GameSelect(ImGuiWindowFlags flags = 0);
	void About(ImGuiWindowFlags flags = 0);

private:
	Arcane::OrthoCameraController* m_CamController;
	std::unordered_map<std::string, Arcane::Shared<Arcane::Texture2D>> m_TextureMap;

	ImFont* m_Font;
};