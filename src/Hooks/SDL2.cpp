#include "../ImGUI/imgui.h"
#include "../ImGUI/imgui_internal.h"
#include "../ImGUI/imgui_freetype.h"
#include "../ImGUI/examples/imgui_impl_opengles3.h"
#include "../ImGUI/examples/libs/gl3w/GL/gl3w.h"
#include "../Shortcuts.hpp"
#include "../UI/UI.hpp"
#include "../UI/Style.hpp"
#include "Hooks.hpp"
#include "../Hacks/Visuals.hpp"
#include <SDL2/SDL.h>

#define INCBIN_PREFIX res
#include "../incbin/incbin.h"
INCBIN(PlexRegular, "../src/Resources/IBMPlexSans-Medium.ttf");
INCBIN(PlexBold, "../src/Resources/IBMPlexSans-Bold.ttf");
INCBIN(PlexMono, "../src/Resources/IBMPlexMono-Text.ttf");
INCBIN(ESPFont, "../src/Resources/ESPFont.ttf");
ImFont* UI::plex_regular = nullptr;
ImFont* UI::plex_bold = nullptr;
ImFont* UI::plex_mono = nullptr;
ImFont* UI::espfont = nullptr;

bool SDL2::wantRebuild = true;
u32 SDL2::fontFlags = 0;

std::uintptr_t oSwapWindow;
std::uintptr_t swapWindowOffset;
void (*oSDL_GL_SwapWindow)(SDL_Window*);

std::uintptr_t oPollEvent;
std::uintptr_t pollEventOffset;
i32 (*oSDL_PollEvent)(SDL_Event*);

static void HandleSDLEvent(SDL_Event* event)
{
	ImGuiIO& io = ImGui::GetIO();
	switch (event->type)
	{
		case SDL_MOUSEWHEEL:
			if (event->wheel.y > 0)
				io.MouseWheel = 1;
			if (event->wheel.y < 0)
				io.MouseWheel = -1;

			return;
		case SDL_MOUSEBUTTONDOWN:
			if (event->button.button == SDL_BUTTON_LEFT)
				io.MouseDown[0] = true;
			if (event->button.button == SDL_BUTTON_RIGHT)
				io.MouseDown[1] = true;
			if (event->button.button == SDL_BUTTON_MIDDLE)
				io.MouseDown[2] = true;

			return;
		case SDL_MOUSEBUTTONUP:
			if (event->button.button == SDL_BUTTON_LEFT)
				io.MouseDown[0] = false;
			if (event->button.button == SDL_BUTTON_RIGHT)
				io.MouseDown[1] = false;
			if (event->button.button == SDL_BUTTON_MIDDLE)
				io.MouseDown[2] = false;

			return;
		case SDL_TEXTINPUT:
			io.AddInputCharactersUTF8(event->text.text);

			return;
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			i32 key = event->key.keysym.sym & ~SDLK_SCANCODE_MASK;
			io.KeysDown[key] = (event->type == SDL_KEYDOWN);
			io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
			io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
			io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
			io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);

			return;
	}
}

