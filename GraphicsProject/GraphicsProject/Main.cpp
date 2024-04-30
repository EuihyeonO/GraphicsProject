#include "framework.h"
#include "GraphicsProject.h"
#include "EngineBase.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    EngineBase NewEngineBase;

    if (!NewEngineBase.Init(hInstance, 1600, 900))
    {
        return -1;
    }

    NewEngineBase.Loop();

    WPARAM EndParam = NewEngineBase.End();

    return (int)EndParam;
}