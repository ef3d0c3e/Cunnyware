#include "Hooker.hpp"
#include "Offsets.hpp"
#include "Util/Util.hpp"

u32 Offsets::playerAnimStateOffset = 0;
void Hooker::FindPlayerAnimStateOffset()
{
	// In C_CSPlayer::Spawn, it references the offset of each player's animation state so it can do a reset.
	// mov     rdi, [rbx+4148h] <----- this offset(0x4148)
	// test    rdi, rdi
	// jz      short loc_C50837
	// call    AnimState__Reset
	// 55 48 89 E5 53 48 89 FB 48 83 EC 28 48 8B 05 ?? ?? ?? ?? 48 8B 00
	std::uintptr_t C_CSPlayer__Spawn = FindPatternInModule("/client_client.so",
					u8"\x55\x48\x89\xE5\x53\x48\x89\xFB\x48\x83\xEC\x28\x48\x8B\x05"
					"\x00\x00\x00\x00" //??
					"\x48\x8B\x00",
					"xxxxxxxxxxxxxxx????xxx");
	Offsets::playerAnimStateOffset = *(unsigned int*)(C_CSPlayer__Spawn + 52);
}

u32 Offsets::playerAnimOverlayOffset = 0;
void Hooker::FindPlayerAnimOverlayOffset()
{
	// C_BaseAnimatingOverlay::GetAnimOverlay - Adding 35 to get to the offset
	// 55 48 89 E5 41 56 41 55 41 89 F5 41 54 53 48 89 FB 8B
	Offsets::playerAnimOverlayOffset = *(u32*)(FindPatternInModule("/client_client.so",
					u8"\x55\x48\x89\xE5\x41\x56\x41\x55\x41\x89\xF5\x41\x54\x53\x48\x89\xFB\x8B",
				    "xxxxxxxxxxxxxxxxxx") + 35);
}

std::uintptr_t SetAbsOriginFnAddr = 0;
void Hooker::FindAbsFunctions()
{
	// C_BaseEntity::SetAbsOrigin()
	// 55 48 89 E5 41 55 41 54 49 89 F4 53 48 89 FB 48 83 EC ?? E8 ?? ?? ?? ?? F3
	SetAbsOriginFnAddr = FindPatternInModule("/client_client.so",
					u8"\x55\x48\x89\xE5\x41\x55\x41\x54\x49\x89\xF4\x53\x48\x89\xFB\x48\x83\xEC"
					"\x00" //??
					"\xE8"
					"\x00\x00\x00\x00" //??
					"\xF3",
					"xxxxxxxxxxxxxxxxxx?x????x");
}
