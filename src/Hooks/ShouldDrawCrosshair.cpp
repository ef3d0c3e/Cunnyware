#include "Hooks.hpp"

bool Hooks::ShouldDrawCrosshair(void* thisptr)
{
	return clientModeVMT->GetOriginalMethod<bool (*)(void*)>(29)(thisptr);
}
