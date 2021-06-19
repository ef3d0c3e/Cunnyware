#ifndef SDK_IVPANEL_HPP
#define SDK_IVPANEL_HPP

#include "Defs.hpp"

class IVPanel : public non_constructible
{
public:
	const char* GetName(VPANEL vguiPanel)
	{
		return getvfunc<const char* (*)(void*, VPANEL)>(this, 37)(this, vguiPanel);
	}
};

#endif // SDK_IVPANEL_HPP
