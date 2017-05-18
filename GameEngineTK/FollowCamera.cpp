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
	m_keyboard = nullptr;
}

void FollowCamera::Update()
{

	//キーボードの状態取得
	Keyboard::State keyboardstate = m_keyboard->GetState();
	m_keyboardTracker.Update(keyboardstate);
	//Cキーを押すごとにFPS/TPSを切り替え
	if (m_keyboardTracker.IsKeyPressed(Keyboard::Keys::C))
	{
		////フラグを切り替え
		//if (ModeChange != true)
		//{
		//	ModeChange = true;
		//}
		//else
		//{
		//	ModeChange = false;
		//}

		//フラグを切り替え(上の処理が一行に出来る)
		ModeChange = !ModeChange;
		
	}
	if (ModeChange == true)
	{
		//TPSカメラ処理
		const float CAMERA_DISTANCE = 5.0f;
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
		eyepos = refpos + cameraV;
		//視点を現在位置から補間する
		eyepos = m_eyepos + (eyepos - m_eyepos)*0.05f;
		//参照点を現在位置から補間する
		refpos = m_refpos + (refpos - m_refpos)*0.2f;

		SetEyePos(eyepos);		//thisを省略
		SetRefPos(refpos);
	}
	if (ModeChange == false)
	{
		const float CAMERA_DISTANCE = 5.0f;
		//視点、参照点
		Vector3 eyepos, refpos;
		//自機の目の位置あたりを視点とする
		Vector3 UpPos;
		UpPos = m_target_pos + Vector3(0, 0.2f, 0);
		//参照点と視点の差分のベクトル
		Vector3 cameraV(0.0f, 0.0f, -CAMERA_DISTANCE);
		//自機の後ろに回り込んための回転行列を生成
		Matrix rotmat = Matrix::CreateRotationY(m_target_angle);
		//差分ベクトルを回転
		cameraV = Vector3::TransformNormal(cameraV, rotmat);
		eyepos = UpPos + cameraV*0.1f;
		//参照点座標を計算
		refpos = eyepos + cameraV;

		SetEyePos(eyepos);		//thisを省略
		SetRefPos(refpos);
	}

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

void FollowCamera::SetKeyboard(DirectX::Keyboard * keyboard)
{
	m_keyboard = keyboard;
}
