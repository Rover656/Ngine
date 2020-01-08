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

#include "GameApp.hpp"

#if defined(PLATFORM_UWP)

#include <ppltasks.h>

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
        while (m_game->m_running && !m_game->m_gameWindow->ShouldClose()) {
            m_game->_runFrame();
        }

        // If we reach this point, we're done.
        m_game->m_running = false;
    }

    void GameApp::SetWindow(Windows::UI::Core::CoreWindow^ window) {
        // Register window events
        auto navigation = SystemNavigationManager::GetForCurrentView();
        navigation->BackRequested += ref new Windows::Foundation::EventHandler<Windows::UI::Core::BackRequestedEventArgs ^>(this, &GameApp::OnBackRequested);
    }

    void GameApp::Uninitialize() {
        // If this is ever reached (Which I doubt very much) clean
        m_game->_cleanup();
    }

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
        m_app = app_;
    }

    Windows::ApplicationModel::Core::IFrameworkView^ GameApplicationSource::CreateView() {
        return m_app;
    }
}

#endif
