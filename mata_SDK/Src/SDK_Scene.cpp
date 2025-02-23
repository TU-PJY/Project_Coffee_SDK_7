#include "SDK_Scene.h"
#include "SDK_ErrorMessage.h"
#include "SDK_SoundTool.h"
#include "SDK_FPSIndicator.h"

SDK::SDK_Scene SDK::Scene;

void SDK::SDK_Scene::InputFrameTime(float ElapsedTime) {
	FrameTime = ElapsedTime;
}

std::string SDK::SDK_Scene::ModeName() {
	return CurrentRunningModeName;
}

bool SDK::SDK_Scene::CheckCurrentMode(SDK::MODE_PTR ModePtr) {
	if (ModePtr == CurrentRunningModePtr)
		return true;
	return false;
}

SDK::MODE_PTR SDK::SDK_Scene::Mode() {
	return CurrentRunningModePtr;
}

void SDK::SDK_Scene::Stop() {
	UpdateActivateCommand = false;
}

void SDK::SDK_Scene::Resume() {
	if (!ErrorOccured)
		UpdateActivateCommand = true;
}

void SDK::SDK_Scene::Update() {
	if (!ErrorScreenState && ErrorOccured) {
		SDK::SoundTool.StopAllSounds();
		SwitchToErrorScreen();
		ErrorScreenState = true;
	}

	if (ErrorOccured)
		return;

	for (int Layer = 0; Layer < SceneLayer; ++Layer) {
		for (auto& Object : ObjectList[Layer]) {
			if (UpdateActivateCommand) {
				if (!Object->DeleteCommand) {
					if (FloatingFocusCommand) {
						if(Object->FloatingCommand)
							Object->UpdateFunc(FrameTime);
					}
					else
						Object->UpdateFunc(FrameTime);
				}
			}

			if (LoopEscapeCommand || ErrorOccured) {
				LoopEscapeCommand = false;
				return;
			}

			if (Object->DeleteCommand)
				AddLocation(Layer, CurrentReferLocation);

			else if (Object->SwapCommand)
				AddLocation(Layer, CurrentReferLocation);

			++CurrentReferLocation;
		}
		CurrentReferLocation = 0;
	}
}

void SDK::SDK_Scene::Render() {
	for (int i = 0; i < SceneLayer; ++i) {
		for (auto& Object : ObjectList[i]) {
			if (!Object->DeleteCommand) 
				Object->RenderFunc();
		}
	}
}

void SDK::SDK_Scene::Init(SDK::MODE_PTR ModeFunction) {
	ModeFunction();
	for (int Layer = 0; Layer < SceneLayer; ++Layer)
		CommandLocation[Layer].reserve(DELETE_LOCATION_BUFFER_SIZE);
}

void SDK::SDK_Scene::SwitchMode(SDK::MODE_PTR ModeFunction) {
	if (CurrentRunningModePtr == ModeFunction) {
		SetErrorScreen(ERROR_TYPE_EXECUTED_MODE_EXECUTION, CurrentRunningModeName);
		return;
	}

	ClearAll();

	if (DestructorBuffer)
		DestructorBuffer();

	ModeFunction();

	if (FloatingActivateCommand) {
		FloatingActivateCommand = false;
		FloatingFocusCommand = false;
	}

	LoopEscapeCommand = true;
}

void SDK::SDK_Scene::RegisterDestructor(SDK::MODE_PTR DestructorFunction) {
	DestructorBuffer = DestructorFunction;
}

void SDK::SDK_Scene::RegisterModeName(std::string ModeName) {
	CurrentRunningModeName = ModeName;
}

void SDK::SDK_Scene::RegisterModePtr(SDK::MODE_PTR ModePtr) {
	CurrentRunningModePtr = ModePtr;
}

void SDK::SDK_Scene::RegisterController(SUBCLASSPROC Controller, int Type) {
	if(CurrentController)
		RemoveWindowSubclass(SDK::SystemHWND, CurrentController, 1);
	SetWindowSubclass(SDK::SystemHWND, Controller, 1, 0);
	CurrentController = Controller;

	if(Type == MODE_TYPE_DEFAULT)
		ControllerBuffer = Controller;
}

void SDK::SDK_Scene::RegisterInputObjectList(std::vector<SDK::Object*>& Vec) {
	InputObjectListPtr = &Vec;
}

void SDK::SDK_Scene::StartFloatingMode(SDK::MODE_PTR ModeFunction, bool FloatingFocusFlag) {
	if (FloatingActivateCommand) {
		SetErrorScreen(ERROR_TYPE_EXECUTED_FLOATING_MODE_EXECUTUION, CurrentRunningModeName);
		return;
	}

	PrevRunningModeName = CurrentRunningModeName;
	PrevRunningModePtr = CurrentRunningModePtr;
	ModeFunction();
	FloatingFocusCommand = FloatingFocusFlag;

	FloatingActivateCommand = true;
}

