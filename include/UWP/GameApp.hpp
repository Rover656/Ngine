/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright 2020 NerdThings (Reece Mackie)
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
**********************************************************************************************/

#include "../Config.hpp"

#if defined(PLATFORM_UWP)
#ifndef GAMEAPP_H
#define GAMEAPP_H

#include <Windows.h>
#include <chrono>
#include <memory>
#include <wrl.h>

#include "../Game.hpp"
#include "../Math.hpp"
#include "../Window.hpp"

using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::Devices::Input;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
using namespace Microsoft::WRL;
using namespace Platform;

namespace ngine::UWP {
    /**
     * Quick and nasty bootstrapper.
     * This saves the game pointer until the UWP app starts.
     * This helps us to bypass UWP restrictions.
     */
    class NEAPI UWPGameBootstrapper {
    public:
        /**
         * The current game we are launching with
         */
        static Game* CurrentGame;

        /**
         * Execute this game.
         * Loads onto a static member for GameApp.
         *
         * @param game_ Game to execute.
         */
        static void ExecuteGame(Game *game);
    };

    ref class GameApp sealed : public Windows::ApplicationModel::Core::IFrameworkView {
        /**
         * The game we are running
         */
        Game *m_game = nullptr;

        /**
         * The main update thread.
         */
        std::thread m_updateThread;
    public:
        /**
         * Create a new game app.
         */
        GameApp();

        /**
         * Initialize app
         */
        virtual void Initialize(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView);

        /**
         * Load app.
         */
        virtual void Load(Platform::String^ entryPoint);        

        /**
         * Run app.
         */
        virtual void Run();

        /**
         * Set app window.
         */
        virtual void SetWindow(Windows::UI::Core::CoreWindow^ window);
        
        /**
         * Uninitialize the app.
         */
        virtual void Uninitialize();

    protected:
        void OnActivated(Windows::ApplicationModel::Core::CoreApplicationView ^sender, Windows::ApplicationModel::Activation::IActivatedEventArgs ^args);

        /**
         * OnBackRequested event handler.
         */
        void OnBackRequested(Platform::Object ^sender, Windows::UI::Core::BackRequestedEventArgs ^args);

        /**
         * OnSuspended event handler.
         */
        void OnSuspended(Platform::Object ^sender, Windows::ApplicationModel::SuspendingEventArgs ^args);
        void OnResuming(Platform::Object ^sender, Platform::Object ^args);       
    };

    /**
     * The application source for running the game.
     */
    ref class GameApplicationSource sealed : Windows::ApplicationModel::Core::IFrameworkViewSource {
        /**
         * The game app.
         */
        GameApp ^m_app;
    public:
        /**
         * Create the application source.
         */
        GameApplicationSource(GameApp ^app);

        /**
         * Create the app view (returns the app).
         */
        virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView();
    };
}

#endif //GAMEAPP_H
#endif
