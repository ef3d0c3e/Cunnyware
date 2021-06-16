#ifndef SDK_IENGINECLIENT_HPP
#define SDK_IENGINECLIENT_HPP

#include "Defs.hpp"
#include "SurfInfo.hpp"
#include "IMaterial.hpp"
#include "PlayerInfo.hpp"
#include "ClientTextmessage.hpp"
#include "CAudioSource.hpp"
#include "ButtonCode.hpp"
#include "ISpatialQuery.hpp"
#include "IMaterialSystem.hpp"
#include "AudioState.hpp"
#include "INetChannelInfo.hpp"
#include "IAchievementMgr.hpp"
#include "ISPSharedMemory.hpp"
#include "CSteamAPIContext.hpp"

struct model_t;
struct Frustum_t;
class CPhysCollide;
class CGamestatsData;
struct InputContextHandle; // InputContextHandle_t

typedef void (*DemoCustomDataCallbackFn)(u8* data, std::size_t size);

static constexpr inline std::size_t MaxEdicts = 1<<11;

class IEngineClient
{
public:
	virtual  ~IEngineClient() = 0;

	// Find the model's surfaces that intersect the given sphere.
	// Returns the number of surfaces filled in.
	virtual i32 GetIntersectingSurfaces(
		const model_t* model,
		const Vec3& vCenter,
		const f32 radius,
		const bool bOnlyVisibleSurfaces, // Only return surfaces visible to vCenter.
		SurfInfo* pInfos,
		const i32 nMaxInfos) = 0;

	// Get the lighting intensivty for a specified point
// If bClamp is specified, the resulting Vec3 is restricted to the 0.0 to 1.0 for each element
	virtual Vec3 GetLightForPoint(const Vec3& pos, bool bClamp) = 0;

	// Traces the line and reports the material impacted as well as the lighting information for the impact point
	virtual IMaterial* TraceLineMaterialAndLighting(const Vec3& start, const Vec3& end, Vec3& diffuseLightColor, Vec3& baseColor) = 0;

	// Given an input text buffer data pointer, parses a single token into the variable token and returns the new
	//  reading position
	virtual const char* ParseFile(const char* data, char* token, i32 maxLen) = 0;
	virtual bool CopyFile(const char* source, const char* destination) = 0;

	// Gets the dimensions of the game window
	virtual void GetScreenSize(i32& width, i32& height) = 0;

	// Forwards szCmdString to the server, sent reliably if bReliable is set
	virtual void ServerCmd(const char* cmdString, bool reliable = true) = 0;
	// Inserts szCmdString into the command buffer as if it was typed by the client to his/her console.
	// Note: Calls to this are checked against FCVAR_CLIENTCMD_CAN_EXECUTE (if that bit is not set, then this function can't change it).
	//       Call ClientCmd_Unrestricted to have access to FCVAR_CLIENTCMD_CAN_EXECUTE vars.
	virtual void ClientCmd(const char* cmdString) = 0;

	// Fill in the player info structure for the specified player index (name, model, etc.)
	virtual bool GetPlayerInfo(i32 ent_num, PlayerInfo* info) = 0;

	// Retrieve the player entity number for a specified userID
	virtual i32 GetPlayerForUserID(i32 userID) = 0;

	// Retrieves text message system information for the specified message by name
	virtual ClientTextmessage* TextMessageGet(const char* name) = 0;

	// Returns true if the console is visible
	virtual bool Con_IsVisible() = 0;

	// Get the entity index of the local player
	virtual i32 GetLocalPlayer() = 0;

	// Client DLL is hooking a model, loads the model into memory and returns  pointer to the model_t
	virtual const model_t* LoadModel(const char* name, bool prop = false) = 0;

	// Get the exact server timesstamp ( server time ) from the last message received from the server
	virtual f32 GetLastTimeStamp() = 0;

	// Given a CAudioSource (opaque pointer), retrieve the underlying CSentence object ( stores the words, phonemes, and close
	//  captioning data )
	virtual CSentence* GetSentence(CAudioSource* audioSource) = 0;
	// Given a CAudioSource, determines the length of the underlying audio file (.wav, .mp3, etc.)
	virtual f32 GetSentenceLength(CAudioSource* audioSource) = 0;
	// Returns true if the sound is streaming off of the hard disk (instead of being memory resident)
	virtual bool IsStreaming(CAudioSource* audioSource) const = 0;

