#include "GameLogic.h"

void Game::Init(const Arcane::Shared<Arcane::SoundLibrary>& soundLib)
{
	m_SoundMap["laser"] = soundLib->Load("assets/sound/laserShoot.wav");

	m_TextureMap["bb_blocks"] = Arcane::Texture2D::Create("assets/textures/brickbreak-clone/blocks.png");
	m_TextureMap["bb_sprites"] = Arcane::Texture2D::Create("assets/textures/brickbreak-clone/sprites.png");

	m_TextureMap["astr_ships"] = Arcane::Texture2D::Create("assets/textures/asteroids-clone/ships.png");
	m_TextureMap["astr_rocks"] = Arcane::Texture2D::Create("assets/textures/asteroids-clone/rocks.png");
}

void Game::StartGame(GameState& state, Arcane::Shared<Arcane::OrthoCameraController>& camController)
{
	switch (state)
	{
	case GameState::Asteroids:
		m_Asteroids = Arcane::CreateUnique<SpaceGame::Gameplay>(camController, m_TextureMap, m_SoundMap);
		break;
	case GameState::PacMan: break;
	case GameState::BrickBreak: break;
	case GameState::Menu:
		camController->SetZoom(1.0f); 
		break;
	default: break;
	}
}

void Game::Dispose()
{
	m_TextureMap.clear();
	m_SoundMap.clear();
}

void Game::OnUpdate(Arcane::Timestep ts, Arcane::Shared<Arcane::OrthoCameraController>& camControl, GameState& state, MenuState& menuState)
{
	m_CurrentGame = state;
	if (state == GameState::Menu) return;
	if (m_MenuSet && m_MenuState != MenuState::NotActive)
	{
		menuState = m_MenuState;
		state = GameState::Menu;
		m_MenuSet = false;
		return;
	}

	switch (state)
	{
	case GameState::Asteroids:
	{
		m_Asteroids->Update(ts, camControl);
		break;
	}
	case GameState::PacMan: break;
	case GameState::BrickBreak: break;
	}
}

void Game::OnEvent(Arcane::Event& e)
{
	Arcane::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Arcane::KeyPressedEvent>(ARC_BIND_EVENT_FN(Game::OnKeyPressed));

	switch (m_CurrentGame)
	{
	case GameState::Asteroids:
	{
		m_Asteroids->Events(e);
		break;
	}
	case GameState::PacMan: break;
	case GameState::BrickBreak: break;
	default: break;
	}
}

void Game::OnRender(Arcane::Timestep ts, const Arcane::Shared<Arcane::OrthoCameraController>& camControl, const GameState& state)
{
	switch (state)
	{
	case GameState::Asteroids:
	{
		m_Asteroids->Render(ts, camControl);
		break;
	}
	case GameState::PacMan: break;
	case GameState::BrickBreak: break;
	}
}

void Game::OnImGuiRender(ImGuiWindowFlags flags, const GameState& state)
{
	switch (state)
	{
	case GameState::Asteroids:
	{
		m_Asteroids->RenderUI(flags, m_MenuState, m_MenuSet);
		break;
	}
	case GameState::PacMan: break;
	case GameState::BrickBreak: break;
	}
}

bool Game::OnKeyPressed(Arcane::KeyPressedEvent& e)
{

	return false;
}
