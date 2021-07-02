#include "Visuals.hpp"
#include "../Config.hpp"
#include "../Util/Draw.hpp"
#include "../Interface.hpp"
#include "../Hooks/Hooks.hpp"
#include "../SDK/C_BaseEntity.hpp"
#include "../SDK/C_BasePlayer.hpp"
#include "../SDK/C_BaseCombatWeapon.hpp"
#include "../SDK/Recv.hpp"
#include "../SDK/ICollideable.hpp"
#include "../SDK/StudioHdr.hpp"
#include "../UI/Widgets.hpp"
#include "../UI/UI.hpp"
#include "Info.hpp"
#include "Util.hpp"
#include <sstream>

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
EXPORT(bool, Settings::ESP::Enemies::healthBarHealthBasedColor) = false;
EXPORT(bool, Settings::ESP::Enemies::ammoBar) = true;
EXPORT(ImVec4, Settings::ESP::Enemies::ammoBarColor) = ImVec4(.33f, .53f, .81f, 1.f);
EXPORT(ImVec4, Settings::ESP::Enemies::ammoBarReloadingColor) = ImVec4(.73f, .53f, .81f, 1.f);
EXPORT(bool, Settings::ESP::Enemies::currentWeapon) = true;
EXPORT(ImVec4, Settings::ESP::Enemies::currentWeaponColor) = ImVec4(.60f, .73f, .90f, 1.f);
EXPORT(bool, Settings::ESP::Enemies::otherWeapon) = false;
EXPORT(ImVec4, Settings::ESP::Enemies::otherWeaponColor) = ImVec4(.60f, .73f, .90f, 1.f);
EXPORT(bool, Settings::ESP::Enemies::grenades) = false;
EXPORT(ImVec4, Settings::ESP::Enemies::grenadesColor) = ImVec4(.60f, .73f, .90f, 1.f);
EXPORT(bool, Settings::ESP::Enemies::name) = true;
EXPORT(ImVec4, Settings::ESP::Enemies::nameColor) = ImVec4(.91f, .62f, .25, 1.f);
EXPORT(bool, Settings::ESP::Enemies::clan) = false;
EXPORT(ImVec4, Settings::ESP::Enemies::clanColor) = ImVec4(.54f, .87f, .53f, 1.f);
EXPORT(bool, Settings::ESP::Enemies::hp) = true;
EXPORT(ImVec4, Settings::ESP::Enemies::hpColor) = ImVec4(.94f, .56f, .56f, 1.f);
EXPORT(bool, Settings::ESP::Enemies::armor) = true;
EXPORT(ImVec4, Settings::ESP::Enemies::armorColor) = ImVec4(.82f, .82f, .82f, 1.f);
EXPORT(bool, Settings::ESP::Enemies::kit) = false;
EXPORT(ImVec4, Settings::ESP::Enemies::kitColor) = ImVec4(.18f, .65f, .98f, 1.f);
EXPORT(ImVec4, Settings::ESP::Enemies::kitDefusingColor) = ImVec4(.28f, .45f, .98f, 1.f);
EXPORT(bool, Settings::ESP::Enemies::bomb) = false;
EXPORT(ImVec4, Settings::ESP::Enemies::bombColor) = ImVec4(.18f, .65f, .98f, 1.f);
EXPORT(ImVec4, Settings::ESP::Enemies::bombColorPlanting) = ImVec4(.48f, .65f, .88f, 1.f);
EXPORT(bool, Settings::ESP::Enemies::hostage) = false;
EXPORT(ImVec4, Settings::ESP::Enemies::hostageColor) = ImVec4(.88f, .52f, .06f, 1.f);
EXPORT(ImVec4, Settings::ESP::Enemies::hostageGrabbingColor) = ImVec4(.98f, .52f, .06f, 1.f);
EXPORT(bool, Settings::ESP::Enemies::scoped) = true;
EXPORT(ImVec4, Settings::ESP::Enemies::scopedColor) = ImVec4(.17f, .44f, .76f, 1.f);
EXPORT(bool, Settings::ESP::Enemies::dormant) = false;
EXPORT(ImVec4, Settings::ESP::Enemies::dormantColor) = ImVec4(.76f, .17f, .44f, 1.f);
EXPORT(bool, Settings::ESP::Enemies::ping) = true;
EXPORT(ImVec4, Settings::ESP::Enemies::pingColor) = ImVec4(.17f, .76f, .21f, 1.f);

