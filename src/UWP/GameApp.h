/**********************************************************************************************
*
*   Ngine - The 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#include "../Ngine.h"

#if defined(PLATFORM_UWP)
#ifndef GAMEAPP_H
#define GAMEAPP_H

#include <Windows.h>
#include <chrono>
#include <memory>
#include <wrl.h>

#include "../Game.h"
#include "../Window.h"
#include "../Vector2.h"

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

namespace NerdThings::Ngine::UWP {
    /*
     * Quick and nasty bootstrapper.
     * This saves the game pointer until the UWP app starts.
     * This helps us to bypass UWP restrictions.
     */
    class NEAPI UWPGameBootstrapper {
    public:
        // Public Fields

        /*
         * The current game we are launching with
         */
        static Game* CurrentGame;

        // Public Methods

        /*
         * Execute this game.
         * Loads onto static member for GameApp.
         */
        static void ExecuteGame(Game *game_);
    };

    ref class GameApp sealed : public Windows::ApplicationModel::Core::IFrameworkView {
        // Private Fields

        /*
         * The game we are running
         */
        Game *_Game = nullptr;

        /*
         * Whether or not the game should be restarted
         */
        bool _ShouldRestart = false;
    public:

        // Public Constructor

        /*
         * Create a new game app
         */
        GameApp();

        // Public Methods

        /*
         * Initialize app
         */
        virtual void Initialize(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView);

        /*
         * Load app
         */
        virtual void Load(Platform::String^ entryPoint);        

        /*
         * Run app
         */
        virtual void Run();

        /*
         * Set app window
         */
        virtual void SetWindow(Windows::UI::Core::CoreWindow^ window);
        
        /*
         * Uninitialize the app
         */
        virtual void Uninitialize();

    protected:

        // Protected Methods

        /*
         * OnBackRequested event handler
         */
        void OnBackRequested(Platform::Object ^sender, Windows::UI::Core::BackRequestedEventArgs ^args);

        /*
         * OnSuspended event handler
         */
        void OnSuspended(Platform::Object ^sender, Windows::ApplicationModel::SuspendingEventArgs ^args);
    };

    /*
     * The application source for running the game
     */
    ref class GameApplicationSource sealed : Windows::ApplicationModel::Core::IFrameworkViewSource {
        // Private fields

        /*
         * The game app
         */
        GameApp ^_App;
    public:
        // Public Constructor

        /*
         * Create the application source.
         */
        GameApplicationSource(GameApp ^app_);

        // Public Methods

        /*
         * Create the app view (returns the app).
         */
        virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView();
    };
}

#endif //GAMEAPP_H
#endif
