#include "Widgets.hpp"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "../ImGUI/imgui.cpp"
#include "../ImGUI/imgui_internal.h"
#include "UI.hpp"
#include <tuple>
#include <GL/gl.h>

using namespace std::literals;
static bool isDisabled = false;

// {{{ Misc
ImU32 Darken(ImU32 col, i32 factor)
{
	auto& c = *reinterpret_cast<Vector<u8, 4>*>(&col);
	return (c.x >> factor) |  ((c.y >> factor) << 8) | ((c.z >> factor) << 16) | (c.w << 24);
}

void RenderFrame(ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, float border_size, float rounding, ImU32 border_col, ImDrawCornerFlags corner_flags = ImDrawCornerFlags_All)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	window->DrawList->AddRectFilled(p_min, p_max, fill_col, rounding);
	if (border_size > 0.0f)
		window->DrawList->AddRect(p_min, p_max, border_col, rounding, corner_flags, border_size);
}

void RenderRectGradient(ImVec2 p_min, ImVec2 p_max, ImU32 col_top, ImU32 col_bot)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	window->DrawList->AddRectFilledMultiColor(p_min, p_max, col_top, col_top, col_bot, col_bot);
}

void TabListButton(ImVec2 p_min, ImVec2 p_max, ImU32 col_top, ImU32 col_bot, ImU32 border_col_out, ImU32 border_col_in, ImDrawCornerFlags corner_flags = ImDrawCornerFlags_All)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	window->DrawList->AddRectFilledMultiColor(p_min, p_max, col_top, col_top, col_bot, col_bot);
	window->DrawList->AddRect(p_min, p_max, border_col_out, 0.f, corner_flags, 1.f);
	window->DrawList->AddRect(p_min+ImVec2(1.f, 1.f), p_max-ImVec2(1.f, 1.f), border_col_in, 0.f, corner_flags, 1.f);
}

void UI::RenderText(ImVec2 pos, const char* text, const char* text_end, bool hide_text_after_hash)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;

	// Hide anything after a '##' string
	const char* text_display_end;
	if (hide_text_after_hash)
	{
		text_display_end = ImGui::FindRenderedTextEnd(text, text_end);
	}
	else
	{
		if (!text_end)
			text_end = text + strlen(text); // FIXME-OPT
		text_display_end = text_end;
	}

	if (text != text_display_end)
	{
		window->DrawList->AddText(g.Font, g.FontSize, pos, GetColor(ImGuiCol_Text), text, text_display_end);
		if (g.LogEnabled)
			LogRenderedText(&pos, text, text_display_end);
	}
}

// Default clip_rect uses (pos_min,pos_max)
// Handle clipping on CPU immediately (vs typically let the GPU clip the triangles that are overlapping the clipping rectangle edges)
void UI::RenderTextClipped(const ImVec2& pos_min, const ImVec2& pos_max, const char* text, const char* text_end, const ImVec2* text_size_if_known, const ImVec2& align, const ImRect* clip_rect)
{
	// Hide anything after a '##' string
	const char* text_display_end = ImGui::FindRenderedTextEnd(text, text_end);
	const int text_len = (int)(text_display_end - text);
	if (text_len == 0)
		return;

	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;

	// Perform CPU side clipping for single clipped element to avoid using scissor state
	ImVec2 pos = pos_min;
	const ImVec2 text_size = text_size_if_known ? *text_size_if_known : ImGui::CalcTextSize(text, text_display_end, false, 0.0f);

	const ImVec2* clip_min = clip_rect ? &clip_rect->Min : &pos_min;
	const ImVec2* clip_max = clip_rect ? &clip_rect->Max : &pos_max;
	bool need_clipping = (pos.x + text_size.x >= clip_max->x) || (pos.y + text_size.y >= clip_max->y);
	if (clip_rect) // If we had no explicit clipping rectangle then pos==clip_min
		need_clipping |= (pos.x < clip_min->x) || (pos.y < clip_min->y);

	// Align whole block. We should defer that to the better rendering function when we'll have support for individual line alignment.
	if (align.x > 0.0f) pos.x = ImMax(pos.x, pos.x + (pos_max.x - pos.x - text_size.x) * align.x);
	if (align.y > 0.0f) pos.y = ImMax(pos.y, pos.y + (pos_max.y - pos.y - text_size.y) * align.y);

	// Render
	if (need_clipping)
	{
		ImVec4 fine_clip_rect(clip_min->x, clip_min->y, clip_max->x, clip_max->y);
		window->DrawList->AddText(g.Font, g.FontSize, pos, GetColor(ImGuiCol_Text), text, text_display_end, 0.0f, &fine_clip_rect);
	}
	else
	{
		window->DrawList->AddText(g.Font, g.FontSize, pos, GetColor(ImGuiCol_Text), text, text_display_end, 0.0f, NULL);
	}
	if (g.LogEnabled)
		LogRenderedText(&pos, text, text_display_end);
}

void UI::Shadow(const ImVec2& p_min, const ImVec2& p_max, const ImU32 color[4])
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;

	window->DrawList->AddRectFilledMultiColor(p_min, p_max, color[0], color[1], color[2], color[3]);
}

/*
void UI::RenderArrow(const ImVec2& p_min, ImGuiDir dir, float scale, const ImU32 color)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;

	float sz = g.FontSize * 0.8f * scale;
	ImVec2 pos = p_min;
	float thickness = ImMax(sz / 5.0f, 1.0f);
	sz -= thickness*0.5f;
	pos += ImVec2(thickness*0.25f, thickness*0.25f);
	
	const float dist = sz / 2.0f;
	float bx, by;
	switch (dir)
	{
		case ImGuiDir_Up:
			bx = pos.x + dist;
			by = pos.y + dist * 0.66f;
			window->DrawList->PathLineTo(ImVec2(bx - dist, by + dist));
			window->DrawList->PathLineTo(ImVec2(bx, by));
			window->DrawList->PathLineTo(ImVec2(bx + dist, by + dist));
			break;
		case ImGuiDir_Down:
			bx = pos.x + dist;
			by = pos.y + sz - dist * .33f;
			window->DrawList->PathLineTo(ImVec2(bx - dist, by - dist));
			window->DrawList->PathLineTo(ImVec2(bx, by));
			window->DrawList->PathLineTo(ImVec2(bx + dist, by - dist));
			break;
		case ImGuiDir_Left:
			bx = pos.x + dist * .66f;
			by = pos.y + sz - dist * .66f;
			window->DrawList->PathLineTo(ImVec2(bx + dist, by + dist));
			window->DrawList->PathLineTo(ImVec2(bx, by));
			window->DrawList->PathLineTo(ImVec2(bx + dist, by - dist));
			break;
		case ImGuiDir_Right:
			bx = pos.x + dist * 1.33f;
			by = pos.y + sz - dist * .66f;
			window->DrawList->PathLineTo(ImVec2(bx - dist, by + dist));
			window->DrawList->PathLineTo(ImVec2(bx, by));
			window->DrawList->PathLineTo(ImVec2(bx - dist, by - dist));
			break;

	}
	window->DrawList->PathStroke(color, false, thickness);
}
*/

void UI::RenderArrow(const ImVec2& p_min, ImGuiDir dir, float scale, const ImU32 color)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;

	float sz = g.FontSize * 0.8f * scale;
	ImVec2 pos = p_min;
	float thickness = ImMax(sz / 5.0f, 1.0f);
	sz -= thickness*0.5f;
	pos += ImVec2(thickness*0.25f, thickness*0.25f);
	
	const float dist = sz / 2.0f;
	float bx, by;
	switch (dir)
	{
		case ImGuiDir_Up:
			bx = pos.x + dist;
			by = pos.y + dist * 0.66f;
			window->DrawList->AddTriangleFilled(ImVec2(bx - dist, by + dist),
			ImVec2(bx, by),
			ImVec2(bx + dist, by + dist), color);
			break;
		case ImGuiDir_Down:
			bx = pos.x + dist;
			by = pos.y + sz - dist * .33f;
			window->DrawList->AddTriangleFilled(ImVec2(bx - dist, by - dist),
			ImVec2(bx, by),
			ImVec2(bx + dist, by - dist), color);
			break;
		case ImGuiDir_Left:
			bx = pos.x + dist * .66f;
			by = pos.y + sz - dist * .66f;
			window->DrawList->AddTriangleFilled(ImVec2(bx + dist, by + dist),
			ImVec2(bx, by),
			ImVec2(bx + dist, by - dist), color);
			break;
		case ImGuiDir_Right:
			bx = pos.x + dist * 1.33f;
			by = pos.y + sz - dist * .66f;
			window->DrawList->AddTriangleFilled(ImVec2(bx - dist, by + dist),
			ImVec2(bx, by),
			ImVec2(bx - dist, by - dist), color);
			break;

	}
}

void Tooltip(const char* label)
{
	ImGuiContext& g = *GImGui;
	char window_name[16];
	ImFormatString(window_name, IM_ARRAYSIZE(window_name), "##Tooltip_%02d", g.TooltipOverrideCount);
	if (ImGuiWindow* window = ImGui::FindWindowByName(window_name))
		if (window->Active)
		{
			// Hide previous tooltip from being displayed. We can't easily "reset" the content of a window so we create a new one.
			window->HiddenFrames = 1;
			ImFormatString(window_name, IM_ARRAYSIZE(window_name), "##Tooltip_%02d", ++g.TooltipOverrideCount);
		}
	ImGuiWindowFlags flags = ImGuiWindowFlags_Tooltip|ImGuiWindowFlags_NoInputs|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoSavedSettings|ImGuiWindowFlags_AlwaysAutoResize|ImGuiWindowFlags_NoNav;

	ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, Settings::Style::tooltip_border);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, Settings::Style::tooltip_padding);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, Settings::Style::tooltip_rounding);
	ImGui::PushStyleColor(ImGuiCol_Text, Settings::Style::tooltip_color[0]);
	ImGui::PushStyleColor(ImGuiCol_Border, Settings::Style::tooltip_color[1]);
	ImGui::PushStyleColor(ImGuiCol_PopupBg, Settings::Style::tooltip_color[2]);
	ImGui::PushFont(UI::plex_mono);
	ImGui::Begin(window_name, NULL, flags);

	ImGui::Text(label);

	IM_ASSERT(ImGui::GetCurrentWindowRead()->Flags & ImGuiWindowFlags_Tooltip);   // Mismatched BeginTooltip()/EndTooltip() calls
	ImGui::End();
	ImGui::PopFont();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
}

void UI::Desc(const char *label)
{
	if (!ImGui::IsItemHovered())
		return;

	Tooltip(label);
}

void UI::Section(const char* label)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;

	ImGui::PushFont(UI::plex_bold);
	ImGui::Text(label);
	ImGui::PopFont();

	const ImRect bb(window->DC.CursorPos + ImVec2(4, 0), window->DC.CursorPos + ImVec2(ImGui::GetContentRegionAvail().x-4, 1.f));

	window->DrawList->AddRectFilledMultiColor(bb.Min, bb.Max, Settings::Style::section_sep_left, Settings::Style::section_sep_right, Settings::Style::section_sep_right, Settings::Style::section_sep_left);
	ImGui::Spacing();
	ImGui::Spacing();
}

void UI::PushDisabled(bool cond)
{
	if (!cond)
		return;

	isDisabled = true;
	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
}

void UI::PopDisabled(bool cond)
{
	if (!cond)
		return;
	isDisabled = false;
	ImGui::PopItemFlag();
}

ImU32 UI::GetColor(ImU32 col, int factor)
{
	if (isDisabled)
		return (col & 0xFF000000)
			| (((col & 0x00FF0000) >> factor) & 0x00FF0000)
			| (((col & 0x0000FF00) >> factor) & 0x0000FF00)
			| (((col & 0x000000FF) >> factor) & 0x000000FF);
	
	return col;
}

ImU32 UI::GetColor(ImGuiCol idx)
{
    ImGuiStyle& style = GImGui->Style;
    ImVec4 c = style.Colors[idx];
	if (idx == ImGuiCol_Text)
		return GetColor(ImGui::ColorConvertFloat4ToU32(c), 1);
	return GetColor(ImGui::ColorConvertFloat4ToU32(c), 2);
}

ImVec4 UI::GetColor(const ImVec4& col, int factor)
{
	if (isDisabled)
		return ImVec4(col.x / (1<<factor), col.y / (1<<factor), col.z / (1<<factor), col.w);
	
	return col;
}

void UI::RenderTextWrapped(ImVec2 pos, const char* text, const char* text_end, float wrap_width)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;

	if (!text_end)
		text_end = text + strlen(text); // FIXME-OPT

	if (text != text_end)
	{
		window->DrawList->AddText(g.Font, g.FontSize, pos, GetColor(ImGuiCol_Text), text, text_end, wrap_width);
		if (g.LogEnabled)
			LogRenderedText(&pos, text, text_end);
	}
}

void UI::TextUnformatted(const char* text, const char* text_end)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImGuiContext& g = *GImGui;
	IM_ASSERT(text != NULL);
	const char* text_begin = text;
	if (text_end == NULL)
		text_end = text + strlen(text); // FIXME-OPT

	const ImVec2 text_pos(window->DC.CursorPos.x, window->DC.CursorPos.y + window->DC.CurrentLineTextBaseOffset);
	const float wrap_pos_x = window->DC.TextWrapPos;
	const bool wrap_enabled = wrap_pos_x >= 0.0f;
	if (text_end - text > 2000 && !wrap_enabled)
	{
		// Long text!
		// Perform manual coarse clipping to optimize for long multi-line text
		// From this point we will only compute the width of lines that are visible. Optimization only available when word-wrapping is disabled.
		// We also don't vertically center the text within the line full height, which is unlikely to matter because we are likely the biggest and only item on the line.
		const char* line = text;
		const float line_height = ImGui::GetTextLineHeight();
		const ImRect clip_rect = window->ClipRect;
		ImVec2 text_size(0,0);

		if (text_pos.y <= clip_rect.Max.y)
		{
			ImVec2 pos = text_pos;

			// Lines to skip (can't skip when logging text)
			if (!g.LogEnabled)
			{
				int lines_skippable = (int)((clip_rect.Min.y - text_pos.y) / line_height);
				if (lines_skippable > 0)
				{
					int lines_skipped = 0;
					while (line < text_end && lines_skipped < lines_skippable)
					{
						const char* line_end = strchr(line, '\n');
						if (!line_end)
							line_end = text_end;
						line = line_end + 1;
						lines_skipped++;
					}
					pos.y += lines_skipped * line_height;
				}
			}

			// Lines to render
			if (line < text_end)
			{
				ImRect line_rect(pos, pos + ImVec2(FLT_MAX, line_height));
				while (line < text_end)
				{
					const char* line_end = strchr(line, '\n');
					if (ImGui::IsClippedEx(line_rect, 0, false))
						break;

					const ImVec2 line_size = ImGui::CalcTextSize(line, line_end, false);
					text_size.x = ImMax(text_size.x, line_size.x);
					RenderText(pos, line, line_end, false);
					if (!line_end)
						line_end = text_end;
					line = line_end + 1;
					line_rect.Min.y += line_height;
					line_rect.Max.y += line_height;
					pos.y += line_height;
				}

				// Count remaining lines
				int lines_skipped = 0;
				while (line < text_end)
				{
					const char* line_end = strchr(line, '\n');
					if (!line_end)
						line_end = text_end;
					line = line_end + 1;
					lines_skipped++;
				}
				pos.y += lines_skipped * line_height;
			}

			text_size.y += (pos - text_pos).y;
		}

		ImRect bb(text_pos, text_pos + text_size);
		ImGui::ItemSize(bb);
		ImGui::ItemAdd(bb, 0);
	}
	else
	{
		const float wrap_width = wrap_enabled ? ImGui::CalcWrapWidthForPos(window->DC.CursorPos, wrap_pos_x) : 0.0f;
		const ImVec2 text_size = ImGui::CalcTextSize(text_begin, text_end, false, wrap_width);

		// Account of baseline offset
		ImRect bb(text_pos, text_pos + text_size);
		ImGui::ItemSize(text_size);
		if (!ImGui::ItemAdd(bb, 0))
			return;

		// Render (we don't hide text after ## in this end-user function)
		RenderTextWrapped(bb.Min, text_begin, text_end, wrap_width);
	}
}

void UI::TextV(const char* fmt, va_list args)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return;

	ImGuiContext& g = *GImGui;
	const char* text_end = g.TempBuffer + ImFormatStringV(g.TempBuffer, IM_ARRAYSIZE(g.TempBuffer), fmt, args);
	TextUnformatted(g.TempBuffer, text_end);
}

void UI::Text(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	TextV(fmt, args);
	va_end(args);
}
// }}}

