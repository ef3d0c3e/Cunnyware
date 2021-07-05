#include "Tabs.hpp"
#include "../../ImGUI/TextEditor.h"
#include "../Widgets.hpp"
#include "../../Hacks/Visuals.hpp"
#include "../../Hacks/Info.hpp"
#include "../../Hacks/Util.hpp"

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
static ESPEntity ChamsEnt = 0;

const static std::vector<std::string> ESPEnts
{
	"Enemies", "Allies", "Localplayer", "Weapons", "Projectiles", "Bomb", "Chickens", "Fishes"
};

static void ESPLeft()
{
	const static std::array<std::string, Settings::ESP::BoxType::size> BoxTypes
	{
		"None", "2D Box", "3D Box"
	};

	Child("##ESPLEFT", 17.05f);
	switch(ESPEnt)
	{
		case ESPEntity::ENEMY:
		//{{{ Enemy
		{
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
			}
			ImGui::NextColumn();
			{
				UI::Checkbox("Health-based", &Settings::ESP::Enemies::healthBarHealthBasedColor);
				UI::Desc("The healt bar color will\nadapt to the target's health");
			}
			ImGui::EndColumns();
			UI::Checkbox("Ammo bar", &Settings::ESP::Enemies::ammoBar);
			UI::ColorEdit(std::make_pair("Ammo Bar Reloading Color"s, &Settings::ESP::Enemies::ammoBarReloadingColor), std::make_pair("Ammo Bar Color"s, &Settings::ESP::Enemies::ammoBarColor));


			UI::Section("Weapons");
			static std::vector<std::tuple<const char*, const char*, bool&>> weapons
			{
				{"Current", NULL, Settings::ESP::Enemies::currentWeapon},
				{"Other", NULL, Settings::ESP::Enemies::otherWeapon},
				{"Grenades", NULL, Settings::ESP::Enemies::grenades},
			};
			static std::string format;
			UI::CheckboxCombo(" ##WEAPONS", weapons, format, " + ");
			UI::ColorEdit(std::make_pair("Current Weapon Color"s, &Settings::ESP::Enemies::currentWeaponColor),
				std::make_pair("Other Weapon Color"s, &Settings::ESP::Enemies::otherWeaponColor),
				std::make_pair("Grenade Color"s, &Settings::ESP::Enemies::grenadesColor));
			break;
		}
		//}}}
		case ESPEntity::ALLY:
		//{{{ Ally
		{
			UI::Checkbox("Enabled##ESP", &Settings::ESP::Allies::enabled);

			UI::Section("Outlines");
			UI::ListCombo("Box", BoxTypes, *reinterpret_cast<u32*>(&Settings::ESP::Allies::box));
			UI::ColorEdit(std::make_pair("Visible Boxes Color"s, &Settings::ESP::Allies::boxColorVisible), std::make_pair("Invisible Boxes Color"s, &Settings::ESP::Allies::boxColorInvisible));

			UI::Checkbox("Skeleton", &Settings::ESP::Allies::skeleton);
			UI::ColorEdit(std::make_pair("Visible Skeleton Color"s, &Settings::ESP::Allies::skeletonColorVisible), std::make_pair("Invisible Skeleton Color"s, &Settings::ESP::Allies::skeletonColorInvisible));

			UI::Checkbox("Barrel", &Settings::ESP::Allies::barrel);
			UI::ColorEdit(std::make_pair("Barrel Color"s, &Settings::ESP::Allies::barrelColor));
			UI::SliderFloat("Barrel length", &Settings::ESP::Allies::barrelLength, 0.f, 0.f, 1024.f, "%.0fu");

			UI::Section("Bars");

			ImGui::Columns(2, NULL, false);
			{
				UI::Checkbox("Health bar", &Settings::ESP::Allies::healthBar);
				UI::ColorEdit(std::make_pair("Healt Bar Color"s, &Settings::ESP::Allies::healthBarColor));
			}
			ImGui::NextColumn();
			{
				UI::Checkbox("Health-based", &Settings::ESP::Allies::healthBarHealthBasedColor);
				UI::Desc("The healt bar color will\nadapt to the target's health");
			}
			ImGui::EndColumns();
			UI::Checkbox("Ammo bar", &Settings::ESP::Allies::ammoBar);
			UI::ColorEdit(std::make_pair("Ammo Bar Reloading Color"s, &Settings::ESP::Allies::ammoBarReloadingColor), std::make_pair("Ammo Bar Color"s, &Settings::ESP::Allies::ammoBarColor));


			UI::Section("Weapons");
			static std::vector<std::tuple<const char*, const char*, bool&>> weapons
			{
				{"Current", NULL, Settings::ESP::Allies::currentWeapon},
					{"Other", NULL, Settings::ESP::Allies::otherWeapon},
					{"Grenades", NULL, Settings::ESP::Allies::grenades},
			};
			static std::string format;
			UI::CheckboxCombo(" ##WEAPONS", weapons, format, " + ");
			UI::ColorEdit(std::make_pair("Current Weapon Color"s, &Settings::ESP::Allies::currentWeaponColor),
					std::make_pair("Other Weapon Color"s, &Settings::ESP::Allies::otherWeaponColor),
					std::make_pair("Grenade Color"s, &Settings::ESP::Allies::grenadesColor));
			break;
		}
		//}}}
	}
	EndChild();
}

