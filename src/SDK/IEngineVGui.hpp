#ifndef SDK_IENGINEVGUI_HPP
#define SDK_IENGINEVGUI_HPP

#include "Defs.hpp"

MAKE_CENUM_Q(VGuiPanel, u32,
	ROOT, 0,
	GAMEUIDLL, 1,  // the console, game menu
	CLIENTDLL, 2,
	TOOLS, 3,
	INGAMESCREENS, 4,
	GAMEDLL, 5,
	CLIENTDLL_TOOLS, 6,
	GAMEUIBACKGROUND, 7, // the console background, shows under all other stuff in 3d engine view
);

class IEngineVGui : public non_constructible
{
public:
	virtual ~IEngineVGui() = 0;
	virtual VPANEL GetPanel(VGuiPanel type) = 0;
	virtual bool IsGameUIVisible() = 0;
	virtual void ActivateGameUI() = 0;
};

#endif // SDK_IENGINEVGUI_HPP
