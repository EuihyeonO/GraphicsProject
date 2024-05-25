#include "EngineBase.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "SphereRenderer.h"
#include "ScreenRenderer.h"

#include "BoxRenderer.h"
#include "ZeldaRenderer.h"
#include "PostProcess.h"
#include "BloomPostProcess.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

EngineBase::EngineBase()
{
}

EngineBase::~EngineBase()
{
}

ATOM EngineBase::MyRegisterClass(HINSTANCE _hInstance)
{
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = &EngineBase::WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = _hInstance;
    wcex.hIcon = nullptr; //LoadIcon(_hInstance, MAKEINTRESOURCE(IDI_GRAPHICSPROJECT));
    wcex.hCursor = nullptr; //LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr; //MAKEINTRESOURCEW(IDC_GRAPHICSPROJECT);
    wcex.lpszClassName = L"WindowDefault";
    wcex.hIconSm = nullptr; //LoadIcon(wcex._hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

LRESULT CALLBACK EngineBase::WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(_hWnd, _message, _wParam, _lParam))
        return true;

    switch (_message) {
    case WM_SIZE:
        break;
    case WM_SYSCOMMAND:
        if ((_wParam & 0xfff0) == SC_KEYMENU)
            return 0;
        break;
    case WM_MOUSEMOVE:
        break;
    case WM_LBUTTONUP:
        break;
    case WM_RBUTTONUP:
        break;
    case WM_KEYDOWN:
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }

    return ::DefWindowProc(_hWnd, _message, _wParam, _lParam);
}

BOOL EngineBase::InitInstance(HINSTANCE _hInstance, int nCmdShow)
{
    hInstance = _hInstance;

    RECT Rt = { 0, 0, WindowWidth, WindowHeight };

    hWnd = CreateWindowW(L"WindowDefault", L"GraphicsProject", WS_OVERLAPPEDWINDOW,
        150, 50, Rt.right - Rt.left, Rt.bottom - Rt.top, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

void EngineBase::Loop()
{
    while (WM_QUIT != msg.message)
    {
        if ((PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            float CurDelta = ImGui::GetIO().DeltaTime;

            //아래 코드 정리하자
            DirectX::SimpleMath::Matrix RotMat = DirectX::SimpleMath::Matrix::CreateRotationX(CameraRotation.x) *
                                                 DirectX::SimpleMath::Matrix::CreateRotationY(CameraRotation.y) *
                                                 DirectX::SimpleMath::Matrix::CreateRotationZ(CameraRotation.z);
            
            EyePos = CameraTranslation;
            EyeDir = DirectX::XMVector3Transform({ 0.0f, 0.0f, 1.0f }, RotMat);
            UpDir = DirectX::XMVector3TransformCoord({ 0.0f, 1.0f, 0.0f }, RotMat);

            ViewMat = DirectX::XMMatrixLookToLH(EyePos, EyeDir, UpDir);

            WorldLight.EyeWorld = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3(0.0f), ViewMat.Invert());
           
            ImguiUpdate();

            Update(CurDelta);
            Render(CurDelta);

            ImGui::Render();

            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

            SwapChain->Present(1, 0);
        }
    }
}

WPARAM EngineBase::End()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    return msg.wParam;
}

void EngineBase::CreateAllShader()
{
    {
        std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 * 2, D3D11_INPUT_PER_VERTEX_DATA, 0},
        };

        BOOL Result = CreateVertexShader(L"MeshLightVertexShader.hlsl", inputElements);

        if (Result == FALSE)
        {
            std::cout << "MeshLightVertexShader Create Failed" << std::endl;
            return;
        }
    }

    {
        std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 * 2, D3D11_INPUT_PER_VERTEX_DATA, 0},
        };

        BOOL Result = CreateVertexShader(L"CubeMapVertexShader.hlsl", inputElements);

        if (Result == FALSE)
        {
            std::cout << "CubeMapVertexShader Create Failed" << std::endl;
            return;
        }
    }

    {
        std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 * 2, D3D11_INPUT_PER_VERTEX_DATA, 0},
        };

        BOOL Result = CreateVertexShader(L"EnvMapVertexShader.hlsl", inputElements);

        if (Result == FALSE)
        {
            std::cout << "EnvMapVertexShader Create Failed" << std::endl;
            return;
        }
    }

    {
        std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 * 2, D3D11_INPUT_PER_VERTEX_DATA, 0},
        };

        BOOL Result = CreateVertexShader(L"MeshVertexShader.hlsl", inputElements);

        if (Result == FALSE)
        {
            std::cout << "MeshVertexShader Create Failed" << std::endl;
            return;
        }
    }

    {
        std::vector<D3D11_INPUT_ELEMENT_DESC> inputElements =
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 3 * 2, D3D11_INPUT_PER_VERTEX_DATA, 0},
        };

        BOOL Result = CreateVertexShader(L"BloomVertexShader.hlsl", inputElements);

        if (Result == FALSE)
        {
            std::cout << "BloomVertexShader Create Failed" << std::endl;
            return;
        }
    }

