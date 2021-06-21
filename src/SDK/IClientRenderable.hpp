#ifndef SDK_ICLIENTRENDERABLE_HPP
#define SDK_ICLIENTRENDERABLE_HPP

#include "Defs.hpp"
class IClientUnknown;
struct model_t;
class ClientRenderHandle;
class IPVSNotify;
class ClientModelRenderable;

typedef u16 ClientShadowHandle;
typedef u16 ModelInstanceHandle;

MAKE_CENUM_Q(ShadowType, u32,
	NONE, 0,
	SIMPLE, 1,
	RENDER_TO_TEXTURE, 2,
	RENDER_TO_TEXTURE_DYNAMIC, 3, // the shadow is always changing state
	RENDER_TO_DEPTH_TEXTURE, 4,
	RENDER_TO_TEXTURE_DYNAMIC_CUSTOM, 5, // changing, and entity uses custom rendering code for shadow
);

struct RenderableInstance
{
	u8 alpha;
};


class IClientRenderable
{
public:
	// Gets at the containing class...
	virtual IClientUnknown* GetIClientUnknown() = 0;

	// Data accessors
	virtual Vec3 const& GetRenderOrigin() = 0;
	virtual QAngle const& GetRenderAngles() = 0;
	virtual bool ShouldDraw() = 0;
	virtual i32 GetRenderFlags() = 0; // ERENDERFLAGS_xxx
	virtual void Unused() const = 0;

	virtual ClientShadowHandle GetShadowHandle() const = 0;

	// Used by the leaf system to store its render handle.
	virtual ClientRenderHandle& RenderHandle() = 0;

	// Render baby!
	virtual const model_t* GetModel() const = 0;
	virtual i32 DrawModel(i32 flags, const RenderableInstance& instance) = 0;

	// Get the body parameter
	virtual i32 GetBody() = 0;

	// Determine the color modulation amount
	virtual void GetColorModulation(f32* color) = 0;

	// Returns false if the entity shouldn't be drawn due to LOD.
	// (NOTE: This is no longer used/supported, but kept in the vtable for backwards compat)
	virtual bool LODTest() = 0;

	// Call this to get the current bone transforms for the model.
	// currentTime parameter will affect interpolation
	// nMaxBones specifies how many matrices pBoneToWorldOut can hold. (Should be greater than or
	// equal to studiohdr_t::numbones. Use MAXSTUDIOBONES to be safe.)
	virtual bool SetupBones(Mat3x4* boneToWorldOut, i32 maxBones, i32 boneMask, f32 currentTime) = 0;

	virtual void SetupWeights(const Mat3x4* boneToWorld, i32 flexWeightCount, f32* flexWeights, f32* flexDelayedWeights) = 0;
	virtual void DoAnimationEvents() = 0;

	// Return this if you want PVS notifications. See IPVSNotify for more info.
	// Note: you must always return the same value from this function. If you don't,
	// undefined things will occur, and they won't be good.
	virtual IPVSNotify* GetPVSNotifyInterface() = 0;

	// Returns the bounds relative to the origin (render bounds)
	virtual void GetRenderBounds(Vec3& mins, Vec3& maxs) = 0;

	// returns the bounds as an AABB in worldspace
	virtual void GetRenderBoundsWorldspace(Vec3& mins, Vec3& maxs) = 0;

	// These normally call through to GetRenderAngles/GetRenderBounds, but some entities custom implement them.
	virtual void GetShadowRenderBounds(Vec3& mins, Vec3& maxs, ShadowType shadowType) = 0;

	// Should this object be able to have shadows cast onto it?
	virtual bool ShouldReceiveProjectedTextures(int flags) = 0;

	// These methods return true if we want a per-renderable shadow cast direction + distance
	virtual bool GetShadowCastDistance(f32* dist, ShadowType shadowType) const = 0;
	virtual bool GetShadowCastDirection(Vec3* direction, ShadowType shadowType) const = 0;

	// Other methods related to shadow rendering
	virtual bool IsShadowDirty() = 0;
	virtual void MarkShadowDirty(bool dirty) = 0;

	// Iteration over shadow hierarchy
	virtual IClientRenderable* GetShadowParent() = 0;
	virtual IClientRenderable* FirstShadowChild() = 0;
	virtual IClientRenderable* NextShadowPeer() = 0;

	// Returns the shadow cast type
	virtual ShadowType ShadowCastType() = 0;

	virtual void Unused2() = 0;

	// Create/get/destroy model instance
	virtual void CreateModelInstance() = 0;
	virtual ModelInstanceHandle GetModelInstance() = 0;

	// Returns the transform from RenderOrigin/RenderAngles to world
	virtual const Mat3x4& RenderableToWorldTransform() = 0;

	// Attachments
	virtual i32 LookupAttachment(const char* attachmentName) = 0;
	virtual bool GetAttachment(i32 number, Vec3& origin, QAngle& angles) = 0;
	virtual bool GetAttachment(i32 number, Mat3x4& matrix) = 0;
	virtual bool ComputeLightingOrigin(i32 attachmentIndex, Vec3 modelLightingCenter, const Mat3x4& matrix, Vec3& transformedLightingCenter) = 0;

	// Rendering clip plane, should be 4 f32s, return value of NULL indicates a disabled render clip plane
	virtual f32* GetRenderClipPlane() = 0;

	// Get the skin parameter
	virtual i32 GetSkin() = 0;

	virtual void OnThreadedDrawSetup() = 0;

	virtual bool UsesFlexDelayedWeights() = 0;

	virtual void RecordToolMessage() = 0;
	virtual bool ShouldDrawForSplitScreenUser(i32 slot) = 0;

	// NOTE: This is used by renderables to override the default alpha modulation,
	// not including fades, for a renderable. The alpha passed to the function
	// is the alpha computed based on the current renderfx.
	virtual u8 OverrideAlphaModulation(u8 alpha) = 0;

	// NOTE: This is used by renderables to override the default alpha modulation,
	// not including fades, for a renderable's shadow. The alpha passed to the function
	// is the alpha computed based on the current renderfx + any override
	// computed in OverrideAlphaModulation
	virtual u8 OverrideShadowAlphaModulation(u8 alpha) = 0;

	virtual IClientModelRenderable* GetClientModelRenderable() = 0;
};

#endif // SDK_ICLIENTRENDERABLE_HPP
