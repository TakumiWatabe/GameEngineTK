/// <summary>
/// 自機に追従するカメラクラス
/// </summary>
#pragma once

#include "Camera.h"
#include <Keyboard.h>

class FollowCamera : public Camera
{
public:
	//追従対象のカメラの距離
	static const float CAMERA_DISTANCE;

	//コンストラクタ
	FollowCamera(int width,int height);

	//毎フレーム更新
	void Update() override;

	//視点をセット
	void SetTargetPos(const DirectX::SimpleMath::Vector3& targetpos);

	//視点をセット
	void SetTargetAngle(float targetangle);

	//キーボードをセット
	void SetKeyboard(DirectX::Keyboard* keyboard);
protected:
	//追従対象の座標
	DirectX::SimpleMath::Vector3 m_target_pos;
	//追従対象の回転角
	float m_target_angle;
	//キーボード
	DirectX::Keyboard* m_keyboard;
	//キーボードトラッカー
	DirectX::Keyboard::KeyboardStateTracker m_keyboardTracker;
	//FPS/TPSの切り替え用フラグ
	bool ModeChange=true;
};
