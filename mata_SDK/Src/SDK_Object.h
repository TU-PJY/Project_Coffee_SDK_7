#pragma once
#include "SDK_UtilPack.h"

namespace SDK {
	class Object {
	public:
		std::string ObjectTag{};
		int         ObjectLayer{};

		bool        FloatingCommand{};
		bool        StaticCommand{};

		bool        DeleteCommand{};
		bool        SwapCommand{};
		bool        ControllerCommand{};

		// Initialize matrix. Select a render type.
		void Begin(int RenderType = RENDER_TYPE_DEFAULT);

		// Initialize unit matrix.
		void IdentityUnitMatrix();

		// Updates the object's position relative to the viewport. Choose whether to apply aspect ratio.
		void ComputeViewportPosition(float& DestX, float& DestY, bool ApplyAspect = true);

		// Updates the object's position relative to the viewport. Choose whether to apply aspect ratio.
		void ComputeViewportPosition(SDK::Vector2& DestValue, bool ApplyAspect);

		// Updates the object's position relative to the local coordinate system.
		void ComputeLocalPosition(float& DestX, float& DestY);

		// Updates the object's position relative to the local coordinate system.
		void ComputeLocalPosition(SDK::Vector2& DestPosition);

		// class destructor
		virtual ~Object() {}

		// Updates a value inside an object class.
		virtual void UpdateFunc(float FrameTime) {}

		// Renders image resources used by object classes.
		virtual void RenderFunc() {}

		// Sends keyboard events to a specific object class.
		virtual void InputKey(SDK::KeyEvent& Event) {}

		// Sends mouse events to a specific object class.
		virtual void InputMouse(int Type) {}

		// Initializes the control state for a specific object class.
		virtual void ResetControlState() {}

		// Obtains an AABB object from a specific object class.
		virtual SDK::AABB GetAABB() { return {}; }

		// Obtains an OOBB object from a specific object class.
		virtual SDK::OOBB GetOOBB() { return{}; }

		// Obtains an BoundingCircle object from a specific object class.
		virtual SDK::BoundingCircle GetBoundingCircle() { return {}; }

		// playmode manager functions
		virtual void SetGoToTitle() {}
		virtual void PlayBGM() {}
		virtual void StopBGM() {}

		// timewatch functions
		virtual void Stop() {}
		virtual void Start() {}
		virtual void AddTime() {}
		virtual void DeleteTime() {}

		// people functions
		virtual void HitPeople() {}
		virtual void EnableBack() {}

		// xion functions
		virtual void PushPeople() {}

		// score funxtions
		virtual void AddScore(int Score) {}

		// shelf functions
		virtual void EnableCoffeeHit() {}
		virtual ItemStruct GetFrontCoffee() { return{}; }
		virtual ItemStruct GetNextCoffee() { return{}; }
		virtual void BreakCoffee() {}

		// ed functions
		virtual void TellNextPosition(float PositionValue) {}
		virtual float GetPosition() { return{}; }
		virtual void EnableInput() {}
		virtual void DisableInput() {}

		// cover functions
		virtual bool GetState() { return{}; }

	private:
		glm::vec4 ViewportPosition();
		glm::vec4 LocalPosition();
	};
}