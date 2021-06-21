#ifndef SDK_ICOLLIDEABLE_HPP
#define SDK_ICOLLIDEABLE_HPP

#include "Defs.hpp"

class IHandleEntity;
struct Ray;
struct trace_t;
struct model_t;
class IClientUnknown;

MAKE_CENUM_Q(SolidType, u32,
	NONE, 0,     // no solid model
	BSP, 1,      // a BSP tree
	BBOX, 2,     // an AABB
	OBB, 3,      // an OBB (not implemented yet)
	OBB_YAW, 4,  // an OBB, constrained so that it can only yaw
	CUSTOM, 5,   // Always call into the entity for tests
	VPHYSICS, 6, // solid vphysics object, get vcollide from the model and collide with that
);

class ICollideable
{
public:
	// Gets at the entity handle associated with the collideable
	virtual IHandleEntity* GetEntityHandle() = 0;

	// These methods return the bounds of an OBB measured in "collision" space
	// which can be retreived through the CollisionToWorldTransform or
	// GetCollisionOrigin/GetCollisionAngles methods
	virtual const Vec3& OBBMins() const = 0;
	virtual const Vec3& OBBMaxs() const = 0;

	// Returns the bounds of a world-space box used when the collideable is being traced
	// against as a trigger. It's only valid to call these methods if the solid flags
	// have the FSOLID_USE_TRIGGER_BOUNDS flag set.
	virtual void WorldSpaceTriggerBounds(Vec3* pVecWorldMins, Vec3* pVecWorldMaxs) const = 0;

	// custom collision test
	virtual bool TestCollision(const Ray& ray, u32 fContentsMask, trace_t& tr) = 0;

	// Perform hitbox test, returns true *if hitboxes were tested at all*!!
	virtual bool TestHitboxes(const Ray& ray, u32 fContentsMask, trace_t& tr) = 0;

	// Returns the BRUSH model index if this is a brush model. Otherwise, returns -1.
	virtual i32 GetCollisionModelIndex() = 0;

	// Return the model, if it's a studio model.
	virtual const model_t* GetCollisionModel() = 0;

	// Get angles and origin.
	virtual const Vec3& GetCollisionOrigin() const = 0;
	virtual const QAngle& GetCollisionAngles() const = 0;
	virtual const Mat3x4& CollisionToWorldTransform() const = 0;

	// Return a SOLID_ define.
	virtual SolidType GetSolid() const = 0;
	virtual i32 GetSolidFlags() const = 0;

	// Gets at the containing class...
	virtual IClientUnknown* GetIClientUnknown() = 0;

	// We can filter out collisions based on collision group
	virtual i32 GetCollisionGroup() const = 0;

	// Returns a world-aligned box guaranteed to surround *everything* in the collision representation
	// Note that this will surround hitboxes, trigger bounds, physics.
	// It may or may not be a tight-fitting box and its volume may suddenly change
	virtual void WorldSpaceSurroundingBounds(Vec3* pVecMins, Vec3* pVecMaxs) = 0;

	virtual bool ShouldTouchTrigger(i32 triggerSolidFlags) const = 0;

	// returns NULL unless this collideable has specified FSOLID_ROOT_PARENT_ALIGNED
	virtual const Mat3x4* GetRootParentToWorldTransform() const = 0;
};

#endif // SDK_ICOLLIDEABLE_HPP
