// CS2DummyInit.cpp - Simulates CS2 GPU initialization
#include <windows.h>
#include <d3d11.h>
#include <dxgi.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_DESTROY) PostQuitMessage(0);
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    const int width = 1920, height = 1080;
    
    // 1. Register window class
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0, 0,
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      L"DummyDX11Window", NULL };
    RegisterClassEx(&wc);

    // 2. Create full-screen borderless window
    HWND hwnd = CreateWindowEx(0, wc.lpszClassName, L"CS2 Dummy Init",
        WS_POPUP, 0, 0, width, height, NULL, NULL, wc.hInstance, NULL);

    // 3. Setup swap chain
    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Width = width;
    scd.BufferDesc.Height = height;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hwnd;
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.Windowed = FALSE;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    ID3D11Device* device = nullptr;
    ID3D11DeviceContext* context = nullptr;
    IDXGISwapChain* swapchain = nullptr;

    D3D_FEATURE_LEVEL featureLevel;
    if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0,
        NULL, 0, D3D11_SDK_VERSION, &scd, &swapchain, &device, &featureLevel, &context))) {
        MessageBox(NULL, L"Failed to initialize D3D11.", L"Error", MB_OK);
        return 1;
    }

    // 4. Show window
    ShowWindow(hwnd, SW_SHOW);
    SetForegroundWindow(hwnd);

    // 5. Run 30-second render loop
    DWORD startTime = GetTickCount();
    MSG msg = {};
    while (GetTickCount() - startTime < 30000) {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        swapchain->Present(1, 0);
    }

    // 6. Cleanup
    swapchain->Release();
    context->Release();
    device->Release();
    DestroyWindow(hwnd);
    UnregisterClass(wc.lpszClassName, wc.hInstance);

    return 0;
}
