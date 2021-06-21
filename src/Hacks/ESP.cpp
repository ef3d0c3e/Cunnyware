#include "Visuals.hpp"
#include "../Config.hpp"
#include "../Util/Draw.hpp"
#include "../Interface.hpp"
#include "../SDK/C_BaseEntity.hpp"
#include "../Hooks/Hooks.hpp"

// {{{ Configuration
// Enemies
EXPORT(bool, Settings::ESP::Enemies::enabled) = true;
EXPORT(Settings::ESP::BoxType, Settings::ESP::Enemies::box) = Settings::ESP::BoxType::BOX_2D;
EXPORT(ImVec4, Settings::ESP::Enemies::boxColorVisible) = ImVec4(.91f, .62f, .25, 1.f);
EXPORT(ImVec4, Settings::ESP::Enemies::boxColorInvisible) = ImVec4(1.f, .42f, .25, 1.f);
EXPORT(bool, Settings::ESP::Enemies::skeleton) = false;
EXPORT(ImVec4, Settings::ESP::Enemies::skeletonColorVisible) = ImVec4(.91f, .62f, .25, 1.f);
EXPORT(ImVec4, Settings::ESP::Enemies::skeletonColorInvisible) = ImVec4(1.f, .42f, .25, 1.f);
EXPORT(bool, Settings::ESP::Enemies::barrel) = false;
EXPORT(f32, Settings::ESP::Enemies::barrelLength) = 128.f;
EXPORT(ImVec4, Settings::ESP::Enemies::barrelColor) = ImVec4(.40f, .58f, .80f, 1.f);
EXPORT(bool, Settings::ESP::Enemies::healthBar) = true;
EXPORT(ImVec4, Settings::ESP::Enemies::healthBarColor) = ImVec4(.27f, .71f, .26f, 1.f);
EXPORT(bool, Settings::ESP::Enemies::healthBarHealthBasedColor) = true;
EXPORT(bool, Settings::ESP::Enemies::ammoBar) = true;
EXPORT(ImVec4, Settings::ESP::Enemies::ammoBarColor) = ImVec4(.33f, .53f, .81f, 1.f);
EXPORT(bool, Settings::ESP::Enemies::primaryWeapon) = true;
EXPORT(ImVec4, Settings::ESP::Enemies::primaryWeaponColor) = ImVec4(.60f, .73f, .90f, 1.f);
EXPORT(bool, Settings::ESP::Enemies::secondaryWeapon) = false;
EXPORT(ImVec4, Settings::ESP::Enemies::secondaryWeaponColor) = ImVec4(.60f, .73f, .90f, 1.f);
EXPORT(bool, Settings::ESP::Enemies::grenades) = false;
EXPORT(ImVec4, Settings::ESP::Enemies::grenadesColor) = ImVec4(.60f, .73f, .90f, 1.f);
EXPORT(bool, Settings::ESP::Enemies::name) = true;
EXPORT(ImVec4, Settings::ESP::Enemies::nameColor) = ImVec4(.91f, .62f, .25, 1.f);
EXPORT(bool, Settings::ESP::Enemies::clan) = false;
EXPORT(ImVec4, Settings::ESP::Enemies::clanColor) = ImVec4(.54f, .87f, .53f, 1.f);
EXPORT(bool, Settings::ESP::Enemies::hp) = true;
EXPORT(ImVec4, Settings::ESP::Enemies::hpColor) = ImVec4(.94f, .56f, .56f, 1.f);
EXPORT(bool, Settings::ESP::Enemies::kit) = false;
EXPORT(ImVec4, Settings::ESP::Enemies::kitColor) = ImVec4(.18f, .65f, .98f, 1.f);
EXPORT(bool, Settings::ESP::Enemies::bomb) = false;
EXPORT(ImVec4, Settings::ESP::Enemies::bombColor) = ImVec4(.18f, .65f, .98f, 1.f);
EXPORT(bool, Settings::ESP::Enemies::hostage) = false;
EXPORT(ImVec4, Settings::ESP::Enemies::hostageColor) = ImVec4(.88f, .52f, .06f, 1.f);
EXPORT(bool, Settings::ESP::Enemies::reloading) = true;
EXPORT(ImVec4, Settings::ESP::Enemies::reloadingColor) = ImVec4(.17f, .44f, .76f, 1.f);
EXPORT(bool, Settings::ESP::Enemies::scoped) = true;
EXPORT(ImVec4, Settings::ESP::Enemies::scopedColor) = ImVec4(.17f, .44f, .76f, 1.f);
EXPORT(bool, Settings::ESP::Enemies::dormant) = false;
EXPORT(ImVec4, Settings::ESP::Enemies::dormantColor) = ImVec4(.76f, .17f, .44f, 1.f);
EXPORT(bool, Settings::ESP::Enemies::ping) = true;
EXPORT(ImVec4, Settings::ESP::Enemies::pingColor) = ImVec4(.17f, .76f, .21f, 1.f);
// }}}

static Mat4x4 vMatrix;
bool ESP::WorldToScreen(const Vec3& origin, Vec2& screen)
{
	f32 w = vMatrix[3][0] * origin.x
		  + vMatrix[3][1] * origin.y
		  + vMatrix[3][2] * origin.z
		  + vMatrix[3][3];

	if ( w < 0.01f ) // Is Not in front of our player
		return false;

	f32 width = (float)Paint::engineWidth;
	f32 height = (float)Paint::engineHeight;

	f32 halfWidth = width / 2;
	f32 halfHeight = height / 2;

	f32 inverseW = 1 / w;

	screen.x = halfWidth +
				(0.5f * ((vMatrix[0][0] * origin.x +
						  vMatrix[0][1] * origin.y +
						  vMatrix[0][2] * origin.z +
						  vMatrix[0][3]) * inverseW) * width + 0.5f);

	screen.y = halfHeight -
				(0.5f * ((vMatrix[1][0] * origin.x +
						  vMatrix[1][1] * origin.y +
						  vMatrix[1][2] * origin.z +
						  vMatrix[1][3]) * inverseW) * height + 0.5f);
	return true;
}

void DrawEnemy(C_BaseEntity* e)
{

}

void ESP::Paint()
{
	if(!engine->IsInGame())
		return;
	vMatrix = engine->WorldToScreenMatrix();

	if (!Settings::Visuals::enabled || !engine->IsInGame())
		return;

	C_BaseEntity* lp = entityList->GetClientEntity(engine->GetLocalPlayer());
	if (!lp)
		return;

	for (i32 i = 1; i < entityList->GetHighestEntityIndex(); ++i)
	{
		C_BaseEntity* ent = entityList->GetClientEntity(i);
		if (!ent)
			continue;
		ClientClass* client = ent->GetClientClass();


	}
}
