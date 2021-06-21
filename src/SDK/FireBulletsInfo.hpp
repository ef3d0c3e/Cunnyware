#ifndef SDK_FIREBULLETSINFO_HPP
#define SDK_FIREBULLETSINFO_HPP

#include "Defs.hpp"

class CBaseEntity;

MAKE_CENUM_Q(FireBulletsFlags, u32,
	FIRST_SHOT_ACCURATE, 0x1,         // Pop the first shot with perfect accuracy
	DONT_HIT_UNDERWATER, 0x2,         // If the shot hits its target underwater, don't damage it
	ALLOW_WATER_SURFACE_IMPACTS, 0x4, // If the shot hits water surface, still call DoImpactEffect
	TEMPORARY_DANGER_SOUND, 0x8,      // Danger sounds added from this impact can be stomped immediately if another is queued
	NO_PIERCING_SPARK, 0x16,          // do a piercing spark effect when a bullet penetrates an alien
	HULL, 0x32,                       // bullet trace is a hull rather than a line
	ANGULAR_SPREAD, 0x64,             // bullet spread is based on uniform random change to angles rather than gaussian search
);

struct FireBulletsInfo
{
	FireBulletsInfo()
	{
		shots = 1;
		spread = Vec3(0, 0, 0);
		distance = 8192;
		tracerFreq = 4;
		damage = 0.0f;
		playerDamage = 0.0f;
		attacker = NULL;
		flags = 0;
		additionalIgnoreEnt = NULL;
		damageForceScale = 1.0f;

		primaryAttack = true;
	}

	FireBulletsInfo(i32 shots,
			const Vec3& src,
			const Vec3& dir,
			const Vec3& spread,
			f32 distance,
			i32 ammoType,
			bool primaryAttack = true)
	{
		this->shots = shots;
		this->src = src;
		this->dirShooting = dir;
		this->spread = spread;
		this->distance = distance;
		this->ammoType = ammoType;
		this->tracerFreq = 4;
		this->damage = 0;
		this->playerDamage = 0;
		this->attacker = NULL;
		this->flags = 0;
		this->additionalIgnoreEnt = NULL;
		this->damageForceScale = 1.0f;
		this->primaryAttack = primaryAttack;
	}

	i32 shots;
	Vec3 src;
	Vec3 dirShooting;
	Vec3 spread;
	f32 distance;
	i32 ammoType;
	i32 tracerFreq;
	f32 damage;
	f32 playerDamage; // Damage to be used instead of m_flDamage if we hit a player
	i32 flags; // See FireBulletsFlags
	f32 damageForceScale;
	CBaseEntity* attacker;
	CBaseEntity* additionalIgnoreEnt;
	bool primaryAttack;
};

#endif // SDK_FIREBULLETSINFO_HPP
