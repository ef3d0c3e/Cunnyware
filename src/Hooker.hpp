#ifndef HOOKER_HPP
#define HOOKER_HPP

#include "Util/Defs.hpp"

extern std::uintptr_t SetAbsOriginFnAddr;

namespace Hooker
{
void FindPlayerAnimStateOffset();
void FindPlayerAnimOverlayOffset();
void FindAbsFunctions();
}

#endif // HOOKER_HPP
