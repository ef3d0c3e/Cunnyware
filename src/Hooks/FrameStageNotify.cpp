#include "Hooks.hpp"

void Hooks::FrameStageNotify(void* thisptr, ClientFrameStage curStage)
{
	clientVMT->GetOriginalMethod<void (*)(void*, ClientFrameStage)>(37)(thisptr, curStage);
}