//픽셀쉐이더

    {
        BOOL Result = CreatePixelShader(L"MeshLightPixelShader.hlsl");

        if (Result == FALSE)
        {
            std::cout << "MeshLightPixelShader Create Failed" << std::endl;
            return;
        }
    }

    {
        BOOL Result = CreatePixelShader(L"CubeMapPixelShader.hlsl");

        if (Result == FALSE)
        {
            std::cout << "CubeMapPixelShader Create Failed" << std::endl;
            return;
        }
    }

    {
        BOOL Result = CreatePixelShader(L"EnvMapPixelShader.hlsl");

        if (Result == FALSE)
        {
            std::cout << "EnvMapPixelShader Create Failed" << std::endl;
            return;
        }
    }

    {
        BOOL Result = CreatePixelShader(L"MeshPixelShader.hlsl");

        if (Result == FALSE)
        {
            std::cout << "MeshPixelShader Create Failed" << std::endl;
            return;
        }
    }

    {
        BOOL Result = CreatePixelShader(L"BloomPixelShader.hlsl");

        if (Result == FALSE)
        {
            std::cout << "BloomPixelShader Create Failed" << std::endl;
            return;
        }
    }
}

void EngineBase::LoadAllTexture()
{
    ResourceManager::LoadTexture("skybox.dds", ETextureType::CubeMap);
    ResourceManager::LoadTexture("BoxTexture.png", ETextureType::Diffuse);
}

void EngineBase::Update(float _DeltaTime)
{
    for (std::shared_ptr<Renderer> Renderer : Renderers)
    {
        Renderer->Update(_DeltaTime);
    }

    for (std::shared_ptr<Renderer> Renderer : Renderers)
    {
        Renderer->ConstantBufferUpdate();
    }
}

