#ifndef SDK_PLAYERINFO_HPP
#define SDK_PLAYERINFO_HPP

#include "../Util/Defs.hpp"

typedef u64 CRC32;

struct PlayerInfo // player_info_t
{
	static constexpr inline std::size_t MaxPlayerNameLength = 128;
	u64 unknown;
	// network xuid
	u64 xuid;
	// scoreboard information
	std::array<char, MaxPlayerNameLength> name;
	// local server user ID, unique while server is running
	i32 userID;
	// global unique player identifer
	static constexpr inline std::size_t SignedGuidLen = 32;
	std::array<char, SignedGuidLen+1> guid;
	// friends identification number
	u32 friendsID;
	// friends name
	std::array<char, MaxPlayerNameLength> friendsName;
	// true, if player is a bot controlled by game.dll
	bool fakePLAYER;
	// true if player is the HLTV proxy
	bool isHLTV;
	// custom files CRC for this player
	static constexpr inline std::size_t MaxCustomFiles = 4;
	std::array<CRC32, MaxCustomFiles> customFiles;
	// this counter increases each time the server downloaded a new file
	u8 filesDownloaded;
};

#endif // SDK_PLAYERINFO_HPP
