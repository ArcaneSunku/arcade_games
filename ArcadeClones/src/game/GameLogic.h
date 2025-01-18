#pragma once

#include <Arcane.h>

#include <imgui.h>

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
	Game() : m_SndLib(nullptr) {}
	~Game() {}

	void Init(const Arcane::Unique<Arcane::SoundLibrary>& soundLib);
	void Dispose();

	void OnUpdate(Arcane::Timestep ts, const Arcane::OrthoCameraController& camControl, const Game::State& state);
	void OnEvent(Arcane::Event& e, const Game::State& state);

	void OnRender(Arcane::Timestep ts, const Arcane::OrthoCameraController& camControl, const Game::State& state);
	void OnImGuiRender(ImGuiWindowFlags flags, const Game::State& state);

private:
	Arcane::SoundLibrary* m_SndLib;
	std::unordered_map<std::string, Arcane::Shared<Arcane::Texture2D>> m_TextureMap;
	std::unordered_map<std::string, uint32_t> m_SoundMap;

	Arcane::SoundSource* m_LaserSource;
};