// Allies
EXPORT(bool, Settings::ESP::Allies::enabled) = true;
EXPORT(Settings::ESP::BoxType, Settings::ESP::Allies::box) = Settings::ESP::BoxType::BOX_2D;
EXPORT(ImVec4, Settings::ESP::Allies::boxColorVisible) = ImVec4(.06f, .76f, .06f, 1.f);
EXPORT(ImVec4, Settings::ESP::Allies::boxColorInvisible) = ImVec4(.06f, .41f, .76f, 1.f);
EXPORT(bool, Settings::ESP::Allies::skeleton) = false;
EXPORT(ImVec4, Settings::ESP::Allies::skeletonColorVisible) = ImVec4(.06f, .76f, .06f, 1.f);
EXPORT(ImVec4, Settings::ESP::Allies::skeletonColorInvisible) = ImVec4(.06f, .41f, .76f, 1.f);
EXPORT(bool, Settings::ESP::Allies::barrel) = false;
EXPORT(f32, Settings::ESP::Allies::barrelLength) = 128.f;
EXPORT(ImVec4, Settings::ESP::Allies::barrelColor) = ImVec4(.40f, .58f, .80f, 1.f);
EXPORT(bool, Settings::ESP::Allies::healthBar) = false;
EXPORT(ImVec4, Settings::ESP::Allies::healthBarColor) = ImVec4(.27f, .71f, .26f, 1.f);
EXPORT(bool, Settings::ESP::Allies::healthBarHealthBasedColor) = false;
EXPORT(bool, Settings::ESP::Allies::ammoBar) = false;
EXPORT(ImVec4, Settings::ESP::Allies::ammoBarColor) = ImVec4(.33f, .53f, .81f, 1.f);
EXPORT(ImVec4, Settings::ESP::Allies::ammoBarReloadingColor) = ImVec4(.73f, .53f, .81f, 1.f);
EXPORT(bool, Settings::ESP::Allies::currentWeapon) = true;
EXPORT(ImVec4, Settings::ESP::Allies::currentWeaponColor) = ImVec4(.60f, .73f, .90f, 1.f);
EXPORT(bool, Settings::ESP::Allies::otherWeapon) = false;
EXPORT(ImVec4, Settings::ESP::Allies::otherWeaponColor) = ImVec4(.60f, .73f, .90f, 1.f);
EXPORT(bool, Settings::ESP::Allies::grenades) = false;
EXPORT(ImVec4, Settings::ESP::Allies::grenadesColor) = ImVec4(.60f, .73f, .90f, 1.f);
EXPORT(bool, Settings::ESP::Allies::name) = true;
EXPORT(ImVec4, Settings::ESP::Allies::nameColor) = ImVec4(.06f, .76f, .06f, 1.f);
EXPORT(bool, Settings::ESP::Allies::clan) = false;
EXPORT(ImVec4, Settings::ESP::Allies::clanColor) = ImVec4(.54f, .87f, .53f, 1.f);
EXPORT(bool, Settings::ESP::Allies::hp) = true;
EXPORT(ImVec4, Settings::ESP::Allies::hpColor) = ImVec4(.94f, .56f, .56f, 1.f);
EXPORT(bool, Settings::ESP::Allies::armor) = true;
EXPORT(ImVec4, Settings::ESP::Allies::armorColor) = ImVec4(.82f, .82f, .82f, 1.f);
EXPORT(bool, Settings::ESP::Allies::kit) = false;
EXPORT(ImVec4, Settings::ESP::Allies::kitColor) = ImVec4(.18f, .65f, .98f, 1.f);
EXPORT(ImVec4, Settings::ESP::Allies::kitDefusingColor) = ImVec4(.28f, .45f, .98f, 1.f);
EXPORT(bool, Settings::ESP::Allies::bomb) = false;
EXPORT(ImVec4, Settings::ESP::Allies::bombColor) = ImVec4(.18f, .65f, .98f, 1.f);
EXPORT(ImVec4, Settings::ESP::Allies::bombColorPlanting) = ImVec4(.48f, .65f, .88f, 1.f);
EXPORT(bool, Settings::ESP::Allies::hostage) = false;
EXPORT(ImVec4, Settings::ESP::Allies::hostageColor) = ImVec4(.88f, .52f, .06f, 1.f);
EXPORT(ImVec4, Settings::ESP::Allies::hostageGrabbingColor) = ImVec4(.98f, .52f, .06f, 1.f);
EXPORT(bool, Settings::ESP::Allies::scoped) = true;
EXPORT(ImVec4, Settings::ESP::Allies::scopedColor) = ImVec4(.17f, .44f, .76f, 1.f);
EXPORT(bool, Settings::ESP::Allies::dormant) = false;
EXPORT(ImVec4, Settings::ESP::Allies::dormantColor) = ImVec4(.76f, .17f, .44f, 1.f);
EXPORT(bool, Settings::ESP::Allies::ping) = true;
EXPORT(ImVec4, Settings::ESP::Allies::pingColor) = ImVec4(.17f, .76f, .21f, 1.f);
// }}}

static Mat4x4 vMatrix;
static Vec2 screenResFactor;

inline ImColor transparent(ImColor base, f32 alpha)
{
	return ImColor(base.Value.x, base.Value.y, base.Value.z, alpha);
}

bool ESP::WorldToScreen(const Vec3& origin, Vec2& screen)
{
	f32 w = vMatrix[3][0] * origin.x
		  + vMatrix[3][1] * origin.y
		  + vMatrix[3][2] * origin.z
		  + vMatrix[3][3];

	if ( w < 0.01f ) // Is Not in front of our player
		return false;

	f32 width = (f32)Paint::engineWidth;
	f32 height = (f32)Paint::engineHeight;

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
	screen *= screenResFactor;

	return true;
}

bool WorldToScreen2(const Vec3& origin, Vec2& screen)
{
	f32 w = vMatrix[3][0] * origin.x
		  + vMatrix[3][1] * origin.y
		  + vMatrix[3][2] * origin.z
		  + vMatrix[3][3];

	f32 width = (f32)Paint::engineWidth;
	f32 height = (f32)Paint::engineHeight;

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
	screen *= screenResFactor;

	return true;
}

f32 GetFontSize(C_BaseEntity* ent, C_BasePlayer* lp)
{
	//const f32 dist = (ent->GetVecOrigin() - lp->GetVecOrigin()).Length();
	// TODO: this scaling looks very bad, maybe select accurate font in draw::imtext
	
	return 18;
}

