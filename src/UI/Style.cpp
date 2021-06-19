#include "Style.hpp"

namespace Settings::Style
{
	// Window
	float child_padding = 10;
	ImU32 window_bg = 0xFF1E1E1E;
	ImU32 window_accent = 0xFFF02020;
	ImU32 window_border = 0x70232323;
	//ImU32 title = 0xFFFF0000;
	ImU32 title = 0xFFFFFFFF;
	ImU32 child_bg = 0xFF232323;
	ImU32 child_border = 0xFF030303;
	ImU32 tablist = 0xFF0F0F0F; //#DC3600;
	ImU32 tablist_bg1[4] = {0xFF292929, 0xFF292929, 0xFF252525, 0xFF191919}; // normal, selected, hovered, active
	ImU32 tablist_bg2[4] = {0xFF111111, 0xFF191919, 0xFF151515, 0xFF090909}; // normal, selected, hovered, active
	ImU32 tablist_accent[2] = {0xFFF0F0F0, 0xFFA9FF5F};
	ImU32 tablist_line = 0xFFA9FF5F;
	ImU32 tablist_border[2] = {0xFF101010, 0xFF292929}; // out, in
	float tablist_current_standout = 8.f;
	ImU32 tablist_shadow = 0x40000000;
	float tablist_shadow_scale = 0.01f;

	// Tooltip
	ImVec2 tooltip_padding(6.f, 6.f);
	float tooltip_rounding = 4.f;
	float tooltip_border = 0.f;
	ImU32 tooltip_color[3] = { 0xFFCACACA, 0xD0D0D0D0, 0xAA1D1D1D }; // text, border, background

	// Section
	ImU32 section_sep_left = 0xA090F090;
	ImU32 section_sep_right = 0x00000000;


	// Buttons
	ImU32 button_text = 0xFF202020;
	ImU32 button_bg[3] = { 0xFFB8B8B8, 0xFFCCCCCC, 0xFFAAAAAA }; // normal, hovered, active
	ImU32 button2_border = 0xFF030303;
	ImU32 button2_text = 0xFFF0F0F0;
	ImU32 button2_bg[3] = { 0xFF363432, 0xFF282825, 0xFF181716 }; // normal, hovered, active

	ImU32 selector_bg[4] = {0x00000000, 0xFF2C2C2C, 0xFF202020, 0xFF101010}; // normal, selected, hovered, active
	ImU32 selector_accent = 0xFF68E000;
	ImU32 selector_border = 0xFF4E4E4E;
	ImU32 selector_shadow = 0x40000000;
	float selector_shadow_scale = 0.05f;

	// Sliders
	float slider_thickness = 0.7f;
	float slider_value_offset = -2.f;
	ImU32 slider_grab = 0xFFB5B5B5;
	ImU32 slider_filled = 0xFF629E2E;
	ImU32 slider_empty = 0xFF353535;
	ImU32 slider_border = 0xFF030303;

	// Checkbox
	ImU32 checkmark = 0xFF629E2E;
	ImU32 checkbox_bg[4] = { 0xFF272727, 0xFF383838, 0xFF484848, 0xFF282828 }; // normal, checked, hovered, active
	ImU32 checkbox_border = 0xFF101010;
	f32 checkbox_spacing = 8.f;

	// InputLine
	ImU32 textinput_border = 0xFF030303;
	ImU32 textinput_bg = 0xFF363432;
	ImU32 textinput_text = 0xFFD0D0D0;

	// Combo
	ImU32 arrow_foreground[3] = {0xFF707070, 0xFF707070, 0xFF707070}; // normal, hovered, open
	ImU32 arrow_background[3] = {0xFF282828, 0xFF282828, 0xFF282828}; // normal, hovered, open
	ImU32 combo_text = 0xFF979797;
	ImU32 combo_border = 0xFF030303;
	ImU32 combo_background[3] = {0xFF282828, 0xFF282828, 0xFF282828}; // normal, hovered, open
	ImU32 combo_popup[5] = {0xFF030303, 0xFF282828, 0xFF101010, 0xFF1F1F1F, 0xFF1A1A1A}; // border, background, current, hovered, active
	float combo_text_padding = 10.f;
	float combo_vertical_padding = 4.f;

	// Colors
	ImVec2 color_button_scale(1.3f, .7f);
	ImU32 color_button_border = 0x50FFFFFF;
}
