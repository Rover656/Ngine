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

#include "../../Audio/AudioDevice.h"
#include "../../Filesystem/Resources.h"
#include "../../Graphics/OpenGL/OpenGL.h"
#include "../../Graphics/GraphicsManager.h"
#include "../../Input/Keyboard.h"
#include "../../Input/Mouse.h"
#include "../../Game.h"
#include "../../Window.h"
#include "../../Vector2.h"

using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::Devices::Input;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
//using namespace Windows::Gaming::Input;
using namespace Windows::Graphics::Display;
using namespace Microsoft::WRL;
using namespace Platform;

namespace NerdThings::Ngine::UWP {
    ref class GameApp sealed : public Windows::ApplicationModel::Core::IFrameworkView {
        /*
         * App window height
         */
        int _Height = 0;

        /*
         * App window width
         */
        int _Width = 0;
    public:

        GameApp() {
            // Get dimensions
            auto dims = Game::CurrentGame->GetDefaultWindowDimensions();

            // Set dimensions
            _Width = (int)dims.X;
            _Height = (int)dims.Y;
        }

        // IFrameworkView Methods.

        virtual void Initialize(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView) {
            // Get ready to get activated
            applicationView->Activated += ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &GameApp::OnActivated);
        }

        virtual void SetWindow(Windows::UI::Core::CoreWindow^ window) {
            // Init
            Window::Init();

            // Navigation hack. Stops "B" from closing the game
            auto navigation = SystemNavigationManager::GetForCurrentView();
            navigation->BackRequested += ref new Windows::Foundation::EventHandler<Windows::UI::Core::BackRequestedEventArgs ^>(this, &GameApp::OnBackRequested);
        }

        virtual void Load(Platform::String^ entryPoint) {}

        virtual void Run() {
            // Run the current game.
            // We had to do this because .NET c++ does not like native pointers inside of this class
            Game::CurrentGame->Run();
        }

        virtual void Uninitialize() {}

    protected:

        // Application lifecycle event handlers.

        void OnActivated(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView, Windows::ApplicationModel::Activation::IActivatedEventArgs^ args) {
            // Run() won't start until the CoreWindow is activated.
            CoreWindow::GetForCurrentThread()->Activate();
        }

        void OnBackRequested(Platform::Object ^sender, Windows::UI::Core::BackRequestedEventArgs ^args) {
            args->Handled = true;
        }
    };

    // Application source for creating the program
    ref class GameApplicationSource sealed : Windows::ApplicationModel::Core::IFrameworkViewSource {
        GameApp ^_App;
    public:
        GameApplicationSource(GameApp ^app_) {
            _App = app_;
        }

        virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView() {
            return _App;
        }
    };
}

#endif //GAMEAPP_H
#endif
