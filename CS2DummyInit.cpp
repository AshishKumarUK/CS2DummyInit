// CS2DummyInit.cpp
#include <windows.h>
#include <d3d11.h>
#include <iostream>
#pragma comment(lib, "d3d11.lib")

int main(int argc, char* argv[]) {
    int runSeconds = 30;
    for (int i = 1; i + 1 < argc; i++) {
        if (strcmp(argv[i], "-time") == 0) runSeconds = atoi(argv[i+1]);
    }

    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_9_1
    };
    D3D_FEATURE_LEVEL obtained = D3D_FEATURE_LEVEL_11_0;
    IDXGISwapChain* swapChain = nullptr;
    ID3D11Device* dev = nullptr;
    ID3D11DeviceContext* ctx = nullptr;
    DXGI_SWAP_CHAIN_DESC desc = {};
    desc.BufferCount = 1;
    desc.BufferDesc.Width = 1;
    desc.BufferDesc.Height = 1;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.OutputWindow = GetConsoleWindow();
    desc.SampleDesc.Count = 1;
    desc.Windowed = TRUE;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0,
        featureLevels, _countof(featureLevels),
        D3D11_SDK_VERSION, &desc,
        &swapChain, &dev, &obtained, &ctx
    );

    if (FAILED(hr)) {
        std::cerr << "Failed to init D3D11 device\n";
        return 1;
    }
    std::cout << "DX11 device initialized, running for "
              << runSeconds << " seconds...\n";
    Sleep(runSeconds * 1000);
    swapChain->Release();
    dev->Release();
    ctx->Release();
    std::cout << "Done. GPU should now be initialized.\n";
    return 0;
}
