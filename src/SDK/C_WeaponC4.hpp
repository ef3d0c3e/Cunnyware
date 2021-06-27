#ifndef SDK_C_WEAPONC4_HPP
#define SDK_C_WEAPONC4_HPP

#include "C_BaseCombatWeapon.hpp"

class C_WeaponC4 : public C_BaseCombatWeapon
{
public:
	bool GetStartedArming()
	{
		return *(bool*)((uintptr_t)this + offsets.DT_WeaponC4.m_bStartedArming);
	}
};

#endif // SDK_C_WEAPONC4_HPP
