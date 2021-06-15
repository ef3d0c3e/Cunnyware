#ifndef SDK_CUTL_BYTESWAP_HPP
#define SDK_CUTL_BYTESWAP_HPP

#include "../../Util/Defs.hpp"

class CByteswap : non_constructible
{
public:
	u32 swapBytes = 1;
	u32 bigEndian = 1;
};

#endif // SDK_CUTL_BYTESWAP_HPP
