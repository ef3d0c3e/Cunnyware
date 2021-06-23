#ifndef UTIL_DRAW_HPP
#define UTIL_DRAW_HPP

#include "Defs.hpp"
#include "../ImGUI/imgui.h"
#include <deque>

MAKE_CENUM_Q(DrawType, u8,
	LINE, 0,
	RECT, 1,
	RECT_FILLED, 2,
	CIRCLE, 3,
	CIRCLE_FILLED, 4,
	CIRCLE_3D, 5,
	TEXT, 6);

MAKE_CENUM_Q(TextFlags, u8,
	None, 1<<1,
	Outline, 1<<2,
	Shadow, 1<<3);

struct DrawRequest
{
	DrawRequest() {}

	DrawType type;
	Rect2i rect;
	u32 segments;
	f32 radius;
	f32 thickness;
	ImColor color;
	TextFlags flags;
	Vec3 pos;
	std::string text;
};

namespace Draw
{
extern std::deque<DrawRequest> drawRequests;

void AddLine(Rect2i bounds, ImColor color, f32 thickness = 1.0f);
void AddRect(Rect2i rect, ImColor color, f32 thickness = 1.f, f32 rounding = 0.f, i32 flags = 0);
void AddRectFilled(Rect2i rect, ImColor color, f32 rounding = 0.f, i32 flags = 0);
void AddCircle(Vec2i center, f32 radius, ImColor color, u32 segments, f32 thickness);
void AddCircleFilled(Vec2i center, f32 radius, ImColor color, u32 segments);
void AddCircle3D(Vec3 pos, f32 radius, ImColor color, u32 segments, f32 thickness = 1.0f);
void AddText(Vec2i pos, std::string&& text, ImColor color, TextFlags flags);


void ImText(Vec2 pos, ImColor color, const std::string& text, TextFlags flags, f32 wrap_width, const Vec4* cpu_fine_clip_rect);
void ImCircle(Vec2 point, ImColor color, f32 radius, u32 num_segments, f32 thickness);
void ImCircleFilled(Vec2 point, ImColor color, f32 radius, u32 num_segments);
void ImRect(Rect2 rect, ImColor color, f32 rounding, i32 rounding_corners_flags, f32 thickness);
void ImRectFilled(Rect2 rect, ImColor color, f32 rounding, i32 rounding_corners_flags);
void ImLine(Rect2 rect, ImColor color, f32 thickness);
void ImCircle3D(Vec3 position, u32 segments, f32 radius, ImColor color, f32 thickness);
} // Draw

#endif // UTIL_DRAW_HPP