// {{{ Buttons
bool UI::Button(const char* label, const ImVec2& size_arg)
{
	ImGuiButtonFlags flags = 0;

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrentLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
		pos.y += window->DC.CurrentLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f + label_size.y * 6, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, pos + size);
	ImGui::ItemSize(bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	if (window->DC.ItemFlags & ImGuiItemFlags_ButtonRepeat)
		flags |= ImGuiButtonFlags_Repeat;
	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);
	if (pressed)
		ImGui::MarkItemValueChanged(id);

	// Render
	const ImU32 col = (held && hovered) ? Settings::Style::button_bg[2] : hovered ? Settings::Style::button_bg[1] :
																					  Settings::Style::button_bg[0];
	ImGui::RenderNavHighlight(bb, id);
	ImGui::RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);
	ImGui::PushStyleColor(ImGuiCol_Text, Settings::Style::button_text);
	UI::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);
	ImGui::PopStyleColor();

	return pressed;
}

bool UI::Button2(const char* label, const ImVec2& size_arg)
{
	ImGuiButtonFlags flags = 0;

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrentLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
		pos.y += window->DC.CurrentLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f + label_size.y * 6, label_size.y + style.FramePadding.y * 2.0f);
	size.y *= 1.2;

	const ImRect bb(pos, pos + size);
	ImGui::ItemSize(bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	if (window->DC.ItemFlags & ImGuiItemFlags_ButtonRepeat)
		flags |= ImGuiButtonFlags_Repeat;
	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);
	if (pressed)
		ImGui::MarkItemValueChanged(id);

	// Render
	const ImU32 col = (held && hovered) ? Settings::Style::button2_bg[2] : hovered ? Settings::Style::button2_bg[1] :
																					  Settings::Style::button2_bg[0];
	ImGui::RenderNavHighlight(bb, id);
	RenderFrame(bb.Min, bb.Max, col, 1.f, 0.f, Settings::Style::button2_border);
	ImGui::PushStyleColor(ImGuiCol_Text, Settings::Style::button2_text);
	ImGui::PushFont(UI::plex_bold);
	UI::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);
	ImGui::PopFont();
	ImGui::PopStyleColor();

	return pressed;
}

bool UI::SelectButton(const char* label, bool selected, const ImVec2& size_arg)
{
	ImGuiButtonFlags flags = 0;

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

	ImVec2 pos = window->DC.CursorPos;
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrentLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
		pos.y += window->DC.CurrentLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

	const ImRect bb(pos, pos + size);
	ImGui::ItemSize(bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	if (window->DC.ItemFlags & ImGuiItemFlags_ButtonRepeat)
		flags |= ImGuiButtonFlags_Repeat;
	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);
	if (pressed)
		ImGui::MarkItemValueChanged(id);

	// Render
	const ImU32 col = (held && hovered) ? Settings::Style::selector_bg[3] : hovered ? Settings::Style::selector_bg[2] :
																		selected	? Settings::Style::selector_bg[1] :
																						Settings::Style::selector_bg[0];

	ImGui::RenderNavHighlight(bb, id);
	if (selected)
	{
		RenderFrame(bb.Min, bb.Max, col, 1.f, 0, Settings::Style::selector_border);
		window->DrawList->AddRectFilled(bb.Min, ImVec2(bb.Min.x + label_size.y * .3, bb.Max.y), Settings::Style::selector_accent, 0);
		ImGui::PushStyleColor(ImGuiCol_Text, Settings::Style::selector_accent);
		UI::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);
		ImGui::PopStyleColor();
	}
	else
	{
		RenderFrame(bb.Min, bb.Max, col, 0.f, 0, Settings::Style::selector_border);
		UI::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, NULL, &label_size, style.ButtonTextAlign, &bb);
	}

	return pressed;
}

bool UI::TabList(const char* label, std::size_t len, u64 tab, u64 cur, u64 max, ImRect& bb_out, const ImVec2& size_arg)
{
	ImGuiButtonFlags flags = 0;

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label, label+len);
	const ImVec2 label_size = ImGui::CalcTextSize(label, label+len, true);

	ImVec2 pos = ImVec2((i32)window->DC.CursorPos.x, (i32)window->DC.CursorPos.y);
	if ((flags & ImGuiButtonFlags_AlignTextBaseLine) && style.FramePadding.y < window->DC.CurrentLineTextBaseOffset) // Try to vertically align buttons that are smaller/have no padding so that text baseline matches (bit hacky, since it shouldn't be a flag)
		pos.y += window->DC.CurrentLineTextBaseOffset - style.FramePadding.y;
	ImVec2 size = ImGui::CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);
	size = ImVec2((i32)size.x, (i32)size.y);

	ImRect bb(pos, pos + size);
	ImGui::ItemSize(bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(bb, id))
		return false;

	if (window->DC.ItemFlags & ImGuiItemFlags_ButtonRepeat)
		flags |= ImGuiButtonFlags_Repeat;
	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, flags);
	if (pressed)
		ImGui::MarkItemValueChanged(id);

	// Render
	const ImU32 col_top = cur == tab ? Settings::Style::tablist_bg1[1] : (held && hovered) ? Settings::Style::tablist_bg1[3] :
																hovered ? Settings::Style::tablist_bg1[2] :
																						  Settings::Style::tablist_bg1[0];
	const ImU32 col_bot = cur == tab ? Settings::Style::tablist_bg2[1] : (held && hovered) ? Settings::Style::tablist_bg2[3] :
																hovered ? Settings::Style::tablist_bg2[2] :
																						  Settings::Style::tablist_bg2[0];

	ImGui::RenderNavHighlight(bb, id);
	bb_out = bb;
	if (cur == tab)
	{
		bb = ImRect{ bb.Min - ImVec2(0, Settings::Style::tablist_current_standout), bb.Max + ImVec2(0, Settings::Style::tablist_current_standout) };

		TabListButton(bb.Min, bb.Max, col_top, col_bot, Settings::Style::tablist_border[0], Settings::Style::tablist_border[1]);

		ImGui::PushStyleColor(ImGuiCol_Text, Settings::Style::tablist_accent[1]);
		bb.Min.y -= (i32)(Settings::Style::tablist_current_standout / 2);
		bb.Max.y -= (i32)(Settings::Style::tablist_current_standout / 2);
	}
	else
	{
		window->DrawList->AddRectFilledMultiColor(bb.Min, bb.Max, col_top, col_top, col_bot, col_bot);
		TabListButton(bb.Min, bb.Max, col_top, col_bot, Settings::Style::tablist_border[0], Settings::Style::tablist_border[1]);
		ImGui::PushStyleColor(ImGuiCol_Text, Settings::Style::tablist_accent[0]);
	}

	UI::RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, label, label+len, &label_size, style.ButtonTextAlign, &bb);
	ImGui::PopStyleColor();

	return pressed;
}


bool UI::TabList(const std::string_view& label, u64 tab, u64 cur, u64 max, ImRect& bb_out, const ImVec2& size_args)
{
	return UI::TabList(label.data(), label.size(), tab, cur, max, bb_out, size_args);
}
// }}}

// {{{ Sliders
template <typename TYPE, typename SIGNEDTYPE, typename FLOATTYPE>
static bool SliderBehaviorT(const ImRect& bb, ImGuiID id, ImGuiDataType data_type, TYPE* v, const TYPE v_origin, const TYPE v_min, const TYPE v_max, const char* format, float power, ImGuiSliderFlags flags)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;

	const bool is_horizontal = (flags & ImGuiSliderFlags_Vertical) == 0;
	const bool is_decimal = (data_type == ImGuiDataType_Float) || (data_type == ImGuiDataType_Double);
	const bool is_power = (power != 1.0f) && is_decimal;

	const float slider_sz = is_horizontal ? (bb.GetWidth()) : (bb.GetHeight());
	SIGNEDTYPE v_range = (v_min < v_max ? v_max - v_min : v_min - v_max);
	const float slider_usable_sz = slider_sz;
	const float slider_usable_pos_min = (is_horizontal ? bb.Min.x : bb.Min.y);
	const float slider_usable_pos_max = (is_horizontal ? bb.Max.x : bb.Max.y);

	// For power curve sliders that cross over sign boundary we want the curve to be symmetric around 0.0f
	float linear_zero_pos; // 0.0->1.0f
	if (is_power && v_min * v_max < 0.0f)
	{
		// Different sign
		const FLOATTYPE linear_dist_min_to_0 = ImPow(v_min >= 0 ? (FLOATTYPE)v_min : -(FLOATTYPE)v_min, (FLOATTYPE)1.0f / power);
		const FLOATTYPE linear_dist_max_to_0 = ImPow(v_max >= 0 ? (FLOATTYPE)v_max : -(FLOATTYPE)v_max, (FLOATTYPE)1.0f / power);
		linear_zero_pos = (float)(linear_dist_min_to_0 / (linear_dist_min_to_0 + linear_dist_max_to_0));
	}
	else
	{
		// Same sign
		linear_zero_pos = v_min < 0.0f ? 1.0f : 0.0f;
	}

	// Process interacting with the slider
	bool value_changed = false;
	if (g.ActiveId == id)
	{
		bool set_new_value = false;
		float clicked_t = 0.0f;
		if (g.ActiveIdSource == ImGuiInputSource_Mouse)
		{
			if (!g.IO.MouseDown[0])
			{
				ImGui::ClearActiveID();
			}
			else
			{
				const float mouse_abs_pos = is_horizontal ? g.IO.MousePos.x : g.IO.MousePos.y;
				clicked_t = (slider_usable_sz > 0.0f) ? ImClamp((mouse_abs_pos - slider_usable_pos_min) / slider_usable_sz, 0.0f, 1.0f) : 0.0f;
				if (!is_horizontal)
					clicked_t = 1.0f - clicked_t;
				set_new_value = true;
			}
		}
		else if (g.ActiveIdSource == ImGuiInputSource_Nav)
		{
			const ImVec2 delta2 = ImGui::GetNavInputAmount2d(ImGuiNavDirSourceFlags_Keyboard | ImGuiNavDirSourceFlags_PadDPad, ImGuiInputReadMode_RepeatFast, 0.0f, 0.0f);
			float delta = is_horizontal ? delta2.x : -delta2.y;
			if (g.NavActivatePressedId == id && !g.ActiveIdIsJustActivated)
			{
				ImGui::ClearActiveID();
			}
			else if (delta != 0.0f)
			{
				clicked_t = SliderBehaviorCalcRatioFromValue<TYPE, FLOATTYPE>(data_type, *v, v_min, v_max, power, linear_zero_pos);
				const int decimal_precision = is_decimal ? ImParseFormatPrecision(format, 3) : 0;
				if ((decimal_precision > 0) || is_power)
				{
					delta /= 100.0f; // Gamepad/keyboard tweak speeds in % of slider bounds
					if (IsNavInputDown(ImGuiNavInput_TweakSlow))
						delta /= 10.0f;
				}
				else
				{
					if ((v_range >= -100.0f && v_range <= 100.0f) || IsNavInputDown(ImGuiNavInput_TweakSlow))
						delta = ((delta < 0.0f) ? -1.0f : +1.0f) / (float)v_range; // Gamepad/keyboard tweak speeds in integer steps
					else
						delta /= 100.0f;
				}
				if (IsNavInputDown(ImGuiNavInput_TweakFast))
					delta *= 10.0f;
				set_new_value = true;
				if ((clicked_t >= 1.0f && delta > 0.0f) || (clicked_t <= 0.0f && delta < 0.0f)) // This is to avoid applying the saturation when already past the limits
					set_new_value = false;
				else
					clicked_t = ImSaturate(clicked_t + delta);
			}
		}

		if (set_new_value)
		{
			TYPE v_new;
			if (is_power)
			{
				// Account for power curve scale on both sides of the zero
				if (clicked_t < linear_zero_pos)
				{
					// Negative: rescale to the negative range before powering
					float a = 1.0f - (clicked_t / linear_zero_pos);
					a = ImPow(a, power);
					v_new = ImLerp(ImMin(v_max, (TYPE)0), v_min, a);
				}
				else
				{
					// Positive: rescale to the positive range before powering
					float a;
					if (ImFabs(linear_zero_pos - 1.0f) > 1.e-6f)
						a = (clicked_t - linear_zero_pos) / (1.0f - linear_zero_pos);
					else
						a = clicked_t;
					a = ImPow(a, power);
					v_new = ImLerp(ImMax(v_min, (TYPE)0), v_max, a);
				}
			}
			else
			{
				// Linear slider
				if (is_decimal)
				{
					v_new = ImLerp(v_min, v_max, clicked_t);
				}
				else
				{
					// For integer values we want the clicking position to match the grab box so we round above
					// This code is carefully tuned to work with large values (e.g. high ranges of U64) while preserving this property..
					FLOATTYPE v_new_off_f = (v_max - v_min) * clicked_t;
					TYPE v_new_off_floor = (TYPE)(v_new_off_f);
					TYPE v_new_off_round = (TYPE)(v_new_off_f + (FLOATTYPE)0.5);
					if (!is_decimal && v_new_off_floor < v_new_off_round)
						v_new = v_min + v_new_off_round;
					else
						v_new = v_min + v_new_off_floor;
				}
			}

			// Round to user desired precision based on format string
			v_new = RoundScalarWithFormat<TYPE, SIGNEDTYPE>(format, data_type, v_new);

			// Apply result
			if (*v != v_new)
			{
				*v = v_new;
				value_changed = true;
			}
		}
	}

	// Draw
	float grab_t = SliderBehaviorCalcRatioFromValue<TYPE, FLOATTYPE>(data_type, *v, v_min, v_max, power, linear_zero_pos);
	if (!is_horizontal)
		grab_t = 1.0f - grab_t;
	const float grab_pos = ImLerp(slider_usable_pos_min, slider_usable_pos_max, grab_t);
	ImRect grab_bb;
	if (is_horizontal)
		grab_bb = ImRect(grab_pos, bb.Min.y, grab_pos, bb.Max.y);
	else
		grab_bb = ImRect(bb.Min.x, grab_pos, bb.Max.x, grab_pos);


	ImRect rect_bb = bb;
	if (Settings::Style::slider_thickness != 1.0f)
	{
		float shrink_amount = (float)(int)((rect_bb.Max.y - rect_bb.Min.y) * 0.5f * (1.0f - Settings::Style::slider_thickness));
		//bb.Min.y += shrink_amount;
		rect_bb.Max.y -= 2 * shrink_amount;
		grab_bb.Max.y -= 2 * shrink_amount;
	}
	if (v_origin == v_min) // O===========v-------M
	{
		RenderFrame(rect_bb.Min, rect_bb.Max, UI::GetColor(Settings::Style::slider_empty), 1.f, 0.f, Settings::Style::slider_border);
		window->DrawList->AddRectFilled(rect_bb.Min + ImVec2(1, 1), ImVec2(grab_bb.Max.x, rect_bb.Max.y) - ImVec2(1, 1), UI::GetColor(Settings::Style::slider_filled), 0.f);
	}
	else if (v_origin == v_max) // m-----------v=======O
	{
		//window->DrawList->AddRectFilled(rect_bb.Min, ImVec2(grab_bb.Min.x + (grab_bb.Max.x - grab_bb.Min.x) * 0.65f, rect_bb.Max.y), UI::GetColor(Settings::Style::slider_empty), 0.f);
		//window->DrawList->AddRectFilled(ImVec2(grab_bb.Max.x - (grab_bb.Max.x - grab_bb.Min.x) * 0.35f, rect_bb.Min.y), rect_bb.Max, UI::GetColor(Settings::Style::slider_filled), 0.f);
		RenderFrame(rect_bb.Min, rect_bb.Max, UI::GetColor(Settings::Style::slider_empty), 1.f, 0.f, Settings::Style::slider_border);
		window->DrawList->AddRectFilled(ImVec2(grab_bb.Max.x, rect_bb.Min.y) + ImVec2(1, 1), rect_bb.Max - ImVec2(1, 1), UI::GetColor(Settings::Style::slider_filled), 0.f);
	}
	else
	{
		RenderFrame(rect_bb.Min, rect_bb.Max, UI::GetColor(Settings::Style::slider_empty), 1.f, 0.f, Settings::Style::slider_border);

		float pos_t = SliderBehaviorCalcRatioFromValue<TYPE, FLOATTYPE>(data_type, v_origin, v_min, v_max, power, linear_zero_pos);
		if (!is_horizontal) //TODO
			pos_t = 1.0f - pos_t;
		const float origin_pos = ImLerp(slider_usable_pos_min, slider_usable_pos_max, pos_t);

		ImRect origin_bb;
		if (*v < v_origin) // m---v=====O----M
			origin_bb = ImRect(grab_pos, rect_bb.Min.y, origin_pos, rect_bb.Max.y);
		else // m---O====v---M
			origin_bb = ImRect(origin_pos, rect_bb.Min.y, grab_pos, rect_bb.Max.y);

		window->DrawList->AddRectFilled(origin_bb.Min + ImVec2(1, 1), origin_bb.Max - ImVec2(1, 1), UI::GetColor(Settings::Style::slider_filled), 0.f);
	}
	//window->DrawList->AddCircleFilled((grab_bb.Min + grab_bb.Max) * .5, Settings::Style::grab_radius, UI::GetColor(Settings::Style::slider_grab));

	// Display value using user-provided display format so user can add prefix/suffix/decorations to the value.
	char value_buf[64];
	const char* value_buf_end = value_buf + DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, v, format);
	ImVec2 value_size = ImGui::CalcTextSize(value_buf, value_buf_end);
	grab_bb.Min += ImVec2(-value_size.x/2, Settings::Style::slider_value_offset);
	// Clamping
	if (grab_bb.Min.x < rect_bb.Min.x)
		grab_bb.Min.x = rect_bb.Min.x;
	if (grab_bb.Min.x+value_size.x > rect_bb.Max.x)
		grab_bb.Min.x = rect_bb.Max.x-value_size.x;

	UI::RenderText(grab_bb.Min, value_buf, value_buf_end);

	return value_changed;
}

