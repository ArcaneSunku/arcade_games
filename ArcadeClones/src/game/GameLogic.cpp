#include "GameLogic.h"

void Game::Init(const Arcane::Shared<Arcane::SoundLibrary>& soundLib)
{
	m_SoundMap["laser"] = soundLib->Load("assets/sound/laserShoot.wav");

	m_TextureMap["bb_blocks"] = Arcane::Texture2D::Create("assets/textures/brickbreak-clone/blocks.png");
	m_TextureMap["bb_sprites"] = Arcane::Texture2D::Create("assets/textures/brickbreak-clone/sprites.png");

	m_TextureMap["astr_ship"] = Arcane::Texture2D::Create("assets/textures/asteroids-clone/ship.png");
	m_TextureMap["astr_alien_ship"] = Arcane::Texture2D::Create("assets/textures/asteroids-clone/enemy_ships.png");

	m_LaserSource = new Arcane::SoundSource();
}

void Game::Dispose()
{
	m_TextureMap.clear();
	m_SoundMap.clear();

	delete m_LaserSource;
}

void Game::OnUpdate(Arcane::Timestep ts, Arcane::OrthoCameraController& camControl, Game::State& state, MenuState& menuState)
{
	if (state == Game::State::Menu) return;
	camControl.OnUpdate(ts);

	if (m_MenuSet && m_MenuState != MenuState::NotActive)
	{
		menuState = m_MenuState;
		state = Game::State::Menu;
		m_MenuSet = false;
		return;
	}

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

void Game::OnEvent(Arcane::Event& e)
{
	Arcane::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Arcane::KeyPressedEvent>(ARC_BIND_EVENT_FN(Game::OnKeyPressed));
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

bool Game::OnKeyPressed(Arcane::KeyPressedEvent& e)
{
	if (e.GetKeyCode() == Arcane::Key::Escape)
	{
		// TODO: Handle this elsewhere and in a different manner
		m_MenuState = MenuState::GameSelect;
		m_MenuSet = true;
	}

	return false;
}
