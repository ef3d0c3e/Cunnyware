#ifndef SDK_CCOMMAND_HPP
#define SDK_CCOMMAND_HPP

#include "../Util/Defs.hpp"
#include <array>

struct Characterset
{
	std::array<char, 256> set;
};

class CCommand
{
public:
	static constexpr inline std::size_t CommandMaxArgc = 64;
	static constexpr inline std::size_t CommandMaxLength = 512;

	i32 argc;
	i32 argv0size;
	std::array<char, CommandMaxLength> argsBuffer;
	std::array<char, CommandMaxLength> argvBuffer;
	std::array<const char, CommandMaxArgc> argv;
};

#endif // SDK_CCOMMAND_HPP
