#ifndef HOOKS_HOOK_HPP
#define HOOKS_HOOK_HPP

#include "../Util/Hooker.hpp"
#include "../Util/VMT.hpp"
#include "../Util/Defs.hpp"
#include "../Interface.hpp"

namespace Hooks
{
	void SetKeyCodeState(void* thisptr, ButtonCode code, bool pressed);
	void SetMouseCodeState(void* thisptr, ButtonCode code, MouseCodeState state);

	i32 PumpWindowsMessageLoop(void* thisptr, void* ecx);
}

namespace Paint
{
	extern int windowWidth;
	extern int windowHeight;
}

namespace SetKeyCodeState
{
	extern bool shouldListen;
	extern ButtonCode* keyOutput;
}

namespace SDL2
{
	void Hook();
	void UnHook();
}

#endif // HOOKS_HOOK_HPP