bool GetBox(C_BaseEntity* ent, Rect2& box)
{
	// Variables
	Vec3 flb, brt, blb, frt, frb, brb, blt, flt; // think of these as Front-Left-Bottom/Front-Left-Top... Etc.
	f32 left, top, right, bottom;

	// Get the locations
	const Vec3 min = ent->GetCollideable()->OBBMins() + ent->GetVecOrigin();
	const Vec3 max = ent->GetCollideable()->OBBMaxs() + ent->GetVecOrigin();

	// Points of a 3d bounding box
	std::array<Vec3, 8> points = {
		Vec3(min.x, min.y, min.z),
		Vec3(min.x, max.y, min.z),
		Vec3(max.x, max.y, min.z),
		Vec3(max.x, min.y, min.z),
		Vec3(max.x, max.y, max.z),
		Vec3(min.x, max.y, max.z),
		Vec3(min.x, min.y, max.z),
		Vec3(max.x, min.y, max.z)
	};

	// Get screen positions
	if (debugOverlay->ScreenPosition(points[3], flb) || debugOverlay->ScreenPosition(points[5], brt) ||
		debugOverlay->ScreenPosition(points[0], blb) || debugOverlay->ScreenPosition(points[4], frt) ||
		debugOverlay->ScreenPosition(points[2], frb) || debugOverlay->ScreenPosition(points[1], brb) ||
		debugOverlay->ScreenPosition(points[6], blt) || debugOverlay->ScreenPosition(points[7], flt))
		return false;

	// Put them in an array (maybe start them off in one later for speed?)
	std::array<Vec3, 8> arr{flb, brt, blb, frt, frb, brb, blt, flt};

	// Init
	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	// Find the bounding corners for our box
	for (std::size_t i = 1; i < 8; ++i)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	// Width / height
	box = Rect2{Vec2{left, top}, Vec2{right-left, bottom-top}};

	return true;
}

void DrawBox(Rect2 box, C_BaseEntity* ent, ImColor color, Settings::ESP::BoxType type)
{
	if (type == Settings::ESP::BoxType::BOX_2D)
	{
		Draw::AddRect(
			Rect2i(
				Vec2i{(i32)box.x.x, (i32)box.x.y},
				Vec2i{(i32)(box.y.x+box.x.x), (i32)(box.y.y+box.x.y)}
			),
			ImColor(0.f, 0.f, 0.f, 0.5f*color.Value.w),
			3.f,
			2.f,
			ImDrawCornerFlags_All);
		Draw::AddRect(
			Rect2i(
				Vec2i{(i32)box.x.x, (i32)box.x.y},
				Vec2i{(i32)(box.y.x+box.x.x), (i32)(box.y.y+box.x.y)}
			),
			color,
			1.f,
			2.f,
			ImDrawCornerFlags_All);
	}
	else if (type == Settings::ESP::BoxType::BOX_3D)
	{
		const Vec3 min = ent->GetCollideable()->OBBMins() + ent->GetVecOrigin();
		const Vec3 max = ent->GetCollideable()->OBBMaxs() + ent->GetVecOrigin();

		std::array<Vec3, 8> points
		{
			Vec3(min.x, min.y, min.z),
			Vec3(min.x, max.y, min.z),
			Vec3(max.x, max.y, min.z),
			Vec3(max.x, min.y, min.z),
			Vec3(min.x, min.y, max.z),
			Vec3(min.x, max.y, max.z),
			Vec3(max.x, max.y, max.z),
			Vec3(max.x, min.y, max.z)
		};
		static constexpr std::array<std::array<i32, 2>, 12> edges
		{
			std::array<i32, 2>{0, 1}, std::array<i32, 2>{1, 2}, std::array<i32, 2>{2, 3}, std::array<i32, 2>{3, 0},
			std::array<i32, 2>{4, 5}, std::array<i32, 2>{5, 6}, std::array<i32, 2>{6, 7}, std::array<i32, 2>{7, 4},
			std::array<i32, 2>{0, 4}, std::array<i32, 2>{1, 5}, std::array<i32, 2>{2, 6}, std::array<i32, 2>{3, 7}
		};


		for (const auto& edge : edges)
		{
			Vec3 p1, p2;
			if (debugOverlay->ScreenPosition(points[edge[0]], p1) ||
				debugOverlay->ScreenPosition(points[edge[1]], p2))
				return;
			Draw::AddLine(
				Rect2i(
					Vec2i{(i32)p1.x, (i32)p1.y},
					Vec2i{(i32)p2.x, (i32)p2.y}
				),
				ImColor(0.f, 0.f, 0.f, 0.5f*color.Value.w),
				3.f);
			Draw::AddLine(
				Rect2i(
					Vec2i{(i32)p1.x, (i32)p1.y},
					Vec2i{(i32)p2.x, (i32)p2.y}
				),
				color);
		}
	}
}

void DrawSkeleton(C_BasePlayer* player, ImColor color)
{
	StudioHdr* studioModel = modelInfo->GetStudioModel(player->GetModel());
	if (!studioModel)
		return;

	static std::array<Mat3x4, 128> boneToWorld;
	if (!player->SetupBones(boneToWorld.data(), 128, BoneMask::HITBOX, 0))
		return;

	for (i32 i = 0; i < studioModel->numBones; i++)
	{
		MStudioBone* bone = studioModel->Bone(i);
		if (!bone || !(bone->flags & BoneMask::HITBOX) || bone->parent == -1)
			continue;

		Vec3 bonePos1;
		if (debugOverlay->ScreenPosition(Vec3{boneToWorld[i][0][3], boneToWorld[i][1][3], boneToWorld[i][2][3]}, bonePos1))
			continue;

		Vec3 bonePos2;
		if (debugOverlay->ScreenPosition(Vec3{boneToWorld[bone->parent][0][3], boneToWorld[bone->parent][1][3], boneToWorld[bone->parent][2][3]}, bonePos2))
			continue;

		Draw::AddLine(Rect2i{Vec2i{bonePos1.x, bonePos1.y}, Vec2i{bonePos2.x, bonePos2.y}}, ImColor(0.f, 0.f, 0.f, 0.5f*color.Value.w), 3.f);
		Draw::AddLine(Rect2i{Vec2i{bonePos1.x, bonePos1.y}, Vec2i{bonePos2.x, bonePos2.y}}, color, 1.f);
	}
}

void DrawBarrel(C_BasePlayer* p, ImColor color, f32 length)
{
	Vec2 start;
	const Vec3 head = p->GetBonePosition(Bones::HEAD);
	WorldToScreen2(head, start);
	Vec2 end;
	WorldToScreen2(head + Math::AngleForward(*p->GetEyeAngles())*length, end);

	Draw::AddLine(Rect2i{start.As<i32, 2>(), end.As<i32, 2>()}, ImColor(0.f, 0.f, 0.f, 0.5f*color.Value.w), 3.f);
	Draw::AddLine(Rect2i{start.As<i32, 2>(), end.As<i32, 2>()}, color, 1.f);
}

