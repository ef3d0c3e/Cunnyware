#ifndef SDK_GAMEEVENTLISTENER_HPP
#define SDK_GAMEEVENTLISTENER_HPP

#include "Defs.hpp"
class IGameEvent;

#define EVENT_DEBUG_ID_INIT 42
#define EVENT_DEBUG_ID_SHUTDOWN 13

class GameEventListener
{
public:
	virtual ~GameEventListener() = 0;
	virtual void FireGameEvent(IGameEvent* event);
	virtual i32 GetEventDebugID() = 0;
};

#endif // SDK_GAMEEVENTLISTENER_HPP
