#ifndef SDK_C_BASEENTITY_HPP
#define SDK_C_BASEENTITY_HPP

#include "IClientEntity.hpp"
#include "../Offsets.hpp"
#include "../Hooker.hpp"

MAKE_CENUM_Q(MoveType, u32,
	NONE, 0,
	ISOMETRIC, 1,
	WALK, 2,
	STEP, 3,
	FLY, 4,
	FLYGRAVITY, 5,
	VPHYSICS, 6,
	PUSH, 7,
	NOCLIP, 8,
	LADDER, 9,
	OBSERVER, 10,
	CUSTOM, 11,
	LAST, CUSTOM,
	MAX_BITS, 4);

MAKE_CENUM_Q(TeamID, i32,
	UNASSIGNED, 0,
	SPECTATOR, 1,
	TERRORIST, 2,
	COUNTER_TERRORIST, 3);

class C_BaseEntity : public IClientEntity
{
public:
	IClientNetworkable* GetNetworkable()
	{
		return (IClientNetworkable*)((uintptr_t)this + 0x10);
	}

	void SetAbsOrigin(const Vec3* const angles)
	{
		asm volatile("mov %0, %%rdi;\n\t"
					 "mov %1, %%rsi;\n\t"
					 "call *%P2;"
					 :
					 : "r"(this), "r"(angles), "r"(SetAbsOriginFnAddr)
					 : "%rdi", "%rsi");
	}

	void SetModelIndex(int index)
	{
		typedef void (*oSetModelIndex)(void*, int);
		return getvfunc<oSetModelIndex>(this, 111)(this, index);
	}

	int* GetModelIndex()
	{
		return (int*)((uintptr_t)this + offsets.DT_BaseViewModel.m_nModelIndex);
	}

	float GetAnimTime()
	{
		return *(float*)((uintptr_t)this + offsets.DT_BaseEntity.m_flAnimTime);
	}

	float GetSimulationTime()
	{
		return *(float*)((uintptr_t)this + offsets.DT_BaseEntity.m_flSimulationTime);
	}

	TeamID GetTeam()
	{
		return *(TeamID*)((uintptr_t)this + offsets.DT_BaseEntity.m_iTeamNum);
	}

	int GetSurvivalTeam()
	{
		return *(int*)((uintptr_t)this + offsets.DT_CSPlayer.m_nSurvivalTeam);
	}

	Vec3 GetVecOrigin()
	{
		return *(Vec3*)((uintptr_t)this + offsets.DT_BaseEntity.m_vecOrigin);
	}

	MoveType GetMoveType()
	{
		return *(MoveType*)((uintptr_t)this + offsets.DT_BaseEntity.m_MoveType);
	}

	ICollideable* GetCollideable()
	{
		return (ICollideable*)((uintptr_t)this + offsets.DT_BaseEntity.m_Collision);
	}

	bool* GetSpotted()
	{
		return (bool*)((uintptr_t)this + offsets.DT_BaseEntity.m_bSpotted);
	}

	uint32_t* GetSpottedByMask()
	{
		return (uint32_t*)((uintptr_t)this + offsets.DT_BaseEntity.m_bSpottedByMask);
	}
};

#endif // SDK_C_BASEENTITY_HPP
