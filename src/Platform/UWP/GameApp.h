#if defined(PLATFORM_UWP)
#ifndef GAMEAPP_H
#define GAMEAPP_H

// TODO: Make not header only

#include "../../ngine.h"

#define NOMINMAX
#include <Windows.h>
#include <chrono>
#include <memory>
#include <wrl.h>

#include "../../Audio/AudioManager.h"
#include "../../Graphics/OpenGL/OpenGL.h"
#include "../../Graphics/GraphicsManager.h"
#include "../../Input/Keyboard.h"
#include "../../Input/Mouse.h"
#include "../../Game.h"
#include "../../Resources.h"
#include "../../Window.h"
#include "../../Vector2.h"

using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::Devices::Input;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Gaming::Input;
using namespace Windows::Graphics::Display;
using namespace Microsoft::WRL;
using namespace Platform;

namespace NerdThings::Ngine::UWP {
    ref class GameApp : public Windows::ApplicationModel::Core::IFrameworkView
    {
        bool _WindowClosed = false;
        bool _WindowVisible = true;

        int _Width = 640;
        int _Height = 480;

        int _CurrentPointerID = -1;

        Windows::UI::Core::CoreWindow ^_Window;

        std::shared_ptr<Game> _Game;

        /*
         * The render target used for enforcing resolution
         */
        Graphics::TRenderTarget _RenderTarget;
    public:
        // IFrameworkView Methods.
        virtual void Initialize(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView)
        {
            // Register event handlers for app lifecycle. This example includes Activated, so that we
            // can make the CoreWindow active and start rendering on the window.
            applicationView->Activated += ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &GameApp::OnActivated);

            // Logic for other event handlers could go here.
            // Information about the Suspending and Resuming event handlers can be found here:
            // http://msdn.microsoft.com/en-us/library/windows/apps/xaml/hh994930.aspx

            CoreApplication::Resuming += ref new Windows::Foundation::EventHandler<Platform::Object^>(this, &GameApp::OnResuming);
        }

        Windows::UI::Core::CoreWindow ^GetWindow()
        {
            return _Window;
        }

        virtual void SetWindow(Windows::UI::Core::CoreWindow^ window)
        {
            _Window = window;

            window->SizeChanged += ref new TypedEventHandler<CoreWindow^, WindowSizeChangedEventArgs^>(this, &GameApp::OnWindowSizeChanged);
            window->VisibilityChanged += ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &GameApp::OnVisibilityChanged);
            window->Closed += ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &GameApp::OnWindowClosed);

            window->PointerPressed += ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &GameApp::PointerPressed);
            window->PointerWheelChanged += ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(this, &GameApp::PointerWheelChanged);
            window->KeyDown += ref new TypedEventHandler<CoreWindow ^, KeyEventArgs ^>(this, &GameApp::OnKeyDown);
            window->KeyUp += ref new TypedEventHandler<CoreWindow ^, KeyEventArgs ^>(this, &GameApp::OnKeyUp);

            Windows::Devices::Input::MouseDevice::GetForCurrentView()->MouseMoved += ref new TypedEventHandler<MouseDevice^, MouseEventArgs^>(this, &GameApp::MouseMoved);

            DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();
            currentDisplayInformation->DpiChanged += ref new TypedEventHandler<DisplayInformation^, Object^>(this, &GameApp::OnDpiChanged);
            currentDisplayInformation->OrientationChanged += ref new TypedEventHandler<DisplayInformation^, Object^>(this, &GameApp::OnOrientationChanged);

            // Init
            Window::UWPApp = this;
            Window::Init(_Width, _Height, "");
#if defined(GRAPHICS_OPENGL33) || defined(GRAPHICS_OPENGLES2)
            Graphics::OpenGL::GL::Init();
            ConsoleMessage("The OpenGL API has been initialized.", "NOTICE", "GAME");
