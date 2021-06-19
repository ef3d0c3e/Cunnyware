#ifndef SDK_IVMODELRENDER_HPP
#define SDK_IVMODELRENDER_HPP

#include "Defs.hpp"
class IClientRenderable;
typedef u16 ModelInstanceHandle;
struct model_t;
class IMaterial;
struct OverrideType;
struct CStudioHdr;
struct LightCacheHandle;
struct Ray;
struct DrawModelInfo;
class IMatRenderContext;
struct DrawModelInfo;
struct DrawModelState;
class ITexture;
struct MaterialLightingState;
struct MaterialLightingState;
struct StudioDecalHandle;
struct DataCacheHandle;

MAKE_CENUM_Q(OverrideType, u32,
	NORMAL, 0,
	BUILD_SHADOWS, 1,
	DEPTH_WRITE, 2,
	SELECTIVE, 3,
	SSAO_DEPTH_WRITE, 4);

struct ModelRenderInfo
{
	Vec3 origin;
	QAngle angles;
	IClientRenderable* renderable;
	const model_t* model;
	const Mat3x4* modelToWorld;
	const Mat3x4* lightingOffset;
	const Vec3* lightingOrigin;
	i32 flags;
	i32 entityIndex;
	i32 skin;
	i32 body;
	i32 hitboxset;
	ModelInstanceHandle instance;

	ModelRenderInfo()
	{
		modelToWorld = NULL;
		lightingOffset = NULL;
		lightingOrigin = NULL;
	}
};

struct StaticPropRenderInfo
{
	const Mat3x4* modelToWorld;
	const model_t* model;
	IClientRenderable* renderable;
	Vec3* lightingOrigin;
	ModelInstanceHandle instance;
	u8 skin;
	u8 alpha;
};

struct LightingQuery
{
	Vec3 lightingOrigin;
	ModelInstanceHandle instanceHandle;
	bool ambientBoost;
};

struct StaticLightingQuery : public LightingQuery
{
	IClientRenderable* renderable;
};


class IMesh;
class IPooledVBAllocator;
struct ColorMeshInfo
{
	// A given color mesh can own a unique Mesh, or it can use a shared Mesh
	// (in which case it uses a sub-range defined by m_nVertOffset and m_nNumVerts)
	IMesh* mesh;
	IPooledVBAllocator* pooledVBAllocator;
	i32 vertOffsetInBytes;
	i32 numVerts;
};

class IVModelRender
{
public:
	virtual i32 DrawModel(i32 flags,
						  IClientRenderable* pRenderable,
						  ModelInstanceHandle instance,
						  i32 entity_index,
						  const model_t* model,
						  Vec3 const& origin,
						  QAngle const& angles,
						  i32 skin,
						  i32 body,
						  i32 hitboxset,
						  const Mat3x4* modelToWorld = NULL,
						  const Mat3x4* pLightingOffset = NULL) = 0;

	// This causes a material to be used when rendering the model instead
	// of the materials the model was compiled with
	virtual void ForcedMaterialOverride(IMaterial* newMaterial, OverrideType overrideType = OverrideType::NORMAL, i32 materialIndex = -1) = 0;
	virtual bool IsForcedMaterialOverride() = 0;

	virtual void SetViewTarget(const CStudioHdr* pStudioHdr, i32 nBodyIndex, const Vec3& target) = 0;

	// Creates, destroys instance data to be associated with the model
	virtual ModelInstanceHandle CreateInstance(IClientRenderable* renderable, LightCacheHandle* cache = NULL) = 0;
	virtual void DestroyInstance(ModelInstanceHandle handle) = 0;

	// Associates a particular lighting condition with a model instance handle.
	// FIXME: This feature currently only works for static props. To make it work for entities, etc.,
	// we must clean up the lightcache handles as the model instances are removed.
	// At the moment, since only the static prop manager uses this, it cleans up all LightCacheHandles
	// at level shutdown.
	virtual void SetStaticLighting(ModelInstanceHandle mdlHandle, LightCacheHandle* lightHandle) = 0;
	virtual LightCacheHandle GetStaticLighting(ModelInstanceHandle handle) = 0;

	// moves an existing InstanceHandle to a nex Renderable to keep decals etc. Models must be the same
	virtual bool ChangeInstance(ModelInstanceHandle handle, IClientRenderable* renderable) = 0;

