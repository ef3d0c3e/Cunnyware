#ifndef HOOKER_HPP
#define HOOKER_HPP

#include "Util/Defs.hpp"

extern std::uintptr_t SetAbsOriginFnAddr;
typedef i32 (*GetSequenceActivityFn)(void*, i32); // C_BaseAnimating::GetSequenceActivity(int sequence).
extern GetSequenceActivityFn GetSeqActivity;

namespace Hooker
{
void FindPlayerAnimStateOffset();
void FindPlayerAnimOverlayOffset();
void FindAbsFunctions();
void FindSequenceActivity();
}

#endif // HOOKER_HPP