#endif
        }

        virtual void Load(Platform::String^ entryPoint) {}

        void SetDimensions(int width, int height)
        {
            // Set dimensions
            this->_Width = width;
            this->_Height = height;
        }

        virtual void Run()
        {
            Game::CurrentGame->Run();

            return;
            // Create render target
            if (_Game->GetConfig() & MAINTAIN_DIMENSIONS) {
                _RenderTarget = Graphics::TRenderTarget(_Game->GetDimensions().X, _Game->GetDimensions().Y);
            }

            // Timing
            std::chrono::nanoseconds lag(0);
            auto started = std::chrono::high_resolution_clock::now();

            auto lastFPS = _Game->GetFPS();
            auto timeStep = std::chrono::milliseconds(int(1.0f / float(lastFPS) * 1000.0f));

            // Init audio
            ConsoleMessage("Attempting to initialize audio device.", "NOTICE", "GAME");
            Audio::AudioManager::InitDevice();

            // Check if the device was created
            if (Audio::AudioManager::IsReady()) {
                ConsoleMessage("Audio device initialized successfully..", "NOTICE", "GAME");
            }
            else {
                ConsoleMessage("Failed to create audio device, audio will be unavailable.", "WARN", "GAME");
            }

            // Invoke OnRun
            _Game->OnRun({});

            _Game->SetIsRunning(true);

            while (!Window::ShouldClose() && _Game->IsRunning()) {
                // Window/Game Size variables
                auto w = static_cast<float>(Window::GetWidth());
                auto h = static_cast<float>(Window::GetHeight());
                auto iw = static_cast<float>(_Game->GetDimensions().X);
                auto ih = static_cast<float>(_Game->GetDimensions().Y);
                auto scale = std::min(w / iw, h / ih);
                auto offsetX = (w - iw * scale) * 0.5;
                auto offsetY = (h - ih * scale) * 0.5;

                // Get the time since the last frame
                auto deltaTime = std::chrono::high_resolution_clock::now() - started;
                started = std::chrono::high_resolution_clock::now();
                lag += std::chrono::duration_cast<std::chrono::nanoseconds>(deltaTime);

                // Update timestep if FPS has changed
                if (_Game->GetFPS() != lastFPS) {
                    lastFPS = _Game->GetFPS();
                    timeStep = std::chrono::milliseconds(int(1.0f / float(lastFPS) * 1000.0f));

                    ConsoleMessage("Timestep updated to match FPS.", "NOTICE", "GAMEAPP");
                }

                // Setup mouse
                if (_Game->GetConfig() & MAINTAIN_DIMENSIONS && _RenderTarget.IsValid()) {
                    Input::Mouse::SetScale(iw / (w - offsetX * 2.0f), ih / (h - offsetY * 2.0f));
                    Input::Mouse::SetOffset(-offsetX, -offsetY);
                }

                // Skip if we are going to catch up more than 5 seconds, that is too much (May not fix what I am experiencing)
                if (lag.count() >= 5e+9) lag = std::chrono::nanoseconds(0);

                // Run Updates
                while (lag >= timeStep) {
                    // Run a single update
                    lag -= timeStep;
                    Update();
                }

                // Prep for drawing
                Graphics::Renderer::BeginDrawing();

                // Clear
                Graphics::Renderer::Clear(Graphics::TColor::Orange);

                // If using, start using target
                if (_Game->GetConfig() & MAINTAIN_DIMENSIONS && _RenderTarget.IsValid()) {
                    _RenderTarget.GetTexture().SetTextureWrap(WRAP_CLAMP);
                    _RenderTarget.GetTexture().SetTextureFilter(_Game->RenderTargetFilterMode);
                    Graphics::GraphicsManager::PushTarget(_RenderTarget);
                }

                // Clear the background
                _Game->Clear();

                // Draw
                _Game->Draw();

                // If using a target, draw target
                if (_Game->GetConfig() & MAINTAIN_DIMENSIONS && _RenderTarget.IsValid()) {
                    auto popped = false;
                    Graphics::GraphicsManager::PopTarget(popped);

                    Graphics::Renderer::DrawTexture(_RenderTarget.GetTexture(),
                        {
                            (w - iw * scale) * 0.5f,
                            (h - ih * scale) * 0.5f,
                            iw * scale,
                            ih * scale
                        },
                        {
                            0,
                            0,
                            static_cast<float>(_RenderTarget.Width),
                            static_cast<float>(_RenderTarget.Height) * -1
                        },
                        Graphics::TColor::White);
                }

                // Reset mouse
                if (_Game->GetConfig() & MAINTAIN_DIMENSIONS && _RenderTarget.IsValid()) {
                    Input::Mouse::SetScale(1, 1);
                    Input::Mouse::SetOffset(0, 0);
                }

                // Finish drawing
                Graphics::Renderer::EndDrawing();

                // Swap buffers
                Window::SwapBuffers();

                // Poll inputs
                Window::PollEvents();

                // Release thread to CPU (Stops weird idle cpu usage and fps drops)
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }

            // Delete loaded resources
            Resources::DeleteAll();

            // Close audio
            ConsoleMessage("Closing audio device.", "NOTICE", "GAME");
            Audio::AudioManager::CloseDevice();

            // Close window
            ConsoleMessage("Closing window.", "NOTICE", "GAME");
            Window::Cleanup();

            ConsoleMessage("Game successfully shut down.", "NOTICE", "GAME");
        }

        //Called every frame (Maybe add draw)
        virtual void Update() {}

        virtual void Uninitialize() {}

    protected:

        // Application lifecycle event handlers.
        void OnActivated(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView, Windows::ApplicationModel::Activation::IActivatedEventArgs^ args)
        {
            // Run() won't start until the CoreWindow is activated.
            CoreWindow::GetForCurrentThread()->Activate();
        }

        void OnResuming(Platform::Object^ sender, Platform::Object^ args) {}

        // Window event handlers.
        void OnWindowSizeChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ args)
        {

        }

        void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args)
        {
            _WindowVisible = args->Visible;
        }

        void OnWindowClosed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::CoreWindowEventArgs^ args)
        {
            _WindowClosed = true;
        }

        // DisplayInformation event handlers.
        void OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args) {}
        void OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args) {}

        // Input event handlers
        void PointerPressed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::PointerEventArgs^ args)
        {
            //Get the current active pointer ID for our loop
            _CurrentPointerID = args->CurrentPoint->PointerId;
            args->Handled = true;
        }

        void PointerWheelChanged(Windows::UI::Core::CoreWindow ^sender, Windows::UI::Core::PointerEventArgs^ args)
        {

        }

        void MouseMoved(Windows::Devices::Input::MouseDevice^ mouseDevice, Windows::Devices::Input::MouseEventArgs^ args)
        {
        }

        void OnKeyDown(Windows::UI::Core::CoreWindow ^ sender, Windows::UI::Core::KeyEventArgs ^ args)
        {
        }

        void OnKeyUp(Windows::UI::Core::CoreWindow ^ sender, Windows::UI::Core::KeyEventArgs ^ args)
        {
        }
    };

    // Application source for creating the program
    ref class GameApplicationSource sealed : Windows::ApplicationModel::Core::IFrameworkViewSource
    {
        GameApp ^_App;
    public:
        GameApplicationSource(GameApp ^app_)
        {
            _App = app_;
        }

        virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView()
        {
            return _App;
        }
    };
}

#endif //GAMEAPP_H
#endif
