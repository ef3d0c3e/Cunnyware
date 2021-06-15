#ifndef SHORTCUTS_HPP
#define SHORTCUTS_HPP

#include "SDK/ButtonCode.hpp"
#include <SDL2/SDL.h>

namespace Shortcuts
{
	void PollEvent(SDL_Event* ev);
	void SetKeyCodeState(ButtonCode code, bool pressed);
}

#endif // SHORTCUTS_HPP