void DrawHealthBar(const Rect2& box, C_BasePlayer* p, ImColor color, bool healthBased)
{
	f32 f = 1.f - std::max(std::min(p->GetHealth(), 100), 0) / 100.f;

	if (healthBased)
	{
		Draw::AddRectFilled(Rect2i{Vec2i{(i32)box.x.x - 6, (i32)box.x.y}, Vec2i{(i32)box.x.x - 2, (i32)(box.x.y + box.y.y)}}, ImColor(0.f, 0.f, 0.f, 0.5f));
		Draw::AddRectFilled(Rect2i{Vec2i{(i32)box.x.x - 5, (i32)box.x.y+1 + (i32)(box.y.y*f)}, Vec2i{(i32)box.x.x - 3, (i32)(box.x.y + box.y.y)-1}},
				ImColor(f, 1.f - f, 0.f, 1.f));
	}
	else
	{
		Draw::AddRectFilled(Rect2i{Vec2i{(i32)box.x.x - 6, (i32)box.x.y}, Vec2i{(i32)box.x.x - 2, (i32)(box.x.y + box.y.y)}}, ImColor(0.f, 0.f, 0.f, color.Value.w*.5f));
		Draw::AddRectFilled(Rect2i{Vec2i{(i32)box.x.x - 5, (i32)box.x.y+1 + (i32)(box.y.y*f)}, Vec2i{(i32)box.x.x - 3, (i32)(box.x.y + box.y.y)-1}}, color);
	}
}

bool DrawAmmoBar(const Rect2& box, C_BasePlayer* p, ImColor ammo, ImColor reloading)
{
	bool isReloading = false;
	f32 perc = 0.f;
	auto weapon = PlayerAdditionalInfo::GetWeapon(p);
	if (!weapon)
		return false;

	CUtlVector<AnimationLayer>* layers = p->GetAnimOverlay();
	for (i32 i = 0; i <= layers->Size(); ++i)
	{
		if (p->GetSequenceActivity(layers->operator[](i).sequence) != static_cast<i32>(CCSGOAnimStatePoses::ACT_CSGO_RELOAD) ||
			layers->operator[](i).weight == 0.f)
			continue;

		isReloading = true;
		perc = layers->operator[](i).cycle;
		break;
	}

	if (isReloading)
	{
		Draw::AddRectFilled(Rect2i{Vec2i{box.x.x, box.x.y + box.y.y + 2}, Vec2i{box.x.x + box.y.x, box.x.y + box.y.y + 6}}, ImColor(0.f, 0.f, 0.f, reloading.Value.w*.5f));
		Draw::AddRectFilled(Rect2i{Vec2i{box.x.x + 1, box.x.y + box.y.y + 3}, Vec2i{box.x.x + (box.y.x - 1)*perc, box.x.y + box.y.y + 5}}, reloading);
	}
	else
	{
		const i32 current  = reinterpret_cast<C_BaseCombatWeapon*>(weapon)->GetAmmo();
		const i32 clipSize = reinterpret_cast<C_BaseCombatWeapon*>(weapon)->GetCSWpnData()->GetClipSize();
		Draw::AddRectFilled(Rect2i{Vec2i{box.x.x, box.x.y + box.y.y + 2}, Vec2i{box.x.x + box.y.x, box.x.y + box.y.y + 6}}, ImColor(0.f, 0.f, 0.f, ammo.Value.w*.5f));
		Draw::AddRectFilled(Rect2i{Vec2i{box.x.x + 1, box.x.y + box.y.y + 3}, Vec2i{box.x.x + (box.y.x - 1)*((f32)current / clipSize), box.x.y + box.y.y + 5}}, ammo);
	}

	return true;
}

