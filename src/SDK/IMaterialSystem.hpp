#ifndef SDK_IMATERIALSYSTEM_HPP
#define SDK_IMATERIALSYSTEM_HPP

#include "IMaterialProxyFactory.hpp"
#include "IMaterialSystemHardwareConfig.hpp"
#include "MaterialSystemInfo.hpp"
#include "AspectRatioInfo.hpp"
#include "IShader.hpp"

MAKE_CENUM_Q(MaterialThreadMode, i32,
	SINGLE, 0,
	QUEUED_SINGLE, 1,
	QUEUED_THREADED, 2);

MAKE_CENUM_Q(RenderTargetSizeMode, i32,
	NO_CHANGE, 0,			// Only allowed for render targets that don't want a depth buffer
	// (because if they have a depth buffer, the render target must be less than or equal to the size of the framebuffer).
	DEFAULT, 1,				// Don't play with the specified width and height other than making sure it fits in the framebuffer.
	PICMIP, 2,				// Apply picmip to the render target's width and height.
	HDR, 3,					// frame_buffer_width / 4
	FULL_FRAME_BUFFER, 4,	// Same size as frame buffer, or next lower power of 2 if we can't do that.
	OFFSCREEN, 5,			// Target of specified size, don't mess with dimensions
	FULL_FRAME_BUFFER_ROUNDED_UP, 6 // Same size as the frame buffer, rounded up if necessary for systems that can't do non-power of two textures.
);

MAKE_CENUM_Q(MaterialRenderTargetDepth, i32,
	SHARED, 0x0,
	SEPARATE, 0x1,
	NONE, 0x2,
	ONLY, 0x3);

typedef u16 MaterialHandle;

typedef void (*ModeChangeCallbackFn)();
typedef void (*MaterialBufferReleaseFn)(int changeFlags);
typedef void (*MaterialBufferRestoreFn)(int changeFlags);
typedef void (*EndFrameCleanupFn)();
typedef void (*OnLevelShutdownFn)(void* userData);
typedef bool (*EndFramePriorToNextContextFn)();

class MaterialSystemSortInfo; // MaterialSystem_SortInfo_t
class IMatRenderContext;
class MaterialLock; // MaterialLock_t
class MaterialContextType; // MaterialContextType_t
class GPUMemoryStats;
class IFileList;
class MaterialTextureInfo; // MaterialTextureInfo_t
class IClientMaterialSystem;
class IPaintMapTextureManager;
class IPaintMapDataManager;
class ApplicationPerformanceCountersInfo;
class ApplicationInstantCountersInfo;

class IMaterialSystem : public IAppSystem, public non_constructible
{
public:
	// Placeholder for API revision
	virtual bool Connect(CreateInterfaceFn factory) = 0;
	virtual void Disconnect() = 0;
	virtual void* QueryInterface(const char* interfaceName) = 0;
	virtual InitReturnVal Init() = 0;
	virtual void Shutdown() = 0;

	//---------------------------------------------------------
	// Initialization and shutdown
	//---------------------------------------------------------

	// Call this to initialize the material system
	// returns a method to create interfaces in the shader dll
	virtual CreateInterfaceFn Init(char const* shaderAPIDLL,
								   IMaterialProxyFactory* materialProxyFactory,
								   CreateInterfaceFn fileSystemFactory,
								   CreateInterfaceFn cvarFactory = NULL) = 0;

	// Call this to set an explicit shader version to use
	// Must be called before Init().
	virtual void SetShaderAPI(char const* shaderAPIDLL) = 0;

	// Must be called before Init(), if you're going to call it at all...
	virtual void SetAdapter(i32 adapter, i32 flags) = 0;

	// Call this when the mod has been set up, which may occur after init
	// At this point, the game + gamebin paths have been set up
	virtual void ModInit() = 0;
	virtual void ModShutdown() = 0;

	//---------------------------------------------------------
	//
	//---------------------------------------------------------
	virtual void SetThreadMode(MaterialThreadMode mode, i32 serviceThread = -1) = 0;
	virtual MaterialThreadMode GetThreadMode() = 0;
	virtual void ExecuteQueued() = 0;

