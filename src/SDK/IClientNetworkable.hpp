#ifndef SDK_ICLIENTNETWORKABLE_HPP
#define SDK_ICLIENTNETWORKABLE_HPP

#include "Defs.hpp"

class IClientUnknown;
class ClientClass;
class bf_read;

MAKE_CENUM_Q(ShouldTransmitState, u32,
	START, 1, // The entity is starting to be transmitted (maybe it entered the PVS).
	END, 1,   // Called when the entity isn't being transmitted by the server.
			  // This signals a good time to hide the entity until next time
			  // the server wants to transmit its state.
);

MAKE_CENUM_Q(DataUpdateType, u32,
	DATA_UPDATE_CREATED, 0,	// indicates it was created +and+ entered the pvs
//	DATA_UPDATE_ENTERED_PVS,
	DATA_UPDATE_DATATABLE_CHANGED, 1,
//	DATA_UPDATE_LEFT_PVS,
//	DATA_UPDATE_DESTROYED,		// FIXME: Could enable this, but it's a little worrying
								// since it changes a bunch of existing code
);

class IClientNetworkable : public non_constructible
{
public:
	// Gets at the containing class...
	virtual IClientUnknown* GetIClientUnknown() = 0;

	// Called by the engine when the server deletes the entity.
	virtual void Release() = 0;

	// Supplied automatically by the IMPLEMENT_CLIENTCLASS macros.
	virtual ClientClass* GetClientClass() = 0;

	// This tells the entity what the server says for ShouldTransmit on this entity.
	// Note: This used to be EntityEnteredPVS/EntityRemainedInPVS/EntityLeftPVS.
	virtual void NotifyShouldTransmit(ShouldTransmitState state) = 0;



	//
	// NOTE FOR ENTITY WRITERS:
	//
	// In 90% of the cases, you should hook OnPreDataChanged/OnDataChanged instead of
	// PreDataUpdate/PostDataUpdate.
	//
	// The DataChanged events are only called once per frame whereas Pre/PostDataUpdate
	// are called once per packet (and sometimes multiple times per frame).
	//
	// OnDataChanged is called during simulation where entity origins are correct and
	// attachments can be used. whereas PostDataUpdate is called while parsing packets
	// so attachments and other entity origins may not be valid yet.
	//

	virtual void OnPreDataChanged(DataUpdateType updateType) = 0;
	virtual void OnDataChanged(DataUpdateType updateType) = 0;

	// Called when data is being updated across the network.
	// Only low-level entities should need to know about these.
	virtual void PreDataUpdate(DataUpdateType updateType) = 0;
	virtual void PostDataUpdate(DataUpdateType updateType) = 0;
	virtual void OnDataUnchangedInPVS() = 0;


	// Objects become dormant on the client if they leave the PVS on the server.
	virtual bool IsDormant() = 0;

	// Ent Index is the server handle used to reference this entity.
	// If the index is < 0, that indicates the entity is not known to the server
	virtual i32 GetIndex() const = 0;

	// Server to client entity message received
	virtual void ReceiveMessage(int classID, bf_read& msg) = 0;

	// Get the base pointer to the networked data that GetClientClass->m_pRecvTable starts at.
	// (This is usually just the "this" pointer).
	virtual void* GetDataTableBasePtr() = 0;

	// Tells the entity that it's about to be destroyed due to the client receiving
	// an uncompressed update that's caused it to destroy all entities & recreate them.
	virtual void SetDestroyedOnRecreateEntities() = 0;
};

#endif // SDK_ICLIENTNETWORKABLE_HPP
