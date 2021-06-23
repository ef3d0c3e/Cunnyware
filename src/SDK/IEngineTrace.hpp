#ifndef SDK_IENGINETRACE_HPP
#define SDK_IENGINETRACE_HPP

#include "Ray.hpp"
#include "CTraceFilter.hpp"
#include "IEntityEnumerator.hpp"

class IHandleEntity;
class ITraceListData;
class virtualmeshlist_t;
class BrushSideInfo;

MAKE_CENUM_Q(DebugTraceCounterBehavior, i32,
	SET, 0,
	INC, 1);

MAKE_CENUM_Q(HitGroups, i32,
	GENERIC, 0,
	HEAD, 1,
	CHEST, 2,
	STOMACH, 3,
	LEFTARM, 4,
	RIGHTARM, 5,
	LEFTLEG, 6,
	RIGHTLEG, 7,
    NECK, 8,
	GEAR, 10);

struct cplane_t
{
	Vec3 normal;
	f32 dist;
	u8 type;
	u8 signbits;
	u8 pad[2];
};

struct csurface_t
{
	const char* name;
	i16 surfaceProps;
	u16 flags;
};

struct trace_t
{
	Vec3 startpos;
	Vec3 endpos;
	cplane_t plane;

	f32 fraction;

	i32 contents;
	u64 dispFlags;

	bool allsolid;
	bool startsolid;

	f32 fractionleftsolid;

	csurface_t surface;

	HitGroups hitgroup;
	i16 physicsbone;

	u16 worldSurfaceIndex;
	C_BaseEntity* entityHit;
	i32 hitbox;
};

class IEngineTrace
{
public:
	// Returns the contents mask + entity at a particular world-space position
	virtual i32 GetPointContents(const Vec3& vecAbsPosition, i32 contentsMask = MASK_ALL, IHandleEntity** entity = nullptr) = 0;

	// Returns the contents mask of the world only @ the world-space position (static props are ignored)
	virtual i32 GetPointContents_WorldOnly(const Vec3& vecAbsPosition, i32 contentsMask = MASK_ALL) = 0;

	// Get the point contents, but only test the specific entity. This works
	// on static props and brush models.
	//
	// If the entity isn't a static prop or a brush model, it returns CONTENTS_EMPTY and sets
	// bFailed to true if bFailed is non-null.
	virtual i32 GetPointContents_Collideable(ICollideable* pCollide, const Vec3& vecAbsPosition) = 0;

	// Traces a ray against a particular entity
	virtual void ClipRayToEntity(const Ray& ray, u32 fMask, IHandleEntity* pEnt, trace_t* pTrace) = 0;

	// Traces a ray against a particular entity
	virtual void ClipRayToCollideable(const Ray& ray, u32 fMask, ICollideable* pCollide, trace_t* pTrace) = 0;

	// A version that simply accepts a ray (can work as a traceline or tracehull)
	virtual void TraceRay(const Ray& ray, u32 fMask, ITraceFilter* pTraceFilter, trace_t* pTrace) = 0;

	// A version that sets up the leaf and entity lists and allows you to pass those in for collision.
	virtual void SetupLeafAndEntityListRay(const Ray& ray, ITraceListData* traceData) = 0;
	virtual void SetupLeafAndEntityListBox(const Vec3& vecBoxMin, const Vec3& vecBoxMax, ITraceListData* traceData) = 0;
	virtual void TraceRayAgainstLeafAndEntityList(const Ray& ray, ITraceListData* traceData, u32 fMask, ITraceFilter* traceFilter, trace_t* trace) = 0;

	// A version that sweeps a collideable through the world
	// abs start + abs end represents the collision origins you want to sweep the collideable through
	// vecAngles represents the collision angles of the collideable during the sweep
	virtual void SweepCollideable(ICollideable* pCollide, const Vec3& vecAbsStart, const Vec3& vecAbsEnd, const QAngle& vecAngles, u32 fMask, ITraceFilter* traceFilter, trace_t* pTrace) = 0;

	// Enumerates over all entities along a ray
	// If triggers == true, it enumerates all triggers along a ray
	virtual void EnumerateEntities(const Ray& ray, bool triggers, IEntityEnumerator* enumerator) = 0;

	// Same thing, but enumerate entitys within a box
	virtual void EnumerateEntities(const Vec3& vecAbsMins, const Vec3& vecAbsMaxs, IEntityEnumerator* enumerator) = 0;

	// Convert a handle entity to a collideable.  Useful inside enumer
	virtual ICollideable* GetCollideable(IHandleEntity* pEntity) = 0;

	// HACKHACK: Temp for performance measurments
	virtual i32 GetStatByIndex(i32 index, bool bClear) = 0;


	//finds brushes in an AABB, prone to some false positives
	virtual void GetBrushesInAABB(const Vec3& vMins, const Vec3& vMaxs, CUtlVector<i32>* pOutput, i32 iContentsMask = 0xFFFFFFFF) = 0;

	//Creates a CPhysCollide out of all displacements wholly or partially contained in the specified AABB
	virtual CPhysCollide* GetCollidableFromDisplacementsInAABB(const Vec3& vMins, const Vec3& vMaxs) = 0;

	// gets the number of displacements in the world
	virtual i32 GetNumDisplacements() = 0;

	// gets a specific diplacement mesh
	virtual void GetDisplacementMesh(i32 nIndex, virtualmeshlist_t* pMeshTriList) = 0;

	//retrieve brush planes and contents, returns true if data is being returned in the output pointers, false if the brush doesn't exist
	virtual bool GetBrushInfo(i32 iBrush, CUtlVector<BrushSideInfo>* pBrushSideInfoOut, i32* pContentsOut) = 0;

	virtual bool PointOutsideWorld(const Vec3& ptTest) = 0; //Tests a point to see if it's outside any playable area

	// Walks bsp to find the leaf containing the specified point
	virtual i32 GetLeafContainingPoint(const Vec3& ptTest) = 0;

	virtual ITraceListData* AllocTraceListData() = 0;
	virtual void FreeTraceListData(ITraceListData* ) = 0;

	/// Used only in debugging: get/set/clear/increment the trace debug counter. See comment below for details.
	virtual i32 GetSetDebugTraceCounter(i32 value, DebugTraceCounterBehavior behavior) = 0;
};

#endif // SDK_IENGINETRACE_HPP
