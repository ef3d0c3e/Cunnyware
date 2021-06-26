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

	// CEngineVGui
	void Paint(void* thisptr, PaintMode mode);
	void PaintImGui();

	// IGameEventManager
	bool FireEventClientSide(void* thisptr, class IGameEvent* event);

	// IMaterialSystem
	void OverrideConfig(void* thisptr, MaterialSystemConfig* cfg, bool forceUpdate);
	void BeginFrame(void* thisptr, f32 frameTime);

	// IVModelRender
	void DrawModelExecute(void* thisptr, IMatRenderContext* renderContext, const DrawModelState& state, const ModelRenderInfo& info, Mat3x4* customBoneToWorld);

	// IVPanel
	void PaintTraverse(void* thisptr, VPANEL vguiPanel, bool forceRepaint, bool allowForce);
}

namespace SDL2
{
	void Hook();
	void UnHook();

	extern bool wantRebuild;
	extern u32 fontFlags;
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
	extern i32 windowWidth;
	extern i32 windowHeight;

    extern i32 engineWidth;
    extern i32 engineHeight;
}

#endif // HOOKS_HOOK_HPP
