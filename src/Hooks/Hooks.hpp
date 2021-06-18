#ifndef HOOKS_HOOK_HPP
#define HOOKS_HOOK_HPP

#include "../Util/Hooker.hpp"
#include "../Util/VMT.hpp"
#include "../Util/Defs.hpp"
#include "../Interface.hpp"

namespace Hooks
{
	// InputInternal
	void SetKeyCodeState(void* thisptr, ButtonCode code, bool pressed);
	void SetMouseCodeState(void* thisptr, ButtonCode code, MouseCodeState state);

	// LauncherMgr
	i32 PumpWindowsMessageLoop(void* thisptr, void* ecx);

	// Client
	void FrameStageNotify(void* thisptr, ClientFrameStage curStage);
	void LevelInitPostEntity(void* thisptr);

	// ClientMode
	void OverrideView(void* thisptr, CViewSetup* setup);
	bool CreateMove(void* thisptr, f32 inputSampleTime, CUserCmd* cmd);
	bool ShouldDrawCrosshair(void* thisptr);
	f32 GetViewModelFov(void* thisptr);
}

namespace SDL2
{
	void Hook();
	void UnHook();
}

namespace SetKeyCodeState
{
	extern bool shouldListen;
	extern ButtonCode* keyOutput;
}

namespace OverrideView
{
	extern f32 currentFov;
}

namespace CreateMove
{
	extern bool sendPacket;
}

namespace Paint
{
	extern int windowWidth;
	extern int windowHeight;
}

#endif // HOOKS_HOOK_HPP
