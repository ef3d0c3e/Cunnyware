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
