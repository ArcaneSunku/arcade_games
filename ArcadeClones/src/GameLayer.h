#pragma once

#include "game/GameLogic.h"

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

	Arcane::Unique<Arcane::SoundDevice> m_SndDevice;
	Arcane::Unique<Arcane::SoundLibrary> m_SndLib;

	ImFont* m_Font;
	Game m_Game;
};