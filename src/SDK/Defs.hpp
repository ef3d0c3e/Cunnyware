#ifndef SDK_DEFS_HPP
#define SDK_DEFS_HPP

#include "../Util/Defs.hpp"
#include "../Util/Util.hpp"

struct OcclusionParams
{
	f32 maxOccludeeArea;
	f32 minOccluderArea;
};

MAKE_CENUM_Q(ItemDefinitionIndex, i16,
	INVALID, -1,
	NONE, 0,
	DEAGLE, 1,
	ELITE, 2,
	FIVESEVEN, 3,
	GLOCK, 4,
	AK47, 7,
	AUG, 8,
	AWP, 9,
	FAMAS, 10,
	G3SG1, 11,
	GALILAR, 13,
	M249, 12,
	M4A1, 16,
	MAC10, 17,
	P90, 19,
	MP5, 23,
	UMP45, 24,
	XM1014, 25,
	BIZON, 26,
	MAG7, 27,
	NEGEV, 28,
	SAWEDOFF, 29,
	TEC9, 30,
	TASER, 31,
	HKP2000, 32,
	MP7, 33,
	MP9, 34,
	NOVA, 35,
	P250, 36,
	SHIELD, 37,
	SCAR20, 38,
	SG556, 39,
	SSG08, 40,
	KNIFEGG, 41,
	KNIFE, 42,
	FLASHBANG, 43,
	HEGRENADE, 44,
	SMOKEGRENADE, 45,
	MOLOTOV, 46,
	DECOY, 47,
	INCGRENADE, 48,
	C4, 49,
	HEALTHSHOT, 57,
	KNIFE_T, 59,
	M4A1_SILENCER, 60,
	USP_SILENCER, 61,
	CZ75A, 63,
	REVOLVER, 64,
	TAGRENADE, 68,
	FISTS, 69,
	BREACHCHARGE, 70,
	TABLET, 72,
	MELEE, 74,
	AXE, 75,
	HAMMER, 76,
	SPANNER, 78,
	KNIFE_GHOST, 80,
	FIREBOMB, 81,
	DIVERSION, 82,
	FRAG_GRENADE, 83,
	SNOWBALL, 84,
	BUMPMINE, 85,
	KNIFE_BAYONET, 500,
    KNIFE_CSS, 503,
	KNIFE_FLIP, 505,
	KNIFE_GUT, 506,
	KNIFE_KARAMBIT, 507,
	KNIFE_M9_BAYONET, 508,
	KNIFE_TACTICAL, 509,
	KNIFE_FALCHION, 512,
	KNIFE_SURVIVAL_BOWIE, 514,
	KNIFE_BUTTERFLY, 515,
	KNIFE_PUSH, 516,
	KNIFE_CORD, 517,
	KNIFE_CANIS, 518,
	KNIFE_URSUS, 519,
	KNIFE_GYPSY_JACKKNIFE, 520,
	KNIFE_OUTDOOR, 521,
	KNIFE_STILETTO, 522,
	KNIFE_WIDOWMAKER, 523,
	KNIFE_SKELETON, 525,
	GLOVE_STUDDED_BLOODHOUND, 5027,
	GLOVE_T_SIDE, 5028,
	GLOVE_CT_SIDE, 5029,
	GLOVE_SPORTY, 5030,
	GLOVE_SLICK, 5031,
	GLOVE_LEATHER_WRAP, 5032,
	GLOVE_MOTORCYCLE, 5033,
	GLOVE_SPECIALIST, 5034,
	GLOVE_HYDRA, 5035);

MAKE_CENUM_Q(SkyboxVisibility, u32,
	NOT_VISIBLE, 0,
	SKYBOX3D_VISIBLE, 1,
	SKYBOX2D_VISIBLE, 2);

MAKE_CENUM_Q(EngineInputContextId, u32,
	GAME, 0,
	GAMEUI, 1);

typedef i32 string_t;
typedef i32 QBool;
typedef u64 HCursor;
struct HSCRIPT
{ i32 unused; };
typedef u32 HTOOLHANDLE;
typedef u32 VPANEL;
typedef u16 MDLHandle;

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

