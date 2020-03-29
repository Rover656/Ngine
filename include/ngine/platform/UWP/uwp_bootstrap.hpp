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

#ifndef NGINE_UWP_BOOTSTRAP_HPP
#define NGINE_UWP_BOOTSTRAP_HPP

#include "ngine/config.hpp"

#if defined(PLATFORM_UWP)

#include "ngine/game.hpp"

#include <Windows.h>
#include <memory>
#include <wrl.h>

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

namespace ngine::platform::UWP {
#if defined(DOXYGEN_SHOULD_SKIP_THIS)
    // HACK: Make doxygen see the class name
    ref class BootstrappedGame : public IFrameworkView {
#else
    ref class BootstrappedGame sealed : public IFrameworkView {
#endif
    public:
        /**
         * Initialize app
         */
        virtual void Initialize(Windows::ApplicationModel::Core::CoreApplicationView ^applicationView);

        /**
         * Load app.
         */
        virtual void Load(Platform::String ^entryPoint);

        /**
         * Run app.
         */
        virtual void Run();

        /**
         * Set app window.
         */
        virtual void SetWindow(Windows::UI::Core::CoreWindow ^window);

        /**
         * Uninitialize the app.
         */
        virtual void Uninitialize();
    internal:
        /**
         * Create a new game app.
         */
        BootstrappedGame(Game* game);
    protected:
        void OnActivated(
            Windows::ApplicationModel::Core::CoreApplicationView ^sender,
            Windows::ApplicationModel::Activation::IActivatedEventArgs ^args);

        /**
         * OnBackRequested event handler.
         */
        void OnBackRequested(Platform::Object ^sender,
            Windows::UI::Core::BackRequestedEventArgs ^args);

        /**
         * OnSuspended event handler.
         */
        void OnSuspended(Platform::Object ^sender,
            Windows::ApplicationModel::SuspendingEventArgs ^args);

        void OnResuming(Platform::Object ^sender, Platform::Object ^args);
    private:
        /**
         * The game we are running
         */
        Game *m_game = nullptr;

        /**
         * The main update thread.
         */
        std::thread m_updateThread;
    };

#if defined(DOXYGEN_SHOULD_SKIP_THIS)
    // HACK: Make doxygen see the class name
    ref class BootstrappedGameSource : IFrameworkViewSource {
#else
    ref class BootstrappedGameSource sealed : IFrameworkViewSource {
#endif
    public:
        /**
         * Create app view
         */
        virtual IFrameworkView^ CreateView() override;
    internal:
        /**
         * Create the bootstrapped source
         */
        BootstrappedGameSource(Game* game);
    private:
        BootstrappedGame ^m_game;
    };
}

#endif // defined(PLATFORM_UWP)

#endif //NGINE_UWP_BOOTSTRAP_HPP
