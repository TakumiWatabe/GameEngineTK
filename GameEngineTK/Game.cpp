//
// Game.cpp
//

#include "pch.h"
#include "Game.h"

extern void ExitGame();

using namespace DirectX;
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;


Game::Game() :
    m_window(0),
    m_outputWidth(800),
    m_outputHeight(600),
    m_featureLevel(D3D_FEATURE_LEVEL_9_1)
{
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_window = window;
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateDevice();

    CreateResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */

	//初期化はここに書く
	//==========Initialization=========
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionNormal>>(m_d3dContext.Get());

	//キーボード初期化
	keyboard = std::make_unique<Keyboard>();
	//==========Effect and Input Layout==========

	m_effect  = std::make_unique<BasicEffect>(m_d3dDevice.Get());

	m_view = Matrix::CreateLookAt(Vector3(2.0f, 2.0f, 2.0f),
		Vector3::Zero, Vector3::UnitY);
	m_proj = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f,
		float(m_outputWidth) / float(m_outputHeight), 0.1f, 500.f);

	m_effect->SetView(m_view);
	m_effect->SetProjection(m_proj);

	m_effect ->SetVertexColorEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_effect ->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	m_d3dDevice->CreateInputLayout(VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_inputLayout.GetAddressOf());

	//デバックカメラ生成
	m_debugCamera = std::make_unique<DebugCamera>(m_outputWidth,m_outputHeight);

	//エフェクトファクトリー生成
	m_factory = std::make_unique<EffectFactory>(m_d3dDevice.Get());
	//テクスチャの読み込みパス指定
	m_factory->SetDirectory(L"Resources");
	//天球モデルの読み込み
	m_modelSkydorm = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources/skydorm.cmo", *m_factory);
	//地面モデルの読み込み
	m_modelGround = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources/ground200m.cmo", *m_factory);

	//球モデルの読み込み
	m_modelBall = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources/ball.cmo", *m_factory);

	//球モデルの読み込み
	m_modelTeapod = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources/teapod.cmo", *m_factory);

	//ロボット(タンク)モデルの読み込み
	m_modelTank = Model::CreateFromCMO(m_d3dDevice.Get(), L"Resources/tank.cmo", *m_factory);

	//平行移動(ティーポッド)
	Matrix transmat_Tea = Matrix::CreateTranslation(0.0f, 0, 0);
	//スケーリング
	Matrix scalemat = Matrix::CreateScale(1.0f);
	//ロール
	Matrix rotmatx = Matrix::CreateRotationX(XMConvertToRadians(0.0f));
	//ピッチ(仰角)
	Matrix rotmaty = Matrix::CreateRotationY(XMConvertToRadians(0.1f));
	//ヨー(方位角)
	Matrix rotmatz = Matrix::CreateRotationZ(XMConvertToRadians(0.0f));

	Matrix rotmaty1 = Matrix::CreateRotationY(XMConvertToRadians(0.0f));

	//回転行列(合成)
	Matrix rotmat = rotmaty * rotmatx * rotmatz;

	//ティーポッド
	for (int c = 0; c < 20; c++)
	{
		//rotateBall1 += 0.1f;
		//rotmaty1 = Matrix::CreateRotationY(XMConvertToRadians(c*20.0f + rotateBall1));
		Matrix rotmaty2 = Matrix::CreateRotationY(XMConvertToRadians(c*36.0f + rotateBall2));
		Matrix rotmat2 = rotmaty2*rotmatx;

		angle = (rand() / XM_2PI);
		range = rand() % 100;
		transmat_Tea = Matrix::CreateTranslation(cosf(angle)*range, 0, sinf(angle)*range);
		m_worldTeapod[c] = scalemat * transmat_Tea * rotmat2;
	}
	//transmat_Tea = Matrix::CreateTranslation(cosf(angle)*range, 0, sinf(angle)*range);
	tank_angle = 0.0f;
}

// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    float elapsedTime = float(timer.GetElapsedSeconds());
	//キーボードの取得
	Keyboard::State kb = keyboard->GetState();

    // TODO: Add your game logic here.
    elapsedTime;
	//毎フレーム更新処理
	m_debugCamera->Update();
	//ビュー行列を取得
	m_view = m_debugCamera->GetCameraMatrix();

	//＝＝球のワールド行列を計算＝＝//

	//スケーリング
	Matrix scalemat = Matrix::CreateScale(1.0f);

	//ロール
	Matrix rotmatx = Matrix::CreateRotationX(XMConvertToRadians(0.0f));
	//ピッチ(仰角)
	Matrix rotmaty = Matrix::CreateRotationY(XMConvertToRadians(45.0f));
	//ヨー(方位角)
	Matrix rotmatz = Matrix::CreateRotationZ(XMConvertToRadians(45.0f));

	//回転行列(合成)
	Matrix rotmat = rotmatx * rotmaty * rotmatz;

	//平行移動(内側)
	Matrix transmat = Matrix::CreateTranslation(20.0f, 0, 0);
	//平行移動(外側)
	Matrix transmat2 = Matrix::CreateTranslation(40.0f, 0, 0);
	//平行移動(ティーポッド)
	Matrix transmat_Tea = Matrix::CreateTranslation(20.0f, 0, 0);

	////(内側)
	//for (int i = 0; i < 10; i++)
	//{
	//	rotateBall1 += 0.3f;

	//	//ピッチ(仰角)
	//	Matrix rotmaty1 = Matrix::CreateRotationY(XMConvertToRadians(i*36.0f+rotateBall1));

	//	Matrix rotmat1 = rotmaty1 * rotmatx * rotmatz;
	//	m_worldBall1[i] = scalemat * transmat* rotmaty1 ;
	//}
	////(外側)
	//for (int j = 0; j < 10; j++)
	//{
	//	rotateBall2 -= 0.3f;
	//	//ピッチ(仰角)
	//	Matrix rotmaty2 = Matrix::CreateRotationY(XMConvertToRadians(j*36.0f+rotateBall2));
	//	Matrix rotmat2 = rotmatx * rotmaty2;
	//	m_worldBall2[j] = scalemat * transmat2* rotmaty2 ;
	//}
	//}
	rotateBall2 += 1.0f;



	for (int i = 0; i < 20; i++)
	{
		Matrix rotmaty2 = Matrix::CreateRotationY(XMConvertToRadians(i+rotateBall2));
		Matrix rotmat2 = rotmaty2*rotmatx;
		Matrix transmat_Tea2 = transmat_Tea;
		m_worldTeapod[i] = scalemat * rotmat2 * transmat_Tea2;

		//m_worldTeapod[i] += rotmat2 ;

	}

	//Aキーが押されていたら左旋回
	if (kb.A)
	{
		//自機の角度を回転させる
		tank_angle += 0.03f;
	}
	//Dキーが押されていたら右旋回
	if (kb.D)
	{
		//自機の角度を回転させる
		tank_angle -= 0.03f;
	}

	//Wキーが押されていたら前進
	if (kb.W)
	{
		//移動ベクトル(Z座標)
		Vector3 moveV(0, 0, -0.1f);
		//移動量ベクトルを自機の角度分回転させる
		moveV = Vector3::TransformNormal(moveV, m_worldTank);
		//自機の座標を移動させる
		tank_pos += moveV;
	}
	//Sキーが押されていたら後退
	if (kb.S)
	{
		//移動ベクトル(Z座標)
		Vector3 moveV(0, 0, 0.1f);
		//移動量ベクトルを自機の角度分回転させる
		moveV = Vector3::TransformNormal(moveV, m_worldTank);
		//自機の座標を移動させる
		tank_pos += moveV;
	}


	{//自機のワールド行列を計算
		//タンクの回転行列
		Matrix rotmat = Matrix::CreateRotationY(tank_angle);
		//タンクの平行移動
		Matrix transmat = Matrix::CreateTranslation(tank_pos);
		//平行移動行列をワールド行列にコピー
		m_worldTank = rotmat * transmat;
	}

}

