#include "GameLogic.h"

void Game::Init(const Arcane::Unique<Arcane::SoundLibrary>& soundLib)
{
	m_SndLib = soundLib.get();

	m_SoundMap["laser"] = m_SndLib->Load("assets/sound/laserShoot.wav");

	m_TextureMap["bb_blocks"] = Arcane::Texture2D::Create("assets/textures/brickbreak-clone/blocks.png");
	m_TextureMap["bb_sprites"] = Arcane::Texture2D::Create("assets/textures/brickbreak-clone/sprites.png");

	m_TextureMap["astr_ship"] = Arcane::Texture2D::Create("assets/textures/asteroids-clone/ship.png");
	m_TextureMap["astr_alien_ship"] = Arcane::Texture2D::Create("assets/textures/asteroids-clone/enemy_ships.png");

	m_LaserSource = new Arcane::SoundSource();
}

void Game::Dispose()
{
	m_TextureMap.clear();
}

void Game::OnUpdate(Arcane::Timestep ts, const Arcane::OrthoCameraController& camControl, const Game::State& state)
{
	switch (state)
	{
	case Asteroids: 
	{
		if (Arcane::Input::IsKeyPressed(Arcane::Key::Space) && !m_LaserSource->IsPlaying())
			m_LaserSource->Play(m_SoundMap["laser"]);

		break;
	}
	case PacMan: break;
	case BrickBreak: break;
	}
}

void Game::OnEvent(Arcane::Event& e, const Game::State& state)
{
	switch (state)
	{
	case Asteroids: break;
	case PacMan: break;
	case BrickBreak: break;
	}
}

void Game::OnRender(Arcane::Timestep ts, const Arcane::OrthoCameraController& camControl, const Game::State& state)
{
	switch (state)
	{
	case Asteroids: break;
	case PacMan: break;
	case BrickBreak: break;
	}
}

void Game::OnImGuiRender(ImGuiWindowFlags flags, const Game::State& state)
{
	switch (state)
	{
	case Asteroids: break;
	case PacMan: break;
	case BrickBreak: break;
	}
}