void EngineBase::Render(float _DeltaTime)
{
    float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    Context->ClearRenderTargetView(DoubleBufferRTV.Get(), clearColor);
    Context->ClearDepthStencilView(DepthStencilView.Get(),
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    Context->OMSetRenderTargets(1, DoubleBufferRTV.GetAddressOf(), DepthStencilView.Get());
    Context->OMSetDepthStencilState(DepthStencilState.Get(), 0);
    
    if (isWireFrame == false)
    {
        Context->RSSetState(SolidRasterizerState.Get());
    }
    else
    {
        Context->RSSetState(WireRasterizerState.Get());
    }

    for (std::shared_ptr<Renderer> Renderer : Renderers)
    {
        Renderer->Render(_DeltaTime);
    }

    Context->ClearRenderTargetView(BackBufferRTV.Get(), clearColor);
    Context->OMSetRenderTargets(1, BackBufferRTV.GetAddressOf(), DepthStencilView.Get());

    for (std::shared_ptr<PostProcess> PostProcess : PostProcesses)
    {
        PostProcess->SetTexture(DoubleBufferSRV);
        PostProcess->Render(_DeltaTime);
    }
}

void EngineBase::SetLight()
{
    WorldLight.Lights[0].Direction = { 0.0f, 0.0f, 1.0f };

    WorldLight.Lights[1].Position = { -2.0f, 0.0f, 1.0f };
    WorldLight.Lights[1].Direction = { 0.0f, 1.0f, 0.0f };
    WorldLight.Lights[1].Strength = { 2.0f, 0.0f, 0.0f };
    WorldLight.Lights[1].FallOffStart = 1.0f;
    WorldLight.Lights[1].FallOffEnd = 5.0f;

    WorldLight.Lights[2].Position = { 0.0f, 2.0f, 1.0f };
    WorldLight.Lights[2].Direction = { 0.0f, -1.0f, 0.0f };
    WorldLight.Lights[2].Strength = { 0.0f, 2.0f, 0.0f };
    WorldLight.Lights[2].FallOffStart = 1.0f;
    WorldLight.Lights[2].FallOffEnd = 2.0f;
    WorldLight.Lights[2].SpotPower = 10.0f;
}

BOOL EngineBase::ImguiInit()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    if(!ImGui_ImplWin32_Init(hWnd))
    {
        std::cout << "IMGUI Win32 Init Failed" << std::endl;
        return FALSE;
    }

    if (!ImGui_ImplDX11_Init(Device.Get(), Context.Get()))
    {
        std::cout << "IMGUI Dx11 Init Failed" << std::endl;
        return FALSE;
    }

    AddGUIFunction([this] {ImGui::Checkbox("WireFrame",&isWireFrame);});

    AddGUIFunction([this] {ImGui::SliderFloat3("CameraPos", &CameraTranslation.x, -10.0f, 10.0f); });
    AddGUIFunction([this] {ImGui::SliderFloat3("CameraRot", &CameraRotation.x, -3.14f, 3.14f); });
    AddGUIFunction([this] {ImGui::SliderFloat3("PointLightPos", &WorldLight.Lights[1].Position.x, -10.0f, 10.0f); });
    AddGUIFunction([this] {ImGui::SliderFloat("SpotPower", &WorldLight.Lights[2].SpotPower, 0.0f, 100.0f); });

    return TRUE;
}

void EngineBase::ImguiUpdate()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Hello, world!"); 
    

    for (const std::function<void()> _Func : GUIFunctions)
    {
        _Func();
    }


    ImGui::End();
}

BOOL EngineBase::WindowInit(HINSTANCE _hInstance)
{
    hInstance = _hInstance;

    MyRegisterClass(hInstance);

    if (!InitInstance(hInstance, SW_SHOW))
    {
        std::cout << "InitInstance() is Failed!" << std::endl;
        return FALSE;
    }

    return TRUE;
}

BOOL EngineBase::DirectXInit()
{
    if (!CreateDevice())
    {
        std::cout << "CreateDevice() is Failed!" << std::endl;
        return FALSE;
    }
    
    if (!CreateSwapChain())
    {
        std::cout << "CreateSwapChain() is Failed!" << std::endl;
        return FALSE;
    }

    if (!CreateDoubleBuffer())
    {
        std::cout << "CreateDoubleBuffer() is Failed!" << std::endl;
        return FALSE;
    }
    
    if (!CreateRasterizerState())
    {
        std::cout << "CreateRasterizerState() is Failed!" << std::endl;
        return FALSE;
    }
    
    if (!CreateDepthStencil())
    {
        std::cout << "CreateDepthStencil() is Failed!" << std::endl;
        return FALSE;
    }
    
    SetViewport();

    return TRUE;
}

