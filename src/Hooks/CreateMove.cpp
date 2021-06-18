#include "Hooks.hpp"

bool CreateMove::sendPacket = true;

bool Hooks::CreateMove(void* thisptr, f32 inputSampleTime, CUserCmd* cmd)
{
	clientModeVMT->GetOriginalMethod<bool (*)(void*, f32, CUserCmd*)>(25)(thisptr, inputSampleTime, cmd);
	if (!cmd || cmd->commandNumber == 0)
		return false;

	std::uintptr_t rbp;
	asm volatile("mov %%rbp, %0" : "=r" (rbp));
	bool *sendPacket = (*(bool**)rbp)-0x18;
	CreateMove::sendPacket = true;

	*sendPacket = CreateMove::sendPacket;
	
	return false;
}
