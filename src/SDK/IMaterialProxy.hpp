#ifndef SDK_IMATERIALPROXY_HPP
#define SDK_IMATERIALPROXY_HPP

#include "IMaterial.hpp"
#include "KeyValues.hpp"

class IMaterialProxy : non_constructible
{
public:
	virtual bool Init(IMaterial* material, KeyValues* keyValues) = 0;
	virtual void OnBind(void*) = 0;
	virtual void Release() = 0;
	virtual IMaterial* GetMaterial() = 0;

protected:
	// no one should call this directly
	virtual ~IMaterialProxy()
	{}
};

#endif // SDK_IMATERIALPROXY_HPP
