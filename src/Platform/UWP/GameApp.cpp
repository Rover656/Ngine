#if defined(PLATFORM_UWP)
#include "GameApp.h"

namespace NerdThings::Ngine::UWP {
    Game *UWPGameBootstrapper::CurrentGame = nullptr;
}
#endif
