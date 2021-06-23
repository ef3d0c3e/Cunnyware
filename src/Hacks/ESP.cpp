#include "Visuals.hpp"
#include "../Config.hpp"
#include "../Util/Draw.hpp"
#include "../Interface.hpp"
#include "../Hooks/Hooks.hpp"
#include "../SDK/C_BaseEntity.hpp"
#include "../SDK/C_BasePlayer.hpp"
#include "../SDK/Recv.hpp"
#include "../SDK/ICollideable.hpp"
#include "Util.hpp"

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
static Vec2 screenResFactor;
bool ESP::WorldToScreen(const Vec3& origin, Vec2& screen)
{
	f32 w = vMatrix[3][0] * origin.x
		  + vMatrix[3][1] * origin.y
		  + vMatrix[3][2] * origin.z
		  + vMatrix[3][3];

	if ( w < 0.001f ) // Is Not in front of our player
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
	screen *= screenResFactor;

	return true;
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
	box = {{left, top}, {right-left, bottom-top}};

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

void DrawEnemy(C_BasePlayer* p)
{
	PlayerInfo info;
	engine->GetPlayerInfo(p->GetIndex(), &info);

	Rect2 box;
	if (!GetBox(p, box))
		return;

	bool visible = Util::IsVisible(p, Bones::HEAD, 180.f, false);

	if (visible)
		DrawBox(box, p, Settings::ESP::Enemies::boxColorVisible, Settings::ESP::Enemies::box);
	else
		DrawBox(box, p, Settings::ESP::Enemies::boxColorInvisible, Settings::ESP::Enemies::box);

	Vec2 pos;
	if (ESP::WorldToScreen(p->GetBonePosition(Bones::HEAD), pos))
	{}
	Draw::AddText(pos.as<i32>(), "TTT"s, ImColor(0.f, 1.f, 0.f, 1.f), 0);

	/*
	Draw::AddRect({box.x, box.x+Vec2i(10,10)}, ImColor(0.f, 1.0f, 1.f, 1.f));
	Draw::AddRect({box.y, box.y+Vec2i(10,10)}, ImColor(1.f, 0.0f, 1.f, 1.f));
	Draw::AddText(box.x, fmt::format("name = {}", info.name.data()), ImColor(1.f, 0.0f, 1.f, 1.f), 0);

	Vec3 vOrigin = p->GetVecOrigin();
	Vec3 min = p->GetCollideable()->OBBMins() + vOrigin;
	Vec3 max = p->GetCollideable()->OBBMaxs() + vOrigin;

	Vec3 points[] = { Vec3( min.x, min.y, min.z ),
						Vec3( min.x, max.y, min.z ),
						Vec3( max.x, max.y, min.z ),
						Vec3( max.x, min.y, min.z ),
						Vec3( min.x, min.y, max.z ),
						Vec3( min.x, max.y, max.z ),
						Vec3( max.x, max.y, max.z ),
						Vec3( max.x, min.y, max.z ) };

	int edges[12][2] = {
			{ 0, 1 },
			{ 1, 2 },
			{ 2, 3 },
			{ 3, 0 },
			{ 4, 5 },
			{ 5, 6 },
			{ 6, 7 },
			{ 7, 4 },
			{ 0, 4 },
			{ 1, 5 },
			{ 2, 6 },
			{ 3, 7 },
	};

	for ( const auto edge : edges )
	{
		Vec2 p1, p2;
		bool v = ESP::WorldToScreen( points[edge[0]], p1 );
		v &= ESP::WorldToScreen( points[edge[1]], p2 );
		if (!v)
			return;
		Draw::AddLine( Rect2i(Vec2i(p1.x, p1.y).as<i32>(), Vec2(p2.x, p2.y).as<i32>()), ImColor(1.f, 1.f, 0.f, 1.f) );
	}
	*/
}

void ESP::Paint()
{
	if(!engine->IsInGame())
		return;
	vMatrix = engine->WorldToScreenMatrix();
	screenResFactor = Vec2(Paint::engineWidth * Paint::windowWidth, Paint::engineHeight * Paint::windowHeight);

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
			if (!p->GetAlive())
				continue;

			if (p->GetTeam() != lp->GetTeam() && Settings::ESP::Enemies::enabled)
				DrawEnemy(p);
			//if (p->GetTeam() == lp->GetTeam())
				//DrawAlly(p);
		}
	}
}
