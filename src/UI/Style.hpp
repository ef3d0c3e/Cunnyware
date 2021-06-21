#ifndef UI_STYLE_HPP
#define UI_STYLE_HPP

#include "../ImGUI/imgui.h"
#include "../Util/Defs.hpp"

struct ColorRGBA;
namespace UI
{
	ColorRGBA ToRGBA(ImU32 col);
}

namespace Settings::Style
{

	// Window
	extern float child_padding;
	extern ImU32 window_bg;
	extern ImU32 window_accent;
	extern ImU32 window_border;
	//extern ImU32 title;
	extern ImU32 title;
	extern ImU32 child_bg;
	extern ImU32 child_border;
	extern ImU32 tablist;
	extern ImU32 tablist_bg1[4]; // normal, selected, hovered, active
	extern ImU32 tablist_bg2[4]; // normal, selected, hovered, active
	extern ImU32 tablist_accent[2]; // normal, selected
	extern ImU32 tablist_line;
	extern ImU32 tablist_border[2];
	extern float tablist_current_standout;
	extern ImU32 tablist_shadow;
	extern float tablist_shadow_scale;

	// Tooltip
	extern ImVec2 tooltip_padding;
	extern float tooltip_rounding;
	extern float tooltip_border;
	extern ImU32 tooltip_color[3]; // text, border, background

	// Section
	extern ImU32 section_sep_left;
	extern ImU32 section_sep_right;


	// Buttons
	extern ImU32 button_text;
	extern ImU32 button_bg[3]; // normal, hovered, active
	extern ImU32 button2_border;
	extern ImU32 button2_text;
	extern ImU32 button2_bg[3]; // normal, hovered, active

	extern ImU32 selector_bg[4]; // normal, selected, hovered, active
	extern ImU32 selector_accent;
	extern ImU32 selector_border;
	extern ImU32 selector_shadow;
	extern float selector_shadow_scale;

	// Sliders
	extern f32 slider_thickness;
	extern f32 slider_value_offset;
	extern ImU32 slider_grab;
	extern ImU32 slider_filled;
	extern ImU32 slider_empty;
	extern ImU32 slider_border;

	// Checkbox
	extern ImU32 checkmark;
	extern ImU32 checkbox_bg[4]; // normal, checked, hovered, active
	extern ImU32 checkbox_border;
	extern f32 checkbox_spacing;

	// InputLine
	extern ImU32 textinput_border;
	extern ImU32 textinput_bg;
	extern ImU32 textinput_text;

	// Combo
	extern ImU32 arrow_foreground[3]; // normal, hovered, open
	extern ImU32 arrow_background[3]; // normal, hovered, open
	extern ImU32 combo_text;
	extern ImU32 combo_border;
	extern ImU32 combo_background[3]; // normal, hovered, open
	extern ImU32 combo_popup[5]; // border, background, current, hovered, active
	extern float combo_text_padding;
	extern float combo_vertical_padding;

	// Colors
	extern ImVec2 color_button_scale; // scale ot line height
	extern ImU32 color_button_border;

	// Notifications
	extern ImU32 notification_bg;
	extern ImU32 notification_text;
	extern ImU32 notification_border;
	extern std::array<ImU32, 2> notification_timer;

	// Pie
	extern ImU32 pie_text;
	extern ImU32 pie_button[3]; // normal, hovered, selected
	extern ImU32 pie_bg;
}

#endif // UI_STYLE_HPP
