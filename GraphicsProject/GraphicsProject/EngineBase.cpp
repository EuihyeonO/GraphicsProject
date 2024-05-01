#include "EngineBase.h"

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
    while(WM_QUIT != msg.message)
    {
        if ((PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}

WPARAM EngineBase::End()
{
    return msg.wParam;
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

    //멀티샘플링 안티에일리어싱 (MSAA)
    Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &NumQualityLevels);
    if (NumQualityLevels <= 0)
    {
        std::cout << "MSAA not supported!" << std::endl;
    }

    DXGI_SWAP_CHAIN_DESC SD;
    ZeroMemory(&SD, sizeof(SD));
    SD.BufferDesc.Width = (UINT)WindowWidth;                 // 백버퍼 사이즈 (너비)
    SD.BufferDesc.Height = (UINT)WindowHeight;               // 백버퍼 사이즈 (높이)
    SD.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 색상 포맷
    SD.BufferCount = 2;                                // 백버퍼 개수
    SD.BufferDesc.RefreshRate.Numerator = 60;          // 갱신률 (분자)
    SD.BufferDesc.RefreshRate.Denominator = 1;         // 갱신률 (분모)
    SD.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // 스왑체인을 어떻게 쓸 것인가
    SD.OutputWindow = hWnd;                            // 스왑체인이 사용될 윈도우
    SD.Windowed = TRUE;                                // 창모드, 전체모드 
    SD.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // 창모드, 전체모드 전환을 허용할 것인가
    SD.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;  

    SD.SampleDesc.Count = 4;
    SD.SampleDesc.Quality = NumQualityLevels - 1;

    Result = DXGIFactory->CreateSwapChain(Device.Get(), &SD, SwapChain.GetAddressOf());

    if (Result != S_OK)
    {
        return FALSE;
    }

    //백버퍼의 렌더타겟 뷰 생성
    ID3D11Texture2D* BackBuffer;
    SwapChain->GetBuffer(0, IID_PPV_ARGS(&BackBuffer));

    if (BackBuffer) 
    {
        Device->CreateRenderTargetView(BackBuffer, NULL, &RenderTargetView);
        BackBuffer->Release();
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
    D3D11_RASTERIZER_DESC RD;
    ZeroMemory(&RD, sizeof(D3D11_RASTERIZER_DESC));
    RD.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    RD.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
    RD.FrontCounterClockwise = false;

    HRESULT Result = Device->CreateRasterizerState(&RD, &RasterizerState);
    if (Result != S_OK)
    {
        return FALSE;
    }
    
    return TRUE;
}

BOOL EngineBase::CreateVertexShader(const std::wstring& _ShaderFileName, std::vector<D3D11_INPUT_ELEMENT_DESC> _InputElement)
{
    if (VertexShaders.find(_ShaderFileName) != VertexShaders.end())
    {
        std::cout << "Don't try to Create existed VertexShader" << std::endl;
        return TRUE;
    }

    Microsoft::WRL::ComPtr<ID3DBlob> ShaderBlob;
    Microsoft::WRL::ComPtr<ID3DBlob> ErrorBlob;

    HRESULT Result =
        D3DCompileFromFile(_ShaderFileName.c_str(), 0, 0, "main", "vs_5_0", 0, 0, &ShaderBlob, &ErrorBlob);

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

BOOL EngineBase::CreateInputLayOut(std::vector<D3D11_INPUT_ELEMENT_DESC> _InputElement, Microsoft::WRL::ComPtr<ID3D11InputLayout> _InputLayOut, Microsoft::WRL::ComPtr<ID3DBlob> _ShaderBlob)
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
    Microsoft::WRL::ComPtr<ID3DBlob> ShaderBlob;
    Microsoft::WRL::ComPtr<ID3DBlob> ErrorBlob;

    // 주의: 쉐이더의 시작점의 이름이 "main"인 함수로 지정
    HRESULT Result =
        D3DCompileFromFile(_ShaderFileName.c_str(), 0, 0, "main", "ps_5_0", 0, 0, &ShaderBlob, &ErrorBlob);
    
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

    PixelShaders.insert({ _ShaderFileName, NewPixelShader });

    return TRUE;
}

BOOL EngineBase::CreateDepthStencil()
{
    D3D11_TEXTURE2D_DESC DepthStencilBufferDesc;

    DepthStencilBufferDesc.Width = WindowWidth;
    DepthStencilBufferDesc.Height = WindowHeight;
    DepthStencilBufferDesc.MipLevels = 1;
    DepthStencilBufferDesc.ArraySize = 1;

    DepthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
   
    if (NumQualityLevels > 0) 
    {
        DepthStencilBufferDesc.SampleDesc.Count = 4;
        DepthStencilBufferDesc.SampleDesc.Quality = NumQualityLevels - 1;
    }
    else 
    {
        DepthStencilBufferDesc.SampleDesc.Count = 1; 
        DepthStencilBufferDesc.SampleDesc.Quality = 0;
    }

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

    return TRUE;
}
