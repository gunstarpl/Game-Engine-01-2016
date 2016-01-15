#include "Precompiled.hpp"
#include "System/Config.hpp"
#include "System/Timer.hpp"
#include "System/Window.hpp"
#include "System/InputState.hpp"

//
// Main
//

int main(int argc, char* argv[])
{
    Debug::Initialize();
    Build::Initialize();
    Logger::Initialize();

    // Create the main context.
    Context context;

    // Initialize the config.
    System::Config config;
    config.Load("Game.cfg");
    config.SetTable("Config");

    context.Set(&config);

    // Initialize the timer.
    System::Timer timer;
    timer.SetMaxDelta(1.0f / 10.0f);

    context.Set(&timer);

    // Initialize the window.
    System::Window window;
    if(!window.Initialize(context))
        return -1;

    // Initialize the input state.
    System::InputState inputState;
    if(!inputState.Initialize(context))
        return -1;

    // Tick timer once after the initialization to avoid big
    // time delta value right at the start of the first frame.
    timer.Tick();

    // Main loop.
    window.MakeContextCurrent();

    while(window.IsOpen())
    {
        // Update input state before processing events.
        inputState.Update();

        // Process window events.
        window.ProcessEvents();

        //
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Present the backbuffer to the window.
        window.Present();

        // Tick the timer.
        timer.Tick();
    }

    return 0;
}
