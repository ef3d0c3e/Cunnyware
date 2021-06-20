#include "Tabs.hpp"
#include "../../Config.hpp"
#include "../../Hacks/Visuals.hpp"

void Main()
{
	static std::vector<std::string> configItems;
	static i64 configItemCurrent = -1;
	static bool first = true;
	try
	{
		if (first)
			configItems = Config::GetConfigs(), first = false;
	}
	catch (Exception& e)
	{
		first = false;
		UI::AddNotification(e.what(), UI::NotificationType::ERROR, 5000);
	}

	Child("##CONFIGSLIST", 17.f);
	{
		UI::Section("Configurations");
		ImGui::Columns(2, NULL, false);
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 4));
			UI::ListBox("", &configItemCurrent, configItems, 17);
			ImGui::PopStyleVar();
		}
		ImGui::NextColumn();
		{
			ImGui::Text("Config name");
			static std::array<char, 128> buf = {0};
			UI::InputText("", buf.data(), buf.size(), ImVec2(-1, 0));

			if (UI::Button2("Create", ImVec2(-1, 0)))
			{
				try
				{
					Config::Save(std::string(buf.data(), strlen(buf.data())));
					configItems = Config::GetConfigs();
					UI::AddNotification("Configuraton created!", UI::NotificationType::MESSAGE, 1500);
				}
				catch (Exception& e)
				{
					UI::AddNotification(e.what(), UI::NotificationType::ERROR, 5000);
				}
			}

			if (UI::Button2("Load", ImVec2(-1, 0)))
			{
				if (static_cast<u64>(configItemCurrent) >= configItems.size()) // -1 is covered
					UI::AddNotification("No configuration selected!", UI::NotificationType::ERROR, 5000);
				else
				{
					try
					{
						Config::Load(configItems[configItemCurrent]);
						UI::AddNotification("Configuration loaded!", UI::NotificationType::MESSAGE, 1500);
					}
					catch (Exception& e)
					{
						UI::AddNotification(e.what(), UI::NotificationType::ERROR, 5000);
					}
				}
			}

			if (UI::Button2("Save", ImVec2(-1, 0)))
			{
				try
				{
					Config::Save(configItems[configItemCurrent]);
					configItems = Config::GetConfigs();
					UI::AddNotification("Configuraton saved!", UI::NotificationType::MESSAGE, 1500);
				}
				catch (Exception& e)
				{
					UI::AddNotification(e.what(), UI::NotificationType::ERROR, 5000);
				}
			}

			if (UI::Button2("Refresh", ImVec2(-1, 0)))
			{
				try
				{
					configItems = Config::GetConfigs();
				}
				catch (Exception& e)
				{
					UI::AddNotification(e.what(), UI::NotificationType::ERROR, 5000);
				}
			}

			if (UI::Button2("Delete", ImVec2(-1, 0)))
			{
				if (static_cast<u64>(configItemCurrent) >= configItems.size()) // -1 is covered
					UI::AddNotification("No configuration selected!", UI::NotificationType::ERROR, 5000);
				else
				{
					try
					{
						Config::Delete(configItems[configItemCurrent]);
						configItems = Config::GetConfigs();
						UI::AddNotification("Configuraton deleted!", UI::NotificationType::MESSAGE, 1500);
					}
					catch (Exception& e)
					{
						UI::AddNotification(e.what(), UI::NotificationType::ERROR, 5000);
					}
				}
			}
		}
		ImGui::EndColumns();
	}
	EndChild();
}

void Logs()
{
	Child("##MESSAGES", 17.f, true);
	UI::Logs(UI::messages);
	EndChild();
}

void Menu()
{
	Child("##MENU", 2.5f);
	UI::Section("Graphics options");
	UI::Checkbox("Antialiasing", &Settings::Visuals::antialiasing);
	UI::Desc("Turns on antialiasing for cleaner looking results,\nat the cost of performances");
	EndChild();
}

void Tabs::Configs()
{
	MakeTab
	(
		make_array
		(
			std::make_pair("Configurations"s, (TabContent)
			{
				{ Main }
			}),
			std::make_pair("Logs"s, (TabContent)
			{
				{ Logs }
			}),
			std::make_pair("Menu"s, (TabContent)
			{
				{ Menu, []{} }
			})
		)
	);
}