	// Copy current view orientation into va
	virtual void GetViewAngles(QAngle& va) = 0;
	// Set current view orientation from va
	virtual void SetViewAngles(QAngle& va) = 0;

	// Retrieve the current game's maxclients setting
	virtual i32 GetMaxClients() = 0;

	// Given the string pBinding which may be bound to a key,
	//  returns the string name of the key to which this string is bound. Returns NULL if no such binding exists
	virtual const char* Key_LookupBinding(const char* binding) = 0;

	// Given the name of the key "mouse1", "e", "tab", etc., return the string it is bound to "+jump", "impulse 50", etc.
	virtual const char* Key_BindingForKey(ButtonCode& code) = 0;

	// key trapping (for binding keys)
	virtual void StartKeyTrapMode() = 0;
	virtual bool CheckDoneKeyTrapping(ButtonCode& code) = 0;

	// Returns true if the player is fully connected and active in game (i.e, not still loading)
	virtual bool IsInGame() = 0;
	// Returns true if the player is connected, but not necessarily active in game (could still be loading)
	virtual bool IsConnected() = 0;
	// Returns true if the loading plaque should be drawn
	virtual bool IsDrawingLoadingImage() = 0;
	virtual void HideLoadingPlaque() = 0;

	// Prints the formatted string to the notification area of the screen ( down the right hand edge
	//  numbered lines starting at position 0
	virtual void Con_NPrintf(i32 pos, const char* fmt, ...) = 0;
	// Similar to Con_NPrintf, but allows specifying custom text color and duration information
	virtual void Con_NXPrintf(const struct con_nprint_s* info, const char* fmt, ...) = 0;

	// Is the specified world-space bounding box inside the view frustum?
	virtual i32 IsBoxVisible(const Vec3& mins, const Vec3& maxs) = 0;

	// Is the specified world-space boudning box in the same PVS cluster as the view origin?
	virtual i32 IsBoxInViewCluster(const Vec3& mins, const Vec3& maxs) = 0;

	// Returns true if the specified box is outside of the view frustum and should be culled
	virtual bool CullBox(const Vec3& mins, const Vec3& maxs) = 0;

	// Allow the sound system to paint additional data (during lengthy rendering operations) to prevent stuttering sound.
	virtual void Sound_ExtraUpdate() = 0;

	// Get the current game directory ( e.g., hl2, tf2, cstrike, hl1 )
	virtual const char* GetGameDirectory() = 0;

	// Get access to the world to screen transformation matrix
	virtual const Mat4x4& WorldToScreenMatrix() = 0;

	// Get the matrix to move a point from world space into view space
	// (translate and rotate so the camera is at the origin looking down X).
	virtual const Mat4x4& WorldToViewMatrix() = 0;

	// The .bsp file can have mod-specified data lumps. These APIs are for working with such game lumps.

	// Get mod-specified lump version id for the specified game data lump
	virtual i32 GameLumpVersion(i32 lumpId) const = 0;
	// Get the raw size of the specified game data lump.
	virtual i32 GameLumpSize(i32 lumpId) const = 0;
	// Loads a game lump off disk, writing the data into the buffer pointed to bye pBuffer
	// Returns false if the data can't be read or the destination buffer is too small
	virtual bool LoadGameLump(i32 lumpId, void* buffer, i32 size) = 0;

	// Returns the number of leaves in the level
	virtual i32 LevelLeafCount() const = 0;

	// Gets a way to perform spatial queries on the BSP tree
	virtual ISpatialQuery* GetBSPTreeQuery() = 0;

	// Convert texlight to gamma...
	virtual void LinearToGamma(f32* linear, f32* gamma) = 0;

	// Get the lightstyle value
	virtual f32 LightStyleValue(i32 style) = 0;

	// Computes light due to dynamic lighting at a point
	// If the normal isn't specified, then it'll return the maximum lighting
	virtual void ComputeDynamicLighting(const Vec3& point, const Vec3* normal, Vec3& color) = 0;

