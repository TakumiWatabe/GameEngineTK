#include "FollowCamera.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

//�ÓI�����o�ϐ��̏�����
const float FollowCamera::CAMERA_DISTANCE = 0.5f;

FollowCamera::FollowCamera(int width, int height)
	:Camera(width,height)
{
	m_target_pos=Vector3::Zero;
	m_target_angle = 0.0f;
}

void FollowCamera::Update()
{
	//���_�A�Q�Ɠ_
	Vector3 eyepos, refpos;

	//���@�̏��2m�����Q�Ɠ_
	refpos = m_target_pos + Vector3(0, 2, 0);
	//�Q�Ɠ_�Ǝ��_�̍����̃x�N�g��
	Vector3 cameraV(0.0f, 0.0f, CAMERA_DISTANCE);

	//���@�̌��ɉ�荞�񂽂߂̉�]�s��𐶐�
	Matrix rotmat = Matrix::CreateRotationY(m_target_angle);
	//�����x�N�g������]
	cameraV = Vector3::TransformNormal(cameraV, rotmat);
	//���_���W���v�Z
	eyepos = refpos + cameraV;

	SetEyePos(eyepos);		//this���ȗ�
	SetRefPos(refpos);

	//���N���X�̍X�V
	Camera::Update();

}

void FollowCamera::SetTargetPos(const DirectX::SimpleMath::Vector3 & targetpos)
{
	m_target_pos = targetpos;
}

void FollowCamera::SetTargetAngle(float targetangle)
{
	m_target_angle = targetangle;
}
