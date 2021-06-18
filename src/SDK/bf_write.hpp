#ifndef SDK_BF_WRITE_HPP
#define SDK_BF_WRITE_HPP

#include "../Util/Defs.hpp"

class bf_write
{
public:
	// The current buffer.
	u8* data;
	i32 dataBytes;
	i32 dataBits;

	// Where we are in the buffer.
	i32 curBit;

	// Errors?
	bool overflow;

	bool assertOnOverflow;
	const char* debugName;
};

#endif // SDK_BF_WRITE_HPP