	// Returns the color of the ambient light
	virtual void GetAmbientLightColor(Vec3& color) = 0;

	// Returns the dx support level
	virtual i32 GetDXSupportLevel() = 0;

	// GR - returns the HDR support status
	virtual bool SupportsHDR() = 0;

	// Replace the engine's material system pointer.
	virtual void Mat_Stub(IMaterialSystem* pMatSys) = 0;

	// Get the name of the current map
	virtual void GetChapterName(char* pchBuff, i32 iMaxLength) = 0;
	virtual char const* GetLevelName() = 0;
	virtual char const* GetLevelNameShort() = 0;
#if !defined(NO_VOICE)
	// Obtain access to the voice tweaking API
	virtual struct IVoiceTweak_s* GetVoiceTweakAPI() = 0;
#endif
	// Tell engine stats gathering system that the rendering frame is beginning/ending
	virtual void EngineStats_BeginFrame() = 0;
	virtual void EngineStats_EndFrame() = 0;

	// This tells the engine to fire any events (temp entity messages) that it has queued up this frame.
	// It should only be called once per frame.
	virtual void FireEvents() = 0;

	// Returns an area index if all the leaves are in the same area. If they span multple areas, then it returns -1.
	virtual i32 GetLeavesArea(unsigned short* pLeaves, i32 leaves) = 0;

	// Returns true if the box touches the specified area's frustum.
	virtual bool DoesBoxTouchAreaFrustum(const Vec3& mins, const Vec3& maxs, i32 iArea) = 0;
	virtual i32 GetFrustumList(Frustum_t** list, i32 listMax) = 0;

	// Sets the hearing origin (i.e., the origin and orientation of the listener so that the sound system can spatialize
	//  sound appropriately ).
	virtual void SetAudioState(const AudioState& state) = 0;

	// Sentences / sentence groups
	virtual i32 SentenceGroupPick(i32 groupIndex, char* name, i32 nameBufLen) = 0;
	virtual i32 SentenceGroupPickSequential(i32 groupIndex, char* name, i32 nameBufLen, i32 sentenceIndex, i32 reset) = 0;
	virtual i32 SentenceIndexFromName(const char* sentenceName) = 0;
	virtual const char* SentenceNameFromIndex(i32 sentenceIndex) = 0;
	virtual i32 SentenceGroupIndexFromName(const char* pGroupName) = 0;
	virtual const char* SentenceGroupNameFromIndex(i32 groupIndex) = 0;
	virtual f32 SentenceLength(i32 sentenceIndex) = 0;

	// Computes light due to dynamic lighting at a point
	// If the normal isn't specified, then it'll return the maximum lighting
	// If pBoxColors is specified (it's an array of 6), then it'll copy the light contribution at each box side.
	virtual void ComputeLighting(const Vec3& pt, const Vec3* pNormal, bool bClamp, Vec3& color, Vec3* pBoxColors = NULL) = 0;

	// Activates/deactivates an occluder...
	virtual void ActivateOccluder(i32 nOccluderIndex, bool bActive) = 0;
	virtual bool IsOccluded(const Vec3& vecAbsMins, const Vec3& vecAbsMaxs) = 0;

	// The save restore system allocates memory from a shared memory pool, use this allocator to allocate/free saverestore
	//  memory.
	virtual void* SaveAllocMemory(size_t num, size_t size) = 0;
	virtual void SaveFreeMemory(void* pSaveMem) = 0;

	// returns info interface for client netchannel
	virtual INetChannelInfo* GetNetChannelInfo() = 0;

