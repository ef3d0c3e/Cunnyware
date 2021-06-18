#ifndef SDK_IBASECLIENTDLL_HPP
#define SDK_IBASECLIENTDLL_HPP

#include "IAppSystem.hpp"
#include "ButtonCode.hpp"
#include "Defs.hpp"

class CGlobalVarsBase;
class ClientClass;
class bf_write;
class bf_read;
class CViewSetup;
class ScreenFade;
class CEngineSprite;
class IMaterialProxy;
class CSaveRestoreData;
class datamap_t;
class typedescription_t;
class CStandardRecvProxies;
class IClientEntity;
struct InputEvent;

MAKE_CENUM_Q(ClientFrameStage, i32,
	FRAME_UNDEFINED, -1,			// (haven't run any frames yet)
	FRAME_START, 0,

	// A network packet is being recieved
	FRAME_NET_UPDATE_START, 1,
		// Data has been received and we're going to start calling PostDataUpdate
		FRAME_NET_UPDATE_POSTDATAUPDATE_START, 2,
		// Data has been received and we've called PostDataUpdate on all data recipients
		FRAME_NET_UPDATE_POSTDATAUPDATE_END, 3,
	// We've received all packets, we can now do interpolation, prediction, etc..
	FRAME_NET_UPDATE_END, 4,

	// We're about to start rendering the scene
	FRAME_RENDER_START, 5,
	// We've finished rendering the scene.
	FRAME_RENDER_END, 6);

class IBaseClientDll
{
	virtual ~IBaseClientDll() = 0;

	// Connect appsystem components, get global interfaces, don't run any other init code
	virtual i32 Connect(CreateInterfaceFn appSystemFactory, CGlobalVarsBase* globals) = 0;

	// run other init code here
	virtual i32 Init(CreateInterfaceFn appSystemFactory, CGlobalVarsBase* globals) = 0;

	virtual void PostInit() = 0;

	// Called once when the client DLL is being unloaded
	virtual void Shutdown() = 0;

	// Called at the start of each level change
	virtual void LevelInitPreEntity(char const* mapName) = 0;
	// Called at the start of a new level, after the entities have been received and created
	virtual void LevelInitPostEntity() = 0;
	// Called at the end of a level
	virtual void LevelShutdown() = 0;

	// Request a pointer to the list of client datatable classes
	virtual ClientClass* GetAllClasses() = 0;

	// Called once per level to re-initialize any hud element drawing stuff
	virtual i32 HudVidInit() = 0;
	// Called by the engine when gathering user input
	virtual void HudProcessInput(bool active) = 0;
	// Called oncer per frame to allow the hud elements to think
	virtual void HudUpdate(bool active) = 0;
	// Reset the hud elements to their initial states
	virtual void HudReset() = 0;
	// Display a hud text message
	virtual void HudText(const char* message) = 0;

	// Mouse Input Interfaces
	// Activate the mouse (hides the cursor and locks it to the center of the screen)
	virtual void IN_ActivateMouse() = 0;
	// Deactivates the mouse (shows the cursor and unlocks it)
	virtual void IN_DeactivateMouse() = 0;
	// This is only called during extra sound updates and just accumulates mouse x, y offets and recenters the mouse.
	//  This call is used to try to prevent the mouse from appearing out of the side of a windowed version of the engine if
	//  rendering or other processing is taking too long
	virtual void IN_Accumulate() = 0;
	// Reset all key and mouse states to their initial, unpressed state
	virtual void IN_ClearStates() = 0;
	// If key is found by name, returns whether it's being held down in isdown, otherwise function returns false
	virtual bool IN_IsKeyDown(const char* name, bool& isDown) = 0;
	// Raw keyboard signal, if the client .dll returns 1, the engine processes the key as usual, otherwise,
	//  if the client .dll returns 0, the key is swallowed.
	virtual i32 IN_KeyEvent(i32 eventcode, ButtonCode keynum, const char* currentBinding) = 0;

	// This function is called once per tick to create the player CUserCmd (used for prediction/physics simulation of the player)
	// Because the mouse can be sampled at greater than the tick interval, there is a separate input_sample_frametime, which
	//  specifies how much additional mouse / keyboard simulation to perform.
	virtual void CreateMove(
		i32 sequence_number, // sequence_number of this cmd
		f32 input_sample_frametime, // Frametime for mouse input sampling
		bool active) = 0; // True if the player is active (not paused)

	// If the game is running faster than the tick_interval framerate, then we do extra mouse sampling to avoid jittery input
	//  This code path is much like the normal move creation code, except no move is created
	virtual void ExtraMouseSample(f32 frametime, bool active) = 0;

	// Encode the delta (changes) between the CUserCmd in slot from vs the one in slot to.  The game code will have
	//  matching logic to read the delta.
	virtual bool WriteUsercmdDeltaToBuffer(i32 nSlot, bf_write* buf, i32 from, i32 to, bool isnewcommand) = 0;
	// Demos need to be able to encode/decode CUserCmds to memory buffers, so these functions wrap that
	virtual void EncodeUserCmdToBuffer(i32 nSlot, bf_write& buf, i32 slot) = 0;
	virtual void DecodeUserCmdFromBuffer(i32 nSlot, bf_read& buf, i32 slot) = 0;

	// Set up and render one or more views (e.g., rear view window, etc.).  This called into RenderView below
	virtual void View_Render(VRect* rect) = 0;

