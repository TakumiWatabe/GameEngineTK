#include "FollowCamera.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

//静的メンバ変数の初期化
const float FollowCamera::CAMERA_DISTANCE = 0.5f;

FollowCamera::FollowCamera(int width, int height)
	:Camera(width,height)
{
	m_target_pos=Vector3::Zero;
	m_target_angle = 0.0f;
}

void FollowCamera::Update()
{
	//視点、参照点
	Vector3 eyepos, refpos;

	//自機の上空2mえを参照点
	refpos = m_target_pos + Vector3(0, 2, 0);
	//参照点と視点の差分のベクトル
	Vector3 cameraV(0.0f, 0.0f, CAMERA_DISTANCE);

	//自機の後ろに回り込んための回転行列を生成
	Matrix rotmat = Matrix::CreateRotationY(m_target_angle);
	//差分ベクトルを回転
	cameraV = Vector3::TransformNormal(cameraV, rotmat);
	//視点座標を計算
	eyepos = refpos + cameraV;

	SetEyePos(eyepos);		//thisを省略
	SetRefPos(refpos);

	//基底クラスの更新
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