	// Creates a decal on a model instance by doing a planar projection
	// along the ray. The material is the decal material, the radius is the
	// radius of the decal to create.
	static constexpr inline i32 AddDecalToAllLods = -1;
	virtual void AddDecal(ModelInstanceHandle handle, Ray const& ray, Vec3 const& decalUp, i32 decalIndex, i32 body, bool noPokeThru = false, i32 maxLODToDecal = AddDecalToAllLods, IMaterial* pSpecifyMaterial = NULL, f32 w = 1.0f, f32 h = 1.0f, void* pvProxyUserData = NULL, i32 nAdditionalDecalFlags = 0) = 0;

	// Removes all the decals on a model instance
	virtual void RemoveAllDecals(ModelInstanceHandle handle) = 0;

	// Returns true if the model instance is valid and the decal list is not empty
	virtual bool ModelHasDecals(ModelInstanceHandle handle) = 0;

	// Remove all decals from all models
	virtual void RemoveAllDecalsFromAllModels(bool bRenderContextValid) = 0;

	// Shadow rendering, DrawModelShadowSetup returns the address of the bone-to-world array, NULL in case of error
	virtual Mat3x4* DrawModelShadowSetup(IClientRenderable* renderable, i32 body, i32 skin, DrawModelInfo* info, Mat3x4* customBoneToWorld = NULL) = 0;
	virtual void DrawModelShadow(IClientRenderable* renderable, const DrawModelInfo& info, Mat3x4* customBoneToWorld = NULL) = 0;

	// This gets called when overbright, etc gets changed to recompute static prop lighting.
	virtual bool RecomputeStaticLighting(ModelInstanceHandle handle) = 0;

	virtual void ReleaseAllStaticPropColorData() = 0;
	virtual void RestoreAllStaticPropColorData() = 0;

	// Extended version of drawmodel
	virtual i32 DrawModelEx(ModelRenderInfo& info) = 0;

	virtual i32 DrawModelExStaticProp(IMatRenderContext* renderContext, ModelRenderInfo& info) = 0;

	virtual bool DrawModelSetup(IMatRenderContext* renderContext, ModelRenderInfo& info, DrawModelState* state, Mat3x4** boneToWorldOut) = 0;
	virtual void DrawModelExecute(IMatRenderContext* renderContext, const DrawModelState& state, const ModelRenderInfo& info, Mat3x4* customBoneToWorld = NULL) = 0;

	// Sets up lighting context for a point in space
	virtual void SetupLighting(const Vec3& vecCenter) = 0;

	// doesn't support any debug visualization modes or other model options, but draws static props in the
	// fastest way possible
	virtual i32 DrawStaticPropArrayFast(StaticPropRenderInfo* props, i32 count, bool shadowDepth) = 0;

	// Allow client to override lighting state
	virtual void SuppressEngineLighting(bool suppress) = 0;

	virtual void SetupColorMeshes(i32 totalVerts) = 0;

	// Sets up lighting context for a point in space, with smooth interpolation per model.
	// Passing MODEL_INSTANCE_INVALID as a handle is equivalent to calling SetupLighting.
	virtual void SetupLightingEx(const Vec3& vecCenter, ModelInstanceHandle handle) = 0;

	// Finds the brightest light source illuminating a point. Returns false if there isn't any.
	virtual bool GetBrightestShadowingLightSource(const Vec3& vecCenter, Vec3& lightPos, Vec3& lightBrightness, bool bAllowNonTaggedLights) = 0;

	// Computes lighting state for an array of lighting requests
	virtual void ComputeLightingState(i32 count, const LightingQuery* query, MaterialLightingState* state, ITexture** envCubemapTexture) = 0;

	// Gets an array of decal handles given model instances
	virtual void GetModelDecalHandles(StudioDecalHandle* decals, i32 decalStride, i32 count, const ModelInstanceHandle* handles) = 0;

	// Computes lighting state for an array of lighting requests for renderables which use static lighting
	virtual void ComputeStaticLightingState(i32 count, const StaticLightingQuery* query, MaterialLightingState* state, MaterialLightingState* decalState, ColorMeshInfo** staticLighting, ITexture** envCubemapTexture, DataCacheHandle* colorMeshHandles) = 0;

	// Cleans up lighting state. Must be called after the draw call that uses
	// the color meshes return from ComputeStaticLightingState has been issued
	virtual void CleanupStaticLightingState(i32 count, DataCacheHandle* colorMeshHandles) = 0;
};

#endif // SDK_IVMODELRENDER_HPP