	virtual void OnDebugEvent(const char* event) = 0;

	//---------------------------------------------------------
	// Config management
	//---------------------------------------------------------

	virtual IMaterialSystemHardwareConfig* GetHardwareConfig(const char* version, i32* returnCode) = 0;


	// Call this before rendering each frame with the current config
	// for the material system.
	// Will do whatever is necessary to get the material system into the correct state
	// upon configuration change. .doesn't much else otherwise.
	virtual bool UpdateConfig(bool forceUpdate) = 0;

	// Force this to be the config; update all material system convars to match the state
	// return true if lightmaps need to be redownloaded
	virtual bool OverrideConfig(const MaterialSystemConfig& config, bool forceUpdate) = 0;

	// Get the current config for this video card (as last set by UpdateConfig)
	virtual const MaterialSystemConfig& GetCurrentConfigForVideoCard() const = 0;

	// Gets *recommended* configuration information associated with the display card,
	// given a particular dx level to run under.
	// Use dxlevel 0 to use the recommended dx level.
	// The function returns false if an invalid dxlevel was specified

	// UNDONE: To find out all convars affected by configuration, we'll need to change
	// the dxsupport.pl program to output all column headers into a single keyvalue block
	// and then we would read that in, and send it back to the client
	virtual bool GetRecommendedConfigurationInfo(i32 DXLevel, KeyValues* keyValues) = 0;


	// -----------------------------------------------------------
	// Device methods
	// -----------------------------------------------------------

	// Gets the number of adapters...
	virtual i32 GetDisplayAdapterCount() const = 0;

	// Returns the current adapter in use
	virtual i32 GetCurrentAdapter() const = 0;

	// Returns info about each adapter
	virtual void GetDisplayAdapterInfo(i32 adapter, MaterialAdapterInfo& info) const = 0;

	// Returns the number of modes
	virtual i32 GetModeCount(i32 adapter) const = 0;

	// Returns mode information..
	virtual void GetModeInfo(i32 adapter, i32 mode, MaterialVideoMode& info) const = 0;

	virtual void AddModeChangeCallBack(ModeChangeCallbackFn func) = 0;

	// Returns the mode info for the current display device
	virtual void GetDisplayMode(MaterialVideoMode& mode) const = 0;

	// Sets the mode...
	virtual bool SetMode(void* hwnd, const MaterialSystemConfig& config) = 0;

	virtual bool SupportsMSAAMode(i32 nMSAAMode) = 0;

	// FIXME: REMOVE! Get video card identitier
	virtual const MaterialSystemHardwareIdentifier& GetVideoCardIdentifier() const = 0;

	// Use this to spew information about the 3D layer
	virtual void SpewDriverInfo() const = 0;

	// Get the image format of the back buffer. . useful when creating render targets, etc.
	virtual void GetBackBufferDimensions(i32& width, i32& height) const = 0;
	virtual ImageFormat GetBackBufferFormat() const = 0;

	virtual const AspectRatioInfo& GetAspectRatioInfo() const = 0;

	virtual bool SupportsHDRMode(HDRType HDRModede) = 0;


	// -----------------------------------------------------------
	// Window methods
	// -----------------------------------------------------------

	// Creates/ destroys a child window
	virtual bool AddView(void* hwnd) = 0;
	virtual void RemoveView(void* hwnd) = 0;

	// Sets the view
	virtual void SetView(void* hwnd) = 0;


	// -----------------------------------------------------------
	// Control flow
	// -----------------------------------------------------------

	virtual void BeginFrame(f32 frameTime) = 0;
	virtual void EndFrame() = 0;
	virtual void Flush(bool flushHardware = false) = 0;

	virtual unsigned int GetCurrentFrameCount() = 0;

	/// FIXME: This stuff needs to be cleaned up and abstracted.
	// Stuff that gets exported to the launcher through the engine
	virtual void SwapBuffers() = 0;

