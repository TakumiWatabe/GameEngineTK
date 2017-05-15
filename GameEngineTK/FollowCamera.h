/// <summary>
/// ���@�ɒǏ]����J�����N���X
/// </summary>
#pragma once

#include "Camera.h"
#include <d3d11.h>
#include <SimpleMath.h>

class FollowCamera : public Camera
{
public:
	//�Ǐ]�Ώۂ̃J�����̋���
	static const float CAMERA_DISTANCE;

	//�R���X�g���N�^
	FollowCamera(int width,int height);

	//���t���[���X�V
	void Update() override;

	//���_���Z�b�g
	void SetTargetPos(const DirectX::SimpleMath::Vector3& targetpos);

	//���_���Z�b�g
	void SetTargetAngle(float targetangle);

protected:
	//�Ǐ]�Ώۂ̍��W
	DirectX::SimpleMath::Vector3 m_target_pos;
	//�Ǐ]�Ώۂ̉�]�p
	float m_target_angle;

};
