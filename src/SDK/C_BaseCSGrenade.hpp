#ifndef SDK_C_BASECSGRENADE_HPP
#define SDK_C_BASECSGRENADE_HPP

class C_BaseCSGrenade : public C_BaseCombatWeapon
{
public:
	bool IsHeldByPlayer()
	{
		return *(bool*)((std::uintptr_t)this + offsets.DT_BaseCSGrenade.m_bIsHeldByPlayer);
	}

	bool GetPinPulled()
	{
		return *(bool*)((std::uintptr_t)this + offsets.DT_BaseCSGrenade.m_bPinPulled);
	}

	f32 GetThrowTime()
	{
		return *(f32*)((std::uintptr_t)this + offsets.DT_BaseCSGrenade.m_fThrowTime);
	}

	f32 GetThrowStrength()
	{
		return *(f32*)((std::uintptr_t)this + offsets.DT_BaseCSGrenade.m_flThrowStrength);
	}
};

#endif // SDK_C_BASECSGRENADE_HPP
