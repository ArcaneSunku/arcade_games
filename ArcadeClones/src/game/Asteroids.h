#pragma once

#include "GenericData.h"

#include <imgui.h>

namespace SpaceGame
{
	class Star : public Entity
	{
	public:
		Star(const glm::vec2& pos = { 0.0f, 0.0f }, const glm::vec2& size = {1.0f, 1.0f}, float rotation = 0.0f) : Entity() 
		{
			m_Data.Transform.Position = pos;
			m_Data.Transform.Size = size;
			m_Data.Transform.Rotation = rotation;
		}

		virtual ~Star() override {}

		inline virtual void Render(Arcane::Timestep ts, const Arcane::Shared<Arcane::OrthoCameraController>& camControl)
		{
			Arcane::Renderer2D::DrawRotatedQuad(m_Data.Transform.Position, m_Data.Transform.Size, m_Data.Transform.Rotation, { 1.0f, 1.0f, 1.0f, 1.0f });
		}
	};

	class Bullet : public Entity
	{
	public:
		Bullet(glm::vec2 velocity = glm::vec2(0)) : Entity(), m_Velocity(velocity), m_Lifetime(5.0f) {}
		virtual ~Bullet() override {}

		inline virtual void Update(Arcane::Timestep ts, const Arcane::Shared<Arcane::OrthoCameraController>& camControl)
		{
			if (m_Data.Alive) return;

			m_Data.Transform.Position += m_Velocity;
			m_Lifetime -= ts * 1.5f;

			if (m_Lifetime <= 0.0f)
			{
				m_Data.Alive = false;
				return;
			}
		}

		inline virtual void Render(Arcane::Timestep ts, const Arcane::Shared<Arcane::OrthoCameraController>& camControl)
		{
			if (m_Data.Alive) return;

			Arcane::Renderer2D::DrawRotatedQuad(m_Data.Transform.Position, m_Data.Transform.Size, m_Data.Transform.Rotation, { 1.0f, 1.0f, 1.0f, 1.0f });
		}

	private:
		glm::vec2 m_Velocity;
		float m_Lifetime;

	};

	class Ship : public Entity
	{
	public:
		Ship() : Entity(), m_Texture(nullptr), m_Velocity(0), m_Player(false), m_Moving(false), m_Firing(false) {}

		Ship(Arcane::Shared<Arcane::Texture2D>& texture, bool player = false) :
			Entity(), m_Texture(texture), m_Velocity(0), m_Player(player), m_Moving(false), m_Firing(false)
		{
			SetSprite();
			m_Data.Transform.Size = { .16, .16 };
		}

		virtual ~Ship() override {}

		inline virtual void Update(Arcane::Timestep ts, Arcane::Shared<Arcane::OrthoCameraController>& camControl)
		{
			if (!m_Player)
			{
				// TODO: Implement Space ship logic
				return;
			}

			PlayerUpdate(ts, camControl);
		}

		inline virtual void Render(Arcane::Timestep ts, const Arcane::Shared<Arcane::OrthoCameraController>& camControl)
		{
			if (m_Moving)
			{
				m_Frame += ts * 15;

				if (m_Frame > 1.0f)
					m_Frame = 0.0f;
			}
			else
			{
				m_Frame = 0;
			}

			SetSprite((int) m_Frame);
			Arcane::Renderer2D::DrawRotatedQuad(m_Data.Transform.Position, m_Data.Transform.Size, m_Data.Transform.Rotation, m_Data.Sprite);
		}

		inline bool IsPlayer() const { return m_Player; }

