#ifndef SDK_IVMODELINFO_HPP
#define SDK_IVMODELINFO_HPP

#include "Defs.hpp"
#include "CUtl/Vector.hpp"
#include "CUtl/Buffer.hpp"
class IMaterial;
class IClientRenderable;
struct StudioHdr;
struct model_t;
struct vcollide_t;
struct trace_t;
struct virtualmodel_t;
class CPhysCollide;
struct cplane_t;
struct SCombinerModelInput; // SCombinerModelInput_t
struct CombinedModelLoadedCallback; // TODO: typedef
class IModelLoadCallback;
class KeyValues;

MAKE_CENUM_Q(RenderableTranslucencyType, i32,
	IS_OPAQUE, 0,
	IS_TRANSLUCENT, 1,
	IS_TWO_PASS, 2, // Has both translucent and opaque sub-partsa
);

// For SetupBones
MAKE_CENUM_Q(BoneMask, i32,
	MASK, 0x000FFF00,
	ANYTHING, 0x000FFF00,
	HITBOX, 0x00000100, // Bone (or child) is used by a hit box
	ATTACHMENT, 0x00000200, // Bone (or child) is used by an attachment point
	VERTEX_MASK, 0x0003FC00,
	VERTEX_LOD0, 0x00000400, // Bone (or child) is used by the toplevel model via skinned vertex
	VERTEX_LOD1, 0x00000800,
	VERTEX_LOD2, 0x00001000,
	VERTEX_LOD3, 0x00002000,
	VERTEX_LOD4, 0x00004000,
	VERTEX_LOD5, 0x00008000,
	VERTEX_LOD6, 0x00010000,
	VERTEX_LOD7, 0x00020000,
	BONE_MERGE, 0x00040000, // Bone is available for bone merge to occur against it
	ALWAYS_SETUP, 0x00080000);

class IVModelInfo
{
public:
	virtual ~IVModelInfo() = 0;

	virtual const model_t* GetModel(i32 modelIdx) const = 0;
	// Returns index of model by name
	virtual i32 GetModelIndex(const char* name) const = 0;

	// Returns name of model
	virtual const char* GetModelName(const model_t* model) const = 0;
	virtual vcollide_t* GetVCollide(const model_t* model) const = 0;
	virtual vcollide_t* GetVCollide(i32 modelIdx) const = 0;
	virtual void GetModelBounds(const model_t* model, Vec3& mins, Vec3& maxs) const = 0;
	virtual void GetModelRenderBounds(const model_t* model, Vec3& mins, Vec3& maxs) const = 0;
	virtual i32 GetModelFrameCount(const model_t* model) const = 0;
	virtual i32 GetModelType(const model_t* model) const = 0;
	virtual void* GetModelExtraData(const model_t* model) = 0;
	virtual bool ModelHasMaterialProxy(const model_t* model) const = 0;
	virtual bool IsTranslucent(model_t const* model) const = 0;
	virtual bool IsTranslucentTwoPass(const model_t* model) const = 0;
	virtual void Unused0() = 0;
	virtual RenderableTranslucencyType ComputeTranslucencyType(const model_t* model, i32 skin, i32 body) = 0;
	virtual i32 GetModelMaterialCount(const model_t* model) const = 0;
	virtual i32 GetModelMaterials(const model_t* model, i32 count, IMaterial** material) = 0;
	virtual bool IsModelVertexLit(const model_t* model) const = 0;
	virtual const char* GetModelKeyValueText(const model_t* model) = 0;
	virtual bool GetModelKeyValue(const model_t* model, CUtlBuffer& buf) = 0; // supports keyvalue blocks in submodels
	virtual f32 GetModelRadius(const model_t* model) = 0;

	virtual const StudioHdr* FindModel(const StudioHdr* pStudioHdr, void** cache, const char* modelname) const = 0;
	virtual const StudioHdr* FindModel(void* cache) const = 0;
	virtual virtualmodel_t* GetVirtualModel(const StudioHdr* studioHdr) const = 0;
	virtual u8* GetAnimBlock(const StudioHdr* studioHdr, i32 block, bool preloadIfMissing) const = 0;
	virtual bool HasAnimBlockBeenPreloaded(const StudioHdr* studioHdr, i32 block) const = 0;