void SDK::SDK_Scene::EndFloatingMode() {
	if (!FloatingActivateCommand) {
		SetErrorScreen(ERROR_TYPE_TERMINATED_FLOATING_MODE_TERMINATION, CurrentRunningModeName);
		return;
	}

	ClearFloatingObject();
	CurrentRunningModeName = PrevRunningModeName;
	CurrentRunningModePtr = PrevRunningModePtr;

	RemoveWindowSubclass(SDK::SystemHWND, CurrentController, 1);
	SetWindowSubclass(SDK::SystemHWND, ControllerBuffer, 1, 0);
	CurrentController = ControllerBuffer;

	FloatingActivateCommand = false;
	FloatingFocusCommand = false;
}

SDK::Object* SDK::SDK_Scene::AddObject(SDK::Object* Object, std::string Tag, unsigned int AddLayer, bool UseController, int Type1, int Type2) {
	if (AddLayer > SceneLayer - 2) {
		SetErrorScreen(ERROR_TYPE_OBJECT_ADD_OUT_OF_SCENE_LAYER, CurrentRunningModeName);
		return nullptr;
	}

	if (Type1 == OBJECT_TYPE_STATIC_SINGLE || Type2 == OBJECT_TYPE_STATIC_SINGLE) {
		if (auto Object = Find(Tag); Object)
			return nullptr;
	}

	ObjectList[AddLayer].emplace_back(Object);

	Object->ObjectTag = Tag;
	Object->ObjectLayer = AddLayer;

	if (UseController) 
		InputObjectListPtr->emplace_back(Object);

	if(Type1 == Type2) {
		if(Type1 == OBJECT_TYPE_STATIC || Type1 == OBJECT_TYPE_STATIC_SINGLE)
			Object->StaticCommand = true;

		else if(Type1 == OBJECT_TYPE_FLOATING)
			Object->FloatingCommand = true;
	}

	else {
		if(Type1 == OBJECT_TYPE_STATIC || Type2 == OBJECT_TYPE_STATIC || Type1 == OBJECT_TYPE_STATIC_SINGLE || Type2 == OBJECT_TYPE_STATIC_SINGLE)
			Object->StaticCommand = true;

		if(Type1 == OBJECT_TYPE_FLOATING || Type2 == OBJECT_TYPE_FLOATING)
			Object->FloatingCommand = true;
	}

	return Object;
}

void SDK::SDK_Scene::DeleteObject(SDK::Object* Object) {
	Object->DeleteCommand = true;
	Object->ObjectTag = "";
}

void SDK::SDK_Scene::DeleteObject(std::string Tag, int DeleteRange) {
	switch (DeleteRange) {
	case DELETE_RANGE_SINGLE:
		for (int Layer = 0; Layer < SceneLayer - 1; ++Layer) {
			for (auto const& Object : ObjectList[Layer]) {
				if (Object->ObjectTag.compare(Tag) == 0) {
					Object->DeleteCommand = true;
					Object->ObjectTag = "";
					return;
				}
			}
		}
		break;

	case DELETE_RANGE_ALL:
		for (int Layer = 0; Layer < SceneLayer - 1; ++Layer) {
			for (auto const& Object : ObjectList[Layer]) {
				if (Object->ObjectTag.compare(Tag) == 0) {
					Object->DeleteCommand = true;
					Object->ObjectTag = "";
				}
			}
		}
		break;
	}
}

void SDK::SDK_Scene::AddInputObject(SDK::Object* Object) {
	if (InputObjectListPtr)
		InputObjectListPtr->emplace_back(Object);
}

void SDK::SDK_Scene::DeleteInputObject(SDK::Object* Object) {
	if (InputObjectListPtr) {
		auto Found = std::find(begin(*InputObjectListPtr), end(*InputObjectListPtr), Object);
		if (Found != end(*InputObjectListPtr))
			InputObjectListPtr->erase(Found);
	}
}

void SDK::SDK_Scene::SwapLayer(SDK::Object* Object, unsigned int TargetLayer) {
	if (TargetLayer > SceneLayer - 2) {
		SetErrorScreen(ERROR_TYPE_OBJECT_SWAP_OUT_OF_SCENE_LAYER, CurrentRunningModeName);
		return;
	}
	Object->SwapCommand = true;
	Object->ObjectLayer = TargetLayer;
}

SDK::Object* SDK::SDK_Scene::Find(std::string Tag) {
	for (int Layer = 0; Layer < SceneLayer - 1; ++Layer) {
		for (auto const& Object : ObjectList[Layer]) {
			if (Object->ObjectTag.compare(Tag) == 0)
				return Object;
		}
	}

	return nullptr;
}

SDK::Object* SDK::SDK_Scene::ReverseFind(std::string Tag) {
	for (int Layer = SceneLayer - 2; Layer >= 0; --Layer) {
		for (auto Object = ObjectList[Layer].rbegin(); Object != ObjectList[Layer].rend(); ++Object) {
			if ((*Object)->ObjectTag.compare(Tag) == 0)
				return *Object;
		}
	}

	return nullptr;
}