	// Allow engine to expressly render a view (e.g., during timerefresh)
	// See IVRenderView.h, PushViewFlags_t for nFlags values
	virtual void RenderView(const CViewSetup& view, i32 clearFlags, i32 whatToDraw) = 0;

	// Apply screen fade directly from engine
	virtual void View_Fade(ScreenFade* SF) = 0;

	// The engine has parsed a crosshair angle message, this function is called to dispatch the new crosshair angle
	virtual void SetCrosshairAngle(const QAngle& angle) = 0;

	// Sprite (.spr) model handling code
	// Load a .spr file by name
	virtual void InitSprite(CEngineSprite* sprite, const char* loadname) = 0;
	// Shutdown a .spr file
	virtual void ShutdownSprite(CEngineSprite* sprite) = 0;
	// Returns sizeof( CEngineSprite ) so the engine can allocate appropriate memory
	virtual i32 GetSpriteSize() const = 0;

	// Called when a player starts or stops talking.
	// entindex is -1 to represent the local client talking (before the data comes back from the server).
	// entindex is -2 to represent the local client's voice being acked by the server.
	// entindex is GetPlayer() when the server acknowledges that the local client is talking.
	virtual void VoiceStatus(i32 entindex, i32 iSsSlot, QBool talking) = 0;

	// Networked string table definitions have arrived, allow client .dll to
	//  hook string changes with a callback function ( see INetworkStringTableClient.h )
	virtual void InstallStringTableCallback(char const* tableName) = 0;

	// Notification that we're moving into another stage during the frame.
	virtual void FrameStageNotify(ClientFrameStage curStage) = 0;

	// The engine has received the specified user message, this code is used to dispatch the message handler
	virtual bool DispatchUserMessage(i32 msg_type, bf_read& msg_data) = 0;

	// Save/restore system hooks
	virtual CSaveRestoreData* SaveInit(i32 size) = 0;
	virtual void SaveWriteFields(CSaveRestoreData*, const char*, void*, datamap_t*, typedescription_t*, i32) = 0;
	virtual void SaveReadFields(CSaveRestoreData*, const char*, void*, datamap_t*, typedescription_t*, i32) = 0;
	virtual void PreSave(CSaveRestoreData*) = 0;
	virtual void Save(CSaveRestoreData*) = 0;
	virtual void WriteSaveHeaders(CSaveRestoreData*) = 0;
	virtual void ReadRestoreHeaders(CSaveRestoreData*) = 0;
	virtual void Restore(CSaveRestoreData*, bool) = 0;
	virtual void DispatchOnRestore() = 0;

	// Hand over the StandardRecvProxies in the client DLL's module.
	virtual CStandardRecvProxies* GetStandardRecvProxies() = 0;

	// save game screenshot writing
	virtual void WriteSaveGameScreenshot(const char* filename) = 0;

	// Given a list of "S(wavname) S(wavname2)" tokens, look up the localized text and emit
	//  the appropriate close caption if running with closecaption = 1
	virtual void EmitSentenceCloseCaption(char const* tokenstream) = 0;
	// Emits a regular close caption by token name
	virtual void EmitCloseCaption(char const* captionname, f32 duration) = 0;

	// Returns true if the client can start recording a demo now.  If the client returns false,
	// an error message of up to length bytes should be returned in errorMsg.
	virtual bool CanRecordDemo(char* errorMsg, i32 length) const = 0;

	// Give the Client a chance to do setup/cleanup.
	virtual void OnDemoRecordStart(char const* demoBaseName) = 0;
	virtual void OnDemoRecordStop() = 0;
	virtual void OnDemoPlaybackStart(char const* demoBaseName) = 0;
	virtual void OnDemoPlaybackStop() = 0;

	// Demo polish callbacks.
	virtual void RecordDemoPolishUserInput(i32 cmdIndex) = 0;

	// Cache replay ragdolls
	virtual bool CacheReplayRagdolls(const char* filename, i32 startTick) = 0;

	// Added interface

	// save game screenshot writing
	virtual void WriteSaveGameScreenshotOfSize(const char* filename, i32 width, i32 height) = 0;

	// Gets the current view
	virtual bool GetPlayerView(CViewSetup& playerView) = 0;

	virtual bool ShouldHideLoadingPlaque() = 0;

	virtual void InvalidateMdlCache() = 0;

	virtual void IN_SetSampleTime(f32 frametime) = 0;

	virtual void OnActiveSplitscreenPlayerChanged(i32 newSlot) = 0;
	// We are entering into/leaving split screen mode (or # of players is changing)
	virtual void OnSplitScreenStateChanged() = 0;

	virtual void CenterStringOff() = 0;

	virtual void OnScreenSizeChanged(i32 oldWidth, i32 oldHeight) = 0;

	virtual IMaterialProxy* InstantiateMaterialProxy(const char* proxyName) = 0;

	virtual VPANEL GetFullscreenClientDLLVPanel() = 0;

	// The engine wants to mark two entities as touching
	virtual void MarkEntitiesAsTouching(IClientEntity* e1, IClientEntity* e2) = 0;

	virtual void OnKeyBindingChanged(ButtonCode buttonCode, char const* keyName, char const* newBinding) = 0;

	virtual void SetBlurFade(f32 scale) = 0;

	virtual void ResetHudCloseCaption() = 0;

	// Called by the engine to allow the new GameUI to handle key events
	// Function must return true if the key event was handled
	virtual bool HandleGameUIEvent(const InputEvent& event) = 0;

	virtual bool SupportsRandomMaps() = 0;
};

#endif // SDK_IBASECLIENTDLL_HPP
