#include "Hooks.hpp"

void Hooks::BeginFrame(void* thisptr, f32 frameTime)
{
	if (!engine->IsInGame())
		CreateMove::sendPacket = true;

	materialVMT->GetOriginalMethod<void (*)(void*, f32)>(42)(thisptr, frameTime);
}
