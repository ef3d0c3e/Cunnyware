#ifndef SDK_IENGINEVGUIINTERNAL_HPP
#define SDK_IENGINEVGUIINTERNAL_HPP

#include "IEngineVGui.hpp"
#include "IAppSystem.hpp"
struct InputEvent;
struct LevelLoadingProgress;
class KeyValues;
struct InputContextHandle;

MAKE_CENUM_Q(PaintMode, u32,
	UIPANELS, 1<<0,
	INGAMEPANELS, 1<<1);

class IEngineVGuiInternal : public IEngineVGui
{
public:
	virtual ~IEngineVGuiInternal() = 0;

	virtual void Init() = 0;
	virtual void Connect() = 0;
	virtual void Shutdown() = 0;
	virtual bool SetVGUIDirectories() = 0;
	virtual bool IsInitialized() const = 0;
	virtual CreateInterfaceFn GetGameUIFactory() = 0;
	virtual bool Key_Event(const InputEvent& event) = 0;
	virtual void BackwardCompatibility_Paint() = 0;
	virtual void UpdateButtonState(const InputEvent& event) = 0;
	virtual void PostInit() = 0;

	virtual void Paint(PaintMode mode) = 0;

	// handlers for game UI (main menu)
	virtual void ActivateGameUI() = 0;
	virtual bool HideGameUI() = 0;
	virtual bool IsGameUIVisible() = 0;

	// console
	virtual void ShowConsole() = 0;
	virtual void HideConsole() = 0;
	virtual bool IsConsoleVisible() = 0;
	virtual void ClearConsole() = 0;

	virtual void HideDebugSystem() = 0;

	// level loading
	virtual void OnLevelLoadingStarted(const char* levelName, bool bLocalServer) = 0;
	virtual void OnLevelLoadingFinished() = 0;
	virtual void NotifyOfServerConnect(const char* game, i32 IP, i32 connectionPort, i32 queryPort) = 0;
	virtual void NotifyOfServerDisconnect() = 0;
	virtual void EnabledProgressBarForNextLoad() = 0;
	virtual void UpdateProgressBar(LevelLoadingProgress progress, bool showDialog = true) = 0;
	virtual void UpdateCustomProgressBar(f32 progress, const wchar_t* desc) = 0;
	virtual void StartCustomProgress() = 0;
	virtual void FinishCustomProgress() = 0;
	virtual void UpdateSecondaryProgressBarWithFile(f32 progress, const char* desc, i32 bytesTotal) = 0;
	virtual void UpdateSecondaryProgressBar(f32 progress, const wchar_t* desc) = 0;
	virtual void ShowErrorMessage() = 0;
	virtual void HideLoadingPlaque() = 0;
	virtual void StartLoadingScreenForCommand(const char* command) = 0;
	virtual void StartLoadingScreenForKeyValues(KeyValues* keyValues) = 0;

	// Should pause?
	virtual bool ShouldPause() = 0;
	virtual void SetGameDLLPanelsVisible(bool show) = 0;
	// Allows the level loading progress to show map-specific info
	virtual void SetProgressLevelName(const char* levelName) = 0;

	virtual void Simulate() = 0;

	virtual void SetNotAllowedToHideGameUI(bool notAllowedToHide) = 0;
	virtual void SetNotAllowedToShowGameUI(bool notAllowedToShow) = 0;

	virtual void NeedConnectionProblemWaitScreen() = 0;
	virtual void ShowPasswordUI(char const* currentPW) = 0;
	virtual void OnToolModeChanged(bool gameMode) = 0;

	virtual InputContextHandle GetGameUIInputContext() = 0;

	virtual bool IsPlayingFullScreenVideo() = 0;
};

#endif // SDK_IENGINEVGUIINTERNAL_HPP
