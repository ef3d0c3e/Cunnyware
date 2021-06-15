#ifndef SDK_IMATERIALPROXYFACTORY_HPP
#define SDK_IMATERIALPROXYFACTORY_HPP

#include "IAppSystem.hpp"
#include "IMaterialProxy.hpp"

class IMaterialProxyFactory : non_constructible
{
public:
	virtual IMaterialProxy* CreateProxy(const char* proxyName) = 0;
	virtual void DeleteProxy(IMaterialProxy* proxy) = 0;
	virtual CreateInterfaceFn GetFactory() = 0;
};

#endif // SDK_IMATERIALPROXYFACTORY_HPP
