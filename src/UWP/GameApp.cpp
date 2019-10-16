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
        _Game = UWPGameBootstrapper::CurrentGame;
    }

    // Public Methods

    void GameApp::Initialize(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView) {
        // Register application events
        CoreApplication::Suspending += ref new Windows::Foundation::EventHandler<Windows::ApplicationModel::SuspendingEventArgs ^>(this, &GameApp::OnSuspended);
    }

    void GameApp::Load(Platform::String^ entryPoint) {}

    void GameApp::Run() {
        // Activate window
        CoreWindow::GetForCurrentThread()->Activate();

        // Run the game.
        _Game->Run();
    }

    void GameApp::SetWindow(Windows::UI::Core::CoreWindow^ window) {
        // Register window events
        auto navigation = SystemNavigationManager::GetForCurrentView();
        navigation->BackRequested += ref new Windows::Foundation::EventHandler<Windows::UI::Core::BackRequestedEventArgs ^>(this, &GameApp::OnBackRequested);
    }

    void GameApp::Uninitialize() { }

    // Protected Methods

    void GameApp::OnBackRequested(Platform::Object ^sender, Windows::UI::Core::BackRequestedEventArgs ^args) {
        args->Handled = true;
    }

    void GameApp::OnSuspended(Platform::Object ^sender, Windows::ApplicationModel::SuspendingEventArgs ^args) {
        // Do nothing, it works fine... right?
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
