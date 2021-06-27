#ifndef SDK_VISUALS_HPP
#define SDK_VISUALS_HPP

#include "../Util/Util.hpp"
#include "../ImGUI/imgui.h"

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
	extern bool bomb;
	extern ImVec4 bombColor;
	extern ImVec4 bombColorPlanting;
	extern bool hostage;
	extern ImVec4 hostageColor;
	extern bool reloading;
	extern ImVec4 reloadingColor;
	extern bool scoped;
	extern ImVec4 scopedColor;
	extern bool dormant;
	extern ImVec4 dormantColor;
	extern bool ping;
	extern ImVec4 pingColor;
} // Enemies

} // ESP
} // Settings

namespace ESP
{
	//bool WorldToScreen(const Vec3& pos, ImVec2 screen);
	bool WorldToScreen(const Vec3& origin, Vec2& screen);

	void Paint();
}

#endif // SDK_VISUALS_HPP
