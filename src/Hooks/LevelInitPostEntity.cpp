#include "Hooks.hpp"

void Hooks::LevelInitPostEntity(void* thisptr)
{
    clientVMT->GetOriginalMethod<void (*)(void*)>(6)(thisptr);
}