	// Available on client only!!!
	virtual void GetModelMaterialColorAndLighting(const model_t* model, Vec3 const& origin, QAngle const& angles, trace_t* trace, Vec3& lighting, Vec3& matColor) = 0;
	virtual void GetIlluminationPoint(const model_t* model, IClientRenderable* renderable, Vec3 const& origin, QAngle const& angles, Vec3* lightingCenter) = 0;

	virtual i32 GetModelContents(i32 modelIdx) const = 0;
	virtual StudioHdr* GetStudioModel(const model_t* mod) = 0;
	virtual i32 GetModelSpriteWidth(const model_t* model) const = 0;
	virtual i32 GetModelSpriteHeight(const model_t* model) const = 0;

	// Sets/gets a map-specified fade range (client only)
	virtual void SetLevelScreenFadeRange(f32 minSize, f32 maxSize) = 0;
	virtual void GetLevelScreenFadeRange(f32* minArea, f32* maxArea) const = 0;

	// Sets/gets a map-specified per-view fade range (client only)
	virtual void SetViewScreenFadeRange(f32 minSize, f32 maxSize) = 0;

	// Computes fade alpha based on distance fade + screen fade (client only)
	virtual unsigned char ComputeLevelScreenFade(const Vec3& absOrigin, f32 radius, f32 fadeScale) const = 0;
	virtual unsigned char ComputeViewScreenFade(const Vec3& absOrigin, f32 radius, f32 fadeScale) const = 0;

	// both client and server
	virtual i32 GetAutoplayList(const StudioHdr* studioHdr, u16** autoplayList) const = 0;

	// Gets a virtual terrain collision model (creates if necessary)
	// NOTE: This may return NULL if the terrain model cannot be virtualized
	virtual CPhysCollide* GetCollideForVirtualTerrain(i32 index) = 0;

	virtual bool IsUsingFBTexture(const model_t* model, i32 skin, i32 body, void /*IClientRenderable*/* clientRenderable) const = 0;

	virtual const model_t* FindOrLoadModel(const char* name) const = 0;

	virtual MDLHandle GetCacheHandle(const model_t* model) const = 0;

	// Returns planes of non-nodraw brush model surfaces
	virtual i32 GetBrushModelPlaneCount(const model_t* model) const = 0;
	virtual void GetBrushModelPlane(const model_t* model, i32 index, cplane_t& plane, Vec3* irigin) const = 0;
	virtual i32 GetSurfacepropsForVirtualTerrain(i32 index) = 0;
	virtual bool UsesEnvCubemap(const model_t* model) const = 0;
	virtual bool UsesStaticLighting(const model_t* model) const = 0;

	// Returns index of model by name, dynamically registered if not already known.
	virtual i32 RegisterDynamicModel(const char* name, bool clientSide) = 0;
	virtual i32 RegisterCombinedDynamicModel(const char* name, MDLHandle Handle) = 0;
	virtual void UpdateCombinedDynamicModel(i32 modelIndex, MDLHandle Handle) = 0;

	virtual i32 BeginCombinedModel(const char* pszName, bool bReuseExisting) = 0;
	virtual bool SetCombineModels(i32 modelIdw, const CUtlVector<SCombinerModelInput>& vecModelsToCombine) = 0;
	virtual bool FinishCombinedModel(i32 modelIdx, CombinedModelLoadedCallback func, void* pUserData = NULL) = 0;
	virtual void ReleaseCombinedModel(i32 modelIdx) = 0;

	virtual bool IsDynamicModelLoading(i32 modelIndex) = 0;
	virtual void AddRefDynamicModel(i32 modelIndex) = 0;
	virtual void ReleaseDynamicModel(i32 modelIndex) = 0;

	// Registers callback for when dynamic model has finished loading.
	virtual bool RegisterModelLoadCallback(i32 modelindex, IModelLoadCallback* pCallback, bool bCallImmediatelyIfLoaded = true) = 0;
	virtual void UnregisterModelLoadCallback(i32 modelindex, IModelLoadCallback* pCallback) = 0;

	// Poked by engine
	virtual void OnLevelChange() = 0;

	virtual KeyValues* GetModelKeyValues(const model_t* model) = 0;

	virtual void UpdateViewWeaponModelCache(const char** ppWeaponModels, i32 weaponModels) = 0;
	virtual void TouchWorldWeaponModelCache(const char** ppWeaponModels, i32 weaponModels) = 0;
};

#endif // SDK_IVMODELINFO_HPP
