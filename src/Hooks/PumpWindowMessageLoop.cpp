#include "Hooks.hpp"

#include "../Interface.hpp"
#include "../UI/UI.hpp"

int Hooks::PumpWindowsMessageLoop(void* thisptr, void* ecx)
{
	if (UI::isVisible && !SetKeyCodeState::shouldListen)
		return 0;

	return launcherMgrVMT->GetOriginalMethod<i32 (*)(void*, void*)>(19)(thisptr, ecx);
}
