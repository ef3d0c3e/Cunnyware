#ifndef SDK_CENGINEVGUI_HPP
#define SDK_CENGINEVGUI_HPP

#include "IEngineVGuiInternal.hpp"
#include "CUtl/Vector.hpp"
class CSysModule;
class CStaticPanel;
class CEnginePanel;
class CVProfPanel;
class CBudgetPanelEngine;
class CTextureBudgetPanel;
class CDebugSystemPanel;
class CFocusOverlayPanel;
class CTransitionEffectPanel;
class IInputInternal;
class IMaterial;

MAKE_CENUM_Q(LevelLoadingProgress, i32,
	INVALID, -2,
	DEFAULT, -1,

	NONE, 0,
	CHANGELEVEL, 1,
	SPAWNSERVER, 2,
	LOADWORLDMODEL, 3,
	CRCMAP, 4,
	CRCCLIENTDLL, 5,
	CREATENETWORKSTRINGTABLES, 6,
	PRECACHEWORLD, 7,
	CLEARWORLD, 8,
	LEVELINIT, 9,
	PRECACHE, 10,
	ACTIVATESERVER, 11,
	BEGINCONNECT, 12,
	SIGNONCHALLENGE, 13,
	SIGNONCONNECT, 14,
	SIGNONCONNECTED, 15,
	PROCESSSERVERINFO, 16,
	PROCESSSTRINGTABLE, 17,
	SIGNONNEW, 18,
	SENDCLIENTINFO, 19,
	SENDSIGNONDATA, 20,
	SIGNONSPAWN, 21,
	CREATEENTITIES, 22,
	FULLYCONNECTED, 23,
	PRECACHELIGHTING, 24,
	READYTOPLAY, 25,
	HIGHESTITEM, 26, // must be last item in list
);

class CEngineVGui : public IEngineVGuiInternal
{
public:
	virtual ~CEngineVGui() = 0;

	// Methods of IEngineVGui
	virtual VPANEL GetPanel(VGuiPanel type) = 0;

	// Methods of IEngineVGuiInternal
	virtual void Init() = 0;
	virtual void Connect() = 0;
	virtual void Shutdown() = 0;
	virtual bool SetVGUIDirectories() = 0;
	virtual bool IsInitialized() const = 0;
	virtual bool Key_Event(const InputEvent& event) = 0;
	virtual void UpdateButtonState(const InputEvent& event) = 0;
	virtual void BackwardCompatibility_Paint() = 0;
	virtual void Paint(PaintMode mode) = 0;
	virtual void PostInit() = 0;

	// handlers for game UI (main menu)
	virtual void ActivateGameUI() = 0;
	virtual bool HideGameUI() = 0;
	virtual bool IsGameUIVisible() = 0;

	// console
	virtual void ShowConsole() = 0;
	virtual void HideConsole() = 0;
	virtual bool IsConsoleVisible() = 0;
	virtual void ClearConsole() = 0;

	// level loading
	virtual void OnLevelLoadingStarted(char const* levelName, bool localServer) = 0;
	virtual void OnLevelLoadingFinished() = 0;
	virtual void NotifyOfServerConnect(const char* game, i32 IP, i32 connectionPort, i32 queryPort) = 0;
	virtual void NotifyOfServerDisconnect() = 0;
	virtual void UpdateProgressBar(LevelLoadingProgress progress, bool showDialog = true) = 0;
	virtual void UpdateCustomProgressBar(f32 progress, const wchar_t* desc) = 0;
	virtual void StartCustomProgress() = 0;
	virtual void FinishCustomProgress() = 0;
	virtual void UpdateSecondaryProgressBarWithFile(f32 progress, const char* desc, i32 bytesTotal) = 0;
	virtual void UpdateSecondaryProgressBar(f32 progress, const wchar_t* desc) = 0;
	virtual void StartLoadingScreenForCommand(const char* command) = 0;
	virtual void StartLoadingScreenForKeyValues(KeyValues* keyValues) = 0;

	virtual void EnabledProgressBarForNextLoad() = 0;

	// Should pause?
	virtual bool ShouldPause() = 0;
	virtual void ShowErrorMessage() = 0;

	virtual void SetNotAllowedToHideGameUI(bool notAllowedToHide) = 0;

	virtual void SetNotAllowedToShowGameUI(bool notAllowedToShow) = 0;

	virtual void HideLoadingPlaque() = 0;

	// Allows the level loading progress to show map-specific info
	virtual void SetProgressLevelName(const char* levelName) = 0;

	virtual void OnToolModeChanged(bool gameMode) = 0;
	virtual InputContextHandle GetGameUIInputContext() = 0;

	virtual void NeedConnectionProblemWaitScreen() = 0;
	virtual void ShowPasswordUI(char const* currentPw) = 0;

	virtual bool IsPlayingFullScreenVideo() = 0;

private:
	virtual void Simulate() = 0;

	// debug overlays
	bool IsDebugSystemVisible();
	void HideDebugSystem();

	bool IsShiftKeyDown();
	bool IsAltKeyDown();
	bool IsCtrlKeyDown();
};

#endif // SDK_CENGINEVGUI_HPP
