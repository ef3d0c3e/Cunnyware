#ifndef SDK_VISUALS_HPP
#define SDK_VISUALS_HPP

#include "../Util/Util.hpp"
#include "../ImGUI/imgui.h"
#include "../SDK/IVModelRender.hpp"

struct ChamsMat
{
	std::string code;
	std::string name;
	IMaterial* mat;
	std::vector<ImVec4> colors; // only lerp

	f32 value; // Arbitrary value [slider float / float input box] depending on the modulation type
	MAKE_CENUM_Q(PlayerModulation, u8,
		NONE, 0, // no modulation
		STATIC, 1, // Color at value
		HEALTH, 2, // 0.f dead, 1.f
		ARMOR, 3,
		AMMO, 4, //(ratio / clipsize)
		DISTANCE, 5, // (ratio / MAX),
		PING, 6, // (ratio / MAX),
		DORMANT, 7, // (ratio / 1500)
	);
	MAKE_CENUM_Q(WeaponModulation, u8,
		NONE, 0,
		STATIC, 1,
		AMMO, 2,
		DISTANCE, 3);
	MAKE_CENUM_Q(TimedGrenadeModulation, u8, // TODO ???
		NONE, 0,
		STATIC, 1,
		DISTANCE, 2,
		TIMER, 3);
	MAKE_CENUM_Q(BombModulation, u8,
		NONE, 0,
		STATIC, 1,
		DISTANCE, 2,
		TIMER, 3);
	MAKE_CENUM_Q(Modulation, u8,
		NONE, 0,
		STATIC, 1,
		DISTANCE, 2);
	union
	{
		PlayerModulation playerModulation;
		WeaponModulation weaponModulation;
		TimedGrenadeModulation timedGrenadeModulation;
		BombModulation bombModulation;
		Modulation modulation; // molo/smokes/decoy/chickens/hostages
	};
};

namespace Settings
{
namespace Visuals
{
	extern bool enabled;
	extern bool antialiasing;
} // Visuals

namespace ESP
{
MAKE_CENUM_Q(BoxType, u32,
	NONE, 0,
	BOX_2D, 1,
	BOX_3D, 2);
namespace Enemies
{
	extern bool enabled;
	extern BoxType box;
	extern ImVec4 boxColorVisible;
	extern ImVec4 boxColorInvisible;

	extern bool skeleton;
	extern ImVec4 skeletonColorVisible;
	extern ImVec4 skeletonColorInvisible;
	extern bool barrel;
	extern f32 barrelLength;
	extern ImVec4 barrelColor;

	extern bool healthBar;
	extern ImVec4 healthBarColor;
	extern bool healthBarHealthBasedColor;
	extern bool ammoBar;
	extern ImVec4 ammoBarColor;
	extern ImVec4 ammoBarReloadingColor;

	extern bool currentWeapon;
	extern ImVec4 currentWeaponColor;
	extern bool otherWeapon;
	extern ImVec4 otherWeaponColor;
	extern bool grenades;
	extern ImVec4 grenadesColor;

	// Informations
	extern bool name;
	extern ImVec4 nameColor;
	extern bool clan;
	extern ImVec4 clanColor;
	extern bool hp;
	extern ImVec4 hpColor;
	extern bool armor;
	extern ImVec4 armorColor;
	extern bool kit;
	extern ImVec4 kitColor;
	extern ImVec4 kitDefusingColor;
	extern bool bomb;
	extern ImVec4 bombColor;
	extern ImVec4 bombColorPlanting;
	extern bool hostage;
	extern ImVec4 hostageColor;
	extern ImVec4 hostageGrabbingColor;
	extern bool scoped;
	extern ImVec4 scopedColor;
	extern bool dormant;
	extern ImVec4 dormantColor;
	extern bool ping;
	extern ImVec4 pingColor;
} // Enemies

namespace Allies
{
	extern bool enabled;
	extern BoxType box;
	extern ImVec4 boxColorVisible;
	extern ImVec4 boxColorInvisible;

	extern bool skeleton;
	extern ImVec4 skeletonColorVisible;
	extern ImVec4 skeletonColorInvisible;
	extern bool barrel;
	extern f32 barrelLength;
	extern ImVec4 barrelColor;

	extern bool healthBar;
	extern ImVec4 healthBarColor;
	extern bool healthBarHealthBasedColor;
	extern bool ammoBar;
	extern ImVec4 ammoBarColor;
	extern ImVec4 ammoBarReloadingColor;

	extern bool currentWeapon;
	extern ImVec4 currentWeaponColor;
	extern bool otherWeapon;
	extern ImVec4 otherWeaponColor;
	extern bool grenades;
	extern ImVec4 grenadesColor;

	// Informations
	extern bool name;
	extern ImVec4 nameColor;
	extern bool clan;
	extern ImVec4 clanColor;
	extern bool hp;
	extern ImVec4 hpColor;
	extern bool armor;
	extern ImVec4 armorColor;
	extern bool kit;
	extern ImVec4 kitColor;
	extern ImVec4 kitDefusingColor;
	extern bool bomb;
	extern ImVec4 bombColor;
	extern ImVec4 bombColorPlanting;
	extern bool hostage;
	extern ImVec4 hostageColor;
	extern ImVec4 hostageGrabbingColor;
	extern bool scoped;
	extern ImVec4 scopedColor;
	extern bool dormant;
	extern ImVec4 dormantColor;
	extern bool ping;
	extern ImVec4 pingColor;
} // Allies

} // ESP

namespace Chams
{
namespace Enemies
{
	extern bool drawOriginalModel;
	extern std::vector<ChamsMat> materials;
	extern bool validMaterials;
} // Enemies

namespace Allies
{
	extern bool drawOriginalModel;
	extern std::vector<ChamsMat> materials;
	extern bool validMaterials;
} // Allies

} // Chams

} // Settings

namespace ESP
{
	bool WorldToScreen(const Vec3& origin, Vec2& screen);

	void Paint();
}

namespace Chams
{
	bool DrawModelExecute(void* thisptr, class IMatRenderContext* renderContext, const class DrawModelState& state, const ModelRenderInfo& info, Mat3x4* customBoneToWorld);
}

#endif // SDK_VISUALS_HPP
