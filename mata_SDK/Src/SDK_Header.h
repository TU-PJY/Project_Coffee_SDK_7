#pragma once
#pragma warning(disable: 4244)
#pragma warning(disable: 4305)
#pragma warning(disable: 26444)
#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "winmm.lib")
#include "SDK_Config.h"
#include "glew.h"
#include "freeglut.h"
#include "freeglut_ext.h"
#include "glm/ext.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <locale>
#include <iostream>
#include <vector>
#include <string>
#include <commctrl.h>
#include <mmsystem.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#undef SYSTEM_FONT
using namespace DirectX;

#include "fmod.hpp"
#include "fmod_errors.h"

// global shader location
void ModeAttribute();

namespace SDK {
	// corner position of display
	typedef struct {
		float LeftX, LeftY, RightX, RightY;
	}ViewportRect;
	extern ViewportRect WindowRect;

	// key event
	typedef struct {
		UINT Type;
		WPARAM Key;
	}KeyEvent;

	// ray vector struct
	typedef struct {
		XMVECTOR Origin;
		XMVECTOR Direction;
		float Distance;
		float Length;
	}RayVector;

	// data set struct
	typedef struct {
		int         DataType;
		std::string CategoryName;
		std::string DataName;
		float       DigitValue;
		std::string StringValue;
	}FileData;

	// image struct
	typedef struct {
		unsigned int Texture;
		int          Width, Height;
	}Image;

	// sprite sheet struct
	typedef struct {
		std::vector<unsigned int> Texture;
		int                Width, Height;
		int                Frame;
	}SpriteSheet;

	// pre load image struct
	typedef struct {
		Image* ImagePtr;
		unsigned char* TextureData;
		int            ImageType;
	}PreLoadInfo;

	// pre load sprite sheet struct
	typedef struct {
		SpriteSheet* SpriteSheetPtr;
		std::vector<unsigned char*> TextureData;
		int            ImageType;
	}PreLoadSpriteSheetInfo;

	// sound channel struct
	struct SoundChannel {
		float Volume = 1.0;
		float PlaySpeed = 1.0;
		FMOD::Channel* Channel{};
	};

	// fast sound channel struct
	struct FastSoundChannel {
		float Volume = 1.0;
		float PlaySpeed = 1.0;
		int NumChannel{};
		int PlayChannel = 0;
		int StopChannel = 1;
		std::vector<FMOD::Channel*> Channel{};
	};

	using Matrix4 = glm::mat4;
	using Color3  = glm::vec3;
	using Vector4 = glm::vec4;
	using Vector3 = glm::vec3;
	using Vector2 = glm::vec2;

	using FontName = wchar_t*;

	using ShaderLocation = unsigned int;
	using ShaderResource = unsigned int;
	using SSBO = unsigned int;

	typedef void(*START_MODE_PTR)(void);
	typedef void(*MODE_PTR)(void);
	typedef void(*CONTROLLER_PTR)(void);
	extern START_MODE_PTR START_MODE;

	using SoundChannelGroup = std::vector <SoundChannel>;
	using Sound = FMOD::Sound*;

	using StringDataVec = std::vector<std::string>;
	using DigitDataVec = std::vector<float>;
	using DataFormat = std::vector<FileData>;

	// SinLoop presets
	namespace Preset {
		constexpr float ZeroToPositive = 0.0;
		constexpr float ZeroToNegative = XM_PI;
		constexpr float MaxPositive = XM_PI / 2.0;
		constexpr float MaxNegative = -XM_PI / 2.0;
		constexpr float HalfPositive = XM_PI / 6.0;
		constexpr float HalfNegative = -XM_PI / 6.0;
	}

	// Multiply the value by the window aspect ratio.
	float ASP(float Value);

	class SDK_System {
	private:
		bool  UpdateActivateCommand{};
		float PrevTime{}, CurrentTime{}, DeltaTime{};

		float DestFPS{};
		int   FPSLimit{};

		bool  FullscreenState{};

	public:
		static SDK_System* S_Inst;

		SDK_System() {
			S_Inst = this;
		}

		static void Main();
		static void DisplayReshape(int w, int h);
		void SwitchScreenState();
		void ChangeScreenSize(int ScreenWidth, int ScreenHeight);
		void SetFrameLimit(int FrameLimit);
		void HideCursor();
		void ShowCursor();
		void MoveCursor(int X, int Y);
		void SetBackColor(float R, float G, float B);
		void SetBackColor(SDK::Color3& Color);
		void SetBackColorRGB(int R, int G, int B);
		void SetupSystem(int argc, char** argv);
		void InitSystem();
		void SetGlOption();
		void SetupWindow();
		void LoadShader();
		void Exit();
	};

	extern SDK_System System;
	extern glm::vec3 ViewportColor;
	extern std::wstring LOCALE;
	extern float ASPECT;
	extern int WIDTH, HEIGHT;
	extern int PREV_WIDTH, PREV_HEIGHT;
	extern const glm::mat4 IdentityMatrix;
	constexpr FontName SYSTEM_FONT = L"Roboto";

	extern HWND SystemHWND;
}

// global scope shader
extern SDK::ShaderResource IMAGE_SHADER;
extern SDK::ShaderResource TEXT_SHADER;
extern SDK::ShaderResource SHAPE_SHADER;
extern SDK::ShaderResource MATRIX_COMPT_SHADER;

// global SSBO I/O
extern SDK::SSBO SSBO_MATRIX_INPUT, SSBO_MATRIX_OUTPUT;

