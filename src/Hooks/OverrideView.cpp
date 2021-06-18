#include "Hooks.hpp"
#include "../SDK/CViewSetup.hpp"

f32 OverrideView::currentFov = 90.f;

void Hooks::OverrideView(void* thisptr, CViewSetup* setup)
{
	OverrideView::currentFov = setup->fov;

	clientModeVMT->GetOriginalMethod<void (*)(void*, CViewSetup*)>(19)(thisptr, setup);
}