void DrawEnemy(C_BasePlayer* p, C_BasePlayer* lp)
{
	Rect2 box;
	if (!GetBox(p, box))
		return;

	const f32 fontSize = GetFontSize(p, lp);

	if (PlayerAdditionalInfo::IsVisible(p))
	{
		if (PlayerAdditionalInfo::IsDormant(p))
			DrawBox(box, p, transparent(Settings::ESP::Enemies::boxColorVisible, std::pow(1.f-(f32)PlayerAdditionalInfo::DormantTime(p)/PlayerAdditionalInfo::GetMaxDormantTime(), 2)), Settings::ESP::Enemies::box);
		else
			DrawBox(box, p, Settings::ESP::Enemies::boxColorVisible, Settings::ESP::Enemies::box);
		if (Settings::ESP::Enemies::skeleton)
			DrawSkeleton(p, Settings::ESP::Enemies::skeletonColorVisible);
		if (Settings::ESP::Enemies::barrel)
			DrawBarrel(p, Settings::ESP::Enemies::barrelColor, Settings::ESP::Enemies::barrelLength);
	}
	else
	{
		if (PlayerAdditionalInfo::IsDormant(p))
			DrawBox(box, p, transparent(Settings::ESP::Enemies::boxColorInvisible, std::pow(1.f-(f32)PlayerAdditionalInfo::DormantTime(p)/PlayerAdditionalInfo::GetMaxDormantTime(), 2)), Settings::ESP::Enemies::box);
		else
			DrawBox(box, p, Settings::ESP::Enemies::boxColorInvisible, Settings::ESP::Enemies::box);
		if (Settings::ESP::Enemies::skeleton)
			DrawSkeleton(p, Settings::ESP::Enemies::skeletonColorInvisible);
		if (Settings::ESP::Enemies::barrel)
			DrawBarrel(p, Settings::ESP::Enemies::barrelColor, Settings::ESP::Enemies::barrelLength);
	}
	if (Settings::ESP::Enemies::healthBar)
		DrawHealthBar(box, p, Settings::ESP::Enemies::healthBarColor, Settings::ESP::Enemies::healthBarHealthBasedColor);
	
	//{{{ Top
	// Informations
	f32 top = box.x.y;
	if (Settings::ESP::Enemies::clan)
	{
		std::string clan((*csPlayerResource)->GetClan(p->GetIndex()));
		if (!clan.empty())
		{
			top -= fontSize + 2;
			const Vec2 clanSz = UI::GetTextSize(clan, UI::espfont, fontSize);

			const i32 x = (i32)box.x.x + ((i32)(box.y.x - clanSz.x) >> 1);
			Draw::AddRectFilled(Rect2i{Vec2i{x - 4, top}, Vec2i{x + clanSz.x + 4, top + clanSz.y}}, ImColor(0.f, 0.f, 0.f, .6f*Settings::ESP::Enemies::clanColor.w));
			Draw::AddRectFilled(Rect2i{Vec2i{x - 4, top}, Vec2i{x + clanSz.x + 4, top + 3}}, Settings::ESP::Enemies::clanColor);
			Draw::AddText(Vec2i{x, top + 2}, std::move(clan), Settings::ESP::Enemies::clanColor, TextFlags::Shadow, fontSize);
		}
	}
	if (Settings::ESP::Enemies::name)
	{
		top -= fontSize + 2;
		std::string name(PlayerAdditionalInfo::GetName(p).data());
		const Vec2 nameSz = UI::GetTextSize(name, UI::espfont, fontSize);

		const i32 x = (i32)box.x.x + ((i32)(box.y.x - nameSz.x) >> 1);
		Draw::AddRectFilled(Rect2i{Vec2i{x - 4, top}, Vec2i{x + nameSz.x + 4, top + nameSz.y}}, ImColor(0.f, 0.f, 0.f, .6f*Settings::ESP::Enemies::nameColor.w));
		Draw::AddRectFilled(Rect2i{Vec2i{x - 4, top}, Vec2i{x + nameSz.x + 4, top + 3}}, Settings::ESP::Enemies::nameColor);
		Draw::AddText(Vec2i{x, top + 2}, std::move(name), Settings::ESP::Enemies::nameColor, TextFlags::Shadow, fontSize);
	}
	//}}}
	//{{{ Bottom
	{
		i32 bottom = box.x.y + box.y.y + 2;
		//{{{Ammo bar
		if (Settings::ESP::Enemies::ammoBar)
		{
			if (DrawAmmoBar(box, p, Settings::ESP::Enemies::ammoBarColor, Settings::ESP::Enemies::ammoBarReloadingColor))
				bottom += 5;
		}
		//}}}
		//{{{ Current weapon
		if (Settings::ESP::Enemies::currentWeapon)
		{
			C_BaseCombatWeapon* weapon = PlayerAdditionalInfo::GetWeapon(p);
			if (weapon)
			{
				std::string name = codepointToUtf8(0xE000 + static_cast<i16>(*weapon->GetItemDefinitionIndex()));

				const Vec2 nameSz = UI::GetTextSize(name, UI::espfont, fontSize);
				const i32 x = (i32)box.x.x + ((i32)(box.y.x - nameSz.x) >> 1);
				Draw::AddText(Vec2i{x, bottom}, std::move(name), Settings::ESP::Enemies::currentWeaponColor, TextFlags::Shadow, fontSize);

				bottom += fontSize;
			}
		}
		//}}}
		//{{{ Other weapons
		if (Settings::ESP::Enemies::otherWeapon)
		{
			const CBaseHandle activeWeapon = *p->GetActiveWeapon();
			const auto& weapons = p->GetWeapons();

			std::string name;
			bool first = true;
			for (std::size_t i = 0; i < C_BasePlayer::MaxWeapons; ++i)
			{
				if (weapons[i].index == -1)
					break;
				if (weapons[i].index == activeWeapon.index)
					continue;
				C_BaseCombatWeapon* weapon = reinterpret_cast<C_BaseCombatWeapon*>(entityList->GetClientEntity(weapons[i].index & 0xFFF));
				if (weapon == nullptr || !Util::IsOtherWeapon(weapon->GetCSWpnData()->GetWeaponType()))
					continue;
				if (first)
					first = false, name += " " + codepointToUtf8(0xE000 + static_cast<i16>(*weapon->GetItemDefinitionIndex()));
				else
					name += codepointToUtf8(0xE000 + static_cast<i16>(*weapon->GetItemDefinitionIndex()));
			}


			const Vec2 nameSz = UI::GetTextSize(name, UI::espfont, fontSize);
			const i32 x = (i32)box.x.x + ((i32)(box.y.x - nameSz.x) >> 1);
			Draw::AddText(Vec2i{x, bottom}, std::move(name), Settings::ESP::Enemies::otherWeaponColor, TextFlags::Shadow, fontSize);
			
			bottom += fontSize;
		}//}}}
		//{{{ Grenades
		if (Settings::ESP::Enemies::grenades)
		{
			const CBaseHandle activeWeapon = *p->GetActiveWeapon();
			const auto& weapons = p->GetWeapons();

			std::string name;
			bool first = true;
			for (std::size_t i = 0; i < C_BasePlayer::MaxWeapons; ++i)
			{
				if (weapons[i].index == -1)
					break;
				if (weapons[i].index == activeWeapon.index)
					continue;
				C_BaseCombatWeapon* weapon = reinterpret_cast<C_BaseCombatWeapon*>(entityList->GetClientEntity( weapons[i].index & 0xFFF ));
				if (weapon == nullptr || !Util::IsGrenade(weapon->GetCSWpnData()->GetWeaponType()))
					continue;
				if (first)
					first = false, name += " " + codepointToUtf8(0xE000 + static_cast<i16>(*weapon->GetItemDefinitionIndex()));
				else
					name += codepointToUtf8(0xE000 + static_cast<i16>(*weapon->GetItemDefinitionIndex()));
			}


			const Vec2 nameSz = UI::GetTextSize(name, UI::espfont, fontSize);
			const i32 x = (i32)box.x.x + ((i32)(box.y.x - nameSz.x) >> 1);
			Draw::AddText(Vec2i{x, bottom}, std::move(name), Settings::ESP::Enemies::grenadesColor, TextFlags::Shadow, fontSize);
			
			bottom += fontSize;
		}//}}}
	}
	//}}}
	// {{{ Right
	{
		Vec2 right{box.x.x + box.y.x + 2, box.x.y};
		if (Settings::ESP::Enemies::kit)
		{
			if (p->HasDefuser())
			{
				if (p->IsDefusing())
					Draw::AddText(Vec2i{right.x, right.y}, "\ue066 Defusing!"s, Settings::ESP::Enemies::kitDefusingColor, TextFlags::Shadow, fontSize);
				else
					Draw::AddText(Vec2i{right.x, right.y}, "\ue066"s, Settings::ESP::Enemies::kitColor, TextFlags::Shadow, fontSize);
				right.y += fontSize;
			}
			else if (p->IsDefusing())
			{
				Draw::AddText(Vec2i{right.x, right.y}, "Defusing!"s, Settings::ESP::Enemies::kitDefusingColor, TextFlags::Shadow, fontSize);
				right.y += fontSize;
			}
		}
		if (Settings::ESP::Enemies::armor)
		{
			if (p->HasHelmet())
			{
				Draw::AddText(Vec2i{right.x, right.y}, fmt::format("\ue065 {}"s, p->GetArmor()), Settings::ESP::Enemies::armorColor, TextFlags::Shadow, fontSize);
				right.y += fontSize;
			}
			else if (p->GetArmor() > 0)
			{
				Draw::AddText(Vec2i{right.x, right.y}, fmt::format("\ue064 {}"s, p->GetArmor()), Settings::ESP::Enemies::armorColor, TextFlags::Shadow, fontSize);
				right.y += fontSize;
			}
		}
		if (Settings::ESP::Enemies::bomb && p->GetIndex() == (*csPlayerResource)->GetPlayerC4())
		{
			if (Util::IsPlanting(p))
				Draw::AddText(Vec2i{right.x, right.y}, "\ue031 Planting!"s, Settings::ESP::Enemies::bombColorPlanting, TextFlags::Shadow, fontSize);
			else
				Draw::AddText(Vec2i{right.x, right.y}, "\ue031"s, Settings::ESP::Enemies::bombColor, TextFlags::Shadow, fontSize);
			right.y += fontSize;
		}
		if (Settings::ESP::Enemies::dormant && PlayerAdditionalInfo::IsDormant(p))
		{
			Draw::AddText(Vec2i{right.x, right.y}, fmt::format("\ue067 {}ms"s, PlayerAdditionalInfo::DormantTime(p)), Settings::ESP::Enemies::dormantColor, TextFlags::Shadow, fontSize);
			right.y += fontSize;
		}
		if (Settings::ESP::Enemies::hostage && p->IsGrabbingHostage())
		{
			Draw::AddText(Vec2i{right.x, right.y}, "\ue069 Grabbing!"s, Settings::ESP::Enemies::hostageGrabbingColor, TextFlags::Shadow, fontSize);
			right.y += fontSize;
		}
		else if (Settings::ESP::Enemies::hostage && p->IsRescuing())
		{
			Draw::AddText(Vec2i{right.x, right.y}, "\ue069"s, Settings::ESP::Enemies::hostageColor, TextFlags::Shadow, fontSize);
			right.y += fontSize;
		}
		if (Settings::ESP::Enemies::scoped && p->IsScoped())
		{
			Draw::AddText(Vec2i{right.x, right.y}, "\ue06A"s, Settings::ESP::Enemies::scopedColor, TextFlags::Shadow, fontSize);
			right.y += fontSize;
		}
		if (Settings::ESP::Enemies::ping)
		{
			Draw::AddText(Vec2i{right.x, right.y}, fmt::format("\ue06C {}ms"s, (*csPlayerResource)->GetPing(p->GetIndex())), Settings::ESP::Enemies::pingColor, TextFlags::Shadow, fontSize);
			right.y += fontSize;
		}
	}
	// }}}
}