static void SwapWindow(SDL_Window* window)
{
	static bool first = true;
	if (first)
	{
		// This sets the opengl function pointers and stuff
		gl3wInit();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		// Fixup some keycodes for SDL
		io.KeyMap[ImGuiKey_Tab] = SDLK_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
		io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
		io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
		io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
		io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
		io.KeyMap[ImGuiKey_Delete] = SDLK_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = SDLK_BACKSPACE;
		io.KeyMap[ImGuiKey_Enter] = SDLK_RETURN;
		io.KeyMap[ImGuiKey_Escape] = SDLK_ESCAPE;
		io.KeyMap[ImGuiKey_A] = SDLK_a;
		io.KeyMap[ImGuiKey_C] = SDLK_c;
		io.KeyMap[ImGuiKey_V] = SDLK_v;
		io.KeyMap[ImGuiKey_X] = SDLK_x;
		io.KeyMap[ImGuiKey_Y] = SDLK_y;
		io.KeyMap[ImGuiKey_Z] = SDLK_z;


		ImFontConfig config;
		config.OversampleH = 4;
		config.OversampleV = 4;
		config.PixelSnapH = true;
		config.SizePixels = 20.f;
		UI::plex_regular = io.Fonts->AddFontFromMemoryTTF(const_cast<u8*>(resPlexRegularData), resPlexRegularSize, 20.f, &config);
		UI::plex_bold = io.Fonts->AddFontFromMemoryTTF(const_cast<u8*>(resPlexBoldData), resPlexBoldSize, 20.f, &config);
		UI::plex_mono = io.Fonts->AddFontFromMemoryTTF(const_cast<u8*>(resPlexMonoData), resPlexMonoSize, 20.f, &config);

		constexpr static std::array<ImWchar, 25> ESPRange = {
			0x0020, 0x0787,
			0x0e01, 0x0e0c,
			0x13a0, 0x13de,
			0x1680, 0x1685,
			0x1d00, 0x1e28,
			0x2010, 0x22ba,
			0x2500, 0x2681,
			0x2801, 0x283f,
			0x2e81, 0x2e8a,
			0x2f00, 0x2f03,
			0xdb80, 0xdba7,
			0xe000, 0xf8ff, 0};
		ImFontConfig uniconfig;
		uniconfig.OversampleH = 3;
		uniconfig.OversampleV = 3;
		uniconfig.PixelSnapH = false;
		uniconfig.SizePixels = 18.f;
		UI::espfont = io.Fonts->AddFontFromMemoryTTF(const_cast<u8*>(resESPFontData), resESPFontSize, 18.f, &uniconfig, ESPRange.data());

		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.94f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border] = ImVec4(1.00f, 0.50f, 0.50f, 0.50f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.37f, 0.14f, 0.14f, 0.67f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.39f, 0.20f, 0.20f, 0.67f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.48f, 0.16f, 0.16f, 1.00f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 0.19f, 0.19f, 0.40f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.89f, 0.00f, 0.19f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.19f, 0.19f, 0.39f, 1.00f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.80f, 0.17f, 0.00f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.89f, 0.00f, 0.19f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.33f, 0.35f, 0.36f, 0.53f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.76f, 0.28f, 0.44f, 0.67f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.47f, 0.47f, 0.47f, 0.67f);
		colors[ImGuiCol_Separator] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.85f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.60f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.90f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
		colors[ImGuiCol_WindowBg] = ImGui::ColorConvertU32ToFloat4(Settings::Style::window_bg);
		colors[ImGuiCol_ChildBg] = ImGui::ColorConvertU32ToFloat4(Settings::Style::child_bg);

		ImGuiStyle* style = &ImGui::GetStyle();
		style->WindowPadding = ImVec2(0, 0);
		style->FramePadding = ImVec2(4, 4);
		style->ItemSpacing = ImVec2(0, 0);
		style->ItemInnerSpacing = ImVec2(4, 4);
		style->IndentSpacing = 10;
		style->ScrollbarSize = 12;
		style->GrabMinSize = 4;

		style->WindowRounding = 0;
		style->ChildRounding = 0;
		style->FrameRounding = 4;
		style->PopupRounding = 0;
		style->ScrollbarRounding = 4;
		style->GrabRounding = 0;

		style->WindowBorderSize = 4;
		style->ChildBorderSize = 0;
		style->PopupBorderSize = 0;
		style->FrameBorderSize = 0;

		ImGui_ImplOpenGL3_Init("#version 100");

		first = false;
	}

	ImGuiIO& io = ImGui::GetIO();
	i32 w, h;
	SDL_GetWindowSize(window, &w, &h);
	Paint::windowWidth = w;
	Paint::windowHeight = h;
	io.DisplaySize = ImVec2((float)w, (float)h);

	ImGuiStyle* style = &ImGui::GetStyle();
	style->AntiAliasedLines = Settings::Visuals::antialiasing;
	style->AntiAliasedFill = Settings::Visuals::antialiasing;

	if (SDL2::wantRebuild)
	{
		ImGuiFreeType::BuildFontAtlas(io.Fonts, SDL2::fontFlags);
		SDL2::wantRebuild = false;
		
ImGui_ImplOpenGL3_DestroyDeviceObjects();
ImGui_ImplOpenGL3_CreateDeviceObjects();
	}

	ImGui_ImplOpenGL3_NewFrame();

	static double lastTime = 0.0f;
	u32 time = SDL_GetTicks();
	double currentTime = time / 1000.0;
	io.DeltaTime = lastTime > 0.0 ? (float)(currentTime - lastTime) : (float)(1.0f / 60.0f);

	io.MouseDrawCursor = UI::isVisible && engine->IsInGame();
	io.WantCaptureMouse = UI::isVisible;
	io.WantCaptureKeyboard = UI::isVisible;

	if (UI::isVisible && !SetKeyCodeState::shouldListen)
	{
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				return;

			HandleSDLEvent(&event);
		}
	}

	if (io.WantCaptureMouse)
	{
		i32 mx, my;
		SDL_GetMouseState(&mx, &my);

		io.MousePos = ImVec2((float)mx, (float)my);

		SDL_ShowCursor(io.MouseDrawCursor ? 0 : 1);
	}

	ImGui::NewFrame();
	{
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_Always);
		ImGui::SetNextWindowBgAlpha(0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
		ImGui::Begin("", (bool*)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs);
		ImGui::PopStyleVar();

		UI::Draw();
		UI::DrawNotifications();
		Hooks::PaintImGui();

		ImGui::End();
	}
	ImGui::EndFrame();

	ImGui::GetCurrentContext()->Font->DisplayOffset = ImVec2(0.f, 0.f);
	ImGui::GetCurrentContext()->Font->DisplayOffset = ImVec2(0.f, 0.f);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	oSDL_GL_SwapWindow(window);
}

