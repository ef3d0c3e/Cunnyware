#ifndef SDK_C_BASECOMBATWEAPON_HPP
#define SDK_C_BASECOMBATWEAPON_HPP

#include "C_BaseAttributableItem.hpp"
#include "CCSWeaponInfo.hpp"

class C_BaseCombatWeapon : public C_BaseAttributableItem
{
public:

	i32 GetOwner()
	{
		return *(i32*)((std::uintptr_t)this + offsets.DT_BaseCombatWeapon.m_hOwner);
	}

	i32 GetAmmo()
	{
		return *(i32*)((std::uintptr_t)this + offsets.DT_BaseCombatWeapon.m_iClip1);
	}

	f32 GetNextPrimaryAttack()
	{
		return *(f32*)((std::uintptr_t)this + offsets.DT_BaseCombatWeapon.m_flNextPrimaryAttack);
	}

	bool GetInReload()
	{
		return *(bool*)((std::uintptr_t)this + offsets.DT_BaseCombatWeapon.m_bInReload);
	}

	f32 GetAccuracyPenalty()
	{
		return *(f32*)((std::uintptr_t)this + offsets.DT_WeaponCSBase.m_fAccuracyPenalty);
	}

	f32 GetPostPoneReadyTime()
	{
		return *(f32*)((std::uintptr_t) this + offsets.DT_WeaponCSBase.m_flPostponeFireReadyTime);
	}

	bool GetReloadVisuallyComplete()
	{
		return *(bool*)((std::uintptr_t)this + offsets.DT_WeaponCSBase.m_bReloadVisuallyComplete);
	}

	void DrawCrosshair()
	{ // returns a 1
		typedef void (* oDrawCrosshair)( void* );
		return getvfunc<oDrawCrosshair>( this, 471 )( this );
	}

	CCSWeaponInfo* GetCSWpnData()
	{ // "script file not found" (client_client)
		typedef CCSWeaponInfo* (* oGetCSWpnData)( void* );
		return getvfunc<oGetCSWpnData>( this, 528 )( this );
	}

	f32 GetSpread()
	{
		typedef f32 (* oGetSpread)( void* );
		return getvfunc<oGetSpread>( this, 520 )( this );
	}

	f32 GetInaccuracy()
	{
		typedef f32 (* oGetInaccuracy)( void* );
		return getvfunc<oGetInaccuracy>( this, 550 )( this );
	}

	void UpdateAccuracyPenalty()
	{
		typedef void (* oUpdateAccuracyPenalty)( void* );
		return getvfunc<oUpdateAccuracyPenalty>( this, 551 )( this );
	}
};

#endif // SDK_C_BASECOMBATWEAPON_HPP