static bool SliderBehavior(const ImRect& frame_bb, ImGuiID id, ImGuiDataType data_type, void* v, const void* v_origin, const void* v_min, const void* v_max, const char* format, float power, ImGuiSliderFlags flags = 0)
{
	// Draw frame
	ImGui::RenderNavHighlight(frame_bb, id);

	switch (data_type)
	{
		case ImGuiDataType_S32:
			IM_ASSERT(*(const ImS32*)v_min >= IM_S32_MIN / 2 && *(const ImS32*)v_max <= IM_S32_MAX / 2);
			return SliderBehaviorT<ImS32, ImS32, float>(frame_bb, id, data_type, (ImS32*)v, *(const ImS32*)v_origin, *(const ImS32*)v_min, *(const ImS32*)v_max, format, power, flags);
		case ImGuiDataType_U32:
			IM_ASSERT(*(const ImU32*)v_min <= IM_U32_MAX / 2);
			return SliderBehaviorT<ImU32, ImS32, float>(frame_bb, id, data_type, (ImU32*)v, *(const ImU32*)v_origin, *(const ImU32*)v_min, *(const ImU32*)v_max, format, power, flags);
		case ImGuiDataType_S64:
			IM_ASSERT(*(const ImS64*)v_min >= IM_S64_MIN / 2 && *(const ImS64*)v_max <= IM_S64_MAX / 2);
			return SliderBehaviorT<ImS64, ImS64, double>(frame_bb, id, data_type, (ImS64*)v,*(const ImS64*)v_origin, *(const ImS64*)v_min, *(const ImS64*)v_max, format, power, flags);
		case ImGuiDataType_U64:
			IM_ASSERT(*(const ImU64*)v_min <= IM_U64_MAX / 2);
			return SliderBehaviorT<ImU64, ImS64, double>(frame_bb, id, data_type, (ImU64*)v, *(const ImU64*)v_origin, *(const ImU64*)v_min, *(const ImU64*)v_max, format, power, flags);
		case ImGuiDataType_Float:
			IM_ASSERT(*(const float*)v_min >= -FLT_MAX / 2.0f && *(const float*)v_max <= FLT_MAX / 2.0f);
			return SliderBehaviorT<float, float, float>(frame_bb, id, data_type, (float*)v, *(const float*)v_origin, *(const float*)v_min, *(const float*)v_max, format, power, flags);
		case ImGuiDataType_Double:
			IM_ASSERT(*(const double*)v_min >= -DBL_MAX / 2.0f && *(const double*)v_max <= DBL_MAX / 2.0f);
			return SliderBehaviorT<double, double, double>(frame_bb, id, data_type, (double*)v, *(const double*)v_origin, *(const double*)v_min, *(const double*)v_max, format, power, flags);
		case ImGuiDataType_COUNT:
			break;
	}
	IM_ASSERT(0);
	return false;
}

bool InputScalarAsWidgetReplacement(const ImRect& bb, ImGuiID id, const char* label, ImGuiDataType data_type, void* data_ptr, const char* format)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = ImGui::GetCurrentWindow();

    // Our replacement widget will override the focus ID (registered previously to allow for a TAB focus to happen)
    // On the first frame, g.ScalarAsInputTextId == 0, then on subsequent frames it becomes == id
	ImGui::SetActiveID(g.ScalarAsInputTextId, window);
    g.ActiveIdAllowNavDirFlags = (1 << ImGuiDir_Up) | (1 << ImGuiDir_Down);
	ImGui::SetHoveredID(0);
	ImGui::FocusableItemUnregister(window);

    char fmt_buf[32];
    char data_buf[32];
    format = ImParseFormatTrimDecorations(format, fmt_buf, IM_ARRAYSIZE(fmt_buf));
    DataTypeFormatString(data_buf, IM_ARRAYSIZE(data_buf), data_type, data_ptr, format);
    ImStrTrimBlanks(data_buf);
    ImGuiInputTextFlags flags = ImGuiInputTextFlags_AutoSelectAll | ((data_type == ImGuiDataType_Float || data_type == ImGuiDataType_Double) ? ImGuiInputTextFlags_CharsScientific : ImGuiInputTextFlags_CharsDecimal);
    bool value_changed = UI::InputText(label, data_buf, IM_ARRAYSIZE(data_buf), bb.GetSize(), flags);
    if (g.ScalarAsInputTextId == 0)     // First frame we started displaying the InputText widget
    {
        IM_ASSERT(g.ActiveId == id);    // InputText ID expected to match the Slider ID
        g.ScalarAsInputTextId = g.ActiveId;
		ImGui::SetHoveredID(id);
    }
    if (value_changed)
        return DataTypeApplyOpFromText(data_buf, g.InputTextState.InitialText.begin(), data_type, data_ptr, NULL);
    return false;
}

static bool SliderScalar(const char* label, ImGuiDataType data_type, void* v, void* v_origin, const void* v_min, const void* v_max, const char* format = NULL, float power = 1.f)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
	const f32 w = ImGui::GetContentRegionAvail().x;

	if (label_size.x > 0.0f)
	{
		UI::RenderText(window->DC.CursorPos, label);
		window->DC.CursorPos.y += label_size.y;
	}

	ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, style.FramePadding.y * 2.0f + 2.f /* spacing for the value */));
	const ImRect total_bb(frame_bb.Min, frame_bb.Max);

	// NB- we don't call ItemSize() yet because we may turn into a text edit box below
	if (!ImGui::ItemAdd(total_bb, id, &frame_bb))
	{
		frame_bb.Max.y += 8.f;
		ImGui::ItemSize(total_bb, style.FramePadding.y);
		return false;
	}

	// Default format string when passing NULL
	// Patch old "%.0f" format string to use "%d", read function comments for more details.
	IM_ASSERT(data_type >= 0 && data_type < ImGuiDataType_COUNT);
	if (format == NULL)
		format = GDataTypeInfo[data_type].PrintFmt;
	else if (data_type == ImGuiDataType_S32 && strcmp(format, "%d") != 0)
		format = PatchFormatStringFloatToInt(format);

	// Tabbing or CTRL-clicking on Slider turns it into an input box
	const bool tab_focus_requested = ImGui::FocusableItemRegister(window, id);
	const bool hovered = ImGui::ItemHoverable(frame_bb, id);
	if (tab_focus_requested || (hovered && g.IO.MouseClicked[0]) || g.NavActivateId == id || (g.NavInputId == id && g.ScalarAsInputTextId != id))
	{
		ImGui::SetActiveID(id, window);
		ImGui::SetFocusID(id, window);
		ImGui::FocusWindow(window);
		g.ActiveIdAllowNavDirFlags = (1 << ImGuiDir_Up) | (1 << ImGuiDir_Down);
	}

	// Actual slider behavior + render grab
	ImGui::ItemSize(total_bb, style.FramePadding.y);
	const bool value_changed = SliderBehavior(frame_bb, id, data_type, v, v_origin, v_min, v_max, format, power);
	if (value_changed)
		ImGui::MarkItemValueChanged(id);

	return value_changed;
}

bool UI::SliderFloat(const char* label, float* v, float v_origin, float v_min, float v_max, const char* display_format, float power)
{
	return SliderScalar(label, ImGuiDataType_Float, v, &v_origin, &v_min, &v_max, display_format, power);
}

bool UI::SliderInt(const char* label, int* v, int v_origin, int v_min, int v_max, const char* display_format)
{
	return SliderScalar(label, ImGuiDataType_S32, v, &v_origin, &v_min, &v_max, display_format);
}
// }}}

// {{{ Checkbox
bool UI::Checkbox(const char* label, bool* v)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

	const ImRect check_bb(window->DC.CursorPos + ImVec2(style.FramePadding.x * 1.5f, style.FramePadding.y * 1.5f), window->DC.CursorPos + ImVec2(label_size.y * .8f + style.FramePadding.x * 1.5f, label_size.y * .8f + style.FramePadding.y * 1.5f)); // We want a square shape to we use Y twice
	ImGui::ItemSize(check_bb, style.FramePadding.y);

	ImRect total_bb = check_bb;
	if (label_size.x > 0)
		ImGui::SameLine(0, style.ItemInnerSpacing.x);
	const ImRect text_bb(window->DC.CursorPos + ImVec2(style.FramePadding.y, style.FramePadding.y), window->DC.CursorPos + ImVec2(style.FramePadding.y, style.FramePadding.y) + label_size);
	if (label_size.x > 0)
	{
		ImGui::ItemSize(ImVec2(text_bb.GetWidth(), check_bb.GetHeight() + 8.f), style.FramePadding.y);
		total_bb = ImRect(ImMin(check_bb.Min, text_bb.Min), ImMax(check_bb.Max, text_bb.Max));
	}

	if (!ImGui::ItemAdd(total_bb, id))
		return false;

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
	if (pressed)
	{
		*v = !(*v);
		ImGui::MarkItemValueChanged(id);
	}

	ImGui::RenderNavHighlight(total_bb, id);
	RenderFrame(check_bb.Min, check_bb.Max, GetColor((held && hovered) ? Settings::Style::checkbox_bg[3] : hovered ? Settings::Style::checkbox_bg[2] :
																							  *v		  ? Settings::Style::checkbox_bg[1] :
																											  Settings::Style::checkbox_bg[0]),
				1.f, 0.f, GetColor(Settings::Style::checkbox_border));
	if (*v)
		RenderRectGradient(check_bb.Min + ImVec2(1, 1), check_bb.Max - ImVec2(1, 1), GetColor(Settings::Style::checkmark), GetColor(Darken(Settings::Style::checkmark, 1)));

	if (label_size.x > 0.0f)
		RenderText(text_bb.Min + ImVec2(Settings::Style::checkbox_spacing, 0), label);

	return pressed;
}
// }}}

