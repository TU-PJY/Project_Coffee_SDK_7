#pragma once
#include "SDK_Header.h"
#include "SDK_RectBrush.h"
#include "SDK_CircleBrush.h"
using namespace DirectX;


namespace SDK {
	class OOBB;
	class BoundingCircle;

	class AABB {
	private:
		BoundingBox   aabb;
		bool          Collide{};

		SDK::LineRectBrush LineRect{ true, true };
		SDK::RectBrush     RECT{ true };

	public:
		void Update(float X, float Y, float xScale, float yScale);
		void Update(SDK::Vector2& Position, float xScale, float yScale);
		void Render();
		bool CheckCollision(const AABB& Other);
		bool CheckCollision(const OOBB& Other);
		bool CheckCollision(const BoundingCircle& Other);
		bool CheckCollisionPoint(float X, float Y);
		bool CheckCollisionPoint(SDK::Vector2& Position);
		bool CheckCollisionRay(SDK::RayVector& Ray);
		bool CheckCollisionLine(SDK::RayVector& Ray);
		BoundingBox Get() const;
	};


	class OOBB {
	private:
		BoundingOrientedBox oobb;
		float             Rotation{};
		bool                Collide{};

		SDK::LineRectBrush       LineRect{ true, true };
		SDK::RectBrush           RECT{ true };

	public:
		void Update(float X, float Y, float BoxWidth, float BoxHeight, float RotationValue);
		void Update(SDK::Vector2& Position, float BoxWidth, float BoxHeight, float RotationValue);
		void Render();
		bool CheckCollision(const OOBB& Other);
		bool CheckCollision(const AABB& Other);
		bool CheckCollision(const BoundingCircle& Other);
		bool CheckCollisionPoint(float X, float Y);
		bool CheckCollisionPoint(SDK::Vector2& Position);
		bool CheckCollisionRay(SDK::RayVector& Ray);
		bool CheckCollisionLine(SDK::RayVector& Ray);
		BoundingOrientedBox Get() const;
	};


	class BoundingCircle {
	private:
		BoundingSphere  sphere;
		glm::vec2       Center{};
		float         Radius{};
		float         Size{};

		bool            Collide{};

		SDK::LineCircleBrush LineCircle{ true, true };
		SDK::CircleBrush     Circle{ true };

	public:
		void Update(float X, float Y, float Diameter);
		void Update(SDK::Vector2& Position, float SizeValue);
		void Render();
		bool CheckCollision(const BoundingCircle& Other);
		bool CheckCollision(const AABB& Other);
		bool CheckCollision(const OOBB& Other);
		bool CheckCollisionPoint(float X, float Y);
		bool CheckCollisionPoint(SDK::Vector2& Position);
		bool CheckCollisionRay(SDK::RayVector& Ray);
		bool CheckCollisionLine(SDK::RayVector& Ray);
		BoundingSphere Get() const;
	};
}