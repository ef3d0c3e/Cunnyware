#ifndef SDK_ICLIENTMODE_HPP
#define SDK_ICLIENTMODE_HPP

#include "Defs.hpp"
#include "ButtonCode.hpp"

namespace vgui
{ class Panel; class AnimationController; }

class C_BaseEntity;
class C_BasePlayer;
struct CViewSetup;
class AudioState;

MAKE_CENUM_Q(MessageModeType, i32,
	NONE, 0,
	SAY, 1,
	SAY_TEAM, 2);

class IClientMode
{
public:
	virtual ~IClientMode() = 0;

	// Called before the HUD is initialized.
	virtual void InitViewport() = 0;

	// One time init when .dll is first loaded.
	virtual void Init() = 0;

	// Called when vgui is shutting down.
	virtual void VGui_Shutdown() = 0;

	// One time call when dll is shutting down
	virtual void Shutdown() = 0;

	// Called when switching from one IClientMode to another.
	// This can re-layout the view and such.
	// Note that Enable and Disable are called when the DLL initializes and shuts down.
	virtual void Enable() = 0;
	virtual void EnableWithRootPanel(VPANEL root) = 0;

	// Called when it's about to go into another client mode.
	virtual void Disable() = 0;

	// Called when initializing or when the view changes.
	// This should move the viewport into the correct position.
	virtual void Layout(bool force = false) = 0;

	// Gets at the viewport, if there is one...
	virtual vgui::Panel* GetViewport() = 0;

	// Gets a panel hierarchically below the viewport by name like so "ASWHudInventoryMode/SuitAbilityPanel/ItemPanel1"...
	virtual vgui::Panel* GetPanelFromViewport(const char* namePath) = 0;

	// Gets at the viewports vgui panel animation controller, if there is one...
	virtual vgui::AnimationController* GetViewportAnimationController() = 0;

	// called every time shared client dll/engine data gets changed,
	// and gives the cdll a chance to modify the data.
	virtual void ProcessInput(bool active) = 0;

	// The mode can choose to draw/not draw entities.
	virtual bool ShouldDrawDetailObjects() = 0;
	virtual bool ShouldDrawEntity(C_BaseEntity* ent) = 0;
	virtual bool ShouldDrawLocalPlayer(C_BasePlayer* player) = 0;
	virtual bool ShouldDrawParticles() = 0;

	// The mode can choose to not draw fog
	virtual bool ShouldDrawFog() = 0;

	virtual void OverrideView(CViewSetup* setup) = 0;
	virtual void OverrideAudioState(AudioState* audioState) = 0;
	virtual i32 KeyInput(i32 down, ButtonCode keynum, const char* currentBinding) = 0;
	virtual void StartMessageMode(MessageModeType messageModeType) = 0;
	virtual vgui::Panel* GetMessagePanel() = 0;
	virtual void OverrideMouseInput(f32* x, f32* y) = 0;
	virtual bool CreateMove(f32 inputSampleTime, CUserCmd* cmd) = 0;

	virtual void LevelInit(const char* newmap) = 0;
	virtual void LevelShutdown() = 0;

	// Certain modes hide the view model
	virtual bool ShouldDrawViewModel() = 0;
	virtual bool ShouldDrawCrosshair() = 0;

	// Let mode override viewport for engine
	virtual void AdjustEngineViewport(i32& x, i32& y, i32& width, i32& height) = 0;

	// Called before rendering a view.
	virtual void PreRender(CViewSetup* pSetup) = 0;

	// Called after everything is rendered.
	virtual void PostRender() = 0;

	virtual void PostRenderVGui() = 0;

	virtual void ActivateInGameVGuiContext(vgui::Panel* pPanel) = 0;
	virtual void DeactivateInGameVGuiContext() = 0;
	virtual f32 GetViewModelFOV() = 0;

	virtual bool CanRecordDemo(char* errorMsg, i32 length) const = 0;

	virtual void OnColorCorrectionWeightsReset() = 0;
	virtual f32 GetColorCorrectionScale() const = 0;

	virtual i32 HudElementKeyInput(i32 down, ButtonCode keynum, const char* currentBinding) = 0;

	virtual void DoPostScreenSpaceEffects(const CViewSetup* setup) = 0;

	// Called every frame.
	virtual void Update() = 0;

	virtual void SetBlurFade(f32 scale) = 0;
	virtual f32 GetBlurFade() = 0;
};

#endif // SDK_ICLIENTMODE_HPP
