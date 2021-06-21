#include "Tabs.hpp"
#include "../Widgets.hpp"
#include "../../Hacks/Visuals.hpp"

static void VisualsSettings()
{
	Child("##VISUALSSETTINGS", 7.7);
	{
		UI::Section("Settings");
		
		UI::Checkbox("Enabled", &Settings::Visuals::enabled);
		UI::Desc("Enables visuals globally");
	}
	EndChild();
}

MAKE_CENUM_Q(ESPEntity, i32,
		ENEMY, 0,
		ALLY, 1,
		LOCALPLAYER, 2,
		WEAPON, 3,
		PROJECTILE, 4,
		BOMB, 5,
		CHICKEN, 6,
		FISH, 7);
static ESPEntity ESPEnt = 0;

static void ESPLeft()
{
	const static std::array<std::string, Settings::ESP::BoxType::size> BoxTypes
	{
		"None", "2D Box", "3D Box"
	};

	Child("##ESPBOXES", 17.05f);
	switch(ESPEnt)
	{
		case ESPEntity::ENEMY:
			UI::Checkbox("Enabled##ESP", &Settings::ESP::Enemies::enabled);

			UI::Section("Outlines");
			UI::ListCombo("Box", BoxTypes, *reinterpret_cast<u32*>(&Settings::ESP::Enemies::box));
			UI::ColorEdit(std::make_pair("Visible Boxes Color"s, &Settings::ESP::Enemies::boxColorVisible), std::make_pair("Invisible Boxes Color"s, &Settings::ESP::Enemies::boxColorInvisible));

			UI::Checkbox("Skeleton", &Settings::ESP::Enemies::skeleton);
			UI::ColorEdit(std::make_pair("Visible Skeleton Color"s, &Settings::ESP::Enemies::skeletonColorVisible), std::make_pair("Invisible Skeleton Color"s, &Settings::ESP::Enemies::skeletonColorInvisible));

			UI::Checkbox("Barrel", &Settings::ESP::Enemies::barrel);
			UI::ColorEdit(std::make_pair("Barrel Color"s, &Settings::ESP::Enemies::barrelColor));
			UI::SliderFloat("Barrel length", &Settings::ESP::Enemies::barrelLength, 0.f, 0.f, 1024.f, "%.0fu");

			UI::Section("Bars");

			ImGui::Columns(2, NULL, false);
			{
				UI::Checkbox("Health bar", &Settings::ESP::Enemies::healthBar);
				UI::ColorEdit(std::make_pair("Healt Bar Color"s, &Settings::ESP::Enemies::healthBarColor));

				UI::Checkbox("Ammo bar", &Settings::ESP::Enemies::ammoBar);
				UI::ColorEdit(std::make_pair("Ammo Bar Color"s, &Settings::ESP::Enemies::ammoBarColor));
			}
			ImGui::NextColumn();
			{
				UI::Checkbox("Health-based", &Settings::ESP::Enemies::healthBarHealthBasedColor);
				UI::Desc("The healt bar color will\nadapt to the targer's health");
			}
			ImGui::EndColumns();


			UI::Section("Weapons");
			static std::vector<std::tuple<const char*, const char*, bool&>> weapons
			{
				{"Primary", NULL, Settings::ESP::Enemies::primaryWeapon},
				{"Secondary", NULL, Settings::ESP::Enemies::secondaryWeapon},
				{"Grenades", NULL, Settings::ESP::Enemies::grenades},
			};
			static std::string format;
			UI::CheckboxCombo(" ##WEAPONS", weapons, format, " + ");
			UI::ColorEdit(std::make_pair("Primary Weapon Color"s, &Settings::ESP::Enemies::primaryWeaponColor),
				std::make_pair("Secondary Weapon Color"s, &Settings::ESP::Enemies::secondaryWeaponColor),
				std::make_pair("Grenade Color"s, &Settings::ESP::Enemies::grenadesColor));

			break;
	}
	EndChild();
}

static void ESPRight()
{
	const static std::vector<std::string> ESPEnts
	{
		"Enemies", "Allies", "Localplayer", "Weapons", "Projectiles", "Bomb", "Chickens", "Fishes"
	};

	{
		UI::Button2(ESPEnts[ESPEnt].c_str(), ImVec2(-1, 0));
		if (ImGui::IsItemActive()) // don't wait for release
			ImGui::OpenPopup("##ESPENTS");

		i32 prev = ESPEnt;
		if (UI::PieMenu(ImGui::GetIO().MouseClickedPos[0], "##ESPENTS", ESPEnts, reinterpret_cast<i32*>(&ESPEnt)) == -1)
			ESPEnt = prev;
	}

	ImGui::Dummy(ImVec2(0, 8));
	Child("##ESPLEFT", 15.8f);
	switch(ESPEnt)
	{
		case ESPEntity::ENEMY:
			UI::Section("Informations");
			UI::Checkbox("Name", &Settings::ESP::Enemies::name);
			UI::ColorEdit(std::make_pair("Name Color"s, &Settings::ESP::Enemies::nameColor));
			UI::Checkbox("Clantag", &Settings::ESP::Enemies::clan);
			UI::ColorEdit(std::make_pair("Clantag Color"s, &Settings::ESP::Enemies::clanColor));
			UI::Checkbox("Health", &Settings::ESP::Enemies::hp);
			UI::ColorEdit(std::make_pair("Health Color"s, &Settings::ESP::Enemies::hpColor));
			UI::Checkbox("Defuse kit", &Settings::ESP::Enemies::kit);
			UI::ColorEdit(std::make_pair("Defuse Kit Color"s, &Settings::ESP::Enemies::kitColor));
			UI::Checkbox("Bomb", &Settings::ESP::Enemies::bomb);
			UI::ColorEdit(std::make_pair("Bomb Color"s, &Settings::ESP::Enemies::bombColor));
			UI::Checkbox("Hostage", &Settings::ESP::Enemies::hostage);
			UI::ColorEdit(std::make_pair("Hostage Color"s, &Settings::ESP::Enemies::hostageColor));
			UI::Checkbox("Reloading", &Settings::ESP::Enemies::reloading);
			UI::ColorEdit(std::make_pair("Reloading Color"s, &Settings::ESP::Enemies::reloadingColor));
			UI::Checkbox("Scoped", &Settings::ESP::Enemies::scoped);
			UI::ColorEdit(std::make_pair("Scoped Color"s, &Settings::ESP::Enemies::scopedColor));
			UI::Checkbox("Dormant", &Settings::ESP::Enemies::dormant);
			UI::ColorEdit(std::make_pair("Dormant Color"s, &Settings::ESP::Enemies::dormantColor));
			UI::Checkbox("Ping", &Settings::ESP::Enemies::ping);
			UI::ColorEdit(std::make_pair("Ping Color"s, &Settings::ESP::Enemies::pingColor));
			break;
	}
	EndChild();
}

void Tabs::Visuals()
{
	MakeTab
	(
		make_array
		(
			std::make_pair("General"s, (TabContent)
			{
				{ VisualsSettings, []{} }
			}),
			std::make_pair("ESP"s, (TabContent)
			{
				{ ESPLeft, ESPRight }
			})
		)
	);
}
