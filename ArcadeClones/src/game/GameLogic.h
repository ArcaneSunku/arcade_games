#pragma once

#include <Arcane.h>

#include <imgui.h>


enum MenuState : uint32_t
{
	NotActive = 0,
	Main, GameSelect, About
};

class Game
{
public:
	enum State : uint32_t
	{
		Menu = 0,
		Asteroids,
		PacMan,
		BrickBreak
	};

public:
	Game() : m_MenuState(), m_LaserSource(nullptr) {}
	~Game() {}

	void Init(const Arcane::Shared<Arcane::SoundLibrary>& soundLib);
	void Dispose();

	void OnUpdate(Arcane::Timestep ts, Arcane::OrthoCameraController& camControl, Game::State& gameState, MenuState& menuState);
	void OnEvent(Arcane::Event& e);

	void OnRender(Arcane::Timestep ts, const Arcane::OrthoCameraController& camControl, const Game::State& state);
	void OnImGuiRender(ImGuiWindowFlags flags, const Game::State& state);

private:
	bool OnKeyPressed(Arcane::KeyPressedEvent& e);

private:
	std::unordered_map<std::string, Arcane::Shared<Arcane::Texture2D>> m_TextureMap;
	std::unordered_map<std::string, ALuint> m_SoundMap;

	Arcane::SoundSource* m_LaserSource;
	MenuState m_MenuState;
	bool m_MenuSet = false;
};