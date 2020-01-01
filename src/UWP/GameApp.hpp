/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright (C) 2020 NerdThings.
*
*   LICENSE: GNU LGPLv3
*   View: In Ngine.hpp
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

namespace NerdThings::Ngine::UWP {
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
        static void ExecuteGame(Game *game_);
    };

    ref class GameApp sealed : public Windows::ApplicationModel::Core::IFrameworkView {
        /**
         * The game we are running
         */
        Game *m_game = nullptr;
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
        GameApplicationSource(GameApp ^app_);

        /**
         * Create the app view (returns the app).
         */
        virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView();
    };
}

#endif //GAMEAPP_H
#endif