// Draws the scene.
void Game::Render()
{
	uint16_t indies[] =
	{
		0,1,2,
		2,1,3
	};

	VertexPositionNormal vertices[] =
	{
		{ Vector3(-1,1,0),Vector3(0,0,1) },
		{ Vector3(-1,-1,0),Vector3(0,0,1) },
		{ Vector3(1,1,0),Vector3(0,0,1) },
		{ Vector3(1,-1,0),Vector3(0,0,1) },

	};
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    // TODO: Add your rendering code here.

	//描画処理
	//==========Drawing==========
	DirectX::CommonStates m_states(m_d3dDevice.Get());

	m_d3dContext->OMSetBlendState(m_states.Opaque(), nullptr, 0xFFFFFFFF);
	m_d3dContext->OMSetDepthStencilState(m_states.DepthNone(), 0);
	m_d3dContext->RSSetState(m_states.CullNone());

	m_effect->SetWorld(m_world);
	m_effect->SetView(m_view);
	m_effect->SetProjection(m_proj);

	m_effect ->Apply(m_d3dContext.Get());
	m_d3dContext->IASetInputLayout(m_inputLayout.Get());

	//天球モデルの描画
	m_modelSkydorm->Draw(m_d3dContext.Get(), m_states, Matrix::Identity, m_view, m_proj);

	//地面モデルの描画
	m_modelGround->Draw(m_d3dContext.Get(), m_states, Matrix::Identity, m_view, m_proj);

	//ロボット(タンク)モデルの描画
	m_modelTank->Draw(m_d3dContext.Get(), m_states, m_worldTank, m_view, m_proj);

	////球モデルの描画
	//for (int i = 0; i < 10; i++)
	//{
	//	m_modelBall->Draw(m_d3dContext.Get(), m_states, m_worldBall1[i], m_view, m_proj);
	//	m_modelBall->Draw(m_d3dContext.Get(), m_states, m_worldBall2[i], m_view, m_proj);
	//}

	////ティーポッドモデルの描画
	//for (int i = 0; i < 20; i++)
	//{
	//	m_modelTeapod->Draw(m_d3dContext.Get(), m_states, m_worldTeapod[i], m_view, m_proj);
	//}

	m_batch->Begin();
	//m_batch->DrawLine
	//(
	//	VertexPositionColor(
	//		Vector3(0,0,0),
	//		Color(1,1,1)),
	//	VertexPositionColor(
	//		Vector3(800, 600, 0),
	//		Color(1, 1, 1))
	//	);
	VertexPositionColor v1(Vector3(0.f, 0.5f, 0.5f), Colors::Yellow);
	VertexPositionColor v2(Vector3(0.5f, -0.5f, 0.5f), Colors::Yellow);
	VertexPositionColor v3(Vector3(-0.5f, -0.5f, 0.5f), Colors::Yellow);

	//VertexPositionColor v1(Vector3(0, 500, 0), Colors::Yellow);
	//VertexPositionColor v2(Vector3(500, 0, 0), Colors::Yellow);
	//VertexPositionColor v3(Vector3(0, 0., 0), Colors::Yellow);

	//m_batch->DrawTriangle(v1, v2, v3);

	m_batch->DrawIndexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, indies, 6, vertices, 4);
	m_batch->End();
    Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    // Clear the views.
    m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::CornflowerBlue);
    m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

    // Set the viewport.
    CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
    m_d3dContext->RSSetViewports(1, &viewport);
}

// Presents the back buffer contents to the screen.
void Game::Present()
{
    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    HRESULT hr = m_swapChain->Present(1, 0);

    // If the device was reset we must completely reinitialize the renderer.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        OnDeviceLost();
    }
    else
    {
        DX::ThrowIfFailed(hr);
    }
}

// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowSizeChanged(int width, int height)
{
    m_outputWidth = std::max(width, 1);
    m_outputHeight = std::max(height, 1);

    CreateResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}

