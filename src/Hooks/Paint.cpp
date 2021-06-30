#include "../Hacks/Visuals.hpp"
#include "../Hacks/Info.hpp"
#include "../Util/Draw.hpp"
#include "../UI/UI.hpp"
#include "Hooks.hpp"
#include <mutex>

i32 Paint::windowWidth = 0;
i32 Paint::windowHeight = 0;

i32 Paint::engineWidth = 0;
i32 Paint::engineHeight = 0;

std::mutex drawMutex;

void Hooks::Paint(void* thisptr, PaintMode mode)
{
	engineVGuiVMT->GetOriginalMethod<void (*)(void*, PaintMode)>(15)(thisptr, mode);

	engine->GetScreenSize(Paint::engineHeight, Paint::engineWidth);

	PlayerAdditionalInfo::Paint();

	if (mode & PaintMode::UIPANELS)
	{
		static bool first = true;
		if (first)
		{
			first = false;
			return;
		}
		std::unique_lock<std::mutex> lock(drawMutex);

		u64 prevRecords = Draw::drawRequests.size();

		ESP::Paint();

		Draw::drawRequests.erase(Draw::drawRequests.begin(), Draw::drawRequests.begin() + prevRecords);
	}
}

void Hooks::PaintImGui()
{
	std::unique_lock<std::mutex> lock(drawMutex);

	const Vec2 engineSize(Paint::engineWidth, Paint::engineHeight);
	const Vec2 imSize(Paint::windowWidth, Paint::windowHeight);

	ImGui::PushFont(UI::espfont);

	for (const DrawRequest& req : Draw::drawRequests)
	{
		Rect2 rect{req.rect.x.As<f32>(), req.rect.y.As<f32>()};
		switch (req.type)
		{
			case DrawType::LINE:
				Draw::ImLine(rect, req.color, req.thickness);
				break;
			case DrawType::RECT:
				Draw::ImRect(rect, req.color, req.radius, req.flags, req.thickness);
				break;
			case DrawType::RECT_FILLED:
				Draw::ImRectFilled(rect, req.color, req.radius, req.flags);
				break;
			case DrawType::CIRCLE:
				Draw::ImCircle(rect.x, req.color, req.radius, req.segments, req.thickness);
				break;
			case DrawType::CIRCLE_FILLED:
				Draw::ImCircleFilled(rect.x, req.color, req.radius, req.segments);
				break;
			case DrawType::CIRCLE_3D:
				Draw::ImCircle3D(req.pos, req.segments, req.radius, req.color, req.thickness);
				break;
			case DrawType::TEXT:
				Draw::ImText(rect.x, req.color, req.text, req.radius, req.flags, 0, nullptr);
				break;
		}
	}

	ImGui::PopFont();
}
