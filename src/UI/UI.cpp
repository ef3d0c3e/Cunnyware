#include "UI.hpp"
#include "Tab.hpp"
#include "Tabs/Tabs.hpp"
#include "../Interface.hpp"
#include <fmt/chrono.h>

#include "Image.hpp"
#define INCBIN_PREFIX res
#include "../incbin/incbin.h"
INCBIN(Logo, "../src/Resources/logo.png");

bool UI::isVisible = false;

void UI::SetVisible(bool visible)
{
	UI::isVisible = visible;
	cvar->FindVar("cl_mouseenable")->SetValue(!UI::isVisible);
}

void UI::Draw()
{
	if (!UI::isVisible)
		return;

	constexpr static std::array<std::pair<std::string_view, std::string_view>, 6> tabsName
	{
		std::make_pair("a"sv, "Legit Bot"sv),
		std::make_pair("b"sv, "Rage Bot"sv),
		std::make_pair("c"sv, "Visuals"sv),
		std::make_pair("c"sv, "Misc"sv),
		std::make_pair("d"sv, "Skins"sv),
		std::make_pair("e"sv, "Configs"sv),
	};

	const static std::array<std::function<void()>, 6> tabs =
	{
		[]{}, []{}, Tabs::Visuals, []{}, []{}, Tabs::Configs,
	};

	
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Border] = ImGui::ColorConvertU32ToFloat4(Settings::Style::window_border);

	ImGui::SetNextWindowSize(ImVec2(948,645));
	ImGui::Begin("Cunnyware", NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	{
		ImGui::PushStyleColor(ImGuiCol_ChildBg, Settings::Style::tablist);
		ImGui::BeginChild("##TABLIST", ImVec2(0, 58), true);
		ImGui::PopStyleColor();
		static u64 tab = 0;

		float w = ImGui::GetContentRegionAvailWidth();
		ImGui::Dummy(ImVec2(0, 14));
		ImGui::Dummy(ImVec2(24, 0));
		ImGui::SameLine();
		ImRect bb;

		ImGui::PushFont(UI::plex_bold);
		for (u64 i = 0; i < tabs.size(); ++i)
		{
			if (UI::TabList(tabsName[i].second, tab, i, tabs.size(), bb, ImVec2(w*.12, 44)))
				tab = i;
			ImGui::SameLine();
		}
		ImGui::PopFont();

		{
			static auto logo = []
			{
				Image img(resLogoData, resLogoSize);
				img.Load();
				return img;
			}();

			const f32 scale = std::min(ImGui::GetContentRegionMax().x/logo.GetSize().x, ImGui::GetContentRegionMax().y/logo.GetSize().y);
			const ImVec2 logoSize(logo.GetSize().x*scale, logo.GetSize().y*scale);
			ImGui::SetCursorPos(ImVec2(ImGui::GetWindowSize().x-logoSize.x, 0));
			//ImGui::Image(reinterpret_cast<void*>(logo.GetTexture()), logoSize);
			// TODO
		}

		//ImGui::PushFont(UI::title_font);
		//const auto x_title = ImGui::GetCursorPosX() + (ImGui::GetContentRegionAvailWidth() - ImGui::CalcTextSize("Cunnyware").x)/2;
		//const auto h_title = ImGui::GetWindowSize().y;
		//ImGui::PopFont();
		ImGui::EndChild();

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::ColorConvertU32ToFloat4(Settings::Style::tablist_line));
		ImGui::BeginChild("##LINE", ImVec2(0, 2), true);
		ImGui::EndChild();
		ImGui::PopStyleColor();
		/*
		const auto cursor_restore = ImGui::GetCursorPos();


		ImGui::SetCursorPos(ImVec2(x_title, -4));
		ImGui::PushStyleColor(ImGuiCol_ChildBg, 0x00000000);
		ImGui::BeginChild("##TITLE", ImVec2(0, h_title));
		ImGui::PopStyleColor();
		*/
		
		

		/*
		//ImGui::PushFont(UI::title_font);
		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertU32ToFloat4(Settings::Style::title));
		ImGui::Text("Cunnyware");
		ImGui::PopStyleColor();
		//ImGui::PopFont();
		ImGui::EndChild();
		
		ImGui::SetCursorPos(cursor_restore);
		*/

		tabs[tab]();
	}
	ImGui::End();

}

void UI::AddNotification(const std::string& message, NotificationType type, u64 duration)
{
	const auto now = std::chrono::high_resolution_clock::now();
	notifications.push_back(Notification{
		.message = message,
		.type = type,
		.duration = std::chrono::milliseconds(duration),
		.start = now,
	});
	AddMessage(fmt::format("|{:%H:%M:%S}| {}", now, message), Settings::Style::notification_timer[type]);
}

std::deque<UI::Notification> UI::notifications = {};
void UI::DrawNotifications()
{
	const auto now = std::chrono::high_resolution_clock::now();
	
	ImGui::SetCursorPosY(8);
	for (auto it = UI::notifications.cbegin(); it != UI::notifications.cend();)
	{
		if (it->start + it->duration <= now)
		{
			it = UI::notifications.erase(it);
			continue;
		}

		const f32 ratio = 
			static_cast<f32>(std::chrono::duration_cast<std::chrono::milliseconds>(now - it->start).count()) /
			std::chrono::duration_cast<std::chrono::milliseconds>(it->duration).count();
		
		ImGui::Spacing();
		ImGui::SetCursorPosX(8);
		UI::NotificationMessage(it->message, it->type, ratio);
		++it;
	}
}

UI::Messages UI::messages = UI::Messages
{
	.msgs = std::vector<UI::LogMessage>(UI::Messages::Nums),
	.pos = 0
};
void UI::AddMessage(const std::string& message, ImU32 color)
{
	messages.msgs[messages.pos] = UI::LogMessage
	{
		.color = color,
		.message = message,
		.start = std::chrono::high_resolution_clock::now(),
	};

	messages.pos = (messages.pos+1) % UI::Messages::Nums;
}