// {{{ InputText
bool UI::InputText(const char* label, char* buf, size_t buf_size, ImVec2 size_arg, ImGuiInputTextFlags flags, ImGuiTextEditCallback callback, void* user_data)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	IM_ASSERT(!((flags & ImGuiInputTextFlags_CallbackHistory) && (flags & ImGuiInputTextFlags_Multiline))); // Can't use both together (they both use up/down keys)
	IM_ASSERT(!((flags & ImGuiInputTextFlags_CallbackCompletion) && (flags & ImGuiInputTextFlags_AllowTabInput))); // Can't use both together (they both use tab key)

	ImGuiContext& g = *GImGui;
	const ImGuiIO& io = g.IO;
	const ImGuiStyle& style = g.Style;

	const bool is_multiline = (flags & ImGuiInputTextFlags_Multiline) != 0;
	const bool is_editable = (flags & ImGuiInputTextFlags_ReadOnly) == 0;
	const bool is_password = (flags & ImGuiInputTextFlags_Password) != 0;
	const bool is_undoable = (flags & ImGuiInputTextFlags_NoUndoRedo) == 0;

	if (is_multiline) // Open group before calling GetID() because groups tracks id created within their scope,
		ImGui::BeginGroup();
	const ImGuiID id = window->GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
	ImVec2 size = ImGui::CalcItemSize(size_arg, ImGui::CalcItemWidth(), (is_multiline ? ImGui::GetTextLineHeight() * 8.0f : label_size.y) + style.FramePadding.y * 2.0f); // Arbitrary default of 8 lines high for multi-line
	const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size);
	const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? (style.ItemInnerSpacing.x + label_size.x) : 0.0f, 0.0f));

	ImGuiWindow* draw_window = window;
	if (is_multiline)
	{
		ImGui::ItemAdd(total_bb, id, &frame_bb);
		if (!ImGui::BeginChildFrame(id, frame_bb.GetSize()))
		{
			ImGui::EndChildFrame();
			ImGui::EndGroup();
			return false;
		}
		draw_window = ImGui::GetCurrentWindow();
		draw_window->DC.NavLayerActiveMaskNext |= draw_window->DC.NavLayerCurrentMask; // This is to ensure that EndChild() will display a navigation highlight
		size.x -= draw_window->ScrollbarSizes.x;
	}
	else
	{
		ImGui::ItemSize(total_bb, style.FramePadding.y);
		if (!ImGui::ItemAdd(total_bb, id, &frame_bb))
			return false;
	}
	const bool hovered = ImGui::ItemHoverable(frame_bb, id);
	if (hovered)
		g.MouseCursor = ImGuiMouseCursor_TextInput;

	// Password pushes a temporary font with only a fallback glyph
	if (is_password)
	{
		const ImFontGlyph* glyph = g.Font->FindGlyph('*');
		ImFont* password_font = &g.InputTextPasswordFont;
		password_font->FontSize = g.Font->FontSize;
		password_font->Scale = g.Font->Scale;
		password_font->DisplayOffset = g.Font->DisplayOffset;
		password_font->Ascent = g.Font->Ascent;
		password_font->Descent = g.Font->Descent;
		password_font->ContainerAtlas = g.Font->ContainerAtlas;
		password_font->FallbackGlyph = glyph;
		password_font->FallbackAdvanceX = glyph->AdvanceX;
		IM_ASSERT(password_font->Glyphs.empty() && password_font->IndexAdvanceX.empty() && password_font->IndexLookup.empty());
		ImGui::PushFont(password_font);
	}

	// NB: we are only allowed to access 'edit_state' if we are the active widget.
	ImGuiTextEditState& edit_state = g.InputTextState;

	const bool focus_requested = ImGui::FocusableItemRegister(window, id, (flags & (ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_AllowTabInput)) == 0); // Using completion callback disable keyboard tabbing
	const bool focus_requested_by_code = focus_requested && (window->FocusIdxAllCounter == window->FocusIdxAllRequestCurrent);
	const bool focus_requested_by_tab = focus_requested && !focus_requested_by_code;

	const bool user_clicked = hovered && io.MouseClicked[0];
	const bool user_scrolled = is_multiline && g.ActiveId == 0 && edit_state.Id == id && g.ActiveIdPreviousFrame == draw_window->GetIDNoKeepAlive("#SCROLLY");
	const bool user_nav_input_start = (g.ActiveId != id) && ((g.NavInputId == id) || (g.NavActivateId == id && g.NavInputSource == ImGuiInputSource_NavKeyboard));

	bool clear_active_id = false;

	bool select_all = (g.ActiveId != id) && ((flags & ImGuiInputTextFlags_AutoSelectAll) != 0 || user_nav_input_start) && (!is_multiline);
	if (focus_requested || user_clicked || user_scrolled || user_nav_input_start)
	{
		if (g.ActiveId != id)
		{
			// Start edition
			// Take a copy of the initial buffer value (both in original UTF-8 format and converted to wchar)
			// From the moment we focused we are ignoring the content of 'buf' (unless we are in read-only mode)
			const int prev_len_w = edit_state.CurLenW;
			edit_state.Text.resize(buf_size + 1); // wchar count <= UTF-8 count. we use +1 to make sure that .Data isn't NULL so it doesn't crash.
			edit_state.InitialText.resize(buf_size + 1); // UTF-8. we use +1 to make sure that .Data isn't NULL so it doesn't crash.
			ImStrncpy(edit_state.InitialText.Data, buf, edit_state.InitialText.Size);
			const char* buf_end = NULL;
			edit_state.CurLenW = ImTextStrFromUtf8(edit_state.Text.Data, edit_state.Text.Size, buf, NULL, &buf_end);
			edit_state.CurLenA = (int)(buf_end - buf); // We can't get the result from ImFormatString() above because it is not UTF-8 aware. Here we'll cut off malformed UTF-8.
			edit_state.CursorAnimReset();

			// Preserve cursor position and undo/redo stack if we come back to same widget
			// FIXME: We should probably compare the whole buffer to be on the safety side. Comparing buf (utf8) and edit_state.Text (wchar).
			const bool recycle_state = (edit_state.Id == id) && (prev_len_w == edit_state.CurLenW);
			if (recycle_state)
			{
				// Recycle existing cursor/selection/undo stack but clamp position
				// Note a single mouse click will override the cursor/position immediately by calling stb_textedit_click handler.
				edit_state.CursorClamp();
			}
			else
			{
				edit_state.Id = id;
				edit_state.ScrollX = 0.0f;
				stb_textedit_initialize_state(&edit_state.StbState, !is_multiline);
				if (!is_multiline && focus_requested_by_code)
					select_all = true;
			}
			if (flags & ImGuiInputTextFlags_AlwaysInsertMode)
				edit_state.StbState.insert_mode = true;
			if (!is_multiline && (focus_requested_by_tab || (user_clicked && io.KeyCtrl)))
				select_all = true;
		}
		ImGui::SetActiveID(id, window);
		ImGui::SetFocusID(id, window);
		ImGui::FocusWindow(window);
		if (!is_multiline && !(flags & ImGuiInputTextFlags_CallbackHistory))
			g.ActiveIdAllowNavDirFlags |= ((1 << ImGuiDir_Up) | (1 << ImGuiDir_Down));
	}
	else if (io.MouseClicked[0])
	{
		// Release focus when we click outside
		clear_active_id = true;
	}

	bool value_changed = false;
	bool enter_pressed = false;

	if (g.ActiveId == id)
	{
		if (!is_editable && !g.ActiveIdIsJustActivated)
		{
			// When read-only we always use the live data passed to the function
			edit_state.Text.resize(buf_size + 1);
			const char* buf_end = NULL;
			edit_state.CurLenW = ImTextStrFromUtf8(edit_state.Text.Data, edit_state.Text.Size, buf, NULL, &buf_end);
			edit_state.CurLenA = (int)(buf_end - buf);
			edit_state.CursorClamp();
		}

		edit_state.BufSizeA = buf_size;

		// Although we are active we don't prevent mouse from hovering other elements unless we are interacting right now with the widget.
		// Down the line we should have a cleaner library-wide concept of Selected vs Active.
		g.ActiveIdAllowOverlap = !io.MouseDown[0];
		g.WantTextInputNextFrame = 1;

		// Edit in progress
		const float mouse_x = (io.MousePos.x - frame_bb.Min.x - style.FramePadding.x) + edit_state.ScrollX;
		const float mouse_y = (is_multiline ? (io.MousePos.y - draw_window->DC.CursorPos.y - style.FramePadding.y) : (g.FontSize * 0.5f));

		const bool is_osx = io.OptMacOSXBehaviors;
		if (select_all || (hovered && !is_osx && io.MouseDoubleClicked[0]))
		{
			edit_state.SelectAll();
			edit_state.SelectedAllMouseLock = true;
		}
		else if (hovered && is_osx && io.MouseDoubleClicked[0])
		{
			// Double-click select a word only, OS X style (by simulating keystrokes)
			edit_state.OnKeyPressed(STB_TEXTEDIT_K_WORDLEFT);
			edit_state.OnKeyPressed(STB_TEXTEDIT_K_WORDRIGHT | STB_TEXTEDIT_K_SHIFT);
		}
		else if (io.MouseClicked[0] && !edit_state.SelectedAllMouseLock)
		{
			if (hovered)
			{
				stb_textedit_click(&edit_state, &edit_state.StbState, mouse_x, mouse_y);
				edit_state.CursorAnimReset();
			}
		}
		else if (io.MouseDown[0] && !edit_state.SelectedAllMouseLock && (io.MouseDelta.x != 0.0f || io.MouseDelta.y != 0.0f))
		{
			stb_textedit_drag(&edit_state, &edit_state.StbState, mouse_x, mouse_y);
			edit_state.CursorAnimReset();
			edit_state.CursorFollow = true;
		}
		if (edit_state.SelectedAllMouseLock && !io.MouseDown[0])
			edit_state.SelectedAllMouseLock = false;

		if (io.InputCharacters[0])
		{
			// Process text input (before we check for Return because using some IME will effectively send a Return?)
			// We ignore CTRL inputs, but need to allow ALT+CTRL as some keyboards (e.g. German) use AltGR (which _is_ Alt+Ctrl) to input certain characters.
			bool ignore_inputs = (io.KeyCtrl && !io.KeyAlt) || (is_osx && io.KeySuper);
			if (!ignore_inputs && is_editable && !user_nav_input_start)
				for (int n = 0; n < IM_ARRAYSIZE(io.InputCharacters) && io.InputCharacters[n]; n++)
				{
					// Insert character if they pass filtering
					unsigned int c = (unsigned int)io.InputCharacters[n];
					if (InputTextFilterCharacter(&c, flags, callback, user_data))
						edit_state.OnKeyPressed((int)c);
				}

			// Consume characters
			memset(g.IO.InputCharacters, 0, sizeof(g.IO.InputCharacters));
		}
	}

	bool cancel_edit = false;
	if (g.ActiveId == id && !g.ActiveIdIsJustActivated && !clear_active_id)
	{
		// Handle key-presses
		const int k_mask = (io.KeyShift ? STB_TEXTEDIT_K_SHIFT : 0);
		const bool is_osx = io.OptMacOSXBehaviors;
		const bool is_shortcut_key = (is_osx ? (io.KeySuper && !io.KeyCtrl) : (io.KeyCtrl && !io.KeySuper)) && !io.KeyAlt && !io.KeyShift; // OS X style: Shortcuts using Cmd/Super instead of Ctrl
		const bool is_osx_shift_shortcut = is_osx && io.KeySuper && io.KeyShift && !io.KeyCtrl && !io.KeyAlt;
		const bool is_wordmove_key_down = is_osx ? io.KeyAlt : io.KeyCtrl; // OS X style: Text editing cursor movement using Alt instead of Ctrl
		const bool is_startend_key_down = is_osx && io.KeySuper && !io.KeyCtrl && !io.KeyAlt; // OS X style: Line/Text Start and End using Cmd+Arrows instead of Home/End
		const bool is_ctrl_key_only = io.KeyCtrl && !io.KeyShift && !io.KeyAlt && !io.KeySuper;
		const bool is_shift_key_only = io.KeyShift && !io.KeyCtrl && !io.KeyAlt && !io.KeySuper;

		const bool is_cut = ((is_shortcut_key && IsKeyPressedMap(ImGuiKey_X)) || (is_shift_key_only && IsKeyPressedMap(ImGuiKey_Delete))) && is_editable && !is_password && (!is_multiline || edit_state.HasSelection());
		const bool is_copy = ((is_shortcut_key && IsKeyPressedMap(ImGuiKey_C)) || (is_ctrl_key_only && IsKeyPressedMap(ImGuiKey_Insert))) && !is_password && (!is_multiline || edit_state.HasSelection());
		const bool is_paste = ((is_shortcut_key && IsKeyPressedMap(ImGuiKey_V)) || (is_shift_key_only && IsKeyPressedMap(ImGuiKey_Insert))) && is_editable;
		const bool is_undo = ((is_shortcut_key && IsKeyPressedMap(ImGuiKey_Z)) && is_editable && is_undoable);
		const bool is_redo = ((is_shortcut_key && IsKeyPressedMap(ImGuiKey_Y)) || (is_osx_shift_shortcut && IsKeyPressedMap(ImGuiKey_Z))) && is_editable && is_undoable;

		if (IsKeyPressedMap(ImGuiKey_LeftArrow))
		{
			edit_state.OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_LINESTART : is_wordmove_key_down ? STB_TEXTEDIT_K_WORDLEFT :
																												STB_TEXTEDIT_K_LEFT) |
									k_mask);
		}
		else if (IsKeyPressedMap(ImGuiKey_RightArrow))
		{
			edit_state.OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_LINEEND : is_wordmove_key_down ? STB_TEXTEDIT_K_WORDRIGHT :
																											  STB_TEXTEDIT_K_RIGHT) |
									k_mask);
		}
		else if (IsKeyPressedMap(ImGuiKey_UpArrow) && is_multiline)
		{
			if (io.KeyCtrl)
				SetWindowScrollY(draw_window, ImMax(draw_window->Scroll.y - g.FontSize, 0.0f));
			else
				edit_state.OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_TEXTSTART : STB_TEXTEDIT_K_UP) | k_mask);
		}
		else if (IsKeyPressedMap(ImGuiKey_DownArrow) && is_multiline)
		{
			if (io.KeyCtrl)
				SetWindowScrollY(draw_window, ImMin(draw_window->Scroll.y + g.FontSize, ImGui::GetScrollMaxY()));
			else
				edit_state.OnKeyPressed((is_startend_key_down ? STB_TEXTEDIT_K_TEXTEND : STB_TEXTEDIT_K_DOWN) | k_mask);
		}
		else if (IsKeyPressedMap(ImGuiKey_Home))
		{
			edit_state.OnKeyPressed(io.KeyCtrl ? STB_TEXTEDIT_K_TEXTSTART | k_mask : STB_TEXTEDIT_K_LINESTART | k_mask);
		}
		else if (IsKeyPressedMap(ImGuiKey_End))
		{
			edit_state.OnKeyPressed(io.KeyCtrl ? STB_TEXTEDIT_K_TEXTEND | k_mask : STB_TEXTEDIT_K_LINEEND | k_mask);
		}
		else if (IsKeyPressedMap(ImGuiKey_Delete) && is_editable)
		{
			edit_state.OnKeyPressed(STB_TEXTEDIT_K_DELETE | k_mask);
		}
		else if (IsKeyPressedMap(ImGuiKey_Backspace) && is_editable)
		{
			if (!edit_state.HasSelection())
			{
				if (is_wordmove_key_down)
					edit_state.OnKeyPressed(STB_TEXTEDIT_K_WORDLEFT | STB_TEXTEDIT_K_SHIFT);
				else if (is_osx && io.KeySuper && !io.KeyAlt && !io.KeyCtrl)
					edit_state.OnKeyPressed(STB_TEXTEDIT_K_LINESTART | STB_TEXTEDIT_K_SHIFT);
			}
			edit_state.OnKeyPressed(STB_TEXTEDIT_K_BACKSPACE | k_mask);
		}
		else if (IsKeyPressedMap(ImGuiKey_Enter))
		{
			bool ctrl_enter_for_new_line = (flags & ImGuiInputTextFlags_CtrlEnterForNewLine) != 0;
			if (!is_multiline || (ctrl_enter_for_new_line && !io.KeyCtrl) || (!ctrl_enter_for_new_line && io.KeyCtrl))
			{
				enter_pressed = clear_active_id = true;
			}
			else if (is_editable)
			{
				unsigned int c = '\n'; // Insert new line
				if (InputTextFilterCharacter(&c, flags, callback, user_data))
					edit_state.OnKeyPressed((int)c);
			}
		}
		else if ((flags & ImGuiInputTextFlags_AllowTabInput) && IsKeyPressedMap(ImGuiKey_Tab) && !io.KeyCtrl && !io.KeyShift && !io.KeyAlt && is_editable)
		{
			unsigned int c = '\t'; // Insert TAB
			if (InputTextFilterCharacter(&c, flags, callback, user_data))
				edit_state.OnKeyPressed((int)c);
		}
		else if (IsKeyPressedMap(ImGuiKey_Escape))
		{
			clear_active_id = cancel_edit = true;
		}
		else if (is_undo || is_redo)
		{
			edit_state.OnKeyPressed(is_undo ? STB_TEXTEDIT_K_UNDO : STB_TEXTEDIT_K_REDO);
			edit_state.ClearSelection();
		}
		else if (is_shortcut_key && IsKeyPressedMap(ImGuiKey_A))
		{
			edit_state.SelectAll();
			edit_state.CursorFollow = true;
		}
		else if (is_cut || is_copy)
		{
			// Cut, Copy
			if (io.SetClipboardTextFn)
			{
				const int ib = edit_state.HasSelection() ? ImMin(edit_state.StbState.select_start, edit_state.StbState.select_end) : 0;
				const int ie = edit_state.HasSelection() ? ImMax(edit_state.StbState.select_start, edit_state.StbState.select_end) : edit_state.CurLenW;
				edit_state.TempTextBuffer.resize((ie - ib) * 4 + 1);
				ImTextStrToUtf8(edit_state.TempTextBuffer.Data, edit_state.TempTextBuffer.Size, edit_state.Text.Data + ib, edit_state.Text.Data + ie);
				ImGui::SetClipboardText(edit_state.TempTextBuffer.Data);
			}
			if (is_cut)
			{
				if (!edit_state.HasSelection())
					edit_state.SelectAll();
				edit_state.CursorFollow = true;
				stb_textedit_cut(&edit_state, &edit_state.StbState);
			}
		}
		else if (is_paste)
		{
			if (const char* clipboard = ImGui::GetClipboardText())
			{
				// Filter pasted buffer
				const int clipboard_len = (int)strlen(clipboard);
				ImWchar* clipboard_filtered = (ImWchar*)ImGui::MemAlloc((clipboard_len + 1) * sizeof(ImWchar));
				int clipboard_filtered_len = 0;
				for (const char* s = clipboard; *s;)
				{
					unsigned int c;
					s += ImTextCharFromUtf8(&c, s, NULL);
					if (c == 0)
						break;
					if (c >= 0x10000 || !InputTextFilterCharacter(&c, flags, callback, user_data))
						continue;
					clipboard_filtered[clipboard_filtered_len++] = (ImWchar)c;
				}
				clipboard_filtered[clipboard_filtered_len] = 0;
				if (clipboard_filtered_len > 0) // If everything was filtered, ignore the pasting operation
				{
					stb_textedit_paste(&edit_state, &edit_state.StbState, clipboard_filtered, clipboard_filtered_len);
					edit_state.CursorFollow = true;
				}
				ImGui::MemFree(clipboard_filtered);
			}
		}
	}

	if (g.ActiveId == id)
	{
		if (cancel_edit)
		{
			// Restore initial value. Only return true if restoring to the initial value changes the current buffer contents.
			if (is_editable && strncmp(buf, edit_state.InitialText.Data, buf_size) != 0)
			{
				ImStrncpy(buf, edit_state.InitialText.Data, buf_size);
				value_changed = true;
			}
		}

		// When using 'ImGuiInputTextFlags_EnterReturnsTrue' as a special case we reapply the live buffer back to the input buffer before clearing ActiveId, even though strictly speaking it wasn't modified on this frame.
		// If we didn't do that, code like InputInt() with ImGuiInputTextFlags_EnterReturnsTrue would fail. Also this allows the user to use InputText() with ImGuiInputTextFlags_EnterReturnsTrue without maintaining any user-side storage.
		bool apply_edit_back_to_user_buffer = !cancel_edit || (enter_pressed && (flags & ImGuiInputTextFlags_EnterReturnsTrue) != 0);
		if (apply_edit_back_to_user_buffer)
		{
			// Apply new value immediately - copy modified buffer back
			// Note that as soon as the input box is active, the in-widget value gets priority over any underlying modification of the input buffer
			// FIXME: We actually always render 'buf' when calling DrawList->AddText, making the comment above incorrect.
			// FIXME-OPT: CPU waste to do this every time the widget is active, should mark dirty state from the stb_textedit callbacks.
			if (is_editable)
			{
				edit_state.TempTextBuffer.resize(edit_state.Text.Size * 4);
				ImTextStrToUtf8(edit_state.TempTextBuffer.Data, edit_state.TempTextBuffer.Size, edit_state.Text.Data, NULL);
			}

			// User callback
			if ((flags & (ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory | ImGuiInputTextFlags_CallbackAlways)) != 0)
			{
				IM_ASSERT(callback != NULL);

				// The reason we specify the usage semantic (Completion/History) is that Completion needs to disable keyboard TABBING at the moment.
				ImGuiInputTextFlags event_flag = 0;
				ImGuiKey event_key = ImGuiKey_COUNT;
				if ((flags & ImGuiInputTextFlags_CallbackCompletion) != 0 && IsKeyPressedMap(ImGuiKey_Tab))
				{
					event_flag = ImGuiInputTextFlags_CallbackCompletion;
					event_key = ImGuiKey_Tab;
				}
				else if ((flags & ImGuiInputTextFlags_CallbackHistory) != 0 && IsKeyPressedMap(ImGuiKey_UpArrow))
				{
					event_flag = ImGuiInputTextFlags_CallbackHistory;
					event_key = ImGuiKey_UpArrow;
				}
				else if ((flags & ImGuiInputTextFlags_CallbackHistory) != 0 && IsKeyPressedMap(ImGuiKey_DownArrow))
				{
					event_flag = ImGuiInputTextFlags_CallbackHistory;
					event_key = ImGuiKey_DownArrow;
				}
				else if (flags & ImGuiInputTextFlags_CallbackAlways)
					event_flag = ImGuiInputTextFlags_CallbackAlways;

				if (event_flag)
				{
					ImGuiTextEditCallbackData callback_data;
					memset(&callback_data, 0, sizeof(ImGuiTextEditCallbackData));
					callback_data.EventFlag = event_flag;
					callback_data.Flags = flags;
					callback_data.UserData = user_data;
					callback_data.ReadOnly = !is_editable;

					callback_data.EventKey = event_key;
					callback_data.Buf = edit_state.TempTextBuffer.Data;
					callback_data.BufTextLen = edit_state.CurLenA;
					callback_data.BufSize = edit_state.BufSizeA;
					callback_data.BufDirty = false;

					// We have to convert from wchar-positions to UTF-8-positions, which can be pretty slow (an incentive to ditch the ImWchar buffer, see https://github.com/nothings/stb/issues/188)
					ImWchar* text = edit_state.Text.Data;
					const int utf8_cursor_pos = callback_data.CursorPos = ImTextCountUtf8BytesFromStr(text, text + edit_state.StbState.cursor);
					const int utf8_selection_start = callback_data.SelectionStart = ImTextCountUtf8BytesFromStr(text, text + edit_state.StbState.select_start);
					const int utf8_selection_end = callback_data.SelectionEnd = ImTextCountUtf8BytesFromStr(text, text + edit_state.StbState.select_end);

					// Call user code
					callback(&callback_data);

					// Read back what user may have modified
					IM_ASSERT(callback_data.Buf == edit_state.TempTextBuffer.Data); // Invalid to modify those fields
					IM_ASSERT(callback_data.BufSize == edit_state.BufSizeA);
					IM_ASSERT(callback_data.Flags == flags);
					if (callback_data.CursorPos != utf8_cursor_pos)
						edit_state.StbState.cursor = ImTextCountCharsFromUtf8(callback_data.Buf, callback_data.Buf + callback_data.CursorPos);
					if (callback_data.SelectionStart != utf8_selection_start)
						edit_state.StbState.select_start = ImTextCountCharsFromUtf8(callback_data.Buf, callback_data.Buf + callback_data.SelectionStart);
					if (callback_data.SelectionEnd != utf8_selection_end)
						edit_state.StbState.select_end = ImTextCountCharsFromUtf8(callback_data.Buf, callback_data.Buf + callback_data.SelectionEnd);
					if (callback_data.BufDirty)
					{
						IM_ASSERT(callback_data.BufTextLen == (int)strlen(callback_data.Buf)); // You need to maintain BufTextLen if you change the text!
						edit_state.CurLenW = ImTextStrFromUtf8(edit_state.Text.Data, edit_state.Text.Size, callback_data.Buf, NULL);
						edit_state.CurLenA = callback_data.BufTextLen; // Assume correct length and valid UTF-8 from user, saves us an extra strlen()
						edit_state.CursorAnimReset();
					}
				}
			}

			// Copy back to user buffer
			if (is_editable && strcmp(edit_state.TempTextBuffer.Data, buf) != 0)
			{
				ImStrncpy(buf, edit_state.TempTextBuffer.Data, buf_size);
				value_changed = true;
			}
		}
	}

	// Release active ID at the end of the function (so e.g. pressing Return still does a final application of the value)
	if (clear_active_id && g.ActiveId == id)
		ImGui::ClearActiveID();

	// Render
	// Select which buffer we are going to display. When ImGuiInputTextFlags_NoLiveEdit is set 'buf' might still be the old value. We set buf to NULL to prevent accidental usage from now on.
	const char* buf_display = (g.ActiveId == id && is_editable) ? edit_state.TempTextBuffer.Data : buf;
	buf = NULL;

	if (!is_multiline)
	{
		ImGui::RenderNavHighlight(frame_bb, id);
		RenderFrame(frame_bb.Min, frame_bb.Max, Settings::Style::textinput_bg, 1.f, 0.f, Settings::Style::textinput_border);
	}

	const ImVec4 clip_rect(frame_bb.Min.x, frame_bb.Min.y, frame_bb.Min.x + size.x, frame_bb.Min.y + size.y); // Not using frame_bb.Max because we have adjusted size
	ImVec2 render_pos = is_multiline ? draw_window->DC.CursorPos : frame_bb.Min + style.FramePadding;
	ImVec2 text_size(0.f, 0.f);
	const bool is_currently_scrolling = (edit_state.Id == id && is_multiline && g.ActiveId == draw_window->GetIDNoKeepAlive("#SCROLLY"));
	if (g.ActiveId == id || is_currently_scrolling)
	{
		edit_state.CursorAnim += io.DeltaTime;

		// This is going to be messy. We need to:
		// - Display the text (this alone can be more easily clipped)
		// - Handle scrolling, highlight selection, display cursor (those all requires some form of 1d->2d cursor position calculation)
		// - Measure text height (for scrollbar)
		// We are attempting to do most of that in **one main pass** to minimize the computation cost (non-negligible for large amount of text) + 2nd pass for selection rendering (we could merge them by an extra refactoring effort)
		// FIXME: This should occur on buf_display but we'd need to maintain cursor/select_start/select_end for UTF-8.
		const ImWchar* text_begin = edit_state.Text.Data;
		ImVec2 cursor_offset, select_start_offset;

		{
			// Count lines + find lines numbers straddling 'cursor' and 'select_start' position.
			const ImWchar* searches_input_ptr[2];
			searches_input_ptr[0] = text_begin + edit_state.StbState.cursor;
			searches_input_ptr[1] = NULL;
			int searches_remaining = 1;
			int searches_result_line_number[2] = { -1, -999 };
			if (edit_state.StbState.select_start != edit_state.StbState.select_end)
			{
				searches_input_ptr[1] = text_begin + ImMin(edit_state.StbState.select_start, edit_state.StbState.select_end);
				searches_result_line_number[1] = -1;
				searches_remaining++;
			}

			// Iterate all lines to find our line numbers
			// In multi-line mode, we never exit the loop until all lines are counted, so add one extra to the searches_remaining counter.
			searches_remaining += is_multiline ? 1 : 0;
			int line_count = 0;
			for (const ImWchar* s = text_begin; *s != 0; s++)
				if (*s == '\n')
				{
					line_count++;
					if (searches_result_line_number[0] == -1 && s >= searches_input_ptr[0])
					{
						searches_result_line_number[0] = line_count;
						if (--searches_remaining <= 0)
							break;
					}
					if (searches_result_line_number[1] == -1 && s >= searches_input_ptr[1])
					{
						searches_result_line_number[1] = line_count;
						if (--searches_remaining <= 0)
							break;
					}
				}
			line_count++;
			if (searches_result_line_number[0] == -1)
				searches_result_line_number[0] = line_count;
			if (searches_result_line_number[1] == -1)
				searches_result_line_number[1] = line_count;

			// Calculate 2d position by finding the beginning of the line and measuring distance
			cursor_offset.x = InputTextCalcTextSizeW(ImStrbolW(searches_input_ptr[0], text_begin), searches_input_ptr[0]).x;
			cursor_offset.y = searches_result_line_number[0] * g.FontSize;
			if (searches_result_line_number[1] >= 0)
			{
				select_start_offset.x = InputTextCalcTextSizeW(ImStrbolW(searches_input_ptr[1], text_begin), searches_input_ptr[1]).x;
				select_start_offset.y = searches_result_line_number[1] * g.FontSize;
			}

			// Store text height (note that we haven't calculated text width at all, see GitHub issues #383, #1224)
			if (is_multiline)
				text_size = ImVec2(size.x, line_count * g.FontSize);
		}

		// Scroll
		if (edit_state.CursorFollow)
		{
			// Horizontal scroll in chunks of quarter width
			if (!(flags & ImGuiInputTextFlags_NoHorizontalScroll))
			{
				const float scroll_increment_x = size.x * 0.25f;
				if (cursor_offset.x < edit_state.ScrollX)
					edit_state.ScrollX = (float)(int)ImMax(0.0f, cursor_offset.x - scroll_increment_x);
				else if (cursor_offset.x - size.x >= edit_state.ScrollX)
					edit_state.ScrollX = (float)(int)(cursor_offset.x - size.x + scroll_increment_x);
			}
			else
			{
				edit_state.ScrollX = 0.0f;
			}

			// Vertical scroll
			if (is_multiline)
			{
				float scroll_y = draw_window->Scroll.y;
				if (cursor_offset.y - g.FontSize < scroll_y)
					scroll_y = ImMax(0.0f, cursor_offset.y - g.FontSize);
				else if (cursor_offset.y - size.y >= scroll_y)
					scroll_y = cursor_offset.y - size.y;
				draw_window->DC.CursorPos.y += (draw_window->Scroll.y - scroll_y); // To avoid a frame of lag
				draw_window->Scroll.y = scroll_y;
				render_pos.y = draw_window->DC.CursorPos.y;
			}
		}
		edit_state.CursorFollow = false;
		const ImVec2 render_scroll = ImVec2(edit_state.ScrollX, 0.0f);

		// Draw selection
		if (edit_state.StbState.select_start != edit_state.StbState.select_end)
		{
			const ImWchar* text_selected_begin = text_begin + ImMin(edit_state.StbState.select_start, edit_state.StbState.select_end);
			const ImWchar* text_selected_end = text_begin + ImMax(edit_state.StbState.select_start, edit_state.StbState.select_end);

			float bg_offy_up = is_multiline ? 0.0f : -1.0f; // FIXME: those offsets should be part of the style? they don't play so well with multi-line selection.
			float bg_offy_dn = is_multiline ? 0.0f : 2.0f;
			ImU32 bg_color = ImGui::GetColorU32(ImGuiCol_TextSelectedBg);
			ImVec2 rect_pos = render_pos + select_start_offset - render_scroll;
			for (const ImWchar* p = text_selected_begin; p < text_selected_end;)
			{
				if (rect_pos.y > clip_rect.w + g.FontSize)
					break;
				if (rect_pos.y < clip_rect.y)
				{
					while (p < text_selected_end)
						if (*p++ == '\n')
							break;
				}
				else
				{
					ImVec2 rect_size = InputTextCalcTextSizeW(p, text_selected_end, &p, NULL, true);
					if (rect_size.x <= 0.0f)
						rect_size.x = (float)(int)(g.Font->GetCharAdvance((unsigned short)' ') * 0.50f); // So we can see selected empty lines
					ImRect rect(rect_pos + ImVec2(0.0f, bg_offy_up - g.FontSize), rect_pos + ImVec2(rect_size.x, bg_offy_dn));
					rect.ClipWith(clip_rect);
					if (rect.Overlaps(clip_rect))
						draw_window->DrawList->AddRectFilled(rect.Min, rect.Max, bg_color);
				}
				rect_pos.x = render_pos.x - render_scroll.x;
				rect_pos.y += g.FontSize;
			}
		}

		draw_window->DrawList->AddText(g.Font, g.FontSize, render_pos - render_scroll, Settings::Style::textinput_text, buf_display, buf_display + edit_state.CurLenA, 0.0f, is_multiline ? NULL : &clip_rect);

		// Draw blinking cursor
		bool cursor_is_visible = (!g.IO.OptCursorBlink) || (g.InputTextState.CursorAnim <= 0.0f) || ImFmod(g.InputTextState.CursorAnim, 1.20f) <= 0.80f;
		ImVec2 cursor_screen_pos = render_pos + cursor_offset - render_scroll;
		ImRect cursor_screen_rect(cursor_screen_pos.x, cursor_screen_pos.y - g.FontSize + 0.5f, cursor_screen_pos.x + 1.0f, cursor_screen_pos.y - 1.5f);
		if (cursor_is_visible && cursor_screen_rect.Overlaps(clip_rect))
			draw_window->DrawList->AddLine(cursor_screen_rect.Min, cursor_screen_rect.GetBL(), Settings::Style::textinput_text);

		// Notify OS of text input position for advanced IME (-1 x offset so that Windows IME can cover our cursor. Bit of an extra nicety.)
		if (is_editable)
			g.PlatformImePos = ImVec2(cursor_screen_pos.x - 1, cursor_screen_pos.y - g.FontSize);
	}
	else
	{
		// Render text only
		const char* buf_end = NULL;
		if (is_multiline)
			text_size = ImVec2(size.x, InputTextCalcTextLenAndLineCount(buf_display, &buf_end) * g.FontSize); // We don't need width
		draw_window->DrawList->AddText(g.Font, g.FontSize, render_pos, Settings::Style::textinput_text, buf_display, buf_end, 0.0f, is_multiline ? NULL : &clip_rect);
	}

	if (is_multiline)
	{
		ImGui::Dummy(text_size + ImVec2(0.0f, g.FontSize)); // Always add room to scroll an extra line
		ImGui::EndChildFrame();
		ImGui::EndGroup();
	}

	if (is_password)
		ImGui::PopFont();

	// Log as text
	if (g.LogEnabled && !is_password)
		LogRenderedText(&render_pos, buf_display, NULL);

	if (label_size.x > 0)
		RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

	if (value_changed)
		ImGui::MarkItemValueChanged(id);

	if ((flags & ImGuiInputTextFlags_EnterReturnsTrue) != 0)
		return enter_pressed;
	else
		return value_changed;
}
// }}}

