#pragma once

#include <Arcane.h>

#include <glm/glm.hpp>

enum MenuState : uint32_t
{
	NotActive = 0,
	Main, GameSelect, About
};

enum GameState : uint32_t
{
	Menu = 0,
	Asteroids,
	PacMan,
	BrickBreak
};

struct TransformData
{
	glm::vec2 Position = { 0.0f, 0.0f };
	glm::vec2 Size = { 1.0f, 1.0f };
	float Rotation = 0.0f;
};

struct EntityData
{
	Arcane::Shared<Arcane::SubTexture2D> Sprite;
	TransformData Transform;
	bool Alive = true;
};

struct Entity {
public:
	Entity() : m_Data() {}
	virtual ~Entity() {}

	virtual void Update(Arcane::Timestep ts, Arcane::Shared<Arcane::OrthoCameraController>& camControl) {};
	virtual void Render(Arcane::Timestep ts, const Arcane::Shared<Arcane::OrthoCameraController>& camControl) {};

	inline void SetAlive(bool alive) { m_Data.Alive = alive; }

	inline bool Collides(TransformData& transform) {
        // Get rotated corners
        std::array<glm::vec2, 4> thisCorners = GetRotatedCorners(m_Data.Transform);
        std::array<glm::vec2, 4> otherCorners = GetRotatedCorners(transform);

        // Check for overlap using SAT
        return !HasSeparatingAxis(thisCorners, otherCorners);
	}

	inline const Arcane::Shared<Arcane::SubTexture2D>& GetSprite() const { return m_Data.Sprite; }

    inline const TransformData& GetTransform() const { return m_Data.Transform; }
    inline const glm::vec2& GetPosition() const { return GetTransform().Position; }
    inline const glm::vec2& GetSize() const { return GetTransform().Size; }
    inline const float GetRotation() const { return GetTransform().Rotation; }

    inline TransformData& GetTransform() { return m_Data.Transform; }
    inline glm::vec2& GetPosition() { return GetTransform().Position; }
    inline glm::vec2& GetSize() { return GetTransform().Size; }
    inline float GetRotation() { return GetTransform().Rotation; }

	inline const bool IsAlive() const { return m_Data.Alive; }

private:
    std::array<glm::vec2, 4> GetRotatedCorners(const TransformData& data) {
        glm::vec2 halfSize = data.Size * 0.5f;

        // Define local corners relative to the center
        std::array<glm::vec2, 4> localCorners = {
            glm::vec2(-halfSize.x, -halfSize.y),
            glm::vec2(halfSize.x, -halfSize.y),
            glm::vec2(halfSize.x,  halfSize.y),
            glm::vec2(-halfSize.x,  halfSize.y)
        };

        // Rotation matrix
        float c = cos(data.Rotation);
        float s = sin(data.Rotation);
        glm::mat2 rotationMatrix = { {c, -s}, {s, c} };

        // Rotate and translate corners
        std::array<glm::vec2, 4> worldCorners;
        for (int i = 0; i < 4; i++) {
            worldCorners[i] = rotationMatrix * localCorners[i] + data.Position;
        }
        return worldCorners;
    }

    bool HasSeparatingAxis(const std::array<glm::vec2, 4>& box1, const std::array<glm::vec2, 4>& box2) {
        // List of possible separating axes (each box's edge normals)
        std::array<glm::vec2, 4> axes = {
            GetEdgeNormal(box1[0], box1[1]),
            GetEdgeNormal(box1[1], box1[2]),
            GetEdgeNormal(box2[0], box2[1]),
            GetEdgeNormal(box2[1], box2[2])
        };

        // Check for separation along each axis
        for (const glm::vec2& axis : axes) {
            if (IsSeparated(box1, box2, axis)) {
                return true; // If a separating axis is found, no collision
            }
        }
        return false; // If no separating axis, there is a collision
    }

    glm::vec2 GetEdgeNormal(const glm::vec2& p1, const glm::vec2& p2) {
        glm::vec2 edge = p2 - p1;
        return glm::normalize(glm::vec2(-edge.y, edge.x)); // Perpendicular vector
    }

    bool IsSeparated(const std::array<glm::vec2, 4>& box1, const std::array<glm::vec2, 4>& box2, const glm::vec2& axis) {
        // Project each box onto the axis
        auto [min1, max1] = ProjectOntoAxis(box1, axis);
        auto [min2, max2] = ProjectOntoAxis(box2, axis);

        // Check for gap
        return (max1 < min2 || max2 < min1);
    }

    std::pair<float, float> ProjectOntoAxis(const std::array<glm::vec2, 4>& box, const glm::vec2& axis) {
        float min = glm::dot(box[0], axis);
        float max = min;
        for (int i = 1; i < 4; i++) {
            float projection = glm::dot(box[i], axis);
            min = glm::min(min, projection);
            max = glm::max(max, projection);
        }
        return { min, max };
    }

protected:
	EntityData m_Data;

};