void DrawAlly(C_BasePlayer* p, C_BasePlayer* lp)
{
	Rect2 box;
	if (!GetBox(p, box))
		return;

	const f32 fontSize = GetFontSize(p, lp);

	if (PlayerAdditionalInfo::IsVisible(p))
	{
		if (PlayerAdditionalInfo::IsDormant(p))
			DrawBox(box, p, transparent(Settings::ESP::Allies::boxColorVisible, 1.f-(f32)PlayerAdditionalInfo::DormantTime(p)/PlayerAdditionalInfo::GetMaxDormantTime()), Settings::ESP::Allies::box);
		else
			DrawBox(box, p, Settings::ESP::Allies::boxColorVisible, Settings::ESP::Allies::box);
		if (Settings::ESP::Allies::skeleton)
			DrawSkeleton(p, Settings::ESP::Allies::skeletonColorVisible);
		if (Settings::ESP::Allies::barrel)
			DrawBarrel(p, Settings::ESP::Allies::barrelColor, Settings::ESP::Allies::barrelLength);
	}
	else
	{
		if (PlayerAdditionalInfo::IsDormant(p))
			DrawBox(box, p, transparent(Settings::ESP::Allies::boxColorInvisible, 1.f-(f32)PlayerAdditionalInfo::DormantTime(p)/PlayerAdditionalInfo::GetMaxDormantTime()), Settings::ESP::Allies::box);
		else
			DrawBox(box, p, Settings::ESP::Allies::boxColorInvisible, Settings::ESP::Allies::box);
		if (Settings::ESP::Allies::skeleton)
			DrawSkeleton(p, Settings::ESP::Allies::skeletonColorInvisible);
		if (Settings::ESP::Allies::barrel)
			DrawBarrel(p, Settings::ESP::Allies::barrelColor, Settings::ESP::Allies::barrelLength);
	}
	if (Settings::ESP::Allies::healthBar)
		DrawHealthBar(box, p, Settings::ESP::Allies::healthBarColor, Settings::ESP::Allies::healthBarHealthBasedColor);
	
	//{{{ Top
	// Informations
	f32 top = box.x.y;
	if (Settings::ESP::Allies::clan)
	{
		std::string clan((*csPlayerResource)->GetClan(p->GetIndex()));
		if (!clan.empty())
		{
			top -= fontSize + 2;
			const Vec2 clanSz = UI::GetTextSize(clan, UI::espfont, fontSize);

			const i32 x = (i32)box.x.x + ((i32)(box.y.x - clanSz.x) >> 1);
			Draw::AddRectFilled(Rect2i{Vec2i{x - 4, top}, Vec2i{x + clanSz.x + 4, top + clanSz.y}}, ImColor(0.f, 0.f, 0.f, .6f*Settings::ESP::Allies::clanColor.w));
			Draw::AddRectFilled(Rect2i{Vec2i{x - 4, top}, Vec2i{x + clanSz.x + 4, top + 3}}, Settings::ESP::Allies::clanColor);
			Draw::AddText(Vec2i{x, top + 2}, std::move(clan), Settings::ESP::Allies::clanColor, TextFlags::Shadow, fontSize);
		}
	}
	if (Settings::ESP::Allies::name)
	{
		top -= fontSize + 2;
		std::string name(PlayerAdditionalInfo::GetName(p).data());
		const Vec2 nameSz = UI::GetTextSize(name, UI::espfont, fontSize);

		const i32 x = (i32)box.x.x + ((i32)(box.y.x - nameSz.x) >> 1);
		Draw::AddRectFilled(Rect2i{Vec2i{x - 4, top}, Vec2i{x + nameSz.x + 4, top + nameSz.y}}, ImColor(0.f, 0.f, 0.f, .6f*Settings::ESP::Allies::nameColor.w));
		Draw::AddRectFilled(Rect2i{Vec2i{x - 4, top}, Vec2i{x + nameSz.x + 4, top + 3}}, Settings::ESP::Allies::nameColor);
		Draw::AddText(Vec2i{x, top + 2}, std::move(name), Settings::ESP::Allies::nameColor, TextFlags::Shadow, fontSize);
	}
	//}}}
	//{{{ Bottom
	{
		i32 bottom = box.x.y + box.y.y + 2;
		//{{{Ammo bar
		if (Settings::ESP::Allies::ammoBar)
		{
			if (DrawAmmoBar(box, p, Settings::ESP::Allies::ammoBarColor, Settings::ESP::Allies::ammoBarReloadingColor))
				bottom += 5;
		}
		//}}}
		//{{{ Current weapon
		if (Settings::ESP::Allies::currentWeapon)
		{
			C_BaseCombatWeapon* weapon = PlayerAdditionalInfo::GetWeapon(p);
			if (weapon)
			{
				std::string name = codepointToUtf8(0xE000 + static_cast<i16>(*weapon->GetItemDefinitionIndex()));

				const Vec2 nameSz = UI::GetTextSize(name, UI::espfont, fontSize);
				const i32 x = (i32)box.x.x + ((i32)(box.y.x - nameSz.x) >> 1);
				Draw::AddText(Vec2i{x, bottom}, std::move(name), Settings::ESP::Allies::currentWeaponColor, TextFlags::Shadow, fontSize);

				bottom += fontSize;
			}
		}
		//}}}
		//{{{ Other weapons
		if (Settings::ESP::Allies::otherWeapon)
		{
			const CBaseHandle activeWeapon = *p->GetActiveWeapon();
			const auto& weapons = p->GetWeapons();

			std::string name;
			bool first = true;
			for (std::size_t i = 0; i < C_BasePlayer::MaxWeapons; ++i)
			{
				if (weapons[i].index == -1)
					break;
				if (weapons[i].index == activeWeapon.index)
					continue;
				C_BaseCombatWeapon* weapon = reinterpret_cast<C_BaseCombatWeapon*>(entityList->GetClientEntity(weapons[i].index & 0xFFF));
				if (weapon == nullptr || !Util::IsOtherWeapon(weapon->GetCSWpnData()->GetWeaponType()))
					continue;
				if (first)
					first = false, name += " " + codepointToUtf8(0xE000 + static_cast<i16>(*weapon->GetItemDefinitionIndex()));
				else
					name += codepointToUtf8(0xE000 + static_cast<i16>(*weapon->GetItemDefinitionIndex()));
			}


			const Vec2 nameSz = UI::GetTextSize(name, UI::espfont, fontSize);
			const i32 x = (i32)box.x.x + ((i32)(box.y.x - nameSz.x) >> 1);
			Draw::AddText(Vec2i{x, bottom}, std::move(name), Settings::ESP::Allies::otherWeaponColor, TextFlags::Shadow, fontSize);
			
			bottom += fontSize;
		}//}}}
		//{{{ Grenades
		if (Settings::ESP::Allies::grenades)
		{
			const CBaseHandle activeWeapon = *p->GetActiveWeapon();
			const auto& weapons = p->GetWeapons();

			std::string name;
			bool first = true;
			for (std::size_t i = 0; i < C_BasePlayer::MaxWeapons; ++i)
			{
				if (weapons[i].index == -1)
					break;
				if (weapons[i].index == activeWeapon.index)
					continue;
				C_BaseCombatWeapon* weapon = reinterpret_cast<C_BaseCombatWeapon*>(entityList->GetClientEntity( weapons[i].index & 0xFFF ));
				if (weapon == nullptr || !Util::IsGrenade(weapon->GetCSWpnData()->GetWeaponType()))
					continue;
				if (first)
					first = false, name += " " + codepointToUtf8(0xE000 + static_cast<i16>(*weapon->GetItemDefinitionIndex()));
				else
					name += codepointToUtf8(0xE000 + static_cast<i16>(*weapon->GetItemDefinitionIndex()));
			}


			const Vec2 nameSz = UI::GetTextSize(name, UI::espfont, fontSize);
			const i32 x = (i32)box.x.x + ((i32)(box.y.x - nameSz.x) >> 1);
			Draw::AddText(Vec2i{x, bottom}, std::move(name), Settings::ESP::Allies::grenadesColor, TextFlags::Shadow, fontSize);
			
			bottom += fontSize;
		}//}}}
	}
	//}}}
	// {{{ Right
	{
		Vec2 right{box.x.x + box.y.x + 2, box.x.y};
		if (Settings::ESP::Allies::kit)
		{
			if (p->HasDefuser())
			{
				if (p->IsDefusing())
					Draw::AddText(Vec2i{right.x, right.y}, "\ue066 Defusing!"s, Settings::ESP::Allies::kitDefusingColor, TextFlags::Shadow, fontSize);
				else
					Draw::AddText(Vec2i{right.x, right.y}, "\ue066"s, Settings::ESP::Allies::kitColor, TextFlags::Shadow, fontSize);
				right.y += fontSize;
			}
			else if (p->IsDefusing())
			{
				Draw::AddText(Vec2i{right.x, right.y}, "Defusing!"s, Settings::ESP::Allies::kitDefusingColor, TextFlags::Shadow, fontSize);
				right.y += fontSize;
			}
		}
		if (Settings::ESP::Allies::armor)
		{
			if (p->HasHelmet())
			{
				Draw::AddText(Vec2i{right.x, right.y}, fmt::format("\ue065 {}"s, p->GetArmor()), Settings::ESP::Allies::armorColor, TextFlags::Shadow, fontSize);
				right.y += fontSize;
			}
			else if (p->GetArmor() > 0)
			{
				Draw::AddText(Vec2i{right.x, right.y}, fmt::format("\ue064 {}"s, p->GetArmor()), Settings::ESP::Allies::armorColor, TextFlags::Shadow, fontSize);
				right.y += fontSize;
			}
		}
		if (Settings::ESP::Allies::bomb && p->GetIndex() == (*csPlayerResource)->GetPlayerC4())
		{
			if (Util::IsPlanting(p))
				Draw::AddText(Vec2i{right.x, right.y}, "\ue031 Planting!"s, Settings::ESP::Allies::bombColorPlanting, TextFlags::Shadow, fontSize);
			else
				Draw::AddText(Vec2i{right.x, right.y}, "\ue031"s, Settings::ESP::Allies::bombColor, TextFlags::Shadow, fontSize);
			right.y += fontSize;
		}
		if (Settings::ESP::Allies::dormant && PlayerAdditionalInfo::IsDormant(p))
		{
			Draw::AddText(Vec2i{right.x, right.y}, fmt::format("\ue067 {}ms"s, PlayerAdditionalInfo::DormantTime(p)), Settings::ESP::Allies::dormantColor, TextFlags::Shadow, fontSize);
			right.y += fontSize;
		}
		if (Settings::ESP::Allies::hostage && p->IsGrabbingHostage())
		{
			Draw::AddText(Vec2i{right.x, right.y}, "\ue069 Grabbing!"s, Settings::ESP::Allies::hostageGrabbingColor, TextFlags::Shadow, fontSize);
			right.y += fontSize;
		}
		else if (Settings::ESP::Allies::hostage && p->IsRescuing())
		{
			Draw::AddText(Vec2i{right.x, right.y}, "\ue069"s, Settings::ESP::Allies::hostageColor, TextFlags::Shadow, fontSize);
			right.y += fontSize;
		}
		if (Settings::ESP::Allies::scoped && p->IsScoped())
		{
			Draw::AddText(Vec2i{right.x, right.y}, "\ue06A"s, Settings::ESP::Allies::scopedColor, TextFlags::Shadow, fontSize);
			right.y += fontSize;
		}
		if (Settings::ESP::Allies::ping)
		{
			Draw::AddText(Vec2i{right.x, right.y}, fmt::format("\ue06C {}ms"s, (*csPlayerResource)->GetPing(p->GetIndex())), Settings::ESP::Allies::pingColor, TextFlags::Shadow, fontSize);
			right.y += fontSize;
		}
	}
	// }}}
}

