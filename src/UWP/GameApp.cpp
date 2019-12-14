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

#include "GameApp.h"

#if defined(PLATFORM_UWP)

#include <ppltasks.h>

#include "../Audio/AudioDevice.h"

namespace NerdThings::Ngine::UWP {
    ////////
    // UWPGameBootstrapper
    ////////

    // Public Fields

    Game *UWPGameBootstrapper::CurrentGame = nullptr;

    // Public Methods

    void UWPGameBootstrapper::ExecuteGame(Game *game_) {
        CurrentGame = game_;
    }

    ////////
    // GameApp
    ////////

    // Public Constructor

    GameApp::GameApp() {
        // Store the game
        m_game = UWPGameBootstrapper::CurrentGame;
    }

    // Public Methods

    void GameApp::Initialize(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView) {
        // Register application events
        CoreApplication::Suspending += ref new Windows::Foundation::EventHandler<Windows::ApplicationModel::SuspendingEventArgs ^>(this, &GameApp::OnSuspended);
        CoreApplication::Resuming += ref new Windows::Foundation::EventHandler<Platform::Object ^>(this, &NerdThings::Ngine::UWP::GameApp::OnResuming);
        applicationView->Activated += ref new Windows::Foundation::TypedEventHandler<Windows::ApplicationModel::Core::CoreApplicationView ^, Windows::ApplicationModel::Activation::IActivatedEventArgs ^>(this, &NerdThings::Ngine::UWP::GameApp::OnActivated);
    }

    void GameApp::Load(Platform::String^ entryPoint) {}

    void GameApp::Run() {
        // Run the game.
        while (m_game->m_running) {
            m_game->_runFrame();
        }
    }

    void GameApp::SetWindow(Windows::UI::Core::CoreWindow^ window) {
        // Register window events
        auto navigation = SystemNavigationManager::GetForCurrentView();
        navigation->BackRequested += ref new Windows::Foundation::EventHandler<Windows::UI::Core::BackRequestedEventArgs ^>(this, &GameApp::OnBackRequested);
    }

    void GameApp::Uninitialize() { }

    void GameApp::OnActivated(Windows::ApplicationModel::Core::CoreApplicationView ^sender, Windows::ApplicationModel::Activation::IActivatedEventArgs ^args) {
        // Init game.
        m_game->_init();

        // Mark as running
        m_game->m_running = true;

        // Activate window
        CoreWindow::GetForCurrentThread()->Activate();
    }

    void GameApp::OnBackRequested(Platform::Object ^sender, Windows::UI::Core::BackRequestedEventArgs ^args) {
        args->Handled = true;
    }

    void GameApp::OnSuspended(Platform::Object ^sender, Windows::ApplicationModel::SuspendingEventArgs ^args) {
        // Get the deferral
        auto deferral = args->SuspendingOperation->GetDeferral();

        // Suspend the game.
        concurrency::create_task([this, deferral]() {
            m_game->OnSuspend();
            m_game->m_running = false;
            deferral->Complete();
        });
    }

    void NerdThings::Ngine::UWP::GameApp::OnResuming(Platform::Object ^sender, Platform::Object ^args) {
        m_game->OnResume();
        m_game->m_running = true;
    }


    ////////
    // GameApplicationSource
    ////////

    // Public Constructor

    GameApplicationSource::GameApplicationSource(GameApp ^app_) {
        _App = app_;
    }

    Windows::ApplicationModel::Core::IFrameworkView^ GameApplicationSource::CreateView() {
        return _App;
    }
}

#endif