SDK::Object* SDK::SDK_Scene::FindMulti(std::string Tag, unsigned int SearchLayer, int Index) {
	if (SearchLayer > SceneLayer - 2) {
		SetErrorScreen(ERROR_TYPE_OBJECT_FIND_OUT_OF_SCENE_LAYER, CurrentRunningModeName);
		return nullptr;
	}

	auto Object = ObjectList[SearchLayer][Index];
	if(Object->ObjectTag.compare(Tag) == 0)
		return ObjectList[SearchLayer][Index];
	
	return nullptr;
}

size_t SDK::SDK_Scene::LayerSize(unsigned int TargetLayer) {
	if (TargetLayer > SceneLayer - 2) {
		SetErrorScreen(ERROR_TYPE_RETRIEVE_LAYER_SIZE_OUT_OF_SCENE_LAYER, CurrentRunningModeName);
		return 0;
	}

	return ObjectList[TargetLayer].size();
}

void SDK::SDK_Scene::DeleteTag(SDK::Object* Object) {
	Object->ObjectTag = "";
}

void SDK::SDK_Scene::DeleteTag(std::string Tag) {
	auto Object = Find(Tag);
	if (Object)
		Object->ObjectTag = "";
}

void SDK::SDK_Scene::CompleteCommand() {
	if(!CommandExist)
		return;

	UpdateObjectList();
	CommandExist = false;
}

void SDK::SDK_Scene::SetErrorScreen(int ErrorType, std::string Value1, std::string Value2) {
	if (ErrorOccured)
		return;

	Value1Buffer = Value1;
	Value2Buffer = Value2;
	ErrorTypeBuffer = ErrorType;
	ErrorOccured = true;
}

void SDK::SDK_Scene::AddSystemObject(SDK::Object* Object) {
	if (SystemObjectAdded) {
		SetErrorScreen(ERROR_TYPE_PERMISSION_VIOLATION_SYSTEM_LAYER_ACCESS, CurrentRunningModeName);
		return;
	}

	SystemObjectAdded = true;

	ObjectList[EOL].emplace_back(Object);
}

//////// private ///////////////
void SDK::SDK_Scene::AddLocation(int Layer, int Position) {
	CommandLocation[Layer].emplace_back(Position);
	CommandExist = true;
}

void SDK::SDK_Scene::UpdateObjectList() {
	int Offset{};

	for (int Layer = 0; Layer < SceneLayer - 1; ++Layer) {
		size_t Size = CommandLocation[Layer].size();
		if (Size == 0)
			continue;

		for (int i = 0; i < Size; ++i) {
			auto Object = begin(ObjectList[Layer]) + CommandLocation[Layer][i] - Offset;

			if ((*Object)->DeleteCommand) {
				delete* Object;
				*Object = nullptr;
				ObjectList[Layer].erase(Object);
			}

			else if ((*Object)->SwapCommand) {
				auto Ptr = (*Object);
				ObjectList[Ptr->ObjectLayer].emplace_back((*Object));
				ObjectList[Layer].erase(Object);
				Ptr->SwapCommand = false;
			}

			++Offset;
		}

		CommandLocation[Layer].clear();
		Offset = 0;
	}
}

void SDK::SDK_Scene::ClearFloatingObject() {
	for (int Layer = 0; Layer < SceneLayer - 1; ++Layer) {
		size_t Size = LayerSize(Layer);
		for (int i = 0; i < Size; ++i) {
			if (!ObjectList[Layer][i]->StaticCommand && ObjectList[Layer][i]->FloatingCommand) {
				ObjectList[Layer][i]->DeleteCommand = true;
				ObjectList[Layer][i]->ObjectTag = "";
			}
		}
	}
}

void SDK::SDK_Scene::ClearAll() {
	for (int Layer = 0; Layer < SceneLayer - 1; ++Layer) {
		CommandLocation[Layer].clear();
		size_t Size = LayerSize(Layer);
		for (int i = 0; i < Size; ++i) {
			if (!ObjectList[Layer][i]->StaticCommand) {
				ObjectList[Layer][i]->DeleteCommand = true;
				ObjectList[Layer][i]->ObjectTag = "";
				AddLocation(Layer, i);
			}
		}
	}

	LoopEscapeCommand = true;
}

void ErrorScreenController(unsigned char Key, int X, int Y) {
	if (Key == 27 || Key == 32)
		SDK::System.Exit();
}

void SDK::SDK_Scene::SwitchToErrorScreen() {
	RemoveWindowSubclass(SDK::SystemHWND, CurrentController, 1);
	glutKeyboardFunc(ErrorScreenController);

	SystemObjectAdded = false;

	if (Value2Buffer.empty())
		AddSystemObject(new SDK_ErrorMessage(ErrorTypeBuffer, Value1Buffer));
	else
		AddSystemObject(new SDK_ErrorMessage(ErrorTypeBuffer, Value1Buffer));
}
