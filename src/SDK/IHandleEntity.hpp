#ifndef SDK_IHANDLEENTITY_HPP
#define SDK_IHANDLEENTITY_HPP

class CBaseHandle;

class IHandleEntity
{
public:
	virtual ~IHandleEntity() = 0;
	virtual void SetRefEHandle(const CBaseHandle& handle) = 0;
	virtual const CBaseHandle& GetRefEHandle() const = 0;
};

#endif // SDK_IHANDLEENTITY_HPP
