#ifndef SDK_ICLIENTENTITYLIST_HPP
#define SDK_ICLIENTENTITYLIST_HPP

#include "Defs.hpp"


class IClientEntity;
class ClientClass;
class IClientNetworkable;
class CBaseHandle;
class IClientUnknown;
class C_BaseEntity;

struct EntityCacheInfo_t
{
	// Cached off because GetClientNetworkable is called a *lot*
	IClientNetworkable* networkable;
	u16 baseEntitiesIndex; // Index into m_BaseEntities (or m_BaseEntities.InvalidIndex() if none).
	u16 dormant; // cached dormant state - this is only a bit
};

class IClientEntityList
{
public:
	// Get IClientNetworkable interface for specified entity
	virtual IClientNetworkable* GetClientNetworkable(i32 entnum) = 0;
	virtual IClientNetworkable* GetClientNetworkableFromHandle(CBaseHandle hEnt) = 0;
	virtual IClientUnknown* GetClientUnknownFromHandle(CBaseHandle hEnt) = 0;

	// NOTE: This function is only a convenience wrapper.
	// It returns GetClientNetworkable( entnum )->GetIClientEntity().
	virtual C_BaseEntity* GetClientEntity(i32 entnum) = 0;
	virtual IClientEntity* GetClientEntityFromHandle(/*CBaseHandle*/ void* hEnt) = 0;

	// Returns number of entities currently in use
	virtual int NumberOfEntities(bool includeNonNetworkable) = 0;

	// Returns highest index actually used
	virtual int GetHighestEntityIndex() = 0;

	// Sizes entity list to specified size
	virtual void SetMaxEntities(i32 maxents) = 0;
	virtual int GetMaxEntities() = 0;
	virtual EntityCacheInfo_t* GetClientNetworkableArray() = 0;
};

#endif // SDK_ICLIENTENTITYLIST_HPP