	// Flushes managed textures from the texture cacher
	virtual void EvictManagedResources() = 0;

	virtual void ReleaseResources() = 0;
	virtual void ReacquireResources() = 0;


	// -----------------------------------------------------------
	// Device loss/restore
	// -----------------------------------------------------------

	// Installs a function to be called when we need to release vertex buffers + textures
	virtual void AddReleaseFunc(MaterialBufferReleaseFn func) = 0;
	virtual void RemoveReleaseFunc(MaterialBufferReleaseFn func) = 0;

	// Installs a function to be called when we need to restore vertex buffers
	virtual void AddRestoreFunc(MaterialBufferRestoreFn func) = 0;
	virtual void RemoveRestoreFunc(MaterialBufferRestoreFn func) = 0;

	// Installs a function to be called when we need to delete objects at the end of the render frame
	virtual void AddEndFrameCleanupFunc(EndFrameCleanupFn func) = 0;
	virtual void RemoveEndFrameCleanupFunc(EndFrameCleanupFn func) = 0;

	virtual void OnLevelShutdown() = 0;
	virtual bool AddOnLevelShutdownFunc(OnLevelShutdownFn func, void* userData) = 0;
	virtual bool RemoveOnLevelShutdownFunc(OnLevelShutdownFn func, void* userData) = 0;
	virtual void OnLevelLoadingComplete() = 0;

	// Release temporary HW memory...
	virtual void ResetTempHWMemory(bool exitingLevel = false) = 0;

	// For dealing with device lost in cases where SwapBuffers isn't called all the time (Hammer)
	virtual void HandleDeviceLost() = 0;


	// -----------------------------------------------------------
	// Shaders
	// -----------------------------------------------------------

	// Used to iterate over all shaders for editing purposes
	// GetShaders returns the number of shaders it actually found
	virtual int ShaderCount() const = 0;
	virtual int GetShaders(i32 firstShader, i32 maxCount, IShader** shaderList) const = 0;

	// FIXME: Is there a better way of doing this?
	// Returns shader flag names for editors to be able to edit them
	virtual int ShaderFlagCount() const = 0;
	virtual const char* ShaderFlagName(i32 index) const = 0;

	// Gets the actual shader fallback for a particular shader
	virtual void GetShaderFallback(const char* shaderName, char* fallbackShader, i32 fallbackLength) = 0;


	// -----------------------------------------------------------
	// Material proxies
	// -----------------------------------------------------------

	virtual IMaterialProxyFactory* GetMaterialProxyFactory() = 0;

	// Sets the material proxy factory. Calling this causes all materials to be uncached.
	virtual void SetMaterialProxyFactory(IMaterialProxyFactory* factory) = 0;


	// -----------------------------------------------------------
	// Editor mode
	// -----------------------------------------------------------

	// Used to enable editor materials. Must be called before Init.
	virtual void EnableEditorMaterials() = 0;
	virtual void EnableGBuffers() = 0;

	// -----------------------------------------------------------
	// Stub mode mode
	// -----------------------------------------------------------

	// Force it to ignore Draw calls.
	virtual void SetInStubMode(bool inStubMode) = 0;


	//---------------------------------------------------------
	// Debug support
	//---------------------------------------------------------

	virtual void DebugPrintUsedMaterials(const char* searchSubString, bool verbose) = 0;
	virtual void DebugPrintUsedTextures() = 0;

	virtual void ToggleSuppressMaterial(char const* materialName) = 0;
	virtual void ToggleDebugMaterial(char const* materialName) = 0;


	//---------------------------------------------------------
	// Misc features
	//---------------------------------------------------------
	//returns whether fast clipping is being used or not - needed to be exposed for better per-object clip behavior
	virtual bool UsingFastClipping() = 0;

	virtual i32 StencilBufferBits() = 0; //number of bits per pixel in the stencil buffer


	//---------------------------------------------------------
	// Material and texture management
	//---------------------------------------------------------

	// uncache all materials. .  good for forcing reload of materials.
	virtual void UncacheAllMaterials() = 0;

