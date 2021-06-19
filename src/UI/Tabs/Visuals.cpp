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

		UI::Section("ESP Settings");
		UI::PushDisabled(!Settings::Visuals::enabled);
		UI::Checkbox("Antialiasing", &Settings::Visuals::antialiasing);
		UI::Desc("Turns on antialiasing for cleaner looking results,\nat the cost of performances");
		UI::PopDisabled(!Settings::Visuals::enabled);

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
			})
		)
	);
}
