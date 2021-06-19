#include "Hooks.hpp"

void Hooks::OverrideConfig(void* thisptr, MaterialSystemConfig* cfg, bool forceUpdate)
{
	materialVMT->GetOriginalMethod<void (*)(void*, MaterialSystemConfig*, bool)>(21)(thisptr, cfg, forceUpdate);
}
