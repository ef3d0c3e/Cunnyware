#ifndef SDK_CPLAYERRESOURCE_HPP
#define SDK_CPLAYERRESOURCE_HPP

#include "C_BaseEntity.hpp"

class CPlayerResource : public C_BaseEntity
{
	//DECLARE_CLASS( CPlayerResource, CBaseEntity );
public:
	//DECLARE_SERVERCLASS();
	//DECLARE_DATADESC();

	virtual void Spawn() = 0;
	virtual i32 ObjectCaps() = 0;
	virtual void ResourceThink() = 0;
	virtual void UpdatePlayerData() = 0;
	virtual i32 UpdateTransmitState() = 0;

	i32 updateCounter;
};

#endif // SDK_CPLAYERRESOURCE_HPP
