#ifndef SDK_IENTITYENUMERATOR_HPP
#define SDK_IENTITYENUMERATOR_HPP

class IHandleEntity;

class IEntityEnumerator
{
public:
	// This gets called with each handle
	virtual bool EnumEntity(IHandleEntity* handleEntity) = 0;
};

#endif // SDK_IENTITYENUMERATOR_HPP
