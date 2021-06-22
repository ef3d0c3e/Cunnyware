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

GetSequenceActivityFn GetSeqActivity;

void Hooker::FindSequenceActivity()
{
	// C_BaseAnimating::GetSequenceActivity()
	// 83 FE FF                cmp     esi, 0FFFFFFFFh
	// 74 6B                   jz      short loc_7A1F40
	// 55                      push    rbp
	// 48 89 E5                mov     rbp, rsp
	// 53                      push    rbx
	// 48 89 FB                mov     rbx, rdi
	// 48 83 EC 18             sub     rsp, 18h
	// 48 8B BF D0 2F 00 00    mov     rdi, [rdi+2FD0h]
	// 48 85 FF                test    rdi, rdi
	// 74 13                   jz      short loc_7A1F00
	// loc_7A1EED:             ; CODE XREF: GetSequenceActivity+5F↓j
	// 48 83 3F 00             cmp     qword ptr [rdi], 0
	// 74 3E                   jz      short loc_7A1F31
	// 48 83 C4 18             add     rsp, 18h
	// 31 D2                   xor     edx, edx
	// 83 FE FF 74 ?? 55 48 89 E5 53 48 89 FB 48 83 EC ?? 48 8B BF ?? ?? ?? ?? 48 85 FF 74 ?? 48 83 3F 00 74 ?? 48 83 C4 ?? 31
	uintptr_t funcAddr = FindPatternInModule("/client_client.so",
			u8"\x83\xFE\xFF"
			"\x74\x00"
			"\x55"
			"\x48\x89\xE5"
			"\x53"
			"\x48\x89\xFB"
			"\x48\x83\xEC\x00"
			"\x48\x8B\xBF\x00\x00\x00\x00"
			"\x48\x85\xFF"
			"\x74\x00"
			"\x48\x83\x3F\x00"
			"\x74\x00"
			"\x48\x83\xC4\x00"
			"\x31",
			"xxxx?xxxxxxxxxxx?xxx????xxxx?xxxxx?xxx?x");

	GetSeqActivity = reinterpret_cast<GetSequenceActivityFn>(funcAddr);
}
