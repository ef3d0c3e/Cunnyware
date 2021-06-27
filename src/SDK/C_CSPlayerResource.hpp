#ifndef SDK_C_CSPLAYERRESOURCE_HPP
#define SDK_C_CSPLAYERRESOURCE_HPP

#include "CPlayerResource.hpp"

class C_CSPlayerResource : public CPlayerResource
{
public:
	i32 GetPing(i32 index)
	{
		return *(i32*)((std::uintptr_t)this + offsets.DT_PlayerResource.m_iPing + index * 4);
	}

	i32 GetKills(i32 index)
	{
		return *(i32*)((std::uintptr_t)this + offsets.DT_PlayerResource.m_iKills + index * 4);
	}

	i32 GetAssists(i32 index)
	{
		return *(i32*)((std::uintptr_t)this + offsets.DT_PlayerResource.m_iAssists + index * 4);
	}

	i32 GetDeaths(i32 index)
	{
		return *(i32*)((std::uintptr_t)this + offsets.DT_PlayerResource.m_iDeaths + index * 4);
	}

	bool GetConnected(i32 index)
	{
		return *(bool*)((std::uintptr_t)this + offsets.DT_PlayerResource.m_bConnected + index);
	}

	TeamID GetTeam(i32 index)
	{
		return *(TeamID*)((std::uintptr_t)this + offsets.DT_PlayerResource.m_iTeam + index * 4);
	}

	i32 GetPendingTeam(i32 index)
	{
		return *(i32*)((std::uintptr_t)this + offsets.DT_PlayerResource.m_iPendingTeam + index * 4);
	}

	bool GetAlive(i32 index)
	{
		return *(bool*)((std::uintptr_t)this + offsets.DT_PlayerResource.m_bAlive + index);
	}

	i32 GetHealth(i32 index)
	{
		return *(i32*)((std::uintptr_t)this + offsets.DT_PlayerResource.m_iHealth + index * 4);
	}

	i32 GetPlayerC4()
	{
		return *(i32*)((std::uintptr_t)this + offsets.DT_CSPlayerResource.m_iPlayerC4);
	}

	i32 GetMVPs(i32 index)
	{
		return *(i32*)((std::uintptr_t)this + offsets.DT_CSPlayerResource.m_iMVPs + index * 4);
	}

	i32 GetArmor(i32 index)
	{
		return *(i32*)((std::uintptr_t)this + offsets.DT_CSPlayerResource.m_iArmor + index * 4);
	}

	i32 GetScore(i32 index)
	{
		return *(i32*)((std::uintptr_t)this + offsets.DT_CSPlayerResource.m_iScore + index * 4);
	}

	i32* GetCompetitiveRanking(i32 index)
	{
		return (i32*)((std::uintptr_t)this + offsets.DT_CSPlayerResource.m_iCompetitiveRanking + index * 4);
	}

	i32* GetCompetitiveWins(i32 index)
	{
		return (i32*)((std::uintptr_t)this + offsets.DT_CSPlayerResource.m_iCompetitiveWins + index * 4);
	}

	i32 GetCompTeammateColor(i32 index)
	{
		return *(i32*)((std::uintptr_t)this + offsets.DT_CSPlayerResource.m_iCompTeammateColor + index * 4);
	}

	const char* GetClan(i32 index)
	{
		return (const char*)((std::uintptr_t)this + offsets.DT_CSPlayerResource.m_szClan + index * 16);
	}

	i32* GetActiveCoinRank(i32 index)
	{
		return (i32*)((std::uintptr_t)this + offsets.DT_CSPlayerResource.m_nActiveCoinRank + index * 4);
	}

	i32* GetMusicID(i32 index)
	{
		return (i32*)((std::uintptr_t)this + offsets.DT_CSPlayerResource.m_nMusicID + index * 4);
	}

	i32* GetPersonaDataPublicCommendsLeader(i32 index)
	{
		return (i32*)((std::uintptr_t)this + offsets.DT_CSPlayerResource.m_nPersonaDataPublicCommendsLeader + index * 4);
	}

	i32* GetPersonaDataPublicCommendsTeacher(i32 index)
	{
		return (i32*)((std::uintptr_t)this + offsets.DT_CSPlayerResource.m_nPersonaDataPublicCommendsTeacher + index * 4);
	}

	i32* GetPersonaDataPublicCommendsFriendly(i32 index)
	{
		return (i32*)((std::uintptr_t)this + offsets.DT_CSPlayerResource.m_nPersonaDataPublicCommendsFriendly + index * 4);
	}
};


#endif // SDK_C_CSPLAYERRESOURCE_HPP