// {{{ Combo
bool UI::BeginCombo(const std::string& label, const std::string& preview, const char* tooltip, ImGuiComboFlags flags)
{
	// Always consume the SetNextWindowSizeConstraint() call in our early return paths
	ImGuiContext& g = *GImGui;
	ImGuiCond backup_next_window_size_constraint = g.NextWindowData.SizeConstraintCond;
	g.NextWindowData.SizeConstraintCond = 0;

	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	IM_ASSERT((flags & (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)) != (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)); // Can't use both flags together

	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label.c_str());

	const float arrow_size = (flags & ImGuiComboFlags_NoArrowButton) ? 0.0f : ImGui::GetFrameHeight();
	const ImVec2 label_size = ImGui::CalcTextSize(label.c_str(), NULL, true);

	const float w = (flags & ImGuiComboFlags_NoPreview) ? arrow_size : (label_size.x == 0) ? ImGui::GetContentRegionAvail().x : ImGui::CalcItemWidth();

	const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + ImVec2(w, label_size.y + style.FramePadding.y * 2.0f));
	const ImRect total_bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));
	ImGui::ItemSize(total_bb, style.FramePadding.y);
	if (!ImGui::ItemAdd(total_bb, id, &frame_bb))
		return false;

	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(frame_bb, id, &hovered, &held);
	bool popup_open = ImGui::IsPopupOpen(id);

	if (tooltip != NULL && hovered && !popup_open)
		Tooltip(tooltip);

	const ImRect value_bb(frame_bb.Min, frame_bb.Max - ImVec2(arrow_size, 0.0f));
	const ImU32 frame_col = GetColor(popup_open ? Settings::Style::combo_background[2] :
						hovered ? Settings::Style::combo_background[1] : Settings::Style::combo_background[0]);
	ImGui::RenderNavHighlight(frame_bb, id);
	if (!(flags & ImGuiComboFlags_NoPreview))
		RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, 1.f, 0.f, Settings::Style::combo_border);

	if (!(flags & ImGuiComboFlags_NoArrowButton))
		UI::RenderArrow(
				ImVec2(frame_bb.Max.x - arrow_size + style.FramePadding.y, frame_bb.Min.y + style.FramePadding.y),
				popup_open ? ImGuiDir_Down : ImGuiDir_Left,
				1.f,
				GetColor(popup_open ? Settings::Style::arrow_foreground[2] :
						hovered ? Settings::Style::arrow_foreground[1] : Settings::Style::arrow_foreground[0])
			);
	ImGui::RenderFrameBorder(frame_bb.Min, frame_bb.Max, style.FrameRounding);
	if (!(flags & ImGuiComboFlags_NoPreview))
	{
		ImGui::PushStyleColor(ImGuiCol_Text, Settings::Style::combo_text);
		UI::RenderTextClipped(frame_bb.Min + ImVec2(Settings::Style::combo_text_padding, style.FramePadding.y), value_bb.Max - ImVec2(Settings::Style::combo_text_padding, 0), preview.c_str(), NULL, NULL, ImVec2(0.0f, 0.0f));
		ImGui::PopStyleColor();
	}
	if (label_size.x > 0)
		RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label.c_str());

	if ((pressed || g.NavActivateId == id) && !popup_open)
	{
		if (window->DC.NavLayerCurrent == 0)
			window->NavLastIds[0] = id;
		ImGui::OpenPopupEx(id);
		popup_open = true;
	}

	if (!popup_open)
		return false;

	if (backup_next_window_size_constraint)
	{
		g.NextWindowData.SizeConstraintCond = backup_next_window_size_constraint;
		g.NextWindowData.SizeConstraintRect.Min.x = ImMax(g.NextWindowData.SizeConstraintRect.Min.x, w);
	}
	else
	{
		if ((flags & ImGuiComboFlags_HeightMask_) == 0)
			flags |= ImGuiComboFlags_HeightRegular;
		IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiComboFlags_HeightMask_)); // Only one
		int popup_max_height_in_items = -1;
		if (flags & ImGuiComboFlags_HeightRegular)
			popup_max_height_in_items = 8;
		else if (flags & ImGuiComboFlags_HeightSmall)
			popup_max_height_in_items = 4;
		else if (flags & ImGuiComboFlags_HeightLarge)
			popup_max_height_in_items = 20;
		ImGui::SetNextWindowSizeConstraints(ImVec2(w, 0.0f), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items)));
	}

	char name[16];
	ImFormatString(name, IM_ARRAYSIZE(name), "##Combo_%02d", g.CurrentPopupStack.Size); // Recycle windows based on depth

	// Peak into expected window size so we can position it
	if (ImGuiWindow* popup_window = ImGui::FindWindowByName(name))
		if (popup_window->WasActive)
		{
			ImVec2 size_contents = CalcSizeContents(popup_window);
			ImVec2 size_expected = CalcSizeAfterConstraint(popup_window, CalcSizeAutoFit(popup_window, size_contents));
			if (flags & ImGuiComboFlags_PopupAlignLeft)
				popup_window->AutoPosLastDirection = ImGuiDir_Left;
			ImRect r_outer = FindAllowedExtentRectForWindow(popup_window);
			ImVec2 pos = FindBestWindowPosForPopupEx(frame_bb.GetBL(), size_expected, &popup_window->AutoPosLastDirection, r_outer, frame_bb, ImGuiPopupPositionPolicy_ComboBox);
			ImGui::SetNextWindowPos(pos);
		}

	// Horizontally align ourselves with the framed text
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
	ImGui::PushStyleColor(ImGuiCol_Border, Settings::Style::combo_popup[0]);
	ImGui::PushStyleColor(ImGuiCol_PopupBg, Settings::Style::combo_popup[1]);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, Settings::Style::combo_vertical_padding));
	ImGui::PushStyleVar(ImGuiStyleVar_PopupBorderSize, 1.f);
	bool ret = ImGui::Begin(name, NULL, window_flags);
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	if (!ret)
	{
		ImGui::EndPopup();
		IM_ASSERT(0); // This should never happen as we tested for IsPopupOpen() above
		return false;
	}
	ImGui::PushStyleColor(ImGuiCol_Text, Settings::Style::combo_text);
	return true;
}

