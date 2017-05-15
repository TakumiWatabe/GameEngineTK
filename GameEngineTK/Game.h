//
// Game.h
//

#pragma once

#include "StepTimer.h"
#include <VertexTypes.h>
#include <PrimitiveBatch.h>
#include <Effects.h>
#include <CommonStates.h>
#include <SimpleMath.h>
#include <Model.h>
#include "DebugCamera.h"
#include <Keyboard.h>
#include "Camera.h"
#include "FollowCamera.h"

#define MAX_BALL 10
#define MAX_TEAPOD 20
// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game
{
public:

    Game();

    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const;
	
	
private:

    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();
    void Present();

    void CreateDevice();
    void CreateResources();

    void OnDeviceLost();

	
    // Device resources.
    HWND                                            m_window;
    int                                             m_outputWidth;
    int                                             m_outputHeight;

    D3D_FEATURE_LEVEL                               m_featureLevel;
    Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice1;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext1;

    Microsoft::WRL::ComPtr<IDXGISwapChain>          m_swapChain;
    Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain1;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

    // Rendering loop timer.
    DX::StepTimer                                   m_timer;

	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionNormal>> m_batch;
	std::unique_ptr<DirectX::BasicEffect> m_effect;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	DirectX::SimpleMath::Matrix m_world;
	DirectX::SimpleMath::Matrix m_view;
	DirectX::SimpleMath::Matrix m_proj;

	//キーボード
	std::unique_ptr<DirectX::Keyboard> keyboard;

	//デバッグカメラ
	std::unique_ptr<DebugCamera> m_debugCamera;

	//エフェクトファクトリー
	std::unique_ptr<DirectX::EffectFactory> m_factory;
	//天球モデル
	std::unique_ptr<DirectX::Model> m_modelSkydorm;
	//地面モデル
	std::unique_ptr<DirectX::Model> m_modelGround;
	//球モデル
	std::unique_ptr<DirectX::Model> m_modelBall;
	//ティーポットモデル
	std::unique_ptr<DirectX::Model> m_modelTeapod;
	//ロボット(タンク)モデル
	std::unique_ptr<DirectX::Model> m_modelTank;

	//球ワールド行列
	DirectX::SimpleMath::Matrix m_worldBall1[MAX_BALL];
	DirectX::SimpleMath::Matrix m_worldBall2[MAX_BALL];
	//ティーポッドワールド行列
	DirectX::SimpleMath::Matrix m_worldTeapod[MAX_TEAPOD];
	//ロボット(タンク)ワールド行列
	DirectX::SimpleMath::Matrix m_worldTank;


	//回転させるための変数
	float rotateBall1 = 0.0f;
	float rotateBall2 = 0.0f;

	//ランダムの変数(角度、距離)
	float angle = 0.0f;
	float range = 0.0f;
	float value = 0.0f;

	//自機の座標
	DirectX::SimpleMath::Vector3 tank_pos;
	float tank_angle;

	//カメラ
	std::unique_ptr<FollowCamera> m_Camera;
};