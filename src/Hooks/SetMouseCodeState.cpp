#include "Hooks.hpp"

void Hooks::SetMouseCodeState(void* thisptr, ButtonCode code, MouseCodeState state)
{
	if (SetKeyCodeState::shouldListen && state == MouseCodeState::BUTTON_PRESSED)
	{
		SetKeyCodeState::shouldListen = false;
		*SetKeyCodeState::keyOutput = code;
	}

	inputInternalVMT->GetOriginalMethod<void(*)(void*, ButtonCode, MouseCodeState)>(93)(thisptr, code, state);
}