void UI::EndCombo()
{
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(); // padding
	ImGui::EndCombo();
}

bool UI::Selectable(const char* label, bool selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;

	if ((flags & ImGuiSelectableFlags_SpanAllColumns) && window->DC.ColumnsSet) // FIXME-OPT: Avoid if vertically clipped.
		ImGui::PopClipRect();

	ImGuiID id = window->GetID(label);
	ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
	ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y);
	ImVec2 pos = window->DC.CursorPos;
	pos.y += window->DC.CurrentLineTextBaseOffset;
	ImRect bb_inner(pos, pos + size);
	ImGui::ItemSize(bb_inner);

	// Fill horizontal space.
	ImVec2 window_padding = window->WindowPadding;
	float max_x = (flags & ImGuiSelectableFlags_SpanAllColumns) ? ImGui::GetWindowContentRegionMax().x : ImGui::GetContentRegionMax().x;
	float w_draw = ImMax(label_size.x, window->Pos.x + max_x - window_padding.x - window->DC.CursorPos.x);
	ImVec2 size_draw((size_arg.x != 0 && !(flags & ImGuiSelectableFlags_DrawFillAvailWidth)) ? size_arg.x : w_draw, size_arg.y != 0.0f ? size_arg.y : size.y);
	ImRect bb(pos, pos + size_draw);
	if (size_arg.x == 0.0f || (flags & ImGuiSelectableFlags_DrawFillAvailWidth))
		bb.Max.x += window_padding.x;

	// Selectables are tightly packed together, we extend the box to cover spacing between selectable.
	float spacing_L = (float)(int)(style.ItemSpacing.x * 0.5f);
	float spacing_U = (float)(int)(style.ItemSpacing.y * 0.5f);
	float spacing_R = style.ItemSpacing.x - spacing_L;
	float spacing_D = style.ItemSpacing.y - spacing_U;
	bb.Min.x -= spacing_L;
	bb.Min.y -= spacing_U;
	bb.Max.x += spacing_R;
	bb.Max.y += spacing_D;
	if (!ImGui::ItemAdd(bb, (flags & ImGuiSelectableFlags_Disabled) ? 0 : id))
	{
		if ((flags & ImGuiSelectableFlags_SpanAllColumns) && window->DC.ColumnsSet)
			ImGui::PushColumnClipRect();
		return false;
	}

	// We use NoHoldingActiveID on menus so user can click and _hold_ on a menu then drag to browse child entries
	ImGuiButtonFlags button_flags = 0;
	if (flags & ImGuiSelectableFlags_NoHoldingActiveID)
		button_flags |= ImGuiButtonFlags_NoHoldingActiveID;
	if (flags & ImGuiSelectableFlags_PressedOnClick)
		button_flags |= ImGuiButtonFlags_PressedOnClick;
	if (flags & ImGuiSelectableFlags_PressedOnRelease)
		button_flags |= ImGuiButtonFlags_PressedOnRelease;
	if (flags & ImGuiSelectableFlags_Disabled)
		button_flags |= ImGuiButtonFlags_Disabled;
	if (flags & ImGuiSelectableFlags_AllowDoubleClick)
		button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick;
	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held, button_flags);
	if (flags & ImGuiSelectableFlags_Disabled)
		selected = false;

	// Hovering selectable with mouse updates NavId accordingly so navigation can be resumed with gamepad/keyboard (this doesn't happen on most widgets)
	if (pressed || hovered)
		if (!g.NavDisableMouseHover && g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
		{
			g.NavDisableHighlight = true;
			SetNavID(id, window->DC.NavLayerCurrent);
		}
	if (pressed)
		ImGui::MarkItemValueChanged(id);

	// Render
	if (hovered || selected)
	{
		const ImU32 col = (held && hovered) ? Settings::Style::combo_popup[4] : hovered ? Settings::Style::combo_popup[3] :
																							  Settings::Style::combo_popup[2];
		ImGui::RenderFrame(bb.Min, bb.Max, col, false, 0.0f);
		ImGui::RenderNavHighlight(bb, id, ImGuiNavHighlightFlags_TypeThin | ImGuiNavHighlightFlags_NoRounding);
	}

	if ((flags & ImGuiSelectableFlags_SpanAllColumns) && window->DC.ColumnsSet)
	{
		ImGui::PushColumnClipRect();
		bb.Max.x -= (ImGui::GetContentRegionMax().x - max_x);
	}

	UI::RenderTextClipped(bb_inner.Min+ImVec2(Settings::Style::combo_text_padding, 0), bb.Max-ImVec2(Settings::Style::combo_text_padding, 0), label, NULL, &label_size, ImVec2(0.0f, 0.0f));

	// Automatically close popups
	if (pressed && (window->Flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiSelectableFlags_DontClosePopups) && !(window->DC.ItemFlags & ImGuiItemFlags_SelectableDontClosePopup))
		ImGui::CloseCurrentPopup();
	return pressed;
}

void UI::SetItemDefaultFocus()
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = g.CurrentWindow;
	if (!window->Appearing)
		return;
	if (g.NavWindow == window->RootWindowForNav && (g.NavInitRequest || g.NavInitResultId != 0) && g.NavLayer == g.NavWindow->DC.NavLayerCurrent)
	{
		g.NavInitRequest = false;
		g.NavInitResultId = g.NavWindow->DC.LastItemId;
		g.NavInitResultRectRel = ImRect(g.NavWindow->DC.LastItemRect.Min - g.NavWindow->Pos, g.NavWindow->DC.LastItemRect.Max - g.NavWindow->Pos);
		NavUpdateAnyRequestFlag();
		if (!ImGui::IsItemVisible())
			ImGui::SetScrollHere();
	}
}


void UI::CheckboxCombo(const char* label, const std::vector<std::tuple<const char*, const char*, bool&>>& data, std::string& format, const std::string& sep)
{
	auto UpdateFormat = [&]()
	{
		format = "";
		bool first = true;
		for (const auto& d : data)
		{
			if (!std::get<2>(d))
				continue;

			if (!first)
				format += sep;
			else
				first = false;
			format += std::string(std::get<0>(d));
		}
		format += " "; // Voodoo
	};
	if (format.empty())
		UpdateFormat();

	if (UI::BeginCombo(label, format.c_str()))
	{
		for (auto& d : data)
		{
			ImGui::Dummy(ImVec2(4, 0));
			ImGui::SameLine();
			if (UI::Checkbox(std::get<0>(d), &std::get<2>(d)))
				UpdateFormat();
			if (std::get<1>(d))
				UI::Desc(std::get<1>(d));
		}
		UI::EndCombo();
	}
}
// }}}

// {{{ Color
bool UI::ColorButton(const char* desc_id, const ImVec4& col, ImGuiColorEditFlags flags, ImVec2 size)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;
	
	ImGuiContext& g = *GImGui;
	const ImGuiID id = window->GetID(desc_id);
	float default_size = ImGui::GetFrameHeight();
	if (size.x == 0.0f)
		size.x = default_size;
	if (size.y == 0.0f)
		size.y = default_size;
	const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size);
	ImGui::ItemSize(bb, (size.y >= default_size) ? g.Style.FramePadding.y : 0.0f);
	if (!ImGui::ItemAdd(bb, id))
		return false;
	
	bool hovered, held;
	bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);
	
	if (flags & ImGuiColorEditFlags_NoAlpha)
		flags &= ~(ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf);
	
	ImVec4 col_without_alpha(col.x, col.y, col.z, 1.0f);
	float grid_step = ImMin(size.x, size.y) / 2.99f;
	float rounding = ImMin(g.Style.FrameRounding, grid_step * 0.5f);
	ImRect bb_inner = bb;
	float off = -0.75f; // The border (using Col_FrameBg) tends to look off when color is near-opaque and rounding is enabled. This offset seemed like a good middle ground to reduce those artifacts.
	bb_inner.Expand(off);
	if ((flags & ImGuiColorEditFlags_AlphaPreviewHalf) && col.w < 1.0f)
	{
		float mid_x = (float)(int)((bb_inner.Min.x + bb_inner.Max.x) * 0.5f + 0.5f);
		ImGui::RenderColorRectWithAlphaCheckerboard(ImVec2(bb_inner.Min.x + grid_step, bb_inner.Min.y), bb_inner.Max, ImGui::ColorConvertFloat4ToU32(GetColor(col)), grid_step, ImVec2(-grid_step + off, off), rounding, ImDrawCornerFlags_TopRight | ImDrawCornerFlags_BotRight);
		window->DrawList->AddRectFilled(bb_inner.Min, ImVec2(mid_x, bb_inner.Max.y), ImGui::ColorConvertFloat4ToU32(GetColor(col_without_alpha)), rounding, ImDrawCornerFlags_TopLeft | ImDrawCornerFlags_BotLeft);
	}
	else
	{
		// Because GetColorU32() multiplies by the global style Alpha and we don't want to display a checkerboard if the source code had no alpha
		ImVec4 col_source = (flags & ImGuiColorEditFlags_AlphaPreview) ? col : col_without_alpha;
		if (col_source.w < 1.0f)
			ImGui::RenderColorRectWithAlphaCheckerboard(bb_inner.Min, bb_inner.Max, ImGui::ColorConvertFloat4ToU32(GetColor(col_source)), grid_step, ImVec2(off, off), rounding);
		else
			window->DrawList->AddRectFilled(bb_inner.Min, bb_inner.Max, ImGui::ColorConvertFloat4ToU32(GetColor(col_source)), rounding, ImDrawCornerFlags_All);
	}
	ImGui::RenderNavHighlight(bb, id);
	if (g.Style.FrameBorderSize > 0.0f)
		ImGui::RenderFrameBorder(bb.Min, bb.Max, rounding);
	else
		window->DrawList->AddRect(bb.Min, bb.Max, GetColor(ImGuiCol_FrameBg), rounding); // Color button are often in need of some sort of border
	
	// Drag and Drop Source
	// NB: The ActiveId test is merely an optional micro-optimization, BeginDragDropSource() does the same test.
	if (g.ActiveId == id && !(flags & ImGuiColorEditFlags_NoDragDrop) && ImGui::BeginDragDropSource())
	{
		if (flags & ImGuiColorEditFlags_NoAlpha)
			ImGui::SetDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F, &col, sizeof(float) * 3, ImGuiCond_Once);
		else
			ImGui::SetDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F, &col, sizeof(float) * 4, ImGuiCond_Once);
		ImGui::ColorButton(desc_id, col, flags);
		ImGui::SameLine();
		ImGui::TextUnformatted("Color");
		ImGui::EndDragDropSource();
	}

	// Tooltip
	if (!(flags & ImGuiColorEditFlags_NoTooltip) && hovered)
	{
		Tooltip(desc_id);
	}
	
	if (pressed)
		ImGui::MarkItemValueChanged(id);
	
	return pressed;
}

