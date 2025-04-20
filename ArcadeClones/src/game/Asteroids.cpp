#include "Asteroids.h"

#include <random>

namespace SpaceGame
{
	bool player_alive = false;
	int timer = 0;

	Gameplay::Gameplay(Arcane::Shared<Arcane::OrthoCameraController>& camController, 
		std::unordered_map<std::string, Arcane::Shared<Arcane::Texture2D>>& textureMap,
		std::unordered_map<std::string, ALuint>& soundMap) :
		m_Score(0), m_Lives(3), m_Level(1), m_Started(false), m_Paused(false)
	{
		camController->SetZoom(2.0f);
		m_Textures["ships"] = textureMap["astr_ships"];
		m_Textures["rocks"] = textureMap["astr_rocks"];

		m_Ships.push_back(Arcane::CreateShared<Ship>(m_Textures["ships"], true));
		player_alive = true;

		m_Stars.reserve(25 * 25);

		Arcane::Shared<Asteroid> rock = Arcane::CreateShared<Asteroid>(m_Textures["rocks"], 1, 2);
		rock->SetVelocity({ -0.15f, -0.25f });
		rock->SetPosition({ 1, 1 });

		m_Asteroids.push_back(rock);

		std::random_device dev;
		std::mt19937_64 rng(dev());

		for (auto x = 0; x < 25; x++)
		{
			for (auto y = 0; y < 25; y++)
			{
				std::uniform_real_distribution<float> sizeDist(0.005f, 0.0075f);
				std::uniform_real_distribution<float> rotDist(0.0f, 360.0f);
				std::uniform_real_distribution<float> postDist(-camController->GetAspectRatio() * camController->GetZoom() * 100.0f, camController->GetAspectRatio() * camController->GetZoom() * 100.0f);

				m_Stars.push_back(Arcane::CreateShared<Star>(glm::vec2((x + postDist(rng)) * sizeDist(rng) * 2, (y + postDist(rng)) * sizeDist(rng) * 2), glm::vec2(sizeDist(rng), sizeDist(rng)), rotDist(rng)));
			}
		}
	}

	Gameplay::~Gameplay()
	{
		m_Ships.clear();
		m_Asteroids.clear();

		m_Stars.clear();
		m_ActiveBullets.clear();
	}

	void Gameplay::Update(Arcane::Timestep ts, Arcane::Shared<Arcane::OrthoCameraController>& camControl)
	{
		if (camControl->CanMove()) camControl->SetCanMove(false);
		camControl->OnUpdate(ts);

		if (!player_alive)
			timer = 0;

		timer++;
		if(timer % 60 == 0)
		{
			timer = 0;
			m_Score++;
		}

		if(!player_alive)
		{
			if (Arcane::Input::IsKeyPressed(Arcane::Key::Space))
			{
				if (m_Lives > 0)
				{
					m_Ships.push_back(Arcane::CreateShared<Ship>(m_Textures["ships"], true));
					player_alive = true;
				}
			}
		}

		for (std::vector<Arcane::Shared<Asteroid>>::iterator itr = m_Asteroids.begin(); itr != m_Asteroids.end(); ++itr)
		{
			if (!itr->get()->IsAlive()) {
				m_Asteroids.erase(itr);
				return;
			}

			itr->get()->Update(ts, camControl);
		}

		for (std::vector<Arcane::Shared<Ship>>::iterator itr = m_Ships.begin(); itr != m_Ships.end(); ++itr) 
		{
			if (!itr->get()->IsAlive())
			{
				bool player = itr->get()->IsPlayer();
				m_Ships.erase(itr);
				m_Ships.shrink_to_fit();

				if (player)
				{
					m_Lives--;

					if (m_Lives <= 0)
					{
						m_Score = 0;
						m_Lives = 3;
					}

					player_alive = false;
				}

				return;
			}

			itr->get()->Update(ts, camControl);
		}

		CheckCollisions();
	}

	void Gameplay::Events(Arcane::Event& event)
	{

	}

	void Gameplay::Render(Arcane::Timestep ts, const Arcane::Shared<Arcane::OrthoCameraController>& camControl)
	{
		for (auto it = m_Stars.begin(); it != m_Stars.end(); ++it)
			(*it)->Render(ts, camControl);

		for (std::vector<Arcane::Shared<Asteroid>>::iterator itr = m_Asteroids.begin(); itr != m_Asteroids.end(); ++itr)
		{
			if (!itr->get()->IsAlive())
				continue;

			itr->get()->Render(ts, camControl);
		}

		for (std::vector<Arcane::Shared<Ship>>::iterator itr = m_Ships.begin(); itr != m_Ships.end(); ++itr) {
			if (!itr->get()->IsAlive())
				continue;

			itr->get()->Render(ts, camControl);
		}
	}

	void Gameplay::RenderUI(ImGuiWindowFlags flags, MenuState& menuState, bool& setMenu)
	{
		ImVec2 winPos = ImGui::GetWindowPos();

		ImGui::SetNextWindowPos({ winPos.x - 40, winPos.y - 40 });
		ImGui::SetNextWindowSize({ 150, 75 });

		ImGuiWindowFlags scoreFlags = flags;
		ImGui::Begin("PlayerInfo", (bool*)0, scoreFlags | ImGuiWindowFlags_NoBringToFrontOnFocus);
		for (uint32_t i = 0; i < m_Lives; i++)
		{
			ImGui::SetCursorPos({ 32 * (float)i, 0});
			ImGui::Image(m_Textures["ships"]->GetRendererID(), { 32, 32 }, { 16.0f / 32.0f, 1 }, { 0, 16.0f / 32.0f });
			if (i < m_Lives - 1 && m_Lives != 0) ImGui::SameLine();
		}
		ImGui::Text("Score: %d", GetScore());
		ImGui::End();
	}

	void Gameplay::CheckCollisions()
	{
		for (auto ship : m_Ships)
		{
			for (auto other : m_Ships) {
				if (ship == other) continue;

				if (ship->Collides((TransformData&)other->GetTransform()))
				{
					ARC_INFO("Collision!");
					if (ship->IsPlayer()) 
					{
						ship->SetAlive(false);
						continue;
					}
					else if (other->IsPlayer())
					{
						other->SetAlive(false);
						continue;
					}
				}
			}

			for (auto asteroid : m_Asteroids)
			{
				if (ship->Collides((TransformData&)asteroid->GetTransform()))
				{
					ARC_INFO("Collision!");
					ship->SetAlive(false);
					return;
				}
			}
		}
	}
}