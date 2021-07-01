#ifndef SDK_C_BASEPLAYER_HPP
#define SDK_C_BASEPLAYER_HPP

#include "CUtl/Vector.hpp"
#include "C_BaseEntity.hpp"
#include "AnimationLayer.hpp"
#include "CCSGOAnimState.hpp"
#include "CBaseHandle.hpp"

class C_BaseCombatWeapon;

static constexpr inline std::size_t MaxStudioBones = 128;
static constexpr inline u8 LifeAlive = 0;
static constexpr inline i32 BoneUsedByHitbox = 0x100;

MAKE_CENUM_Q(ObserverMode, i32,
	NONE, 0,
	DEATHCAM, 1,
	FREEZECAM, 2,
	FIXED, 3,
	IN_EYE, 4,
	CHASE, 5,
	ROAMING, 6);

class C_BasePlayer : public C_BaseEntity
{
public:
	i32 GetSequenceActivity(i32 sequence)
	{
		if (!GetSeqActivity)
			return -1;
		return GetSeqActivity(this, sequence);
	}
	CUtlVector<AnimationLayer>* GetAnimOverlay()
	{
		return reinterpret_cast<CUtlVector<AnimationLayer>*>((uintptr_t)this + Offsets::playerAnimOverlayOffset); // 12312
	}
	
	CCSGOAnimState* GetAnimState()
	{
		return *reinterpret_cast<CCSGOAnimState**>((std::uintptr_t)this + Offsets::playerAnimStateOffset);
	}
	
	QAngle* GetViewPunchAngle()
	{
		return (QAngle*)((std::uintptr_t)this + offsets.DT_BasePlayer.m_viewPunchAngle);
	}
	
	QAngle* GetAimPunchAngle()
	{
		return (QAngle*)((std::uintptr_t)this + offsets.DT_BasePlayer.m_aimPunchAngle);
	}
	
	Vec3 GetVecViewOffset()
	{
		return *(Vec3*)((std::uintptr_t)this + offsets.DT_BasePlayer.m_vecViewOffset);
	}
	
	u32 GetTickBase()
	{
		return *(u32*)((std::uintptr_t)this + offsets.DT_BasePlayer.m_nTickBase);
	}
	
	Vec3 GetVelocity()
	{
		return *(Vec3*)((std::uintptr_t)this + offsets.DT_BasePlayer.m_vecVelocity);
	}
	
	i32 GetHealth()
	{
		return *(i32*)((std::uintptr_t)this + offsets.DT_BasePlayer.m_iHealth);
	}
	
	unsigned char GetLifeState()
	{
		return *(unsigned char*)((std::uintptr_t)this + offsets.DT_BasePlayer.m_lifeState);
	}
	
	i32 GetFlags()
	{
		return *(i32*)((std::uintptr_t)this + offsets.DT_BasePlayer.m_fFlags);
	}
	
	ObserverMode* GetObserverMode()
	{
		return (ObserverMode*)((std::uintptr_t)this + offsets.DT_BasePlayer.m_iObserverMode);
	}
	
	void* GetObserverTarget()
	{
		return (void*)((std::uintptr_t)this + offsets.DT_BasePlayer.m_hObserverTarget);
	}
	
	void* GetViewModel()
	{
		return (void*)((std::uintptr_t)this + offsets.DT_BasePlayer.m_hViewModel);
	}
	
	const char* GetLastPlaceName()
	{
		return (const char*)((std::uintptr_t)this + offsets.DT_BasePlayer.m_szLastPlaceName);
	}
	
	i32 GetShotsFired()
	{
		return *(i32*)((std::uintptr_t)this + offsets.DT_CSPlayer.m_iShotsFired);
	}
	
	QAngle* GetEyeAngles()
	{
		return (QAngle*)((std::uintptr_t)this + offsets.DT_CSPlayer.m_angEyeAngles[0]);
	}

	i32 GetMoney()
	{
		return *(i32*)((std::uintptr_t)this + offsets.DT_CSPlayer.m_iAccount);
	}
	
	i32 GetHits()
	{
		return *(i32*)((std::uintptr_t)this + offsets.DT_CSPlayer.m_totalHitsOnServer);
	}
	
	i32 GetArmor()
	{
		return *(i32*)((std::uintptr_t)this + offsets.DT_CSPlayer.m_ArmorValue);
	}
	
	i32 HasDefuser()
	{
		return *(i32*)((std::uintptr_t)this + offsets.DT_CSPlayer.m_bHasDefuser);
	}
	
