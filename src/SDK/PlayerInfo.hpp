#ifndef SDK_PLAYERINFO_HPP
#define SDK_PLAYERINFO_HPP

#include "../Util/Defs.hpp"

typedef u64 CRC32;

struct PlayerInfo // player_info_t
{
	static constexpr inline std::size_t MaxPlayerNameLength = 128;
	u64 unknown;
	// Network xuid
	u64 xuid;
	// Scoreboard information
	std::array<char, MaxPlayerNameLength> name;
	// Local server user ID, unique while server is running
	i32 userID;
	// Global unique player identifer
	static constexpr inline std::size_t SignedGuidLen = 32;
	std::array<char, SignedGuidLen+1> guid;
	// Friends identification number
	u32 friendsID;
	// Friends name
	std::array<char, MaxPlayerNameLength> friendsName;
	// True, if player is a bot controlled by game.dll
	bool fakePLAYER;
	// True if player is the HLTV proxy
	bool isHLTV;
	// Custom files CRC for this player
	static constexpr inline std::size_t MaxCustomFiles = 4;
	std::array<CRC32, MaxCustomFiles> customFiles;
	// This counter increases each time the server downloaded a new file
	u8 filesDownloaded;
};

#endif // SDK_PLAYERINFO_HPP
