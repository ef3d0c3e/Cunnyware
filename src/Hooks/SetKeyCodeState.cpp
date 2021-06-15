#include "Hooks.hpp"
#include "../Shortcuts.hpp"

bool SetKeyCodeState::shouldListen = false;
ButtonCode* SetKeyCodeState::keyOutput = nullptr;

void Hooks::SetKeyCodeState(void* thisptr, ButtonCode code, bool pressed)
{
	if (SetKeyCodeState::shouldListen && pressed)
	{
		SetKeyCodeState::shouldListen = false;
		*SetKeyCodeState::keyOutput = code;
	}

	if (!SetKeyCodeState::shouldListen)
		Shortcuts::SetKeyCodeState(code, pressed);


	inputInternalVMT->GetOriginalMethod<void (*)(void*, ButtonCode, bool)>(92)(thisptr, code, pressed);
}