	bool IsDefusing()
	{
		return *(bool*)((std::uintptr_t)this + offsets.DT_CSPlayer.m_bIsDefusing);
	}
	
	bool IsGrabbingHostage()
	{
		return *(bool*)((std::uintptr_t)this + offsets.DT_CSPlayer.m_bIsGrabbingHostage);
	}
	
	bool IsScoped()
	{
		return *(bool*)((std::uintptr_t)this + offsets.DT_CSPlayer.m_bIsScoped);
	}
	
	bool GetImmune()
	{
		return *(bool*)((std::uintptr_t)this + offsets.DT_CSPlayer.m_bGunGameImmunity);
	}
	
	bool IsRescuing()
	{
		return *(bool*)((std::uintptr_t)this + offsets.DT_CSPlayer.m_bIsRescuing);
	}
	
	i32 HasHelmet()
	{
		return *(i32*)((std::uintptr_t)this + offsets.DT_CSPlayer.m_bHasHelmet);
	}
	
	bool IsFlashed() // Pasted from CSGOSimple.
	{ // GetFlashBangTime() - globalVars->curtime > 2.0f
		return *(f32*)((std::uintptr_t)this->GetFlashMaxAlpha() - 0x8) > 200.0;
	}
	
	f32 GetFlashBangTime()
	{
		return *(f32*)((std::uintptr_t)this + 0xABF4);
	}
	
	f32 GetFlashDuration()
	{
		return *(f32*)((std::uintptr_t)this + offsets.DT_CSPlayer.m_flFlashDuration);
	}
	
	f32* GetFlashMaxAlpha()
	{
		return (f32*)((std::uintptr_t)this + offsets.DT_CSPlayer.m_flFlashMaxAlpha);
	}
	
	f32* GetLowerBodyYawTarget()
	{
		return (f32*)((std::uintptr_t)this + offsets.DT_CSPlayer.m_flLowerBodyYawTarget);
	}
	
	CBaseHandle* GetActiveWeapon()
	{
		return (CBaseHandle*)((std::uintptr_t)this + offsets.DT_BaseCombatCharacter.m_hActiveWeapon);
	}
	
	static constexpr inline std::size_t MaxWeapons = 64;
	const std::array<CBaseHandle, MaxWeapons>& GetWeapons()
	{
		return *(const std::array<CBaseHandle, MaxWeapons>*)((std::uintptr_t)this + offsets.DT_BaseCombatCharacter.m_hMyWeapons);
	}
	
	i32* GetWearables()
	{
		return (i32*)((std::uintptr_t)this + offsets.DT_BaseCombatCharacter.m_hMyWearables);
	}
	
	bool GetAlive()
	{
		return this->GetHealth() > 0 && this->GetLifeState() == LifeAlive;
	}
	
	Vec3 GetEyePosition()
	{
		return this->GetVecOrigin() + this->GetVecViewOffset();
	}
	
	inline Vec3 GetBonePosition(i32 boneIndex)
	{
		Mat3x4 BoneMatrix[MaxStudioBones];
	
		if (!this->SetupBones(BoneMatrix, MaxStudioBones, BoneUsedByHitbox, 0))
			return this->GetVecOrigin();
	
		Mat3x4 hitbox = BoneMatrix[boneIndex];
	
		return Vec3(hitbox[0][3], hitbox[1][3], hitbox[2][3]);
	}
	/*
		inline Vector GetNoInterpBonePosition(int boneIndex)
		{
			matrix3x4_t BoneMatrix[MaxStudioBones];
			Vector backup = this->GetAbsOrigin();
			Vector noInterp = this->GetVecOrigin();
			this->SetAbsOrigin( &noInterp );
			if (!this->SetupBones(BoneMatrix, MaxStudioBones, BONE_USED_BY_HITBOX, 0)) {
				this->SetAbsOrigin( &backup );
				return this->GetAbsOrigin( );
			}
	
			matrix3x4_t hitbox = BoneMatrix[boneIndex];
	
			this->SetAbsOrigin( &backup );
			return Vector(hitbox[0][3], hitbox[1][3], hitbox[2][3]);
		}*/
	
	QAngle* GetVAngles()
	{
		return (QAngle*)((std::uintptr_t)this + offsets.DT_BasePlayer.deadflag + 0x4);
	}
	
	i32 GetHitboxSetCount()
	{
		return *(i32*)((std::uintptr_t)this + offsets.DT_BaseAnimating.m_nHitboxSet);
	}
};

#endif // SDK_C_BASEPLAYER_HPP