bool UI::ColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags, const float* ref_col)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	ImDrawList* draw_list = window->DrawList;

	ImGuiStyle& style = g.Style;
	ImGuiIO& io = g.IO;

	ImGui::PushID(label);
	ImGui::BeginGroup();

	if (!(flags & ImGuiColorEditFlags_NoSidePreview))
		flags |= ImGuiColorEditFlags_NoSmallPreview;

	// Context menu: display and store options.
	if (!(flags & ImGuiColorEditFlags_NoOptions))
		ColorPickerOptionsPopup(flags, col);

	// Read stored options
	if (!(flags & ImGuiColorEditFlags__PickerMask))
		flags |= ((g.ColorEditOptions & ImGuiColorEditFlags__PickerMask) ? g.ColorEditOptions : ImGuiColorEditFlags__OptionsDefault) & ImGuiColorEditFlags__PickerMask;
	IM_ASSERT(ImIsPowerOfTwo((int)(flags & ImGuiColorEditFlags__PickerMask))); // Check that only 1 is selected
	if (!(flags & ImGuiColorEditFlags_NoOptions))
		flags |= (g.ColorEditOptions & ImGuiColorEditFlags_AlphaBar);

	// Setup
	int components = (flags & ImGuiColorEditFlags_NoAlpha) ? 3 : 4;
	bool alpha_bar = (flags & ImGuiColorEditFlags_AlphaBar) && !(flags & ImGuiColorEditFlags_NoAlpha);
	ImVec2 picker_pos = window->DC.CursorPos;
	float square_sz = ImGui::GetFrameHeight();
	float bars_width = square_sz * 0.8f; // Arbitrary smallish width of Hue/Alpha picking bars
	float sv_picker_size = ImMax(bars_width * 1, ImGui::CalcItemWidth() - (alpha_bar ? 2 : 1) * (bars_width + style.ItemInnerSpacing.x)); // Saturation/Value picking box
	float bar0_pos_x = picker_pos.x + sv_picker_size + style.ItemInnerSpacing.x;
	float bar1_pos_x = bar0_pos_x + bars_width + style.ItemInnerSpacing.x;
	float bars_triangles_half_sz = (float)(int)(bars_width * 0.20f);

	float backup_initial_col[4];
	memcpy(backup_initial_col, col, components * sizeof(float));

	float wheel_thickness = sv_picker_size * 0.08f;
	float wheel_r_outer = sv_picker_size * 0.50f;
	float wheel_r_inner = wheel_r_outer - wheel_thickness;
	ImVec2 wheel_center(picker_pos.x + (sv_picker_size + bars_width) * 0.5f, picker_pos.y + sv_picker_size * 0.5f);

	// Note: the triangle is displayed rotated with triangle_pa pointing to Hue, but most coordinates stays unrotated for logic.
	float triangle_r = wheel_r_inner - (int)(sv_picker_size * 0.027f);
	ImVec2 triangle_pa = ImVec2(triangle_r, 0.0f); // Hue point.
	ImVec2 triangle_pb = ImVec2(triangle_r * -0.5f, triangle_r * -0.866025f); // Black point.
	ImVec2 triangle_pc = ImVec2(triangle_r * -0.5f, triangle_r * +0.866025f); // White point.

	float H, S, V;
	ImGui::ColorConvertRGBtoHSV(col[0], col[1], col[2], H, S, V);

	bool value_changed = false, value_changed_h = false, value_changed_sv = false;

	ImGui::PushItemFlag(ImGuiItemFlags_NoNav, true);
	if (flags & ImGuiColorEditFlags_PickerHueWheel)
	{
		// Hue wheel + SV triangle logic
		ImGui::InvisibleButton("hsv", ImVec2(sv_picker_size + style.ItemInnerSpacing.x + bars_width, sv_picker_size));
		if (ImGui::IsItemActive())
		{
			ImVec2 initial_off = g.IO.MouseClickedPos[0] - wheel_center;
			ImVec2 current_off = g.IO.MousePos - wheel_center;
			float initial_dist2 = ImLengthSqr(initial_off);
			if (initial_dist2 >= (wheel_r_inner - 1) * (wheel_r_inner - 1) && initial_dist2 <= (wheel_r_outer + 1) * (wheel_r_outer + 1))
			{
				// Interactive with Hue wheel
				H = ImAtan2(current_off.y, current_off.x) / IM_PI * 0.5f;
				if (H < 0.0f)
					H += 1.0f;
				value_changed = value_changed_h = true;
			}
			float cos_hue_angle = ImCos(-H * 2.0f * IM_PI);
			float sin_hue_angle = ImSin(-H * 2.0f * IM_PI);
			if (ImTriangleContainsPoint(triangle_pa, triangle_pb, triangle_pc, ImRotate(initial_off, cos_hue_angle, sin_hue_angle)))
			{
				// Interacting with SV triangle
				ImVec2 current_off_unrotated = ImRotate(current_off, cos_hue_angle, sin_hue_angle);
				if (!ImTriangleContainsPoint(triangle_pa, triangle_pb, triangle_pc, current_off_unrotated))
					current_off_unrotated = ImTriangleClosestPoint(triangle_pa, triangle_pb, triangle_pc, current_off_unrotated);
				float uu, vv, ww;
				ImTriangleBarycentricCoords(triangle_pa, triangle_pb, triangle_pc, current_off_unrotated, uu, vv, ww);
				V = ImClamp(1.0f - vv, 0.0001f, 1.0f);
				S = ImClamp(uu / V, 0.0001f, 1.0f);
				value_changed = value_changed_sv = true;
			}
		}
		if (!(flags & ImGuiColorEditFlags_NoOptions))
			ImGui::OpenPopupOnItemClick("context");
	}
	else if (flags & ImGuiColorEditFlags_PickerHueBar)
	{
		// SV rectangle logic
		ImGui::InvisibleButton("sv", ImVec2(sv_picker_size, sv_picker_size));
		if (ImGui::IsItemActive())
		{
			S = ImSaturate((io.MousePos.x - picker_pos.x) / (sv_picker_size - 1));
			V = 1.0f - ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 1));
			value_changed = value_changed_sv = true;
		}
		if (!(flags & ImGuiColorEditFlags_NoOptions))
			ImGui::OpenPopupOnItemClick("context");

		// Hue bar logic
		ImGui::SetCursorScreenPos(ImVec2(bar0_pos_x, picker_pos.y));
		ImGui::InvisibleButton("hue", ImVec2(bars_width, sv_picker_size));
		if (ImGui::IsItemActive())
		{
			H = ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 1));
			value_changed = value_changed_h = true;
		}
	}

	// Alpha bar logic
	if (alpha_bar)
	{
		ImGui::SetCursorScreenPos(ImVec2(bar1_pos_x, picker_pos.y));
		ImGui::InvisibleButton("alpha", ImVec2(bars_width, sv_picker_size));
		if (ImGui::IsItemActive())
		{
			col[3] = 1.0f - ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 1));
			value_changed = true;
		}
	}
	ImGui::PopItemFlag(); // ImGuiItemFlags_NoNav

	if (!(flags & ImGuiColorEditFlags_NoSidePreview))
	{
		ImGui::SameLine(0, style.ItemInnerSpacing.x);
		ImGui::BeginGroup();
	}

	if (!(flags & ImGuiColorEditFlags_NoSidePreview))
	{
		ImGui::PushItemFlag(ImGuiItemFlags_NoNavDefaultFocus, true);
		ImVec4 col_v4(col[0], col[1], col[2], (flags & ImGuiColorEditFlags_NoAlpha) ? 1.0f : col[3]);
		if ((flags & ImGuiColorEditFlags_NoLabel))
		UI::ColorButton("Current", col_v4, (flags & (ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_NoTooltip)), ImVec2(square_sz * 1.5f, square_sz));
		if (ref_col != NULL)
		{
			ImVec4 ref_col_v4(ref_col[0], ref_col[1], ref_col[2], (flags & ImGuiColorEditFlags_NoAlpha) ? 1.0f : ref_col[3]);
			if (UI::ColorButton("Previous", ref_col_v4, (flags & (ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf | ImGuiColorEditFlags_NoTooltip)), ImVec2(square_sz * 1.5f, square_sz)))
			{
				memcpy(col, ref_col, components * sizeof(float));
				value_changed = true;
			}
		}
		ImGui::PopItemFlag();
		ImGui::EndGroup();
	}

	// Convert back color to RGB
	if (value_changed_h || value_changed_sv)
		ImGui::ColorConvertHSVtoRGB(H >= 1.0f ? H - 10 * 1e-6f : H, S > 0.0f ? S : 10 * 1e-6f, V > 0.0f ? V : 1e-6f, col[0], col[1], col[2]);

	ImGuiColorEditFlags sub_flags_to_forward = ImGuiColorEditFlags__DataTypeMask | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoSmallPreview | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaPreviewHalf;
	ImGuiColorEditFlags sub_flags = (flags & sub_flags_to_forward) | ImGuiColorEditFlags_NoPicker;
	value_changed |= ImGui::ColorEdit4("##hex", col, sub_flags | ImGuiColorEditFlags_HEX);

	// Try to cancel hue wrap (after ColorEdit), if any
	if (value_changed)
	{
		float new_H, new_S, new_V;
		ImGui::ColorConvertRGBtoHSV(col[0], col[1], col[2], new_H, new_S, new_V);
		if (new_H <= 0 && H > 0)
		{
			if (new_V <= 0 && V != new_V)
				ImGui::ColorConvertHSVtoRGB(H, S, new_V <= 0 ? V * 0.5f : new_V, col[0], col[1], col[2]);
			else if (new_S <= 0)
				ImGui::ColorConvertHSVtoRGB(H, new_S <= 0 ? S * 0.5f : new_S, new_V, col[0], col[1], col[2]);
		}
	}

	ImVec4 hue_color_f(1, 1, 1, 1);
	ImGui::ColorConvertHSVtoRGB(H, 1, 1, hue_color_f.x, hue_color_f.y, hue_color_f.z);
	ImU32 hue_color32 = ImGui::ColorConvertFloat4ToU32(hue_color_f);
	ImU32 col32_no_alpha = ImGui::ColorConvertFloat4ToU32(ImVec4(col[0], col[1], col[2], 1.0f));

	const ImU32 hue_colors[6 + 1] = { IM_COL32(255, 0, 0, 255), IM_COL32(255, 255, 0, 255), IM_COL32(0, 255, 0, 255), IM_COL32(0, 255, 255, 255), IM_COL32(0, 0, 255, 255), IM_COL32(255, 0, 255, 255), IM_COL32(255, 0, 0, 255) };
	ImVec2 sv_cursor_pos;

	if (flags & ImGuiColorEditFlags_PickerHueWheel)
	{
		// Render Hue Wheel
		const float aeps = 1.5f / wheel_r_outer; // Half a pixel arc length in radians (2pi cancels out).
		const int segment_per_arc = ImMax(4, (int)wheel_r_outer / 12);
		for (int n = 0; n < 6; n++)
		{
			const float a0 = (n) / 6.0f * 2.0f * IM_PI - aeps;
			const float a1 = (n + 1.0f) / 6.0f * 2.0f * IM_PI + aeps;
			const int vert_start_idx = draw_list->VtxBuffer.Size;
			draw_list->PathArcTo(wheel_center, (wheel_r_inner + wheel_r_outer) * 0.5f, a0, a1, segment_per_arc);
			draw_list->PathStroke(IM_COL32_WHITE, false, wheel_thickness);
			const int vert_end_idx = draw_list->VtxBuffer.Size;

			// Paint colors over existing vertices
			ImVec2 gradient_p0(wheel_center.x + ImCos(a0) * wheel_r_inner, wheel_center.y + ImSin(a0) * wheel_r_inner);
			ImVec2 gradient_p1(wheel_center.x + ImCos(a1) * wheel_r_inner, wheel_center.y + ImSin(a1) * wheel_r_inner);
			ImGui::ShadeVertsLinearColorGradientKeepAlpha(draw_list->VtxBuffer.Data + vert_start_idx, draw_list->VtxBuffer.Data + vert_end_idx, gradient_p0, gradient_p1, hue_colors[n], hue_colors[n + 1]);
		}

		// Render Cursor + preview on Hue Wheel
		float cos_hue_angle = ImCos(H * 2.0f * IM_PI);
		float sin_hue_angle = ImSin(H * 2.0f * IM_PI);
		ImVec2 hue_cursor_pos(wheel_center.x + cos_hue_angle * (wheel_r_inner + wheel_r_outer) * 0.5f, wheel_center.y + sin_hue_angle * (wheel_r_inner + wheel_r_outer) * 0.5f);
		float hue_cursor_rad = value_changed_h ? wheel_thickness * 0.65f : wheel_thickness * 0.55f;
		int hue_cursor_segments = ImClamp((int)(hue_cursor_rad / 1.4f), 9, 32);
		draw_list->AddCircleFilled(hue_cursor_pos, hue_cursor_rad, hue_color32, hue_cursor_segments);
		draw_list->AddCircle(hue_cursor_pos, hue_cursor_rad + 1, IM_COL32(128, 128, 128, 255), hue_cursor_segments);
		draw_list->AddCircle(hue_cursor_pos, hue_cursor_rad, IM_COL32_WHITE, hue_cursor_segments);

		// Render SV triangle (rotated according to hue)
		ImVec2 tra = wheel_center + ImRotate(triangle_pa, cos_hue_angle, sin_hue_angle);
		ImVec2 trb = wheel_center + ImRotate(triangle_pb, cos_hue_angle, sin_hue_angle);
		ImVec2 trc = wheel_center + ImRotate(triangle_pc, cos_hue_angle, sin_hue_angle);
		ImVec2 uv_white = ImGui::GetFontTexUvWhitePixel();
		draw_list->PrimReserve(6, 6);
		draw_list->PrimVtx(tra, uv_white, hue_color32);
		draw_list->PrimVtx(trb, uv_white, hue_color32);
		draw_list->PrimVtx(trc, uv_white, IM_COL32_WHITE);
		draw_list->PrimVtx(tra, uv_white, IM_COL32_BLACK_TRANS);
		draw_list->PrimVtx(trb, uv_white, IM_COL32_BLACK);
		draw_list->PrimVtx(trc, uv_white, IM_COL32_BLACK_TRANS);
		draw_list->AddTriangle(tra, trb, trc, IM_COL32(128, 128, 128, 255), 1.5f);
		sv_cursor_pos = ImLerp(ImLerp(trc, tra, ImSaturate(S)), trb, ImSaturate(1 - V));
	}
	else if (flags & ImGuiColorEditFlags_PickerHueBar)
	{
		// Render SV Square
		draw_list->AddRectFilledMultiColor(picker_pos, picker_pos + ImVec2(sv_picker_size, sv_picker_size), IM_COL32_WHITE, hue_color32, hue_color32, IM_COL32_WHITE);
		draw_list->AddRectFilledMultiColor(picker_pos, picker_pos + ImVec2(sv_picker_size, sv_picker_size), IM_COL32_BLACK_TRANS, IM_COL32_BLACK_TRANS, IM_COL32_BLACK, IM_COL32_BLACK);
		ImGui::RenderFrameBorder(picker_pos, picker_pos + ImVec2(sv_picker_size, sv_picker_size), 0.0f);
		sv_cursor_pos.x = ImClamp((float)(int)(picker_pos.x + ImSaturate(S) * sv_picker_size + 0.5f), picker_pos.x + 2, picker_pos.x + sv_picker_size - 2); // Sneakily prevent the circle to stick out too much
		sv_cursor_pos.y = ImClamp((float)(int)(picker_pos.y + ImSaturate(1 - V) * sv_picker_size + 0.5f), picker_pos.y + 2, picker_pos.y + sv_picker_size - 2);

		// Render Hue Bar
		for (int i = 0; i < 6; ++i)
			draw_list->AddRectFilledMultiColor(ImVec2(bar0_pos_x, picker_pos.y + i * (sv_picker_size / 6)), ImVec2(bar0_pos_x + bars_width, picker_pos.y + (i + 1) * (sv_picker_size / 6)), hue_colors[i], hue_colors[i], hue_colors[i + 1], hue_colors[i + 1]);
		float bar0_line_y = (float)(int)(picker_pos.y + H * sv_picker_size + 0.5f);
		ImGui::RenderFrameBorder(ImVec2(bar0_pos_x, picker_pos.y), ImVec2(bar0_pos_x + bars_width, picker_pos.y + sv_picker_size), 0.0f);
		RenderArrowsForVerticalBar(draw_list, ImVec2(bar0_pos_x - 1, bar0_line_y), ImVec2(bars_triangles_half_sz + 1, bars_triangles_half_sz), bars_width + 2.0f);
	}

	// Render cursor/preview circle (clamp S/V within 0..1 range because floating points colors may lead HSV values to be out of range)
	float sv_cursor_rad = value_changed_sv ? 10.0f : 6.0f;
	draw_list->AddCircleFilled(sv_cursor_pos, sv_cursor_rad, col32_no_alpha, 12);
	draw_list->AddCircle(sv_cursor_pos, sv_cursor_rad + 1, IM_COL32(128, 128, 128, 255), 12);
	draw_list->AddCircle(sv_cursor_pos, sv_cursor_rad, IM_COL32_WHITE, 12);

	// Render alpha bar
	if (alpha_bar)
	{
		float alpha = ImSaturate(col[3]);
		ImRect bar1_bb(bar1_pos_x, picker_pos.y, bar1_pos_x + bars_width, picker_pos.y + sv_picker_size);
		ImGui::RenderColorRectWithAlphaCheckerboard(bar1_bb.Min, bar1_bb.Max, IM_COL32(0, 0, 0, 0), bar1_bb.GetWidth() / 2.0f, ImVec2(0.0f, 0.0f));
		draw_list->AddRectFilledMultiColor(bar1_bb.Min, bar1_bb.Max, col32_no_alpha, col32_no_alpha, col32_no_alpha & ~IM_COL32_A_MASK, col32_no_alpha & ~IM_COL32_A_MASK);
		float bar1_line_y = (float)(int)(picker_pos.y + (1.0f - alpha) * sv_picker_size + 0.5f);
		ImGui::RenderFrameBorder(bar1_bb.Min, bar1_bb.Max, 0.0f);
		RenderArrowsForVerticalBar(draw_list, ImVec2(bar1_pos_x - 1, bar1_line_y), ImVec2(bars_triangles_half_sz + 1, bars_triangles_half_sz), bars_width + 2.0f);
	}

	ImGui::EndGroup();

	if (value_changed && memcmp(backup_initial_col, col, components * sizeof(float)) == 0)
		value_changed = false;
	if (value_changed)
		ImGui::MarkItemValueChanged(window->DC.LastItemId);

	ImGui::PopID();

	return value_changed;
}