// global scope shader location
extern SDK::ShaderLocation IMAGE_OPACITY_LOCATION, IMAGE_COLOR_LOCATION, IMAGE_MODEL_LOCATION;
extern SDK::ShaderLocation IMAGE_PROJECTION_LOCARION, IMAGE_VIEW_LOCATION, IMAGE_VIEW_POSITION_LOCATION;
extern SDK::ShaderLocation BLUR_STRENGTH_LOCATION, BLUR_STATE_LOCATION, BLUR_EXECUTION_LOCATION, TEXTURE_SIZE_LOCATION;

extern SDK::ShaderLocation TEXT_OPACITY_LOCATION, TEXT_COLOR_LOCATION, TEXT_MODEL_LOCATION;
extern SDK::ShaderLocation TEXT_PROJECTION_LOCATION, TEXT_VIEW_LOCATION, TEXT_VIEW_POSITION_LOCATION;

extern SDK::ShaderLocation SHAPE_OPACITY_LOCATION, SHAPE_COLOR_LOCATION, SHAPE_MODEL_LOCATION;
extern SDK::ShaderLocation SHAPE_PROJECTION_LOCATION, SHAPE_VIEW_LOCATION, SHAPE_VIEW_POSITION_LOCATION;

enum SDK_MouseState {
	EVENT_NONE,
	LEFT_BUTTON_DOWN,
	LEFT_BUTTON_UP,
	RIGHT_BUTTON_DOWN,
	RIGHT_BUTTON_UP,
	MIDDLE_BUTTON_DOWN,
	MIDDLE_BUTTON_UP,
	FORWARD_BUTTON_DOWN,
	FORWARD_BUTTON_UP,
	BACKWARD_BUTTON_DOWN,
	BACKWARD_BUTTON_UP,
	WHEEL_DOWN,
	WHEEL_UP
};

enum SDK_ZoomType {
	ZOOM_IN,
	ZOOM_OUT
};

enum SDK_RenderType {
	RENDER_TYPE_DEFAULT,
	RENDER_TYPE_STATIC
};

enum SDK_ShaderType {
	SHADER_TYPE_IMAGE,
	SHADER_TYPE_TEXT,
	SHADER_TYPE_SHAPE
};

enum SDK_ImageType {
	IMAGE_TYPE_LINEAR,
	IMAGE_TYPE_NEAREST
};

enum SDK_DataType {
	DATA_TYPE_DIGIT,
	DATA_TYPE_STRING
};

enum SDK_ImageFlip {
	FLIP_TYPE_NONE,
	FLIP_TYPE_H,
	FLIP_TYPE_V,
	FLIP_TYPE_HV
};

enum SDK_TextAlign {
	ALIGN_DEFAULT,
	ALIGN_MIDDLE,
	ALIGN_LEFT
};

enum SDK_TextHeightClamp {
	HEIGHT_ALIGN_DEFAULT,
	HEIGHT_ALIGN_MIDDLE,
	HEIGHT_ALIGN_UNDER
};

enum SDK_CheckTimeOption {
	CHECK_AND_RESUME,
	CHECK_AND_RESET,
	CHECK_AND_INTERPOLATE,
	CHECK_AND_STOP
};

enum SDK_UIClampEdge {
	EDGE_LEFT_DOWN,
	EDGE_LEFT_UP,
	EDGE_RIGHT_UP,
	EDGE_RIGHT_DOWN
};

enum SDK_LineType {
	LINE_TYPE_RECT,
	LINE_TYPE_ROUND
};

enum SDK_EXClampType {
	CLAMP_GREATER,
	CLAMP_LESS,
	CLAMP_FIXED,
	CLAMP_RETURN
};

enum SDK_DeleteRange {
	DELETE_RANGE_SINGLE,
	DELETE_RANGE_ALL
};

enum SDK_ModeType {
	MODE_TYPE_NONE,
	MODE_TYPE_DEFAULT,
	MODE_TYPE_FLOATING
};

enum SDK_ObjectType {
	OBJECT_TYPE_NONE,
	OBJECT_TYPE_STATIC,
	OBJECT_TYPE_STATIC_SINGLE,
	OBJECT_TYPE_FLOATING
};

enum DistTypeEnum {
	RANDOM_TYPE_REAL,
	RANDOM_TYPE_INT
};

enum SDK_SwitchType {
	TRUE_KEY_DOWN,
	FALSE_KEY_DOWN
};

enum SDK_SystemErrorType {
	ERROR_TYPE_IMAGE_LOAD,
	ERROR_TYPE_SOUND_LOAD,
	ERROR_TYPE_FONT_LOAD,
	ERROR_TYPE_SCRIPT_LOAD,
	ERROR_TYPE_SCRIPT_PARSE,
	ERROR_TYPE_SCRIPT_DATA,
	ERROR_TYPE_SCRIPT_CATEGORY,
	ERROR_TYPE_DATA_FILE_PARSE,
	ERROR_TYPE_DATA_FILE_DATA_LOAD,
	ERROR_TYPE_DATA_FILE_DATA_WRITE,
	ERROR_TYPE_DATA_FILE_CATEGORY,
	ERROR_TYPE_EXECUTED_MODE_EXECUTION,
	ERROR_TYPE_EXECUTED_FLOATING_MODE_EXECUTUION,
	ERROR_TYPE_TERMINATED_FLOATING_MODE_TERMINATION,
	ERROR_TYPE_OBJECT_ADD_OUT_OF_SCENE_LAYER,
	ERROR_TYPE_OBJECT_SWAP_OUT_OF_SCENE_LAYER,
	ERROR_TYPE_OBJECT_FIND_OUT_OF_SCENE_LAYER,
	ERROR_TYPE_RETRIEVE_LAYER_SIZE_OUT_OF_SCENE_LAYER,
	ERROR_TYPE_PERMISSION_VIOLATION_SYSTEM_LAYER_ACCESS
};