#include "Camera.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

/// <summary>
/// コンストラクタ
/// </summary>
Camera::Camera(int width,int height)
{
	//メンバ変数の初期化
	m_eyepos = Vector3(0, 0, 5.0f);
	m_refpos =Vector3(10, 10, 10);
	m_upvec = Vector3(0, 1, 0);
	m_fovY = XMConvertToRadians(60.0f);
	m_aspect = (float)width / height;
	m_nearClip = 0.1f;
	m_farClip = 1000.0f;

	//ビュー行列の生成
	m_view = Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);

	//射影行列の生成(透視投影)
	m_proj = Matrix::CreatePerspectiveFieldOfView(m_fovY, m_aspect, m_nearClip, m_farClip);

}
/// <summary>
/// デストラクタ
/// </summary>
Camera::~Camera()
{

}
/// <summary>
/// Updata
/// </summary>
void Camera::Update()
{
	//ビュー行列の生成
	m_view = Matrix::CreateLookAt(m_eyepos, m_refpos, m_upvec);

	//射影行列の生成
	m_proj = Matrix::CreatePerspectiveFieldOfView(m_fovY, m_aspect, m_nearClip,m_farClip);
}

//ビュー行列を取得
const Matrix& Camera::GetView()
{
	return m_view;
}

//プロジェクション行列を取得
const Matrix& Camera::GetProjection()
{
	return m_proj;
}

//視点をセット
void Camera::SetEyePos(const Vector3& eyepos)
{
	m_eyepos = eyepos;

}
//注視点をセット
void Camera::SetRefPos(const Vector3& refpos)
{
	m_refpos = refpos;

}
//上方向ベクトルをセット
void Camera::SetUpVec(const Vector3& upvec)
{
	m_upvec = upvec;
}

void Camera::SetFovY(float fovY)
{
	m_fovY = fovY;
}

void Camera::SetAspect(float aspect)
{
	m_aspect = aspect;
}

void Camera::SetNearClip(float nearclip)
{
	m_nearClip = nearclip;
}

void Camera::SetFarClip(float farClip)
{
	m_farClip = farClip;
}
