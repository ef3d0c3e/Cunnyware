#ifndef HACKS_INFO_HPP
#define HACKS_INFO_HPP

#include "../Util/Defs.hpp"

class PlayerAdditionalInfo
{
	bool dormant; ///< Is dormant
	u32 dormantTime; ///< (ms) Since dormant state

	// ...store bt info
	
public:
	PlayerAdditionalInfo();
	~PlayerAdditionalInfo();

	static bool ShouldDraw(const class C_BasePlayer* player);
	static u32 DormantTime(const class C_BasePlayer* player);

	static void NewRound(); // Call at begining of every round
	static void Paint(); // Call every frame before everything else
};

#endif // HACKS_INFO_HPP
