#ifndef SDK_DEFS_HPP
#define SDK_DEFS_HPP

#include "../Util/Defs.hpp"

struct OcclusionParams
{
	f32 maxOccludeeArea;
	f32 minOccluderArea;
};

MAKE_CENUM_Q(SkyboxVisibility, u32,
	NOT_VISIBLE, 0,
	SKYBOX3D_VISIBLE, 1,
	SKYBOX2D_VISIBLE, 2);

MAKE_CENUM_Q(EngineInputContextId, u32,
	GAME, 0,
	GAMEUI, 1);

typedef i32 string_t;

struct CUserCmd
{
	virtual ~CUserCmd() {}
	
	i32 commandNumber;
	i32 tickCount;
	QAngle viewAngles;
	QAngle aimDirection;
	f32 forwardMove;
	f32 sideMove;
	f32 upMove;
	i32 buttons;
	u8 impulse;
	i32 weaponSelect;
	i32 weaponSubtype;
	i32 randomSeed;
	i16 mousedx;
	i16 mousedy;
	bool hasBeenPredicted;
	QAngle headAngles;
	Vec3 headOffset;
};

#endif // SDK_DEFS_HPP
