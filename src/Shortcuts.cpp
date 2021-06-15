#include "Shortcuts.hpp"
#include "UI/UI.hpp"

void Shortcuts::PollEvent(SDL_Event* ev)
{
	if (((ev->key.keysym.sym == SDLK_INSERT && ev->type == SDL_KEYDOWN))
	|| ((ev->key.keysym.mod & KMOD_LALT) && ev->key.keysym.sym == SDLK_i && ev->type == SDL_KEYDOWN)){
		UI::SetVisible(!UI::isVisible);
    }
}

void Shortcuts::SetKeyCodeState(ButtonCode code, bool pressed)
{
}
