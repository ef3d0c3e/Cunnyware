#ifndef SDK_IMATERIAL_HPP
#define SDK_IMATERIAL_HPP

#include "ImageFormat.hpp"
#include "IMaterialVar.hpp"
#include "KeyValues.hpp"

MAKE_CENUM_Q(PreviewImageRetVal, i32,
	BAD, 0,
	OK, 1,
	NO_PREVIEW, 2);

typedef u64 VertexFormat;

class IMaterial
{
public:
	// Get the name of the material.  This is a full path to
	// the vmt file starting from "hl2/materials" (or equivalent) without
	// a file extension.
	virtual const char* GetName() const = 0;
	virtual const char* GetTextureGroupName() const = 0;

	// Get the preferred size/bitDepth of a preview image of a material.
	// This is the sort of image that you would use for a thumbnail view
	// of a material, or in WorldCraft until it uses materials to render.
	// separate this for the tools maybe
	virtual PreviewImageRetVal GetPreviewImageProperties(i32* width, i32* height, ImageFormat* imageFormat, bool* isTranslucent) const = 0;

	// Get a preview image at the specified width/height and bitDepth.
	// Will do resampling if necessary.(not yet!!! :) )
	// Will do color format conversion. (works now.)
	virtual PreviewImageRetVal GetPreviewImage(u8* data,
												 i32 width,
												 i32 height,
												 ImageFormat imageFormat) const = 0;
	//
	virtual i32 GetMappingWidth() = 0;
	virtual i32 GetMappingHeight() = 0;

	virtual i32 GetNumAnimationFrames() = 0;

	// For material subrects (material pages).  Offset(u,v) and scale(u,v) are normalized to texture.
	virtual bool InMaterialPage() = 0;
	virtual void GetMaterialOffset(Vec2& offset) = 0;
	virtual void GetMaterialScale(Vec2& scale) = 0;
	virtual IMaterial* GetMaterialPage() = 0;

	// find a vmt variable.
	// This is how game code affects how a material is rendered.
	// The game code must know about the params that are used by
	// the shader for the material that it is trying to affect.
	virtual IMaterialVar* FindVar(const char* varName, bool* found, bool complain = true) = 0;

	// The user never allocates or deallocates materials.  Reference counting is
	// used instead.  Garbage collection is done upon a call to
	// IMaterialSystem::UncacheUnusedMaterials.
	virtual void IncrementReferenceCount() = 0;
	virtual void DecrementReferenceCount() = 0;

	inline void AddRef()
	{
		IncrementReferenceCount();
	}
	inline void Release()
	{
		DecrementReferenceCount();
	}

	// Each material is assigned a number that groups it with like materials
	// for sorting in the application.
	virtual int GetEnumerationID() const = 0;

	virtual void GetLowResColorSample(float s, float t, float* color) const = 0;

	// This computes the state snapshots for this material
	virtual void RecomputeStateSnapshots() = 0;

	// Are we translucent?
	virtual bool IsTranslucent() = 0;

	// Are we alphatested?
	virtual bool IsAlphaTested() = 0;

	// Are we vertex lit?
	virtual bool IsVertexLit() = 0;

	// Gets the vertex format
	virtual VertexFormat GetVertexFormat() const = 0;

	// returns true if this material uses a material proxy
	virtual bool HasProxy() const = 0;

	virtual bool UsesEnvCubemap() = 0;

	virtual bool NeedsTangentSpace() = 0;

	virtual bool NeedsPowerOfTwoFrameBufferTexture(bool checkSpecificToThisFrame = true) = 0;
	virtual bool NeedsFullFrameBufferTexture(bool checkSpecificToThisFrame = true) = 0;

	// returns true if the shader doesn't do skinning itself and requires
	// the data that is sent to it to be preskinned.
	virtual bool NeedsSoftwareSkinning() = 0;

	// Apply constant color or alpha modulation
	virtual void AlphaModulate(float alpha) = 0;
	virtual void ColorModulate(float r, float g, float b) = 0;

	// Material Var flags...
	virtual void SetMaterialVarFlag(MaterialVarFlags flag, bool on) = 0;
	virtual bool GetMaterialVarFlag(MaterialVarFlags flag) const = 0;

	// Gets material reflectivity
	virtual void GetReflectivity(Vec3& reflect) = 0;

	// Gets material property flags
	virtual bool GetPropertyFlag(MaterialPropertyTypes type) = 0;

	// Is the material visible from both sides?
	virtual bool IsTwoSided() = 0;

	// Sets the shader associated with the material
	virtual void SetShader(const char* shaderName) = 0;

	// Can't be const because the material might have to precache itself.
	virtual int GetNumPasses() = 0;

	// Can't be const because the material might have to precache itself.
	virtual i32 GetTextureMemoryBytes() = 0;

	// Meant to be used with materials created using CreateMaterial
	// It updates the materials to reflect the current values stored in the material vars
	virtual void Refresh() = 0;

	// GR - returns true is material uses lightmap alpha for blending
	virtual bool NeedsLightmapBlendAlpha() = 0;

	// returns true if the shader doesn't do lighting itself and requires
	// the data that is sent to it to be prelighted
	virtual bool NeedsSoftwareLighting() = 0;

	// Gets at the shader parameters
	virtual i32 ShaderParamCount() const = 0;
	virtual IMaterialVar** GetShaderParams() = 0;

	// Returns true if this is the error material you get back from IMaterialSystem::FindMaterial if
	// the material can't be found.
	virtual bool IsErrorMaterial() const = 0;

	virtual void Unused() = 0;

	// Gets the current alpha modulation
	virtual f32 GetAlphaModulation() = 0;
	virtual void GetColorModulation(f32* r, f32* g, f32* b) = 0;

	// Is this translucent given a particular alpha modulation?
	virtual bool IsTranslucentUnderModulation(f32 fAlphaModulation = 1.0f) const = 0;

	// fast find that stores the index of the found var in the string table in local cache
	virtual IMaterialVar* FindVarFast(char const* pVarName, unsigned int* pToken) = 0;

	// Sets new VMT shader parameters for the material
	virtual void SetShaderAndParams(KeyValues* pKeyValues) = 0;
	virtual const char* GetShaderName() const = 0;

	virtual void DeleteIfUnreferenced() = 0;

	virtual bool IsSpriteCard() = 0;

	virtual void CallBindProxy(void* proxyData) = 0;

	virtual void RefreshPreservingMaterialVars() = 0;

	virtual bool WasReloadedFromWhitelist() = 0;

	virtual bool SetTempExcluded(bool bSet, int nExcludedDimensionLimit) = 0;

	virtual int GetReferenceCount() const = 0;
};

#endif // SDK_IMATERIAL_HPP