void ESP::Paint()
{
	if(!engine->IsInGame())
		return;
	vMatrix = engine->WorldToScreenMatrix();
	screenResFactor = Vec2((f32)Paint::windowWidth / Paint::engineWidth, (f32)Paint::windowHeight / Paint::engineHeight);

	if (!Settings::Visuals::enabled)
		return;

	auto lp = reinterpret_cast<C_BasePlayer*>(entityList->GetClientEntity(engine->GetLocalPlayer()));
	if (!lp)
		return;

	for (i32 i = 1; i < entityList->GetHighestEntityIndex(); ++i)
	{
		auto ent = entityList->GetClientEntity(i);
		if (!ent)
			continue;

		auto client = ent->GetClientClass();
		if (client->classID == EClassIds::CCSPlayer)
		{
			auto p = reinterpret_cast<C_BasePlayer*>(ent);
			if (!p->GetAlive() || !PlayerAdditionalInfo::ShouldDraw(p))
				continue;

			if (Settings::ESP::Enemies::enabled && p->GetTeam() != lp->GetTeam())
				DrawEnemy(p, lp);
			else if (Settings::ESP::Allies::enabled && p->GetTeam() == lp->GetTeam() && p != lp)
				DrawAlly(p, lp);
			//else if (p == lp && Settings::ESP::Localplayer::enabled)
				//DrawLocal(p);
		}
	}
}