	// Remove any materials from memory that aren't in use as determined
	// by the IMaterial's reference count.
	virtual void UncacheUnusedMaterials(bool recomputeStateSnapshots = false) = 0;

	// Load any materials into memory that are to be used as determined
	// by the IMaterial's reference count.
	virtual void CacheUsedMaterials() = 0;

	// Force all textures to be reloaded from disk.
	virtual void ReloadTextures() = 0;

	// Reloads materials
	virtual void ReloadMaterials(const char* subString = NULL) = 0;

	// Create a procedural material. The keyvalues looks like a VMT file
	virtual IMaterial* CreateMaterial(const char* materialName, KeyValues* VMTKeyValues) = 0;

	// Find a material by name.
	// The name of a material is a full path to
	// the vmt file starting from "hl2/materials" (or equivalent) without
	// a file extension.
	// eg. "dev/dev_bumptest" refers to somethign similar to:
	// "d:/hl2/hl2/materials/dev/dev_bumptest.vmt"
	//
	// Most of the texture groups for pTextureGroupName are listed in texture_group_names.h.
	//
	// Note: if the material can't be found, this returns a checkerboard material. You can
	// find out if you have that material by calling IMaterial::IsErrorMaterial().
	// (Or use the global IsErrorMaterial function, which checks if it's null too).
	virtual IMaterial* FindMaterial(char const* materialName, const char* textureGroupName, bool complain = true, const char* complainPrefix = NULL) = 0;

	//---------------------------------
	// This is the interface for knowing what materials are available
	// is to use the following functions to get a list of materials.  The
	// material names will have the full path to the material, and that is the
	// only way that the directory structure of the materials will be seen through this
	// interface.
	// NOTE:  This is mostly for worldcraft to get a list of materials to put
	// in the "texture" browser.in Worldcraft
	virtual MaterialHandle FirstMaterial() const = 0;

	// returns InvalidMaterial if there isn't another material.
	// WARNING: you must call GetNextMaterial until it returns NULL,
	// otherwise there will be a memory leak.
	virtual MaterialHandle NextMaterial(MaterialHandle h) const = 0;

	// This is the invalid material
	virtual MaterialHandle InvalidMaterial() const = 0;

	// Returns a particular material
	virtual IMaterial* GetMaterial(MaterialHandle h) const = 0;

	// Get the total number of materials in the system.  These aren't just the used
	// materials, but the complete collection.
	virtual int GetNumMaterials() const = 0;

	//---------------------------------

	virtual ITexture* FindTexture(char const* textureName, const char* textureGroupName, bool complain = true) = 0;

	// Checks to see if a particular texture is loaded
	virtual bool IsTextureLoaded(char const* textureName) const = 0;

	// Creates a procedural texture
	virtual ITexture* CreateProceduralTexture(const char* textureName,
											  const char* textureGroupName,
											  i32 w,
											  i32 h,
											  ImageFormat fmt,
											  i32 nFlags) = 0;

	//
	// Render targets
	//
	virtual void BeginRenderTargetAllocation() = 0;
	virtual void EndRenderTargetAllocation() = 0; // Simulate an Alt-Tab in here, which causes a release/restore of all resources

	// Creates a render target
	// If depth == true, a depth buffer is also allocated. If not, then
	// the screen's depth buffer is used.
	// Creates a texture for use as a render target
	virtual ITexture* CreateRenderTargetTexture(i32 w,
												i32 h,
												RenderTargetSizeMode sizeMode, // Controls how size is generated (and regenerated on video mode change).
												ImageFormat format,
												MaterialRenderTargetDepth depth = MaterialRenderTargetDepth::SHARED) = 0;

	virtual ITexture* CreateNamedRenderTargetTextureEx(const char* pRTName, // Pass in NULL here for an unnamed render target.
													   i32 w,
													   i32 h,
													   RenderTargetSizeMode sizeMode, // Controls how size is generated (and regenerated on video mode change).
													   ImageFormat format,
													   MaterialRenderTargetDepth depth = MaterialRenderTargetDepth::SHARED,
													   u32 textureFlags = CompiledVtfFlags::TEXTUREFLAGS_CLAMPS | CompiledVtfFlags::TEXTUREFLAGS_CLAMPT,
													   u32 renderTargetFlags = 0) = 0;