BOOL EngineBase::CreateDevice()
{
    //하드웨어 드라이버를 사용할건가 소프트웨어 드라이버를 사용할건가
    const D3D_DRIVER_TYPE DriverType = D3D_DRIVER_TYPE_HARDWARE;

    //디버그 기능을 사용할 것인가
    UINT CreateDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    //다이렉트X의 버전 목록 (만약 컴퓨터에 버전이 없다면, 더 낮은 버전으로 Device 생성을 시도
    const D3D_FEATURE_LEVEL featureLevels[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_9_3 };
    
    //생성된 Device의 다이렉트X 버전을 저장
    D3D_FEATURE_LEVEL featureLevel;

    HRESULT Result = D3D11CreateDevice(
        nullptr,                  // nullptr이면 기본 어댑터를 사용
        DriverType,               // 어떤 드라이버를 사용하여 Device를 만들 것인가
        0,                        // 소프트웨어 드라이버를 사용할 것이라면, 어떤걸 사용할지 선택하는 옵션인듯
        CreateDeviceFlags,        // 플래그
        featureLevels,            // 다이렉트X 버전 목록 배열
        ARRAYSIZE(featureLevels), // 위의 배열의 사이즈
        D3D11_SDK_VERSION,        // 무조건 D3D11_SDK_VERSION 쓰라고 써있네
        &Device,                  // 생성된 디바이스를 저장
        &featureLevel,            // 생성된 디바이스의 DirectX 버전을 저장
        &Context                  // 생성된 디바이스의 Context를 저장
    );

    if (Result != S_OK)
    {
        return FALSE;
    }

    return TRUE;
}

BOOL EngineBase::CreateSwapChain()
{
    HRESULT Result = S_OK;

    Microsoft::WRL::ComPtr<IDXGIDevice> DXGIDevice;
    Result = Device.As(&DXGIDevice);

    Microsoft::WRL::ComPtr<IDXGIAdapter> DXGIAdapter;
    Result = DXGIDevice->GetAdapter(&DXGIAdapter);

    Microsoft::WRL::ComPtr<IDXGIFactory> DXGIFactory;
    Result = DXGIAdapter->GetParent(IID_PPV_ARGS(&DXGIFactory));

    SwapChain.Reset();

    DXGI_SWAP_CHAIN_DESC SD;
    ZeroMemory(&SD, sizeof(SD));
    SD.BufferDesc.Width = (UINT)WindowWidth;                 // 백버퍼 사이즈 (너비)
    SD.BufferDesc.Height = (UINT)WindowHeight;               // 백버퍼 사이즈 (높이)
    SD.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 색상 포맷
    SD.BufferCount = 2;                                // 백버퍼 개수
    SD.BufferDesc.RefreshRate.Numerator = 60;          // 갱신률 (분자)
    SD.BufferDesc.RefreshRate.Denominator = 1;         // 갱신률 (분모)
    SD.BufferUsage = DXGI_USAGE_SHADER_INPUT | DXGI_USAGE_RENDER_TARGET_OUTPUT;  // 스왑체인을 어떻게 쓸 것인가
    SD.OutputWindow = hWnd;                            // 스왑체인이 사용될 윈도우
    SD.Windowed = TRUE;                                // 창모드, 전체모드 
    SD.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // 창모드, 전체모드 전환을 허용할 것인가
    SD.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;  

    SD.SampleDesc.Count = 1;
    SD.SampleDesc.Quality = 0;

    Result = DXGIFactory->CreateSwapChain(Device.Get(), &SD, SwapChain.GetAddressOf());

    if (Result != S_OK)
    {
        return FALSE;
    }

    //백버퍼의 렌더타겟 뷰, 쉐이더 리소스 뷰 생성
    Microsoft::WRL::ComPtr<ID3D11Texture2D> BackBuffer;
    SwapChain->GetBuffer(0, IID_PPV_ARGS(BackBuffer.GetAddressOf()));

    if (BackBuffer != nullptr) 
    {
        Device->CreateRenderTargetView(BackBuffer.Get(), nullptr, BackBufferRTV.GetAddressOf());
        Device->CreateShaderResourceView(BackBuffer.Get(), nullptr, BackBufferSRV.GetAddressOf());
    }
    else 
    {
        std::cout << "CreateRenderTargetView() failed!" << std::endl;
        return FALSE;
    }

    return TRUE;
}