bool UI::ColorEdit4(const char* label, ImVec4& col, ImGuiColorEditFlags flags)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const float square_sz = ImGui::GetFrameHeight();
	//const float w_items_all = ImGui::CalcItemWidth() - w_extra;
	//const char* label_display_end = ImGui::FindRenderedTextEnd(label);

	ImGui::BeginGroup();
	ImGui::PushID(label);

	// If we're not showing any slider there's no point in doing any HSV conversions
	const ImGuiColorEditFlags flags_untouched = flags;
	if (flags & ImGuiColorEditFlags_NoInputs)
		flags = (flags & (~ImGuiColorEditFlags__InputsMask)) | ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_NoOptions;

	// Read stored options
	if (!(flags & ImGuiColorEditFlags__InputsMask))
		flags |= (g.ColorEditOptions & ImGuiColorEditFlags__InputsMask);
	if (!(flags & ImGuiColorEditFlags__DataTypeMask))
		flags |= (g.ColorEditOptions & ImGuiColorEditFlags__DataTypeMask);
	if (!(flags & ImGuiColorEditFlags__PickerMask))
		flags |= (g.ColorEditOptions & ImGuiColorEditFlags__PickerMask);
	flags |= (g.ColorEditOptions & ~(ImGuiColorEditFlags__InputsMask | ImGuiColorEditFlags__DataTypeMask | ImGuiColorEditFlags__PickerMask));

	const bool alpha = (flags & ImGuiColorEditFlags_NoAlpha) == 0;
	const int components = alpha ? 4 : 3;

	// Convert to the formats we need
	float f[4] = { col.x, col.y, col.z, alpha ? col.w : 1.0f };
	if (flags & ImGuiColorEditFlags_HSV)
		ImGui::ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);
	int i[4] = { IM_F32_TO_INT8_UNBOUND(f[0]), IM_F32_TO_INT8_UNBOUND(f[1]), IM_F32_TO_INT8_UNBOUND(f[2]), IM_F32_TO_INT8_UNBOUND(f[3]) };

	bool value_changed = false;
	bool value_changed_as_float = false;

	ImGuiWindow* picker_active_window = NULL;
	const ImVec4 col_v4(col.x, col.y, col.z, alpha ? col.w : 1.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.f);
	ImGui::PushStyleColor(ImGuiCol_Border, Settings::Style::color_button_border);
	if (UI::ColorButton(label, col_v4, ImGuiColorEditFlags_AlphaPreview, ImVec2(square_sz, square_sz) * Settings::Style::color_button_scale))
	{
		if (!(flags & ImGuiColorEditFlags_NoPicker))
		{
			// Store current color and open a picker
			g.ColorPickerRef = col_v4;
			ImGui::OpenPopup("picker");
			ImGui::SetNextWindowPos(window->DC.LastItemRect.GetBL() + ImVec2(-1, style.ItemSpacing.y));
		}
	}
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	if (!(flags & ImGuiColorEditFlags_NoOptions))
		ImGui::OpenPopupOnItemClick("context");

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(17, 8));
	ImGui::PushStyleColor(ImGuiCol_PopupBg, 0x00000000);
	if (ImGui::BeginPopup("picker"))
	{
		picker_active_window = g.CurrentWindow;
		ImGuiColorEditFlags picker_flags_to_forward = ImGuiColorEditFlags__DataTypeMask | ImGuiColorEditFlags__PickerMask | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_AlphaBar;
		ImGuiColorEditFlags picker_flags = (flags_untouched & picker_flags_to_forward) | ImGuiColorEditFlags__InputsMask | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreviewHalf;
		ImGui::PushItemWidth(square_sz * 8.0f); // Use 256 + bar sizes?
		value_changed |= UI::ColorPicker4("##picker", (float*)&col, picker_flags | ImGuiColorEditFlags_AlphaBar, &g.ColorPickerRef.x);
		ImGui::PopItemWidth();
		ImGui::EndPopup();
	}
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	

	// Convert back
	if (picker_active_window == NULL)
	{
		if (!value_changed_as_float)
			for (int n = 0; n < 4; n++)
				f[n] = i[n] / 255.0f;
		if (flags & ImGuiColorEditFlags_HSV)
			ImGui::ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
		if (value_changed)
		{
			col.x = f[0];
			col.y = f[1];
			col.z = f[2];
			if (alpha)
				col.w = f[3];
		}
	}

	ImGui::PopID();
	ImGui::EndGroup();

	// Drag and Drop Target
	// NB: The flag test is merely an optional micro-optimization, BeginDragDropTarget() does the same test.
	if ((window->DC.LastItemStatusFlags & ImGuiItemStatusFlags_HoveredRect) && !(flags & ImGuiColorEditFlags_NoDragDrop) && ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
		{
			memcpy((float*)&col, payload->Data, sizeof(float) * 3);
			value_changed = true;
		}
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
		{
			memcpy((float*)&col, payload->Data, sizeof(float) * components);
			value_changed = true;
		}
		ImGui::EndDragDropTarget();
	}

	// When picker is being actively used, use its active id so IsItemActive() will function on ColorEdit4().
	if (picker_active_window && g.ActiveId != 0 && g.ActiveIdWindow == picker_active_window)
		window->DC.LastItemId = g.ActiveId;

	if (value_changed)
		ImGui::MarkItemValueChanged(window->DC.LastItemId);

	return value_changed;
}

bool UI::ColorEdit4(const char* label, ImColor& color, ImGuiColorEditFlags flags)
{
	const ImVec4 col = color.Value;
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const float square_sz = ImGui::GetFrameHeight();

	ImGui::BeginGroup();
	ImGui::PushID(label);

	// If we're not showing any slider there's no point in doing any HSV conversions
	const ImGuiColorEditFlags flags_untouched = flags;
	if (flags & ImGuiColorEditFlags_NoInputs)
		flags = (flags & (~ImGuiColorEditFlags__InputsMask)) | ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_NoOptions;

	// Read stored options
	if (!(flags & ImGuiColorEditFlags__InputsMask))
		flags |= (g.ColorEditOptions & ImGuiColorEditFlags__InputsMask);
	if (!(flags & ImGuiColorEditFlags__DataTypeMask))
		flags |= (g.ColorEditOptions & ImGuiColorEditFlags__DataTypeMask);
	if (!(flags & ImGuiColorEditFlags__PickerMask))
		flags |= (g.ColorEditOptions & ImGuiColorEditFlags__PickerMask);
	flags |= (g.ColorEditOptions & ~(ImGuiColorEditFlags__InputsMask | ImGuiColorEditFlags__DataTypeMask | ImGuiColorEditFlags__PickerMask));

	const bool alpha = (flags & ImGuiColorEditFlags_NoAlpha) == 0;
	const int components = alpha ? 4 : 3;

	// Convert to the formats we need
	float f[4] = { col.x, col.y, col.z, alpha ? col.w : 1.0f };
	if (flags & ImGuiColorEditFlags_HSV)
		ImGui::ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);
	//int i[4] = { IM_F32_TO_INT8_UNBOUND(f[0]), IM_F32_TO_INT8_UNBOUND(f[1]), IM_F32_TO_INT8_UNBOUND(f[2]), IM_F32_TO_INT8_UNBOUND(f[3]) };

	bool value_changed = false;

	ImGuiWindow* picker_active_window = NULL;
	const ImVec4 col_v4(col.x, col.y, col.z, alpha ? col.w : 1.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.f);
	ImGui::PushStyleColor(ImGuiCol_Border, Settings::Style::color_button_border);
	if (UI::ColorButton(label, col_v4, ImGuiColorEditFlags_AlphaPreview, ImVec2(square_sz, square_sz) * Settings::Style::color_button_scale))
	{
		if (!(flags & ImGuiColorEditFlags_NoPicker))
		{
			// Store current color and open a picker
			g.ColorPickerRef = col_v4;
			ImGui::OpenPopup("picker");
			ImGui::SetNextWindowPos(window->DC.LastItemRect.GetBL() + ImVec2(-1, style.ItemSpacing.y));
		}
	}
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();

	if (!(flags & ImGuiColorEditFlags_NoOptions))
		ImGui::OpenPopupOnItemClick("context");

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(17, 8)); // When we grab the knot it gets alot bigger
	ImGui::PushStyleColor(ImGuiCol_PopupBg, 0x00000000);
	if (ImGui::BeginPopup("picker"))
	{
		picker_active_window = g.CurrentWindow;
		ImGuiColorEditFlags picker_flags_to_forward = ImGuiColorEditFlags__DataTypeMask | ImGuiColorEditFlags__PickerMask | ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_AlphaBar;
		ImGuiColorEditFlags picker_flags = (flags_untouched & picker_flags_to_forward) | ImGuiColorEditFlags__InputsMask | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaPreviewHalf;
		ImGui::PushItemWidth(square_sz * 8.0f); // Use 256 + bar sizes?
		value_changed |= UI::ColorPicker4("##picker", (float*)&color, picker_flags | ImGuiColorEditFlags_AlphaBar, &g.ColorPickerRef.x);
		ImGui::PopItemWidth();
		ImGui::EndPopup();
	}
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();

	ImGui::PopID();
	ImGui::EndGroup();

	// Drag and Drop Target
	// NB: The flag test is merely an optional micro-optimization, BeginDragDropTarget() does the same test.
	if ((window->DC.LastItemStatusFlags & ImGuiItemStatusFlags_HoveredRect) && !(flags & ImGuiColorEditFlags_NoDragDrop) && ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
		{
			memcpy((float*)&col, payload->Data, sizeof(float) * 3);
			value_changed = true;
		}
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
		{
			memcpy((float*)&col, payload->Data, sizeof(float) * components);
			value_changed = true;
		}
		ImGui::EndDragDropTarget();
	}

	// When picker is being actively used, use its active id so IsItemActive() will function on ColorEdit4().
	if (picker_active_window && g.ActiveId != 0 && g.ActiveIdWindow == picker_active_window)
		window->DC.LastItemId = g.ActiveId;

	if (value_changed)
		ImGui::MarkItemValueChanged(window->DC.LastItemId);

	return value_changed;
}

// }}}

// {{{ ListBox
bool UI::ListBoxHeader(const char* label, const ImVec2& size_arg)
{
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems)
		return false;

	const ImGuiStyle& style = ImGui::GetStyle();
	const ImGuiID id = ImGui::GetID(label);
	const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

	// Size default to hold ~7 items. Fractional number of items helps seeing that we can scroll down/up without looking at scrollbar.
		ImVec2 size = ImGui::CalcItemSize(size_arg, ImGui::CalcItemWidth(), ImGui::GetTextLineHeightWithSpacing() * 7.4f + style.ItemSpacing.y);
	if (size_arg.x == 0.f && label_size.x == 0.f)
		size.x = ImGui::GetContentRegionAvailWidth();
	ImVec2 frame_size = ImVec2(size.x, ImMax(size.y, label_size.y));
	ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + frame_size);
	ImRect bb(frame_bb.Min, frame_bb.Max + ImVec2(label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f, 0.0f));
	window->DC.LastItemRect = bb; // Forward storage for ListBoxFooter.. dodgy.

	ImGui::BeginGroup();
	if (label_size.x > 0)
		RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

	ImGui::PushStyleColor(ImGuiCol_Border, Settings::Style::combo_popup[0]);
	ImGui::PushStyleColor(ImGuiCol_FrameBg, Settings::Style::combo_popup[1]);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.f);
	ImGui::BeginChildFrame(id, frame_bb.GetSize());
	ImGui::PopStyleColor();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
	ImGui::PopStyleVar();
	return true;
}

bool UI::ListBoxHeader(const char* label, int items_count, int height_in_items)
{
	// Size default to hold ~7 items. Fractional number of items helps seeing that we can scroll down/up without looking at scrollbar.
	// We don't add +0.40f if items_count <= height_in_items. It is slightly dodgy, because it means a dynamic list of items will make the widget resize occasionally when it crosses that size.
	// I am expecting that someone will come and complain about this behavior in a remote future, then we can advise on a better solution.
	if (height_in_items < 0)
		height_in_items = ImMin(items_count, 7);
	float height_in_items_f = height_in_items < items_count ? (height_in_items + 0.40f) : (height_in_items + 0.00f);

	// We include ItemSpacing.y so that a list sized for the exact number of items doesn't make a scrollbar appears. We could also enforce that by passing a flag to BeginChild().
	ImVec2 size;
	size.x = 0.0f;
	size.y = ImGui::GetTextLineHeightWithSpacing() * height_in_items_f + ImGui::GetStyle().ItemSpacing.y;
	return ListBoxHeader(label, size);
}

void UI::ListBoxFooter()
{
	ImGuiWindow* parent_window = ImGui::GetCurrentWindow()->ParentWindow;
	const ImRect bb = parent_window->DC.LastItemRect;
	const ImGuiStyle& style = ImGui::GetStyle();

	ImGui::EndChildFrame();

	// Redeclare item size so that it includes the label (we have stored the full size in LastItemRect)
	// We call SameLine() to restore DC.CurrentLine* data
	ImGui::SameLine();
	parent_window->DC.CursorPos = bb.Min;
	ImGui::ItemSize(bb, style.FramePadding.y);
	ImGui::EndGroup();
}

bool UI::ListBox(const char* label, int* current_item, bool (*items_getter)(void*, int, const char**), void* data, int items_count, int height_in_items)
{
	if (!ListBoxHeader(label, items_count, height_in_items))
		return false;

	// Assume all items have even height (= 1 line of text). If you need items of different or variable sizes you can create a custom version of ListBox() in your code without using the clipper.
	bool value_changed = false;
	ImGuiListClipper clipper(items_count, ImGui::GetTextLineHeightWithSpacing()); // We know exactly our line height here so we pass it as a minor optimization, but generally you don't need to.
	while (clipper.Step())
		for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
		{
			const bool item_selected = (i == *current_item);
			const char* item_text;
			if (!items_getter(data, i, &item_text))
				item_text = "*Unknown item*";

			ImGui::PushID(i);
			if (Selectable(item_text, item_selected))
			{
				*current_item = i;
				value_changed = true;
			}
			if (item_selected)
				SetItemDefaultFocus();
			ImGui::PopID();
		}
	ListBoxFooter();
	return value_changed;
}

bool UI::ListBox(const char* label, int* current_item, const std::vector<std::string>& elems, int height_items)
{
	if (!ListBoxHeader(label, elems.size(), height_items))
		return false;

	// Assume all items have even height (= 1 line of text). If you need items of different or variable sizes you can create a custom version of ListBox() in your code without using the clipper.
	bool value_changed = false;
	ImGuiListClipper clipper(elems.size(), ImGui::GetTextLineHeightWithSpacing()); // We know exactly our line height here so we pass it as a minor optimization, but generally you don't need to.
	while (clipper.Step())
		for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
		{
			const bool item_selected = (i == *current_item);

			ImGui::PushID(i);
			if (Selectable(elems[i].c_str(), item_selected))
			{
				*current_item = i;
				value_changed = true;
			}
			if (item_selected)
				SetItemDefaultFocus();
			ImGui::PopID();
		}
	ImGui::ListBoxFooter();
	return value_changed;
}
// }}}

// {{{ Popups
void UI::OpenPopupEx(ImGuiID id)
{
	ImGuiContext& g = *GImGui;
	ImGuiWindow* parent_window = g.CurrentWindow;
	int current_stack_size = g.CurrentPopupStack.Size;
	ImGuiPopupRef popup_ref; // Tagged as new ref as Window will be set back to NULL if we write this into OpenPopupStack.
	popup_ref.PopupId = id;
	popup_ref.Window = NULL;
	popup_ref.ParentWindow = parent_window;
	popup_ref.OpenFrameCount = g.FrameCount;
	popup_ref.OpenParentId = parent_window->IDStack.back();
	popup_ref.OpenMousePos = g.IO.MousePos;
	popup_ref.OpenPopupPos = NavCalcPreferredRefPos();

	//printf("[%05d] OpenPopupEx(0x%08X)\n", g.FrameCount, id);
	if (g.OpenPopupStack.Size < current_stack_size + 1)
	{
		g.OpenPopupStack.push_back(popup_ref);
	}
	else
	{
		// Gently handle the user mistakenly calling OpenPopup() every frame. It is a programming mistake! However, if we were to run the regular code path, the ui
		// would become completely unusable because the popup will always be in hidden-while-calculating-size state _while_ claiming focus. Which would be a very confusing
		// situation for the programmer. Instead, we silently allow the popup to proceed, it will keep reappearing and the programming error will be more obvious to understand.
		if (g.OpenPopupStack[current_stack_size].PopupId == id && g.OpenPopupStack[current_stack_size].OpenFrameCount == g.FrameCount - 1)
		{
			g.OpenPopupStack[current_stack_size].OpenFrameCount = popup_ref.OpenFrameCount;
		}
		else
		{
			// Close child popups if any, then flag popup for open/reopen
			g.OpenPopupStack.resize(current_stack_size + 1);
			g.OpenPopupStack[current_stack_size] = popup_ref;
		}

		// When reopening a popup we first refocus its parent, otherwise if its parent is itself a popup it would get closed by ClosePopupsOverWindow().
		// This is equivalent to what ClosePopupToLevel() does.
		//if (g.OpenPopupStack[current_stack_size].PopupId == id)
		//	FocusWindow(parent_window);
	}
}

void UI::OpenPopup(const char* str_id)
{
	ImGuiContext& g = *GImGui;
	OpenPopupEx(g.CurrentWindow->GetID(str_id));
}

bool UI::BeginPopupEx(ImGuiID id, ImGuiWindowFlags extra_flags)
{
	ImGuiContext& g = *GImGui;
	if (!ImGui::IsPopupOpen(id))
	{
		g.NextWindowData.Clear(); // We behave like Begin() and need to consume those values
		return false;
	}

	char name[20];
	if (extra_flags & ImGuiWindowFlags_ChildMenu)
		ImFormatString(name, IM_ARRAYSIZE(name), "##Menu_%02d", g.CurrentPopupStack.Size);	// Recycle windows based on depth
	else
		ImFormatString(name, IM_ARRAYSIZE(name), "##Popup_%08x", id); // Not recycling, so we can close/open during the same frame

	bool is_open = ImGui::Begin(name, NULL, extra_flags | ImGuiWindowFlags_Popup);
	if (!is_open) // NB: Begin can return false when the popup is completely clipped (e.g. zero size display)
	{
		EndPopup();
		return false;
	}

	return true;
}

bool UI::BeginPopup(const char* str_id, ImGuiWindowFlags flags )
{
	ImGuiContext& g = *GImGui;
	if (g.OpenPopupStack.Size <= g.CurrentPopupStack.Size) // Early out for performance
	{
		g.NextWindowData.Clear(); // We behave like Begin() and need to consume those values
		return false;
	}
	return BeginPopupEx(g.CurrentWindow->GetID(str_id), flags|ImGuiWindowFlags_AlwaysAutoResize|ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoSavedSettings);
}

void UI::EndPopup()
{
	ImGuiContext& g = *GImGui; (void)g;
	IM_ASSERT(g.CurrentWindow->Flags & ImGuiWindowFlags_Popup);  // Mismatched BeginPopup()/EndPopup() calls
	IM_ASSERT(g.CurrentPopupStack.Size > 0);

	// Make all menus and popups wrap around for now, may need to expose that policy.
	ImGui::NavMoveRequestTryWrapping(g.CurrentWindow, ImGuiNavMoveFlags_LoopY);

	ImGui::End();
}
// }}}