MAKE_CENUM_Q(EClassIds, u32,
	CAI_BaseNPC, 0,
	CAK47, 1,
	CBaseAnimating, 2,
	CBaseAnimatingOverlay, 3,
	CBaseAttributableItem, 4,
	CBaseButton, 5,
	CBaseCombatCharacter, 6,
	CBaseCombatWeapon, 7,
	CBaseCSGrenade, 8,
	CBaseCSGrenadeProjectile, 9,
	CBaseDoor, 10,
	CBaseEntity, 11,
	CBaseFlex, 12,
	CBaseGrenade, 13,
	CBaseParticleEntity, 14,
	CBasePlayer, 15,
	CBasePropDoor, 16,
	CBaseTeamObjectiveResource, 17,
	CBaseTempEntity, 18,
	CBaseToggle, 19,
	CBaseTrigger, 20,
	CBaseViewModel, 21,
	CBaseVPhysicsTrigger, 22,
	CBaseWeaponWorldModel, 23,
	CBeam, 24,
	CBeamSpotlight, 25,
	CBoneFollower, 26,
	CBRC4Target, 27,
	CBreachCharge, 28,
	CBreachChargeProjectile, 29,
	CBreakableProp, 30,
	CBreakableSurface, 31,
	CBumpMine, 32,
	CBumpMineProjectile, 33,
	CC4, 34,
	CCascadeLight, 35,
	CChicken, 36,
	CColorCorrection, 37,
	CColorCorrectionVolume, 38,
	CCSGameRulesProxy, 39,
	CCSPlayer, 40,
	CCSPlayerResource, 41,
	CCSRagdoll, 42,
	CCSTeam, 43,
	CDangerZone, 44,
	CDangerZoneController, 45,
	CDEagle, 46,
	CDecoyGrenade, 47,
	CDecoyProjectile, 48,
	CDrone, 49,
	CDronegun, 50,
	CDynamicLight, 51,
	CDynamicProp, 52,
	CEconEntity, 53,
	CEconWearable, 54,
	CEmbers, 55,
	CEntityDissolve, 56,
	CEntityFlame, 57,
	CEntityFreezing, 58,
	CEntityParticleTrail, 59,
	CEnvAmbientLight, 60,
	CEnvDetailController, 61,
	CEnvDOFController, 62,
	CEnvGasCanister, 63,
	CEnvParticleScript, 64,
	CEnvProjectedTexture, 65,
	CEnvQuadraticBeam, 66,
	CEnvScreenEffect, 67,
	CEnvScreenOverlay, 68,
	CEnvTonemapController, 69,
	CEnvWind, 70,
	CFEPlayerDecal, 71,
	CFireCrackerBlast, 72,
	CFireSmoke, 73,
	CFireTrail, 74,
	CFish, 75,
	CFists, 76,
	CFlashbang, 77,
	CFogController, 78,
	CFootstepControl, 79,
	CFunc_Dust, 80,
	CFunc_LOD, 81,
	CFuncAreaPortalWindow, 82,
	CFuncBrush, 83,
	CFuncConveyor, 84,
	CFuncLadder, 85,
	CFuncMonitor, 86,
	CFuncMoveLinear, 87,
	CFuncOccluder, 88,
	CFuncReflectiveGlass, 89,
	CFuncRotating, 90,
	CFuncSmokeVolume, 91,
	CFuncTrackTrain, 92,
	CGameRulesProxy, 93,
	CGrassBurn, 94,
	CHandleTest, 95,
	CHEGrenade, 96,
	CHostage, 97,
	CHostageCarriableProp, 98,
	CIncendiaryGrenade, 99,
	CInferno, 100,
	CInfoLadderDismount, 101,
	CInfoMapRegion, 102,
	CInfoOverlayAccessor, 103,
	CItem_Healthshot, 104,
	CItemCash, 105,
	CItemDogtags, 106,
	CKnife, 107,
	CKnifeGG, 108,
	CLightGlow, 109,
	CMapVetoPickController, 110,
	CMaterialModifyControl, 111,
	CMelee, 112,
	CMolotovGrenade, 113,
	CMolotovProjectile, 114,
	CMovieDisplay, 115,
	CParadropChopper, 116,
	CParticleFire, 117,
	CParticlePerformanceMonitor, 118,
	CParticleSystem, 119,
	CPhysBox, 120,
	CPhysBoxMultiplayer, 121,
	CPhysicsProp, 122,
	CPhysicsPropMultiplayer, 123,
	CPhysMagnet, 124,
	CPhysPropAmmoBox, 125,
	CPhysPropLootCrate, 126,
	CPhysPropRadarJammer, 127,
	CPhysPropWeaponUpgrade, 128,
	CPlantedC4, 129,
	CPlasma, 130,
	CPlayerPing, 131,
	CPlayerResource, 132,
	CPointCamera, 133,
	CPointCommentaryNode, 134,
	CPointWorldText, 135,
	CPoseController, 136,
	CPostProcessController, 137,
	CPrecipitation, 138,
	CPrecipitationBlocker, 139,
	CPredictedViewModel, 140,
	CProp_Hallucination, 141,
	CPropCounter, 142,
	CPropDoorRotating, 143,
	CPropJeep, 144,
	CPropVehicleDriveable, 145,
	CRagdollManager, 146,
	CRagdollProp, 147,
	CRagdollPropAttached, 148,
	CRopeKeyframe, 149,
	CSCAR17, 150,
	CSceneEntity, 151,
	CSensorGrenade, 152,
	CSensorGrenadeProjectile, 153,
	CShadowControl, 154,
	CSlideshowDisplay, 155,
	CSmokeGrenade, 156,
	CSmokeGrenadeProjectile, 157,
	CSmokeStack, 158,
	CSnowball, 159,
	CSnowballPile, 160,
	CSnowballProjectile, 161,
	CSpatialEntity, 162,
	CSpotlightEnd, 163,
	CSprite, 164,
	CSpriteOriented, 165,
	CSpriteTrail, 166,
	CStatueProp, 167,
	CSteamJet, 168,
	CSun, 169,
	CSunlightShadowControl, 170,
	CSurvivalSpawnChopper, 171,
	CTablet, 172,
	CTeam, 173,
	CTeamplayRoundBasedRulesProxy, 174,
	CTEArmorRicochet, 175,
	CTEBaseBeam, 176,
	CTEBeamEntPoint, 177,
	CTEBeamEnts, 178,
	CTEBeamFollow, 179,
	CTEBeamLaser, 180,
	CTEBeamPoints, 181,
	CTEBeamRing, 182,
	CTEBeamRingPoint, 183,
	CTEBeamSpline, 184,
	CTEBloodSprite, 185,
	CTEBloodStream, 186,
	CTEBreakModel, 187,
	CTEBSPDecal, 188,
	CTEBubbles, 189,
	CTEBubbleTrail, 190,
	CTEClientProjectile, 191,
	CTEDecal, 192,
	CTEDust, 193,
	CTEDynamicLight, 194,
	CTEEffectDispatch, 195,
	CTEEnergySplash, 196,
	CTEExplosion, 197,
	CTEFireBullets, 198,
	CTEFizz, 199,
	CTEFootprintDecal, 200,
	CTEFoundryHelpers, 201,
	CTEGaussExplosion, 202,
	CTEGlowSprite, 203,
	CTEImpact, 204,
	CTEKillPlayerAttachments, 205,
	CTELargeFunnel, 206,
	CTEMetalSparks, 207,
	CTEMuzzleFlash, 208,
	CTEParticleSystem, 209,
	CTEPhysicsProp, 210,
	CTEPlantBomb, 211,
	CTEPlayerAnimEvent, 212,
	CTEPlayerDecal, 213,
	CTEProjectedDecal, 214,
	CTERadioIcon, 215,
	CTEShatterSurface, 216,
	CTEShowLine, 217,
	CTesla, 218,
	CTESmoke, 219,
	CTESparks, 220,
	CTESprite, 221,
	CTESpriteSpray, 222,
	CTest_ProxyToggle_Networkable, 223,
	CTestTraceline, 224,
	CTEWorldDecal, 225,
	CTriggerPlayerMovement, 226,
	CTriggerSoundOperator, 227,
	CVGuiScreen, 228,
	CVoteController, 229,
	CWaterBullet, 230,
	CWaterLODControl, 231,
	CWeaponAug, 232,
	CWeaponAWP, 233,
	CWeaponBaseItem, 234,
	CWeaponBizon, 235,
	CWeaponCSBase, 236,
	CWeaponCSBaseGun, 237,
	CWeaponCycler, 238,
	CWeaponElite, 239,
	CWeaponFamas, 240,
	CWeaponFiveSeven, 241,
	CWeaponG3SG1, 242,
	CWeaponGalil, 243,
	CWeaponGalilAR, 244,
	CWeaponGlock, 245,
	CWeaponHKP2000, 246,
	CWeaponM249, 247,
	CWeaponM3, 248,
	CWeaponM4A1, 249,
	CWeaponMAC10, 250,
	CWeaponMag7, 251,
	CWeaponMP5Navy, 252,
	CWeaponMP7, 253,
	CWeaponMP9, 254,
	CWeaponNegev, 255,
	CWeaponNOVA, 256,
	CWeaponP228, 257,
	CWeaponP250, 258,
	CWeaponP90, 259,
	CWeaponSawedoff, 260,
	CWeaponSCAR20, 261,
	CWeaponScout, 262,
	CWeaponSG550, 263,
	CWeaponSG552, 264,
	CWeaponSG556, 265,
	CWeaponShield, 266,
	CWeaponSSG08, 267,
	CWeaponTaser, 268,
	CWeaponTec9, 269,
	CWeaponTMP, 270,
	CWeaponUMP45, 271,
	CWeaponUSP, 272,
	CWeaponXM1014, 273,
	CWeaponZoneRepulsor, 274,
	CWorld, 275,
	CWorldVguiText, 276,
	DustTrail, 277,
	MovieExplosion, 278,
	ParticleSmokeGrenade, 279,
	RocketTrail, 280,
	SmokeTrail, 281,
	SporeExplosion, 282,
	SporeTrail, 283,
);

