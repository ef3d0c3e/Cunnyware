#ifndef SDK_ICLIENTENTITY_HPP
#define SDK_ICLIENTENTITY_HPP

#include "IClientUnknown.hpp"
#include "IClientRenderable.hpp"
#include "IClientNetworkable.hpp"
#include "IClientThinkable.hpp"

class CMouthInfo;
struct SpatializationInfo; // SpatializationInfo_t

class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
	// Delete yourself.
	virtual void Release() = 0;

	// Network origin + angles
	virtual const Vec3& GetAbsOrigin() const = 0;
	virtual const QAngle& GetAbsAngles() const = 0;

	virtual CMouthInfo* GetMouth() = 0;

	// Retrieve sound spatialization info for the specified sound on this entity
	// Return false to indicate sound is not audible
	virtual bool GetSoundSpatialization(SpatializationInfo& info) = 0;

	virtual bool IsBlurred() = 0;
};

#endif // SDK_ICLIENTENTITY_HPP