	virtual ITexture* CreateNamedRenderTargetTexture(const char* pRTName,
													 i32 w,
													 i32 h,
													 RenderTargetSizeMode sizeMode, // Controls how size is generated (and regenerated on video mode change).
													 ImageFormat format,
													 MaterialRenderTargetDepth depth = MaterialRenderTargetDepth::SHARED,
													 bool bClampTexCoords = true,
													 bool bAutoMipMap = false) = 0;

	// Must be called between the above Begin-End calls!
	virtual ITexture* CreateNamedRenderTargetTextureEx2(const char* pRTName, // Pass in NULL here for an unnamed render target.
														i32 w,
														i32 h,
														RenderTargetSizeMode sizeMode, // Controls how size is generated (and regenerated on video mode change).
														ImageFormat format,
														MaterialRenderTargetDepth depth = MaterialRenderTargetDepth::SHARED,
														u32 textureFlags = CompiledVtfFlags::TEXTUREFLAGS_CLAMPS | CompiledVtfFlags::TEXTUREFLAGS_CLAMPT,
														u32 renderTargetFlags = 0) = 0;

	// -----------------------------------------------------------
	// Lightmaps
	// -----------------------------------------------------------

	// To allocate lightmaps, sort the whole world by material twice.
	// The first time through, call AllocateLightmap for every surface.
	// that has a lightmap.
	// The second time through, call AllocateWhiteLightmap for every
	// surface that expects to use shaders that expect lightmaps.
	virtual void BeginLightmapAllocation() = 0;
	virtual void EndLightmapAllocation() = 0;

	virtual void CleanupLightmaps() = 0;

	// returns the sorting id for this surface
	virtual i32 AllocateLightmap(i32 width, i32 height, i32 offsetIntoLightmapPage[2], IMaterial* pMaterial) = 0;
	// returns the sorting id for this surface
	virtual i32 AllocateWhiteLightmap(IMaterial* pMaterial) = 0;

	// lightmaps are in linear color space
	// lightmapPageID is returned by GetLightmapPageIDForSortID
	// lightmapSize and offsetIntoLightmapPage are returned by AllocateLightmap.
	// You should never call UpdateLightmap for a lightmap allocated through
	// AllocateWhiteLightmap.
	virtual void UpdateLightmap(i32 lightmapPageID, i32 lightmapSize[2], i32 offsetIntoLightmapPage[2], f32* pFloatImage, f32* pFloatImageBump1, f32* pFloatImageBump2, f32* pFloatImageBump3) = 0;

	// fixme: could just be an array of ints for lightmapPageIDs since the material
	// for a surface is already known.
	virtual i32 GetNumSortIDs() = 0;
	virtual void GetSortInfo(MaterialSystemSortInfo* sortInfoArray) = 0;

	// Read the page size of an existing lightmap by sort id (returned from AllocateLightmap())
	virtual void GetLightmapPageSize(i32 lightmap, i32* width, i32* height) const = 0;

	virtual void ResetMaterialLightmapPageInfo() = 0;

	virtual bool IsStereoSupported() = 0;
	virtual bool IsStereoActiveThisFrame() const = 0;
	virtual void NVStereoUpdate() = 0;

	virtual void ClearBuffers(bool clearColor, bool clearDepth, bool clearStencil = false) = 0;

	virtual void SpinPresent(u16 frames) = 0;

	// -----------------------------------------------------------
	// Access the render contexts
	// -----------------------------------------------------------
	virtual IMatRenderContext* GetRenderContext() = 0;

	virtual void BeginUpdateLightmaps() = 0;
	virtual void EndUpdateLightmaps() = 0;