	// Debugging functionality:
	// Very slow routine to draw a physics model
	virtual void DebugDrawPhysCollide(const CPhysCollide* pCollide, IMaterial* pMaterial, const Mat3x4& transform, const ColorRGBA& color) = 0;
	// This can be used to notify test scripts that we're at a particular spot in the code.
	virtual void CheckPoint(const char* pName) = 0;
	// Draw portals if r_DrawPortals is set (Debugging only)
	virtual void DrawPortals() = 0;
	// Determine whether the client is playing back or recording a demo
	virtual bool IsPlayingDemo() = 0;
	virtual bool IsRecordingDemo() = 0;
	virtual bool IsPlayingTimeDemo() = 0;
	virtual i32 GetDemoRecordingTick() = 0;
	virtual i32 GetDemoPlaybackTick() = 0;
	virtual i32 GetDemoPlaybackStartTick() = 0;
	virtual f32 GetDemoPlaybackTimeScale() = 0;
	virtual i32 GetDemoPlaybackTotalTicks() = 0;
	// Is the game paused?
	virtual bool IsPaused() = 0;

	// What is the game timescale multiplied with the host_timescale?
	virtual f32 GetTimescale() const = 0;

	// Is the game currently taking a screenshot?
	virtual bool IsTakingScreenshot() = 0;
	// Is this a HLTV broadcast ?
	virtual bool IsHLTV() = 0;
	// Is this a Replay demo?
	virtual bool IsReplay() = 0;
	// is this level loaded as just the background to the main menu? (active, but unplayable)
	virtual bool IsLevelMainMenuBackground() = 0;
	// returns the name of the background level
	virtual void GetMainMenuBackgroundName(char* dest, i32 destlen) = 0;

	// Occlusion system control
	virtual void SetOcclusionParameters(const OcclusionParams& params) = 0;

	// What language is the user expecting to hear .wavs in, "english" or another...
	virtual void GetUILanguage(char* dest, i32 destlen) = 0;

	// Can skybox be seen from a particular point?
	virtual SkyboxVisibility IsSkyboxVisibleFromPoint(const Vec3& vecPoint) = 0;

	// Get the pristine map entity lump string.  (e.g., used by CS to reload the map entities when restarting a round.)
	virtual const char* GetMapEntitiesString() = 0;

	// Is the engine in map edit mode ?
	virtual bool IsInEditMode() = 0;

	// current screen aspect ratio (eg. 4.0f/3.0f, 16.0f/9.0f)
	virtual f32 GetScreenAspectRatio(i32 viewportWidth, i32 viewportHeight) = 0;

	// allow the game UI to login a user
	virtual bool REMOVED_SteamRefreshLogin(const char* password, bool isSecure) = 0;
	virtual bool REMOVED_SteamProcessCall(bool& finished) = 0;

	// allow other modules to know about engine versioning (one use is a proxy for network compatability)
	virtual u32 GetEngineBuildNumber() = 0; // engines build
	virtual const char* GetProductVersionString() = 0; // mods version number (steam.inf)

	// Communicates to the color correction editor that it's time to grab the pre-color corrected frame
	// Passes in the actual size of the viewport
	virtual void GrabPreColorCorrectedFrame(i32 x, i32 y, i32 width, i32 height) = 0;

	virtual bool IsHammerRunning() const = 0;

	// Inserts szCmdString into the command buffer as if it was typed by the client to his/her console.
	// And then executes the command string immediately (vs ClientCmd() which executes in the next frame)
	//
	// Note: this is NOT checked against the FCVAR_CLIENTCMD_CAN_EXECUTE vars.
	virtual void ExecuteClientCmd(const char* szCmdString) = 0;

	// returns if the loaded map was processed with HDR info. This will be set regardless
	// of what HDR mode the player is in.
	virtual bool MapHasHDRLighting() = 0;

	virtual i32 GetAppID() = 0;

	// Just get the leaf ambient light - no caching, no samples
	virtual Vec3 GetLightForPointFast(const Vec3& pos, bool bClamp) = 0;

	// This version does NOT check against FCVAR_CLIENTCMD_CAN_EXECUTE.
	virtual void ClientCmd_Unrestricted(const char* szCmdString) = 0;

	// This used to be accessible through the cl_restrict_server_commands cvar.
	// By default, Valve games restrict the server to only being able to execute commands marked with FCVAR_SERVER_CAN_EXECUTE.
	// By default, mods are allowed to execute any server commands, and they can restrict the server's ability to execute client
	// commands with this function.
	virtual void SetRestrictServerCommands(bool bRestrict) = 0;

