/**********************************************************************************************
 *
 *   Ngine - A game framework.
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

#include "ngine/utils/UWP/uwp_bootstrap.hpp"

#if defined(PLATFORM_UWP)

namespace ngine::utils::UWP {
    void BootstrappedGame::Initialize(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView) {
        applicationView->Activated +=
            ref new Windows::Foundation::TypedEventHandler<
            Windows::ApplicationModel::Core::CoreApplicationView^,
            Windows::ApplicationModel::Activation::IActivatedEventArgs^>(
                this, &BootstrappedGame::OnActivated);
    }

    void BootstrappedGame::Load(Platform::String^ entryPoint) {}

    void BootstrappedGame::Run() {
        while (m_game->m_running) {
            m_game->_performDraw();
        }
        m_game->m_running = false;
    }

    void BootstrappedGame::SetWindow(Windows::UI::Core::CoreWindow^ window) {
        // Register window events
        auto navigation = SystemNavigationManager::GetForCurrentView();
        navigation->BackRequested += ref new Windows::Foundation::EventHandler<
            Windows::UI::Core::BackRequestedEventArgs^>(
                this, &BootstrappedGame::OnBackRequested);
    }

    void BootstrappedGame::Uninitialize() {
        m_game->cleanup();
    }

    BootstrappedGame::BootstrappedGame(Game *game) {
        m_game = game;
    }

    void BootstrappedGame::OnActivated(Windows::ApplicationModel::Core::CoreApplicationView^ sender, Windows::ApplicationModel::Activation::IActivatedEventArgs^ args) {
        // Init
        m_game->init();

        // Start running
        m_game->m_running = true;

        // Activate window
        CoreWindow::GetForCurrentThread()->Activate();
    }

    void BootstrappedGame::OnBackRequested(Platform::Object^ sender, Windows::UI::Core::BackRequestedEventArgs^ args) {
        args->Handled = true;
    }

    void BootstrappedGame::OnSuspended(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ args) {
        throw ref new Platform::NotImplementedException();
    }

    void BootstrappedGame::OnResuming(Platform::Object^ sender, Platform::Object^ args) {
        throw ref new Platform::NotImplementedException();
    }

    BootstrappedGameSource::BootstrappedGameSource(Game* game) {
        m_game = ref new BootstrappedGame(game);
    }

    IFrameworkView ^BootstrappedGameSource::CreateView() {
        return m_game;
    }
}

#endif // defined(PLATFORM_UWP)
