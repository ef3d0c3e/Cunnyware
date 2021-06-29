#include "Draw.hpp"
#include "../Hacks/Visuals.hpp"

std::deque<DrawRequest> Draw::drawRequests = {};

void Draw::AddLine(Rect2i bounds, ImColor color, f32 thickness)
{
	DrawRequest req = {};
	req.type = DrawType::LINE;
	req.rect = bounds;
	req.thickness = thickness;
	req.color = color;

	drawRequests.push_back(std::move(req));
}

void Draw::AddRect(Rect2i rect, ImColor color, f32 thickness, f32 rounding, i32 flags)
{
	DrawRequest req = {};
	req.type = DrawType::RECT;
	req.rect = rect;
	req.thickness = thickness;
	req.radius = rounding;
	req.flags = flags;
	req.color = color;

	drawRequests.push_back(std::move(req));
}

void Draw::AddRectFilled(Rect2i rect, ImColor color, f32 rounding, i32 flags)
{
	DrawRequest req = {};
	req.type = DrawType::RECT_FILLED;
	req.rect = rect;
	req.radius = rounding;
	req.flags = flags;
	req.color = color;

	drawRequests.push_back(std::move(req));
}

void Draw::AddCircle(Vec2i center, f32 radius, ImColor color, u32 segments, f32 thickness)
{
	DrawRequest req = {};
	req.type = DrawType::CIRCLE;
	req.rect.x = center;
	req.radius = radius;
	req.segments = segments;
	req.thickness = thickness;
	req.color = color;

	drawRequests.push_back(std::move(req));
}

void Draw::AddCircleFilled(Vec2i center, f32 radius, ImColor color, u32 segments)
{
	DrawRequest req = {};
	req.type = DrawType::CIRCLE_FILLED;
	req.rect.x = center;
	req.radius = radius;
	req.segments = segments;
	req.color = color;

	drawRequests.push_back(std::move(req));
}

void Draw::AddCircle3D(Vec3 pos3D, f32 radius, ImColor color, u32 segments, f32 thickness)
{
	DrawRequest req = {};
	req.type = DrawType::CIRCLE_3D;
	req.pos = pos3D;
	req.radius = radius;
	req.segments = segments;
	req.thickness = thickness;
	req.color = color;

	drawRequests.push_back(std::move(req));
}

void Draw::AddText(Vec2i pos, std::string&& text, ImColor color, TextFlags flags, f32 size)
{
	DrawRequest req = {};
	req.type = DrawType::TEXT;
	req.rect.x = pos;
	req.color = color;
	req.text = std::move(text);
	req.flags = flags;
	req.radius = size;

	drawRequests.push_back(std::move(req));
}


void Draw::ImText(Vec2 pos, ImColor color, const std::string& text, f32 size, TextFlags flags, f32 wrap_width, const Vec4* cpu_fine_clip_rect_)
{
	ImColor shading;
	shading.Value.x = 0;
	shading.Value.y = 0;
	shading.Value.z = 0;
	shading.Value.w = color.Value.w / 2;
	const ImVec4* cpu_fine_clip_rect = reinterpret_cast<const ImVec4*>(cpu_fine_clip_rect_);

	const auto text_begin = text.data();
	const auto text_end = text.data()+text.size();

	if (flags & TextFlags::Outline)
	{
		ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), size, ImVec2(pos.x - 1, pos.y - 1),
											shading, text_begin, text_end, wrap_width,
											cpu_fine_clip_rect);
		ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), size, ImVec2(pos.x + 2, pos.y),
											shading, text_begin, text_end, wrap_width,
											cpu_fine_clip_rect);
		ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), size, ImVec2(pos.x, pos.y + 2),
											shading, text_begin, text_end, wrap_width,
											cpu_fine_clip_rect);
		ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), size, ImVec2(pos.x - 2, pos.y),
											shading, text_begin, text_end, wrap_width,
											cpu_fine_clip_rect);
	}

	if (flags & TextFlags::Shadow)
		ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), size, ImVec2(pos.x + 1, pos.y + 1),
											shading, text_begin, text_end, wrap_width,
											cpu_fine_clip_rect);

	ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), size, {pos.x, pos.y}, color, text_begin, text_end,
										wrap_width, cpu_fine_clip_rect);
}

void Draw::ImCircle(Vec2 point, ImColor color, f32 radius, u32 num_segments, f32 thickness)
{
	ImGui::GetWindowDrawList()->AddCircle({point.x, point.y}, radius, color, num_segments, thickness);
}
void Draw::ImCircleFilled(Vec2 point, ImColor color, f32 radius, u32 num_segments)
{
	ImGui::GetWindowDrawList()->AddCircleFilled({point.x, point.y}, radius, color, num_segments);
}

void Draw::ImRect(Rect2 rect, ImColor color, f32 rounding, i32 rounding_corners_flags, f32 thickness)
{
	ImGui::GetWindowDrawList()->AddRect({rect.x.x, rect.x.y}, {rect.y.x, rect.y.y}, color, rounding, rounding_corners_flags, thickness);
}

void Draw::ImRectFilled(Rect2 rect, ImColor color, f32 rounding, i32 rounding_corners_flags)
{
	ImGui::GetWindowDrawList()->AddRectFilled({rect.x.x, rect.x.y}, {rect.y.x, rect.y.y}, color, rounding, rounding_corners_flags);
}
void Draw::ImLine(Rect2 rect, ImColor color, f32 thickness)
{
	ImGui::GetWindowDrawList()->AddLine({rect.x.x, rect.x.y}, {rect.y.x, rect.y.y}, color, thickness);
}

void Draw::ImCircle3D(Vec3 position, u32 segments, f32 radius, ImColor color, f32 thickness)
{
	float step = (float)M_PI * 2.0f / (float)segments;

	for (float a = 0; a < (M_PI * 2.0f); a += step)
	{
		Vec3 start(radius * cosf(a) + position.x,
					 radius * sinf(a) + position.y,
					 position.z);
		Vec3 end(radius * cosf(a + step) + position.x,
				   radius * sinf(a + step) + position.y,
				   position.z);

		Vec2 start2d, end2d;
		if (!ESP::WorldToScreen(start, start2d) || !ESP::WorldToScreen(end, end2d))
			continue;

		Draw::ImLine(Rect2{start2d, end2d}, color, thickness);
	}
}
