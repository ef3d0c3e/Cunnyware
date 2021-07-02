#ifndef UI_TAB_HPP
#define UI_TAB_HPP

#define IMGUI_DEFINE_MATH_OPERATORS
#include "../ImGUI/imgui.h"
#include "../ImGUI/imgui_internal.h"
#include "Style.hpp"
#include "Widgets.hpp"
#include "UI.hpp"
#include <array>
#include <functional>
#include <string>
#include "../Interface.hpp"

using namespace std::literals;

void Child(const char* label, float elems, bool scrolling = false);
void EndChild();
void Child2(const char* label, float elems, bool scrolling = false);
void EndChild2();
bool BeginPopupModal(const char* label, bool* p_open = NULL);
void EndPopup();

/*
    v     v     v <- spacings
+--+ +---+ +---+!
|c1| |###| |###|!
|c2| +---+ |###|!
|c3| +---+ |###|!
|  | |###| +---+:
|  | +---+ +---+:
+--+       |###|:
each spacing are .01
*/
template <std::size_t NCategory>
__attribute__((always_inline))
void MakeTab(const std::array<std::string, NCategory>& categories,
		const std::array<void(*)(void), NCategory>& left,
		const std::array<void(*)(void), NCategory>& right)
{
	const auto&& [w, h] = ImGui::GetContentRegionAvail();

	static unsigned int category = 0;

	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0);
	ImGui::BeginChild("##SELECTOR", ImVec2(w*.20f, h), true);

	ImGui::PushFont(UI::plex_bold);
	for (unsigned int i = 0; i < categories.size(); ++i)
	{
		if (UI::SelectButton(categories[i].c_str(), i == category, ImVec2(ImGui::GetContentRegionAvailWidth(), 40)))
			category = i;
	}
	ImGui::PopFont();

	{ // Shadow
		const auto pos = ImVec2(ImGui::GetCursorPos().x + ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowPos().y);
		float y = ImGui::GetCursorPosY();
		ImGui::SetCursorPosY(pos.y);
		static ImU32 color[4] = {0, Settings::Style::selector_shadow, Settings::Style::selector_shadow, 0};
		UI::Shadow(pos-ImVec2(ImGui::GetWindowContentRegionWidth()*Settings::Style::selector_shadow_scale, 0), pos+ImVec2(0, ImGui::GetWindowSize().y), color);
		ImGui::SetCursorPosY(y);
	}

	ImGui::EndChild();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();

	ImGui::SameLine();
	ImGui::Dummy(ImVec2(w*.01f, 0));
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
	ImGui::BeginChild("##SCROLL", ImVec2(0, 0), true);
	ImGui::PopStyleColor();
	{
		const auto&& [w, h] = ImGui::GetContentRegionAvail();
		ImGui::Dummy(ImVec2(0, w*.02f));

		ImGui::Columns(2, NULL, false);
		ImGui::SetColumnWidth(0, w*.495f);

		left[category]();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(Settings::Style::child_padding, 0));
		ImGui::NextColumn();
		ImGui::PopStyleVar();
		ImGui::SetColumnWidth(1, w*.495f);

		right[category]();
	}
	ImGui::EndChild();
}

template <std::size_t NCategory>
void MakeTab(const std::array<std::pair<std::string, std::vector<std::vector<void(*)(void)>>>, NCategory>& categories)
{
	const auto&& [w, h] = ImGui::GetContentRegionAvail();

	static unsigned int category = 0;

	ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0);
	ImGui::BeginChild("##SELECTOR", ImVec2(w*.20f, h), true);

	ImGui::PushFont(UI::plex_bold);
	for (unsigned int i = 0; i < categories.size(); ++i)
	{
		if (UI::SelectButton(categories[i].first.c_str(), i == category, ImVec2(ImGui::GetContentRegionAvailWidth(), 40)))
			category = i;
	}
	ImGui::PopFont();

	{ // Shadow
		const auto pos = ImVec2(ImGui::GetCursorPos().x + ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionWidth(), ImGui::GetWindowPos().y);
		float y = ImGui::GetCursorPosY();
		ImGui::SetCursorPosY(pos.y);
		static ImU32 color[4] = {0, Settings::Style::selector_shadow, Settings::Style::selector_shadow, 0};
		UI::Shadow(pos-ImVec2(ImGui::GetWindowContentRegionWidth()*Settings::Style::selector_shadow_scale, 0), pos+ImVec2(0, ImGui::GetWindowSize().y), color);
		ImGui::SetCursorPosY(y);
	}

	ImGui::EndChild();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();

	ImGui::SameLine();
	ImGui::Dummy(ImVec2(w*.01f, 0));
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
	ImGui::BeginChild("##SCROLL", ImVec2(0, 0), true);
	ImGui::PopStyleColor();
	{
		const auto&& [w, h] = ImGui::GetContentRegionAvail();
		ImGui::Dummy(ImVec2(0, w*.02f));

		for (unsigned int i = 0; i < categories[category].second.size(); ++i)
		{
			const auto numCols = categories[category].second[i].size();
			if (numCols > 1)
				ImGui::Columns(numCols, (const char*)&i, false);
			else
				ImGui::Columns(2, (const char*)&i, false); // We can't have single columns, so we create a second column with no content (therefore no width)
			for (unsigned int j = 0; j < numCols; ++j)
			{
				if (j != 0)
				{
					if (numCols > 1)
						ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(w*.01f, 0));
					ImGui::NextColumn();
					if (numCols > 1)
						ImGui::PopStyleVar();
				}
				ImGui::SetColumnWidth(j, w*.99f / numCols); // we save a percent for additional padding when the scrollbar is here

				categories[category].second[i][j]();
			}
			ImGui::EndColumns();
		}
	}
	ImGui::EndChild();
}

#endif // UI_TAB_HPP