// These are the resources that depend on the device.
void Game::CreateDevice()
{
    UINT creationFlags = 0;

#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    static const D3D_FEATURE_LEVEL featureLevels [] =
    {
        // TODO: Modify for supported Direct3D feature levels (see code below related to 11.1 fallback handling).
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };

    // Create the DX11 API device object, and get a corresponding context.
    HRESULT hr = D3D11CreateDevice(
        nullptr,                                // specify nullptr to use the default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        featureLevels,
        _countof(featureLevels),
        D3D11_SDK_VERSION,
        m_d3dDevice.ReleaseAndGetAddressOf(),   // returns the Direct3D device created
        &m_featureLevel,                        // returns feature level of device created
        m_d3dContext.ReleaseAndGetAddressOf()   // returns the device immediate context
        );

    if (hr == E_INVALIDARG)
    {
        // DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it.
        hr = D3D11CreateDevice(nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            creationFlags,
            &featureLevels[1],
            _countof(featureLevels) - 1,
            D3D11_SDK_VERSION,
            m_d3dDevice.ReleaseAndGetAddressOf(),
            &m_featureLevel,
            m_d3dContext.ReleaseAndGetAddressOf()
            );
    }

    DX::ThrowIfFailed(hr);

#ifndef NDEBUG
    ComPtr<ID3D11Debug> d3dDebug;
    if (SUCCEEDED(m_d3dDevice.As(&d3dDebug)))
    {
        ComPtr<ID3D11InfoQueue> d3dInfoQueue;
        if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
        {
#ifdef _DEBUG
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
            D3D11_MESSAGE_ID hide [] =
            {
                D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
                // TODO: Add more message IDs here as needed.
            };
            D3D11_INFO_QUEUE_FILTER filter = {};
            filter.DenyList.NumIDs = _countof(hide);
            filter.DenyList.pIDList = hide;
            d3dInfoQueue->AddStorageFilterEntries(&filter);
        }
    }
#endif

    // DirectX 11.1 if present
    if (SUCCEEDED(m_d3dDevice.As(&m_d3dDevice1)))
        (void)m_d3dContext.As(&m_d3dContext1);

    // TODO: Initialize device dependent objects here (independent of window size).
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateResources()
{
    // Clear the previous window size specific context.
    ID3D11RenderTargetView* nullViews [] = { nullptr };
    m_d3dContext->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
    m_renderTargetView.Reset();
    m_depthStencilView.Reset();
    m_d3dContext->Flush();

    UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
    UINT backBufferHeight = static_cast<UINT>(m_outputHeight);
    DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
    DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    UINT backBufferCount = 2;

    // If the swap chain already exists, resize it, otherwise create one.
    if (m_swapChain)
    {
        HRESULT hr = m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
        {
            // If the device was removed for any reason, a new device and swap chain will need to be created.
            OnDeviceLost();

            // Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
            // and correctly set up the new device.
            return;
        }
        else
        {
            DX::ThrowIfFailed(hr);
        }
    }
    else
    {
        // First, retrieve the underlying DXGI Device from the D3D Device.
        ComPtr<IDXGIDevice1> dxgiDevice;
        DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

        // Identify the physical adapter (GPU or card) this device is running on.
        ComPtr<IDXGIAdapter> dxgiAdapter;
        DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

        // And obtain the factory object that created it.
        ComPtr<IDXGIFactory1> dxgiFactory;
        DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

        ComPtr<IDXGIFactory2> dxgiFactory2;
        if (SUCCEEDED(dxgiFactory.As(&dxgiFactory2)))
        {
            // DirectX 11.1 or later

            // Create a descriptor for the swap chain.
            DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
            swapChainDesc.Width = backBufferWidth;
            swapChainDesc.Height = backBufferHeight;
            swapChainDesc.Format = backBufferFormat;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.BufferCount = backBufferCount;

            DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = { 0 };
            fsSwapChainDesc.Windowed = TRUE;

            // Create a SwapChain from a Win32 window.
            DX::ThrowIfFailed(dxgiFactory2->CreateSwapChainForHwnd(
                m_d3dDevice.Get(),
                m_window,
                &swapChainDesc,
                &fsSwapChainDesc,
                nullptr,
                m_swapChain1.ReleaseAndGetAddressOf()
                ));

            DX::ThrowIfFailed(m_swapChain1.As(&m_swapChain));
        }
        else
        {
            DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
            swapChainDesc.BufferCount = backBufferCount;
            swapChainDesc.BufferDesc.Width = backBufferWidth;
            swapChainDesc.BufferDesc.Height = backBufferHeight;
            swapChainDesc.BufferDesc.Format = backBufferFormat;
            swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.OutputWindow = m_window;
            swapChainDesc.SampleDesc.Count = 1;
            swapChainDesc.SampleDesc.Quality = 0;
            swapChainDesc.Windowed = TRUE;

            DX::ThrowIfFailed(dxgiFactory->CreateSwapChain(m_d3dDevice.Get(), &swapChainDesc, m_swapChain.ReleaseAndGetAddressOf()));
        }

        // This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
        DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
    }

    // Obtain the backbuffer for this window which will be the final 3D rendertarget.
    ComPtr<ID3D11Texture2D> backBuffer;
    DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

    // Create a view interface on the rendertarget to use on bind.
    DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));

    // Allocate a 2-D surface as the depth/stencil buffer and
    // create a DepthStencil view on this surface to use on bind.
    CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

    ComPtr<ID3D11Texture2D> depthStencil;
    DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));

    // TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

    m_depthStencilView.Reset();
    m_renderTargetView.Reset();
    m_swapChain1.Reset();
    m_swapChain.Reset();
    m_d3dContext1.Reset();
    m_d3dContext.Reset();
    m_d3dDevice1.Reset();
    m_d3dDevice.Reset();

    CreateDevice();

    CreateResources();
}