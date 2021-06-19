#include "Tabs.hpp"

void Main()
{
	Child("##CONFIGSLIST", 16.8f);
	{
		UI::Section("Configurations");
		ImGui::Columns(2, NULL, false);
		{
			static std::vector<std::string> configItems = {"cfg1", "my super strong config lol!"};
				//GetConfigs();
			static int configItemCurrent = -1;
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 4));
			UI::ListBox("", &configItemCurrent, configItems, 17);
			ImGui::PopStyleVar();
		}
		ImGui::NextColumn();
		{
			ImGui::Text("Config name");
			std::array<char, 128> buf = {0};
			UI::InputText("", buf.data(), buf.size(), ImVec2(-1, 0));

			if (UI::Button2("Create", ImVec2(-1, 0)))
			{

			}

			if (UI::Button2("Load", ImVec2(-1, 0)))
			{

			}

			if (UI::Button2("Save", ImVec2(-1, 0)))
			{

			}

			if (UI::Button2("Refresh", ImVec2(-1, 0)))
			{

			}

			if (UI::Button2("Delete", ImVec2(-1, 0)))
			{

			}
		}
		ImGui::EndColumns();
	}
	EndChild();
}

void Tabs::Configs()
{
	MakeTab
	(
		make_array
		(
			std::make_pair("Configurations"s, (std::vector<std::vector<void(*)(void)>>)
			{
				{ Main }
			})
		)
	);
}
