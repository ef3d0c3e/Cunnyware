#include "Hooks.hpp"

f32 Hooks::GetViewModelFov(void* thisptr)
{
	return clientModeVMT->GetOriginalMethod<f32 (*)(void*)>(36)(thisptr);
}
