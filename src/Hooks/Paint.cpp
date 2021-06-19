#include "Hooks.hpp"

i32 Paint::windowWidth = 0;
i32 Paint::windowHeight = 0;

i32 Paint::engineWidth = 0;
i32 Paint::engineHeight = 0;

void Hooks::Paint(void* thisptr, PaintMode mode)
{
	engineVGuiVMT->GetOriginalMethod<void (*)(void*, PaintMode)>(15)(thisptr, mode);

	engine->GetScreenSize(Paint::engineHeight, Paint::engineWidth);
}

void Hooks::PaintImGui()
{

}