typedef i32 Bone;

MAKE_CENUM_Q(Bones, Bone,
	HIP, 3,
	LOWER_SPINE, 4,
	MIDDLE_SPINE, 5,
	UPPER_SPINE, 6,
	NECK, 7,
	HEAD, 8);

#define CONTENTS_EMPTY			0		// No contents

#define CONTENTS_SOLID			0x1		// an eye is never valid in a solid
#define CONTENTS_WINDOW			0x2		// translucent, but not watery (glass)
#define CONTENTS_AUX			0x4
#define CONTENTS_GRATE			0x8		// alpha-tested "grate" textures.  Bullets/sight pass through, but solids don't
#define CONTENTS_SLIME			0x10
#define CONTENTS_WATER			0x20
#define CONTENTS_BLOCKLOS		0x40	// block AI line of sight
#define CONTENTS_OPAQUE			0x80	// things that cannot be seen through (may be non-solid though)
#define LAST_VISIBLE_CONTENTS	CONTENTS_OPAQUE

#define ALL_VISIBLE_CONTENTS (LAST_VISIBLE_CONTENTS | (LAST_VISIBLE_CONTENTS-1))

#define CONTENTS_TESTFOGVOLUME	0x100
#define CONTENTS_UNUSED			0x200

// unused
// NOTE: If it's visible, grab from the top + update LAST_VISIBLE_CONTENTS
// if not visible, then grab from the bottom.
// CONTENTS_OPAQUE + SURF_NODRAW count as CONTENTS_OPAQUE (shadow-casting toolsblocklight textures)
#define CONTENTS_BLOCKLIGHT		0x400

