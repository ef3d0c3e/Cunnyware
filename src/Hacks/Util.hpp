#ifndef HACKS_UTIL_HPP
#define HACKS_UTIL_HPP

#include "../SDK/C_BaseEntity.hpp"
#include "../SDK/C_BasePlayer.hpp"
#include "../SDK/CCSWeaponInfo.hpp"

MAKE_CENUM_Q(GameTypes, i32,
	CLASSIC, 0,     // Casual and competitive (see gamerules_*.cfg)
	GUNGAME, 1,     // Arms race
	TRAINING, 2,    // training map - just one map in this mode
	CUSTOM, 3,      // capture the flag and such, set by map; not supported by matchmaking, supported through workshop, there are very few maps that use this game mode
	COOPERATIVE, 4, // players vs bots - matchmade, operation missions
	DANGER_ZONE, 6);

namespace Util
{
	bool IsDangerZone();
	bool IsVisible(C_BasePlayer* player, i32 bone, f32 fov = 180.f, bool smoke_check = false);
	bool IsSpotVisible(C_BasePlayer* player, Vec3 spot, f32 fov = 180.f, bool smoke_check = false);
	bool IsVisibleThroughEnemies(C_BasePlayer *player, i32 bone, f32 fov = 180.f, bool smoke_check = false);
	bool IsSpotVisibleThroughEnemies(C_BasePlayer *player, Vec3 spot, f32 fov = 180.f, bool smoke_check = false);
	bool IsPlanting(C_BasePlayer* player);
	bool IsTeamMate(C_BasePlayer* player, C_BasePlayer* localplayer);
	bool IsOtherWeapon(CSWeaponType type);
	bool IsGrenade(CSWeaponType type);
}


#endif // HACKS_UTIL_HPP
