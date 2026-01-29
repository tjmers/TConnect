#include "../graphics/win_graphics.h"


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

Graphics* graphics;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    // Create window class and define fields
    WNDCLASSEXW window{};
    window.hInstance = hInstance;
    window.lpszClassName = L"tconnect_client";
    window.lpfnWndProc = WndProc;
    window.cbSize = sizeof(window);
    window.style = CS_OWNDC | CS_DBLCLKS;
    window.cbClsExtra = 0;
    window.cbWndExtra = 0;
    window.hIcon = nullptr;
    window.hCursor = LoadCursor(NULL, IDC_ARROW);
    window.lpszMenuName = nullptr;
    window.hIconSm = nullptr;
    RegisterClassExW(&window);

    // Window size
    RECT client_region = { 0, 0, Graphics::STARTING_SCREEN_WIDTH, Graphics::STARTING_SCREEN_HEIGHT };
    AdjustWindowRectEx(&client_region, WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_BORDER | WS_SIZEBOX, FALSE, 0);

    // Create window
    HWND hWnd = CreateWindowExW(0, L"tconnect_client", L"TConnect", WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_BORDER | WS_SIZEBOX, 75, 25, client_region.right - client_region.left, client_region.bottom - client_region.top, nullptr, nullptr, hInstance, nullptr);

    Graphics g;
    graphics = &g;

    if (!g.Init(hWnd)) {
        std::cerr << "Error initalizing Direct2D\n";
        return 1;
    }

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);


    MSG msg;
    BOOL gResult;

    while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (gResult == -1) {
        std::cerr << "Something went wrong :(\n";
        return 1;
    }

    return static_cast<int>(msg.wParam);
}
#pragma GCC diagnostic pop // -Wunused-parameter

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        
        case WM_DESTROY:
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;

        default:
            return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }
}