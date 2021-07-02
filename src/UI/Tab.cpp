#include "Tab.hpp"

static float CalcHeight(float elems)
{
	ImGuiContext& g = *GImGui;
    return elems*(g.FontSize + g.Style.FramePadding.y * 2.0f + 8.f/2)+Settings::Style::child_padding*2.f-5.f;
}

void Child(const char* label, float elems, bool scrolling)
{
	ImGui::PushStyleColor(ImGuiCol_Border, Settings::Style::child_border);
	ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.f);
	ImGui::BeginChild(label, ImVec2(-1, CalcHeight(elems)), true, scrolling ? 0 : ImGuiWindowFlags_NoScrollWithMouse);
	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
	ImGui::Dummy(ImVec2(0, Settings::Style::child_padding));
	ImGui::Dummy(ImVec2(Settings::Style::child_padding, 0));
	ImGui::SameLine();
	const auto&& [w, h] = ImGui::GetContentRegionAvail();
	ImGui::PushStyleColor(ImGuiCol_ChildBg, 0x00000000);
	ImGui::BeginChild(label, ImVec2(w-Settings::Style::child_padding, h-Settings::Style::child_padding), true, scrolling ? 0 : ImGuiWindowFlags_NoScrollWithMouse);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 8));
}

void EndChild()
{
	ImGui::PopStyleVar();
	ImGui::EndChild();
	ImGui::PopStyleColor();
	ImGui::EndChild();
	ImGui::Dummy(ImVec2(0, Settings::Style::child_padding));
}

void Child2(const char* label, float elems, bool scrolling)
{
	ImGui::PushStyleColor(ImGuiCol_Border, Settings::Style::child_border);
	ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.f);
	ImGui::BeginChild(label, ImVec2(-1, CalcHeight(elems)), true, scrolling ? 0 : ImGuiWindowFlags_NoScrollWithMouse);
	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 8));
}

void EndChild2()
{
	ImGui::PopStyleVar();
	ImGui::EndChild();
	ImGui::Dummy(ImVec2(0, Settings::Style::child_padding));
}

bool BeginPopupModal(const char* label, bool* p_open)
{
	return ImGui::BeginPopupModal(label, p_open, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
}

void EndPopup()
{
	ImGui::PopStyleVar();
}
