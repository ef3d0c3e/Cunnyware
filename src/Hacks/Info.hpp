#ifndef HACKS_INFO_HPP
#define HACKS_INFO_HPP

#include "../Util/Defs.hpp"
#include "../SDK/PlayerInfo.hpp"

class PlayerAdditionalInfo
{
	bool dormant; ///< Is dormant
	u32 dormantTime; ///< (ms) Since dormant state
	bool visible; ///< Is playerr visible
	PlayerInfo info;

	// ...store bt info
	
public:
	PlayerAdditionalInfo();
	~PlayerAdditionalInfo();

	static bool ShouldDraw(const class C_BasePlayer* player);
	static bool IsDormant(const class C_BasePlayer* player);
	static u32 DormantTime(const class C_BasePlayer* player);
	inline static u32 GetMaxDormantTime() { return 1500; } ///< (ms) To still draw players after going dormant
	static bool IsVisible(const class C_BasePlayer* player);
	static const std::array<char, PlayerInfo::MaxPlayerNameLength>& GetName(const class C_BasePlayer* player);

	static void NewRound(); // Call at begining of every round
	static void Paint(); // Call every frame before everything else
};

#endif // HACKS_INFO_HPP