	private:
		inline void PlayerUpdate(Arcane::Timestep ts, Arcane::Shared<Arcane::OrthoCameraController>& camControl)
		{
			if (Arcane::Input::IsKeyPressed(Arcane::Key::Escape))
			{
				m_Data.Alive = false;
				return;
			}

			bool rotLeft = Arcane::Input::IsKeyPressed(Arcane::Key::A);
			bool rotRight = Arcane::Input::IsKeyPressed(Arcane::Key::D);

			if (rotLeft)
			{
				m_Data.Transform.Rotation += ts * ROTATION_SPEED;
				m_Moving = true;
			}
			else if (rotRight)
			{
				m_Data.Transform.Rotation -= ts * ROTATION_SPEED;
				m_Moving = true;
			}

			float speed = ACCELERATION_FACTOR * ts;
			if (Arcane::Input::IsKeyPressed(Arcane::Key::W))
			{
				m_Velocity.x -= glm::sin(glm::radians(m_Data.Transform.Rotation)) * speed;
				m_Velocity.y += glm::cos(glm::radians(m_Data.Transform.Rotation)) * speed;

				m_Moving = true;
			}
			else if (Arcane::Input::IsKeyPressed(Arcane::Key::S))
			{
				m_Velocity.x += glm::sin(glm::radians(m_Data.Transform.Rotation)) * speed;
				m_Velocity.y -= glm::cos(glm::radians(m_Data.Transform.Rotation)) * speed;

				m_Moving = true;
			}
			else
			{
				if (!rotLeft && !rotRight)
					m_Moving = false;
			}

			m_Velocity *= glm::vec2(DRAG_FACTOR);

			if (glm::length(m_Velocity) > MAX_VELOCITY)
				m_Velocity = glm::normalize(m_Velocity) * MAX_VELOCITY;

			m_Data.Transform.Position += m_Velocity * glm::vec2(ts);

			if (m_Data.Transform.Rotation >= 360) m_Data.Transform.Rotation -= 360;
			if (m_Data.Transform.Rotation <= 0) m_Data.Transform.Rotation += 360;

			glm::vec2 camPos = camControl->GetPosition();
			float camWidth = camControl->GetAspectRatio() * camControl->GetZoom();
			float camHeight = camControl->GetZoom();

			if (m_Data.Transform.Position.x + m_Data.Transform.Size.x * 0.5f <= camPos.x - camWidth)
				m_Data.Transform.Position.x = camPos.x + camWidth + m_Data.Transform.Size.x * 0.5f;
			else if (m_Data.Transform.Position.x - m_Data.Transform.Size.x * 0.5f >= camPos.x + camWidth)
				m_Data.Transform.Position.x = camPos.x - camWidth - m_Data.Transform.Size.x * 0.5f;

			if (m_Data.Transform.Position.y + m_Data.Transform.Size.y * 0.5f <= camPos.y - camHeight)
				m_Data.Transform.Position.y = camPos.y + camHeight + m_Data.Transform.Size.y * 0.5f;
			else if (m_Data.Transform.Position.y - m_Data.Transform.Size.y * 0.5f >= camPos.y + camHeight)
				m_Data.Transform.Position.y = camPos.y - camHeight - m_Data.Transform.Size.y * 0.5f;
		}

		inline void SetSprite(int frame = 0) {
			if (m_Player) m_Data.Sprite = Arcane::SubTexture2D::CreateFromCoords(m_Texture, { frame, 1 }, { 16, 16 });
			else m_Data.Sprite = Arcane::SubTexture2D::CreateFromCoords(m_Texture, { frame, 0 }, { 16, 16 });
		}

		inline void SetPosition(glm::vec2& position) {
			m_Data.Transform.Position = position;
		}

		inline void SetVelocity(glm::vec2& velocity) {
			m_Velocity = velocity;
		}

	private:
		Arcane::Shared<Arcane::Texture2D> m_Texture;
		glm::vec2 m_Velocity;
		bool m_Player;
		bool m_Moving, m_Firing;
		float m_Frame = 0;

		const float MAX_VELOCITY = 1.5f;
		const float DRAG_FACTOR  = 0.9875f;
		const float ROTATION_SPEED = 175.0f;
		const float ACCELERATION_FACTOR = 2.5f;
	};

	class Asteroid : public Entity
	{
	public:
		Asteroid(Arcane::Shared<Arcane::Texture2D>& texture, int size = 3, int varient = 0) : 
			Entity(), m_Texture(texture), m_Size(size), m_Varient(varient), m_Velocity(0)
		{
			if (varient > 2) varient = 2;
			m_Data.Sprite = Arcane::SubTexture2D::CreateFromCoords(texture, { varient, 0 }, { 32, 32 });
			m_Data.Transform.Size = glm::vec2(.4 * size, .4 * size);
		}

		virtual ~Asteroid() override {}

