#ifndef HOOKER_HPP
#define HOOKER_HPP

#include "Util/Defs.hpp"
class KeyValues;

extern std::uintptr_t SetAbsOriginFnAddr;
typedef i32 (*GetSequenceActivityFn)(void*, i32); // C_BaseAnimating::GetSequenceActivity(int sequence).
extern GetSequenceActivityFn GetSeqActivity;
typedef void (*InitKeyValuesFn)(KeyValues* kv, const char* name);
extern InitKeyValuesFn InitKeyValues;
typedef void (*LoadFromBufferFn)(KeyValues* kv, const char* name, const char* buffer /* \0-term */, void*, const char*, void*);
extern LoadFromBufferFn LoadFromBuffer;

namespace Hooker
{
void FindPlayerAnimStateOffset();
void FindPlayerAnimOverlayOffset();
void FindAbsFunctions();
void FindSequenceActivity();
void FindPlayerResource();
void FindInitKeyValues();
void FindLoadFromBuffer();
}

#endif // HOOKER_HPP
