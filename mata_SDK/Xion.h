#pragma once
#include <SDK_Scene.h>

class Xion : public SDK::Object {
private:
	// ���� ��ġ
	SDK::Vector2 Position{};

	// ��ǥ �̵� ��ġ
	float DestPosition{};

	// ���� ���� �ִϸ��̼� ũ��
	float VerticalSize{};

	// ���� �ִϸ��̼�
	SDK::SinLoop VerticalLoop{};

	// ����� ��
	GLfloat TiltValue{};

	// ����� ���� �ִϸ��̼�
	SDK::SinLoop TiltLoop{};
	
	// ���� ��ġ
	SDK::Vector2 ShakeValue{};

	// ���� Ÿ�̸�
	SDK::Timer ShakeTimer{};

	// ȸ����
	float Rotation{};

	// ���� ȸ����
	float HRotation{};
	float DestHRotation{};

	// �����̴� ����
	bool MoveState{};

	// �̵忡�� ���� ����
	bool HitState{};

	// �Ѿ��� ����
	bool FellDown{};

	// �и��� ����
	bool PushState{};
	
	// ������
	int Frame{};

	SDK::SoundChannel SndChannel{};

	// ���ӿ��� ������ Ÿ�̸�
	SDK::Timer GameOverTimer{};

	bool ShowBack{};

public:
	// MoveState�� Ȱ��ȭ�� ���·� �����Ǹ� ������ ��ġ�� �̵��Ѵ�.
	// MoveState�� ��Ȱ��ȭ�� ���·� �����Ǹ� �̵����� �ʴ´�.
	Xion(float PositionValue, float DestPositionValue, bool BoolMoveState, int FrameValue);
	void UpdateFunc(float FrameTime);
	void RenderFunc();
	void HitPeople();
	void PushPeople();
	void SetFrame(int Value);
	void EnableBack();
	void DisableBack();
};