	// If set to true (defaults to true for Valve games and false for others), then IVEngineClient::ClientCmd
	// can only execute things marked with FCVAR_CLIENTCMD_CAN_EXECUTE.
	virtual void SetRestrictClientCommands(bool bRestrict) = 0;

	// Sets the client renderable for an overlay's material proxy to bind to
	virtual void SetOverlayBindProxy(i32 overlayID, void* bindProxy) = 0;

	virtual bool CopyFrameBufferToMaterial(const char* pMaterialName) = 0;

	// Causes the engine to read in the user's configuration on disk
	virtual void ReadConfiguration(const i32 controller, const bool readDefault) = 0;

	virtual void SetAchievementMgr(IAchievementMgr* achievementMgr) = 0;
	virtual IAchievementMgr* GetAchievementMgr() = 0;

	virtual bool MapLoadFailed() = 0;
	virtual void SetMapLoadFailed(bool state) = 0;

	virtual bool IsLowViolence() = 0;
	virtual const char* GetMostRecentSaveGame() = 0;
	virtual void SetMostRecentSaveGame(const char* filename) = 0;

	virtual void StartXboxExitingProcess() = 0;
	virtual bool IsSaveInProgress() = 0;
	virtual u32 OnStorageDeviceAttached(i32 controller) = 0;
	virtual void OnStorageDeviceDetached(i32 controller) = 0;

	// generic screenshot writing
	virtual void WriteScreenshot(const char* filename) = 0;

	virtual void ResetDemoInterpolation() = 0;

	// For non-split screen games this will always be zero
	virtual i32 GetActiveSplitScreenPlayerSlot() = 0;
	virtual i32 SetActiveSplitScreenPlayerSlot(i32 slot) = 0;

	// This is the current # of players on the local host
	virtual bool SetLocalPlayerIsResolvable(char const* context, i32 line, bool resolvable) = 0;
	virtual bool IsLocalPlayerResolvable() = 0;

	virtual i32 GetSplitScreenPlayer(i32 slot) = 0;
	virtual bool IsSplitScreenActive() = 0;
	virtual bool IsValidSplitScreenSlot(i32 slot) = 0;
	virtual i32 FirstValidSplitScreenSlot() = 0; // -1 == invalid
	virtual i32 NextValidSplitScreenSlot(i32 previousSlot) = 0; // -1 == invalid

	//Finds or Creates a shared memory space, the returned pointer will automatically be AddRef()ed
	virtual ISPSharedMemory* GetSinglePlayerSharedMemorySpace(const char* name, i32 ent_num = MaxEdicts) = 0;

	// Computes an ambient cube that includes ALL dynamic lights
	virtual void ComputeLightingCube(const Vec3& pt, bool clamp, Vec3* boxColors) = 0;

	//All callbacks have to be registered before demo recording begins. TODO: Macro'ize a way to do it at startup
	virtual void RegisterDemoCustomDataCallback(string_t callbackSaveID, DemoCustomDataCallbackFn callback) = 0;
	virtual void RecordDemoCustomData(DemoCustomDataCallbackFn callback, const void* data, std::size_t dataLength) = 0;

	// global sound pitch scaling
	virtual void SetPitchScale(f32 flPitchScale) = 0;
	virtual f32 GetPitchScale() = 0;

	// Load/unload the SFM - used by Replay
	virtual bool LoadFilmmaker() = 0;
	virtual void UnloadFilmmaker() = 0;

	// leaf flag management. Allows fast leaf enumeration of leaves that have a flag set

	// set a bit in a leaf flag
	virtual void SetLeafFlag(i32 leafIndex, i32 flagBits) = 0;

	// you must call this once done modifying flags. Not super fast.
	virtual void RecalculateBSPLeafFlags() = 0;

	virtual bool DSPGetCurrentDASRoomNew() = 0;
	virtual bool DSPGetCurrentDASRoomChanged() = 0;
	virtual bool DSPGetCurrentDASRoomSkyAbove() = 0;
	virtual f32 DSPGetCurrentDASRoomSkyPercent() = 0;
	virtual void SetMixGroupOfCurrentMixer(const char* groupname, const char* szparam, f32 val, i32 setMixerType) = 0;
	virtual i32 GetMixLayerIndex(const char* mixlayername) = 0;
	virtual void SetMixLayerLevel(i32 index, f32 level) = 0;


