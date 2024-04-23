#include <SDL.h>
#include <SDL_image.h>
#include <Pomegranate/window.h>
#include<imgui.h>
#include<backends/imgui_impl_sdl3.h>
#include<backends/imgui_impl_sdlrenderer3.h>
using namespace Pomegranate;

#include "CodeEditor.h"

//Main window
Window main_window = Window("Window", 1024, 720);



int main(int argc, char* argv[])
{
    pomegranate_init();

    main_window.open();
    main_window.make_current();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigWindowsMoveFromTitleBarOnly = true;

    // Setup Platform/Renderer backends
    ImGui_ImplSDL3_InitForSDLRenderer(main_window.get_sdl_window(),main_window.get_sdl_renderer());
    ImGui_ImplSDLRenderer3_Init(main_window.get_sdl_renderer());

    CodeEditor editor = CodeEditor();

    float tick_time = 0.0;
    bool is_running = true;
    SDL_Event event;

    while (is_running)
    {
        Uint64 start = SDL_GetPerformanceCounter(); //For delta time

        //- - - - - # SDL2 Stuff # - - - - -

        //Poll events
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL3_ProcessEvent(&event);
            InputManager::process_event(event);
            Window::process_event(&event);
            if (event.type == SDL_EVENT_QUIT) {
                is_running = false;
            }
        }

        //- - - - - # UPDATE # - - - - -
        SDL_SetRenderTarget(Window::current->get_sdl_renderer(), Window::current->get_render_texture());
        SDL_SetRenderDrawColor(Window::current->get_sdl_renderer(), 255, 255, 255, 255);
        SDL_RenderClear(Window::current->get_sdl_renderer());

        //Begin imgui
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        editor.render();

        //Draw imgui
        ImGui::Render();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData());
        //Copy the render texture to the window
        Window::current->display();

        //- - - - - # AFTER FRAME # - - - - -

        //Calculate delta time
        Uint64 end = SDL_GetPerformanceCounter();
        float secondsElapsed = (float)(end - start) / (float)SDL_GetPerformanceFrequency();
        delta_time = secondsElapsed;
        tick_time += delta_time;
    }
    main_window.close();
    pomegranate_quit(); //Cleanup
    return 0;
}