void EngineBase::SetViewport()
{
    ZeroMemory(&ScreenViewPort, sizeof(D3D11_VIEWPORT));
    ScreenViewPort.TopLeftX = 0;
    ScreenViewPort.TopLeftY = 0;
    ScreenViewPort.Width = float(WindowWidth);
    ScreenViewPort.Height = float(WindowHeight);
    ScreenViewPort.MinDepth = 0.0f;
    ScreenViewPort.MaxDepth = 1.0f;
    
    Context->RSSetViewports(1, &ScreenViewPort);
}

BOOL EngineBase::CreateRasterizerState()
{
    {
        D3D11_RASTERIZER_DESC RD;
        ZeroMemory(&RD, sizeof(D3D11_RASTERIZER_DESC));
        RD.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
        RD.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
        RD.FrontCounterClockwise = false;

        HRESULT Result = Device->CreateRasterizerState(&RD, &SolidRasterizerState);
        if (Result != S_OK)
        {
            return FALSE;
        }
    }

    {
        D3D11_RASTERIZER_DESC RD;
        ZeroMemory(&RD, sizeof(D3D11_RASTERIZER_DESC));
        RD.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
        RD.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
        RD.FrontCounterClockwise = false;

        HRESULT Result = Device->CreateRasterizerState(&RD, &WireRasterizerState);
        if (Result != S_OK)
        {
            return FALSE;
        }
    }
    
    return TRUE;
}

