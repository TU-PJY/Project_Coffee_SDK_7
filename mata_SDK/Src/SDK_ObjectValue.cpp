#include "SDK_ObjectValue.h"

// Object Value
SDK::Matrix4 SDK::MoveMatrix	     = SDK::Matrix4(1.0);
SDK::Matrix4 SDK::RotateMatrix		 = SDK::Matrix4(1.0);
SDK::Matrix4 SDK::ScaleMatrix		 = SDK::Matrix4(1.0);
SDK::Matrix4 SDK::FlipMatrix         = SDK::Matrix4(1.0);
SDK::Matrix4 SDK::ResultMatrix       = SDK::Matrix4(1.0);

SDK::Matrix4 SDK::UnitMoveMatrix     = SDK::Matrix4(1.0);
SDK::Matrix4 SDK::UnitRotateMatrix   = SDK::Matrix4(1.0);
SDK::Matrix4 SDK::UnitScaleMatrix    = SDK::Matrix4(1.0);
SDK::Matrix4 SDK::UnitFlipMatrix     = SDK::Matrix4(1.0);

SDK::Matrix4 SDK::ViewportPositionMatrix = SDK::Matrix4(1.0);
SDK::Color3  SDK::ObjectColor;

float SDK::ObjectOpacityValue;
float SDK::ObjectBlurValue;

float SDK::UnitBlurValue;
float SDK::UnitOpacityValue;