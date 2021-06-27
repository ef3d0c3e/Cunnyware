#ifndef SDK_FILEWEAPONINFO_HPP
#define SDK_FILEWEAPONINFO_HPP

#include "Defs.hpp"

class CHudTexture;

MAKE_CENUM_Q(WeaponSound, i32,
	EMPTY, 0,
	SINGLE, 1,
	SINGLE_NPC, 2,
	WPN_DOUBLE, 3, // Can't be "DOUBLE" because windows.h uses it.
	DOUBLE_NPC, 4,
	BURST, 5,
	RELOAD, 6,
	RELOAD_NPC, 7,
	MELEE_MISS, 8,
	MELEE_HIT, 9,
	MELEE_HIT_WORLD, 10,
	SPECIAL1, 11,
	SPECIAL2, 12,
	SPECIAL3, 13,
	TAUNT, 14,
	FAST_RELOAD, 15,

	// Add new shoot sound types here
	REVERSE_THE_NEW_SOUND, 16);

class FileWeaponInfo
{
public:
	static constexpr inline std::size_t MaxShootSounds = 16;
	static constexpr inline std::size_t MaxWeaponString = 80;
	static constexpr inline std::size_t MaxWeaponPrefix = 16;
	static constexpr inline std::size_t MaxWeaponAmmoName = 32;
	bool parsedScript;
	bool loadedHudElements;

	std::array<char, MaxWeaponString> className;
	std::array<char, MaxWeaponString> printName;
	std::array<char, MaxWeaponString> viewModel;
	std::array<char, MaxWeaponString> worldModel;
	std::array<char, MaxWeaponAmmoName> ammo1;
	std::array<char, MaxWeaponString> worldDroppedModel;
	std::array<char, MaxWeaponPrefix> animationPrefix;

	i32 slot;
	i32 position;
	i32 maxClip1;
	i32 maxClip2;
	i32 defaultClip1;
	i32 defaultClip2;
	i32 weight;
	i32 rumbleEffect;
	bool autoSwitchTo;
	bool autoSwitchFrom;
	i32 flags;

	std::array<char, MaxWeaponAmmoName> ammo2;
	std::array<char, MaxWeaponString> aiAddOn;

	// Sound blocks
	std::array<std::array<char, WeaponSound::size>, MaxWeaponString> shootSounds;

	i32 ammoType;
	i32 ammo2Type;
	bool meleeWeapon;

	// This tells if the weapon was built right-handed (defaults to true).
	// This helps cl_righthand make the decision about whether to flip the model or not.
	bool builtRightHanded;
	bool allowFlipping;

	// Sprite data, read from the data file
	i32 spriteCount;
	CHudTexture* iconActive;
	CHudTexture* iconInactive;
	CHudTexture* iconAmmo;
	CHudTexture* iconAmmo2;
	CHudTexture* iconCrosshair;
	CHudTexture* iconAutoaim;
	CHudTexture* iconZoomedCrosshair;
	CHudTexture* iconZoomedAutoaim;
	CHudTexture* iconSmall;
};

#endif // SDK_FILEWEAPONINFO_HPP