BOOL EngineBase::CreateVertexShader(const std::wstring& _ShaderFileName, std::vector<D3D11_INPUT_ELEMENT_DESC> _InputElement)
{
    if (VertexShaders.find(_ShaderFileName) != VertexShaders.end())
    {
        return TRUE;
    }

    Microsoft::WRL::ComPtr<ID3DBlob> ShaderBlob;
    Microsoft::WRL::ComPtr<ID3DBlob> ErrorBlob;

    UINT CompileFlag = 0;
#if defined(DEBUG) || defined(_DEBUG)
    CompileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    HRESULT Result = D3DCompileFromFile(_ShaderFileName.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", CompileFlag, 0, &ShaderBlob, &ErrorBlob);

    if (Result != S_OK) 
    {
        // 파일이 없을 경우
        if ((Result & D3D11_ERROR_FILE_NOT_FOUND) != 0) 
        {
            std::cout << "File not found." << std::endl;
        }

        // 에러 메시지가 있으면 출력
        if (ErrorBlob)
        {
            std::cout << "Shader compile error\n" << (char*)ErrorBlob->GetBufferPointer() << std::endl;
        }

        return FALSE;
    }

    Microsoft::WRL::ComPtr<ID3D11VertexShader> NewVertextShader;

    Result = 
        EngineBase::GetInstance().GetDevice()->CreateVertexShader(ShaderBlob->GetBufferPointer(), ShaderBlob->GetBufferSize(), NULL,
        &NewVertextShader);

    if (Result != S_OK)
    {
        std::cout << "CreateVertexShader() failed" << std::endl;
        return FALSE;
    }

    Microsoft::WRL::ComPtr<ID3D11InputLayout> _InputLayOut;
    if (!CreateInputLayOut(_InputElement, _InputLayOut, ShaderBlob))
    {
        return FALSE;
    }

    VertexShaders.insert({ _ShaderFileName, {NewVertextShader, _InputLayOut} });

    return TRUE;
}

BOOL EngineBase::CreateInputLayOut(std::vector<D3D11_INPUT_ELEMENT_DESC> _InputElement, Microsoft::WRL::ComPtr<ID3D11InputLayout>& _InputLayOut, Microsoft::WRL::ComPtr<ID3DBlob> _ShaderBlob)
{
    HRESULT Result =
    EngineBase::GetInstance().GetDevice()->CreateInputLayout(_InputElement.data(), UINT(_InputElement.size()),
        _ShaderBlob->GetBufferPointer(), _ShaderBlob->GetBufferSize(),
        &_InputLayOut);

    if (Result != S_OK)
    {
        std::cout << "CreateInputLayOut() failed" << std::endl;
        return FALSE;
    }

    return TRUE;
}

BOOL EngineBase::CreatePixelShader(const std::wstring& _ShaderFileName)
{
    if (PixelShaders.find(_ShaderFileName) != PixelShaders.end())
    {
        return TRUE;
    }

    Microsoft::WRL::ComPtr<ID3DBlob> ShaderBlob;
    Microsoft::WRL::ComPtr<ID3DBlob> ErrorBlob;

    UINT CompileFlag = 0;
#if defined(DEBUG) || defined(_DEBUG)
    CompileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    HRESULT Result = D3DCompileFromFile(_ShaderFileName.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", CompileFlag, 0, &ShaderBlob, &ErrorBlob);
    
    if (Result != S_OK)
    {
        // 파일이 없을 경우
        if ((Result & D3D11_ERROR_FILE_NOT_FOUND) != 0)
        {
            std::cout << "File not found." << std::endl;
        }

        // 에러 메시지가 있으면 출력
        if (ErrorBlob)
        {
            std::cout << "Shader compile error\n" << (char*)ErrorBlob->GetBufferPointer() << std::endl;
        }

        return FALSE;
    }

    Microsoft::WRL::ComPtr<ID3D11PixelShader> NewPixelShader;

    Result = 
    EngineBase::GetInstance().GetDevice()->CreatePixelShader(ShaderBlob->GetBufferPointer(), ShaderBlob->GetBufferSize(), NULL,
        &NewPixelShader);

    if (Result != S_OK)
    {
        std::cout << "CreatePixelShader() failed" << std::endl;
        return FALSE;
    }

    PixelShaders.insert({ _ShaderFileName, NewPixelShader});

    return TRUE;
}

BOOL EngineBase::CreateDoubleBuffer()
{
    Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture;

    D3D11_TEXTURE2D_DESC txtDesc;
    ZeroMemory(&txtDesc, sizeof(txtDesc));
    txtDesc.Width = WindowWidth;
    txtDesc.Height = WindowHeight;
    txtDesc.MipLevels = txtDesc.ArraySize = 1;
    txtDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // 이미지 처리용도
    txtDesc.SampleDesc.Count = 1;
    txtDesc.Usage = D3D11_USAGE_DEFAULT;
    txtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS;
    txtDesc.MiscFlags = 0;
    txtDesc.CPUAccessFlags = 0;
    
    D3D11_RENDER_TARGET_VIEW_DESC viewDesc;
    viewDesc.Format = txtDesc.Format;
    viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    viewDesc.Texture2D.MipSlice = 0;

    HRESULT Result;
    Result = Device->CreateTexture2D(&txtDesc, NULL, Texture.GetAddressOf());
    if (Result != S_OK)
    {
        std::cout << "CreateTexture2D() failed" << std::endl;
        return FALSE;
    }

    Result = Device->CreateRenderTargetView(Texture.Get(), &viewDesc, DoubleBufferRTV.GetAddressOf());
    if (Result != S_OK)
    {
        std::cout << "CreateRenderTargetView() failed" << std::endl;
        return FALSE;
    }

    Result = Device->CreateShaderResourceView(Texture.Get(), nullptr, DoubleBufferSRV.GetAddressOf());
    if (Result != S_OK)
    {
        std::cout << "CreateShaderResourceView() failed" << std::endl;
        return FALSE;
    }

    return TRUE;
}

void EngineBase::AddRenderer(std::shared_ptr<Renderer> _NewRenderer)
{
    Renderers.push_back(_NewRenderer);
}

void EngineBase::CreatePostProcess()
{
    std::shared_ptr<BloomPostProcess> Bloom = std::make_shared<BloomPostProcess>();
    Bloom->Init();

    PostProcesses.push_back(Bloom);

}

BOOL EngineBase::CreateDepthStencil()
{
    D3D11_TEXTURE2D_DESC DepthStencilBufferDesc;

    DepthStencilBufferDesc.Width = WindowWidth;
    DepthStencilBufferDesc.Height = WindowHeight;
    DepthStencilBufferDesc.MipLevels = 1;
    DepthStencilBufferDesc.ArraySize = 1;

    DepthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
   
    DepthStencilBufferDesc.SampleDesc.Count = 1; 
    DepthStencilBufferDesc.SampleDesc.Quality = 0;

    DepthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    DepthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    DepthStencilBufferDesc.CPUAccessFlags = 0;
    DepthStencilBufferDesc.MiscFlags = 0;

    HRESULT Result = Device->CreateTexture2D(&DepthStencilBufferDesc, 0, DepthStencilBuffer.GetAddressOf());

    if (Result != S_OK)
    {
        return FALSE;
    }

    Result = Device->CreateDepthStencilView(DepthStencilBuffer.Get(), 0, &DepthStencilView);
    
    if (Result != S_OK)
    {
        return FALSE;
    }

    D3D11_DEPTH_STENCIL_DESC DepthStencilDesc;
    ZeroMemory(&DepthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
    DepthStencilDesc.DepthEnable = true;
    DepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
    DepthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

    Result = Device->CreateDepthStencilState(&DepthStencilDesc, DepthStencilState.GetAddressOf());

    if (Result != S_OK)
    {
        return FALSE;
    }

    return TRUE;
}

BOOL EngineBase::CreateSampler()
{
    {
        Microsoft::WRL::ComPtr<ID3D11SamplerState> NewSampler;

        // Texture sampler 만들기
        D3D11_SAMPLER_DESC SamplerDesc;
        ZeroMemory(&SamplerDesc, sizeof(SamplerDesc));

        SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        SamplerDesc.MinLOD = 0;
        SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

        // Create the Sample State
        HRESULT Result = EngineBase::GetInstance().GetDevice()->CreateSamplerState(&SamplerDesc, NewSampler.GetAddressOf());
        if (Result != S_OK)
        {
            std::cout << "CreateSamplerState failed : LINEARWRAP" << std::endl;
            return FALSE;
        }

        Samplers.insert({ "LINEARWRAP", NewSampler });
    }

    {
        Microsoft::WRL::ComPtr<ID3D11SamplerState> NewSampler;

        // Texture sampler 만들기
        D3D11_SAMPLER_DESC SamplerDesc;
        ZeroMemory(&SamplerDesc, sizeof(SamplerDesc));

        SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        SamplerDesc.MinLOD = 0;
        SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

        // Create the Sample State
        HRESULT Result = EngineBase::GetInstance().GetDevice()->CreateSamplerState(&SamplerDesc, NewSampler.GetAddressOf());
        if (Result != S_OK)
        {
            std::cout << "CreateSamplerState failed : LINEARCLAMP" << std::endl;
            return FALSE;
        }

        Samplers.insert({ "LINEARCLAMP", NewSampler });
    }

    return TRUE;
}

BOOL EngineBase::Init(HINSTANCE _hInstance, int _Width, int _Height)
{
    WindowWidth = _Width;
    WindowHeight = _Height;

    if (!WindowInit(_hInstance))
    {
        return FALSE;
    }

    if (!DirectXInit())
    {
        return FALSE;
    }

    if (!ImguiInit())
    {
        return FALSE;
    }

    CreateAllShader();
    LoadAllTexture();
    CreateSampler();

    CreatePostProcess();

    SetLight();

    ResourceManager::Load("zeldaPosed001.fbx");

    Renderer::CreateRenderer<ZeldaRenderer>();
    Renderer::CreateRenderer<BoxRenderer>();
    Renderer::CreateRenderer<SphereRenderer>();

    return TRUE;
}
