#pragma once

#include "Asteroids.h"

class Game
{
public:

public:
	Game() : m_MenuState() {}
	~Game() {}

	void Init(const Arcane::Shared<Arcane::SoundLibrary>& soundLib);
	void StartGame(GameState& state, Arcane::Shared<Arcane::OrthoCameraController>&);
	void Dispose();

	void OnUpdate(Arcane::Timestep ts, Arcane::Shared<Arcane::OrthoCameraController>& camControl, GameState& gameState, MenuState& menuState);
	void OnEvent(Arcane::Event& e);

	void OnRender(Arcane::Timestep ts, const Arcane::Shared<Arcane::OrthoCameraController>& camControl, const GameState& state);
	void OnImGuiRender(ImGuiWindowFlags flags, const GameState& state);

private:
	bool OnKeyPressed(Arcane::KeyPressedEvent& e);

private:
	std::unordered_map<std::string, Arcane::Shared<Arcane::Texture2D>> m_TextureMap;
	std::unordered_map<std::string, ALuint> m_SoundMap;

	Arcane::Unique<SpaceGame::Gameplay> m_Asteroids;

	MenuState m_MenuState;
	GameState m_CurrentGame;
	bool m_MenuSet = false;
};