	virtual bool IsCreatingReslist() = 0;
	virtual bool IsCreatingXboxReslist() = 0;

	virtual void SetTimescale(f32 timescale) = 0;

	// Methods to set/get a gamestats data container so client & server running in same process can send combined data
	virtual void SetGamestatsData(CGamestatsData* gamestatsData) = 0;
	virtual CGamestatsData* GetGamestatsData() = 0;

	// Given the string pBinding which may be bound to a key,
	//  returns the string name of the key to which this string is bound. Returns NULL if no such binding exists
	// Increment start count to iterate through multiple keys bound to the same binding
	// iAllowJoystick defaults to -1 witch returns joystick and non-joystick binds, 0 returns only non-joystick, 1 returns only joystick
	virtual const char* Key_LookupBindingEx(const char* binding, i32 userId = -1, i32 startCount = 0, i32 allowJoystick = -1) = 0;

	// Updates dynamic light state. Necessary for light cache to work properly for d- and elights
	virtual void UpdateDAndELights() = 0;

	// Methods to get bug count for internal dev work stat tracking.
	// Will get the bug count and clear it every map transition
	virtual i32 GetBugSubmissionCount() const = 0;
	virtual void ClearBugSubmissionCount() = 0;

	// Is there water anywhere in the level?
	virtual bool DoesLevelContainWater() const = 0;

	// How much time was spent in server simulation?
	virtual f32 GetServerSimulationFrameTime() const = 0;

	virtual void SolidMoved(class IClientEntity* solidEnt, class ICollideable* solidCollide, const Vec3* prevAbsOrigin, bool accurateBboxTriggerChecks) = 0;
	virtual void TriggerMoved(class IClientEntity* triggerEnt, bool accurateBboxTriggerChecks) = 0;

	// Using area bits, check whether the area of the specified point flows into the other areas
	virtual void ComputeLeavesConnected(const Vec3& vecOrigin, i32 count, const i32* leafIndices, bool* isConnected) = 0;

	// Is the engine in Commentary mode?
	virtual bool IsInCommentaryMode() = 0;

	virtual void SetBlurFade(f32 amount) = 0;
	virtual bool IsTransitioningToLoad() = 0;

	virtual void SearchPathsChangedAfterInstall() = 0;

	virtual void ConfigureSystemLevel(i32 CPULevel, i32 GPULevel) = 0;

	virtual void SetConnectionPassword(char const* currentPW) = 0;

	virtual CSteamAPIContext* GetSteamAPIContext() = 0;

	virtual void SubmitStatRecord(char const* mapName, u32 blobVersion, u32 blobSize, const void* blob) = 0;

	// Sends a key values server command, not allowed from scripts execution
	// Params:
	//	pKeyValues	- key values to be serialized and sent to server
	//				  the pointer is deleted inside the function: pKeyValues->deleteThis()
	virtual void ServerCmdKeyValues(KeyValues* keyValues) = 0;
	// Tells the engine what and where to paint
	virtual void PaintSurface(const model_t* model, const Vec3& position, const Color& color, f32 radius) = 0;
	// Enable paint in the engine for project Paint
	virtual void EnablePaintmapRender() = 0;
	virtual void TracePaintSurface(const model_t* model, const Vec3& position, f32 radius, class CUtlVector_Color& surfColors) = 0;
	virtual void RemoveAllPaint() = 0;

	virtual bool IsActiveApp() = 0;

	// is this client running inside the same process as an active server?
	virtual bool IsClientLocalToActiveServer() = 0;

	// Callback for LevelInit to tick the progress bar during time consuming operations
	virtual void TickProgressBar() = 0;

	// Returns the requested input context
	virtual InputContextHandle GetInputContext(EngineInputContextId id) = 0;

	// let client lock mouse to the window bounds
	virtual void SetMouseWindowLock(bool bLockToWindow) = 0;
};

#endif // SDK_IENGINECLIENT_HPP
