#ifndef UI_WIDGETS_HPP
#define UI_WIDGETS_HPP

#define IMGUI_DEFINE_MATH_OPERATORS
#include "../ImGUI/imgui.h"
#include "../ImGUI/imgui_internal.h"
#include "Style.hpp"
#include <vector>
#include <string>

namespace UI
{
	void RenderText(ImVec2 pos, const char* text, const char* text_end = NULL, bool hide_text_after_hash = true);
	void RenderTextClipped(const ImVec2& pos_min, const ImVec2& pos_max, const char* text, const char* text_end, const ImVec2* text_size_if_known, const ImVec2& align = ImVec2(0,0), const ImRect* clip_rect = NULL);
	void Shadow(const ImVec2& p_min, const ImVec2& p_max, const ImU32 color[4]);
	void RenderArrow(const ImVec2& p_min, ImGuiDir dir, float scale, const ImU32 color);
	void Desc(const char* label);
	void Section(const char* label); // Height is +1.8
	void PushDisabled(bool cond = true);
	void PopDisabled(bool cond = true);
	ImU32 GetColor(ImU32 col, int factor = 2);
	ImU32 GetColor(ImGuiCol idx);
	ImVec4 GetColor(const ImVec4& col, int factor = 2);
	void RenderTextWrapped(ImVec2 pos, const char* text, const char* text_end, float wrap_width);
	void TextUnformatted(const char* text, const char* text_end = NULL);
	void TextV(const char* fmt, va_list args);
	void Text(const char* label, ...);

	bool Button(const char* label, const ImVec2& size_args = ImVec2(0, 0));
	bool Button2(const char* label, const ImVec2& size_args = ImVec2(0, 0)); // button was so great they made a button 2
	bool SelectButton(const char* label, bool selected, const ImVec2& size_args = ImVec2(0, 0));
	bool TabList(const char* label, std::size_t len, u64 tab, u64 cur, u64 max, ImRect& bb_out, const ImVec2& size_args = ImVec2(0, 0));
	bool TabList(const std::string_view& label, u64 tab, u64 cur, u64 max, ImRect& bb_out, const ImVec2& size_args = ImVec2(0, 0));

	bool SliderFloat(const char* label, float* v, float v_origin, float v_min, float v_max, const char* display_format = "%.3f", float power = 1.f);
	bool SliderInt(const char* label, int* v, int v_origin, int v_min, int v_max, const char* display_format = "%d");

	bool Checkbox(const char* label, bool* v);

	bool InputText(const char* label, char* buf, size_t buf_size, ImVec2 size_args = ImVec2(0, 0), ImGuiInputTextFlags flags = 0, ImGuiTextEditCallback callback = NULL, void* user_data = NULL);

	bool BeginCombo(const std::string& label, const std::string& preview, const char* tooltip = NULL, ImGuiComboFlags flags = 0);
	void EndCombo();
	bool Selectable(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size_arg = ImVec2(0,0));
	void SetItemDefaultFocus();
	void CheckboxCombo(const char* label, const std::vector<std::tuple<const char*, const char*, bool&>>& data, std::string& format, const std::string& sep = ", "); // name, desc, value
	template <class Container, class T>
		requires std::is_integral_v<T>
	bool ListCombo(const std::string& label, const Container& elems, T& id)
	{
		bool changed = false;
		if (UI::BeginCombo(label, elems[static_cast<unsigned int>(id)]))
		{
			for (unsigned int i = 0; i < elems.size(); ++i)
			{
				if (UI::Selectable(elems[i].c_str(), static_cast<unsigned int>(id) == i))
					id = static_cast<T>(i), changed = true;
				if (static_cast<unsigned int>(id) == i)
					UI::SetItemDefaultFocus();
			}
			UI::EndCombo();
		}

		return changed;
	}

	bool ColorButton(const char* desc_id, const ImVec4& col, ImGuiColorEditFlags flags = 0, ImVec2 size = ImVec2(0, 0));
	bool ColorPicker4(const char* label, float col[4], ImGuiColorEditFlags, const float* ref_col);
	bool ColorEdit4(const char* label, ImVec4& col, ImGuiColorEditFlags = 0);
	bool ColorEdit4(const char* label, ImColor& col, ImGuiColorEditFlags = 0);

	template <class... Ts>
	void ColorEdit(const Ts&... t) //label, colorvar
	{
		static_assert(( (std::is_same_v<Ts, std::pair<std::string, ImColor*>> || std::is_same_v<Ts, std::pair<std::string, ImColor*>>) && ... ));

		const auto width = ImGui::GetFrameHeight() * Settings::Style::color_button_scale.x + ImGui::GetStyle().ItemSpacing.x + 2.f; // 2.f -> border
		const auto x = ImGui::GetContentRegionMax().x - width*sizeof...(Ts) + ImGui::GetStyle().ItemSpacing.x;

		std::size_t i = 0;
		((ImGui::SameLine(x+width*i), ++i, UI::ColorEdit4(t.first.c_str(), *t.second)), ...);
	};

	bool ListBoxHeader(const char* label, const ImVec2& size_arg);
	bool ListBoxHeader(const char* label, int items_count, int height_in_items);
	void ListBoxFooter();
	bool ListBox(const char* label, int* current_item, bool (*items_getter)(void*, int, const char**), void* data, int items_count, int height_in_items = -1);
	bool ListBox(const char* label, int* current_item, const std::vector<std::string>& elems, int height_items = 6);

	void OpenPopupEx(ImGuiID id);
	void OpenPopup(const char* str_id);
	bool BeginPopupEx(ImGuiID id, ImGuiWindowFlags extra_flags = 0);
	bool BeginPopup(const char* str_id, ImGuiWindowFlags flags = 0);
	void EndPopup();
}

#endif // UI_WIDGETS_HPP