#define CONTENTS_TEAM1			0x800	// per team contents used to differentiate collisions
#define CONTENTS_TEAM2			0x1000	// between players and objects on different teams

// ignore CONTENTS_OPAQUE on surfaces that have SURF_NODRAW
#define CONTENTS_IGNORE_NODRAW_OPAQUE	0x2000

// hits entities which are MOVETYPE_PUSH (doors, plats, etc.)
#define CONTENTS_MOVEABLE		0x4000

// remaining contents are non-visible, and don't eat brushes
#define CONTENTS_AREAPORTAL		0x8000

#define CONTENTS_PLAYERCLIP		0x10000
#define CONTENTS_MONSTERCLIP	0x20000

// currents can be added to any other contents, and may be mixed
#define CONTENTS_CURRENT_0		0x40000
#define CONTENTS_CURRENT_90		0x80000
#define CONTENTS_CURRENT_180	0x100000
#define CONTENTS_CURRENT_270	0x200000
#define CONTENTS_CURRENT_UP		0x400000
#define CONTENTS_CURRENT_DOWN	0x800000

#define CONTENTS_ORIGIN			0x1000000	// removed before bsping an entity

#define CONTENTS_MONSTER		0x2000000	// should never be on a brush, only in game
#define CONTENTS_DEBRIS			0x4000000
#define CONTENTS_DETAIL			0x8000000	// brushes to be added after vis leafs
#define CONTENTS_TRANSLUCENT	0x10000000	// auto set if any surface has trans
#define CONTENTS_LADDER			0x20000000
#define CONTENTS_HITBOX			0x40000000	// use accurate hitboxes on trace