static void ESPRight()
{
	{
		UI::Button2(ESPEnts[ESPEnt].c_str(), ImVec2(-1, 0));
		if (ImGui::IsItemActive()) // don't wait for release
			ImGui::OpenPopup("##ESPENTS");

		i32 prev = ESPEnt;
		if (UI::PieMenu(ImGui::GetIO().MouseClickedPos[0], "##ESPENTS", ESPEnts, reinterpret_cast<i32*>(&ESPEnt)) == -1)
			ESPEnt = prev;
	}

	ImGui::Dummy(ImVec2(0, 8));
	Child("##ESPRIGHT", 15.8f);
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
			UI::Checkbox("Armor", &Settings::ESP::Enemies::armor);
			UI::ColorEdit(std::make_pair("Armor Color"s, &Settings::ESP::Enemies::armorColor));
			UI::Checkbox("Defuse kit", &Settings::ESP::Enemies::kit);
			UI::ColorEdit(std::make_pair("Defusing Color"s, &Settings::ESP::Enemies::kitDefusingColor), std::make_pair("Defuse Kit Color"s, &Settings::ESP::Enemies::kitColor));
			UI::Checkbox("Bomb", &Settings::ESP::Enemies::bomb);
			UI::ColorEdit(std::make_pair("Planting Color"s, &Settings::ESP::Enemies::bombColorPlanting), std::make_pair("Bomb Color"s, &Settings::ESP::Enemies::bombColor));
			UI::Checkbox("Hostage", &Settings::ESP::Enemies::hostage);
			UI::ColorEdit(std::make_pair("Grabbing Hostage Color"s, &Settings::ESP::Enemies::hostageGrabbingColor), std::make_pair("Hostage Color"s, &Settings::ESP::Enemies::hostageColor));
			UI::Checkbox("Scoped", &Settings::ESP::Enemies::scoped);
			UI::ColorEdit(std::make_pair("Scoped Color"s, &Settings::ESP::Enemies::scopedColor));
			UI::Checkbox("Dormant", &Settings::ESP::Enemies::dormant);
			UI::ColorEdit(std::make_pair("Dormant Color"s, &Settings::ESP::Enemies::dormantColor));
			UI::Checkbox("Ping", &Settings::ESP::Enemies::ping);
			UI::ColorEdit(std::make_pair("Ping Color"s, &Settings::ESP::Enemies::pingColor));
			break;
		case ESPEntity::ALLY:
			UI::Section("Informations");
			UI::Checkbox("Name", &Settings::ESP::Allies::name);
			UI::ColorEdit(std::make_pair("Name Color"s, &Settings::ESP::Allies::nameColor));
			UI::Checkbox("Clantag", &Settings::ESP::Allies::clan);
			UI::ColorEdit(std::make_pair("Clantag Color"s, &Settings::ESP::Allies::clanColor));
			UI::Checkbox("Health", &Settings::ESP::Allies::hp);
			UI::ColorEdit(std::make_pair("Health Color"s, &Settings::ESP::Allies::hpColor));
			UI::Checkbox("Armor", &Settings::ESP::Allies::armor);
			UI::ColorEdit(std::make_pair("Armor Color"s, &Settings::ESP::Allies::armorColor));
			UI::Checkbox("Defuse kit", &Settings::ESP::Allies::kit);
			UI::ColorEdit(std::make_pair("Defusing Color"s, &Settings::ESP::Allies::kitDefusingColor), std::make_pair("Defuse Kit Color"s, &Settings::ESP::Enemies::kitColor));
			UI::Checkbox("Bomb", &Settings::ESP::Allies::bomb);
			UI::ColorEdit(std::make_pair("Planting Color"s, &Settings::ESP::Allies::bombColorPlanting), std::make_pair("Bomb Color"s, &Settings::ESP::Enemies::bombColor));
			UI::Checkbox("Hostage", &Settings::ESP::Allies::hostage);
			UI::ColorEdit(std::make_pair("Grabbing Hostage Color"s, &Settings::ESP::Allies::hostageGrabbingColor), std::make_pair("Hostage Color"s, &Settings::ESP::Enemies::hostageColor));
			UI::Checkbox("Scoped", &Settings::ESP::Allies::scoped);
			UI::ColorEdit(std::make_pair("Scoped Color"s, &Settings::ESP::Allies::scopedColor));
			UI::Checkbox("Dormant", &Settings::ESP::Allies::dormant);
			UI::ColorEdit(std::make_pair("Dormant Color"s, &Settings::ESP::Allies::dormantColor));
			UI::Checkbox("Ping", &Settings::ESP::Allies::ping);
			UI::ColorEdit(std::make_pair("Ping Color"s, &Settings::ESP::Allies::pingColor));
			break;
	}
	EndChild();
}

