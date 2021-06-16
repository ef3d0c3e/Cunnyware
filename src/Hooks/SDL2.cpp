#include "../ImGUI/imgui.h"
#include "../ImGUI/examples/imgui_impl_opengles3.h"
#include "../ImGUI/examples/libs/gl3w/GL/gl3w.h"
#include "../ImGUI/imgui_internal.h"
#include "../Shortcuts.hpp"
#include "../UI/UI.hpp"
#include "Hooks.hpp"
#include <SDL2/SDL.h>

#define INCBIN_PREFIX res
#include "../incbin/incbin.h"
INCBIN(PlexRegular, "../src/Resources/IBMPlexSans-Medium.ttf");
INCBIN(PlexBold, "../src/Resources/IBMPlexSans-Bold.ttf");
INCBIN(PlexMono, "../src/Resources/IBMPlexMono-Text.ttf");
ImFont* UI::plex_regular = nullptr;
ImFont* UI::plex_bold = nullptr;
ImFont* UI::plex_mono = nullptr;

std::uintptr_t oSwapWindow;
std::uintptr_t swapWindowOffset;
void (*oSDL_GL_SwapWindow)(SDL_Window*);

std::uintptr_t oPollEvent;
std::uintptr_t pollEventOffset;
int (*oSDL_PollEvent)(SDL_Event*);

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
			int key = event->key.keysym.sym & ~SDLK_SCANCODE_MASK;
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

        ImGui_ImplOpenGL3_Init("#version 100");

		first = false;
	}

	ImGuiIO& io = ImGui::GetIO();
	int w, h;
	SDL_GetWindowSize(window, &w, &h);
	Paint::windowWidth = w;
	Paint::windowHeight = h;
	io.DisplaySize = ImVec2((float)w, (float)h);

	ImGui_ImplOpenGL3_NewFrame();

	static double lastTime = 0.0f;
	Uint32 time = SDL_GetTicks();
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
		int mx, my;
		SDL_GetMouseState(&mx, &my);

		io.MousePos = ImVec2((float)mx, (float)my);

		SDL_ShowCursor(io.MouseDrawCursor ? 0 : 1);
	}

	ImGui::NewFrame();
	{
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_Always);
		ImGui::SetNextWindowBgAlpha(0.0f);
		ImGui::Begin("", (bool*)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs);
		{
			UI::Draw();
		}
		ImGui::End();
	}
	ImGui::EndFrame();

	ImGui::GetCurrentContext()->Font->DisplayOffset = ImVec2(0.f, 0.f);
	ImGui::GetCurrentContext()->Font->DisplayOffset = ImVec2(0.f, 0.f);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	oSDL_GL_SwapWindow(window);
}

int PollEvent(SDL_Event* event)
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
