#ifndef UI_UI_HPP
#define UI_UI_HPP

#include <imgui.h>
#include "../Util/Defs.hpp"
#include <chrono>
#include <deque>

namespace UI
{
	////////////////////////////////////////////////
	/// \brief Indicates wether or not to display the UI (menu)
	////////////////////////////////////////////////
	extern bool isVisible;

	extern ImFont* plex_regular;
	extern ImFont* plex_bold;
	extern ImFont* plex_mono;
	extern ImFont* espfont;

	////////////////////////////////////////////////
	/// \brief Set the UI visible or not
	////////////////////////////////////////////////
	void SetVisible(bool visible);

	////////////////////////////////////////////////
	/// \brief Draw main window
	////////////////////////////////////////////////
	void Draw();

	MAKE_CENUM_Q(NotificationType, u8,
		MESSAGE, 0,
		ERROR, 1);
	struct Notification
	{
		std::string message;
		NotificationType type;

		std::chrono::milliseconds duration;
		std::chrono::time_point<std::chrono::high_resolution_clock> start;
	};
	extern std::deque<Notification> notifications;
	void AddNotification(const std::string& message, NotificationType type, u64 duration);

	////////////////////////////////////////////////
	/// \brief Draw notifications
	////////////////////////////////////////////////
	void DrawNotifications();

	struct LogMessage
	{
		ImU32 color;
		std::string message;
		std::chrono::time_point<std::chrono::high_resolution_clock> start;
	};
	struct Messages // Cyclic buffer
	{
		static constexpr inline u64 Nums = 1024; ///< Maximum number of messages
		std::vector<LogMessage> msgs;
		u64 pos;
	};
	extern Messages messages;
	void AddMessage(const std::string& message, ImU32 color);
};

#endif // UI_UI_HPP