i32 PollEvent(SDL_Event* event)
{
	Shortcuts::PollEvent(event);

	return oSDL_PollEvent(event);
}

void SDL2::Hook()
{
	// SwapWindow
	const auto swapWindowFn = symbols["bin/linux64/libSDL2-2.0.so.0"s]["SDL_GL_SwapWindow"s] + 1;
	swapWindowOffset = reinterpret_cast<std::uintptr_t>(GetAbsoluteAddress(swapWindowFn, 1, 5));
	oSwapWindow = *reinterpret_cast<std::uintptr_t*>(swapWindowOffset);
	oSDL_GL_SwapWindow = reinterpret_cast<decltype(oSDL_GL_SwapWindow)>(oSwapWindow);
	*reinterpret_cast<std::uintptr_t*>(swapWindowOffset) = reinterpret_cast<std::uintptr_t>(&SwapWindow);

	// PollEvent
	const auto pollEventFn = symbols["bin/linux64/libSDL2-2.0.so.0"s]["SDL_PollEvent"s] + 1;
	pollEventOffset = reinterpret_cast<std::uintptr_t>(GetAbsoluteAddress(pollEventFn, 1, 5));
	oPollEvent = *reinterpret_cast<std::uintptr_t*>(pollEventOffset);
	oSDL_PollEvent = reinterpret_cast<decltype(oSDL_PollEvent)>(oPollEvent);
	*reinterpret_cast<std::uintptr_t*>(pollEventOffset) = reinterpret_cast<std::uintptr_t>(&PollEvent);
}

void SDL2::UnHook()
{
	*reinterpret_cast<std::uintptr_t*>(swapWindowOffset) = oSwapWindow;
	*reinterpret_cast<std::uintptr_t*>(pollEventOffset) = oPollEvent;
}