	// -----------------------------------------------------------
	// Methods to force the material system into non-threaded, non-queued mode
	// -----------------------------------------------------------
	virtual MaterialLock Lock() = 0;
	virtual void Unlock(MaterialLock) = 0;

	// Create a custom render context. Cannot be used to create MATERIAL_HARDWARE_CONTEXT
	virtual IMatRenderContext* CreateRenderContext(MaterialContextType type) = 0;

	// Set a specified render context to be the global context for the thread. Returns the prior context.
	virtual IMatRenderContext* SetRenderContext(IMatRenderContext*) = 0;

	virtual bool SupportsCSAAMode(i32 numSamples, i32 qualityLevel) = 0;

	virtual void RemoveModeChangeCallBack(ModeChangeCallbackFn func) = 0;

	// Finds or create a procedural material.
	virtual IMaterial* FindProceduralMaterial(const char* materialName, const char* textureGroupName, KeyValues* VMTKeyValues) = 0;

	virtual void AddTextureAlias(const char* alias, const char* realName) = 0;
	virtual void RemoveTextureAlias(const char* alias) = 0;

	// returns a lightmap page ID for this allocation, -1 if none available
	// frameID is a number that should be changed every frame to prevent locking any textures that are
	// being used to draw in the previous frame
	virtual i32 AllocateDynamicLightmap(i32 lightmapSize[2], i32* pOutOffsetIntoPage, i32 frameID) = 0;

	virtual void SetExcludedTextures(const char* scriptName, bool usingWeaponModelCache) = 0;
	virtual void UpdateExcludedTextures() = 0;

	virtual bool IsInFrame() const = 0;

	virtual void CompactMemory() = 0;

	virtual void GetGPUMemoryStats(GPUMemoryStats& stats) = 0;

	// For sv_pure mode. The filesystem figures out which files the client needs to reload to be "pure" ala the server's preferences.
	virtual void ReloadFilesInList(IFileList* filesToReload) = 0;

	// Get information about the texture for texture management tools
	virtual bool GetTextureInformation(char const* textureName, MaterialTextureInfo& info) const = 0;

	// call this once the render targets are allocated permanently at the beginning of the game
	virtual void FinishRenderTargetAllocation() = 0;

	virtual void ReEnableRenderTargetAllocation_IRealizeIfICallThisAllTexturesWillBeUnloadedAndLoadTimeWillSufferHorribly() = 0;
	virtual bool AllowThreading(bool bAllow, i32 serviceThread) = 0;

	virtual bool GetRecommendedVideoConfig(KeyValues* keyValues) = 0;

	virtual IClientMaterialSystem* GetClientMaterialSystemInterface() = 0;

	virtual bool CanDownloadTextures() const = 0;
	virtual i32 GetNumLightmapPages() const = 0;

	virtual IPaintMapTextureManager* RegisterPaintMapDataManager(IPaintMapDataManager* dataManager) = 0; //You supply an interface we can query for bits, it gives back an interface you can use to drive updates

	virtual void BeginUpdatePaintmaps() = 0;
	virtual void EndUpdatePaintmaps() = 0;
	virtual void UpdatePaintmap(i32 paintmap, u8* paintData, i32 numRects, Rect2i* pRects) = 0;

	virtual ITexture* CreateNamedMultiRenderTargetTexture(const char* pRTName, i32 w, i32 h, RenderTargetSizeMode sizeMode, ImageFormat format, MaterialRenderTargetDepth depth, u32 textureFlags, u32 renderTargetFlags) = 0;

	virtual void RefreshFrontBufferNonInteractive() = 0;

	virtual u32 GetFrameTimestamps(ApplicationPerformanceCountersInfo& apci, ApplicationInstantCountersInfo& aici) = 0;

	virtual void DoStartupShaderPreloading() = 0;

	virtual void AddEndFramePriorToNextContextFunc(EndFramePriorToNextContextFn func) = 0;
	virtual void RemoveEndFramePriorToNextContextFunc(EndFramePriorToNextContextFn func) = 0;
};

#endif // SDK_IMATERIALSYSTEM_HPP
