#include "framework.h"
#include "GraphicsProject.h"
#include "EngineBase.h"

#include <iostream>
#include <d3d11.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    EngineBase NewEngineBase;

    if (!NewEngineBase.Init(hInstance))
    {
        std::cout << "Init Failed!" << std::endl;
        return -1;
    }

    NewEngineBase.Loop();

    WPARAM EndParam = NewEngineBase.End();

    return (int)EndParam;
}