#include "Hooks.hpp"

void Hooks::PaintTraverse(void* thisptr, VPANEL vguiPanel, bool forceRepaint, bool allowForce)
{
	panelVMT->GetOriginalMethod<void (*)(void*, VPANEL, bool, bool)>(42)(thisptr, vguiPanel, forceRepaint, allowForce);
}
