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
        // Save game
        _Game = UWPGameBootstrapper::CurrentGame;

        // Get dimensions
        auto dims = _Game->GetDefaultWindowDimensions();

        // Set dimensions
        _Width = (int)dims.X;
        _Height = (int)dims.Y;
    }

    // Public Methods

    void GameApp::Initialize(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView) {
        // Get ready to get activated
        applicationView->Activated += ref new TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &GameApp::OnActivated);
    }

    void GameApp::Load(Platform::String^ entryPoint) {}

    void GameApp::Run() {
        // Run the game.
        _Game->Run();
    }

    void GameApp::SetWindow(Windows::UI::Core::CoreWindow^ window) {
        // Init
        Window::Init();

        // Navigation hack. Stops "B" from closing the game
        auto navigation = SystemNavigationManager::GetForCurrentView();
        navigation->BackRequested += ref new Windows::Foundation::EventHandler<Windows::UI::Core::BackRequestedEventArgs ^>(this, &GameApp::OnBackRequested);
    }

    // Protected Methods

    void GameApp::OnBackRequested(Platform::Object ^sender, Windows::UI::Core::BackRequestedEventArgs ^args) {
        args->Handled = true;
    }

    void GameApp::OnActivated(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView, Windows::ApplicationModel::Activation::IActivatedEventArgs^ args) {
        // Run() won't start until the CoreWindow is activated.
        CoreWindow::GetForCurrentThread()->Activate();
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