		inline virtual void Update(Arcane::Timestep ts, Arcane::Shared<Arcane::OrthoCameraController>& camControl)
		{
			m_Data.Transform.Position.x += m_Velocity.x * ts;
			m_Data.Transform.Position.y += m_Velocity.y * ts;

			m_Data.Transform.Rotation += m_Velocity.x / m_Velocity.y * ts * 100;

			glm::vec2 camPos = camControl->GetPosition();
			float camWidth   = camControl->GetAspectRatio() * camControl->GetZoom();
			float camHeight  = camControl->GetZoom();

			if (m_Data.Transform.Position.x + m_Data.Transform.Size.x * 0.5f <= camPos.x - camWidth)
				m_Data.Transform.Position.x = camPos.x + camWidth + m_Data.Transform.Size.x * 0.5f;
			else if (m_Data.Transform.Position.x - m_Data.Transform.Size.x * 0.5f >= camPos.x + camWidth)
				m_Data.Transform.Position.x = camPos.x - camWidth - m_Data.Transform.Size.x * 0.5f;

			if (m_Data.Transform.Position.y + m_Data.Transform.Size.y * 0.5f <= camPos.y - camHeight)
				m_Data.Transform.Position.y = camPos.y + camHeight + m_Data.Transform.Size.y * 0.5f;
			else if (m_Data.Transform.Position.y - m_Data.Transform.Size.y * 0.5f >= camPos.y + camHeight)
				m_Data.Transform.Position.y = camPos.y - camHeight - m_Data.Transform.Size.y * 0.5f;
		}

		inline virtual void Render(Arcane::Timestep ts, const Arcane::Shared<Arcane::OrthoCameraController>& camControl)
		{
			Arcane::Renderer2D::DrawRotatedQuad(m_Data.Transform.Position, m_Data.Transform.Size, m_Data.Transform.Rotation, m_Data.Sprite);
		}

		inline void SetPosition(const glm::vec2& position) {
			m_Data.Transform.Position = position;
		}

		inline void SetVelocity(const glm::vec2& velocity)
		{
			m_Velocity = velocity;
		}

		inline const glm::vec2& GetVelocity() const
		{
			return m_Velocity;
		}

		inline glm::vec2& GetVelocity()
		{
			return m_Velocity;
		}

	private:
		inline void SetVariant(int varient = 0)
		{
			if (varient > 2) varient = 0;

			m_Varient = varient;
			m_Data.Sprite = Arcane::SubTexture2D::CreateFromCoords(m_Texture, { m_Varient, 0 }, { 32, 32 });
		}

		inline void SetSize(int size = 3)
		{
			m_Data.Transform.Size = glm::vec2(.4 * size, .4 * size);
		}

	private:
		Arcane::Shared<Arcane::Texture2D> m_Texture;
		glm::vec2 m_Velocity;
		int m_Varient, m_Size;
	};

	class Gameplay
	{
	public:
		Gameplay(Arcane::Shared<Arcane::OrthoCameraController>& camController, 
			std::unordered_map<std::string, Arcane::Shared<Arcane::Texture2D>>& textureMap,
			std::unordered_map<std::string, ALuint>& soundMap);
		~Gameplay();

		void Update(Arcane::Timestep ts, Arcane::Shared<Arcane::OrthoCameraController>& camControl);
		void Events(Arcane::Event& event);

		void Render(Arcane::Timestep ts, const Arcane::Shared<Arcane::OrthoCameraController>& camControl);
		void RenderUI(ImGuiWindowFlags flags, MenuState& menuState, bool& setMenu);

		inline uint32_t GetScore() { return m_Score; }
		inline uint32_t GetLives() { return m_Lives; }

	private:
		void CheckCollisions();

	private:
		std::vector<Arcane::Shared<Star>>     m_Stars;
		std::vector<Arcane::Shared<Asteroid>> m_Asteroids;
		std::vector<Arcane::Shared<Bullet>>   m_ActiveBullets;

		std::vector<Arcane::Shared<Ship>> m_Ships;
		std::map<std::string, Arcane::Shared<Arcane::Texture2D>> m_Textures;

		uint32_t m_Score, m_Lives, m_Level;
		bool m_Started, m_Paused;

	};

}