static i64 selected = -1;


const static std::array<std::string, ChamsMat::PlayerModulation::size> PlayerModulationNames{
	"None", "Static", "Health", "Armor", "Ammo", "Distance", "Ping", "Dormant Time"
};
const static std::array<std::string, ChamsMat::WeaponModulation::size> WeaponModulationNames{
	"None", "Static", "Ammo", "Distance"
};
const static std::array<std::string, ChamsMat::TimedGrenadeModulation::size> TimedGrenadeModulationNames{
	"None", "Static", "Distance", "Timer"
};
const static std::array<std::string, ChamsMat::BombModulation::size> BombModulationNames{
	"None", "Static", "Distance", "Timer"
};
const static std::array<std::string, ChamsMat::Modulation::size> ModulationNames{
	"None", "Static", "Distance"
};
void ChamsLeft()
{
	switch (ChamsEnt)
	{
		case ESPEntity::ENEMY:
		{
			if (selected >= Settings::Chams::Enemies::materials.size())
				selected = -1;
			if (selected < 0)
				return;

			Child("##CHAMSMODULATION", 17.05f);
			if (UI::Button2("Delete material", ImVec2(-1, 0)))
			{
				UI::AddNotification(fmt::format("Material '{}' deleted!", Settings::Chams::Enemies::materials[selected].name), UI::NotificationType::MESSAGE, 1500);
				if (Settings::Chams::Enemies::materials[selected].mat)
					Settings::Chams::Enemies::materials[selected].mat->DecrementReferenceCount();
				Settings::Chams::Enemies::validMaterials = false;
					
				Settings::Chams::Enemies::materials.erase(Settings::Chams::Enemies::materials.begin()+selected);
				selected = -1;
				EndChild();
				return;
			}
			UI::Section(fmt::format("{}'s configuration", Settings::Chams::Enemies::materials[selected].name).c_str());
			if (UI::ListCombo("Color modulation", PlayerModulationNames, Settings::Chams::Enemies::materials[selected].playerModulation))
				Settings::Chams::Enemies::materials[selected].value = 0.f;
			switch (Settings::Chams::Enemies::materials[selected].playerModulation)
			{
				case ChamsMat::PlayerModulation::STATIC:
					UI::SliderFloat("Value", &Settings::Chams::Enemies::materials[selected].value, 0.f, 0.f, 1.f);
					UI::Desc("Static value");
					break;
				case ChamsMat::PlayerModulation::AMMO:
					UI::SliderFloat("Fallback", &Settings::Chams::Enemies::materials[selected].value, 0.f, 0.f, 1.f);
					UI::Desc("Value to use in case the player has no weapon");
					break;
				case ChamsMat::PlayerModulation::DISTANCE:
					UI::InputFloat("Max distance", &Settings::Chams::Enemies::materials[selected].value, 0.f, 0.f, "%.0fu");
					break;
				case ChamsMat::PlayerModulation::PING:
					UI::SliderFloat("Max Ping", &Settings::Chams::Enemies::materials[selected].value, 0.f, 0.f, 1000.f, "%.0fms");
					break;
				case ChamsMat::PlayerModulation::DORMANT:
					UI::SliderFloat("Max Dormant Time", &Settings::Chams::Enemies::materials[selected].value, 0.f, 0.f, static_cast<f32>(PlayerAdditionalInfo::GetMaxDormantTime()), "%.0fms");
					break;
				default: break;
			}
			std::size_t i = 1;
			for (auto& col : Settings::Chams::Enemies::materials[selected].colors)
			{
				UI::ColorEdit4(fmt::format("Color {}", i).c_str(), col);
				ImGui::SameLine();
				if (UI::ButtonText("\ue002", 0xFF0000FF, ImVec2(ImGui::GetFrameHeight(), ImGui::GetFrameHeight())) && Settings::Chams::Enemies::materials[selected].colors.size() > 1)
					Settings::Chams::Enemies::materials[selected].colors.erase(Settings::Chams::Enemies::materials[selected].colors.begin()+i-1);
				UI::Desc("Remove color");
				++i;
			}
			if (UI::ButtonText("\ue000", 0xFF00FF00, ImVec2(ImGui::GetFrameHeight(), ImGui::GetFrameHeight())))
				Settings::Chams::Enemies::materials[selected].colors.push_back(ImVec4(1.f, 1.f, 1.f, 1.f));
			UI::Desc("Add new color");
			
			EndChild();
		}
		break;
		default: break;
	}
}