#define MASK_SHOT	(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)

#define MASK_ALL					(0xFFFFFFFF)
// everything that is normally solid
#define MASK_SOLID					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
// everything that blocks player movement
#define MASK_PLAYERSOLID			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
// blocks npc movement
#define MASK_NPCSOLID				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER|CONTENTS_GRATE)
// blocks fluid movement
#define MASK_NPCFLUID				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTERCLIP|CONTENTS_WINDOW|CONTENTS_MONSTER)
// water physics in these contents
#define MASK_WATER					(CONTENTS_WATER|CONTENTS_MOVEABLE|CONTENTS_SLIME)
// everything that blocks lighting
#define MASK_OPAQUE					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_OPAQUE)
// everything that blocks lighting, but with monsters added.
#define MASK_OPAQUE_AND_NPCS		(MASK_OPAQUE|CONTENTS_MONSTER)
// everything that blocks line of sight for AI
#define MASK_BLOCKLOS				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_BLOCKLOS)
// everything that blocks line of sight for AI plus NPCs
#define MASK_BLOCKLOS_AND_NPCS		(MASK_BLOCKLOS|CONTENTS_MONSTER)
// everything that blocks line of sight for players
#define MASK_VISIBLE					(MASK_OPAQUE|CONTENTS_IGNORE_NODRAW_OPAQUE)
// everything that blocks line of sight for players, but with monsters added.
#define MASK_VISIBLE_AND_NPCS		(MASK_OPAQUE_AND_NPCS|CONTENTS_IGNORE_NODRAW_OPAQUE)
// bullets see these as solid
#define MASK_SHOT					(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_HITBOX)
// bullets see these as solid, except monsters (world+brush only)
#define MASK_SHOT_BRUSHONLY			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_DEBRIS)
// non-raycasted weapons see this as solid (includes grates)
#define MASK_SHOT_HULL				(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_MONSTER|CONTENTS_WINDOW|CONTENTS_DEBRIS|CONTENTS_GRATE)
// hits solids (not grates) and passes through everything else
#define MASK_SHOT_PORTAL			(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTER)
// everything normally solid, except monsters (world+brush only)
#define MASK_SOLID_BRUSHONLY		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_GRATE)
// everything normally solid for player movement, except monsters (world+brush only)
#define MASK_PLAYERSOLID_BRUSHONLY	(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_PLAYERCLIP|CONTENTS_GRATE)
// everything normally solid for npc movement, except monsters (world+brush only)
#define MASK_NPCSOLID_BRUSHONLY		(CONTENTS_SOLID|CONTENTS_MOVEABLE|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
// just the world, used for route rebuilding
#define MASK_NPCWORLDSTATIC			(CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP|CONTENTS_GRATE)
// just the world, used for route rebuilding
#define MASK_NPCWORLDSTATIC_FLUID	(CONTENTS_SOLID|CONTENTS_WINDOW|CONTENTS_MONSTERCLIP)
// These are things that can split areaportals
#define MASK_SPLITAREAPORTAL		(CONTENTS_WATER|CONTENTS_SLIME)

// UNDONE: This is untested, any moving water
#define MASK_CURRENT				(CONTENTS_CURRENT_0|CONTENTS_CURRENT_90|CONTENTS_CURRENT_180|CONTENTS_CURRENT_270|CONTENTS_CURRENT_UP|CONTENTS_CURRENT_DOWN)

// everything that blocks corpse movement
// UNDONE: Not used yet / may be deleted
#define MASK_DEADSOLID				(CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_WINDOW|CONTENTS_GRATE)

#endif // SDK_DEFS_HPP
