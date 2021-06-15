#ifndef UI_UI_HPP
#define UI_UI_HPP

#include <imgui.h>

namespace UI
{
	////////////////////////////////////////////////
	/// \brief Indicates wether or not to display the UI (menu)
	////////////////////////////////////////////////
	extern bool isVisible;

	extern ImFont* plex_regular;
	extern ImFont* plex_bold;
	extern ImFont* plex_mono;

	////////////////////////////////////////////////
	/// \brief Set the UI visible or not
	////////////////////////////////////////////////
	void SetVisible(bool visible);

	////////////////////////////////////////////////
	/// \brief Draw every ImGui-related widget
	////////////////////////////////////////////////
	void Draw();
};

#endif // UI_UI_HPP