static TextEditor editor = []
{
	TextEditor editor;
	auto lang = TextEditor::LanguageDefinition::C();

	editor.SetLanguageDefinition(lang);
	editor.SetTabSize(2);
	return editor;
}();

void ChamsRight()
{
	UI::ListCombo("", ESPEnts, ChamsEnt);
	ImGui::Dummy(ImVec2(0, 8));

	static std::array<char, 128> buf = {0};
	auto addMaterial = [&](std::vector<ChamsMat>& materials, const std::string& name)
	{
		if (name.empty())
		{
			UI::AddNotification("Invalid material name!", UI::NotificationType::ERROR, 5000);
			return;
		}
		if (std::find_if(Settings::Chams::Enemies::materials.begin(), Settings::Chams::Enemies::materials.end(),
					[&](const ChamsMat& m) { return m.name == name; }) != Settings::Chams::Enemies::materials.end())
		{
			UI::AddNotification(fmt::format("Material '{}' already exists!", name), UI::NotificationType::ERROR, 5000);
			return;
		}

		materials.push_back(ChamsMat{
			.code = "\"VertexLitGeneric\"\n"
			"{\n"
			"	\"$basetexture\" \"vgui/white_additive\"\n"
			"	\"$model\" \"1\"\n"
			"	\"$flat\" \"1\"\n"
			"	\"$nocull\" \"0\"\n"
			"	\"$selfillum\" \"1\"\n"
			"	\"$halflambert\" \"1\"\n"
			"	\"$nofog\" \"0\"\n"
			"	\"$ignorez\" \"1\"\n"
			"	\"$znearer\" \"0\"\n"
			"	\"$wireframe\" \"0\"\n"
			"}",
			.name = std::move(name),
			.mat = nullptr,
			.colors = std::vector<ImVec4>{ImVec4(1.f, 1.f, 1.f, 1.f)},
			.value = 0.f,
		});

		buf[0] = 0;
		UI::AddNotification(fmt::format("Material '{}' created", name), UI::NotificationType::MESSAGE, 1500);
	};

	Child("##CHAMSSETTINGS", 15.8f, true);
	switch (ChamsEnt)
	{
		case ESPEntity::ENEMY:
		{
			if (Settings::Chams::Enemies::validMaterials)
				UI::Text("State: valid");
			else
				UI::Text("State: invalid");
			UI::Checkbox("Draw original model", &Settings::Chams::Enemies::drawOriginalModel);
			ImGui::Columns(2, NULL, false);
			{
				UI::InputText("", buf.data(), buf.size(), ImVec2(-1, 0));
			}
			ImGui::NextColumn();
			{
				if (UI::Button("Add material", ImVec2(-1, 0)))
				{
					addMaterial(Settings::Chams::Enemies::materials, buf.data());
					Settings::Chams::Enemies::validMaterials = false;
				}
			}
			ImGui::EndColumns();
			if (UI::Button2("Compile materials", ImVec2(-1, 0)))
			{
				if (selected >= 0)
					Settings::Chams::Enemies::materials[selected].code = editor.GetText();
				//material->UncacheUnusedMaterials();
				Settings::Chams::Enemies::validMaterials = true;
				for (auto& material : Settings::Chams::Enemies::materials)
				{
					if (material.mat)
						material.mat->DecrementReferenceCount();

					const auto beg = material.code.find('"');
					const auto end = material.code.find('"', beg+1);
					if (end == std::string::npos || beg >= end)
					{
						UI::AddNotification(fmt::format("Cannot determine material type for '{}'", material.name), UI::NotificationType::ERROR, 5000);
						Settings::Chams::Enemies::validMaterials = false;
					}
					else
					{
						static u64 created = 0;
						std::string type = material.code.substr(beg+1, end-end-1);
						material.mat = Util::CreateMaterial(fmt::format("createdmatcunyware_{}", created++), type, material.code);
						if (material.mat == nullptr)
						{
							UI::AddNotification(fmt::format("Material '{}' failed to compile", material.name), UI::NotificationType::ERROR, 5000);
							Settings::Chams::Enemies::validMaterials = false;
						}
					}
				}
				if (Settings::Chams::Enemies::validMaterials)
					UI::AddNotification("Compilation successfull", UI::NotificationType::MESSAGE, 1500);
			}
			UI::Section("Materials order");
			i64 prevSelected = selected;
			i32 ret = UI::DragList(Settings::Chams::Enemies::materials, selected, [](const ChamsMat& m) { return m.name.c_str(); },
					[&](i64 s){ if (s >= 0) Settings::Chams::Enemies::materials[s].code = editor.GetText(); });
			if (ret == 1)
				editor.SetText(Settings::Chams::Enemies::materials[selected].code);
			break;
		}
		default: break;
	}
	EndChild();
}

void ChamsEditor()
{
	Child2("##CHAMSEDITOR", 17.05f);
	if (selected < 0)
	{
		EndChild2();
		return;
	}

	ImGui::PushFont(UI::iosevka_mono);
	editor.Render("Chams editor");
	ImGui::PopFont();
//https://www.unknowncheats.me/forum/counterstrike-global-offensive/240933-change-chams-materials.html
	EndChild2();
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
			}),
			std::make_pair("Chams"s, (TabContent)
			{
				{ ChamsLeft, ChamsRight },
				{ ChamsEditor }
			})
		)
	);
}
