#include "UI.hpp"
#include "../Interface.hpp"

bool UI::isVisible = false;

void UI::SetVisible(bool visible)
{
	UI::isVisible = visible;
	cvar->FindVar("cl_mouseenable")->SetValue(!UI::isVisible);
}

void UI::Draw()
{
	if (isVisible)
	{
		ImGui::Begin("Test window drawx");
		ImGui::Text("In Game: %d", cvar->FindVar("cl_mouseenable")->GetInt());
		static char buf[32];
		ImGui::InputText("abc", buf, 32);
		ImGui::End();
	}
}
