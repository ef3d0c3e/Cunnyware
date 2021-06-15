#ifndef SDK_CLIENTTEXTMESSAGE_HPP
#define SDK_CLIENTTEXTMESSAGE_HPP

#include "../Util/Defs.hpp"
#include "Color.hpp"

struct ClientTextmessage
{
	i32 effect;
	ColorRGBA c1, c2;
	Vec2 pos;
	f32 fadein;
	f32 fadeout;
	f32 holdtime;
	f32 fxtime;
	const char* VGuiSchemeFontName; // If null, use default font for messages
	const char* name;
	const char* message;
	bool RoundedRectBackdropBox;
	f32 BoxSize; // as a function of font height
	std::array<u8, 4> boxcolor;
	char const* clearMessage; // message to clear
};

#endif // SDK_CLIENTTEXTMESSAGE_HPP
