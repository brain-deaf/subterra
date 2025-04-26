// Subterra.cpp — minimal SDL3 + bgfx “hello world” (PS1-purple clear)

#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>        // SDL_GetWindowProperties
#include <SDL3/SDL_properties.h>   // SDL_GetPointerProperty

#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <iostream>
#include <chrono>

int main(int /*argc*/, char** /*argv*/)
{
    //--------------------------------------------------------------------
    // 1) Initialise SDL (video)
    //--------------------------------------------------------------------
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "[SDL] Init failed: " << SDL_GetError() << '\n';
        return 1;
    }

    //--------------------------------------------------------------------
    // 2) Create an SDL window
    //--------------------------------------------------------------------
    SDL_Window* window = SDL_CreateWindow("PSX Hello World",
        1280, 720,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!window)
    {
        std::cerr << "[SDL] Window creation failed: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    //--------------------------------------------------------------------
    // 3) Grab the native window handle via SDL3 property API
    //--------------------------------------------------------------------
    SDL_PropertiesID props = SDL_GetWindowProperties(window);

    bgfx::PlatformData pd{};
#if defined(_WIN32)
    pd.nwh = SDL_GetPointerProperty(props,
        SDL_PROP_WINDOW_WIN32_HWND_POINTER,
        nullptr);                 // Win32 HWND
#elif defined(__APPLE__)
    pd.nwh = SDL_GetPointerProperty(props,
        SDL_PROP_WINDOW_COCOA_WINDOW_POINTER,
        nullptr);                 // NSWindow*
#else
    pd.nwh = SDL_GetPointerProperty(props,
        SDL_PROP_WINDOW_X11_WINDOW_POINTER,
        nullptr);                 // X11 Window
#endif
    if (!pd.nwh)
    {
        std::cerr << "[SDL] Failed to obtain native window handle: "
            << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    //--------------------------------------------------------------------
    // 4) Initialise bgfx (attach native handle through init params)
    //--------------------------------------------------------------------
    bgfx::Init init{};
    init.type = bgfx::RendererType::Count; // auto-detect
    init.resolution.width = 1920;
    init.resolution.height = 1080;
    init.resolution.reset = BGFX_RESET_VSYNC;
    init.platformData = pd;

    if (!bgfx::init(init))
    {
        std::cerr << "[bgfx] init() failed\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // View 0: clear to PS1-purple and cover the whole window
    bgfx::setViewClear(0,
        BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
        0x553388FF,   // ABGR
        1.0f,
        0);
    bgfx::setViewRect(0, 0, 0, 1920, 1080);

    //--------------------------------------------------------------------
    // 5) Main loop
    //--------------------------------------------------------------------
    bool   running = true;
    auto   last = std::chrono::high_resolution_clock::now();

    while (running)
    {
        SDL_Event ev;
        while (SDL_PollEvent(&ev))
        {
            if (ev.type == SDL_EVENT_QUIT)
                running = false;

            else if (ev.type == SDL_EVENT_WINDOW_RESIZED)
            {
                uint32_t w = static_cast<uint32_t>(ev.window.data1);
                uint32_t h = static_cast<uint32_t>(ev.window.data2);
                bgfx::reset(w, h, BGFX_RESET_VSYNC);
                bgfx::setViewRect(0, 0, 0, w, h);
            }
        }

        bgfx::touch(0);   // submit nothing — just clear
        bgfx::frame();

        // Optional frame-time print
        auto   now = std::chrono::high_resolution_clock::now();
        double dt = std::chrono::duration<double, std::milli>(now - last).count();
        std::cerr << "frame: " << dt << " ms\r" << std::flush;
        last = now;
    }

    //--------------------------------------------------------------------
    // 6) Clean-up
    //--------------------------------------------------------------------
    bgfx::shutdown();
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
