#ifndef SDK_C_BASEATTRIBUTABLEITEM_HPP
#define SDK_C_BASEATTRIBUTABLEITEM_HPP

#include "C_BaseEntity.hpp"

class C_BaseAttributableItem : public C_BaseEntity
{
public:
	ItemDefinitionIndex* GetItemDefinitionIndex()
	{
		return (ItemDefinitionIndex*)((std::uintptr_t)this + offsets.DT_BaseAttributableItem.m_iItemDefinitionIndex);
	}

	bool* GetInitialized()
	{
		return (bool*)((std::uintptr_t)this + offsets.DT_BaseAttributableItem.m_bInitialized);
	}

	i32* GetItemIDHigh()
	{
		return (i32*)((std::uintptr_t)this + offsets.DT_BaseAttributableItem.m_iItemIDHigh);
	}

	i32* GetEntityQuality()
	{
		return (i32*)((std::uintptr_t)this + offsets.DT_BaseAttributableItem.m_iEntityQuality);
	}

	char* GetCustomName()
	{
		return (char*)((std::uintptr_t)this + offsets.DT_BaseAttributableItem.m_szCustomName);
	}

	i32* GetFallbackPaintKit()
	{
		return (i32*)((std::uintptr_t)this + offsets.DT_BaseAttributableItem.m_nFallbackPaintKit);
	}

	i32* GetFallbackSeed()
	{
		return (i32*)((std::uintptr_t)this + offsets.DT_BaseAttributableItem.m_nFallbackSeed);
	}

	f32* GetFallbackWear()
	{
		return (f32*)((std::uintptr_t)this + offsets.DT_BaseAttributableItem.m_flFallbackWear);
	}

	i32* GetFallbackStatTrak()
	{
		return (i32*)((std::uintptr_t)this + offsets.DT_BaseAttributableItem.m_nFallbackStatTrak);
	}

	i32* GetAccountID()
	{
		return (i32*)((std::uintptr_t)this + offsets.DT_BaseAttributableItem.m_iAccountID);
	}
};

#endif // SDK_C_BASEATTRIBUTABLEITEM_HPP
