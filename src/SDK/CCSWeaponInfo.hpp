#ifndef SDK_CCSWEAPONINFO_HPP
#define SDK_CCSWEAPONINFO_HPP

#include "FileWeaponInfo.hpp"

MAKE_CENUM_Q(CSWeaponType, i32,
	KNIFE, 0,
	PISTOL, 1,
	SUBMACHINEGUN, 2,
	RIFLE, 3,
	SHOTGUN, 4,
	SNIPERRIFLE, 5,
	MACHINEGUN, 6,
	C4, 7,
	PLACEHOLDER, 8,
	GRENADE, 9,
	UNKNOWN, 10,
	STACKABLEITEM, 11,
	FISTS, 12,
	BREACHCHARGE, 13,
	BUMPMINE, 14,
	TABLET, 15,
	MELEE, 16);

class CCSWeaponInfo : public FileWeaponInfo
{
public:
	char* GetConsoleName()
	{
		return *(char**) ( ( std::uintptr_t )this + 0x8);
	}

	i32 GetClipSize()
	{
		return *( i32* ) ( ( std::uintptr_t )this + 0x20);
	}

	CSWeaponType GetWeaponType()
	{
		return *( CSWeaponType* ) ( ( std::uintptr_t )this + 0x140);
	}

	void SetWeaponType( CSWeaponType type )
	{
		*( CSWeaponType* ) ( ( std::uintptr_t )this + 0x140) = type;
	}

	i32 GetDamage()
	{
		return *( i32* ) ( ( std::uintptr_t )this + 0x16C);
	}

	f32 GetWeaponArmorRatio()
	{
		return *( f32* ) ( ( std::uintptr_t )this + 0x170);
	}

	f32 GetPenetration()
	{
		return *( f32* ) ( ( std::uintptr_t )this + 0x178);
	}

	f32 GetRange()
	{
		return *( f32* ) ( ( std::uintptr_t )this + 0x184);
	}

	f32 GetRangeModifier()
	{
		return *( f32* ) ( ( std::uintptr_t )this + 0x188);
	}

	f32 GetMaxPlayerSpeed()
	{
		return *( f32* ) ( ( std::uintptr_t )this + 0x1B0);
	}

	i32 GetZoomLevels()
	{ // Doesn't work correctly on some weapons.
		return *( i32* ) ( ( std::uintptr_t )this + 0x23C); // DT_WeaponCSBaseGun.m_zoomLevel ?
	}

	char* GetTracerEffect()
	{
		return *( char** ) ( ( std::uintptr_t )this + 0x280);
	}

	int* GetTracerFrequency()
	{
		return ( int* ) ( ( std::uintptr_t )this + 0x288);
	}
};

#endif // SDK_CCSWEAPONINFO_HPP
