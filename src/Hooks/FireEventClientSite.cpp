#include "Hooks.hpp"
#include "../SDK/IGameEvent.hpp"

bool Hooks::FireEventClientSide(void* thisptr, IGameEvent* event)
{
	return gameEventVMT->GetOriginalMethod<bool (*)(void*,  IGameEvent*)>(10)(thisptr, event);
}
