#ifndef SDK_CBASEPHONEMETAG_HPP
#define SDK_CBASEPHONEMETAG_HPP

#include "../Util/Defs.hpp"

class CBasePhonemeTag : non_constructible
{
public:
	f32 startTime;
	f32 endTime;
	u16 phonemeCode;
};

#endif // SDK_CBASEPHONEMETAG_HPP
